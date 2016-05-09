
// A Dialog Base class that allows you supplies a few functions to help with the
// creation and use of dialogs that render GDraw regions into themselves


#include "camtypes.h"
#include "rnddlgs.h"
#include "ccdc.h"
#include "grnddib.h"


// This is not compulsory, but you may as well put it in so that the correct version
// of your file can be registered in the .exe
DECLARE_SOURCE("$Revision$");

// An implement to match the Declare in the .h file.
CC_IMPLEMENT_MEMDUMP(RenderDialogOp, DialogOp)

// This will get Camelot to display the filename and linenumber of any memory allocations
// that are not released at program exit
#define new CAM_DEBUG_NEW





/********************************************************************************************

>	RenderDialogOp::RenderDialogOp(CDlgResID DialogResID, CDlgMode Mode)
	: DialogOp(DialogResID, Mode)

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	18/10/94
	Inputs:		DialogResID - The Dialogs ID
				Mode - The display mode (Modal, Modeless)
	Purpose:	Constructor - It just passes these parameters down to the dialogOp

********************************************************************************************/

RenderDialogOp::RenderDialogOp(CDlgResID DialogResID, CDlgMode Mode) : DialogOp(DialogResID, Mode)
{
}


