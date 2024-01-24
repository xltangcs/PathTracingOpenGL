#pragma once

#include "App/Renderer.h"

class PBR : public Renderer
{
public:
	PBR();
	virtual void Render(Camera& camera) override;
};