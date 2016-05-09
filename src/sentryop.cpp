

#include "camtypes.h"
#include "sentryop.h" 


DECLARE_SOURCE("$Revision$");
CC_IMPLEMENT_DYNCREATE(SentryOperation, Operation)

#define new CAM_DEBUG_NEW                                                                    

/********************************************************************************************

>	SentryOperation::SentryOperation()

	Author:		Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
	Created:	15/2/94
	Inputs:		-
	Outputs:	-
	Returns:	-
	Purpose:	SentryOperation constructor, calls the constructor of the base  class
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

SentryOperation::SentryOperation(): Operation()
{
}

/********************************************************************************************

>	SentryOperation::~SentryOperation()

	Author:		Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
	Created:	15/2/94
	Inputs:		-
	Outputs:	-
	Returns:	-
	Purpose:	SentryOperation destructor 
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

SentryOperation::~SentryOperation()
{
}

