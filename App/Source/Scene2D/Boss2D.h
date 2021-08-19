#pragma once

// Include shader
#include "RenderControl\shader.h"

//CEnemy2D
#include "Enemy2D.h"

// Include the Map2D as we will use it to check the player's movements and actions
class CMap2D;

//Include CEntityManager to get curr round
class CEntityManager;

//Include Player2D
class CPlayer2D;

// Include Settings
#include "GameControl\Settings.h"

//Math.h
#include "Math/MyMath.h"

//Camera
#include "Camera2D.h"

class CBoss2D : public CEnemy2D
{
	public :
		CBoss2D(void);
		virtual ~CBoss2D(void);

		bool Init(void);

		// Update
		void Update(const double dElapsedTime);

		// PreRender
		void PreRender(void);

		// Render
		void Render(void);

		// PostRender
		void PostRender(void);

	protected:
		enum class ATK {
			A_CIRCLE, //Attacks in a circular pattern.
			A_ATTACK, //Shoots at the player normally
			A_TOTAL
		};

		std::vector<ATK>* arrATK; //Current attack during current round
		int fsmIndex; //Current index of where its at in the array
		int roundIndex; //Current round

		float currAtkDuration;
		float currPauseDuration;

		std::vector<int>* arrAtkDuration;
		std::vector<int>* arrPauseDuration;

		bool isSeen;

		float bulletAng; //Angle of which where the bullet will come from
		int maxBulletTimer[(int)ATK::A_TOTAL]; //Timer in terms of frames
		int bulletTimer;

		//Rendering
		//Mesh
		CMesh* quadMesh;

		// Current color
		glm::vec4 currentColor;
		bool bIsActive;

		//Handlers
		CEntityManager* cEntityManager;

		//Functions
		// Load a texture
		bool LoadTexture(const char* filename, GLuint& iTextureID);

		//Randomise timer
		int RandomiseTimer(bool atk);

		//Randomise attacks
		ATK RandomiseAttack(void);

		//Update attacks
		void UpdateAttack(float dElapsedTime);
};

