#include "Graphics/UModel.h"

//External Libs 
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>
#include <ASSIMP/mesh.h>

void UModel::ImportModel(const UString& filePath)
{
	//crete an assimp importer 
	Assimp::Importer importer;

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

	//meshes counts 
	UUi32 meshesCreated = 0;

	//find all meshes in the scene and fail if any of the fail
	if (!FindImportMeshes(*scene->mRootNode, *scene, sceneTransform, &meshesCreated)) {
		UDebug::Log("Model failed to convert ASSIMP scene" + filePath, LT_ERROR);
		return;
	}

	//set the materials srtack size to the amount of materials on the model 
	m_materialsStack.resize(scene->mNumMaterials);


	//log the success of the model 
	UDebug::Log("Model successfully imported with (" + std::to_string(meshesCreated) + " ) + meshes: "
		+ filePath, LT_SUCCESS);
}

void UModel::Render(const TShared<UShaderProgram>& shader, const TArray<TShared<USLight>>& lights)
{
	for (const auto& mesh2 : m_meshStack) {
		mesh2->Render(shader, m_transform, lights, m_materialsStack[mesh2->materialIndex]);
	}

}

void UModel::SetMaterialBySlot(unsigned int slot, TShared<USMaterial>& material)
{
	//showe that the material slot exsist 
	if (slot >= m_materialsStack.size()) {
		UDebug::Log("No material slot exsists at that index: " + std::to_string(slot), LT_WARN);
		return;
	}

	//change the material if it does 
	m_materialsStack[slot] = material;
}

bool UModel::FindImportMeshes(const aiNode& node, const aiScene& scene,
	const aiMatrix4x4& parentTransform, UUi32* meshesCreated)
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

			if (aMesh->HasTextureCoords(0)) {
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

		//get the material index from the assimp mesh and set our mesh index to the same 
		lMesh->materialIndex = aMesh->mMaterialIndex;

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

		//counst the meshes created
		++*meshesCreated;

	}


	//adding the relative to the parent transform 
	const aiMatrix4x4 nodeRelTransform = parentTransform * node.mTransformation;

	// loop through all of the child nodes inside this node 
	for (UUi32 i = 0; i < node.mNumChildren; ++i) {
		if (!FindImportMeshes(*node.mChildren[i], scene, nodeRelTransform, meshesCreated))
			return false;
	}

	return true;
}
