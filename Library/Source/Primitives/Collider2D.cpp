#include "Collider2D.h"

// Include Shader Manager
#include "..\RenderControl\ShaderManager.h"

#include <GLFW/glfw3.h>
#include "../GameControl/Settings.h"

Collider2D::Collider2D()
	: vec2Dimensions(glm::vec2(0.5f, 0.5f))
	, position(glm::vec3(1.f))
	, vec4Colour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f))
	, bIsDisplayed(true)
	, fLineWidth(1.0f)
	, colliderEnabled(true)
{
	sLineShaderName = "LineShader";
}

Collider2D::~Collider2D()
{
}

bool Collider2D::Init()
{
	vec2Dimensions = glm::vec2(CSettings::GetInstance()->TILE_WIDTH, CSettings::GetInstance()->TILE_HEIGHT) * 0.5f;

	float vertices[] = {
		-vec2Dimensions.x, -vec2Dimensions.y, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		vec2Dimensions.x, -vec2Dimensions.y, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		vec2Dimensions.x, vec2Dimensions.y, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		vec2Dimensions.x, vec2Dimensions.y, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-vec2Dimensions.x, vec2Dimensions.y, vec4Colour.x, vec4Colour.y, vec4Colour.z,
		-vec2Dimensions.x, -vec2Dimensions.y, vec4Colour.x, vec4Colour.y, vec4Colour.z,
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
	glLineWidth(fLineWidth);
	 
	return true;
}

void Collider2D::SetLineShader(const std::string& name)
{
	sLineShaderName = name;
}

bool Collider2D::CollideWith(Collider2D* object)
{
	if (object->colliderEnabled)
	{
		//vec2Dimensions is half width and half height	
		bool collisionX = abs(position.x - object->position.x) < vec2Dimensions.x + object->vec2Dimensions.x;
		bool collisionY = abs(position.y - object->position.y) < vec2Dimensions.y + object->vec2Dimensions.y;

		return collisionX && collisionY;
	}
	
	return false;
}

Collider2D::CorrectedAxis Collider2D::ResolveCollision(Collider2D* object)
{
	glm::vec2 direction = object->position - position;

	float shortestXDist = 10000;
	float shortestYDist = 10000;
	for (int i = -1; i <= 1; i++)
	{
		//-1 and 1 
		if (i != 0)
		{
			float tempxdist = glm::length(glm::vec2(position.x + vec2Dimensions.x * i, 0) - glm::vec2(object->position.x + object->vec2Dimensions.x * -i, 0));
			if (tempxdist < shortestXDist)
				shortestXDist = tempxdist;

			float tempydist = glm::length(glm::vec2(0, position.y + vec2Dimensions.y * i) - glm::vec2(0, object->position.y + object->vec2Dimensions.y * -i));
			if (tempydist < shortestYDist)
				shortestYDist = tempydist;
		}
	}

	if (shortestXDist < shortestYDist)
	{
		glm::vec2 correctionAxis = glm::normalize(glm::vec2(direction.x * -1, 0.f));
		position += glm::vec3(shortestXDist, 0, 0) * glm::vec3(correctionAxis, 0);
		return CorrectedAxis::X;
	}
	else
	{
		glm::vec2 correctionAxis = glm::normalize(glm::vec2(0.f, direction.y * -1));
		position += glm::vec3(0, shortestYDist, 0) * glm::vec3(correctionAxis, 0);
		return CorrectedAxis::Y;
	}

}

void Collider2D::ResolveCollisionX(Collider2D* object)
{
	glm::vec2 direction = object->position - position;

	float shortestXDist = 10000;
	float shortestYDist = 10000;
	for (int i = -1; i <= 1; i++)
	{
		//-1 and 1 
		if (i != 0)
		{
			float tempxdist = glm::length(glm::vec2(position.x + vec2Dimensions.x * i, 0) - glm::vec2(object->position.x + object->vec2Dimensions.x * -i, 0));
			if (tempxdist < shortestXDist)
				shortestXDist = tempxdist;

			float tempydist = glm::length(glm::vec2(0, position.y + vec2Dimensions.y * i) - glm::vec2(0, object->position.y + object->vec2Dimensions.y * -i));
			if (tempydist < shortestYDist)
				shortestYDist = tempydist;
		}
	}

	if (shortestXDist < shortestYDist)
	{
		glm::vec2 correctionAxis = glm::normalize(glm::vec2(direction.x * -1, 0.f));
		position += glm::vec3(shortestXDist, 0, 0) * glm::vec3(correctionAxis, 0);
	}
}

void Collider2D::ResolveCollisionY(Collider2D* object)
{
	glm::vec2 direction = object->position - position;

	float shortestXDist = 10000;
	float shortestYDist = 10000;
	for (int i = -1; i <= 1; i++)
	{
		//-1 and 1 
		if (i != 0)
		{
			float tempxdist = glm::length(glm::vec2(position.x + vec2Dimensions.x * i, 0) - glm::vec2(object->position.x + object->vec2Dimensions.x * -i, 0));
			if (tempxdist < shortestXDist)
				shortestXDist = tempxdist;

			float tempydist = glm::length(glm::vec2(0, position.y + vec2Dimensions.y * i) - glm::vec2(0, object->position.y + object->vec2Dimensions.y * -i));
			if (tempydist < shortestYDist)
				shortestYDist = tempydist;
		}
	}

	if (shortestXDist > shortestYDist)
	{
		glm::vec2 correctionAxis = glm::normalize(glm::vec2(0.f, direction.y * -1));
		position += glm::vec3(0, shortestYDist, 0) * glm::vec3(correctionAxis, 0);
	}
}

void Collider2D::PreRender(void)
{
	// Use the shader defined for this class
	CShaderManager::GetInstance()->Use(sLineShaderName);
}

void Collider2D::Render(void)
{
	if (!bIsDisplayed)
		return;

	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, position);
	CShaderManager::GetInstance()->activeShader->setMat4("transform", transform);

	// render box
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINE_LOOP, 0, 6);
}

void Collider2D::PostRender(void)
{
}
