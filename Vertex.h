#pragma once

#include <glm.hpp>
#include <vector>


class Vertex
{
public:
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoord;

	Vertex& operator=(const Vertex& vert)
	{
		this->position = vert.position;
		this->normal = vert.normal;
		this->texcoord = vert.texcoord;

		return *this;
	}
	Vertex(glm::vec3 pos = { 0,0,0 }, glm::vec3 normal = { 0,0,0 }, glm::vec2 texcoord = { 0,0 })
	{
		this->position = pos;
		this->normal = normal;
		this->texcoord = texcoord;
	}

};



void loadVertexArray(std::vector<float> &fArray, std::vector<Vertex> &Vec,unsigned int vecSize = 8)
{
	Vec.clear();
	if (vecSize == 8)
	{
		int vertexArraySize = fArray.size() / 8;

		for (int i = 0; i < vertexArraySize; i++) {
			Vertex v;
			v.position[0] = fArray[i * 8];
			v.position[1] = fArray[i * 8 + 1];
			v.position[2] = fArray[i * 8 + 2];
			v.normal[0] = fArray[i * 8 + 3];
			v.normal[1] = fArray[i * 8 + 4];
			v.normal[2] = fArray[i * 8 + 5];
			v.texcoord[0] = fArray[i * 8 + 6];
			v.texcoord[1] = fArray[i * 8 + 7];
			Vec.push_back(v);
		}
	}
	else if (vecSize == 3)
	{
		int vertexArraySize = fArray.size() / 3;

		for (int i = 0; i < vertexArraySize; i++) {
			Vertex v;
			v.position[0] = fArray[i * 3];
			v.position[1] = fArray[i * 3 + 1];
			v.position[2] = fArray[i * 3 + 2];
			Vec.push_back(v);
		}
	}
}