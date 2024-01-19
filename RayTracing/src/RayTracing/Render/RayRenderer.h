#pragma once

#include "RayTracing/Renderer.h"

class RayRenderer : public Renderer
{
public:
	RayRenderer();
	virtual void Render(Camera& camera) override;
};