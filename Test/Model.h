/*#ifndef MODEL_H
#define MODEL_H


#include <GL\GL.h>
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

class Model
{
public:
    
    vector<Mesh> meshes;
    string directory;
        
    Model(string const& path) 
    {}
       
    void Draw()
    {}

private:
    
    void loadModel(string const& path)
    {}
        
    void processNode(aiNode* node, const aiScene* scene)
    {}

    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        return Mesh(vertices, indices);
    }        
};

#endif
*/



#ifndef MODEL_H
#define MODEL_H

// ���������� GLEW (������ GL/GL.h)
#include <GL/glew.h> 

// ���������� ���������� �� PDF (���. 8) [cite: 254-267]
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class Model
{
public:
    // ������ ������ (���. 6)
    vector<Mesh> meshes; // [cite: 204]
    string directory; // [cite: 205]

    // �����������, ��������� ���� � ����� (���. 6) [cite: 206]
    Model(string const& path)
    {
        loadModel(path); // [cite: 220]
    }

    // ������ ������ (��� �� �����) (���. 6) [cite: 208]
    void Draw(Shader& shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:
    // ��������� ������ � ������� ASSIMP (���. 6) [cite: 211]
    void loadModel(string const& path)
    {
        Assimp::Importer importer;
        // PDF �� ���������� ���� ���, �� �� ����� ��� ��������
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
        {
            cout << "ERROR::ASSIMP::" << importer.GetErrorString() << endl;
            return;
        }
        directory = path.substr(0, path.find_last_of('/')); // [cite: 223]

        // ������������ �������� ���� ����������
        processNode(scene->mRootNode, scene); // [cite: 224]
    }

    // ����������� ��������� ����� (���. 6, 7) [cite: 212]
    void processNode(aiNode* node, const aiScene* scene)
    {
        // ������������ ��� ����� �������� ���� (���. 7)
        for (unsigned int i = 0; i < node->mNumMeshes; i++) // [cite: 226]
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]]; // [cite: 228]
            meshes.push_back(processMesh(mesh, scene)); // [cite: 228]
        }
        // ������������ �������� ����
        for (unsigned int i = 0; i < node->mNumChildren; i++) // [cite: 230]
        {
            processNode(node->mChildren[i], scene); // [cite: 232]
        }
    }

    // ��������� ����� (Mesh) (���. 6, 7) [cite: 213]
    Mesh processMesh(aiMesh* mesh, const aiScene* scene)
    {
        vector<Vertex> vertices; // [cite: 236]
        vector<unsigned int> indices; // [cite: 236]

        // 1. ������������ ������� (���. 7)
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) // [cite: 238]
        {
            Vertex vertex; // [cite: 240]
            glm::vec3 vector; // [cite: 241]

            // �������
            vector.x = mesh->mVertices[i].x; // [cite: 242]
            vector.y = mesh->mVertices[i].y; // [cite: 243]
            vector.z = mesh->mVertices[i].z; // [cite: 244]
            vertex.Position = vector; // [cite: 245]

            // ������� (PDF ������������� ��� � struct Vertex, �� �� ����� ���)
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            else
            {
                vertex.Normal = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            // PDF �� ��������� ��������, ������� �� ���� ����������

            vertices.push_back(vertex);
        }

        // 2. ������������ ������� (���. 8)
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) // [cite: 247]
        {
            aiFace face = mesh->mFaces[i]; // [cite: 249]
            for (unsigned int j = 0; j < face.mNumIndices; j++) // [cite: 250]
                indices.push_back(face.mIndices[j]); // [cite: 250]
        }

        // 3. ���������� ������� ����� (���. 8)
        return Mesh(vertices, indices); // [cite: 252]
    }
};
#endif

