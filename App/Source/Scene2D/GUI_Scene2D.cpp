/**
 CGUI_Scene2D
 By: Toh Da Jun
 Date: May 2021
 */
#include "GUI_Scene2D.h"
#include "../TextureManager/TextureManager.h"

#include <iostream>
#include <sstream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene2D::CGUI_Scene2D(void)
	: cSettings(NULL)
	, m_fProgressBar(0.0f)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene2D::~CGUI_Scene2D(void)
{
	//if (cInventoryManager)
	//{
	//	cInventoryManager->Destroy();
	//	cInventoryManager = NULL;
	//}

	// Show the mouse pointer
	if (cSettings->bDisableMousePointer == true)
		glfwSetInputMode(cSettings->pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		// Hide the cursor
		if (cSettings->bShowMousePointer == false)
			glfwSetInputMode(cSettings->pWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// We won't delete this since it was created elsewhere
	cSettings = NULL;
}

/**
  @brief Initialise this instance
  */
bool CGUI_Scene2D::Init(void)
{
	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Store the CFPSCounter singleton instance here
	cFPSCounter = CFPSCounter::GetInstance();



	// Define the window flags
	window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	//// Show the mouse pointer
	//glfwSetInputMode(CSettings::GetInstance()->pWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	cPlayerInventory = CInventoryManager::GetInstance()->Get("Player");

	
	//cInventoryMain = CInventoryMain::GetInstance();
	cTextureManager = CTextureManager::GetInstance();
	cTextureManager->Init();


	

	//// Add a Tree as one of the inventory items
	//cInventoryItem = cInventoryManager->Add("Shuriken", "Image/Collectibles/shuriken.png", 999, 0);
	//cInventoryItem->vec2Size = glm::vec2(25, 25);
	//cInventoryItem = cInventoryManager->Add("Potion", "Image/items/potion.png", 2, 0);
	//cInventoryItem->vec2Size = glm::vec2(25, 25);
	//cInventoryItem = cInventoryManager->Add("Hook", "Image/items/grappling_hook.png", 2, 0);
	//cInventoryItem->vec2Size = glm::vec2(25, 25);

	// Add a Lives icon as one of the inventory items
	// cInventoryItem = cInventoryManager->Add("Lives", "Image/Collectibles/Scene2D_Lives.tga", 5, 3);
	// cInventoryItem->vec2Size = glm::vec2(25, 25);

	// Add a Health icon as one of the inventory items
	// cInventoryItem = cInventoryManager->Add("Health", "Image/Scene2D_Health.tga", 100, 100);
	// cInventoryItem->vec2Size = glm::vec2(25, 25);

	// Add a Tree as one of the inventory items
	// cInventoryItem = cInventoryManager->Add("Shuriken", "Image/Collectibles/shuriken.png", 999, 5);
	// cInventoryItem->vec2Size = glm::vec2(25, 25);
	// cInventoryItem = cInventoryManager->Add("Potion", "Image/items/potion.png", 2, 0);
	// cInventoryItem->vec2Size = glm::vec2(25, 25);
	// cInventoryItem = cInventoryManager->Add("Hook", "Image/items/grappling_hook.png", 2, 0);
	// cInventoryItem->vec2Size = glm::vec2(25, 25);


	fInterval = 0;
	iMinutes = 0;
	iSeconds = 0;


	return true;
}

/**
 @brief Update this instance
 */
void CGUI_Scene2D::Update(const double dElapsedTime)
{
	
}


/**
 @brief Set up the OpenGL display environment before rendering
 */
void CGUI_Scene2D::PreRender(void)
{
}

/**
 @brief Render this instance
 */
void CGUI_Scene2D::Render(void)
{
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CGUI_Scene2D::PostRender(void)
{
}
