
// Implementation of the Coord class

#include "camtypes.h"


/********************************************************************************************

> double Coord::Distance(const Coord& Coord) const

	Author: 	Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/3/94
	Inputs:		A coord
	Outputs:	None
	Returns:	the distance between this coord and the given coord
	Purpose:	Accurate distance function.
	Errors:		None.

********************************************************************************************/

double Coord::Distance(const Coord& Coord) const
{
	double dx 		= (double) (this->x-Coord.x);
	double dy 		= (double) (this->y-Coord.y);
	double sumsqrs 	= (dx*dx)+(dy*dy);
	
	return (sqrt(sumsqrs));
}

