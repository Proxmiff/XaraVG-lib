
// StringListItem class

#include "camtypes.h"
#include "strlist.h"

CC_IMPLEMENT_DYNAMIC(StringListItem, ListItem)

/********************************************************************************************

	StringListItem::StringListItem()

	Author:		Graham_Walmsley (Xara Group Ltd) <camelotdev@xara.com>
	Created:	15/4/97
	Inputs:		-
	Outputs:	-
	Purpose:	Default contructor
	
********************************************************************************************/

StringListItem::StringListItem()
{
	//Set our member variable to an empty string
	m_String.Empty();
}

/********************************************************************************************

	StringListItem::StringListItem(const String_256& strToCopy)

	Author:		Graham_Walmsley (Xara Group Ltd) <camelotdev@xara.com>
	Created:	15/4/97
	Inputs:		strToCopy	String to put into our member variable
	Outputs:	-
	Purpose:	Specific contructor
	
********************************************************************************************/

StringListItem::StringListItem(const String_256& strToCopy)
{
	m_String=strToCopy;
}

