

#include "camtypes.h"
#include "cache.h"


DECLARE_SOURCE( "$Revision$" );


CC_IMPLEMENT_DYNCREATE(CachedObject, CCObject)

#if _DEBUG
CC_IMPLEMENT_DYNCREATE(DummyCachedObject, CachedObject)
#endif

// Declare smart memory handling in Debug builds
#define new CAM_DEBUG_NEW


/*********************************************************************************************

>	CachedObject::CachedObject()

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	20/01/97
	Purpose:	default constructor for CachedObject
				We get the size object: sizeof(this). It's used by the delete algorithms.
	Note:		
	SeeAlso:	
**********************************************************************************************/
CachedObject::CachedObject()
{
	m_Size = sizeof(this);
	m_Immortal = FALSE;
	m_hObj = 0;
}


/*********************************************************************************************

>	CachedObject::CachedObject(INT32 size)

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	20/01/97
	Purpose:	constructor for CachedObject. It's used mainly in debug build when I explicitly
				set the size of the object rather than using sizeof().
	Note:		
	SeeAlso:	
**********************************************************************************************/

CachedObject::CachedObject(INT32 size)
{
	m_Size = size;
	m_Immortal = FALSE;
	m_hObj = 0;
}


/*********************************************************************************************

>	CachedObject::CachedObject(BOOL immortal, UINT32 size)

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	20/01/97
	Purpose:	constructor for CachedObject.It's used mainly in debug build when I explicitly
				set the size of the object rather than using sizeof(). It also useful if I need
				to specify whether the objwect is immortal or not.
				The second parameter is optionnal.
				(e.g. CachedObject *pDummyObject = new CachedObject(TRUE);
				or:		CachedObject *pDummyObject = new CachedObject(TRUE, 100);
	Note:		
	SeeAlso:	
**********************************************************************************************/
CachedObject::CachedObject(BOOL immortal, UINT32 size)
{
	m_Size = size;
	m_Immortal = immortal;
	m_hObj = 0;
}


/*********************************************************************************************

>	CachedObject::~CachedObject()

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	20/01/97
	Purpose:	destructor for CachedObject
	Note:		
	SeeAlso:	
**********************************************************************************************/
CachedObject::~CachedObject()
{
}


/*********************************************************************************************

>	CachedObject::StoreHandle(CACHE_HANDLE Handle)

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	20/01/97
	Inputs:		Handle: the object handle
	Return:		TRUE
	Purpose:	store the CachedObject Handle generated by the add() function
	Note:		
	SeeAlso:	
**********************************************************************************************/

BOOL CachedObject::StoreHandle(CACHE_HANDLE Handle)
{
	m_hObj = Handle;

	return TRUE;
}


/*********************************************************************************************

>	BOOL CachedObject::SetSize(UINT32 NewSize)

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	03/02/97
	Purpose:	modify the size of the object
	Note:		
	SeeAlso:	
**********************************************************************************************/

BOOL CachedObject::SetSize(UINT32 NewSize)
{
	m_Size = NewSize;
	return TRUE;
}


//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////
// DummyCachedObject
// Class used only for debug
//////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////


/*********************************************************************************************

>	DummyCachedObject::DummyCachedObject()

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/01/97
	Purpose:	Dummy class code
	Note:		
	SeeAlso:	
**********************************************************************************************/

#if _DEBUG

DummyCachedObject::DummyCachedObject()
{
	m_DataMembers = 0;
}

DummyCachedObject::DummyCachedObject(INT32 size):CachedObject(size)
{
	m_DataMembers = 0;
}

DummyCachedObject::DummyCachedObject(BOOL immortal, UINT32 size):CachedObject(immortal, size)
{
	m_DataMembers = 0;
}

DummyCachedObject::~DummyCachedObject()
{
}

#endif
