#include "EntityManager.h"

#include "MobEnemy2D.h"

#include "Boss2D.h"

#include "EnemyBullet2D.h"
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
	if (cPlayer2D) {
		delete cPlayer2D;
		cPlayer2D = nullptr;
	}

	if (cBoss2D) {
		delete cBoss2D;
		cBoss2D = nullptr;
	}

	for (unsigned i = 0; i < m_enemyList.size(); i++) {
		delete m_enemyList[i];
		m_enemyList[i] = nullptr;
	}
	m_enemyList.clear();

	for (unsigned i = 0; i < m_cloneList.size(); i++) {
		delete m_cloneList[i];
		m_cloneList[i] = nullptr;
	}
	m_cloneList.clear();

	for (unsigned i = 0; i < m_eBulletList.size(); i++) {
		delete m_eBulletList[i];
		m_eBulletList[i] = nullptr;
	}
	m_eBulletList.clear();
}

bool CEntityManager::EntityManagerInit(void)
{
	// Create and initialise the Map 2D
	cMap2D = CMap2D::GetInstance();
	// Set a shader to this class
	cMap2D->SetShader("2DShader");
	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();
	cInputHandler = CInputHandler::GetInstance();

	unsigned int uiRow = -1;
	unsigned int uiCol = -1;

	//player init
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

	//enemy init
	while (cMap2D->FindValue(300, uiRow, uiCol)) {
		m_enemyList.push_back(enemyFactory.CreateEnemy(300));
	}

	while (cMap2D->FindValue(301, uiRow, uiCol)) {
		m_enemyList.push_back(enemyFactory.CreateEnemy(301));
	}

	while (cMap2D->FindValue(303, uiRow, uiCol)) {
		m_enemyList.push_back(enemyFactory.CreateEnemy(303));
	}

	while (cMap2D->FindValue(304, uiRow, uiCol)) {
		m_enemyList.push_back(enemyFactory.CreateEnemy(304));
	}

	//Boss initialisation
	if (cMap2D->FindValue(305, uiRow, uiCol)) {
		cBoss2D = dynamic_cast<CBoss2D*>(enemyFactory.CreateEnemy(305));

		if (cMap2D->FindValue(305, uiRow, uiCol)) {
			DEBUG_MSG("ERROR: TOO MANY BOSS IN LEVEL. THERE SHOULD ONLY BE ONE BOSS");
			return false;
		}
		if (cBoss2D->Init() == false)
		{
			cout << "Failed to load CBoss2D" << endl;
			return false;
		}
	}

	//clone init
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
	currRound = 0;

	return true;
}

std::vector<CEnemy2D*> CEntityManager::GetAllEnemies(void) {
	std::vector<CEnemy2D*> arr;
	arr.push_back(cBoss2D);
	arr.insert(arr.end(), m_enemyList.begin(), m_enemyList.end());

	arr.erase(std::remove(arr.begin(), arr.end(), nullptr), arr.end());

	return arr;
}

void CEntityManager::PushEnemy(CEnemy2D* enemy) {
	if (enemy)
		m_enemyList.push_back(enemy);
	else
		DEBUG_MSG("ENEMY NOT ADDED AS IT IS A NULLPTR.");
}

bool CEntityManager::Clone(void)
{
	CPlayer2D* clone = new CPlayer2D();
	clone->SetShader("2DColorShader");

	if (!clone->Init(cPlayer2D->GetCheckpoint(),m_cloneList.size()))
	{
		std::cout << "Failed to clone Player\n";
		return false;
	}
	clone->SetClone(true);
	clone->SetKeyInputs(cInputHandler->GetAllKeyboardInputs());
	clone->SetMouseInputs(cInputHandler->GetAllMouseInputs());
	m_cloneList.push_back(clone);

	return true;
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

CPlayer2D* CEntityManager::GetPlayer()
{
	return cPlayer2D;
}

CBoss2D* CEntityManager::GetBoss()
{
	return cBoss2D;
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
	for (unsigned i = 0; i < m_enemyList.size(); i++)
	{
		m_enemyList[i]->PreRender();
		m_enemyList[i]->Render();
		m_enemyList[i]->PostRender();

		m_enemyList[i]->RenderCollider();
	}

	if (cBoss2D) {
		cBoss2D->PreRender();
		cBoss2D->Render();
		cBoss2D->PostRender();
		
		cBoss2D->RenderCollider();
	}
}

void CEntityManager::RenderBullets(void) {
	for (unsigned i = 0; i < m_eBulletList.size(); i++) {
		m_eBulletList[i]->PreRender();
		m_eBulletList[i]->Render();
		m_eBulletList[i]->PostRender();
		
		m_eBulletList[i]->RenderCollider();
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
	// Call the cPlayer2D's update method before Map2D as we want to capture the inputs before map2D update
	cPlayer2D->Update(dElapsedTime);

	if (cBoss2D)
		cBoss2D->Update(dElapsedTime);

	for (unsigned i = 0; i < m_cloneList.size(); ++i)
	{
		m_cloneList[i]->Update(dElapsedTime);
	}

	// Call all the cEnemy2D's update method before Map2D 
	// as we want to capture the updates before map2D update
	for (unsigned i = 0; i < m_enemyList.size(); i++) {
		m_enemyList[i]->Update(dElapsedTime);

		//Delete conditions
		if (m_enemyList[i]->GetHealth() <= 0) {
			delete m_enemyList[i];
			m_enemyList[i] = nullptr;
		}
	}

	m_enemyList.erase(std::remove(m_enemyList.begin(), m_enemyList.end(), nullptr), m_enemyList.end()); //Remove any nullptrs in the array

	//Call enemy bullets
	for (unsigned i = 0; i < m_eBulletList.size(); i++) {
		m_eBulletList[i]->Update(dElapsedTime);

		EnemyBullet2D* eBullet = dynamic_cast<EnemyBullet2D*>(m_eBulletList[i]);
		if (eBullet && (eBullet->OutOfWorld() || eBullet->GetHealth() <= 0)) 
		{
			cout << "bullet deleted" << endl;
			delete m_eBulletList[i];
			m_eBulletList[i] = nullptr;
		}
		if (dynamic_cast<Projectiles*>(m_eBulletList[i]))
		{
			if (dynamic_cast<Projectiles*>(m_eBulletList[i])->bDestroyed || dynamic_cast<Projectiles*>(m_eBulletList[i])->bOutsideBoundary())
			{
				cout << "bullet deleted" << endl;
				delete m_eBulletList[i];
				m_eBulletList[i] = nullptr;
			}
		}
	}

	//Remove any nullptrs in bullet array
	m_eBulletList.erase(std::remove(m_eBulletList.begin(), m_eBulletList.end(), nullptr), m_eBulletList.end());

	
	//Keyboard inputs
	if (cKeyboardController->IsKeyPressed(GLFW_KEY_C))
	{
		Clone();
	}
}

void CEntityManager::PushBullet(CObject2D* bullet) {
	m_eBulletList.push_back(bullet);
}

void CEntityManager::PushInteractables(Interactables* interactable)
{
	m_interactableList.push_back(interactable);
}
