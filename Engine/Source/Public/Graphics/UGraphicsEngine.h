#pragma once
#include<EngineTypes.h>
#include<Graphics/USMaterial.h>
typedef void* SDL_GLContext;
struct SDL_Window;
struct USCollision;
class UModel;
class UMesh;
class UShaderProgram;
struct USCamera;
struct USLight;
struct USPointLight;
struct USDirLight;
struct USMaterial;

class UGraphicsEngine {
public:
	UGraphicsEngine() = default;
	~UGraphicsEngine() = default;

	//initialise the graphics engine 
	bool InitEngine(SDL_Window* sdlWindow, const bool& vsync);

	//render the graphics engine 
	void Render(SDL_Window* sdlWindow);
	//return a weak version of the camera 
	TWeak<USCamera>GetCamera() { return m_camera; }

	//create point light and return shared pointer 
	TWeak<USPointLight> CreatePointLight();

	//create directional light and return shared pointer
	TWeak<USDirLight> CreateDirLight();

	//import a model and rertun a weak pointer 
	TShared<UModel>ImportModel(const UString& path);

	//creates a material for the engoine 
	TShared<USMaterial> CreateMaterial();

	//creates a collision meshe to be rendered as a wireframe 
    void CreateCollisionMesh(const TWeak<USCollision>& col);

private:
	// storing memory location for open gl context 
	SDL_GLContext m_sdlGLContext;

	//store the shader for the engine
	TShared<UShaderProgram> m_shader;

	TShared<UShaderProgram> m_wireShader;
	//store camera 
	TShared<USCamera> m_camera;
	//store all light in the engine 
	TArray<TShared<USLight>> m_lights;

	//stored all of the models in the engine 
	TArray<TWeak<UModel>> m_models;

	//stored all of the collision meshes
	TArray<TWeak<USCollision>> m_collisions;

	//default material for all models 
	TShared<USMaterial>m_defaultMaterials; 
};