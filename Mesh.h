#pragma once

#include <iostream>
#include <vector>

#include "Shader.h"
#include "Vertex.h"
#include "Texture.h"
#include "Material.h"

#include <vector>
#include <gtc/matrix_transform.hpp>

class Face {
public:
	unsigned int v1, v2, v3, v4;
	unsigned int t1, t2, t3, t4;
	unsigned int n1, n2, n3, n4;
	int elementCount;
	Face(unsigned int v1 = -1, unsigned int t1 = -1, unsigned int n1 = -1, unsigned int v2 = -1, unsigned int t2 = -1, unsigned int n2 = -1, unsigned int v3 = -1, unsigned int t3 = -1, unsigned int n3 = -1)
	{
		this->v1 = v1;
		this->t1 = t1;
		this->n1 = n1;
		this->v2 = v2;
		this->t2 = t2;
		this->n2 = n2;
		this->v3 = v3;
		this->t3 = t3;
		this->n3 = n3;
	}
};


class Mesh
{
private: 
	std::string name;
	Material material;
	Vertex* vertexArray;
	unsigned nrOfVertices;
	GLuint* indexArray;
	unsigned nrOfIndices;
	GLuint VAO;
	GLuint VBO;
	GLuint EBO;
	char dataCount = 3;


	glm::vec3 position;
	glm::vec3 origin;
	glm::vec3 rotation;
	glm::vec3 scale;

	glm::mat4 ModelMatrix;

	void initVAO()
	{
		//Create VAO
		glGenVertexArrays(1, &this->VAO);
		glBindVertexArray(this->VAO);

		//GEN VBO AND BIND AND SEND DATA
		glGenBuffers(1, &this->VBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->nrOfVertices * sizeof(Vertex), this->vertexArray, GL_STATIC_DRAW);

		//GEN EBO AND BIND AND SEND DATA
		if (this->nrOfIndices > 0)
		{
			glGenBuffers(1, &this->EBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->nrOfIndices * sizeof(GLuint), this->indexArray, GL_STATIC_DRAW);
		}

		//SET VERTEXATTRIBPOINTERS AND ENABLE (INPUT ASSEMBLY)
		//Position
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);
		//Normal
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));
		glEnableVertexAttribArray(1);
		//Texcoord
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texcoord));
		glEnableVertexAttribArray(2);


		//BIND VAO 0
		//glBindVertexArray(0);
	}

	void updateUniforms(Shader* shader)
	{
		if (glGetUniformLocation(shader->ID, "model") != -1)
		{
			shader->setMat4("model", this->ModelMatrix);
		}
	}

	void updateModelMatrix()
	{
		this->ModelMatrix = glm::mat4(1.f);
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->origin);
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.x), glm::vec3(1.f, 0.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.y), glm::vec3(0.f, 1.f, 0.f));
		this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(this->rotation.z), glm::vec3(0.f, 0.f, 1.f));
		this->ModelMatrix = glm::translate(this->ModelMatrix, this->position - this->origin);
		this->ModelMatrix = glm::scale(this->ModelMatrix, this->scale);
	}


public:
	Mesh()
	{
		this->name = "";
		this->position = { 0.0f, 0.0f, 0.0f };
		this->origin = { 0.0f, 0.0f, 0.0f };
		this->rotation = { 0.0f, 0.0f, 0.0f };
		this->scale = { 0.0f, 0.0f, 0.0f };

		this->nrOfVertices = nrOfVertices;
		this->nrOfIndices = nrOfIndices;

		this->vertexArray = nullptr;
		this->indexArray = nullptr;
		this->material = Material();

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(
		std::string name,
		std::vector<Vertex> vertexArray,
		const unsigned& nrOfVertices,
		std::vector<Face> faceArray,
		const unsigned& nrOfIndices,
		glm::vec3 position = glm::vec3(0.f),
		glm::vec3 origin = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.f),
		Material m = Material()
	)
	{
		this->name = name;
		this->position = position;
		this->origin = origin;
		this->rotation = rotation;
		this->scale = scale;

		this->nrOfVertices = nrOfVertices;
		this->nrOfIndices = nrOfIndices;

		this->vertexArray = new Vertex[this->nrOfVertices];
		for (size_t i = 0; i < nrOfVertices; i++)
		{
			this->vertexArray[i] = vertexArray[i];
		}

		//this->indexArray = new GLuint[this->nrOfIndices];
		//int k = 0;
		//for (size_t i = 0; i < faceArray.size(); i++)
		//{
		//		this->indexArray[k] = faceArray[i].v1;
		//		k++;
		//		this->indexArray[k] = faceArray[i].v2;
		//		k++;
		//		this->indexArray[k] = faceArray[i].v3;
		//		k++;	
		//}
		this->material = m;
		this->initVAO();
		this->updateModelMatrix();

	}

	Mesh(
		std::string name,
		std::vector<Vertex> vertexArray,
		const unsigned& nrOfVertices,
		GLuint* indexArray = nullptr,
		const unsigned& nrOfIndices = 0,
		glm::vec3 position = glm::vec3(0.f),
		glm::vec3 origin = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.f),
		Material m = Material()
	)
	{
		this->name = name;
		this->position = position;
		this->origin = origin;
		this->rotation = rotation;
		this->scale = scale;

		this->nrOfVertices = nrOfVertices;
		this->nrOfIndices = nrOfIndices;

		this->vertexArray = new Vertex[this->nrOfVertices];
		for (size_t i = 0; i < nrOfVertices; i++)
		{
			this->vertexArray[i] = vertexArray[i];
		}

		this->indexArray = new GLuint[this->nrOfIndices];
		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->indexArray[i] = indexArray[i];
		}
		this->material = m;

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(
		std::string name,
		std::vector<Vertex> vertexArray,
		const unsigned& nrOfVertices,
		std::vector<unsigned int> indexArray,
		const unsigned& nrOfIndices = 0,
		glm::vec3 position = glm::vec3(0.f),
		glm::vec3 origin = glm::vec3(0.f),
		glm::vec3 rotation = glm::vec3(0.f),
		glm::vec3 scale = glm::vec3(1.f),
		Material m = Material()
	)
	{
		this->name = name;
		this->position = position;
		this->origin = origin;
		this->rotation = rotation;
		this->scale = scale;

		this->nrOfVertices = nrOfVertices;
		this->nrOfIndices = nrOfIndices;

		this->vertexArray = new Vertex[this->nrOfVertices];
		for (size_t i = 0; i < nrOfVertices; i++)
		{
			this->vertexArray[i] = vertexArray[i];
		}

		this->indexArray = new GLuint[this->nrOfIndices];
		for (size_t i = 0; i < nrOfIndices; i++)
		{
			this->indexArray[i] = indexArray[i];
		}
		this->material = m;

		this->initVAO();
		this->updateModelMatrix();
	}

	Mesh(const Mesh& obj)
	{
		this->material = obj.material;
		this->position = obj.position;
		this->origin = obj.origin;
		this->rotation = obj.rotation;
		this->scale = obj.scale;

		this->nrOfVertices = obj.nrOfVertices;
		this->nrOfIndices = obj.nrOfIndices;

		this->vertexArray = new Vertex[this->nrOfVertices];
		for (size_t i = 0; i < this->nrOfVertices; i++)
		{
			this->vertexArray[i] = obj.vertexArray[i];
		}

		this->indexArray = new GLuint[this->nrOfIndices];
		for (size_t i = 0; i < this->nrOfIndices; i++)
		{
			this->indexArray[i] = obj.indexArray[i];
		}

		this->initVAO();
		this->updateModelMatrix();
	}

	~Mesh()
	{
		glDeleteVertexArrays(1, &this->VAO);
		glDeleteBuffers(1, &this->VBO);

		if (this->nrOfIndices > 0)
		{
			glDeleteBuffers(1, &this->EBO);
		}

		delete[] this->vertexArray;
		if(this->nrOfIndices != 0)
		delete[] this->indexArray;
	}
	Material* getMaterial()
	{
		return &material;
	}


	void setDataCount(char val)
	{
		dataCount = val;
	}
	char getDataCount()
	{
		return dataCount;
	}

	void setPosition(const glm::vec3 position)
	{
		this->position = position;
	}

	glm::vec3 getPosition()
	{
		return this->position;
	}

	void setOrigin(const glm::vec3 origin)
	{
		this->origin = origin;
	}

	glm::vec3 getOrigin()
	{
		return this->origin;
	}

	void setRotation(const glm::vec3 rotation)
	{
		this->rotation = rotation;
	}
	
	glm::vec3 getRotation()
	{
		return this->rotation;
	}

	void setScale(const glm::vec3 setScale)
	{
		this->scale = setScale;
	}

	glm::vec3 getScale()
	{
		return this->scale;
	}




	void move(const glm::vec3 position)
	{
		this->position += position;
	}

	void rotate(const glm::vec3 rotation)
	{
		this->rotation += rotation;
	}

	void scaleUp(const glm::vec3 scale)
	{
		this->scale += scale;
	}

	void update()
	{

	}

	void bindTexture(Shader* shader, Texture tex, GLint texUnit)
	{
		shader->use();
		//Bind VAO
		glBindVertexArray(this->VAO);
		tex.bind(texUnit);
//		glBindVertexArray(0);
		shader->unuse();

	}

	void bindVAO()
	{
		glBindVertexArray(this->VAO);
	}

	void render(Shader* shader)
	{
		//shader->use();
		//Update uniforms
		this->updateModelMatrix();
		this->updateUniforms(shader);

		shader->use();

		material.sendToShader(*shader);
		//Bind VAO
		glBindVertexArray(this->VAO);


		//RENDER
		if (this->nrOfIndices == 0)
			glDrawArrays(GL_TRIANGLES, 0, this->nrOfVertices);
		else
			glDrawElements(GL_TRIANGLES, this->nrOfIndices, GL_UNSIGNED_INT, 0);

		//Cleanup
		glBindVertexArray(0);
		//glUseProgram(0);
		//glActiveTexture(0);
		//glBindTexture(GL_TEXTURE_2D, 0);
	}
};