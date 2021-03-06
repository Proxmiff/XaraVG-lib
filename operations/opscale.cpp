
// Operation for doing an interactive scale of the selection


#include "camtypes.h"
#include "opscale.h"


#include "selector.h"

// This is not compulsory, but you may as well put it in so that the correct version
// of your file can be registered in the .exe
DECLARE_SOURCE("$Revision$");

// An implement to match the Declare in the .h file.
// If you have many classes, it is recommended to place them all together, here at the start of the file
CC_IMPLEMENT_DYNCREATE(OpScaleTrans, TransOperation)

// This will get Camelot to display the filename and linenumber of any memory allocations
// that are not released at program exit
#define new CAM_DEBUG_NEW





/********************************************************************************************

>	OpScaleTrans::OpScaleTrans()

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	20/7/93
	Purpose:	Constructor. Does nothing.

********************************************************************************************/

OpScaleTrans::OpScaleTrans(): TransOperation()
{
	StatusHelpID = _R(IDS_SCALETRANS_STATUS1);
	StatusHelpID2 = _R(IDS_SCALETRANS_STATUS2);
}





/********************************************************************************************
>	virtual void OpScaleTrans::InitTransformImmediate(OpParam* pParam)

	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	4/11/94
	Inputs:		pParam		---		pointer an OpParam whose Param2 member is a pointer
									to two FIXED16's that represent the scale factor for
									the width and height of the selction.
	Outputs:	-
	Returns:	-
	Purpose:	
	Errors:		-
	SeeAlso:	-
********************************************************************************************/

void OpScaleTrans::InitTransformImmediate(OpParam* pParam)
{

	FIXED16* pfx = (FIXED16*) PVOID(pParam->Param2);
	XScaleFactor = *pfx;
	YScaleFactor = *(pfx + 1);


	if (XScaleFactor == 0)
		XScaleFactor = 1.0;

	if (YScaleFactor == 0)
		YScaleFactor = 1.0;

}






/********************************************************************************************

>	virtual void OpScaleTrans::InitTransformOnDrag(DocCoord PointerPos, ClickModifiers ClickMods)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	19/7/94
	Inputs:		PointerPos - The position of the mouse at the start of the drag
				ClickMods - the keyboard modifiers that were active at the start of the drag
	Purpose:	Sets up the parameters needed to build the transform matrix at the start
				of the drag. This base class version of this function does nothing.

********************************************************************************************/

void OpScaleTrans::InitTransformOnDrag(DocCoord PointerPos, ClickModifiers ClickMods)
{
	// WEBSTER - markn 14/2/97
	// Rewritten so it will work when "snap to the grid" is on

	// Note the pointer pos at the beginning of the drag
	PointerStart.x = PointerPos.x;
	PointerStart.y = PointerPos.y;

	// Note the width & height of the original bounds - used to calc scale factors
	UnitWidth  = BoundingData.Width;
	UnitHeight = BoundingData.Height;

	// Work out which x point of the bounds to take, based on the blob that was clicked on
	switch (StartBlob)
	{
		case 3:
		case 8:	BoundsStartPoint.x = BoundingData.x + BoundingData.Width;
				break;
		case 1:
		case 6:	BoundsStartPoint.x = BoundingData.x;
				UnitWidth = -UnitWidth;					// if low x, then we need -ve x scale factors
				break;

		default: ERROR3("Unrecognised blob values"); break;
	}

	// Work out which x point of the bounds to take, based on the blob that was clicked on
	switch (StartBlob)
	{
		case 1:
		case 3:	BoundsStartPoint.y = BoundingData.y + BoundingData.Height;
				break;
		case 6:
		case 8:	BoundsStartPoint.y = BoundingData.y;
				UnitHeight = -UnitHeight;				// if low y, then we need -ve y scale factors
				break;

		default: ERROR3("Unrecognised blob values"); break;
	}

	// Make sure that they are not zero
	if (UnitWidth==0)	UnitWidth = 1;
	if (UnitHeight==0)	UnitHeight = 1;

	// Set the scale factors to 1 (ie no scale)
	XScaleFactor = FIXED16(1);
	YScaleFactor = FIXED16(1);
}




/********************************************************************************************

>	virtual void OpScaleTrans::UpdateTransformOnDrag(DocCoord PointerPos, Spread*, ClickModifiers&)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	19/7/94
	Inputs:		PointerPos - The latest position of the mouse
				Spread* - The spread the mouse is over
				ClickModifiers - The click modifiers in place now
	Purpose:	Does the calculations needed when the mouse moves to keep all the params
				needed to build the rotation matrix up to date.

********************************************************************************************/

void OpScaleTrans::UpdateTransformOnDrag(DocCoord PointerPos, Spread* pClickSpread, ClickModifiers& ClickMods)
{
	// Make sure that the cursor does not wrap around at the edge of spreads
	if (pClickSpread != StartSpread)
		PointerPos = MakeRelativeToSpread(StartSpread, pClickSpread, PointerPos);

	// Snap the pointer position
	if (!ClickMods.Constrain)
		DocView::SnapSelected(pClickSpread, &PointerPos, FALSE, TRUE);

	// If the Adjust key is down then we should adjust the Unit sizes to compensate
	// for the moving centre of transform
	INT32 WorkingHeight = UnitHeight;
	INT32 WorkingWidth = UnitWidth;

	// if Adjust is being used, adjust the height and width so that scaling still works
	if (ClickMods.Adjust)
	{
		WorkingHeight /= 2;
		WorkingWidth /= 2;
	}

	// WEBSTER - markn 14/2/97
	// Rewritten so it will work when "snap to the grid" is on

	// Work out how far the pointer has moved compared with the initial pointer pos
	INT32 pdx = PointerPos.x - PointerStart.x;
	INT32 pdy = PointerPos.y - PointerStart.y;

	// Move the bounds point by the distance the mouse pointer has moved and
	// calc the snapped version of the new bounds pos
	DocCoord SnappedBoundsPoint(BoundsStartPoint.x + pdx,BoundsStartPoint.y + pdy);

	if (!ClickMods.Constrain)
		DocView::SnapSelected(StartSpread, &SnappedBoundsPoint, FALSE, TRUE);

	// Calc the proportion the bounds have changed compared with the original working dimensions
	INT32 dx = (SnappedBoundsPoint.x - BoundsStartPoint.x) + WorkingWidth;
	INT32 dy = (SnappedBoundsPoint.y - BoundsStartPoint.y) + WorkingHeight;

	// Calc the scale factors as a fraction of the change between the origin & scaled dimensions
	XScaleFactor = FIXED16(double(dx) / double(WorkingWidth));
	YScaleFactor = FIXED16(double(dy) / double(WorkingHeight));

	// Deal with the aspect ratio being locked
	if (LockAspect)
	{
		// If the aspect ratio is locked, X and Y should be the same.
		// Always take the greatest scale factor. 
		// But be careful to retain the sign of the scale factors - only make their
		// magnitudes equal! (Sorry about the yucky implementation of this!)
		if (YScaleFactor > XScaleFactor)
		{
			FIXED16 AbsY = (YScaleFactor < 0 ? -YScaleFactor : YScaleFactor);
			XScaleFactor = (XScaleFactor < 0 ? -AbsY : AbsY);
		}
		else
		{
			FIXED16 AbsX = (XScaleFactor < 0 ? -XScaleFactor : XScaleFactor);
			YScaleFactor = (YScaleFactor < 0 ? -AbsX : AbsX);
		}
	}

	// Deal with constrain.
	// Constrain scale factors to be whole numbers.
	// This has to be done here instead of overriding the ConstrainDrag function.
	if (ClickMods.Constrain)
	{
		// Constrained squash forces scale to whole number multiple of original size...
		FIXED16 Sign;

		Sign = (XScaleFactor<0 ? -1 : 1);			// Compute sign of pure scale
		XScaleFactor+=FIXED16((double)0.5);			// Round scale to nearest integer
		XScaleFactor = XScaleFactor.MakeShort();
		if (XScaleFactor==0) XScaleFactor = Sign;	// Prevent scale from being zero by setting
													// it to +/- 1 depending on sign of pure scale.
		Sign = (YScaleFactor<0 ? -1 : 1);
		YScaleFactor+=FIXED16((double)0.5);
		YScaleFactor = YScaleFactor.MakeShort();
		if (YScaleFactor==0) YScaleFactor = Sign;
	}

	// Update information for the user...
	// Must update x,y,w,h,scale and possible angle.
	BoundingData.XScale = XScaleFactor;
	BoundingData.YScale = YScaleFactor;
	BoundingData.ScaleChanged = TRUE;
	// See also UpdateTransformBoundingData function in this file...
}



/********************************************************************************************

>	void OpScaleTrans::BuildMatrix()

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	19/7/94
	Purpose:	Builds the transform matrix required to rotate the selection about the
				point CentreOfRot by the required number of degrees

********************************************************************************************/

void OpScaleTrans::BuildMatrix()
{
	// Build a matrix to scale by the correct amount and one to translate back from the origin
	Matrix ScaleBy(XScaleFactor, YScaleFactor);
	Matrix TransFromOrigin(CentreOfTrans.x, CentreOfTrans.y);

	// First translate the centre of scaling to the origin, then scale, then translate back
	Transform = Matrix(-CentreOfTrans.x, -CentreOfTrans.y);
	Transform *= ScaleBy;
	Transform *= TransFromOrigin;
}





/********************************************************************************************

>	virtual void OpScaleTrans::ConstrainDrag(DocCoord* PointerPos)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	19/7/94
	Inputs:		PointerPos - The current position of the mouse that needs to be constrained
	Outputs:	PointerPos - The position of the mouse after it has been constrained
	Purpose:	Will constrain the mouse position to lie along rays from the centre of
				rotation at the constrain angle apart (45 degrees by default)

********************************************************************************************/

void OpScaleTrans::ConstrainDrag(DocCoord* PointerPos)
{
	// Constrain the mouse movement to hoizontal, vertical or diagonal
	DocView::ConstrainToAngle(CentreOfTrans, PointerPos);
}


/********************************************************************************************

>	BOOL OpScaleTrans::Declare()

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	5/7/93
	Returns:	TRUE if all went OK, False otherwise
	Purpose:	Adds the operation to the list of all known operations

********************************************************************************************/

BOOL OpScaleTrans::Declare()
{
	return (RegisterOpDescriptor(0, _R(IDS_SCALETRANS), CC_RUNTIME_CLASS(OpScaleTrans),
								OPTOKEN_SCALE, TransOperation::GetState)); 
}



/********************************************************************************************

>	void OpScaleTrans::UpdateTransformBoundingData()

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	04/9/94
	Purpose:	Override the base class version of this fn to ensure that new bounds
				are calculated and placed in the BoundingData structure.

********************************************************************************************/

void OpScaleTrans::UpdateTransformBoundingData()
{
#ifndef STANDALONE
	if (pSelTool != NULL)
	{

		ComputeNewBounds();
		// Tell the tool about the current transform bounding data
		pSelTool->DragMove(&BoundingData);
	}
#endif
}




/********************************************************************************************

>	FIXED16 OpScaleTrans::GetScalar()

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	09/01/2004
	Returns:	FIXED16 value of the equivalent scale factor for the transform.
	Purpose:	Returns the Transforms idea of the equivalent scale factor for the transform.
				This is mainly used to help with the scaling of line widths to save them
				trying to extract the scale factor from the matrix.

********************************************************************************************/

FIXED16 OpScaleTrans::GetScalar()
{
	// Just return the current value of the scale factor
	return YScaleFactor;
}
