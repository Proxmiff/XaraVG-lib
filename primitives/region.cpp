

// REGION.CPP
//
// Created: 08/6/93 by Will
//

#include "camtypes.h"
#include "region.h"

DECLARE_SOURCE("$Revision$");

/********************************************************************************************

>	Region::Region()

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	08/6/93
	Inputs:		-
	Outputs:	-
	Returns:	-
	Purpose:	Default Constructor for Region Class. Creats a Null region.
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

Region::Region()
{
	CurrentRegion.lox = 0;
	CurrentRegion.loy = 0;
	CurrentRegion.hix = 0;
	CurrentRegion.hiy = 0;
}

/********************************************************************************************

>	Region::Region(DocRect RectRegion)

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	08/6/93
	Inputs:		A DocRect describing the bounding rectangle of the region.
	Outputs:	-
	Returns:	-
	Purpose:	Creates a Region from the specified rectangle.
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

Region::Region(DocRect RectRegion)
{
	CurrentRegion = RectRegion;
}

/********************************************************************************************

>	BOOL Region::MergeRegion(Region *RegionToMerge)

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	8/6/93
	Inputs:		A pointer to a region to merge
	Outputs:	-
	Returns:	TRUE if merged OK, or FALSE if failed.
	Purpose:	Merges one region with another.
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

BOOL Region::MergeRegion(Region *RegionToMerge)
{
	return FALSE;
}
