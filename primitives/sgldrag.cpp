
// sgldrag.cpp - Line gallery item dragging

#include "camtypes.h"

#include "sgldrag.h"
#include "dragmgr.h"
#include "dragbmp.h"
#include "camview.h"
#include "grndbmp.h"
#include "nodepath.h"
#include "lineattr.h"
#include "sgline.h"
#include "osrndrgn.h"


// Implement the dynamic class bits...
CC_IMPLEMENT_DYNCREATE(GalleryLineDragInfo, BitmapDragInformation)
CC_IMPLEMENT_DYNAMIC(SGLineDragTarget, SGListDragTarget);

// This line mustn't go before any CC_IMPLEMENT_... macros
#define new CAM_DEBUG_NEW

// This determines where, if at all, the text description of a dragged line gallery item
// will be displayed.
const LineAttrItem::TextPosition c_eLineAttrDragTextPos = LineAttrItem::NO_LABEL;


/********************************************************************************************

>	SGLineDragTarget::SGLineDragTarget(DialogOp *TheDialog, CGadgetID TheGadget = NULL)
	 
	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95
	Inputs:		TheDialog - The kernel dialog in which the target exists
				TheGadget - The gadget within that dialogue which is the target

	Purpose:	Constructor

********************************************************************************************/

SGLineDragTarget::SGLineDragTarget(DialogOp *TheDialog, CGadgetID TheGadget)
					: SGListDragTarget(TheDialog, TheGadget)
{
	ERROR3IF(!TheDialog->IsKindOf(CC_RUNTIME_CLASS(LineGallery)),
			"You can only use SGLineDragTargets with LineGallery dialogues!");
}



/********************************************************************************************

	BOOL SGLineDragTarget::ProcessEvent(DragEventType Event, DragInformation *pDragInfo,
											OilCoord *pMousePos, KeyPress* pKeyPress)

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95
	Inputs:		Event - Indicates what has happened
				pDragInfo - points to drag information describing this drag. This
				should be a BitmapDragInformation or derivation thereof
				pMousePos - points to information on the current mouse position, in OIL coords
				pKeyPress - NULL, or if for a keypress event, keypress information

	Returns:	TRUE to claim the event, FALSE to let it through to other targets

	Purpose:	Event Handler for SuperGallery listitem drag events. Overrides the
				base class handler to enable it to sort out the node being dragged
				for Line drags.

********************************************************************************************/

BOOL SGLineDragTarget::ProcessEvent(DragEventType Event, DragInformation *pDragInfo,
										OilCoord *pMousePos, KeyPress* pKeyPress)
{
	if (!pDragInfo->IsKindOf(CC_RUNTIME_CLASS(GalleryLineDragInfo)))
		return(FALSE);

	SGDisplayNode *DraggedNode = NULL;
	BOOL IsSimpleBitmapDrag = TRUE;

	if (IS_A(pDragInfo, GalleryLineDragInfo))
	{
		DraggedNode = ((GalleryLineDragInfo *)pDragInfo)->GetDraggedLineAttr();
	}

	if (DraggedNode != NULL)
	{
		switch(Event)
		{
			case DRAGEVENT_COMPLETED:
				HandleDragCompleted((SuperGallery *) TargetDialog,
									DraggedNode, pMousePos, IsSimpleBitmapDrag);
				return(TRUE);


			case DRAGEVENT_MOUSESTOPPED:
			case DRAGEVENT_MOUSEMOVED:
			case DRAGEVENT_MOUSEIDLE:
				// Call a subroutine to work out and set our current cursor shape
				return(DetermineCursorShape((SuperGallery *) TargetDialog,
											DraggedNode, pMousePos));
			default:
				break;
		}
	}

	// Otherwise, we aren't interested in the event, so we don't claim it
	return(FALSE);
}

/********************************************************************************************

>	void GalleryLineDragInfo::GalleryLineDragInfo() 
	 
	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95		  

	Purpose:	Default constructor - do not call this constructor

********************************************************************************************/

GalleryLineDragInfo::GalleryLineDragInfo()
{
	ERROR3("Default GalleryLineDragInfo constructor called");	
}



/********************************************************************************************

>	GalleryLineDragInfo::GalleryLineDragInfo(LineAttrItem *pSourceItem,
											SGMouseInfo *pMouseInfo, SGMiscInfo *pMiscInfo,
 											BOOL IsAdjust = FALSE)
	 
	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95		  

	Inputs:		pSourceItem - The gallery item from which the drag originated
				pMouseInfo	- The mouse info which made the item start the drag
				pMiscInfo	- The MiscInfo which accompanied the mouse event
				IsAdjust	- TRUE if this is an adjust (line-Bitmap) drag

	Purpose:	Constructor

********************************************************************************************/

GalleryLineDragInfo::GalleryLineDragInfo(LineAttrItem *pSourceItem,
											SGMouseInfo *pMouseInfo, SGMiscInfo *pMiscInfo,
 											BOOL IsAdjust)
					  : BitmapDragInformation(NULL, 100,50,0,0,
					  							IsAdjust)
{
	SourceItem	= pSourceItem;	// Copy the source item pointer

	pAttr 		= SourceItem->CreateNewAttribute(IsAdjust);

	MouseInfo	= *pMouseInfo;	// Duplicate the structures (they may cease to exist
	MiscInfo	= *pMiscInfo;	// soon after the drag is started)
}

/********************************************************************************************

>	GalleryLineDragInfo::~GalleryLineDragInfo()
 
	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95		  

	Purpose:	Destructor. Deletes the dragged bitmap.

********************************************************************************************/

GalleryLineDragInfo::~GalleryLineDragInfo()
{
	if (TheBitmap != NULL)
	{
		delete TheBitmap;
		TheBitmap = NULL;
	}

	if (pAttr != NULL)
		delete pAttr;
}

/********************************************************************************************

>	void GalleryLineDragInfo::OnClick(INT32 Flags,POINT Point) 
	 
	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95		  
	Inputs:		-
	Outputs:	-
	Returns:	-
	Purpose:	This is called if a drag was attempted but never started because it was a 
				click all along. It calls back the SourceItem SGDisplayLine, to get it
				to handle the click.
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

void GalleryLineDragInfo::OnClick(INT32 Flags ,POINT Point)
{
	if (SourceItem != NULL)
		SourceItem->DragWasReallyAClick(&MouseInfo, &MiscInfo);
}

/********************************************************************************************

>	void GalleryLineDragInfo::GetCursorID(DragTarget* pDragTarget)

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95
	Purpose:	Set cursor over this target


********************************************************************************************/

UINT32 GalleryLineDragInfo::GetCursorID(DragTarget* pDragTarget)
{
	if (pDragTarget && pDragTarget->IS_KIND_OF(ViewDragTarget))
	{
		PageDropInfo PageDropInfo;
		((ViewDragTarget*)pDragTarget)->GetDropInfo(&PageDropInfo);

		NodeRenderableInk* pObjectHit 	= PageDropInfo.pObjectHit;
		ObjectDragTarget TargetHit 		= PageDropInfo.TargetHit;

		if (IS_A(pAttr, AttrStartArrow) || IS_A(pAttr, AttrEndArrow))
		{
			if (pObjectHit && pObjectHit->IS_KIND_OF(NodePath))
			{
				Path* pPath = &((NodePath*)pObjectHit)->InkPath;
				BOOL IsStart;
				if (DropStartOrEndArrow(pPath, PageDropInfo.DropPos, &IsStart))
				{
					TargetHit = IsStart ? STARTCOL_TARGET : ENDCOL_TARGET;
				}
			}
		}

		ClickModifiers ClickMods = ClickModifiers::GetClickModifiers();
		BOOL IsInside = ClickMods.Constrain;

		if (!IsInside && pObjectHit && pObjectHit->IsCompound())
		{
			TargetHit = MANY_TARGET;
		}

		switch (TargetHit)
		{
			case FILL_TARGET:
				return IsInside ? _R(IDC_DROPINSIDEONLINE) : _R(IDC_CANDROPONLINE);
			case LINE_TARGET:
				return IsInside ? _R(IDC_DROPINSIDEONLINE) : _R(IDC_CANDROPONLINE);
			case STARTCOL_TARGET:
				return IsInside ? _R(IDC_DROPINSIDEONFILLSTART) : _R(IDC_CANDROPONFILLSTART);
			case ENDCOL_TARGET:
				return IsInside ? _R(IDC_DROPINSIDEONFILLEND) : _R(IDC_CANDROPONFILLEND);
			case MANY_TARGET:
				return IsInside ? _R(IDC_DROPINSIDEONLINE) : _R(IDC_CANDROPONLINE);

			case NO_TARGET:
				return _R(IDC_CANDROPONPAGE);
			default:
				break;
		};

		return _R(IDC_CANDROPONPAGE);
	}

	return _R(IDC_CANTDROP);
}

/********************************************************************************************

>	virtual BOOL GalleryLineDragInfo::GetStatusLineText(String_256 * TheText, DragTarget* pDragTarget)

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95
	Returns:	Whether String is valid
	Purpose:	provide status line text for this target
   
********************************************************************************************/

BOOL GalleryLineDragInfo::GetStatusLineText(String_256 * TheText, DragTarget* pDragTarget)
{
	ERROR2IF(TheText==NULL,FALSE,"NULL string in GetStatusLineText()");

	if (TheBitmap == NULL || TheBitmap->ActualBitmap == NULL)
		return FALSE;

 	String_256 DragString(_R(IDS_SGLDRAG_DRAG_LINE_ATTR)); // "Dragging line attribute"
	DragString += String_8(_R(IDS_SGDFONTS_STAT_COLON_SEP)); //" : ";

	if (pDragTarget && pDragTarget->IS_KIND_OF(ViewDragTarget))
	{
		PageDropInfo PageDropInfo;
		((ViewDragTarget*)pDragTarget)->GetDropInfo(&PageDropInfo);

		NodeRenderableInk* pObjectHit 	= PageDropInfo.pObjectHit;
		ObjectDragTarget TargetHit 		= PageDropInfo.TargetHit;

		String_256 ObjectDesc = _R(IDS_SGLDRAG_THIS_OBJECT); // " this object";

		if (pObjectHit)
		{
			ObjectDesc = pObjectHit->Describe(FALSE);
		}

		ClickModifiers ClickMods = ClickModifiers::GetClickModifiers();
		BOOL IsInside = ClickMods.Constrain;

		if (!IsInside && pObjectHit && pObjectHit->IsCompound())
		{
			TargetHit = MANY_TARGET;
		}

		switch (TargetHit)
		{
			case FILL_TARGET:
			case LINE_TARGET:
			case STARTCOL_TARGET:
			case ENDCOL_TARGET:
				DragString += String_64(_R(IDS_SGLDRAG_DROP_TO_APPLY)); // "Drop to apply this attribute to this ";
				DragString += ObjectDesc;
				if (IsInside)
					DragString += String_64(_R(IDS_SGLDRAG_SPACE_ALONE)); // " alone";
				break;

			case MANY_TARGET:
				DragString += String_64(_R(IDS_SGLDRAG_DROP_TO_APPLY)); // "Drop to apply this attribute to this ";
				DragString += ObjectDesc;
				DragString += String_64(_R(IDS_SGLDRAG_CONTROL_DROP_INSIDE)); // "; Press 'Control' to drop 'Inside'";
				break;

			case NO_TARGET:
				DragString += String_64(_R(IDS_SGLDRAG_DROP_CURRENT_ATTRIBUTE)); // "Drop to set the Current Attribute";
				break;
			default:
				break;
		};

	 	*TheText = DragString;
		return TRUE;
	}
 	
	return FALSE;
}

/********************************************************************************************

>	BOOL GalleryBitmapDragInfo::OnPageDrop(ViewDragTarget* pDragTarget)
 
	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95		  
	Inputs:		-
	Outputs:	-
	Returns:	-
	Purpose:	Called when a Line Attr is dropped onto the page.
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

BOOL GalleryLineDragInfo::OnPageDrop(ViewDragTarget* pDragTarget)
{
	PageDropInfo PageDropInfo;
	((ViewDragTarget*)pDragTarget)->GetDropInfo(&PageDropInfo);
	NodeRenderableInk* pObjectHit = PageDropInfo.pObjectHit;

	NodeAttribute* Attrib = SourceItem->CreateNewAttribute(IsAnAdjustDrag());

	if (Attrib == NULL)
		return FALSE;

	if (pObjectHit && (IS_A(Attrib, AttrStartArrow) || IS_A(Attrib, AttrEndArrow)))
	{
		if (!pObjectHit->IS_KIND_OF(NodePath))
		{
			delete Attrib;
			return FALSE;
		}

		Path* pPath = &((NodePath*)pObjectHit)->InkPath;
		BOOL IsStart;

		if (!DropStartOrEndArrow(pPath, PageDropInfo.DropPos, &IsStart))
		{
			delete Attrib;
			return FALSE;
		}

		Attrib = MakeStartOrEndArrow(Attrib, IsStart);
	}

	if (pObjectHit) 
	{
		// Hit a Line Object, so apply attribute to it
		AttributeManager::ApplyAttribToNode(pObjectHit, Attrib);
	}
	else
	{
		// Didn't hit anything, so just set the current attribute
		AttributeManager::AttributeSelected(Attrib);
	}

	return TRUE;
}


/********************************************************************************************

>	INT32 GalleryLineDragInfo::GetDragTransparency()
 
	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95		  
	Returns:	-
	Purpose:	Specifies how transparent a drag should be.
				A value of 0, will cause a normal solid blit.
				Any other value (between 0 and 100), will cause a masked blit.
	SeeAlso:	-

********************************************************************************************/

INT32 GalleryLineDragInfo::GetDragTransparency()
{
//	if (IS_A(pAttr, AttrStartArrow) || IS_A(pAttr, AttrEndArrow))
//		return 50;

	return 50;
}

/********************************************************************************************

>	KernelBitmap* GalleryLineDragInfo::GetSolidDragMask()

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95		  
	Returns:	-
	Purpose:	Makes a 1bpp KernelBitmap mask for the solid drag.
	SeeAlso:	-

********************************************************************************************/

KernelBitmap* GalleryLineDragInfo::GetSolidDragMask()
{
	// Note we abuse this call (like our base class abuses this call) to create the bitmap
	// itself. We don't use DragMask itself anymore (i.e. it stays NULL)
	if (!DragMask && !TheBitmap)
	{
		DocView *View = DocView::GetCurrent();
		if (View == NULL)
		{
			return NULL;
		}
		
		Spread *pSpread = View->FindEnclosingSpread(OilCoord(0,0));
		if (pSpread == NULL)
		{
			return NULL;
		}

		// Find the size of the rendered item.
		DocRect ClipRegion(0,0, 750*100, 750*50);
//		ClipRegion.lo.x = ClipRegion.lo.y = 0;
//		SourceItem->GetSize(c_eLineAttrDragTextPos, &ClipRegion.hi.x, &ClipRegion.hi.y);
		Matrix ConvertMatrix;
		FIXED16 ViewScale = 1;

		wxScreenDC DisplayDC;
		double dpi = (double) OSRenderRegion::GetFixedDCPPI(DisplayDC).GetWidth();

		GRenderBitmap* pMaskRegion 	= new GRenderBitmap(ClipRegion, ConvertMatrix, ViewScale, 
														32, dpi);

		pMaskRegion->SetDoCompression(TRUE); // misnamed call to indicate we want transparency
		pMaskRegion->AttachDevice(View, &DisplayDC, pSpread);

		// Make a Mask Bitmap
		pMaskRegion->StartRender();
	  	SourceItem->Render(pMaskRegion, ClipRegion, c_eLineAttrDragTextPos);
		pMaskRegion->StopRender();

		OILBitmap* pOilMaskBmp = pMaskRegion->ExtractBitmap();
		TheBitmap = new KernelBitmap(pOilMaskBmp, TRUE);	

		delete pMaskRegion;
	}

	return BitmapDragInformation::GetSolidDragMask();
}

/********************************************************************************************

>	BOOL GalleryLineDragInfo::DropStartOrEndArrow(Path* pPathHit, DocCoord DropPos,
											  BOOL* IsStart)

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95		  
	Returns:	-
	Purpose:	Checks to see whether the arrow was dropped nearer to the start or end of
				a path.
	SeeAlso:	-

********************************************************************************************/

BOOL GalleryLineDragInfo::DropStartOrEndArrow(Path* pPathHit, DocCoord DropPos,
											  BOOL* IsStart)
{
	double NearestDist = -1;
	BOOL GotNearest = FALSE;
	BOOL NearestIsStart = FALSE;

	DocCoord* Coords = pPathHit->GetCoordArray();
	PathVerb* Verbs  = pPathHit->GetVerbArray();
	INT32 NumCoords   = pPathHit->GetNumCoords();

	DocCoord ArrowCentre;
	DocCoord ArrowDirection;

	INT32 PathIndex = 0;

	// Find the first position of an ArrowHead
	BOOL GotPos = ArrowRec::GetFirstArrowPos(TRUE,
											 Coords, Verbs, NumCoords, 
								   			 &PathIndex, &ArrowCentre, &ArrowDirection);
	while (GotPos)
	{
		double Dist = ArrowCentre.Distance(DropPos);

		if (NearestDist == -1 || Dist < NearestDist)
		{
			NearestDist = Dist;
			GotNearest = TRUE;
			NearestIsStart = TRUE;
		}

		// Find the next Arrow position (if there are any more subpaths)
		GotPos = ArrowRec::GetNextArrowPos(TRUE,
								 		   Coords, Verbs, NumCoords, 
							     		   &PathIndex, &ArrowCentre, &ArrowDirection);
	}

	// Now test all the end positions
	PathIndex = 0;

	// Find the first position of an ArrowHead
	GotPos = ArrowRec::GetFirstArrowPos(FALSE,
										 Coords, Verbs, NumCoords, 
							   			 &PathIndex, &ArrowCentre, &ArrowDirection);
	while (GotPos)
	{
		double Dist = ArrowCentre.Distance(DropPos);

		if (NearestDist == -1 || Dist < NearestDist)
		{
			NearestDist = Dist;
			GotNearest = TRUE;
			NearestIsStart = FALSE;
		}

		// Find the next Arrow position (if there are any more subpaths)
		GotPos = ArrowRec::GetNextArrowPos(FALSE,
								 		   Coords, Verbs, NumCoords, 
							     		   &PathIndex, &ArrowCentre, &ArrowDirection);
	}

	if (GotNearest)
		*IsStart = NearestIsStart;

	return GotNearest;
}

/********************************************************************************************

>	NodeAttribute* GalleryLineDragInfo::MakeStartOrEndArrow(NodeAttribute* pArrowAttr, BOOL Start)

	Author:		Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/4/95		  
	Returns:	-
	Purpose:	Makes a Start or End arrow depending on where the arrow was dropped.
	SeeAlso:	-

********************************************************************************************/

NodeAttribute* GalleryLineDragInfo::MakeStartOrEndArrow(NodeAttribute* pArrowAttr, BOOL Start)
{
	ArrowRec Arrow;
	NodeAttribute* NewAttr;

	if (IS_A(pArrowAttr, AttrStartArrow))
	{
		Arrow = ((AttrStartArrow*)pArrowAttr)->Value.StartArrow;
	}
	else
	{
		Arrow = ((AttrEndArrow*)pArrowAttr)->Value.EndArrow;
	}

	if (Start)
	{
		NewAttr = new AttrStartArrow();	
		((AttrStartArrow*)NewAttr)->Value.StartArrow = Arrow;
	}
	else
	{
		NewAttr = new AttrEndArrow();	
		((AttrEndArrow*)NewAttr)->Value.EndArrow = Arrow;
	}

	delete pArrowAttr;
	return NewAttr;
}
