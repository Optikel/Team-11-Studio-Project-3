/**
 Map2D
 By: Toh Da Jun
 Date: Mar 2020
 */
#include "Map2D.h"

// Include Shader Manager
#include "RenderControl\ShaderManager.h"

// Include Filesystem
#include "System\filesystem.h"
// Include ImageLoader
#include "System\ImageLoader.h"
#include "Primitives/MeshBuilder.h"

#include <iostream>
#include <vector>

using namespace std;

/**
 @brief Constructor This constructor has protected access modifier as this class will be a Singleton
 */
CMap2D::CMap2D(void)
	: uiCurLevel(0)
	, camera(NULL)
{
	uiNumLevels = 0;
}

/**
 @brief Destructor This destructor has protected access modifier as this class will be a Singleton
 */
CMap2D::~CMap2D(void)
{
	// optional: de-allocate all resources once they've outlived their purpose:
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	// Set this to NULL since it was created elsewhere, so we let it be deleted there.
	cSettings = NULL;
}

/**
@brief Init Initialise this instance
*/
bool CMap2D::Init(const unsigned int uiNumLevels,
	const unsigned int uiNumRows,
	const unsigned int uiNumCols)
{
	// Get the handler to the CSettings instance
	cSettings = CSettings::GetInstance();

	// Get the handler to the CTextureManager instance
	cTextureManager = CTextureManager::GetInstance();

	//Initialising objects
	arrObject.clear();
	for (unsigned i = 0; i < uiNumLevels; i++)
		arrObject.push_back(std::vector<CObject2D*>());

	//Initialising limits of map
	arrLevelLimit.clear();
	arrGrid.clear();
	arrObject.clear();
	for (unsigned i = 0; i < uiNumLevels; i++) {
		arrLevelLimit.push_back(glm::i32vec2());
		arrObject.push_back(std::vector<CObject2D*>());
		arrGrid.push_back(std::vector<std::vector<CObject2D*>>());
	}


	// Store the map sizes in cSettings
	uiCurLevel = 0;
	this->uiNumLevels = uiNumLevels;
	cSettings->NUM_TILES_XAXIS = uiNumCols;
	cSettings->NUM_TILES_YAXIS = uiNumRows;
	cSettings->UpdateSpecifications();

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//CS: Create the Quad Mesh using the mesh builder
	quadMesh = CMeshBuilder::GenerateQuad(glm::vec4(1, 1, 1, 1), cSettings->TILE_WIDTH, cSettings->TILE_HEIGHT);

	// Load and create textures
	// Load the ground texture

	/// <summary>
	/// 1 -> Player
	/// 2 - 99 -> NO COLLISION
	///		2 - 10  -> Collectibles
	///		11 - 20 -> Interactables?
	///			11 - 20 -> Toggles
	///			16 - 20 -> In Range??
	///		21 - 30 -> AOE Effect
	/// 
	/// 100 - 200 -> COLLISION
	///		100 - 149 -> Ground Tiles?
	///		150 - 159 -> Physics Affected Objects? e.g. boulder
	/// 300 - 400 -> ENEMY / ENTITY
	/// </summary>


	//Camera
	camera = Camera2D::GetInstance();

	return true;
}

/**
@brief Update Update this instance
*/
void CMap2D::Update(const double dElapsedTime)
{
	for (auto object : arrObject[uiCurLevel])
	{
		object->Update(dElapsedTime);
	}
}

/**
 @brief PreRender Set up the OpenGL display environment before rendering
 */
void CMap2D::PreRender(void)
{
	// Activate blending mode
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// bind textures on corresponding texture units
	glActiveTexture(GL_TEXTURE0);

	// Activate the shader
	CShaderManager::GetInstance()->Use(sShaderName);
}

/**
 @brief Render Render this instance
 */
void CMap2D::Render(void)
{
	// get matrix's uniform location and set matrix
	unsigned int transformLoc = glGetUniformLocation(CShaderManager::GetInstance()->activeShader->ID, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

	//Render(MY VERSION)

	//Camera init
	glm::vec2 offset = glm::vec2(float(cSettings->NUM_TILES_XAXIS / 2.f) - 1.f, float(cSettings->NUM_TILES_YAXIS / 2.f) - 1.f);
	glm::vec2 cameraPos = camera->getCurrPos();

	for (unsigned i = 0; i < arrObject[uiCurLevel].size(); i++) {
		CObject2D* currObj = arrObject[uiCurLevel][i];
		glm::vec2 objCamPos = currObj->vTransform - cameraPos + offset;

		glm::vec2 actualPos = cSettings->ConvertIndexToUVSpace(objCamPos);

		float clampX = 1.01f;
		float clampY = 1.01f;
		if (actualPos.x <= -clampX || actualPos.x >= clampX || actualPos.y <= -clampY || actualPos.y >= clampY)
			continue;

		transform = glm::mat4(1.f);
		transform = glm::translate(transform, glm::vec3(actualPos.x, actualPos.y, 0.f));
		/*transform = glm::translate(transform, glm::vec3(w
			cSettings->ConvertIndexToUVSpace(cSettings->x, currObj.indexSpace.x, false, 0),
			cSettings->ConvertIndexToUVSpace(cSettings->y, currObj.indexSpace.y, false),
			0.f
		));*/

		// Update the shaders with the latest transform
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

		RenderTile(*currObj);
	}
}

/**
 @brief PostRender Set up the OpenGL display environment after rendering.
 */
void CMap2D::PostRender(void)
{
	// Disable blending
	glDisable(GL_BLEND);

	for (unsigned i = 0; i < arrObject[uiCurLevel].size(); i++)
	{
		CObject2D* currObj = arrObject[uiCurLevel][i];

		currObj->GetCollider()->PreRender();
		currObj->GetCollider()->Render();
		currObj->GetCollider()->PostRender();
	}
}

int CMap2D::GetLevelCol(void) {
	return arrLevelLimit[uiCurLevel].x;
}

int CMap2D::GetLevelRow(void) {
	return arrLevelLimit[uiCurLevel].y;
}

glm::i32vec2 CMap2D::GetLevelLimit(void) {
	return arrLevelLimit[uiCurLevel];
}

/**
 @brief Set the value at certain indices in the arrMapInfo
 @param iRow A const int variable containing the row index of the element to set to
 @param iCol A const int variable containing the column index of the element to set to
 @param iValue A const int variable containing the value to assign to this arrMapInfo
 */
void CMap2D::SetMapInfo(const unsigned int uiRow, const unsigned int uiCol, const int iValue, const bool bInvert)
{
	for (unsigned i = 0; i < arrObject[uiCurLevel].size(); i++) {
		CObject2D* obj = arrObject[uiCurLevel][i];

		if (obj->vTransform.x == uiCol && obj->vTransform.y == uiRow) {
			if (iValue == 0) {
				arrGrid[uiCurLevel][GetLevelRow() - obj->vTransform.y - 1][obj->vTransform.x] = nullptr;

				delete obj;
				arrObject[uiCurLevel][i] = nullptr;
				arrObject[uiCurLevel].erase(arrObject[uiCurLevel].begin() + i);


				return;
			}

			obj->SetValue(iValue);
			return;
		}
	}

	if (iValue == 0)
		return;

	CObject2D* newObj = objFactory.CreateObject(iValue);
	newObj->vTransform = glm::i32vec2(uiCol, uiRow);
	newObj->SetValue(iValue);
	newObj->Init();

	arrObject[uiCurLevel].push_back(newObj);
}

void CMap2D::ReplaceGridInfo(const unsigned int uiRow, const unsigned uiCol, CObject2D* target, const bool bInvert)
{
	// Used for checking current spot
	CObject2D* obj = nullptr;

	// Used to check new spot
	CObject2D* newSpot = nullptr;
	if (bInvert)
	{
		obj = arrGrid[uiCurLevel][GetLevelRow() - target->GetCurrentIndex().y - 1][target->GetCurrentIndex().x];
		newSpot =  arrGrid[uiCurLevel][GetLevelRow() - uiRow - 1][uiCol];
	}
	else
	{
		obj = arrGrid[uiCurLevel][target->GetCurrentIndex().y][target->GetCurrentIndex().x];
		newSpot = arrGrid[uiCurLevel][uiRow][uiCol];
	}

	if (newSpot == nullptr)
	{
		if (obj == target)
		{
			if (bInvert)
			{
				arrGrid[uiCurLevel][target->GetCurrentIndex().y][target->GetCurrentIndex().x] = nullptr;
				arrGrid[uiCurLevel][uiRow][uiCol] = target;
			}
			else
			{
				arrGrid[uiCurLevel][target->GetCurrentIndex().y][target->GetCurrentIndex().x] = nullptr;
				arrGrid[uiCurLevel][uiRow][uiCol] = target;
			}

			target->SetCurrentIndex(glm::i32vec2(uiCol, uiRow));
		}
	}
	else
	{
		cout << "SOMETHING IN GRID SPACE" << endl;
	}
}

std::vector<CObject2D*> CMap2D::GetObjectArr()
{
	return arrObject[uiCurLevel];
}

CObject2D* CMap2D::GetCObject(const unsigned int uiCol, const unsigned int uiRow, const bool bInvert)
{
	if(bInvert)
		return arrGrid[uiCurLevel][GetLevelRow() - uiRow - 1][uiCol];
	else
		return arrGrid[uiCurLevel][uiRow][uiCol];
}

/**
 @brief Get the value at certain indices in the arrMapInfo
 @param iRow A const int variable containing the row index of the element to get from
 @param iCol A const int variable containing the column index of the element to get from
 @param bInvert A const bool variable which indicates if the row information is inverted
 */
int CMap2D::GetMapInfo(const unsigned int uiRow, const unsigned int uiCol, const bool bInvert) const
{
	//Check if theres object on tile
	for (unsigned i = 0; i < arrObject[uiCurLevel].size(); i++) {
		CObject2D* obj = arrObject[uiCurLevel][i];

		if (obj->vTransform.x == uiCol && obj->vTransform.y == uiRow)
			return obj->Getvalue();
	}

	//Return false if theres nothing on the tile
	return 0;
}


/**
 @brief Load a map
 */ 
bool CMap2D::LoadMap(string filename, const unsigned int uiCurLevel)
{
	doc = rapidcsv::Document(FileSystem::getPath(filename).c_str());

	arrLevelLimit[uiCurLevel] = glm::i32vec2(doc.GetColumnCount(), doc.GetRowCount());

	// Read the rows and columns of CSV data into arrMapInfo
	for (unsigned int uiRow = 0; uiRow < (unsigned int)doc.GetRowCount(); uiRow++)
	{
		// Read a row from the CSV file
		std::vector<std::string> row = doc.GetRow<std::string>(uiRow);

		arrGrid[uiCurLevel].push_back(std::vector<CObject2D*>());
		
		// Load a particular CSV value into the arrMapInfo
		for (unsigned int uiCol = 0; uiCol < (unsigned int)doc.GetColumnCount(); ++uiCol)
		{
			//Init of objects values
			int currVal = (int)stoi(row[uiCol]);

			arrGrid[uiCurLevel][uiRow].push_back(nullptr);

			if (currVal > 0) {
				CObject2D* currObj = objFactory.CreateObject(currVal);
				currObj->SetValue(currVal);
				currObj->SetCurrentIndex(glm::i32vec2(uiCol, uiRow));

				//Position of values
				glm::vec2 currIndex;
				currIndex.x = (float)uiCol;
				currIndex.y = (float)doc.GetRowCount() - (float)uiRow - 1.f;
				currObj->vTransform = currIndex;

				currObj->Init();

				arrObject[uiCurLevel].push_back(currObj);
				//Add in new CObj pointer if available
				arrGrid[uiCurLevel][uiRow][uiCol] = currObj;
			}

		}
	}

	return true;
}

/**
 @brief Save the tilemap to a text file
 @param filename A string variable containing the name of the text file to save the map to
 */
bool CMap2D::SaveMap(string filename, const unsigned int uiCurLevel)
{
	// Update the rapidcsv::Document from arrMapInfo

	//Reset the map
	for (unsigned int uiRow = 0; uiRow < cSettings->NUM_TILES_YAXIS; uiRow++)
	{
		for (unsigned int uiCol = 0; uiCol < cSettings->NUM_TILES_XAXIS; uiCol++)
		{
			doc.SetCell(uiCol, uiRow, 0);
		}
		//cout << endl; //Add back if i mess up somewhere future jevon you know what i mean dont cb
	}

	for (unsigned i = 0; i < arrObject[uiCurLevel].size(); i++) {
		CObject2D* obj = arrObject[uiCurLevel][i];

		doc.SetCell(obj->vTransform.x, cSettings->NUM_TILES_YAXIS - 1 - obj->vTransform.y, obj->Getvalue());
	}

	// Save the rapidcsv::Document to a file
	doc.Save(FileSystem::getPath(filename).c_str());

	return true;
}

/**
@brief Find the indices of a certain value in arrMapInfo
@param iValue A const int variable containing the row index of the found element
@param iRow A const int variable containing the row index of the found element
@param iCol A const int variable containing the column index of the found element
@param bInvert A const bool variable which indicates if the row information is inverted
*/
bool CMap2D::FindValue(const int iValue, unsigned int& uirRow, unsigned int& uirCol)
{
	for (unsigned i = 0; i < arrObject[uiCurLevel].size(); i++) {
		CObject2D* obj = arrObject[uiCurLevel][i];

		if (obj->Getvalue() == iValue) {
			uirCol = obj->vTransform.x;
			uirRow = obj->vTransform.y; //For now keep the same

			return true;
		}
	}

	return false;
}

/**
 @brief Set current level
 */
void CMap2D::SetCurrentLevel(unsigned int uiCurLevel)
{
	if (uiCurLevel < uiNumLevels)
	{
		this->uiCurLevel = uiCurLevel;
	}
}

/**
 @brief Get current level
 */
unsigned int CMap2D::GetCurrentLevel(void) const
{
	return uiCurLevel;
}


void CMap2D::RenderTile(const CObject2D obj) {
	glBindTexture(GL_TEXTURE_2D, cTextureManager->MapOfTextureIDs.at(obj.Getvalue()));

	glBindVertexArray(VAO);
	//CS: Render the tile
	quadMesh->Render();
	glBindVertexArray(0);
}
