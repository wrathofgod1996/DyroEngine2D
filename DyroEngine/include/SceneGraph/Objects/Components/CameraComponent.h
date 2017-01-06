#ifndef _CAMERACOMPONENT_H
#define _CAMERACOMPONENT_H

#include "SceneGraph\Objects\Components\Component.h"

#include "Math/Objects/Matrix2D.h"

class TransformComponent;

class CameraComponent : public Component
{
	/**
	\note Must be present in every subclass definition.
	*/
	OBJECT_STATICS("CameraComponent")

public:
	CameraComponent(const std::tstring& name = _T(""));
	virtual ~CameraComponent();

	virtual bool initialize();
	virtual void update();
	virtual bool shutdown();

	const Matrix2D getViewMatrix() const;

private:
	TransformComponent* transform;
};

#endif