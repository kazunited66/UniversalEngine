#pragma once
#include "EngineTypes.h"


class UTexture {
public:
	UTexture();
	~UTexture();

	//import a file and convert it to a texture 
	bool LoadTexture(const UString& fileName, const UString& path);

	//activate the texture to use for open gl 
	void BindTexture(const UUi32& textureNumber);

	//deactive the texture in open gl 
	void Unbind();

	//gets the import of the texture 
	UString GetImportPath() const { return m_path; }

	//get the custom file of the texture 
	UString GetName() const { return m_fileName;  }

	//get the id of the texture for open gl 
	UUi32 GetID() const { return m_ID; }

	//get 


private:
	//import path of the image 
	UString m_path;

	//custom name of the texture 
	UString m_fileName;

	//the id for the texture in open gl 
	UUi32 m_ID;

	//textur parameters 
	int m_width, m_height, m_channels; 



	
};