#pragma once

#include "RayTracing/Renderer.h"

class PBR : public Renderer
{
public:
	PBR();
	virtual void Render(Camera& camera) override;
};