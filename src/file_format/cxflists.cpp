
// Implements bits of the stuff defined in cxflists.h

#include "camtypes.h"

#include "cxflists.h"

#include "cxftags.h"


//----------------------------------------------------------------------------------------------

/***********************************************************************************************

>	InsertLevelStackItem::InsertLevelStackItem(BaseCamelotFilter* pThisFilter)

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	2/9/96
	Inputs:		pThisFilter = ptr to the filter this item is used by
	Returns:	-
	Purpose:	The default constructor

***********************************************************************************************/

InsertLevelStackItem::InsertLevelStackItem(BaseCamelotFilter* pThisFilter)
{ 
	NumNodesInserted	= 0; 
	pLastInsertedNode	= NULL; 
	pLastHandler		= NULL; 
	LastHandledTag		= TAG_UNDEFINED;
	pFilter				= pThisFilter;
	LastNodeInformedChildrenComplete = FALSE;

	ERROR3IF(pFilter == NULL,"pFilter is NULL");
}

/***********************************************************************************************

>	BOOL InsertLevelStackItem::SetLastInsertedNode(Node* pNode)

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	18/9/96
	Inputs:		pNode = ptr to node just inserted into the tree
	Returns:	TRUE if ok, FALSE if a failure has ocurred
	Purpose:	Sets the last inserted node of this tree

***********************************************************************************************/

BOOL InsertLevelStackItem::SetLastInsertedNode(Node* pNode)
{
	ERROR3IF(pNode == NULL,"pNode is NULL");

	if (pNode != NULL)
	{
		if (pNode->FindPrevious() == pLastInsertedNode)
		{
			if (!InformLastInsertedNodeChildrenComplete())
				return FALSE;
		}

		pLastInsertedNode = pNode;
		LastNodeInformedChildrenComplete = FALSE;

		IncNumNodesInserted();			// Inc the number of nodes on this level
	}

	return TRUE;
}

/***********************************************************************************************

>	BOOL InsertLevelStackItem::InformLastInsertedNodeChildrenComplete()

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	18/9/96
	Inputs:		-
	Returns:	TRUE if ok, FALSE if a failure has ocurred
	Purpose:	Tells the last inserted node that its children are complete

***********************************************************************************************/

BOOL InsertLevelStackItem::InformLastInsertedNodeChildrenComplete()
{
	ERROR3IF(pFilter == NULL,"pFilter is NULL");

	if (!LastNodeInformedChildrenComplete && pLastInsertedNode != NULL && pFilter != NULL)
	{
		if (!pFilter->ReadPostChildren(pLastInsertedNode))
			return FALSE;

		LastNodeInformedChildrenComplete = TRUE;
	}

	return TRUE;
}


//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------

/***********************************************************************************************

>	InsertLevelStack::InsertLevelStack();

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	3/9/96
	Inputs:		-
	Returns:	-
	Purpose:	The default constructor

***********************************************************************************************/

InsertLevelStack::InsertLevelStack()
{
	InsertLevel = 0;
}

/***********************************************************************************************

>	InsertLevelStack::~InsertLevelStack();

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	3/9/96
	Inputs:		-
	Returns:	-
	Purpose:	The default destructor - deletes all the list's items

***********************************************************************************************/

InsertLevelStack::~InsertLevelStack()
{
	DeleteAll();
}

/***********************************************************************************************

>	void InsertLevelStack::Add(InsertLevelStackItem* pItem)

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	3/9/96
	Inputs:		pItem
	Returns:	-
	Purpose:	Adds the item to the top of the stack

***********************************************************************************************/

void InsertLevelStack::Add(InsertLevelStackItem* pItem)
{
	InsertLevel++;
	AddTail(pItem);
}

/***********************************************************************************************

>	InsertLevelStackItem* InsertLevelStack::Get()

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	3/9/96
	Inputs:		-
	Returns:	ptr to a InsertLevelStackItem 
	Purpose:	Returns a ptr to the item on the top of the stack, without removing it

***********************************************************************************************/

InsertLevelStackItem* InsertLevelStack::Get()
{
	return (InsertLevelStackItem*)List::GetTail();
}

/***********************************************************************************************

>	InsertLevelStackItem* InsertLevelStack::Pop()

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	3/9/96
	Inputs:		-
	Returns:	ptr to a InsertLevelStackItem 
	Purpose:	Removes the item from the top of the stack, and returns a ptr to it

***********************************************************************************************/

InsertLevelStackItem* InsertLevelStack::Pop()
{
	if (InsertLevel == 0)
	{
		ERROR3("InsertLevel == 0");
		return NULL;
	}

	InsertLevel--;

	return (InsertLevelStackItem*)List::RemoveTail();
}

