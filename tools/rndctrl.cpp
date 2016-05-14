

// RNDCTRL.CPP


#include "camtypes.h"
#include "rndctrl.h"

DECLARE_SOURCE("$Revision$");

/********************************************************************************************

>	RenderControl::RenderControl()

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/5/93
	Inputs:		-
	Outputs:	-
	Returns:	-
	Purpose:	Default Constructor for RenderControl Class
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

RenderControl::RenderControl()
{
}

/********************************************************************************************

>	BOOL RenderControl::StopRendering()

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/5/93
	Inputs:		-
	Outputs:	-
	Returns:	TRUE if you should stop
	Purpose:	Called by a rendering routine to determine whether it should stop or not
	Errors:		-
	SeeAlso:	-

********************************************************************************************/


BOOL RenderControl::StopRendering()
{
	return FALSE;	// Force foreground rendering for now !!
}
