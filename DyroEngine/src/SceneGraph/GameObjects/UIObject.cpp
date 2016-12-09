#include "SceneGraph/GameObjects/UIObject.h"

#include "SceneGraph/Component/TransformComponent.h"

UIObject::UIObject(const std::tstring& name)
	:GameObject(name)
	,transform(new TransformComponent())
{}
UIObject::~UIObject()
{}

bool UIObject::initialize()
{
	return GameObject::initialize();
}
void UIObject::upddate()
{
	GameObject::update();
}
bool UIObject::shutdown()
{
	return GameObject::shutdown();
}

TransformComponent* UIObject::getTransform() const
{
	return this->transform;
}