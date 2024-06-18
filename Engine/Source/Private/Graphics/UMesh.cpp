#include "Graphics/UMesh.h"
#include "Debug/UDebug.h"
#include "Graphics/UShaderProgram.h"
//External Libs
#include<GLEW/glew.h>
UMesh::UMesh()
{
	m_vao = m_vbo = m_eao = 0;
	UDebug::Log("Mesh created");


}

UMesh::~UMesh()
{
	UDebug::Log("Mesh destroyed");
}

bool UMesh::CreateMesh(const std::vector<USVertextData> vertices, const std::vector<uint32_t>& indices)
{
	//store the vertex data
	m_vertices = vertices;
	m_indices = indices;

	//create a vertex array object (VAO)
	//assign the id for the object to the m_vao variable
	//store a reference to any VBO;s attached to the VAO
	glGenVertexArrays(1, &m_vao);

	//test if the vao failed 
	if (m_vao == 0) {
		//convert error into a readabble string 
		std::string errorMsg = reinterpret_cast<const char*>( glewGetErrorString(glGetError()));
		UDebug::Log("Mesh failed to create VAO: " + errorMsg, LT_WARN);
		return false;
	}
	//this says use vao as the active working vao for any vao functions 
	glBindVertexArray(m_vao);



	//create a vertex  buffer object
	//vertex buffer object holds the data for the vertices in the GPU 
	glGenBuffers(1, &m_vbo);
	//test if the vbo failed 
	if (m_vbo == 0) {
		//convert error into a readabble string 
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		UDebug::Log("Mesh failed to create VBO: " + errorMsg, LT_WARN);
		return false;
	}
	//bind the buffer object to say this is the active working vbo
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);



	//create a Element Array Buffer
	glGenBuffers(1, &m_eao);
	//test if the eao failed 
	if (m_eao == 0) {
		//convert error into a readabble string 
		std::string errorMsg = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
		UDebug::Log("Mesh failed to create EAO: " + errorMsg, LT_WARN);
		return false;
	}
	//bind the eao as the active element array buffer object
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_eao);


	//set the buffer data 
	//start with the VBO which stores the vertex data 
	glBufferData(
		GL_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(m_vertices.size() * sizeof(USVertextData)), //size of each data in bytes 
		m_vertices.data(),//memory location of the frequency 
		GL_STATIC_DRAW //this data will not be modified frequencly 
	);

	//set the data for the EAO 
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		static_cast<GLsizeiptr>(m_indices.size() * sizeof(uint32_t)),
		m_indices.data(),
		GL_STATIC_DRAW
	);

	//pass out the vertex data in sperate formats 
	glEnableVertexAttribArray(0);

	//set the position of data to the 0 index of the attribute array 
	glVertexAttribPointer(
		0,//location to store data ain the attribute array 
		3,//how many number to pass into the attribute array index 
		GL_FLOAT,//the type of data to store 
		GL_FALSE,//should we normalise the values, generally no
		sizeof(USVertextData),//how big is each data array in a VertexData
		nullptr//how many numbers to skip 
		);

	//pass out the vertex data in sperate formats 
	glEnableVertexAttribArray(0);

	//set the position of data to the 0 index of the attribute array 
	glVertexAttribPointer(
		0,//location to store data ain the attribute array 
		3,//how many number to pass into the attribute array index 
		GL_FLOAT,//the type of data to store 
		GL_FALSE,//should we normalise the values, generally no
		sizeof(USVertextData),//how big is each data array in a VertexData
		nullptr//how many numbers to skip 
		);



	//pass out the vertex data in sperate formats 
	glEnableVertexAttribArray(1);

	//set the position of data to the 0 index of the attribute array 
	glVertexAttribPointer(
		1,//location to store data ain the attribute array 
		3,//how many number to pass into the attribute array index 
		GL_FLOAT,//the type of data to store 
		GL_FALSE,//should we normalise the values, generally no
		sizeof(USVertextData),//how big is each data array in a VertexData
		(void*)(sizeof(float) * 3)//how many numbers to skip in byts
	);


	//common practice to clear the vao from the GPU 
	glBindVertexArray(0);
	
	return true;
}

void UMesh::Render(const std::shared_ptr<UShaderProgram>& shader, const USTransform& transform)
{
	shader->Activate();
	//update the transform of the mesh based on the model transform 
	shader->SetModelTransform(transform); 


	//binding this aesh as the active vao
	glBindVertexArray(m_vao);

	//render the vao
	glDrawElements(
		GL_TRIANGLES,//draw the mesh as triangles 
		static_cast<GLsizei>(m_indices.size()),//how many vertices are there? 
		GL_UNSIGNED_INT, //what typefo data is the index array  
		nullptr //how many are you gonna skip 
	);

	//clear the vao 
	glBindVertexArray(0);
}
