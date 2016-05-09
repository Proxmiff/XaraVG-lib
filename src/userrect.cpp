


#include "camtypes.h"
#include "userrect.h"

DECLARE_SOURCE("$Revision$");


/**********************************************************************************************

>    BOOL UserRect::IsAdjacent() const

     Author: 	Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
     Created:   14/7/93
     Inputs:	The Fuzzy limit in MILLIPOINTS.  
     Outputs:   - 
     
     Returns:   TRUE if the rectangles are adjacent within a Fuzzy limit.
     			
     Purpose:	To check for adjacent rectangles.

     Errors:	

**********************************************************************************************/  

BOOL UserRect::IsAdjacent(const UserRect& Rect, MILLIPOINT Fuzzy) const
{
	// Check for an an empty rectangle
	if (IsEmpty())
		return FALSE;

    // Detect an invalid rectangle
	ENSURE(IsValid(), "UserRect::IsAdjacent() was called on\nan invalid rectangle.");  

#if 0
	// This is the expansion of the rampant return statement below

	// Near top or bottom
	BOOL NearBot = (ABS(Rect.hi.y - lo.y)) <= Fuzzy;
	BOOL NearTop = (ABS(Rect.lo.y - hi.y)) <= Fuzzy;

	// Near left or right
	BOOL NearLeft  = (ABS(Rect.hi.x - lo.x)) <= Fuzzy;
	BOOL NearRight = (ABS(Rect.lo.x - hi.x)) <= Fuzzy;

	// Overlaps
	BOOL OverLeft  = (ABS(Rect.lo.x - lo.x)) <= Fuzzy;
	BOOL OverRight = (ABS(Rect.hi.x - hi.x)) <= Fuzzy;
	BOOL OverTop   = (ABS(Rect.hi.y - hi.y)) <= Fuzzy;
	BOOL OverBot   = (ABS(Rect.lo.y - lo.y)) <= Fuzzy;

	// Adjacent to the top or bottom?
	BOOL TopOrBot    = (NearTop || NearBot) && OverLeft && OverRight;
	BOOL LeftOrRight = (NearLeft || NearRight) && OverTop && OverBot;

	return (TopOrBot || LeftOrRight);
#endif

	// This is explained more carefully in the section above that is excluded from the build
	return (( ((ABS(Rect.lo.x - lo.x))<=Fuzzy) && ((ABS(Rect.hi.x - hi.x))<=Fuzzy) &&
			 (((ABS(Rect.hi.y - lo.y))<=Fuzzy) || ((ABS(Rect.lo.y - hi.y))<=Fuzzy)) ) ||
	        ( ((ABS(Rect.lo.y - lo.y))<=Fuzzy) && ((ABS(Rect.hi.y - hi.y))<=Fuzzy) &&
			 (((ABS(Rect.hi.x - lo.x))<=Fuzzy) || ((ABS(Rect.lo.x - hi.x))<=Fuzzy)) ));
}



/*********************************************************************************************

>    UserRect UserRect::Union(const UserRect& R)

     Author: 	Tim_Browse (Xara Group Ltd) <camelotdev@xara.com>
     Created:   17/5/93
     Inputs:	R - the rectangle to join with the object.
     Outputs:   - 
     
     Returns:   The simple union (bounding box).
     			
     Purpose:	Compute union of two rectangles.
     			
     			If one of the rectangles involved is invalid, the other is returned as the
     			result.  In the case of both being invalid, one of the invalid rectangles
     			is returned as the result.  It is undefined which rectangle is returned in
     			this case.

     Errors:	None.

**********************************************************************************************/  

UserRect UserRect::Union(const UserRect& R) const
{
	// Special cases for invalid rectangles...
	if (!IsValid()  ||  IsEmpty())
		return R;
	if (!R.IsValid()  ||  R.IsEmpty())
		return *this;
		
	// Return the result
	UserRect U;
	U.lo.x = min(lo.x, R.lo.x);
	U.lo.y = min(lo.y, R.lo.y);
	U.hi.x = max(hi.x, R.hi.x);
	U.hi.y = max(hi.y, R.hi.y);
	return U;
}



/*********************************************************************************************

>    UserRect UserRect::Intersection(const UserRect& R)

     Author: 	Tim_Browse (Xara Group Ltd) <camelotdev@xara.com>
     Created:   17/5/93
     Inputs:	R - the rectangle to intersect with.
     Outputs:   - 
     
     Returns:   The intersection.
     			
     Purpose:	Compute intersection of two rectangles.

     			If one of the rectangles involved is invalid, the other is returned as the
     			result.  In the case of both being invalid, one of the invalid rectangles
     			is returned as the result.  It is undefined which rectangle is returned in
     			this case.

     Errors:	None.

**********************************************************************************************/  

UserRect UserRect::Intersection(const UserRect& R) const
{
	// Special cases for invalid rectangles...
	if (!IsValid())
		return R;
	if (!R.IsValid())
		return *this;
	
	// Special cases when empty rectangles
	if (IsEmpty())
		return *this;
	if (R.IsEmpty())
		return R;

	// Return the result
	UserRect I;
	I.lo.x = max(lo.x, R.lo.x);
	I.lo.y = max(lo.y, R.lo.y);
	I.hi.x = min(hi.x, R.hi.x);
	I.hi.y = min(hi.y, R.hi.y);
	return I;
}



/*********************************************************************************************

>    INT32 UserRect::SplitRect(const UserRect& R, UserRect* SubRects)

     Author: 	Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
     Created:   14/7/93
     Inputs:	R is the rectangle to intersect with, SubRects is an array of UserRect to fill.  
     Outputs:   Puts any sub-rects into SubRects. 
     
     Returns:   The number of subrectangles found. 0 for no intersection.
     			
     Purpose:	Splits a rectangle into sub-rectangles which all exclude the interecting
     			rectangle R.  The number of sub-rects generated depends on how the rectangles
     			intersect.
     				 _______________________       	 ___________________
	            	|                       |      	|                   |
     				| *this    A            |      	| *this    A        |
     				|_ _ _  ________  _ _ _ |      	|_ _ _ _ _  ________|__________
     				|      |        |       |      	|          |        |          |
     				|  B   |   R    |   C   |      	|    B     |        |          |
     				|_ _ _ |________| _ _ _ |      	|__________|________| R        |
     				|                       |      	           |                   |
                    |          D    Index=4 |      	           |           Index=2 |
     				|_______________________|      	           |___________________|
     Errors:	

**********************************************************************************************/  

INT32 UserRect::SplitRect(const UserRect& R, UserRect* SubRects)
{
	if (!this->IsIntersectedWith(R))
		return 0;	// No intersection  

	INT32 Index = 0;

	if (R.hi.y < hi.y)
		SubRects[Index++] = UserRect(lo.x, R.hi.y, hi.x, hi.y);	// Sub-Rect A
		
	if (R.lo.y > lo.y)
		SubRects[Index++] = UserRect(lo.x, lo.y, hi.x, R.lo.y);	// Sub-Rect D

	INT32 Ry0 = max(R.lo.y, lo.y);
	INT32 Ry1 = min(R.hi.y, hi.y);
	
	if (R.lo.x > lo.x)
		SubRects[Index++] = UserRect(lo.x, Ry0, R.lo.x, Ry1);			// Sub-Rect B		

	if (R.hi.x < hi.x)
		SubRects[Index++] = UserRect(R.hi.x, Ry0, hi.x, Ry1);			// Sub-Rect C		

	return Index;
}



/***********************************************************************************************

>	void UserRect::IncludePoint(const UserCoord& Point)

	Author:		Tim_Browse (Xara Group Ltd) <camelotdev@xara.com>
	Created:	27/5/93
	Inputs:		Point - coordinate to include into this rectangle.
	Outputs:	-
	Returns:	-
	Purpose:	Expand a rectangle so that it includes the given point.
	Errors:		An assertion failure if the rectangle is invalid.
	SeeAlso:	-

***********************************************************************************************/

void UserRect::IncludePoint(const UserCoord& Point)
{
    // Detect an invalid rectangle
	ENSURE(IsValid(), "UserRect::IncludePoint() was called on an \ninvalid rectangle.");  

	// Extend lower corner to include point if necessary
	lo.x = min(lo.x, Point.x);
	lo.y = min(lo.y, Point.y);
	
	// Extend upper corner to include point if necessary
	// (remember upper corner is exclusive)
	hi.x = max(hi.x, (INT32)(Point.x + 1L));
	hi.y = max(hi.y, (INT32)(Point.y + 1L));
}

// extra functions specific to the DocRect class

/*
$Log: userrect.def $
Revision 1.1  1995/09/12  17:11:49  Ed
Initial revision

*/

DECLARE_SOURCE("$Revision$");

//#include "docrect.h" - in camtypes.h [AUTOMATICALLY REMOVED]

/********************************************************************************************
>	DocRect UserRect::ToSpread(Spread* pSpread)

	Author:		Ed_Cornes (Xara Group Ltd) <camelotdev@xara.com>
	Created:	6/9/95
	Inputs:		pSpread - 
	Returns:	SpreadRect
	Purpose:	Convert a UserRect to a SpreadRect (AKA DocRect)
********************************************************************************************/

DocRect UserRect::ToSpread(Spread* pSpread)
{
	return DocRect( this->lo.ToSpread(pSpread), this->hi.ToSpread(pSpread) );
}


