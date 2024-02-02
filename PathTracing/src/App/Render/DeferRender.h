#pragma once

#include "App/Renderer.h"
#include "Core/Geometry/BoxGeometry.h"
#include "Core/Geometry/PlaneGeometry.h"


class DeferRender : public Renderer
{
public:
	DeferRender();
	virtual void Render(Camera& camera) override;

private:
	Shader geometryShader, sceneShader;

	PlaneGeometry groundGeometry;            // µÿ√Ê

	int SCREEN_WIDTH = 1280;
	int SCREEN_HEIGHT = 720;

	unsigned int gBuffer;
	unsigned int gPosition;
};