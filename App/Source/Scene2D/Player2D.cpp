﻿/**
 Player2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Player2D.h"

#include <iostream>
using namespace std;

// Include Shader Manager
#include "RenderControl\ShaderManager.h"

// Include ImageLoader
#include "System\ImageLoader.h"

// Include the Map2D as we will use it to check the player's movements and actions
#include "Map2D.h"
#include "Primitives/MeshBuilder.h"

// Include Game Manager
#include "GameManager.h"

#include "Camera2D.h"

#include "Math/MyMath.h"

//Interactables
#include "Boulder2D.h"

//Items
#include "Projectiles.h"


/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CPlayer2D::CPlayer2D(void)
	: cMap2D(NULL)
	, cKeyboardController(NULL)
	, cMouseController(NULL)
	, cInventoryManager(NULL)
	, cInventoryItem(NULL)
	, cSoundController(NULL)
	, cKeyboardInputHandler(NULL)
	, iTempFrameCounter(0)
	//, bDamaged(false)
	, bIsClone(false)

{
	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first

	// Initialise vecIndex
	vTransform = glm::i32vec2(0);

	type = PLAYER;

	animatedSprites = nullptr;
	camera = nullptr;
	checkpoint  = glm::i32vec2();
	currentColor = glm::vec4();
}

CPlayer2D::CPlayer2D(string cloneName) : CEntity2D() {
	
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CPlayer2D::~CPlayer2D(void)
{
	// We won't delete this since it was created elsewhere
	cSoundController = NULL;

	// We won't delete this since it was created elsewhere
	// cInventoryManager = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardController = NULL;

	// We won't delete this since it was created elsewhere
	cKeyboardInputHandler = NULL;

	// We won't delete this since it was created elsewhere
	cMap2D = NULL;

	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
}

/**
  @brief Initialise this instance
  */
bool CPlayer2D::Init(void)
{
	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();
	cKeyboardController->Reset();

	cMouseController = CMouseController::GetInstance();

	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Get the handler to the CMap2D instance
	cMap2D = CMap2D::GetInstance();
	// Find the indices for the player in arrMapInfo, and assign it to cPlayer2D
	unsigned int uiRow = -1;
	unsigned int uiCol = -1;
	if (cMap2D->FindValue(1, uiRow, uiCol) == false)
		return false;	// Unable to find the start position of the player, so quit this game

	// Erase the value of the player in the arrMapInfo
	cMap2D->SetMapInfo(uiRow, uiCol, 0);

	// Set checkpoint position to start position
	checkpoint = glm::vec2(uiCol, uiRow);
	// Set the start position of the Player to iRow and iCol
	vTransform = glm::vec2(uiCol, uiRow);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Load the player texture
	if (LoadTexture("Image/Cyborg/Cyborg.png", iTextureID) == false)
	{
		std::cout << "Failed to load player tile texture" << std::endl;
		return false;
	}
	
	state = S_IDLE;
	facing = RIGHT;
	//CS: Create the animated sprite and setup the animation 
	animatedSprites = CMeshBuilder::GenerateSpriteAnimation(10, 6, cSettings->TILE_WIDTH, cSettings->TILE_HEIGHT);
	animatedSprites->AddAnimation("run", 0, 6);
	animatedSprites->AddAnimation("idle", 6, 10);
	animatedSprites->AddAnimation("jump", 12, 16);
	animatedSprites->AddAnimation("double_jump", 18, 24);
	animatedSprites->AddAnimation("death", 24, 30);
	animatedSprites->AddAnimation("attack", 36, 42);
	animatedSprites->AddAnimation("climb", 48, 54);
	animatedSprites->AddAnimation("hit", 54, 56);
	//CS: Play the "idle" animation as default
	animatedSprites->PlayAnimation("idle", -1, 1.0f);

	//CS: Init the color to white
	currentColor = glm::vec4(1.0, 1.0, 1.0, 1.0);

	// Get the handler to the CInventoryManager instance
    cInventoryManager = CInventoryManager::GetInstance();

	jumpCount = 0;

	camera = Camera2D::GetInstance();

	fMovementSpeed = 5.f;
	fJumpSpeed = 5.f;

	//fMovementSpeed = 1.f;
	//fJumpSpeed = 1.f;

	// Get the handler to the CSoundController
	cSoundController = CSoundController::GetInstance();

	cKeyboardInputHandler = CKeyboardInputHandler::GetInstance();

	collider2D->vec2Dimensions = glm::vec2(0.20000f,0.50000f);
	collider2D->Init();

	cPhysics2D.Init(&vTransform);
	return true;
}

bool CPlayer2D::Init(glm::i32vec2 spawnpoint)
{
	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();
	// Reset all keys since we are starting a new game
	cKeyboardController->Reset();

	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Get the handler to the CMap2D instance
	cMap2D = CMap2D::GetInstance();

	// Erase the value of the player in the arrMapInfo
	//cMap2D->SetMapInfo(uiRow, uiCol, 0);

	// Set checkpoint position to start position
	checkpoint = spawnpoint;
	// Set the start position of the Player to iRow and iCol
	vTransform = spawnpoint;

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Load the player texture
	if (LoadTexture("Image/Cyborg/Cyborg.png", iTextureID) == false)
	{
		std::cout << "Failed to load player tile texture" << std::endl;
		return false;
	}

	state = S_IDLE;
	facing = RIGHT;
	//CS: Create the animated sprite and setup the animation 
	animatedSprites = CMeshBuilder::GenerateSpriteAnimation(10, 6, cSettings->TILE_WIDTH, cSettings->TILE_HEIGHT);
	animatedSprites->AddAnimation("run", 0, 6);
	animatedSprites->AddAnimation("idle", 6, 10);
	animatedSprites->AddAnimation("jump", 12, 16);
	animatedSprites->AddAnimation("double_jump", 18, 24);
	animatedSprites->AddAnimation("death", 24, 30);
	animatedSprites->AddAnimation("attack", 36, 42);
	animatedSprites->AddAnimation("climb", 48, 54);
	animatedSprites->AddAnimation("hit", 54, 56);
	//CS: Play the "idle" animation as default
	animatedSprites->PlayAnimation("idle", -1, 1.0f);

	//CS: Init the color to white
	currentColor = glm::vec4(1.0, 1.0, 1.0, 1.0);

	jumpCount = 0;

	camera = Camera2D::GetInstance();

	fMovementSpeed = 5.f;
	fJumpSpeed = 5.f;

	// Get the handler to the CSoundController
	cSoundController = CSoundController::GetInstance();

	cKeyboardInputHandler = CKeyboardInputHandler::GetInstance();

	collider2D->Init();
	collider2D->SetPosition(vTransform);

	cPhysics2D.Init(&vTransform);

	return true;
}

glm::i32vec2 CPlayer2D::GetCheckpoint(void) {
	return checkpoint;
}

/**
 @brief Reset this instance
 */
bool CPlayer2D::Reset()
{
	unsigned int uiRow = -1;
	unsigned int uiCol = -1;
	if (cMap2D->FindValue(1, uiRow, uiCol) == false)
		return false;	// Unable to find the start position of the player, so quit this game

	// Erase the value of the player in the arrMapInfo
	cMap2D->SetMapInfo(uiRow, uiCol, 0);

	// Set checkpoint to start position
	checkpoint = glm::i32vec2(uiCol, uiRow);
	// Set the start position of the Player to iRow and iCol
	vTransform = glm::i32vec2(uiCol, uiRow);

	//CS: Reset double jump
	jumpCount = 0;

	//CS: Play the "idle" animation as default
	animatedSprites->PlayAnimation("idle", -1, 1.0f);

	//CS: Init the color to white
	currentColor = glm::vec4(1.0, 1.0, 1.0, 1.0);

	return true;
}

/**
 @brief Update this instance
 */
void CPlayer2D::Update(const double dElapsedTime)
{
	// Store the old position
	vOldTransform = vTransform;

	// Get keyboard & Mouse updates
	InputUpdate(dElapsedTime);
	
	cPhysics2D.Update(dElapsedTime);

	// Update Collider2D Position
	collider2D->position = vTransform;

	//COLLISION RESOLUTION ON Y_AXIS AND X_AXIS
	int range = 3;
	cPhysics2D.SetboolGrounded(false);

	//Stores nearby objects and its dist to player into a vector 
	vector<pair<CObject2D*, float>> aabbVector;
	for (int i = 0; i < 2; i++)
	{
		for (int row = -range; row <= range; row++) //y
		{
			for (int col = -range; col <= range; col++) //x
			{
				int rowCheck = vTransform.y + row;
				int colCheck = vTransform.x + col;

				if (rowCheck < 0 || colCheck < 0 || rowCheck > cMap2D->GetLevelRow() - 1 || colCheck > cMap2D->GetLevelCol() - 1) continue;
				if (cMap2D->GetCObject(colCheck, rowCheck))
				{
					CObject2D* obj = cMap2D->GetCObject(colCheck, rowCheck);
					float distance = glm::length( obj->vTransform - vTransform );
					aabbVector.push_back({obj, distance });
				}
			}
		}
	}
	//Sorts vector based on shortest dist from player to object
	sort(aabbVector.begin(), aabbVector.end(), [](const std::pair<CObject2D*, float>& a, const std::pair<CObject2D*, float>& b)
	{
		return a.second < b.second;
	});

	// Detects and Resolves Collsion
	for (auto aabbTuple : aabbVector)
	{
		CObject2D* obj = aabbTuple.first;
		Collision data = (collider2D->CollideWith(obj->GetCollider()));
		if (std::get<0>(data))
		{
			if (obj->GetCollider()->colliderType == Collider2D::COLLIDER_QUAD)
			{
				collider2D->ResolveAABB(obj->GetCollider(), Direction::UP);
				collider2D->ResolveAABB(obj->GetCollider(), Direction::RIGHT);

				if (std::get<1>(data) == Direction::UP)
					cPhysics2D.SetboolGrounded(true);
			}
			else if (obj->GetCollider()->colliderType == Collider2D::COLLIDER_CIRCLE)
			{
				if (glm::dot(cPhysics2D.GetVelocity(), obj->vTransform - vTransform) > 0)
					collider2D->ResolveAABBCircle(obj->GetCollider(), data, Collider2D::COLLIDER_QUAD);

				if (std::get<1>(data) == Direction::DOWN)
					cPhysics2D.SetboolGrounded(true);
			}

			vTransform = collider2D->position;
			obj->vTransform = obj->GetCollider()->position;

			if (obj->type == CObject2D::ENTITY_TYPE::INTERACTABLES)
			{
				if (static_cast<Interactables*>(obj)->interactableType == Interactables::INTERACTABLE_TYPE::BOULDER)
				{
					glm::vec2 direction = glm::normalize(obj->vTransform - vTransform);
					static_cast<Boulder2D*>(obj)->GetPhysics().SetForce(glm::vec2(120.f, 0) * direction);
					cPhysics2D.SetVelocity(glm::vec2(0.f));
				}
			}
		}
	}

	
	//BOUNDARY CHECK
	//if (vTransform.y > cMap2D->GetLevelRow() - 1 || vTransform.x > cMap2D->GetLevelCol() - 1 || vTransform.y < -1 || vTransform.x < -1)
	//{
		//Reset to checkpoint option
		//ResetToCheckPoint();
	//}
	LockWithinBoundary();
	//animation States
	switch (state)
	{
	case S_IDLE:
		animatedSprites->PlayAnimation("idle", -1, 1.f);
		break;
	case S_MOVE:
		animatedSprites->PlayAnimation("run", -1, 0.6f);
		break;
	case S_JUMP:
		animatedSprites->PlayAnimation("jump", 1, 0.6f);
		break;
	case S_DOUBLE_JUMP:
		animatedSprites->PlayAnimation("double_jump", 1, 0.4f);
		break;
	case S_HOLD:
		animatedSprites->PlayAnimation("idle", -1, 1.2f);
		break;
	case S_ATTACK:
		animatedSprites->PlayAnimation("attack", 1, 0.6f);
		break;
	case S_CLIMB:
		animatedSprites->PlayAnimation("climb", 1, 1.f);
		break;
	case S_DEATH:
		animatedSprites->PlayAnimation("death", 1, 3.f);
		break;
	}

	//CS: Update the animated sprite
	animatedSprites->Update(dElapsedTime);

	iTempFrameCounter++;
}

/**
 @brief Set up the OpenGL display environment before rendering
 */
void CPlayer2D::PreRender(void)
{
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Activate the shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render this instance
 */
void CPlayer2D::Render(void)
{
	glBindVertexArray(VAO);
	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "transform");
	unsigned int colorLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "runtime_color");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
	
	//Get camera transforms and use them instead
	glm::vec2 offset = glm::i32vec2((cSettings->NUM_TILES_XAXIS / 2), (cSettings->NUM_TILES_YAXIS / 2));
	glm::vec2 cameraPos = camera->getCurrPos();

	glm::vec2 IndexPos = vTransform;

	glm::vec2 actualPos = IndexPos - cameraPos + offset;
	actualPos = cSettings->ConvertIndexToUVSpace(actualPos);

	transform = glm::translate(transform, glm::vec3(actualPos.x, actualPos.y, 0.f));

	if (facing == LEFT)
		transform = glm::rotate(transform, glm::radians(180.f), glm::vec3(0.f, 1.f, 0.f));
	// Update the shaders with the latest transform
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
	glUniform4fv(colorLoc, 1, glm::value_ptr(currentColor));

	// Get the texture to be rendered
	glBindTexture(GL_TEXTURE_2D, iTextureID);

	//CS: Render the animated sprite
	animatedSprites->Render();

	glBindVertexArray(0);

}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CPlayer2D::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);
}

CPlayer2D::STATE CPlayer2D::Getstate() const
{
	return state;
}

/**
@brief Load a texture, assign it a code and store it in MapOfTextureIDs.
@param filename A const char* variable which contains the file name of the texture
*/
bool CPlayer2D::LoadTexture(const char* filename, GLuint& iTextureID)
{
	// Variables used in loading the texture
	int width, height, nrChannels;
	
	// texture 1
	// ---------
	glGenTextures(1, &iTextureID);
	glBindTexture(GL_TEXTURE_2D, iTextureID);
	// set the texture wrapping parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	CImageLoader* cImageLoader = CImageLoader::GetInstance();
	unsigned char *data = cImageLoader->Load(filename, width, height, nrChannels, true);
	if (data)
	{
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		return false;
	}
	// Free up the memory of the file data read in
	free(data);

	return true;
}

void CPlayer2D::InputUpdate(double dt)
{
	state = S_IDLE;
	
	std::vector<std::array<bool, KEYBOARD_INPUTS::INPUT_TOTAL>> keyboardInputs = (bIsClone) ? m_CloneKeyboardInputs : cKeyboardInputHandler->GetAllInputs();
	if ((unsigned)iTempFrameCounter >= keyboardInputs.size())
		return;

	glm::vec2 velocity = cPhysics2D.GetVelocity();
	if (keyboardInputs[iTempFrameCounter][KEYBOARD_INPUTS::W])
	{
		velocity.y = fMovementSpeed;
		cPhysics2D.SetboolGrounded(false);
	}
	else if (keyboardInputs[iTempFrameCounter][KEYBOARD_INPUTS::S])
	{
		velocity.y = -fMovementSpeed;
	}
	if (keyboardInputs[iTempFrameCounter][KEYBOARD_INPUTS::D])
	{
		velocity.x = fMovementSpeed;
		state = S_MOVE;
		facing = RIGHT;
	}
	else if (keyboardInputs[iTempFrameCounter][KEYBOARD_INPUTS::A])
	{
		velocity.x = -fMovementSpeed;
		state = S_MOVE;
		facing = LEFT;
	}

	if (keyboardInputs[iTempFrameCounter][KEYBOARD_INPUTS::SPACE])
	{
		velocity.y = fJumpSpeed;
		cPhysics2D.SetboolGrounded(false);
	}

	if (glm::length(velocity) > 0.f)
		cPhysics2D.SetVelocity(velocity);

	if (cKeyboardController->IsKeyPressed(GLFW_KEY_V))
	{
		if (state != S_ATTACK)
		{
			cSoundController->PlaySoundByID(6);
			state = S_ATTACK;
		}
	}

	if (cMouseController->IsButtonPressed(0))
	{
		cInventoryItem = cInventoryManager->GetItem("Shuriken");
		if (cInventoryItem->GetCount() > 0)
		{
			if (cMap2D->InsertMapInfo((int)vTransform.y, (int)vTransform.x, 2))
			{
				glm::vec2 distance = Camera2D::GetInstance()->GetCursorPosInWorldSpace() - vTransform;

				CObject2D* shuriken = cMap2D->GetCObject((int)vTransform.x, (int)vTransform.y);
				shuriken->vTransform = vTransform;

				static_cast<Projectiles*>(shuriken)->GetPhysics().SetForce(distance * 200.f);
				cInventoryItem->Remove(1);
			}
		}
	}
}

/**
 @brief Update the health and lives.
 */
void CPlayer2D::UpdateHealthLives(void)
{
	// Update health and lives
	// Check if a life is lost
	if (cInventoryItem->GetCount() <= 0)
	{
		state = S_DEATH;
		cSoundController->PlaySoundByID(9);

		// Check if there is no lives left...
		if (cInventoryItem->GetCount() < 0)
		{
			// Player loses the game
			CGameManager::GetInstance()->bPlayerLost = true;
		}
	}
}

void CPlayer2D::SetClone(bool bIsClone)
{
	this->bIsClone = bIsClone;
}

bool CPlayer2D::IsClone()
{
	return bIsClone;
}

void CPlayer2D::SetInputs(std::vector<std::array<bool, KEYBOARD_INPUTS::INPUT_TOTAL>> inputs)
{
	m_CloneKeyboardInputs = inputs;
}

void CPlayer2D::ResetToCheckPoint()
{
	vTransform = checkpoint;
	cPhysics2D.SetVelocity(glm::vec2(0.f));
	cKeyboardController->Reset();
}

void CPlayer2D::LockWithinBoundary()
{
	glm::vec2 minVal = glm::vec2(0.5f, 0.f) - glm::vec2(collider2D->vec2Dimensions.x, 0);
	minVal *= -1;

	glm::vec2 mapDimensions = cMap2D->GetLevelLimit();
	glm::vec2 maxVel = mapDimensions - glm::vec2(3.f, 3.f) + glm::vec2(0.5f - collider2D->vec2Dimensions.x, 0);

	vTransform = glm::clamp(vTransform, minVal, maxVel);
	collider2D->SetPosition(vTransform);
}

CPlayer2D* const CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}
