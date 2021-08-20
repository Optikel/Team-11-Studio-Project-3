/**
 CScene2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

// Include SingletonTemplate
#include "DesignPatterns\SingletonTemplate.h"

// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLM
#include <includes/glm.hpp>
#include <includes/gtc/matrix_transform.hpp>
#include <includes/gtc/type_ptr.hpp>

// Include Shader Manager
#include "RenderControl\ShaderManager.h"

// Include the Map2D as we will use it to check the player's movements and actions

//#include "Map2D.h"

#include "Map2D.h"
//Camera
#include "Camera2D.h"

#include "Enemy2D.h"

// Include vector
//#include <vector>

// Include Keyboard controller
//#include "Inputs\KeyboardController.h"

// GUI
//#include "GUI/GUI.h"
// GUI_Scene2D
#include "GUI_Scene2D.h"

// Game Manager
#include "GameManager.h"

// Include SoundController
#include "..\SoundController\SoundController.h"

#include "EntityManager.h"

class CScene2D : public CSingletonTemplate<CScene2D>
{
	friend CSingletonTemplate<CScene2D>;
public:
	// Init
	bool Init(void);

	// Update
	bool Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

	void CreateIMGUI();
	void DeleteIMGUI();

	bool isCompleted;

	float fCooldown = 0.f;
protected:
	// The handler containing the instance of the 2D Map
	CMap2D* cMap2D;
	// The handler containing the instance of CPlayer2Ds
	CPlayer2D* cPlayer2D;
	std::vector<CPlayer2D*> clones;

	// A vector containing the instance of CEnemy2Ds
	vector<CEntity2D*> enemyVector;

	// Keyboard Controller singleton instance
	CKeyboardController* cKeyboardController;

	CKeyboardInputHandler* cKeyboardInputHandler;

	// A transformation matrix for controlling where to render the entities
	glm::mat4 transform;

	// GUI_Scene2D
	CGUI_Scene2D* cGUI_Scene2D;

	// Game Manager
	CGameManager* cGameManager;

	CEntityManager* cEntityManager;

	// Handler to the CSoundController
	CSoundController* cSoundController;

	CInventoryManager* cInventoryManager;

	//Handler for camera
	Camera2D* cameraHandler;

	// Constructor
	CScene2D(void);
	// Destructor
	virtual ~CScene2D(void);
	
	template <typename T>
	void LoadEnemy();
};

template<typename T>
inline void CScene2D::LoadEnemy()
{
	while (true)
	{
		CEnemy2D* cEnemy2D = new T();
		// Pass shader to cEnemy2D
		cEnemy2D->SetShader("2DColorShader");
		// Initialise the instance
		if (cEnemy2D->Init() == true)
		{
			enemyVector.push_back(cEnemy2D);
		}
		else
		{
			// Break out of this loop if the enemy has all been loaded
			break;
		}
	}
}

