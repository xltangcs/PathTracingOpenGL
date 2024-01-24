#pragma once

#include "App/Renderer.h"

class ContainerRenderer : public Renderer
{
public:
	ContainerRenderer();
	virtual void Render(Camera& camera) override;
};