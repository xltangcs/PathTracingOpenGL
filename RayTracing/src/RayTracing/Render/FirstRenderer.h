#pragma once

#include "RayTracing/Renderer.h"

class FirstRenderer : public Renderer
{
public:
	FirstRenderer();
	virtual void Render(Camera& camera) override;
};