/**
 CGUI_Scene2D
 By: Toh Da Jun
 Date: May 2021
 */
#include "GUI_Scene2D.h"

#include <iostream>
using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene2D::CGUI_Scene2D(void)
	: cSettings(NULL)
	, m_fProgressBar(0.0f)
	, cInventoryManager(NULL)
	, cInventoryItem(NULL)
{
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CGUI_Scene2D::~CGUI_Scene2D(void)
{
	if (cInventoryManager)
	{
		cInventoryManager->Destroy();
		cInventoryManager = NULL;
	}

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

	// Initialise the cInventoryManager
	cInventoryManager = CInventoryManager::GetInstance();


	// Add a Tree as one of the inventory items
	cInventoryItem = cInventoryManager->Add("Shuriken", "Image/Collectibles/shuriken.png", 999, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);
	cInventoryItem = cInventoryManager->Add("Potion", "Image/items/potion.png", 2, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);
	cInventoryItem = cInventoryManager->Add("Hook", "Image/items/grappling_hook.png", 2, 0);
	cInventoryItem->vec2Size = glm::vec2(25, 25);

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
	// Calculate the relative scale to our default windows width
	float relativeScale_x = cSettings->iWindowWidth / 800.0f;
	float relativeScale_y = cSettings->iWindowHeight / 600.0f;
	
	relativeScale_x = Math::Max(1.f, relativeScale_x);
	relativeScale_y = Math::Max(1.f, relativeScale_y);

	if (iSeconds == 60)
	{
		iMinutes += 1;
		iSeconds = 0;
		fInterval = 0;
	}

	fInterval++;
	iSeconds = fInterval / 110;

	// Create an invisible window which covers the entire OpenGL window
	ImGui::Begin("Invisible window", NULL, window_flags);
	ImGui::SetWindowPos(ImVec2(0.0f, 0.0f));
	ImGui::SetWindowSize(ImVec2((float)cSettings->iWindowWidth, (float)cSettings->iWindowHeight));
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);

	// Display the FPS
	/*ImGui::TextColored(ImVec4(1, 1, 0, 1), "FPS: %d", cFPSCounter->GetFrameRate());*/
	ImGui::TextColored(ImVec4(1, 1, 1, 1),"Timer = %d : %d",iMinutes,iSeconds);

	// Render the inventory items
	cInventoryItem = cInventoryManager->GetItem("Shuriken");
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));  // Set a background color
	ImGuiWindowFlags inventoryWindowFlags = ImGuiWindowFlags_AlwaysAutoResize |
		ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoScrollbar;
	ImGui::Begin("Shuriken", NULL, inventoryWindowFlags);
	ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.03f, cSettings->iWindowHeight * 0.92f));
	ImGui::SetWindowSize(ImVec2(200.0f * relativeScale_x, 25.0f * relativeScale_y));
	ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
		ImVec2(cInventoryItem->vec2Size.x * relativeScale_x, cInventoryItem->vec2Size.y * relativeScale_y),
		ImVec2(0, 1), ImVec2(1, 0));
	ImGui::SameLine();
	ImGui::SetWindowFontScale(1.5f * relativeScale_y);
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Shuriken: %d", cInventoryItem->GetCount());
	ImGui::End();
	ImGui::PopStyleColor();


	//potion 1
	{
		////if selected
		cInventoryItem = cInventoryManager->GetItem("Potion");
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
		//ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.f, 0.f, 0.f, 1.f));
		//ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.f, 0.f, 0.f, 1.f));
		if (cInventoryManager->GetItemIndex() == 1)
		{
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.f, 0.f, 0.f, 1.f));
		}
		ImGui::Begin("Testing : ", NULL, inventoryWindowFlags);
		ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.01f, cSettings->iWindowHeight * 0.065f));
		ImGui::SetWindowSize(ImVec2(200.0f * relativeScale_x, 25.0f * relativeScale_y));
		ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
			ImVec2(cInventoryItem->vec2Size.x * relativeScale_x, cInventoryItem->vec2Size.y * relativeScale_y),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::SetWindowFontScale(1.5f * relativeScale_y);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Potion: %d",cInventoryItem->GetCount());
		ImGui::End();
		ImGui::PopStyleColor();
		if (cInventoryManager->GetItemIndex() == 1)
		{
			ImGui::PopStyleColor();
		}

	}

	//hook
	{
		cInventoryItem = cInventoryManager->GetItem("Hook");
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 1.0f, 1.0f));
		//ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.f, 0.f, 0.f, 1.f));
		if (cInventoryManager->GetItemIndex() == 2)
		{
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1.f, 0.f, 0.f, 1.f));
		}
		ImGui::Begin("Testing2 : ", NULL, inventoryWindowFlags);
		ImGui::SetWindowPos(ImVec2(cSettings->iWindowWidth * 0.01f, cSettings->iWindowHeight * 0.140f));
		ImGui::SetWindowSize(ImVec2(200.0f * relativeScale_x, 60.0f * relativeScale_y));
		ImGui::Image((void*)(intptr_t)cInventoryItem->GetTextureID(),
			ImVec2(cInventoryItem->vec2Size.x * relativeScale_x, cInventoryItem->vec2Size.y * relativeScale_y),
			ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();
		ImGui::SetWindowFontScale(1.9f * relativeScale_y);
		ImGui::TextColored(ImVec4(1, 1, 0, 1), "Hook: %d", cInventoryItem->GetCount());
		ImGui::End();
		ImGui::PopStyleColor();
		if (cInventoryManager->GetItemIndex() == 2)
		{
			ImGui::PopStyleColor();
		}
	}

	ImGui::End();
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
