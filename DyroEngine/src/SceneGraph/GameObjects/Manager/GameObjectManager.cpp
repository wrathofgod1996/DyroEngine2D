#include "SceneGraph\GameObjects\Manager\GameObjectManager.h"
#include "SceneGraph\GameObjects\GameObject.h"
#include "SceneGraph\Component\Component.h"

#include "Core\Rendering\Visualization\Manager\VisualizationFactory.h"
#include "Core\Rendering\Visualization\Manager\VisualizationManager.h"

#include "Interfaces\IRenderable.h"

#include "Helpers\Singleton.h"

#include "Defines\assert.h"

#include <algorithm>

GameObjectManager::GameObjectManager()
{}
GameObjectManager::~GameObjectManager()
{}

bool GameObjectManager::initialize()
{
	this->visualization_manager = new VisualizationManager();
	this->visualization_manager->initialize();

	std::vector<GameObject*> objects;
	getObjects(objects);

	for (GameObject* obj : objects)
	{
		if (obj->getInitialized())
			continue;

		if (!obj->initialize())
			return false;
	}

	return true;
}
bool GameObjectManager::shutdown()
{
	std::vector<GameObject*> objects;
	getObjects(objects);

	for (GameObject* obj : objects)
	{
		if (!obj->shutdown())
			return false;
		SafeDelete(obj);
	}

	return true;
}

void GameObjectManager::addGameObject(GameObject* object)
{
	assert(object == nullptr);

	addObject(object->getObjectID(), object);
}

void GameObjectManager::removeGameObject(unsigned int id)
{
	removeObject(id);
}
void GameObjectManager::removeGameObject(GameObject* object)
{
	removeObject(object);
}

GameObject* GameObjectManager::getGameObject(const std::tstring& name)
{
	std::vector<GameObject*> game_objects;
	getObjects(game_objects);

	std::vector<GameObject*>::const_iterator it = std::find_if(game_objects.begin(),game_objects.end(),
		[name](GameObject* object) -> bool
	{
		return name == object->getName();
	});

	return getObject((*it)->getObjectID());
}
GameObject* GameObjectManager::getGameObject(unsigned int id)
{
	return getObject(id);
}

void GameObjectManager::getGameObjects(std::vector<GameObject*>& objects)
{
	getObjects(objects);
}
void GameObjectManager::getGameObjects(std::map<unsigned int, GameObject*>& objects)
{
	getObjects(objects);
}

bool GameObjectManager::addObject(unsigned int id, GameObject* object)
{
	if (!Manager<GameObject>::addObject(id, object))
		return false;

	VisualizationFactory factory = Singleton<VisualizationFactory>::getInstance();

	std::vector<Component*> components = object->getComponents();
	for (Component* component : components)
	{
		IRenderable* renderable = dynamic_cast<IRenderable*>(component);
		if (renderable == nullptr)
			continue;

		this->visualization_manager->addVisualization(id, factory.createVisualization(object));
	}
}

bool GameObjectManager::removeObject(unsigned int id)
{
	if (!Manager<GameObject>::removeObject(id))
		return false;

	this->visualization_manager->removeVisualization(id);
}
bool GameObjectManager::removeObject(GameObject* object)
{
	if (!Manager<GameObject>::removeObject(object))
		return false;

	this->visualization_manager->removeVisualization(object->getObjectID());
}