#ifndef _TRIANGLESHAPE_H
#define _TRIANGLESHAPE_H

#include "SceneGraph\Object\Objects\Shapes\PolygonShape.h"

class TriangleShape : public PolygonShape
{
public:
	TriangleShape(float xScale = 1.0f, float yScale = 1.0f, float lineWidth = 0.5f);
	virtual ~TriangleShape();
};

#endif