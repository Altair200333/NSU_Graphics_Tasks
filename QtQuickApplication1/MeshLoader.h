#pragma once
#include <iostream>
#include <random>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Material.h"
#include "Mesh.h"

class MeshLoader final
{
    std::string directory;
public:
    struct LoadedModel
    {
        Mesh mesh;
        Material material;
    };
    std::vector<LoadedModel> models;

    std::vector<LoadedModel> loadModel(const std::string& path)
    {
        Assimp::Importer import;
        const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
            return {};
        }
        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
        return models;
    }
    void processNode(aiNode* node, const aiScene* scene)
    {
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene. 
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            models.push_back(processMesh(mesh, scene));
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            processNode(node->mChildren[i], scene);
        }

    }
    LoadedModel processMesh(aiMesh* mesh, const aiScene* scene)
    {
        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // walk through each of the mesh's vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            QVector3D vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.setX(mesh->mVertices[i].x);
            vector.setY(mesh->mVertices[i].y);
            vector.setZ(mesh->mVertices[i].z);
            vertex.vertex = { vector.x(), vector.y(), vector.z() };
            // normals
            if (mesh->HasNormals())
            {
                vector.setX(mesh->mNormals[i].x);
                vector.setY(mesh->mNormals[i].y);
                vector.setZ(mesh->mNormals[i].z);
                vertex.normal = { vector.x(), vector.y(), vector.z() };
            }
            if(mesh->HasVertexColors(0))
            {
                vertex.color = { mesh->mColors[0][i].r, mesh->mColors[0][i].g, mesh->mColors[0][i].b };
            }
            else
            {
                std::uniform_real_distribution<float> dist(0.0, 1.0);
                std::default_random_engine  rd(mesh->mVertices[i].x*10+ mesh->mVertices[i].y * 10 + mesh->mVertices[i].z * 10);
                vertex.color = {dist(rd), dist(rd), dist(rd) };
            }
            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for (unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for (unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
        aiColor3D color(0.f, 0.f, 0.f);
        material->Get(AI_MATKEY_COLOR_DIFFUSE, color);
        QColor albedo = { static_cast<int>(color.r), static_cast<int>(color.g), static_cast<int>(color.b)};
        
        return { Mesh(vertices, indices), Material(albedo) };
    }
   
};