#pragma once

#include "App/Renderer.h"

class FirstRenderer : public Renderer
{
public:
	FirstRenderer();
	virtual void Render(Camera& camera) override;
};