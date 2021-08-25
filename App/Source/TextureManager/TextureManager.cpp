#include "TextureManager.h"

#ifndef GLEW_STATIC
#include <GL/glew.h>
#define GLEW_STATIC
#endif

#include <GLFW/glfw3.h>

#include "System/ImageLoader.h"
#include "../Scene2D/Object2D.h"
#include <iostream>

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
}


bool CTextureManager::Init(void)
{

    if (LoadTexture("Image/Cyborg/Cyborg_Preview.png", PLAYER_TILE) == false)
    {
        std::cout << "Failed to load player tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Ground/GroundTile.png", TILE_GROUND) == false)
    {
        std::cout << "Failed to load ground tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/Ground/GrassTile.png", TILE_GRASS) == false)
    {
        std::cout << "Failed to load GrassTile tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/Ground/LeftGrassTile.png", TILE_LEFT_GRASS) == false)
    {
        std::cout << "Failed to load LeftGrassTile tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/Ground/RightGrassTile.png", TILE_RIGHT_GRASS) == false)
    {
        std::cout << "Failed to load RightGrassTile tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/Ground/TopRightGroundTile.png", TILE_TOP_RIGHT_GROUND) == false) {
        std::cout << "Failed to load TopRightGrassTile tile texture" << std::endl;
    }

    if (LoadTexture("Image/Walls/wall_1.png", TILE_WALL_1) == false)
    {
        std::cout << "Failed to load wall_1 tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/Walls/wall_2.png", TILE_WALL_2) == false)
    {
        std::cout << "Failed to load wall_2 tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/Boulder.png", OBSTACLE_BOULDER) == false)
    {
        std::cout << "Failed to load Boulder tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Collectibles/shuriken.png", PROJECTILES_SHURIKEN) == false)
    {
        std::cout << "Failed to load shuriken tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/items/potion.png", CONSUMABLES_POTION) == false)
    {
        std::cout << "Failed to load potion tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/items/grappling_hook.png", EQUIPMENTS_HOOK) == false)
    {
        std::cout << "Failed to load grappling hook tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/Collectibles/kunai.png", BULLETS_KUNAI) == false)
    {
        std::cout << "Failed to load kunai tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Collectibles/Scene2D_Lives.tga", UI_LIVES) == false)
    {
        std::cout << "Failed to load Scene2D_Lives tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/Switches/switch_1_off.png", INTERACTABLE_SWITCH1_OFF) == false)
    {
        std::cout << "Failed to load switch_1_off tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/Switches/switch_1_on.png", INTERACTABLE_SWITCH1_ON) == false)
    {
        std::cout << "Failed to load switch_1_on tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Switches/pressureplate_1_off.png", INTERACTABLE_PRESSURE_PLATE_OFF) == false)
    {
        std::cout << "Failed to load switch_2_off tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/Switches/pressureplate_1_on.png", INTERACTABLE_PRESSURE_PLATE_ON) == false)
    {
        std::cout << "Failed to load switch_2_on tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Portals/portal.png", INTERACTABLE_PORTAL) == false)
    {
        std::cout << "Failed to load portal15 tile texture" << std::endl;
        return false;
    }


    if (LoadTexture("Image/Portals/portalgate.png", INTERACTABLE_PORTAL_GATE) == false)
    {
        std::cout << "Failed to load portalgate tile texture" << std::endl;
        return false;
    }
    if (LoadTexture("Image/Portals/portalgate_off.png", INTERACTABLE_PORTAL_GATE_OFF) == false)
    {
        std::cout << "Failed to load portalgate_off tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Scene2D_Spikes.tga", INTERACTABLE_SPIKES) == false)
    {
        std::cout << "Failed to load Scene2D_Spikes tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/altar.png", INTERACTABLE_ALTAR) == false)
    {
        std::cout << "Failed to load Scene2D_Spa tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Switches/door_1_closed.png", INTERACTABLE_DOOR_CLOSED) == false)
    {
        std::cout << "Failed to load Door Closed tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Switches/door_1_open.png", INTERACTABLE_DOOR_OPEN) == false)
    {
        std::cout << "Failed to load Door Open tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Enemy/MushroomPreview.png", ENEMY_1) == false)
    {
        std::cout << "Failed to load Mushroom tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Enemy/SnailPreview.png", ENEMY_2) == false)
    {
        std::cout << "Failed to load Pig tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Enemy/PauseMachineGun.png", ENEMY_3) == false)
    {
        std::cout << "Failed to load Snail tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Enemy/MachineGun.png", ENEMY_4) == false)
    {
        std::cout << "Failed to load Machine Gun tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Enemy/PigPreview.png", ENEMY_5) == false)
    {
        std::cout << "Failed to load Pig tile texture" << std::endl;
        return false;
    }

    if (LoadTexture("Image/Objects/Bullet.tga", BULLETS_ENEMY) == false) {
        std::cout << "Failed to load bullet texture" << std::endl;
        return false;
    }

	return true;
}


/**
 @brief Load a texture, assign it a code and store it in MapOfTextureIDs.
 @param filename A const char* variable which contains the file name of the texture
 @param iTextureCode A const int variable which is the texture code.
 */
bool CTextureManager::LoadTexture(const char* filename, const int iTextureCode)
{

	// Variables used in loading the texture
	int width, height, nrChannels;
	unsigned int textureID;

	// texture 1
	// ---------
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	// set the texture wrapping parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	CImageLoader* cImageLoader = CImageLoader::GetInstance();
	unsigned char* data = cImageLoader->Load(filename, width, height, nrChannels, true);
	if (data)
	{
		if (nrChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		else if (nrChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		// Generate mipmaps
		glGenerateMipmap(GL_TEXTURE_2D);

		// Store the texture ID into MapOfTextureIDs
		MapOfTextureIDs.insert(std::pair<int, int>(iTextureCode, textureID));
	}
	else
	{
		return false;
	}
	// Free up the memory of the file data read in
	free(data);

	return true;
}