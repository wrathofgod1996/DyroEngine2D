#include "SceneGraph\Objects\Scenes\Scene.h"

#include "SceneGraph\Objects\GameObjects\GameObject.h"
#include "SceneGraph\Manager\GameObjectManager.h"

#include "Core\System\Objects\Input.h"
#include "Core\System\Manager\SystemManager.h"

#include "Rendering/Objects/Visualization/Visualization.h"
#include "Rendering\Manager\VisualizationManager.h"

#include "Core\Defines\deletemacros.h"

#include "Core/Helpers/Patterns/Singleton.h"

#include "Physics\Collision\ContactListener.h"
#include "Physics\Collision\ContactFilter.h"

#include "Core\Data\Manager\SettingsManager.h"
#include "Core\Data\Manager\LayerManager.h"

#include "Core\Data\Objects\Settings\PhysicsSettings.h"
#include "Core\Data\Objects\Layer.h"

#include "Core/Types/SettingsType.h"

#include "Rendering\Renderer.h"
#include "Rendering\DebugRenderer.h"

#include <Box2D\Box2D.h>

#include <algorithm>

Scene::Scene(const std::tstring& name)
	:Object(name)
	, phyx_world(nullptr)
	, debug_rendering(false)
	, renderer(nullptr)
	, debug_renderer(nullptr)
	, contact_filter(nullptr)
	, contact_listener(nullptr)
	, game_object_manager(nullptr)
{
	OBJECT_INIT(_T("Scene"));

	this->game_object_manager = new GameObjectManager();
	addManager(this->game_object_manager);
}
Scene::~Scene()
{
}

bool Scene::initialize()
{
	setupPyhx();

	this->renderer = new Renderer();

	if (!this->game_object_manager->initialize())
		return false;

	return true;
}
bool Scene::postInitialize()
{
	Input* input = dynamic_cast<Input*>(Singleton<SystemManager>::getInstance().getSystem(SystemType::INPUT_SYSTEM));
	this->setupInput(input);

	this->game_object_manager->setupInput(input);
	if (!this->game_object_manager->postInitialize())
		return false;

	return true;
}
void Scene::update()
{
	//Retrieve the physics settings
	PhysicsSettings* physicsSettings = dynamic_cast<PhysicsSettings*>(Singleton<SettingsManager>::getInstance().getSettings(SettingsType::PHYSICS_SETTINGS));
	if (physicsSettings == nullptr)
		return;

	//Adjust gravity when needed
	if (Vector2D::toBox2DVec(physicsSettings->getGravity()) != this->phyx_world->GetGravity())
		this->phyx_world = new b2World(Vector2D::toBox2DVec(physicsSettings->getGravity()));

	//Update the physics world
	this->phyx_world->Step(physicsSettings->getPhyxTimeStep(), physicsSettings->getVelocityInterpolation(), physicsSettings->getPositionInterpolation());

	//Update the contact listener so we can handle collision check when objects when still colliding
	this->contact_listener->update();
	//Update all the objects that are added to the scene
	this->game_object_manager->update();

	//Renderer the scene after all calculations are done
	triggerRender();
}
bool Scene::shutdown()
{
	bool result = true;

	if (!this->game_object_manager->shutdown())
		result = false;

	SafeDelete(this->game_object_manager);

	SafeDelete(this->debug_renderer);
	SafeDelete(this->contact_filter);
	SafeDelete(this->contact_listener);
	SafeDelete(this->phyx_world);

	SafeDelete(this->renderer);

	return result;
}

void Scene::setupInput(Input* input)
{
}

void Scene::activate()
{
	Object::activate();
}
void Scene::deactive()
{
	Object::deactive();
}

void Scene::enableDebugRendering()
{
	this->debug_rendering = true;
}
void Scene::disableDebugRendering()
{
	this->debug_rendering = false;
}

b2World* Scene::getPhyxWorld()
{
	return this->phyx_world;
}

void Scene::addGameObject(GameObject* object)
{
	object->setScene(this);
	object->setLayer(getManager<LayerManager>()->getLayer(_T("Default")));
	this->game_object_manager->addGameObject(object);
}
void Scene::removeGameObject(GameObject* object)
{
	this->game_object_manager->removeGameObject(object);
}
void Scene::removeGameObject(unsigned int id)
{
	this->game_object_manager->removeGameObject(id);
}

void Scene::getGameObjects(std::vector<GameObject*>& objects) const
{
	this->game_object_manager->getGameObjects(objects);
}
void Scene::getVisualizations(std::vector<Visualization*>& visualizations) const
{
	this->game_object_manager->getVisualizations(visualizations);
}

void Scene::addManager(AbstractManager* manager)
{
	std::vector<AbstractManager*>::const_iterator it = std::find(this->vec_managers.begin(), this->vec_managers.end(), manager);
	if (it != this->vec_managers.end())
		return;

	this->vec_managers.push_back(manager);
}

void Scene::setupPyhx()
{
	this->debug_renderer = new DebugRenderer();
	this->contact_filter = new ContactFilter();
	this->contact_listener = new ContactListener();

	PhysicsSettings* physicsSettings = dynamic_cast<PhysicsSettings*>(Singleton<SettingsManager>::getInstance().getSettings(SettingsType::PHYSICS_SETTINGS));
	if (physicsSettings == nullptr)
		return;

	this->phyx_world = new b2World(Vector2D::toBox2DVec(physicsSettings->getGravity()));

	debug_renderer->SetFlags(b2Draw::e_shapeBit);
	debug_renderer->AppendFlags(b2Draw::e_centerOfMassBit);
	debug_renderer->AppendFlags(b2Draw::e_jointBit);
	debug_renderer->AppendFlags(b2Draw::e_pairBit);

	this->phyx_world->SetDebugDraw(debug_renderer);
	this->phyx_world->SetContactFilter(this->contact_filter);
	this->phyx_world->SetContactListener(this->contact_listener);
}
void Scene::triggerRender()
{
	//Get all visualizations
	std::map<unsigned int, Visualization*> visualizations = this->game_object_manager->getVisualizations();

	//Generate the render items
	std::vector<RenderItem*> items;
	for (const std::pair<unsigned int, Visualization*>& pair : visualizations)
		pair.second->getRenderItems(items);

	//Sort the render items
	std::sort(items.begin(), items.end(),
		[](RenderItem* i1, RenderItem* i2) -> bool
	{
		return i1->getLayer()->getID() < i2->getLayer()->getID();
	});

	//Render visualizations
	this->renderer->render(items);

	//Clear the render item list
	items.clear();
}