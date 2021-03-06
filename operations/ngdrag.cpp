
/*
	$Header: /Camelot/kernel/ngdrag.cpp 2     27/06/00 14:54 Justinf $
	Attribute gallery mouse dragging operations
*/

#include "camtypes.h"

#include "ngcore.h"
#include "ngdrag.h"
#include "ngitem.h"
#include "ngsetop.h"

#include "sginit.h"
#include "dragmgr.h"

#include "scrvw.h"

#include "ccdc.h"
#include "grnddib.h"
#include "grndbmp.h"
#include "wbitmap.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

DECLARE_SOURCE("$Revision$");

// Implement the dynamic class bits...
CC_IMPLEMENT_DYNAMIC(SGNameDrag, BitmapDragInformation);
CC_IMPLEMENT_DYNAMIC(SGNameDragTarget, SGListDragTarget);

// This line mustn't go before any CC_IMPLEMENT_... macros
#define new CAM_DEBUG_NEW


/********************************************************************************************
>	SGNameDrag::SGNameDrag()

	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99		  
	Purpose:	Default constructor, required for dynamic creation.
********************************************************************************************/

SGNameDrag::SGNameDrag()
  :	m_pSourceItem(0)
{
	// Empty.
}



/********************************************************************************************
>	SGNameDrag::SGNameDrag(SGNameItem* pSourceItem,
											 SGMouseInfo* pMouseInfo,
											 SGMiscInfo* pMiscInfo)
	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99		  
	Inputs:		pSourceItem	---		the gallery item from which the drag originated
				pMouseInfo		---		the mouse info which made the item start the drag
				pMiscInfo		---		the MiscInfo which accompanied the mouse event
	Purpose:	Constructs a SGNameDrag object.
********************************************************************************************/

SGNameDrag::SGNameDrag(SGNameItem* pSourceItem, SGMouseInfo* pMouseInfo,
										 SGMiscInfo* pMiscInfo)
  :	BitmapDragInformation(
			0,
			SG_DefaultNameText,
			SG_DefaultSmallIcon * NameGallery::Instance()->GetSelectedItemCount(),
			0, 0, pMouseInfo->MenuClick),
	m_pSourceItem(pSourceItem),
	m_MouseInfo(*pMouseInfo),
	m_MiscInfo(*pMiscInfo)
{
	// Empty.
}



/********************************************************************************************
>	virtual SGNameDrag::~SGNameDrag()

	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99		  
	Purpose:	Destroys a SGNameDrag object.
********************************************************************************************/

SGNameDrag::~SGNameDrag()
{
	// Tidy up (why doesn't the base class do this?)
	delete TheBitmap;
	TheBitmap = 0;
}



/********************************************************************************************
>	virtual void SGNameDrag::OnClick(INT32 nFlags, POINT ptClick) 
	 
	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99		  
	Inputs:		nFlags		---		which keys are depressed during the drag
				ptClick		---		the position of the mosue click
	Purpose:	This is called if a drag was attempted but never started because it was a 
				click all along. It calls back the SourceItem SGDisplayLine, to get it
				to handle the click.
********************************************************************************************/

void SGNameDrag::OnClick(INT32 nFlags, POINT ptClick)
{
	if (m_pSourceItem != 0)
			m_pSourceItem->DragWasReallyAClick(&m_MouseInfo, &m_MiscInfo);
}



/********************************************************************************************
>	void SGNameDrag::GetCursorID(DragTarget* pDragTarget)

	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99
	Inputs:		pDragTarget		---		the drag target to set the cursor for
	Purpose:	Set cursor over this target.
********************************************************************************************/

UINT32 SGNameDrag::GetCursorID(DragTarget* pDragTarget)
{
	// Only allow drops on the document view.
	if (pDragTarget == 0 || !pDragTarget->IS_KIND_OF(ViewDragTarget))
		return _R(IDC_CANTDROP);

	// If there's an object below the cursor then show the "can drop" cursor, otherwise
	// show the "can't drop" cursor.
	PageDropInfo pdInfo;
	((ViewDragTarget*) pDragTarget)->GetDropInfo(&pdInfo);
	return (pdInfo.pObjectHit != 0) ? _R(IDC_CANDROPONPAGE) : _R(IDC_CANTDROP);
}



/********************************************************************************************
>	virtual BOOL SGNameDrag::GetStatusLineText(String_256* pText,
														DragTarget* pDragTarget)
	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99
	Inputs:		pText			---		where to output the status line text
				pDragTarget		---		the target of the drag (ie. the document view)
	Returns:	TRUE if the output string is valid.
	Purpose:	Works out status line text for this target of a Name gallery item drag.
********************************************************************************************/

BOOL SGNameDrag::GetStatusLineText(String_256* pText, DragTarget* pDragTarget)
{
	// Validate outputs and object state.
	ERROR3IF(pText == 0, "SGNameDrag::GetStatusLineText: no output parameter");
	if (TheBitmap == 0 || TheBitmap->ActualBitmap == 0) return FALSE;

	// Only provide status-line help for drops on the document view.
	if (pDragTarget == 0 || !pDragTarget->IS_KIND_OF(ViewDragTarget))
		return FALSE;

	// Get the name and type of the item being dragged.
	String_256 strName;
	m_pSourceItem->GetNameText(&strName);
	String strTypeDesc(((SGNameGroup*) m_pSourceItem->GetParent())->GetTypeID());

	// Describe the object the mouse is over, if any.
	PageDropInfo pdInfo;
	((ViewDragTarget*) pDragTarget)->GetDropInfo(&pdInfo);
	NodeRenderableInk* pObjectHit = pdInfo.pObjectHit;
	if (pObjectHit == 0)
		pText->MakeMsg(_R(IDST_NAMEOP_CANT_DROP), &strName, &strTypeDesc);
	else
	{
		String_64 strObjectDesc;
		strObjectDesc = pObjectHit->Describe(FALSE); 
		pText->MakeMsg(_R(IDST_NAMEOP_DROP_TO_APPLY),
							&strName, &strTypeDesc, &strObjectDesc, &strTypeDesc, &strName);
	}

	return TRUE;
}



/********************************************************************************************
>	virtual BOOL SGNameDrag::CanDropOnPage()

	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99
	Returns:	TRUE if the a name item can be dropped on a page (ie. a document view).
	Purpose:	Called by the kernel dragging code to test whether a dragged name item
				can be dropped on a view onto a document.
********************************************************************************************/

BOOL SGNameDrag::CanDropOnPage()
{
	return TRUE;
}



/********************************************************************************************
>	virtual BOOL SGNameDrag::OnPageDrop(ViewDragTarget* pDragTarget)
 
	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99		  
	Inputs:		pDragTarget		---		the drag target representing the view onto
										a document (the "page").
	Returns:	TRUE if successful.
	Purpose:	Called when a Name gallery item is dropped onto the page.
********************************************************************************************/

BOOL SGNameDrag::OnPageDrop(ViewDragTarget* pDragTarget)
{
	// Extract the address of the object dropped on and apply the dragged name
	// attribute to it.
	PageDropInfo pdInfo;
	pDragTarget->GetDropInfo(&pdInfo);
	if (pdInfo.pObjectHit != 0)
	{
		OpDescriptor* pDesc = OpDescriptor::FindOpDescriptor(OPTOKEN_APPLY_NAMES_TO_ONE);
		ERROR3IF(pDesc == 0, "SGNameDrag::OnPageDrop: no descriptor");
		pDesc->Invoke(&OpParam((INT32) (Node*) pdInfo.pObjectHit, 0));
	}

	return TRUE;
}



/********************************************************************************************
>	virtual INT32 SGNameDrag::GetDragTransparency()
 
	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99		  
	Returns:	A transparency value, in this case, 0%
	Purpose:	Specifies how transparent a drag should be.
********************************************************************************************/

INT32 SGNameDrag::GetDragTransparency()
{
//	return 0;
	return 100;
}



/********************************************************************************************
>	virtual KernelBitmap* SGNameDrag::GetSolidDragMask()

	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99		  
	Returns:	A pointer to a KernelBitmap to use as a 1bpp mask for the dragged item,
				or 0 for failure.
	Purpose:	Renders an image of the dragged item into monochrome bitmap to use as an
				XOR mask for rendering the dragged Name gallery item.
	SeeAlso:	SGNameDrag::MakeImage
********************************************************************************************/

KernelBitmap* SGNameDrag::GetSolidDragMask()
{
	// If the mask already exists then return it else else make it by rendering the
	// dragged item into a 1 bpp bitmap, and then return it.
	return (DragMask != 0) ? DragMask : DragMask = MakeImage(1);
}



/********************************************************************************************
>	virtual BOOL SGNameDrag::OnDrawSolidDrag(CPoint ptOrigin, CDC* pDC)

	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Inputs:		See BitmapDragInformation::OnDrawSolidDrag
	Returns:	TRUE if successful.
	Purpose:	Renders an image of the dragged item into monochrome bitmap to use as an
				XOR mask for rendering the dragged Name gallery item.
	SeeAlso:	SGNameDrag::MakeImage
********************************************************************************************/

BOOL SGNameDrag::OnDrawSolidDrag(CPoint ptOrigin, CDC* pDC)
{
	// If the dragged item's image doesn't exists then make it by rendering the item into
	// a bitmap with the same depth as the screen.
	if (TheBitmap == 0) TheBitmap = MakeImage();
	return BitmapDragInformation::OnDrawSolidDrag(ptOrigin, pDC);
}



/********************************************************************************************
>	virtual KernelBitmap* SGNameDrag::MakeImage(UINT32 nDepth = 0)

	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Inputs:		nDepth		---		depth of bitmap to render into (default is screen
									depth)
	Returns:	A pointer to the rendered image of an item in a KernelBitmap of the given
				depth, or 0 for failure.
	SeeAlso:	SGNameDrag::OnDrawSolidDrag; SGNameDrag::GetSolidDragMask
********************************************************************************************/

KernelBitmap* SGNameDrag::MakeImage(UINT32 nDepth)
{
	// If there's no current View, or no Spread, then fail.
	DocView* pView = DocView::GetCurrent();
	if (pView == 0) return 0;
	Spread* pSpread = pView->FindEnclosingSpread(OilCoord(0, 0));
	if (pSpread == 0) return 0;

	// Create a device context for the display.
	CDC sdc;
	sdc.CreateDC("DISPLAY", 0, 0, 0); 

	// Calculate the size of the rendering and set up the rendering matrix etc.
	Matrix matConvert;
	FIXED16 fxScale = 1;
	INT32 nSel = NameGallery::Instance()->GetSelectedItemCount();
	DocRect drClip(0, 0, SG_DefaultNameText, nSel * SG_DefaultSmallIcon);

	// Work out the destination bitmap's characteristics.
	double dpi = (double) GetDeviceCaps(sdc.m_hDC, LOGPIXELSX);
	if (nDepth == 0)
			nDepth = GetDeviceCaps(sdc.m_hDC, BITSPIXEL) * GetDeviceCaps(sdc.m_hDC, PLANES);

	// Create a render region with the given properties of the display etc.
	GRenderBitmap* pRenderer = new GRenderBitmap(drClip, matConvert, fxScale, nDepth, dpi);
	ERRORIF(pRenderer == 0, _R(IDE_NOMORE_MEMORY), 0);
	pRenderer->AttachDevice(pView, &sdc, pSpread);
	pRenderer->StartRender();
	pRenderer->SaveContext();

	// Blank the background.
	pRenderer->SetLineWidth(0);
	pRenderer->SetLineColour(COLOUR_WHITE);
	pRenderer->SetFillColour(COLOUR_WHITE);
	pRenderer->DrawRect(&drClip);

	// Render the item's name.
	DocColour dcText(COLOUR_BLACK), dcBack(COLOUR_WHITE);
	pRenderer->SetFixedSystemTextColours(&dcText, &dcBack);

	String_256 strName;
	m_pSourceItem->GetNameText(&strName);
	pRenderer->DrawFixedSystemText(&strName, drClip);

	// Create a kernel bitmap from the OIL bitmap and return it.
	pRenderer->RestoreContext();
	pRenderer->StopRender();
	OILBitmap* pOilMaskBmp = pRenderer->ExtractBitmap();
	delete pRenderer;

	KernelBitmap* pkb = new KernelBitmap(pOilMaskBmp, TRUE);
	ERRORIF(pkb == 0, _R(IDE_NOMORE_MEMORY), 0);
	return pkb;
}



/********************************************************************************************
>	SGNameItem* SGNameDrag::GetDraggedNameAttr()

	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	9/2/95
	Returns:	A pointer to the NameGallery display item being dragged,
********************************************************************************************/

SGNameItem* SGNameDrag::GetDraggedNameAttr()
{
	return m_pSourceItem;
}


/********************************************************************************************
>	SGNameDragTarget::SGNameDragTarget(CGadgetID idGadget = 0);
	 
	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99
	Inputs:		idGadget	---		the gadget within that dialogue which is the target
	Purpose:	Initialises a drag target for an item from the Name gallery.
********************************************************************************************/

SGNameDragTarget::SGNameDragTarget(CGadgetID idGadget)
  :	SGListDragTarget(NameGallery::Instance(), idGadget)
{
	// Empty.
}



/********************************************************************************************
	BOOL SGNameDragTarget::ProcessEvent(DragEventType nEvent, DragInformation* pDragInfo,
										OilCoord* pMousePos, KeyPress* pKeyPress)
	Author:		Justin_Flude (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/7/99
	Inputs:		nEvent		---		indicates what has happened
				pDragInfo	---		points to drag information describing this drag. This
									should be a BitmapDragInformation or derivation thereof
				pMousePos	---		points to information on the current mouse position,
									in OIL coords
				pKeyPress	---		NULL, or if for a keypress event, keypress information
	Returns:	TRUE to claim the event, FALSE to let it through to other targets
	Purpose:	Event handler for SGNameItem gallery drag events. Overrides the
				base class handler to enable it to sort out the node being dragged
				for Name drags.
********************************************************************************************/

BOOL SGNameDragTarget::ProcessEvent(DragEventType nEvent, DragInformation* pDragInfo,
									OilCoord* pMousePos, KeyPress* pKeyPress)
{
	ERROR3IF(!pDragInfo->IS_KIND_OF(SGNameDrag),
				"SGNameDragTarget::ProcessEvent: wrong kind of DragInfo");

	SGDisplayNode* pDraggedNode = ((SGNameDrag*) pDragInfo)->GetDraggedNameAttr();
	if (pDraggedNode == 0) return FALSE;

	switch (nEvent)
	{
	case DRAGEVENT_COMPLETED:
		// Drag just one item.
		HandleDragCompleted((NameGallery*) TargetDialog, pDraggedNode, pMousePos, TRUE);
		return TRUE;

	case DRAGEVENT_MOUSEMOVED:
	case DRAGEVENT_MOUSESTOPPED:
	case DRAGEVENT_MOUSEIDLE:
		return DetermineCursorShape((NameGallery*) TargetDialog, pDraggedNode, pMousePos);
	}

	// Not interested in any other event so don't claim it.
	return FALSE;
}
