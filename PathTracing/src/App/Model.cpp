#include "Model.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "Core/tiny_obj_loader.h"

#include <iostream>


Model::Model(const std::string& filename)
{
    LoadFromFile(filename);
}

bool Model::LoadFromFile(const std::string& filename)
{
    Name = filename;

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filename.c_str(), 0, true);

    if (!ret)
    {
        printf("Unable to load model\n");
        return false;
    }

    for (size_t s = 0; s < shapes.size(); s++)
    {
        size_t index_offset = 0;

        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) //every face 
        {
            std::vector<glm::vec3> position, normal;
            for (size_t v = 0; v < shapes[s].mesh.num_face_vertices[f]; v++)
            {
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
                tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
                tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
                position.push_back(glm::vec3(vx, vy, vz));

                if (!attrib.normals.empty())
                {
                    tinyobj::real_t nx = attrib.vertices[3 * idx.normal_index + 0];
                    tinyobj::real_t ny = attrib.vertices[3 * idx.normal_index + 1];
                    tinyobj::real_t nz = attrib.vertices[3 * idx.normal_index + 2];
                    normal.push_back(glm::vec3(nx, ny, nz));
                }
                else
                {
                    normal.push_back(glm::vec3(-1, -1, -1));
                }
                tinyobj::real_t tx, ty;

                if (!attrib.texcoords.empty())
                {
                    tx = attrib.texcoords[2 * idx.texcoord_index + 0];
                    ty = 1.0 - attrib.texcoords[2 * idx.texcoord_index + 1];
                }
                else
                {
                    if (v == 0)
                        tx = ty = 0;
                    else if (v == 1)
                        tx = 0, ty = 1;
                    else
                        tx = ty = 1;
                }
                //coord = {tx, ty};
            }
            Triangles.emplace_back(Triangle(position, normal));
            index_offset += shapes[s].mesh.num_face_vertices[f];
        }
        //std::cout << "Triangle size is " << Triangles.size() << std::endl;
    }
    //std::cout << "Load model " << filename << " successfully!\n";
    //std::cout <<"There are " << Triangles.size() <<" Triangles.\n";
    //std::cout << "address is " << &Triangles[0] << "\n";

    return true;
}
