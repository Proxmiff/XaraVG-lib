
// The base class of the transforms

#include "camtypes.h"

// file version number
DECLARE_SOURCE("$Revision$");

// An implement to match the Declare in the .h file.
CC_IMPLEMENT_DYNAMIC(TransformBase, CCObject)
CC_IMPLEMENT_DYNAMIC(TransInvertable, TransformBase)
CC_IMPLEMENT_DYNAMIC(TransNonInvertable, TransformBase)

// This will get Camelot to display the filename and linenumber of any memory allocations
// that are not released at program exit
// Declare smart memory handling in Debug builds
#define new CAM_DEBUG_NEW





/********************************************************************************************

>	FIXED16 TransformBase::GetScalar()

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	09/01/2004
	Returns:	FIXED16 value of the equivalent scale factor for the transform.
	Purpose:	Returns the Transforms idea of the equivalent scale factor for the transform.
				This is mainly used to help with the scaling of line widths to save them
				trying to extract the scale factor from the matrix.

********************************************************************************************/

FIXED16 TransformBase::GetScalar()
{
	// Just return the current value of the scale factor
	return FIXED16(1.0);
}
