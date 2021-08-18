/**
 CEntity2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Settings
#include "..\GameControl\Settings.h"
#include <string>

//CS: Include Mesh.h to use to draw (include vertex and index buffers)
#include "Mesh.h"
#include <vector>

//Collision Detection and Resolution
#include "../App/Source/Scene2D/Collider2D.h"

using namespace std;

class CEntity2D 
{
public:
	enum ENTITY_TYPE
	{
		NONE = 0,
		PLAYER,
		CLONE,
		ENEMY,
		INTERACTABLES,
		TOTAL,
	};
	ENTITY_TYPE type;

	CEntity2D(ENTITY_TYPE type = NONE);

	CEntity2D(string entity_toClone) {}

	// Constructor
	//CEntity2D(void);

	// Destructor
	virtual ~CEntity2D(void);

	// Init
	virtual bool Init(void);

	// Set the name of the shader to be used in this class instance
	virtual void SetShader(const std::string& _name);

	// Update
	virtual void Update(const double dElapsedTime);

	// PreRender
	virtual void PreRender(void);

	// Render
	virtual void Render(void);

	// PostRender
	virtual void PostRender(void);

	virtual void RenderCollider();

	Collider2D* GetCollider();

	// The i32vec2 which stores the indices of an Entity2D in the Map2D
	glm::vec2 vTransform;

	// The vec2 variable which stores the UV coordinates to render the Entity2D
	glm::vec2 vec2UVCoordinate;

protected:
	// Name of Shader Program instance
	std::string sShaderName;

	//all of entity list
	//std::vector<CEntity2D*> entity_list;

	//CS: The mesh that is used to draw objects
	CMesh* mesh;

	// OpenGL objects
	unsigned int VBO, VAO, EBO;

	// The texture ID in OpenGL
	unsigned int iTextureID;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	Collider2D* collider2D;

	// Load a texture
	virtual bool LoadTexture(const char* filename);

	// Settings
	CSettings* cSettings;
};
