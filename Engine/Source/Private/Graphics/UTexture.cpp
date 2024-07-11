#include "Graphics/UTexture.h"


//Extra Libs 
#include <GLEW/glew.h> 
#include <STB_IMAGE/stb_image.h> 


UTexture::UTexture()
{
    m_path = m_fileName = "";
    m_ID = 0U; 
    m_width = m_height = m_channels = 0; 

}

UTexture::~UTexture()
{
    //as long as an id was generated, delete the texture from open gl 
    if(m_ID >0)
    glDeleteTextures(1, &m_ID);
    UDebug::Log("Texture destoryed - " + m_fileName); 
}

bool UTexture::LoadTexture(const UString& fileName, const UString& path){
    //assign the file name and path 
    m_fileName = fileName;
    m_path = path;

    //stb image imports images upside down 
    //but actually open gl reads in an inverted state 
    stbi_set_flip_vertically_on_load(true);

    //loading image into a computer readable format 
    unsigned char* data = stbi_load(
        m_path.c_str(), //path to the image 
        &m_width, &m_height, //width and height of the image 
        &m_channels, //rgba
        0 //do want to specifically require a certatin amount of chanels 
    );

    //test if the data imported or not 
    if (data == nullptr) {
        UString error = "Failed to load texture -" + m_fileName + ": " + stbi_failure_reason();
        UDebug::Log(error, LT_ERROR);
        return false;
     }

    // test the amount of chanels 
    if (m_channels > 4 || m_channels < 3) {
        UDebug::Log("Failed to import texture - " + m_fileName 
            + ": Incorrect number of chanels, must have 3 or 4 chanels"); 
        return false; 

    }

    //generate the texture ID in open gl 
    glGenTextures(1, &m_ID);


    //test if the generate ID in open gl 
    if (m_ID == 0) {
        UString error = reinterpret_cast<const char*>(glewGetErrorString(glGetError()));
        //UString error = "Failed to generate texture id - " + m_fileName + ";";
        UString errorMsg = "Failed to generate texture id - " + m_fileName + "; " + error;
        UDebug::Log(errorMsg, LT_ERROR);
        return false;


     }

        //bind the texture 
        //tells open gl that we want to use this texture 
        glBindTexture(GL_TEXTURE_2D, m_ID);


        //set some default paramaters for the texture 
        //set the texture wrapping paramaters 
        //if the texture does not fit the model , repeat the texture 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //x 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //y 

        //set the filtering parameters 
        //how much to blur pixels 
        //to resolution of the texture is lower than the size of the model 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLint intFormat = GL_RGB; 

        //if the chanels of the imported image = 4 then set the import  type to rgba 
        if (m_channels == 4) {
            intFormat = GL_RGBA; 
        }
        
        //load the image dat into the texture that we just updated 
        glTexImage2D(
            GL_TEXTURE_2D,
            0, // levels 
            intFormat, //format of the texture 
            m_width, m_height, //width and height 
            0, //border around the image (legacy) 
            intFormat, // format of the texture (external) 
            GL_UNSIGNED_BYTE, //type of data passed in 
            data // image data we got from stbi 
        );

        //generate mip maps
        //lower resolution version of the texture 
        glGenerateMipmap(GL_TEXTURE_2D); 

        //unbind the texture from open gl 
        //make room for next texture 
        Unbind();

        stbi_image_free(data); 
        //log the success of the import 
        UDebug::Log("Successfully imported texture -" + m_fileName, LT_SUCCESS); 

        return true;
}

void UTexture::BindTexture(const UUi32& textureNumber)
{
    glActiveTexture(GL_TEXTURE0 + textureNumber);
    glBindTexture(GL_TEXTURE_2D, m_ID);
}

void UTexture::Unbind()
{
    glBindTexture(GL_TEXTURE_2D, 0); 
}
