#pragma once

#include "App/Renderer.h"
#include "App/Triangle.h"
#include "App/Material.h"



class PathTracing : public Renderer
{
public:
	PathTracing();
	virtual void Render(Camera& camera) override;
private:
    std::vector<Triangle> m_Triangles;
};