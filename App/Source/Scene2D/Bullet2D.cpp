#include "Bullet2D.h"

#include "Map2D.h"
#include <vector>
#include "RenderControl/ShaderManager.h"
#include "Primitives/MeshBuilder.h"
Bullet2D::Bullet2D(int iTextureID)
{
	this->iTextureID = iTextureID; //Shuriken also by default
}

Bullet2D::~Bullet2D()
{
	if (animatedSprites)
		delete animatedSprites;

	glDeleteVertexArrays(1, &VAO);
}

bool Bullet2D::Init()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	cSettings = CSettings::GetInstance();
	mesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), cSettings->TILE_WIDTH, cSettings->TILE_HEIGHT);

	if (!cPhysics2D)
		cPhysics2D = new CPhysics2D;
	if (!collider2D)
		collider2D = new Collider2D;

	cPhysics2D->Init(&vTransform);
	cPhysics2D->SetGravity(0.f);
	cPhysics2D->MAX_SPEED = 50.f;

	collider2D->Init(vTransform, glm::vec2(0.2f), Collider2D::ColliderType::COLLIDER_CIRCLE);
	return false;
}

void Bullet2D::Update(double dElapsedTime)
{
	cPhysics2D->Update(dElapsedTime);
	collider2D->SetPosition(vTransform);

	CMap2D* cMap2D = CMap2D::GetInstance();

	if (cPhysics2D->GetVelocity().x < 0)
		fRotate = 180;
	else
		fRotate = 0;

	ResolveMapCollision(CheckMapCollision());
}

void Bullet2D::PreRender()
{
	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	CShaderManager::GetInstance()->Use("2DShader");
	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);
	// Activate the shader
}

void Bullet2D::Render()
{
	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	//Camera init
	glm::vec2 offset = glm::vec2(float(cSettings->NUM_TILES_XAXIS / 2.f), float(cSettings->NUM_TILES_YAXIS / 2.f));
	glm::vec2 cameraPos = Camera2D::GetInstance()->getCurrPos();

	glm::vec2 objCamPos = vTransform - cameraPos + offset;

	glm::vec2 actualPos = cSettings->ConvertIndexToUVSpace(objCamPos);

	float clampOffset = cSettings->ConvertIndexToUVSpace(CSettings::AXIS::x, 1, false) / 2;

	float clampX = 2.0f + clampOffset;
	float clampY = 2.0f + clampOffset;

	if (actualPos.x >= -clampX && actualPos.x <= clampX && actualPos.y >= -clampY && actualPos.y <= clampY)
	{
		transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
		transform = glm::translate(transform, glm::vec3(glm::vec2(actualPos.x, actualPos.y),
			0.0f));
		transform = glm::rotate(transform, glm::radians(fRotate), glm::vec3(0.f, 0.f, 1.f));
		// Update the shaders with the latest transform
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		// Get the texture to be rendered
		glBindTexture(GL_TEXTURE_2D, CTextureManager::GetInstance()->MapOfTextureIDs.at(iTextureID));

		glBindVertexArray(VAO);
		mesh->Render();

		glBindVertexArray(0);
	}
}

void Bullet2D::PostRender()
{
	// Disable blending
	glDisable(GL_BLEND);
}

void Bullet2D::ResolveMapCollision(std::vector<pair<CObject2D*, float>> aabbvector)
{
	for (auto aabbTuple : aabbvector)
	{
		CObject2D* obj = aabbTuple.first;
		Collision data = (collider2D->CollideWith(obj->GetCollider()));
		if (std::get<0>(data))
		{
			bDestroyed = true;
		}
	}
}

