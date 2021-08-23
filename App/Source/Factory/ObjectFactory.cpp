#include "ObjectFactory.h"

#include "../Scene2D/Boulder2D.h"

#include "../Scene2D/EnemyBullet2D.h"

#include "../Scene2D/Projectiles.h"
#include "../Scene2D/Bullet2D.h"


CObject2D* ObjectFactory::CreateObject(int iTextureID)
{
    CObject2D* newObj = nullptr;

	if (iTextureID > TILE_START && iTextureID < TILE_TOTAL)
	{
		newObj = new CObject2D(iTextureID);
	}
	else if (iTextureID > CONSUMABLES_START && iTextureID < CONSUMABLES_TOTAL)
	{
		newObj = new CObject2D(iTextureID);
	}
	else if (iTextureID > INTERACTABLE_START && iTextureID < INTERACTABLE_TOTAL)
	{
		newObj = new Interactables(iTextureID);
	}
	else if (iTextureID > PROJECTILES_START && iTextureID < PROJECTILES_TOTAL)
	{
		newObj = new Projectiles(iTextureID);
	}
	else if (iTextureID > BULLETS_START && iTextureID < BULLETS_TOTAL)
	{
		newObj = new Bullet2D(iTextureID);
	}
	else if (iTextureID > OBSTACLES_START && iTextureID < OBSTACLES_TOTAL)
	{
		if(iTextureID == OBSTACLE_BOULDER)
			newObj = new Boulder2D(iTextureID);
		else
			newObj = new CObject2D(iTextureID);
	}
	else
		newObj = new CObject2D(iTextureID);

    return newObj;
}

EnemyBullet2D* ObjectFactory::CreateBullet(float angle, glm::vec2 vTransform) {
	EnemyBullet2D* bullet = new EnemyBullet2D;
	if (!bullet->Init(angle, vTransform)) {
		delete bullet;
		bullet = nullptr; //Delete bullet if initialisation fails
	}
	else {
		bullet->SetShader("2DColorShader");
	}

	return bullet;
}
