#include "Graphics/UModel.h"

const std::vector<USVertextData> polyVData = {
	//x    //y    //z     //r    //g   //b    //tx   //ty 
  { {-0.5f, 0.5f, 0.0f }, {1.0f, 1.0f, 1.0f }, {0.0f, 1.0f} }, //vertex data 1 - top left - 0
  { {0.5f,  0.5f, 0.0f }, {1.0f, 1.0f, 1.0f }, {1.0f, 1.0f} }, //vertex data 2 - top right - 1  
  { {-0.5f, -0.5, 0.0f }, {1.0f, 1.0f, 1.0f }, {0.0f, 0.0f} }, //vertex data 3 - bottom left - 2
  { {0.5f, -0.5f, 0.0f }, {1.0f, 1.0f, 1.0f }, {1.0f, 0.0f} }  //vertex data 4 - bottom right - 3 
};

const std::vector<uint32_t> polyIData = {
	0, 1, 2, //triangle 1
	1, 2, 3  //triangle 2
};


const std::vector<USVertextData> cubeVData = {
	// x   // y   // z     // r   // g  // b    // tx  // ty
{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // z vertex data 1 - f top left - 0
{ {	 1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // z vertex data 2 - f top right - 1
{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // z vertex data 3 - f bottom left - 2
{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // z vertex data 4 - f bottom right - 3

{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // z vertex data 5 - b top right - 4
{ {	-1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // z vertex data 6 - b top left - 5
{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // z vertex data 7 - b bottom right - 6
{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // z vertex data 8 - b bottom left - 7

{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // x vertex data 9 - f top left - 8
{ {	 1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // x vertex data 10 - f top right - 9
{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // x vertex data 11 - f bottom left - 10
{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // x vertex data 12 - f bottom right - 11

{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // x vertex data 13 - b top right - 12
{ {	-1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // x vertex data 14 - b top left - 13
{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // x vertex data 15 - b bottom right - 14
{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // x vertex data 16 - b bottom left - 15

{ { -1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } }, // y vertex data 17 - f top left - 16
{ {	 1.0f,  1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // y vertex data 18 - f top right - 17
{ { -1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // y vertex data 19 - f bottom left - 18
{ {  1.0f, -1.0f,  1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // y vertex data 20 - f bottom right - 19

{ {  1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } }, // y vertex data 21 - b top right - 20
{ {	-1.0f,  1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 1.0f } }, // y vertex data 22 - b top left - 21
{ {  1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 0.0f } }, // y vertex data 23 - b bottom right - 22
{ { -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } } // y vertex data 24 - b bottom left - 23
};

const std::vector<uint32_t> cubeIData = {
	0, 1, 2, // f triangle 1
	1, 2, 3, // f triangle 2

	4, 5, 6, // b triangle 1
	5, 6, 7, // b triangle 2

	13, 8, 10, // l triangle 1
	13, 15, 10, // l triangle 2

	9, 12, 14, // r triangle 1
	9, 14, 11, // r triangle 2

	21, 20, 16, // t triangle 1
	20, 17, 16, // t triangle 2

	18, 19, 23, // bt triangle 1
	23, 19, 22 // bt triangle 2
};

void UModel::MakePoly(const TShared<UTexture>& texture)
{
	//crete the debug mesh
	TUnique<UMesh> mesh2 = TMakeUnique<UMesh>();


	//create the mesh and test if it failed 
	if (mesh2->CreateMesh(polyVData, polyIData)) {
		UDebug::Log("Failed to create poly mesh");
	}

	//add the texture and add it to the mesh stack 

	mesh2->SetTexture(texture);
	//std::move will move the reference from the previous reference to a new one - 
	// - without destroying or copying the reference
	m_meshStack.push_back(std::move(mesh2));
}

void UModel::MakeCube(const TShared<UTexture>& texture)
{
	//crete the debug mesh
	TUnique<UMesh> mesh2 = TMakeUnique<UMesh>();


	//create the mesh and test if it failed 
	if (mesh2->CreateMesh(cubeVData, cubeIData)) {
		UDebug::Log("Failed to create cube mesh");
	}

	//add the texture and add it to the mesh stack 

	mesh2->SetTexture(texture);
	//std::move will move the reference from the previous reference to a new one - 
	// - without destroying or copying the reference
	m_meshStack.push_back(std::move(mesh2));

	
}

void UModel::Render(const TShared<UShaderProgram>& shader)
{
	for (const auto& mesh2 : m_meshStack) {
		mesh2->Render(shader, m_transform);
	}

	
}
