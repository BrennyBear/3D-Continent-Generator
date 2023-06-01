#pragma once

#include"Mesh.h"
#include"Texture.h"
#include"Shader.h"
#include"Material.h"


class Model
{
private:
	std::vector<Mesh*> meshes;
	glm::vec3 position;
public:
	Model(
		glm::vec3 position,
		std::vector<Mesh*>& meshes
	)
	{
		this->position = position;

		for (auto* i : meshes)
		{
			this->meshes.push_back(new Mesh(*i));
		}

		for (auto& i : this->meshes)
		{
			i->move(this->position);
			i->setOrigin(this->position);
		}
	}

//	//OBJ file loaded model
//	Model(
//		glm::vec3 position,
//		Material* material,
//		Texture* orTexDif,
//		Texture* orTexSpec,
//		const char* objFile
//	)
//	{
//		this->position = position;
//		this->material = material;
//		this->overrideTextureDiffuse = orTexDif;
//		this->overrideTextureSpecular = orTexSpec;
//
////		std::vector<Vertex> mesh = loadOBJ(objFile);
//		this->meshes.push_back(new Mesh(mesh.data(), mesh.size(), NULL, 0, glm::vec3(1.f, 0.f, 0.f),
//			glm::vec3(0.f),
//			glm::vec3(0.f),
//			glm::vec3(1.f)));
//
//		for (auto& i : this->meshes)
//		{
//			i->move(this->position);
//			i->setOrigin(this->position);
//		}
//	}

	~Model()
	{
		for (auto*& i : this->meshes)
			delete i;
	}

	Mesh* getMesh(int i)
	{
		return meshes.at(i);
	}

	//Functions
	void rotate(const glm::vec3 rotation)
	{
		for (auto& i : this->meshes)
			i->rotate(rotation);
	}
	void setRotation(const glm::vec3 rot)
	{
		for(auto & i : this->meshes)
		{
			i->setRotation(rot);
		}
	}
	void setPosition(const glm::vec3 offset)
	{
		for (auto& i : this->meshes)
		{
			i->setPosition(offset);
		}
	}
	void move(const glm::vec3 offset)
	{
		for (auto& i : this->meshes)
		{
			i->setPosition(i->getPosition() + offset);
		}
	}
	void setScale(const glm::vec3 scale)
	{
		for (auto& i : this->meshes)
		{
			i->setScale(scale);
		}
	}

	void update()
	{

	}

	void render(Shader* shader)
	{
		//Update the uniforms
//		this->updateUniforms();

		//Use a program (BECAUSE SHADER CLASS LAST UNIFORM UPDATE UNUSES IT)
		shader->use();

		//Draw
		for (auto& i : this->meshes)
		{
			//Activate texture for each mesh
			//this->overrideTextureDiffuse->bind(overrideTextureDiffuse->getID());
			//this->overrideTextureSpecular->bind(overrideTextureSpecular->getID());

			i->render(shader); //Activates shader also
		}
	}
};