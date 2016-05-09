
#include "camtypes.h"
#include "cachfifo.h"
#include "objcache.h"
#include "cache.h"


DECLARE_SOURCE( "$Revision$" );


CC_IMPLEMENT_DYNCREATE(ObjectCacheFIFO, ObjectCache)

// Declare smart memory handling in Debug builds
#define new CAM_DEBUG_NEW


/*********************************************************************************************

>	ObjectCacheFIFO::ObjectCacheFIFO()

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/01/97
	Purpose:	default constructor for ObjectCacheFIFO. First contains the handle of the first
				object in the hash table.
	Note:		
	SeeAlso:	
**********************************************************************************************/
ObjectCacheFIFO::ObjectCacheFIFO()
{
	m_Ceiling = 0;
	m_first = 1;
}

/*********************************************************************************************

>	ObjectCacheFIFO::ObjectCacheFIFO(UINT32 ceiling)

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/01/97
	Inputs:		ceiling: max size of available memory for the cache
	Purpose:	constructor for ObjectCacheFIFO
	Note:		
	SeeAlso:	
**********************************************************************************************/
ObjectCacheFIFO::ObjectCacheFIFO(UINT32 ceiling)
{
	m_first = 1;
	m_Ceiling = ceiling;
}

/*********************************************************************************************

>	ObjectCacheFIFO::~ObjectCacheFIFO()

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/01/97
	Purpose:	destructor for ObjectCache
	Note:		
	SeeAlso:	
**********************************************************************************************/
ObjectCacheFIFO::~ObjectCacheFIFO()
{
}


/*********************************************************************************************

>	BOOL ObjectCacheFIFO::Delete()

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/01/97
	Return:		TRUE if success (an object is deleted), FALSE otherwise.
	Purpose:	FIFO Cache replacement policy. 
	Note:		
	SeeAlso:	
**********************************************************************************************/

BOOL ObjectCacheFIFO::DeleteObject()
{
	CachedObject* pObj = NULL;

	if (m_NumObjects == 0)
		return FALSE;

	WORD hObj = m_first;					// First element in the hash table, 
											// corresponds to the first generated handle.
	pObj = LookUp(hObj);
	BOOL ok = (pObj != NULL);

	if (ok)
	{
		Remove(hObj);						// remove from the hash table
		m_NumObjects--;						// decrease the number of objects
		m_CurrentSize -= pObj->GetSize();	// decrease the used space of cache

		delete pObj;
		pObj = NULL;

		if (m_NumObjects >= 1)
			m_first++;

		return TRUE;
	}

	return FALSE;
}
