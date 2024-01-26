#include "PathTracing.h"

#include "App/Model.h"


PathTracing::PathTracing()
	:Renderer("Path Tracing") 
{
	Model m_Model("./assets/models/cornell_box/cbox_greenwall.obj");
	if(m_Model.modelTriangles.size())
	std::cout << "Triangle size is " << m_Model.modelTriangles.size() << std::endl;

}

void PathTracing::Render(Camera& camera)
{

}
