#include "InventoryItem.h"
#include <stdexcept>      // std::invalid_argument

#include <iostream>

/**
@brief Constructor
*/
CInventoryItem::CInventoryItem(unsigned int iTextureID)
	: iItemCount(0)
	, iItemMaxCount(0)
	, vec2Size(glm::vec2(0.0f))
{
	this->iTextureID = iTextureID;
}

/**
@brief Destructor
*/
CInventoryItem::~CInventoryItem(void)
{
}

/**
@brief Add a number of items from this class instance
*/
void CInventoryItem::Add(const int iNumItems)
{
	iItemCount += iNumItems;
	if (iItemCount > iItemMaxCount)
		iItemCount = iItemMaxCount;
}

/**
@brief Remove the number of items from this class instance
*/
void CInventoryItem::Remove(const int iNumItems)
{
	iItemCount -= iNumItems;
}

/**
@brief Get the number of items from this class instance
*/
int CInventoryItem::GetCount(void) const
{
	return iItemCount;
}

/**
@brief  Get the maximum number of items from this class instance
*/
int CInventoryItem::GetMaxCount(void) const
{
	return iItemMaxCount;
}

/**
@brief Get the texture ID
*/
unsigned int CInventoryItem::GetTextureID(void) const
{
	return iTextureID;
}
