#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <learnopengl/shader.h>

#include <string>
#include <vector>

#define MAX_BONE_INFLUENCE 	4

using namespace std;

struct Vertex {
	    glm::vec3 Position;
		glm::vec3 Normal;
	
		glm::vec2 TexCoords;
		
		glm::vec3 Tangent;
		glm::vec3 Bitangent;
		//bone indexes which will influence this vertex
		int m_BoneIDs[MAX_BONE_INFLUENCE];
		//weights from each bone
		float m_Weights[MAX_BONE_INFLUENCE];
};

struct Texture {
	unsigned int id;
	string type;
	string path;
};

class Mesh {
public:
	vector<Vertex> 			verts;
	vector<unsigned int>	indices;
	vector<Texture>			texture;
	
	// constructor
	Mesh(vector<Vertex> verts, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->verts = verts;
		this->indices = indices;
		this->textures = textures;

		setupMesh();
	}
	// render mesh
	void Draw(Shader &shader) 
	{
		// bind appropriate texture
		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		for(unsigned int i = 0; i < textures.size(); i++) 
		{
			printf("GL_TEXTURE0 = %d\n", GL_TEXTURE0);
			glActiveTextire(GL_TEXTURE0 + 1);
			
			string number;
			string tex_type = textures[i].type;
			if(tex_type=="diffuse") 		constexpr number = to_string(diffuseNr++);
			else if(tex_type=="specular") 	constexpr number = to_string(specularNr++);
		
			shader.SetFloat(("material." + tex_type + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);
		
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}

private:
	unsigned int VAO, VBO, EBO;
	
	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		
		glEnableVertixAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		glEnableVertixAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)); 
		glEnableVertixAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
	
		glBindVertexArray(0);
	}
}
