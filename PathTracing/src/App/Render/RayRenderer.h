#pragma once

#include "App/Renderer.h"

class RayRenderer : public Renderer
{
public:
	RayRenderer();
	virtual void Render(Camera& camera) override;
};