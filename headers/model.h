#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <mesh.h>
#include <shader_s.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

class Model
{
	public:
		Model(char *path)
		{
			loadModel(path);
		}
		void Draw(Shader &shader)
		{
			for(unsigned int i = 0; i < meshes.size(); i++)
				meshes[i].Draw(shader);
		}

	private:
		// model data
		vector<Mesh> meshes;
		string directory;

		void loadModel(string const &path) 
		{
			Assimp::Importer importer;
			const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate);

			if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
				cout << "ASSIMP ERROR: " << importer.GetErrorString() << endl;
				return;
			}
			directory = path.substr(0, path.find_last_of('/'));

			processNode(scene->mRootNode, scene);
		}
		
		void processNode(aiNode *node, const aiScene *scene)
		{
			// process all the node's meshes.
			for(unsigned int i = 0; i < node->mNumMeshes; i++) 
			{
				aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
				meshes.push_back(processMesh(mesh, scene));
			}
			// then recursively process the node's children.
			for(unsigned int i = 0; i < node->mNumChildren; i++) 
			{
				processNode(node->mChildren[i], scene);
			}
		}
		
		Mesh processMesh(aiMesh *mesh, const aiScene *scene)
		{
			vector<Vertex> vertices;
			vector<unsigned int> indices;
			vector<Texture> textures;
			
			// retrieving vertices.
			for(unsigned int i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				glm::vec3 tmp_vec;
				
				tmp_vec.x = mesh->mVertices[i].x;
				tmp_vec.y = mesh->mVertices[i].y;
				tmp_vec.z = mesh->mVertices[i].z;
				vertex.Position = tmp_vec;

				tmp_vec.x = mesh->mNormals[i].x;
				tmp_vec.y = mesh->mNormals[i].y;
				tmp_vec.z = mesh->mNormals[i].z;
				vertex.Normal = tmp_vec;

				if(mesh->mTextureCoords[0]) { // mesh contains texture coordinates? Check for testing. 
					glm::vec2 tmp_vec2;
					//always uses the first set texture.
					tmp_vec2.x = mesh->mTextureCoords[0][i].x;
					tmp_vec2.y = mesh->mTextureCoords[0][i].y;
					vertex.TexCoords = tmp_vec2;
					/*

					// tangent
					tmp_vec = mesh->mTangents[i].x;
					tmp_vec = mesh->mTangents[i].y;
					tmp_vec = mesh->mTangents[i].z;
					vertex.Tangent = tmp_vec;
					
					// bitangent
					tmp_vec = mesh->mBitangents[i].x;
					tmp_vec = mesh->mBitangents[i].y;
					tmp_vec = mesh->mBitangents[i].z;
					vertex.Bitangent = tmp_vec;
					*/
				} else {
					vertex.TexCoords = glm::vec2(0.0f, 0.0f);
				}
				vertices.push_back(vertex);
			}

			// retrieving indices
			for(unsigned int i = 0; i < mesh->mNumFaces; i++)
			{
				aiFace face = mesh->mFaces[i];
				for(unsigned int j = 0; j < face.mNumIndices; j++)
				{
					indices.push_back(face.mIndices[j]);
				}
			}

			// retrieving textures
			if(mesh->mMaterialIndex >= 0)
			{
				aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
				vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
				textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

				vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
				textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			}
		}

		vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName)
		{
			vector<Texture> textures;
			for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
			{
				aiString str;
				mat->GetTexture(type, i, &str);
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
			}
		}
};

#endif
