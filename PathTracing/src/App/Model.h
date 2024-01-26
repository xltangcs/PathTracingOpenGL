#pragma once

#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "App/Triangle.h"

class Model
{
public:
    Model(const std::string& filename);
    bool LoadFromFile(const std::string& filename);

    std::vector<Triangle> modelTriangles;
    std::string m_Name;
};