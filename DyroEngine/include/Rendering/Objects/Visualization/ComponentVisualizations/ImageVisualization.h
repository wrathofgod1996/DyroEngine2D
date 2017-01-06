#ifndef _IMAGEVISUALIZATION_H
#define _IMAGEVISUALIZATION_H

#include "Rendering/Objects/Visualization/ComponentVisualizations/ComponentVisualization.h"

class Texture;

class ImageVisualization : public ComponentVisualization
{
public:
	ImageVisualization(Component* object, const std::tstring& name = _T(""));
	virtual ~ImageVisualization();

protected:
	virtual void generateRenderItems(std::vector<RenderItem*>& items);

private:
	Texture* texture;
};

#endif