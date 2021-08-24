#pragma once

// Include AnimatedSprites
#include "Primitives/SpriteAnimation.h"

#include "Object2D.h"

class Interactables : public CObject2D
{
public:

	enum INTERACTABLE_TYPE
	{
		LEVER = OBJECT_TYPE::INTERACTABLE_SWITCH1_OFF,
		// PRESSURE_PLATE = 203,

		DOOR,
	};

	INTERACTABLE_TYPE interactableType;

	Interactables(int iTextureID = 0);

	// Destructor
	virtual ~Interactables(void);

	// Init
	bool Init();

	// Update
	void Update(const double dElapsedTime);

	// PreRender
	void PreRender(void);

	// Render
	void Render(void);

	// PostRender
	void PostRender(void);

	bool Activate();
	void SetInteractableID(int);

private:

	int iInteractableID;

	CMesh* quad;

	// Current color
	glm::vec4 currentColor;

	// Boolean to check if the interactable is currently being interacted with
	bool bInteraction;

	bool LoadTexture(const char* filename, GLuint& iTextureID);
};

