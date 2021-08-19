#pragma once

/**
 CLevelEditorState
 @brief This class is derived from CGameState. It will introduce the game to the player.
 By: Toh Da Jun
 Date: July 2021
 */

#include "GameStateBase.h"
#include "../KeyboardInputHandler/CKeyboardInputHandler.h"
#include "../LevelEditor/LevelEditor.h"
#include "../LevelEditor/LevelGrid.h"
#include "Inputs/MouseController.h"

#ifndef IMGUI_ACTIVE
#include "GUI\imgui.h"
#include "GUI\backends\imgui_impl_glfw.h"
#include "GUI\backends\imgui_impl_opengl3.h"
#define IMGUI_ACTIVE
#endif

class CLevelEditorState : public CGameStateBase
{
public:
	// Constructor
	CLevelEditorState(void);
	// Destructor
	~CLevelEditorState(void);

	// Init this class instance
	virtual bool Init(void);
	// Update this class instance
	virtual bool Update(const double dElapsedTime);
	// Render this class instance
	virtual void Render(void);
	// Destroy this class instance
	virtual void Destroy(void);

protected:
	glm::mat4 transform;

	glm::vec2 vMousePosInWindow;
	glm::vec2 vMousePosConvertedRatio;
	glm::vec2 vMousePosWorldSpace;
	glm::vec2 vMousePosRelativeToCamera;

	unsigned FBO, RBO, textureColorBuffer;
	unsigned int quadVAO, quadVBO;

	CMesh* cursor;

	CSettings*					cSettings;

	CKeyboardInputHandler*		cKeyboardInputHandler;
	CMouseController*			cMouseController;
	CLevelEditor*				cLevelEditor;
	CLevelGrid*					cLevelGrid;

	int activeTile;

	void MoveCamera(void);
	void ScaleMap(void);
	void MouseInput(void);

	void GenerateQuadVAO(void);
	void GenerateFBO(void);
	void RenderQuad(unsigned int iTextureID);
	void ImGuiRender(void);
	void RenderCursor(void);
	void CalculateMousePosition(void);
};
