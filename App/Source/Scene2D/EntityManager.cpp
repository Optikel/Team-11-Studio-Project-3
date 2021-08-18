#include "EntityManager.h"


CEntityManager::CEntityManager()
	: cPlayer2D(NULL)
	, cEnemy2D(NULL)
	, cMap2D(NULL)
	, cCloneTemplate(NULL)
{
}

CEntityManager::~CEntityManager()
{
}

bool CEntityManager::EntityManagerInit(void)
{
	// Create and initialise the Map 2D
	cMap2D = CMap2D::GetInstance();
	// Set a shader to this class
	cMap2D->SetShader("2DShader");
	// Store the keyboard controller singleton instance here
	cKeyboardController = CKeyboardController::GetInstance();
	cKeyboardInputHandler = CKeyboardInputHandler::GetInstance();

	unsigned int uiRow = -1;
	unsigned int uiCol = -1;

	//player init
	if (cMap2D->FindValue(1, uiRow, uiCol) == true)
	{
		cPlayer2D = new CPlayer2D;
		// Pass shader to cPlayer2D
		cPlayer2D->SetShader("2DColorShader");
		cPlayer2D->collidable = true;
		// m_playerList.push_back(cPlayer2D);
		// Initialise the instance
		if (cPlayer2D->Init() == false)
		{
			cout << "Failed to load CPlayer2D" << endl;
			return false;
		}
	}

	//enemy init
	if (cMap2D->FindValue(300, uiRow, uiCol) == true)
	{
		cEnemy2D = new CEnemy2D;
		cEnemy2D->collidable = true;
		cEnemy2D->SetShader("2DColorShader");
		m_enemyList.push_back(cEnemy2D);
		if (cEnemy2D->Init() == false)
		{
			cout << "Failed to load CEnemy2D" << endl;
			return false;
		}
	}

	//clone init
	cCloneTemplate = new CPlayer2D();
	if (cCloneTemplate->Init(cPlayer2D->GetCheckpoint()) == false)
	{
		cout << "Failed to load clone" << endl;
		return false;
	}
	// initialise all default values
	cCloneTemplate->SetClone(true);
	cCloneTemplate->SetShader("2DColorShader");
	// find position to spawn in map
	if (cMap2D->FindValue(1, uiRow, uiCol) == true)
	{
		cCloneTemplate->vTransform = glm::vec2(uiCol, uiRow);
	}
}


bool CEntityManager::Clone(void)
{
	CPlayer2D* clone = new CPlayer2D(*cCloneTemplate);
	clone->SetShader("2DColorShader");

	if (!clone->Init(cPlayer2D->GetCheckpoint()))
	{
		std::cout << "Failed to clone Player\n";
		return false;
	}
	clone->SetInputs(cKeyboardInputHandler->GetAllInputs());
	m_cloneList.push_back(clone);

	return true;
}


bool CEntityManager::CheckCollision(CEntity2D* type1, CEntity2D* type2)
{
	if (type1->type == CEntity2D::PLAYER && type2->type == CEntity2D::ENEMY)
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

void CEntityManager::RenderEnemy(void)
{
	for (int i = 0; i < m_enemyList.size(); i++)
	{
		m_enemyList[i]->PreRender();
		m_enemyList[i]->Render();
		m_enemyList[i]->PostRender();
	}
}

void CEntityManager::RenderClone(void)
{
	for (int i = 0; i < m_cloneList.size(); ++i)
	{
		m_cloneList[i]->PreRender();
		m_cloneList[i]->Render();
		m_cloneList[i]->PostRender();
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

	for (int i = 0; i < m_cloneList.size(); ++i)
	{
		m_cloneList[i]->Update(dElapsedTime);
	}

	// Call all the cEnemy2D's update method before Map2D 
	// as we want to capture the updates before map2D update
	for (int i = 0; i < m_enemyList.size(); i++)
	{
		if (static_cast<CEnemy2D*>(m_enemyList[i])->GetHealth() < 0)
		{
			delete m_enemyList[i];
			m_enemyList.erase(m_enemyList.begin() + i);
		}
	}
	for (int i = 0; i < m_enemyList.size(); i++)
	{
		m_enemyList[i]->Update(dElapsedTime);
	}

	if (cKeyboardController->IsKeyPressed(GLFW_KEY_C))
	{
		Clone();
	}
	
	
	//for (std::vector<CEntity2D*>::iterator it = m_playerList.begin(); it != m_playerList.end(); ++it)
	//{
	//	CEntity2D* player = (CEntity2D*)*it;
	//	{
	//		for (std::vector<CEntity2D*>::iterator it2 = m_enemyList.begin(); it2 != m_enemyList.end(); ++it2)
	//		{
	//			CEntity2D* enemy = (CEntity2D*)*it2;
	//			if (CheckCollision(player, enemy))
	//			{
	//				//reduce health or some shit
	//			}
	//		}
	//	}
	//}
}




