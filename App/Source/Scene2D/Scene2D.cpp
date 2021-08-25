#include "Scene2D.h"
#include <iostream>
using namespace std;

// Include Shader Manager
#include "RenderControl\ShaderManager.h"

#include "System\filesystem.h"
#include "Math/MyMath.h"

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CScene2D::CScene2D(void)
	: cMap2D(NULL)
	, cPlayer2D(NULL)
	, cKeyboardController(NULL)
	, cGameManager(NULL)
	, cSoundController(NULL)
	, CInputHandler(NULL)
	, isCompleted(false)
	, cEntityManager(NULL)
	, cameraHandler(NULL)

{
}

/**
 @brief Destructor
 */
CScene2D::~CScene2D(void)
{
	if (cSoundController)
	{
		// We won't delete this since it was created elsewhere
		cSoundController = NULL;
	}

	if (cGameManager)
	{
		cGameManager->Destroy();
		cGameManager = NULL;
	}

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;

	CInputHandler = NULL;

	// Destroy the enemies
	for (unsigned i = 0; i < enemyVector.size(); i++)
	{
		delete enemyVector[i];
		enemyVector[i] = NULL;
	}
	enemyVector.clear();

	if (cPlayer2D)
	{
		delete cPlayer2D;
		cPlayer2D = NULL;
	}

	if (cMap2D)
	{
		cMap2D->Destroy();
		cMap2D = NULL;
	}

	if (cameraHandler) {
		cameraHandler->Destroy();
		cameraHandler = NULL;
	}

}

/**
@brief Init Initialise this instance
*/ 
bool CScene2D::Init(void)
{
	Math::InitRNG();
	// Include Shader Manager
	CShaderManager::GetInstance()->Use("2DShader");
	CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);

	// Create and initialise the Map 2D
	cMap2D = CMap2D::GetInstance();
	// Set a shader to this class
	cMap2D->SetShader("2DShader");
	// Initialise the instance
	if (cMap2D->Init(3, 24, 32) == false)
	{
		cout << "Failed to load CMap2D" << endl;
		return false;
	}
	// Load the map into an array
	if (cMap2D->LoadMap("Maps/DM2213_Map_Level_Test.csv") == false)
	{
		// The loading of a map has failed. Return false
		return false;
	}

	// Load Scene2DColor into ShaderManager
	CShaderManager::GetInstance()->Add("2DColorShader", "Shader//Scene2DColor.vs", "Shader//Scene2DColor.fs");
	CShaderManager::GetInstance()->Use("2DColorShader");
	CShaderManager::GetInstance()->activeShader->setInt("texture1", 0);
	cEntityManager = CEntityManager::GetInstance();
	cEntityManager->EntityManagerInit();

	cPlayer2D = cEntityManager->GetPlayer();

	cameraHandler = Camera2D::GetInstance();
	cameraHandler->Reset();
	cameraHandler->UpdateTarget(cPlayer2D->vTransform);

	// Create and initialise the CEnemy2D
	enemyVector.clear();
	
	//300
	LoadEnemy<CEnemy2D>();

	// Setup the shaders
	CShaderManager::GetInstance()->Add("textShader", "Shader//text.vs", "Shader//text.fs");
	CShaderManager::GetInstance()->Use("textShader");

	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();

	// Game Manager
	cGameManager = CGameManager::GetInstance();
	cGameManager->Init();

	// Load the sounds into CSoundController
	cSoundController = CSoundController::GetInstance();
	cSoundController->PlaySoundByID(SOUND_ID::BGM_HENESYS);

	CInputHandler = CInputHandler::GetInstance();

	//cInventoryManager = CInventoryManager::GetInstance();
	cInventoryM = CInventoryManager::GetInstance();
		
	return true;
}

/**
@brief Update Update this instance
*/
bool CScene2D::Update(const double dElapsedTime)
{
	++m_FrameStorage.iCurrentFrame;

	cEntityManager->Update(dElapsedTime);

	// Call the Map2D's update method
	cMap2D->Update(dElapsedTime);

	if (fCooldown > 0)
	{
		fCooldown -= (float)dElapsedTime;
	}

	// Get keyboard updates
	if (cKeyboardController->IsKeyDown(GLFW_KEY_F6))
	{
		// Save the current game to a save file
		// Make sure the file is open
		try {
			if (cMap2D->SaveMap("Maps/DM2213_Map_Level_01_SAVEGAMEtest.csv") == false)
			{
				throw runtime_error("Unable to save the current game to a file");
			}
		}
		catch (runtime_error e)
		{
			cout << "Runtime error: " << e.what();
			return false;
		}
	}

	// Paradoxium ability
	if (cKeyboardController->IsKeyPressed(GLFW_KEY_ENTER))
	{
		++m_FrameStorage.iCounter;
		switch (m_FrameStorage.iCounter)
		{
		case 1:
			m_FrameStorage.iStoredFrame = m_FrameStorage.iCurrentFrame;
			m_FrameStorage.spawnPos = cPlayer2D->vTransform;
			break;
		case 2:
			CPlayer2D* clone = CEntityManager::GetInstance()->Clone();
			clone->vTransform = m_FrameStorage.spawnPos;
			clone->iTempFrameCounter = m_FrameStorage.iStoredFrame;
			clone->iFrameCounterEnd = m_FrameStorage.iCurrentFrame;
			m_FrameStorage.iStoredFrame = 0;
			cPlayer2D->vTransform = m_FrameStorage.spawnPos;

			m_FrameStorage.iCounter = 0;
			break;
		}
	}

	//Camera work
	cameraHandler->UpdateTarget(cPlayer2D->vTransform);
	cameraHandler->Update((float)dElapsedTime);
	cameraHandler->ClampCamPos(cMap2D->GetLevelLimit());

	// Check if the game should go to the next level
	if (cGameManager->bLevelCompleted == true)
	{
		if (cMap2D->GetCurrentLevel() + 1 < 3)
		{
			cMap2D->SetCurrentLevel(cMap2D->GetCurrentLevel() + 1);
			int initialsize = enemyVector.size();
			for (int i = 0; i < initialsize; i++)
			{
				delete enemyVector[0];
				enemyVector.erase(enemyVector.begin());
			}
			cPlayer2D->Reset();

			//300
			LoadEnemy<CEnemy2D>();
		}
		//Last Level
		else
		{
			cGameManager->bPlayerWon = true;
		}
		cGameManager->bLevelCompleted = false;
	}

	// Check if the game has been won by the player
	if (cGameManager->bPlayerWon == true)
	{
		// End the game and switch to Win screen
		isCompleted = true;
	}
	// Check if the game should be ended
	else if (cGameManager->bPlayerLost == true)
	{
		cSoundController->PlaySoundByID(SOUND_ID::SOUND_EXPLOSION);
		return false;
	}
	return true;
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CScene2D::PreRender(void)
{
	// Reset the OpenGL rendering environment
	glLoadIdentity();

	// Clear the screen and buffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// Enable 2D texture rendering
	glEnable(GL_TEXTURE_2D);
}

/**
 @brief Render Render this instance
 */
void CScene2D::Render(void)
{
	//Call the Map's background (If there's any)
	cMap2D->RenderBackground();

	// Call the Map2D's PreRender()
	cMap2D->PreRender();
	// Call the Map2D's Render()
	cMap2D->Render();
	// Call the Map2D's PostRender()
	cMap2D->PostRender();

	cEntityManager->RenderInteractables();
	cEntityManager->RenderBullets();
	cEntityManager->RenderEnemy();
	cEntityManager->RenderClone();
	cEntityManager->RenderPlayer();
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CScene2D::PostRender(void)
{
}



