#include "EntityManager.h"

#include "MobEnemy2D.h"

#include "Boss2D.h"

#include "Bullet2D.h"
#include "Projectiles.h"

CEntityManager::CEntityManager()
	: cPlayer2D(NULL)
	, cEnemy2D(NULL)
	, cMap2D(NULL)
	, cBoss2D(NULL)
	, cKeyboardController(NULL)
	, cInputHandler(NULL)
	, CInventoryManager(NULL)
	, currRound(0)
{
}

CEntityManager::~CEntityManager()
{
	Clear();
}

bool CEntityManager::EntityManagerInit(const unsigned int totalLevels)
{
	// Create and initialise the Map 2D
	cMap2D = CMap2D::GetInstance();
	// Set a shader to this class
	cMap2D->SetShader("2DShader");
	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();
	cInputHandler = CInputHandler::GetInstance();

	for (unsigned int i = 0; i < totalLevels; ++i)
	{
		m_enemyList.push_back(std::vector<CEnemy2D*>());
		m_interactableList.push_back(std::vector<Interactables*>());
		m_BulletList.push_back(std::vector<Projectiles*>());
		m_ObstacleList.push_back(std::vector<Obstacle2D*>());
		cBoss2D.push_back(nullptr);
	}

	//for (Interactables* i : m_interactableList)
	//{
	//	i->Init();
	//}

	// clone init
	//cCloneTemplate = new CPlayer2D();
	//if (cCloneTemplate->Init(cPlayer2D->GetCheckpoint(),m_cloneList.size()) == false)
	//{
	//	cout << "Failed to load clone" << endl;
	//	return false;
	//}
	//// initialise all default values
	//cCloneTemplate->SetClone(true);
	//cCloneTemplate->SetShader("2DColorShader");
	//// find position to spawn in map
	//if (cMap2D->FindValue(1, uiRow, uiCol) == true)
	//{
	//	cCloneTemplate->vTransform = glm::vec2(uiCol, uiRow);
	//}
	//currRound = 0;

	return true;
}

std::vector<CEnemy2D*> CEntityManager::GetAllEnemies(void) {
	std::vector<CEnemy2D*> arr;
	arr.push_back(cBoss2D[cMap2D->GetCurrentLevel()]);
	arr.insert(arr.end(), m_enemyList[cMap2D->GetCurrentLevel()].begin(), m_enemyList[cMap2D->GetCurrentLevel()].end());

	arr.erase(std::remove(arr.begin(), arr.end(), nullptr), arr.end());

	return arr;
}

std::vector<CEntity2D*> CEntityManager::GetAllLivingEntities(void) {
	std::vector<CEnemy2D*> enemyArr = GetAllEnemies();
	std::vector<CEntity2D*> convertedEnemyArr(enemyArr.begin(), enemyArr.end());

	std::vector<CPlayer2D*> playerArr = GetAllPlayers();
	std::vector<CEntity2D*> convertedPlayerArr(playerArr.begin(), playerArr.end());

	std::vector<CEntity2D*> totalArr;
	totalArr.reserve(convertedPlayerArr.size() + convertedEnemyArr.size());
	totalArr.insert(totalArr.end(), convertedPlayerArr.begin(), convertedPlayerArr.end());
	totalArr.insert(totalArr.end(), convertedEnemyArr.begin(), convertedEnemyArr.end());

	return totalArr;
}

std::vector<Interactables*> CEntityManager::GetAllInteractables(void)
{
	return m_interactableList[cMap2D->GetCurrentLevel()];
}

std::vector<Obstacle2D*> CEntityManager::GetallObstacles(void)
{
	return std::vector<Obstacle2D*>(m_ObstacleList[cMap2D->GetCurrentLevel()]);
}

void CEntityManager::PushEnemy(CEnemy2D* enemy, const unsigned int currLevel) {
	if (enemy)
		m_enemyList[currLevel].push_back(enemy);
	else
		DEBUG_MSG("ENEMY NOT ADDED AS IT IS A NULLPTR.");
}

CPlayer2D* CEntityManager::Clone(void)
{
	CPlayer2D* clone = new CPlayer2D();
	clone->SetShader("2DColorShader");

	if (!clone->Init(cPlayer2D->GetCheckpoint(), m_cloneList.size()))
	{
		DEBUG_MSG("Failed to clone Player");
		return nullptr;
	}
	clone->SetClone(true);
	clone->SetKeyInputs(cInputHandler->GetAllKeyboardInputs());
	clone->SetMouseInputs(cInputHandler->GetAllMouseInputs());
	m_cloneList.push_back(clone);

	return clone;
}


bool CEntityManager::CheckCollision(CEntity2D* type1, CEntity2D* type2)
{
	if (type1->type == CEntity2D::ENTITY_TYPE::PLAYER && type2->type == CEntity2D::ENTITY_TYPE::ENEMY)
	{
		return false;
	}
	else
		return false;

}

void CEntityManager::RemoveEntity(string type, int amount)
{
	if (type == "ENEMY")
	{
		for (int i = 0; i < amount; i++)
		{
			m_enemyList.erase(m_enemyList.begin());
			return;
		}
	}
	else if (type == "CLONE")
	{
		for (int i = 0; i < amount; i++)
		{
			m_cloneList.erase(m_cloneList.begin());
			return;
		}
	}
	else
		return;
}

bool CEntityManager::InitPlayer()
{
	unsigned int uiRow = -1;
	unsigned int uiCol = -1;

	if (cMap2D->FindValue(1, uiRow, uiCol) == true)
	{
		cPlayer2D = new CPlayer2D;
		// Pass shader to cPlayer2D
		cPlayer2D->SetShader("2DColorShader");
		// m_playerList.push_back(cPlayer2D);
		// Initialise the instance
		if (cPlayer2D->Init() == false)
		{
			cout << "Failed to load CPlayer2D" << endl;
			return false;
		}
	}

	return true;
}

CPlayer2D* CEntityManager::GetPlayer()
{
	return cPlayer2D;
}

CBoss2D* CEntityManager::GetBoss()
{
	return cBoss2D[cMap2D->GetCurrentLevel()];
}

std::vector<CPlayer2D*> CEntityManager::GetAllPlayers(void) 
{
	std::vector<CPlayer2D*> arr;
	
	arr.push_back(cPlayer2D); //Add controllable player

	for (unsigned i = 0; i < m_cloneList.size(); i++) 
	{ //Add the other clones to the vector
		arr.push_back(static_cast<CPlayer2D*>(m_cloneList[i])); 
	}

	return arr;
}

int CEntityManager::GetCurrRound(void) 
{
	return currRound;
}

void CEntityManager::RenderEnemy(void)
{
	for (unsigned i = 0; i < m_enemyList[cMap2D->GetCurrentLevel()].size(); i++)
	{
		m_enemyList[cMap2D->GetCurrentLevel()][i]->PreRender();
		m_enemyList[cMap2D->GetCurrentLevel()][i]->Render();
		m_enemyList[cMap2D->GetCurrentLevel()][i]->PostRender();

		m_enemyList[cMap2D->GetCurrentLevel()][i]->RenderCollider();
	}

	if (cBoss2D[cMap2D->GetCurrentLevel()]) {
		cBoss2D[cMap2D->GetCurrentLevel()]->PreRender();
		cBoss2D[cMap2D->GetCurrentLevel()]->Render();
		cBoss2D[cMap2D->GetCurrentLevel()]->PostRender();
		
		cBoss2D[cMap2D->GetCurrentLevel()]->RenderCollider();
	}
}

void CEntityManager::RenderBullets(void) {
	for (unsigned i = 0; i < m_BulletList[cMap2D->GetCurrentLevel()].size(); i++) {
		m_BulletList[cMap2D->GetCurrentLevel()][i]->PreRender();
		m_BulletList[cMap2D->GetCurrentLevel()][i]->Render();
		m_BulletList[cMap2D->GetCurrentLevel()][i]->PostRender();
		
		m_BulletList[cMap2D->GetCurrentLevel()][i]->RenderCollider();
	}
}

void CEntityManager::RenderInteractables(void)
{
	for (Interactables* i : m_interactableList[cMap2D->GetCurrentLevel()])
	{
		i->PreRender();
		i->Render();
		i->PostRender();

		i->RenderCollider();
	}
}

void CEntityManager::RenderObstacles(void)
{
	for (Obstacle2D* i : m_ObstacleList[cMap2D->GetCurrentLevel()])
	{
		i->PreRender();
		i->Render();
		i->PostRender();

		i->RenderCollider();
	}
}

void CEntityManager::RenderClone(void)
{
	for (unsigned i = 0; i < m_cloneList.size(); ++i)
	{
		m_cloneList[i]->PreRender();
		m_cloneList[i]->Render();
		m_cloneList[i]->PostRender();

		m_cloneList[i]->RenderCollider();
	}
}

void CEntityManager::RenderPlayer(void)
{
	// Call the CPlayer2D's PreRender()
	cPlayer2D->PreRender();
	// Call the CPlayer2D's Render()
	cPlayer2D->Render();
	// Call the CPlayer2D's PostRender()
	cPlayer2D->PostRender();

	cPlayer2D->RenderCollider();
}

void CEntityManager::Update(const double dElapsedTime)
{
	
	for (Interactables* i : m_interactableList[cMap2D->GetCurrentLevel()])
	{
		i->Update(dElapsedTime);
	}

	for (unsigned i = 0; i < m_cloneList.size(); ++i)
	{
		if (m_cloneList[i]->iTempFrameCounter - m_cloneList[i]->iFrameCounterEnd > 180)
		{
			delete m_cloneList[i];
			m_cloneList[i] = nullptr;
			/*m_cloneList.erase(std::remove(std::begin(m_cloneList), std::end(m_cloneList), nullptr), std::end(m_cloneList));*/
			continue;
		}

		m_cloneList[i]->Update(dElapsedTime);
	}

	m_cloneList.erase(std::remove(m_cloneList.begin(), m_cloneList.end(), nullptr), m_cloneList.end()); //Remove any nullptrs in the array

	// Call the cPlayer2D's update method before Map2D as we want to capture the inputs before map2D update
	cPlayer2D->Update(dElapsedTime);

	if (cBoss2D[cMap2D->GetCurrentLevel()])
		cBoss2D[cMap2D->GetCurrentLevel()]->Update(dElapsedTime);


	// Call all the cEnemy2D's update method before Map2D 
	// as we want to capture the updates before map2D update
	for (CEnemy2D* e : m_enemyList[cMap2D->GetCurrentLevel()]) 
	{
		e->Update(dElapsedTime);

		//Delete conditions
		if (e->GetHealth() <= 0) {
			delete e;
			e = nullptr;
		}
	}

	m_enemyList[cMap2D->GetCurrentLevel()].erase(std::remove(m_enemyList[cMap2D->GetCurrentLevel()].begin(), m_enemyList[cMap2D->GetCurrentLevel()].end(), nullptr), m_enemyList[cMap2D->GetCurrentLevel()].end()); //Remove any nullptrs in the array

	//Call enemy bullets
	for (unsigned i = 0; i < m_BulletList[cMap2D->GetCurrentLevel()].size(); i++) {
		m_BulletList[cMap2D->GetCurrentLevel()][i]->Update(dElapsedTime);

		if (dynamic_cast<Projectiles*>(m_BulletList[cMap2D->GetCurrentLevel()][i]))
		{
			if (dynamic_cast<Projectiles*>(m_BulletList[cMap2D->GetCurrentLevel()][i])->bDestroyed || dynamic_cast<Projectiles*>(m_BulletList[cMap2D->GetCurrentLevel()][i])->bOutsideBoundary())
			{
				DEBUG_MSG("bullet deleted");
				delete m_BulletList[cMap2D->GetCurrentLevel()][i];
				m_BulletList[cMap2D->GetCurrentLevel()][i] = nullptr;
			}
		}
	}

	//Remove any nullptrs in bullet array
	m_BulletList[cMap2D->GetCurrentLevel()].erase(std::remove(m_BulletList[cMap2D->GetCurrentLevel()].begin(), m_BulletList[cMap2D->GetCurrentLevel()].end(), nullptr), m_BulletList[cMap2D->GetCurrentLevel()].end());

	for (Obstacle2D* i : m_ObstacleList[cMap2D->GetCurrentLevel()])
	{
		i->Update(dElapsedTime);
	}

	//Keyboard inputs
	/*if (cKeyboardController->IsKeyPressed(GLFW_KEY_C))
	{
		Clone();
	}*/
}

void CEntityManager::PushBullet(Projectiles* bullet, const unsigned int currLevel) {
	m_BulletList[currLevel].push_back(bullet);
}

void CEntityManager::PushInteractables(Interactables* interactable, const unsigned int currLevel)
{
	m_interactableList[currLevel].push_back(interactable);
}

void CEntityManager::PushObstacles(Obstacle2D* obstacle, const unsigned int currLevel)
{
	m_ObstacleList[currLevel].push_back(obstacle);
}

void CEntityManager::Clear(void)
{
	if (cPlayer2D) {
		delete cPlayer2D;
		cPlayer2D = nullptr;
	}

	for (unsigned i = 0; i < cBoss2D.size(); ++i) {
		delete cBoss2D[i];
		cBoss2D[i] = nullptr;
	}
	cBoss2D.clear();

	for (unsigned i = 0; i < m_enemyList.size(); ++i) {
		for (unsigned j = 0; j < m_enemyList[i].size(); ++j)
		{
			delete m_enemyList[i][j];
			m_enemyList[i][j] = nullptr;
		}
		m_enemyList[i].clear();
	}
	m_enemyList.clear();

	for (unsigned i = 0; i < m_cloneList.size(); i++) {
		delete m_cloneList[i];
		m_cloneList[i] = nullptr;
	}
	m_cloneList.clear();

	for (unsigned i = 0; i < m_BulletList.size(); i++) {
		for (unsigned j = 0; j < m_BulletList[i].size(); ++j)
		{
			delete m_BulletList[i][j];
			m_BulletList[i][j] = nullptr;
		}
		m_BulletList[i].clear();
	}
	m_BulletList.clear();

	for (unsigned i = 0; i < m_interactableList.size(); i++) {
		for (unsigned j = 0; j < m_interactableList[i].size(); ++j)
		{
			delete m_interactableList[i][j];
			m_interactableList[i][j] = nullptr;
		}
		m_interactableList[i].clear();
	}
	m_interactableList.clear();

	for (unsigned i = 0; i < m_ObstacleList.size(); i++) {
		for (unsigned j = 0; j < m_ObstacleList[i].size(); ++j)
		{
			delete m_ObstacleList[i][j];
			m_ObstacleList[i][j] = nullptr;
		}
		m_ObstacleList[i].clear();
	}
	m_ObstacleList.clear();
}

