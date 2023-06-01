#pragma once
#pragma once

#include "glad/glad.h"
#include<GLFW/glfw3.h>

#include<glm.hpp>

#include "Shader.h"

class Material
{
public:
	std::string name;
	GLint ambientTex;
	GLint diffuseTex;
	GLint specularTex;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

public:

	Material(std::string name = "",
		GLint ambientTex = -1,
		GLint diffuseTex = -1,
		GLint specularTex = -1,
		glm::vec3 ambient = { 0.5, 0.5, 0.5 },
		glm::vec3 diffuse = { 0.5, 0.5, 0.5 },
		glm::vec3 specular = { 0.2, 0.2, 0.2 },
		float shininess = 50
	)
	{
		this->name = name;
		this->ambientTex = ambientTex;
		this->diffuseTex = diffuseTex;
		this->specularTex = specularTex;
		this->ambient = ambient;
		this->diffuse = diffuse;
		this->specular = specular; 
		this->shininess = shininess;
	}
	~Material() {}
	
	bool isEmpty()
	{
		if (this->name == "")
			return true;
	}

	void setName(std::string val)
	{
		this->name = val;
	}

	void setAmbientTex(GLint val)
	{
		this->ambientTex = val;
	}
	void setDiffuseTex(GLint val)
	{
		this->diffuseTex = val;
	}
	void setSpecularTex(GLint val)
	{
		this->specularTex = val;
	}
	void setAmbient(glm::vec3 val)
	{
		this->ambient = val;
	}
	void setDiffuse(glm::vec3 val)
	{
		this->diffuse = val;
	}
	void setSpecular(glm::vec3 val)
	{
		this->specular = val;
	}
	void setShininess(float val)
	{
		this->shininess = val;
	}

	//Function to load our Uniforms into GLSL shader
	void sendToShader(Shader& program)
	{

		if (glGetUniformLocation(program.ID, "material.diffuseMap") != -1)
		{	
			program.use();
			if (this->diffuseTex != -1)
			{
				program.setInt("material.diffuseMap", this->diffuseTex);
				program.setBool("material.useDiffuseMap", true);
			}
			else
			{
				program.setBool("material.useDiffuseMap", false);
			}
			if (this->specularTex != -1)
			{
				program.setInt("material.specularMap", this->specularTex);
				program.setBool("material.useSpecularMap", true);
			}
			else
			{
				program.setBool("material.useSpecularMap", false);
			}
			program.setVec3("material.ambient", this->ambient);
			program.setVec3("material.diffuse", this->diffuse);
			program.setVec3("material.specular", this->specular);
			program.setFloat("material.shininess", this->shininess);
		}
		//program.unuse();    //Unbinding the program seems to cause all the textures to not load, even when the program is binded again before use
	
	}
};