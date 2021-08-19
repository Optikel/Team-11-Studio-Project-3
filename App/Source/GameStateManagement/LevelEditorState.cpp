// Include GLEW
#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

// Include GLFW
#include <GLFW/glfw3.h>

#include "LevelEditorState.h"

// Include CGameStateManager
#include "GameStateManager.h"

// Include CKeyboardController
#include "Inputs/KeyboardController.h"

#include "System/Debug.h"

#include "Math/MyMath.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor
 */
CLevelEditorState::CLevelEditorState(void)
	: cKeyboardInputHandler(NULL)
	, cLevelEditor(NULL)
	, cMouseController(NULL)
	, cSettings(NULL)
	, cLevelGrid(NULL)
{
}

/**
 @brief Destructor
 */
CLevelEditorState::~CLevelEditorState(void)
{
	cKeyboardInputHandler = NULL;
	cLevelEditor = NULL;
	cMouseController = NULL;
	cSettings = NULL;

	if (cLevelGrid)
	{ 
		cLevelGrid->Destroy();
		cLevelGrid = NULL;
	}

	Destroy();
}

/**
 @brief Init this class instance
 */
bool CLevelEditorState::Init(void)
{
	cout << "CLevelEditorState::Init()\n" << endl;

	cSettings = CSettings::GetInstance();

	cLevelEditor = CLevelEditor::GetInstance();
	cLevelGrid = CLevelGrid::GetInstance();
	cLevelGrid->Init(cLevelEditor->iWorldWidth, cLevelEditor->iWorldHeight);

	Camera2D::GetInstance()->Reset();
	Camera2D::GetInstance()->UpdateTarget(glm::vec2(cSettings->NUM_TILES_XAXIS * 0.5, cSettings->NUM_TILES_YAXIS * 0.5));

	cMouseController = CMouseController::GetInstance();

	cKeyboardInputHandler = CKeyboardInputHandler::GetInstance();
	cKeyboardInputHandler->Init();

	return true;
}

/**
 @brief Update this class instance
 */
bool CLevelEditorState::Update(const double dElapsedTime)
{
	Camera2D::GetInstance()->Update(dElapsedTime);

	if (CKeyboardController::GetInstance()->IsKeyReleased(GLFW_KEY_ESCAPE))
	{
		// Reset the CKeyboardController
		CKeyboardController::GetInstance()->Reset();

		// Load the menu state
		cout << "Loading PauseState" << endl;
		CGameStateManager::GetInstance()->SetPauseGameState("PauseState");
		return true;
	}

	MoveCamera();
	ScaleMap();
	MouseInput();

	return true;
}

/**
 @brief Render this class instance
 */
void CLevelEditorState::Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	cLevelEditor->PreRender();
	cLevelEditor->Render();
	cLevelEditor->PostRender();

	cLevelGrid->PreRender();
	cLevelGrid->Render();
	cLevelGrid->PostRender();

}

/**
 @brief Destroy this class instance
 */
void CLevelEditorState::Destroy(void)
{
	cout << "CLevelEditorState::Destroy()\n" << endl;
}

bool CLevelEditorState::CreateIMGUI()
{
	return false;
}

bool CLevelEditorState::DeleteIMGUI()
{
	return false;
}

void CLevelEditorState::MoveCamera(void)
{
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_W))
	{
		Camera2D::GetInstance()->UpdateTarget(glm::vec2(Camera2D::GetInstance()->getTarget().x, Camera2D::GetInstance()->getTarget().y + 0.2));
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_S))
	{
		Camera2D::GetInstance()->UpdateTarget(glm::vec2(Camera2D::GetInstance()->getTarget().x, Camera2D::GetInstance()->getTarget().y - 0.2));
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_A))
	{
		Camera2D::GetInstance()->UpdateTarget(glm::vec2(Camera2D::GetInstance()->getTarget().x - 0.2, Camera2D::GetInstance()->getTarget().y));
	}
	if (CKeyboardController::GetInstance()->IsKeyDown(GLFW_KEY_D))
	{
		Camera2D::GetInstance()->UpdateTarget(glm::vec2(Camera2D::GetInstance()->getTarget().x + 0.2, Camera2D::GetInstance()->getTarget().y));
	}

}

void CLevelEditorState::ScaleMap(void)
{
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_RIGHT))
	{
		if (cLevelEditor->IncreaseXSize())
			cLevelGrid->iWorldWidth++;
	}
	else if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_LEFT))
	{
		if (cLevelEditor->DecreaseXSize())
			cLevelGrid->iWorldWidth--;
	}
	if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_UP))
	{
		if (cLevelEditor->IncreaseYSize())
			cLevelGrid->iWorldHeight++;
	}
	else if (CKeyboardController::GetInstance()->IsKeyPressed(GLFW_KEY_DOWN))
	{
		if (cLevelEditor->DecreaseYSize())
			cLevelGrid->iWorldHeight--;
	}

}

void CLevelEditorState::MouseInput(void)
{
	glm::vec2 vMousePosInWindow = glm::vec2(cMouseController->GetMousePositionX(), cSettings->iWindowHeight - cMouseController->GetMousePositionY());
	glm::vec2 vMousePosConvertedRatio = glm::vec2(vMousePosInWindow.x - cSettings->iWindowWidth * 0.5, vMousePosInWindow.y - cSettings->iWindowHeight * 0.5);
	glm::vec2 vMousePosWorldSpace = glm::vec2(vMousePosConvertedRatio.x / cSettings->iWindowWidth * cSettings->NUM_TILES_XAXIS, vMousePosConvertedRatio.y / cSettings->iWindowHeight * cSettings->NUM_TILES_YAXIS);
	glm::vec2 vMousePosRelativeToCamera = Camera2D::GetInstance()->getCurrPos() + vMousePosWorldSpace / Camera2D::GetInstance()->getZoom();

	vMousePosRelativeToCamera.x = Math::Clamp(vMousePosRelativeToCamera.x, 0.f, (float)cLevelEditor->iWorldWidth - 1.f);
	vMousePosRelativeToCamera.y = Math::Clamp(vMousePosRelativeToCamera.y, 0.f, (float)cLevelEditor->iWorldHeight - 1.f);

	cout << vMousePosRelativeToCamera.x << ", " << vMousePosRelativeToCamera.y << endl;

	vMousePosRelativeToCamera.x = (int)(vMousePosRelativeToCamera.x);
	vMousePosRelativeToCamera.y = (int)(vMousePosRelativeToCamera.y);

	if (cMouseController->IsButtonDown(CMouseController::LMB))
	{
		// DEBUG_MSG("x:" << u16vec2FinalMousePosInEditor.x << " y:" << u16vec2FinalMousePosInEditor.y);
		DEBUG_MSG("[x: " << vMousePosRelativeToCamera.x << ", y: " << vMousePosRelativeToCamera.y << "] Cell TileID: " << cLevelEditor->GetCell(vMousePosRelativeToCamera.x, vMousePosRelativeToCamera.y, false).iTileID);
		if (cLevelEditor->GetCell(vMousePosRelativeToCamera.x, vMousePosRelativeToCamera.y, false).iTileID == 0)
		{
			cLevelEditor->UpdateCell(vMousePosRelativeToCamera.x, vMousePosRelativeToCamera.y, 100, false);
		}
	}

	if (cMouseController->IsButtonDown(CMouseController::RMB))
	{
		// DEBUG_MSG("x:" << u16vec2FinalMousePosInEditor.x << " y:" << u16vec2FinalMousePosInEditor.y);
		if (cLevelEditor->GetCell(vMousePosRelativeToCamera.x, vMousePosRelativeToCamera.y, false).iTileID != 0)
		{
			cLevelEditor->UpdateCell(vMousePosRelativeToCamera.x, vMousePosRelativeToCamera.y, 0, false);
		}
	}

	if (cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE_YOFFSET) > 0)
	{
		Camera2D::GetInstance()->UpdateZoom(Camera2D::GetInstance()->getTargetZoom() + 0.1);
	}
	if (cMouseController->GetMouseScrollStatus(CMouseController::SCROLL_TYPE_YOFFSET) < 0)
	{
		Camera2D::GetInstance()->UpdateZoom(Camera2D::GetInstance()->getTargetZoom() - 0.1);
	}
}