#include "Graphics/UModel.h"

//External Libs 
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>
#include <ASSIMP/mesh.h>

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

void UModel::ImportModel(const UString& filePath)
{
	//crete an assimp importer 
	Assimp:: Importer importer;

	//import the file and convert the model to an assimp scene 
	//add post processing flag triangulate to make sure the model is triangle 
	const auto scene = importer.ReadFile(filePath, aiProcess_Triangulate);

	//check if the import failed in any way
	//!scene is checking if the object was null 
	//FLAGS_INCOMPLETE is checking if the import failed 
	//rootNode is checking if the model has mesh at all 
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		UDebug::Log("Error importing model form " + filePath + ": " + importer.GetErrorString(), LT_ERROR);
		return;
	}

	aiMatrix4x4 sceneTransform;
	//update the scene matrix to start the location at x0, y0, z0
	aiMatrix4x4::Translation({ 0.0f, 0.0f, 0.0f }, sceneTransform);
	//set the rotation to x0, y0, z0
	sceneTransform.FromEulerAnglesXYZ({ 0.0f, 0.0f, 0.0f });
	//set the scale to x1, y1, z1
	aiMatrix4x4::Scaling({ 1.0f, 1.0f, 1.0f }, sceneTransform);
	//find all meshes in the scene and fail if any of the fail
	if (!FindImportMeshes(*scene->mRootNode, *scene, sceneTransform)) {
		UDebug::Log("Model failed to convert ASSIMP scene" + filePath, LT_ERROR);
		return;
	}

	//log the success of the model 
	UDebug::Log("Model successfully imported: " + filePath, LT_SUCCESS);
}

void UModel::Render(const TShared<UShaderProgram>& shader)
{
	for (const auto& mesh2 : m_meshStack) {
		mesh2->Render(shader, m_transform);
	}

	
}

bool UModel::FindImportMeshes(const aiNode& node, const aiScene& scene,
	const aiMatrix4x4& parentTransform)
{
	//looping throug all the meshes in the node
	for (UUi32 i = 0; i < node.mNumMeshes; ++i) {
		// TO DO: Convert aiMesh to UMesh

		//get the mesh index for the mesh in the scene 
		const auto aMesh = scene.mMeshes[node.mMeshes[i]];

		//store the mesh vertices and andices
		TArray<USVertextData>meshVertices;
		TArray<UUi32> meshIndices;

		//loop through every vertex and get the data for conversion 
		for (UUi64 j = 0; j < aMesh->mNumVertices; ++j) {
			//create an empty vertex
			USVertextData vertex;

			//get the position of the vertex
			vertex.m_position[0] = aMesh->mVertices[j].x;
			vertex.m_position[1] = aMesh->mVertices[j].y;
			vertex.m_position[2] = aMesh->mVertices[j].z;

			//if there are vertex colors then update
			if (aMesh->HasVertexColors(j)) {
				vertex.m_colour[0] = aMesh->mColors[j]->r;
				vertex.m_colour[1] = aMesh->mColors[j]->g;
				vertex.m_colour[2] = aMesh->mColors[j]->b;
			}

			//set the texture coordinates
			//texture coordiantes can have multiple sets
			//the first array index is the set [0]
			//the second array index is the vertex data 

			if (aMesh->HasTextureCoords(j)) {
				vertex.m_textCords[0] = aMesh->mTextureCoords[0][j].x;
				vertex.m_textCords[1] = aMesh->mTextureCoords[0][j].y;

			}
				
			//get the normals  for the model 
			vertex.m_normal[0] = aMesh->mNormals[j].x;
			vertex.m_normal[1] = aMesh->mNormals[j].y;
			vertex.m_normal[2] = aMesh->mNormals[j].z;

			//add the data into our vertex array
			meshVertices.push_back(vertex);
		}

		//the gpu requires a minimum of 3 vertices t orender 
		//fail is there are less than 3
		if (meshVertices.size() < 3) {
			UDebug::Log("Mesh has lesser than 3 vertices", LT_ERROR);

				return false;

		}
		//loop through all of the faces on the parent transform 
		for (UUi64 j = 0; j < aMesh->mNumFaces; ++j) {
			//store the faces as a variable 
			auto face = aMesh->mFaces[j];

			//looping through all the indices in the face 
			//there should only be 3 
			for (UUi32 k = 0; k < face.mNumIndices; ++k) {
				meshIndices.push_back(face.mIndices[k]);
			}
		}

		//crete the mesh object 
		auto lMesh = TMakeUnique<UMesh>();
		
		
		// test if it failed 
		if (!lMesh->CreateMesh(meshVertices, meshIndices)) {
			UDebug::Log("Mesh failed to convert from A mesh to L mesh", LT_ERROR);
			return false;
		}


		//set the ralative transformation for the mesh 
		aiMatrix4x4 relTransform = parentTransform * node.mTransformation;

		//set a default matrix transform for glm 
		glm::mat4 matTransform(1.0f);

		//convert the relative ASSIMP transform into a glm transform 
		matTransform[0][0] = relTransform.a1; matTransform[1][0] = relTransform.a2;
		matTransform[2][0] = relTransform.a3; matTransform[3][0] = relTransform.a4;

		matTransform[0][1] = relTransform.b1; matTransform[1][1] = relTransform.b2;
		matTransform[2][1] = relTransform.b3; matTransform[3][1] = relTransform.b4;

		matTransform[0][2] = relTransform.c1; matTransform[1][2] = relTransform.c2;
		matTransform[2][2] = relTransform.c3; matTransform[3][2] = relTransform.c4;

		matTransform[0][3] = relTransform.d1; matTransform[1][3] = relTransform.d2;
		matTransform[2][3] = relTransform.d3; matTransform[3][3] = relTransform.d4;

		//update the relative transform on the mesh 
		lMesh->SetRelativeTransform(matTransform); 

		//add the texture and add it to the mesh stack 
		m_meshStack.push_back(std::move(lMesh));

	}


	//adding the relative to the parent transform 
	const aiMatrix4x4 nodeRelTransform = parentTransform * node.mTransformation;

	// loop through all of the child nodes inside this node 
	for (UUi32 i = 0; i < node.mNumChildren; ++i) {
		if (!FindImportMeshes(*node.mChildren[i], scene, nodeRelTransform))
			return false;
	}

	return true;
}
