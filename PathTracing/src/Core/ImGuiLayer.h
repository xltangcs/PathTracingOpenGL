#pragma once

#include <string>

class ImGuiLayer
{
public:
	ImGuiLayer() = default;
	virtual~ImGuiLayer() = default;

	virtual void ShowUI(float ts) {};
	virtual void Render(float ts) {};
	virtual void OnUpdate(float ts) {};

private:
};