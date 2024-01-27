#pragma once

#include "App/Renderer.h"

class RayTracing : public Renderer
{
public:
	RayTracing();
	virtual void Render(Camera& camera) override;
};