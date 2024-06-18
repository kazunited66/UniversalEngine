#pragma once

//system Libs
#include <iostream>
#include <vector>
#include <cstdint>

class UShaderProgram;
struct USTransform;


struct USVertextData {

	float m_position[3] = { 0.0f, 0.0f, 0.0f };
	float m_colour[3] = { 1.0f,1.0f,1.0f };
	float m_textCords[2] = { 0.0f, 0.0f };
	float m_normal[3] = { 0.0f, 0.0f, 0.0f };

};
class UMesh {
public: 
	UMesh();
	~UMesh();

	//create a mesh using vertex and index data
	bool CreateMesh(const std::vector<USVertextData> vertices, const std::vector<uint32_t>& indices);

	//draw the mesh to the renderer 
	void Render(const std::shared_ptr<UShaderProgram>& shader, const USTransform& transform);
private:
	//store the vertices
	std::vector<USVertextData> m_vertices;

	//store the indices for the vertex data
	std::vector<uint32_t>m_indices;

	//store the ID for the vertex array object 
	uint32_t m_vao;

	//store the ID for the vertex buffer object 
	uint32_t m_vbo;

	//store the ID for the element array object 
	uint32_t m_eao;




};