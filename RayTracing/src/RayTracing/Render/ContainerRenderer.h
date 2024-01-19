#pragma once

#include "RayTracing/Renderer.h"

class ContainerRenderer : public Renderer
{
public:
	ContainerRenderer();
	virtual void Render(Camera& camera) override;
};