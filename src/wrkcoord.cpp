


#include "camtypes.h" 

/********************************************************************************************

>	OilCoord WorkCoord::ToOil(const WorkCoord& scrollOffset)

	Author: 	Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/8/93
	Inputs:		Scroll offset.
	Outputs:	None
	Returns:	None.
	Purpose:	Converts a WorkCoord to an OilCoord.
	Errors:		None.

********************************************************************************************/

OilCoord WorkCoord::ToOil(const WorkCoord& scrollOffset)
{
	OilCoord result( INT32(x - scrollOffset.x), INT32(y - scrollOffset.y) );
	
	return result;
}

/********************************************************************************************

>	void WorkCoord::ToOil(const WorkCoord& scrollOffset, OilCoord *result)

	Author: 	Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/8/93
	Inputs:		Scroll offset.
	Outputs:	None
	Returns:	None.
	Purpose:	Converts a WorkCoord to an OilCoord.
	Errors:		None.

********************************************************************************************/

void WorkCoord::ToOil(const WorkCoord& scrollOffset, OilCoord *result)
{
	result->x = INT32(x - scrollOffset.x); 
	result->y = INT32(y - scrollOffset.y);
}
 
