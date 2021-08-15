/**
 CSettings
 By: Toh Da Jun
 Date: Mar 2020
 */
#pragma once

 // Include SingletonTemplate
#include "../DesignPatterns/SingletonTemplate.h"

#include <Windows.h>

// Include GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#include <GL/glew.h>
#endif
// Include GLFW
#include <GLFW/glfw3.h>

class CSettings : public CSingletonTemplate<CSettings>
{
	friend CSingletonTemplate<CSettings>;
public:
	enum AXIS
	{
		x = 0,
		y = 1,
		z = 2,
		NUM_AXIS
	};

	// File information
	char* logl_root;	// Root directory for loading digital assets

	// GLFW Information
	GLFWwindow* pWindow;
	bool bUse4XAntiliasing = true;

	// Windows Information
	enum SCREENSIZE
	{
		SSIZE_800x600 = 0,
		SSIZE_1024x768,
		SSIZE_1400x1050,
		SSIZE_1600x1200,
		SSIZE_TOTAL
	};

	SCREENSIZE screenSize;
	// Should make these not hard-coded :P - You Right B)
	unsigned int iWindowWidth;
	unsigned int iWindowHeight;
	unsigned int iWindowPosX = 50;
	unsigned int iWindowPosY = 50;
	bool bDisableMousePointer = false;
	bool bShowMousePointer = true;

	// Frame Rate Information
	const unsigned char FPS = 60; // FPS of this game
	const unsigned int frameTime = 1000 / FPS; // time for each frame

	// Sound Control
	float MASTER_VOLUME;
	float BGM_VOLUME;
	float SFX_VOLUME;

	bool bBGM_Sound;
	bool bSFX_Sound;

	// 2D Settings
	// The variables which stores the specifications of the map
	unsigned int NUM_TILES_XAXIS;
	unsigned int NUM_TILES_YAXIS;

	float TILE_WIDTH;
	float TILE_HEIGHT;

	float NUM_STEPS_PER_TILE_XAXIS;
	float NUM_STEPS_PER_TILE_YAXIS;

	float MICRO_STEP_XAXIS;
	float MICRO_STEP_YAXIS;

	// Update the specifications of the map
	void UpdateSpecifications(void);

	void UpdateWindowSize();

	// Convert an index number of a tile to a coordinate in UV Space
	float ConvertIndexToUVSpace(const AXIS sAxis, const int iIndex, const bool bInvert, const float fOffset = 0.0f);

protected:
	// Constructor
	CSettings(void);

	// Destructor
	virtual ~CSettings(void);
};

