
// UserCoords - ie SpreadCoords relative to the user specified origin

#include "camtypes.h"
#include "usercord.h"

DECLARE_SOURCE("$Revision$");

// Declare smart memory handling in Debug builds
#define new CAM_DEBUG_NEW


/********************************************************************************************
>	DocCoord UserCoord::ToSpread(Spread* pSpread)

	Author:		Ed_Cornes (Xara Group Ltd) <camelotdev@xara.com>
	Created:	5/9/95
	Inputs:		pSpread -
	Returns:	SpreadCoord
	purpose:	Convert between UserCoords and SpreadCoords (AKA DocCoords - Aarrgghh!)
********************************************************************************************/

DocCoord UserCoord::ToSpread(Spread* pSpread)
{
	DocCoord UserOrigin(0,0);
	if (pSpread!=NULL)
		UserOrigin=pSpread->GetUserOrigin();
	else
		ERROR3("UserCoord::ToSpread() - pSpread==NULL");

	return DocCoord(this->x+UserOrigin.x, this->y+UserOrigin.y);
}
