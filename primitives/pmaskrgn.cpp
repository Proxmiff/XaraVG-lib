
// Printing Masked Render Region


#include "camtypes.h"
#include "pmaskrgn.h"
#include "osrndrgn.h"
#include "nodebmp.h"

// An implement to match the Declare in the .h file.
CC_IMPLEMENT_DYNAMIC(PrintingMaskedRenderRegion, MaskedRenderRegion)

// This will get Camelot to display the filename and linenumber of any memory allocations
// that are not released at program exit
#define new CAM_DEBUG_NEW


/********************************************************************************************

>	PrintingMaskedRenderRegion::PrintingMaskedRenderRegion(DocRect Rect, Matrix Mat,
															FIXED16 Scale, double dpi)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/95
	Inputs:		Rect - The Clip Rect of the region
				Mat - the rendering matrix to use
				Scale - the View Scale
				dpi - the dpi to render to
	Purpose:	Constructs a PrintingMaskedRenderReigon
				INT32 dpi changed to double dpi (12/12/95) to improve the range of values allowed
				at the < 1000dpi settings that we will be using.

********************************************************************************************/

PrintingMaskedRenderRegion::PrintingMaskedRenderRegion(DocRect Rect, Matrix Mat, FIXED16 Scale, double dpi, BOOL Printing) :
MaskedRenderRegion(Rect, Mat, Scale, dpi)
{
	// Default to a non complex run
	IsComplexRun = FALSE;
	RenderFlags.Printing = Printing;
}


/********************************************************************************************

>	PrintingMaskedRenderRegion::~PrintingMaskedRenderRegion()

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/95
	Purpose:	destructor - the base classes do all the work here

********************************************************************************************/

PrintingMaskedRenderRegion::~PrintingMaskedRenderRegion()
{
}


/********************************************************************************************

>	virtual WinRect PrintingMaskedRenderRegion::CalculateWinRect( const Matrix& RenderMatrix, const DocRect& docrect,
																  const double dpi)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	4/10/96
	Inputs:		DocRect is a rectangle on document co-ords.
				dpi is the resolution of the device we are rendering to
	Outputs:	RenderMatrix is the rendering matrix, may have e anf f components changed by the call
	Returns:	Object containing the new rectangle coordinates.     			
	Purpose:	To convert a rectangle in Doc coordinates to a rectangle in Win coordinates
				taking account of the destination dpi rather than assuming screen dpi.
				Virtual so that it can be overriden by different render regions if so required.
	SeeAlso:	OSRenderRegion::DocRectToWin;

********************************************************************************************/

WinRect PrintingMaskedRenderRegion::CalculateWinRect( Matrix& RenderMatrix, const DocRect& docrect,
													  const double dpi)
{
	// lets get the Rect into screen co-ords so we can use our big screen bitmap
	// Use a version of OSRenderRegion::DocRectToWin that uses the actual dpi we have stored
	// in PixelsPerInch
	// This goes back to the GRenderRegion form rather than the GRenderBitmap special form

	return OSRenderRegion::BitmapDocRectToWin( RenderMatrix, docrect, dpi );
//	return OSRenderRegion::DocRectToWin( RenderMatrix, docrect, dpi );
}


/********************************************************************************************

>	virtual GMATRIX PrintingMaskedRenderRegion::MakeGavinMatrix(Matrix NewRenderMatrix, DocRect ClipRect, double dPixelsPerInch, BOOL bMasterCapture)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com> (from Will (from Phil code))
	Created:	??/??/2004
	Inputs:		The Kernel Matrix and cliprect to use to create the Gavin Matrix.
	Returns: 	TRUE if the GMatrix was created ok.
	Purpose:	Make a Gavin Matrix out of a Kernel Matrix
	SeeAlso:	GRenderRegion::SetGavinMatrix
	Notes:		Doesn't rely on current state of CurrentClipRect like the above version

********************************************************************************************/

GMATRIX PrintingMaskedRenderRegion::MakeGavinMatrix(Matrix NewRenderMatrix, DocRect ClipRect, double dPixelsPerInch, BOOL bMasterCapture)
{
	// Bypass the behaviour inherited from GRenderBitmap because we don't want it
	return GRenderRegion::MakeGavinMatrix(NewRenderMatrix, ClipRect, dPixelsPerInch, bMasterCapture);
}


/********************************************************************************************

>	void PrintingMaskedRenderRegion::SetMaskDrawingMode(BOOL OnOff)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/95
	Inputs:		OnOff - TRUE means the nodes we are about render will all be treated as
				complex nodes. FALSE means that all the nodes we are about to render are
				treated as simple nodes.
	Purpose:	The render region can either be rendering simple nodes or complex nodes in
				runs (ie you get a block or complex one, then a block of simple ones). This
				function allows you to change the render regions idea of whether the nodes
				are complex or simple.

********************************************************************************************/

void PrintingMaskedRenderRegion::SetMaskDrawingMode(BOOL OnOff)
{
	// Set our bool, making sure that it is always either TRUE or FALSE
	IsComplexRun = (OnOff) ? TRUE : FALSE;
}


/********************************************************************************************

>	virtual void PrintingMaskedRenderRegion::DrawPathToOutputDevice(Path *PathToRender)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/95
	Inputs:		PathToRender - the path to render
	Purpose:	Renders a path black if we are in a complex run, or white if not

********************************************************************************************/

void PrintingMaskedRenderRegion::DrawPathToOutputDevice(Path *PathToRender, PathShape)
{
	// Save the context
	SaveContext();

	// Does this shape make use of complex things
	if (IsComplexRun)
		SetNextShapeColour(Black);
	else
		SetNextShapeColour(White);

	// Draw the path
	GRenderBitmap::DrawPathToOutputDevice(PathToRender);

/*	BOOL RenderFill = FALSE;

	// Get the current fill attr
	FillGeometryAttribute* pFillAttr = (FillGeometryAttribute*) CurrentAttrs[ATTR_FILLGEOMETRY].pAttr;

	if (pFillAttr->GetBitmap() != NULL && pFillAttr->GetBitmap()->GetBPP() <= 8)
	{
	 	INT32 TranspIndex;

		// If it's a masked bitmap fill, then we'll need to actually render it
		// (but we'll force all the non-masked pixels to be rendered black)
		if (pFillAttr->GetBitmap()->GetTransparencyIndex(&TranspIndex))
			RenderFill = TRUE;
	}

	if (RenderFill)
	{
		BOOL OldVMState = RenderFlags.VeryMono;
		RenderFlags.VeryMono = TRUE;	// Forces non-masked pixels to be plotted black

		// Draw the path, with the bitmap fill
		GRenderBitmap::DrawPathToOutputDevice(PathToRender);

		RenderFlags.VeryMono = OldVMState;
	}
	else
	{
		// Does this shape make use of complex things
		if (IsComplexRun)
			SetNextShapeColour(Black);
		else
			SetNextShapeColour(White);

		// Draw the path
		GRenderBitmap::DrawPathToOutputDevice(PathToRender);
	}
*/	
	
	// restore the context
	RestoreContext();
}


/********************************************************************************************

>	virtual void PrintingMaskedRenderRegion::DrawRect(DocRect *RectToRender)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/95
	Inputs:		The rect to render
	Purpose:	Renders a path black if we are in a complex run, or white if not

********************************************************************************************/

void PrintingMaskedRenderRegion::DrawRect(DocRect *RectToRender)
{
	// Save the context
	SaveContext();

	// Does this shape make use of complex things
	if (IsComplexRun)
		SetNextShapeColour(Black);
	else
		SetNextShapeColour(White);

	// Draw the Rect
	GRenderBitmap::DrawRect(RectToRender);

	// restore the context
	RestoreContext();
}


/********************************************************************************************

>	virtual void PrintingMaskedRenderRegion::DrawLine(const DocCoord &StartPoint,
													  const DocCoord &EndPoint)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/95
	Inputs:		StartPoint, EndPoint - the ends of the line
	Purpose:	Renders a path black if we are in a complex run, or white if not

********************************************************************************************/

void PrintingMaskedRenderRegion::DrawLine(const DocCoord &StartPoint, const DocCoord &EndPoint)
{
	// Save the context
	SaveContext();

	// Does this shape make use of complex things
	if (IsComplexRun)
		SetNextShapeColour(Black);
	else
		SetNextShapeColour(White);

	// Draw the Line
	GRenderBitmap::DrawLine(StartPoint, EndPoint);

	// restore the context
	RestoreContext();
}


/********************************************************************************************

>	virtual void PrintingMaskedRenderRegion::DrawPixel(const DocCoord &Point)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/95
	Inputs:		Point - the point to draw the pixel at
	Purpose:	Renders a path black if we are in a complex run, or white if not

********************************************************************************************/

void PrintingMaskedRenderRegion::DrawPixel(const DocCoord &Point)
{
	// Save the context
	SaveContext();

	// Does this shape make use of complex things
	if (IsComplexRun)
		SetNextShapeColour(Black);
	else
		SetNextShapeColour(White);

	// Draw the Pixel
	GRenderBitmap::DrawPixel(Point);

	// restore the context
	RestoreContext();
}


/********************************************************************************************

>	virtual void PrintingMaskedRenderRegion::DrawBitmap(const DocCoord &Point,
														KernelBitmap* pBitmap)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/95
	Inputs:		Point - The point to draw the bitmap at
				pBitmap - at the bitmap
	Purpose:	Renders a path black if we are in a complex run, or white if not

********************************************************************************************/

void PrintingMaskedRenderRegion::DrawBitmap(const DocCoord &Point, KernelBitmap* pBitmap)
{
	// Save the context
	SaveContext();

	// Does this shape make use of complex things
	if (IsComplexRun)
		SetNextShapeColour(Black);
	else
		SetNextShapeColour(White);

	// Draw the bitmap
	GRenderBitmap::DrawBitmap(Point, pBitmap);

	// restore the context
	RestoreContext();
}


/********************************************************************************************

>	virtual void PrintingMaskedRenderRegion::DrawBitmap(const DocCoord &Point, UINT32 BitmapID, UINT32 ToolID = NULL)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/95
	Inputs:		Point - the point to draw the bitmap at
				BitmapID - The ID of the bitmap to draw
				ToolID - the ID of the tool the resource is part of
	Purpose:	Renders a path black if we are in a complex run, or white if not

********************************************************************************************/

void PrintingMaskedRenderRegion::DrawBitmap(const DocCoord &Point, UINT32 BitmapID, UINT32 ToolID)
{
	// Save the context
	SaveContext();

	// Does this shape make use of complex things
	if (IsComplexRun)
		SetNextShapeColour(Black);
	else
		SetNextShapeColour(White);

	// Draw the Bitmap
	GRenderBitmap::DrawBitmap(Point, BitmapID, ToolID);

	// restore the context
	RestoreContext();
}


/********************************************************************************************

>	virtual BOOL PrintingMaskedRenderRegion::DrawTransformedBitmap(NodeBitmap *pNodeBitmap)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	24/7/95
	Inputs:		pNodeBitmap - the Node bitmap to render
	Purpose:	Renders a path black if we are in a complex run, or white if not

********************************************************************************************/

BOOL PrintingMaskedRenderRegion::DrawTransformedBitmap(NodeBitmap *pNodeBitmap)
{
	// Save the context
	SaveContext();

	// Does this shape make use of complex things
	if (IsComplexRun)
		SetNextShapeColour(Black);
	else
		SetNextShapeColour(White);

	// Draw the Bitmap
	BOOL Result = GRenderBitmap::DrawTransformedBitmap(pNodeBitmap);

/*	BOOL Result = TRUE;
	
	// Work out if we can do this bitmap directly
	BOOL CanDoBitmap = FALSE;

	// Does this shape make use of complex things
	if (!(IsComplexRun || pNodeBitmap->NeedsTransparency()))
	{
		// See what kind of bitmaps this render region can do
		if (HostCaps.ArbitraryBitmaps)
		{
			// We can do anything...
			CanDoBitmap = TRUE;
		}
		else
		{
			if (HostCaps.SimpleBitmaps)
			{
				// Check that the bitmap area is rectangular, and is perpendicular to
				// the x and y axes.
				if (pNodeBitmap->HasSimpleOrientation(this))
				{
					// Yes - simple scaling - the render region can do this.
					CanDoBitmap = TRUE;
				}
			}
		}
	}

	// Here we set up reasonable attributes - we want to fill the bitmap
	// path but not stroke it.  NB. This is very important - do not remove
	// otherwise the mask will be wrong!
	SetFillColour(DocColour(COLOUR_BLACK));
	SetLineColour(DocColour(COLOUR_NONE));

	if (CanDoBitmap)
	{
		SetNextShapeColour(White);

		// Draw the path (that represents the bitmap object)
		GRenderBitmap::DrawPathToOutputDevice(&pNodeBitmap->InkPath);
	}
	else
	{
		if (pNodeBitmap->NeedsTransparency())
		{
			// It's a masked bitmap, so we'll need to actually render the non-masked
			// pixels as black 
			BOOL OldVMState = RenderFlags.VeryMono;
			RenderFlags.VeryMono = TRUE;	// Forces non-masked pixels to be plotted black

			// Call base class to plot the bitmap
			RenderRegion::DrawTransformedBitmap(pNodeBitmap);

			RenderFlags.VeryMono = OldVMState;
		}
		else
		{
			SetNextShapeColour(Black);

			// Draw the path (that represents the bitmap object)
			GRenderBitmap::DrawPathToOutputDevice(&pNodeBitmap->InkPath);
		}
	}
*/

	// restore the context
	RestoreContext();

	// return if it worked or not
	return Result;
}




/********************************************************************************************

>	virtual BOOL PrintingMaskedRenderRegion::RenderChar(WCHAR ch, Matrix* pMatrix)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	2/8/95
	Inputs:		ch - the char to render
	Outputs:	pMatrix - the matrix to use to render with
	Returns:	TRUE if it worked, FALSE if not.
	Purpose:	Renders a char into the mask sometimes - there are times when we do not want
				the char to render - eps when we are dealing with postscript, where what we
				render and what postscript renders may be different.

********************************************************************************************/

BOOL PrintingMaskedRenderRegion::RenderChar(WCHAR ch, Matrix* pMatrix)
{
	// Save the context
	SaveContext();

	// Get ready to return
	BOOL Result = TRUE;
	
	// Does this shape make use of complex things
	// if it is not complex them do not try and punch it out of the mask, as this
	// causes problems on postscript devices.
	if (IsComplexRun)
	{
		SetNextShapeColour(Black);
		Result = GRenderBitmap::RenderChar(ch, pMatrix);
	}

	// restore the context
	RestoreContext();

	// return
	return Result;
}

