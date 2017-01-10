#ifndef _SHAPEVISUALIZATION_H
#define _SHAPEVISUALIZATION_H

#include "Rendering/Objects/Visualization/ComponentVisualizations/ConcreteComponentVisualization.h"

class ShapeComponent;

class Component;
class Shape;

class ShapeVisualization : public ConcreteComponentVisualization<ShapeComponent>
{
public:
	ShapeVisualization(Component* object, const std::tstring& name = _T("ShapeVisualization"));
	virtual ~ShapeVisualization();

protected:

	virtual void generateRenderItems(std::vector<RenderItem*>& items) = 0;

	void setShape(Shape* shape);
	Shape* getShape() const;

private:
	Shape* shape;
};

#endif