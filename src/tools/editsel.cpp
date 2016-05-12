

// Edit the selected item whatever that item is...


#include "camtypes.h"

#include "editsel.h"

#include "helpids.h"

#include "blobs.h"

///////////////////////////////////////////////////////////////////////////////////////////
//									EditSelectionOp

/******************************************************************************************
>	EditSelectionOp::EditSelectionOp()

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	23/01/2004
	Purpose:	Constructs a new EditSelectionOp object: setting default operation flags, and 
				adding it to the Live list. 
******************************************************************************************/

CC_IMPLEMENT_DYNCREATE(EditSelectionOp, Operation)

EditSelectionOp::EditSelectionOp(): Operation()
{
}


/******************************************************************************************
>	void EditSelectionOp::Do(OpDescriptor*)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	23/01/2004
	Inputs:		An Operation Descriptor
	Purpose:	Actually "DO" a select all operation.
********************************************************************************************/

void EditSelectionOp::Do(OpDescriptor*)
{
	Range* pSelection = GetApplication()->FindSelection();

	if (pSelection)
	{
		Node* pNode = pSelection->FindFirst();
		if (pNode)
		{
			TCHAR* pOpToken = pNode->GetDefaultOpToken();
			if (pOpToken)
			{
				OpDescriptor* pOpDesc = OpDescriptor::FindOpDescriptor(pOpToken);
				if (pOpDesc)
				{
					pOpDesc->Invoke();
					return;
				}
			}
		}
	}

	End();
}


/********************************************************************************************
>	OpState EditSelectionOp::GetState(String_256* UIDescription, OpDescriptor*)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	23/01/2004
	Inputs:		String - representing the operation description which may be updated
				OpDescriptor - To help update the state
	Purpose:	Find the state of the EditSelectionOp operation.
********************************************************************************************/

OpState EditSelectionOp::GetState(String_256* UIDescription, OpDescriptor*)
{
	INT32 NumSelObjs=GetApplication()->FindSelection()->Count();

	// If no objects are selected, state this as the reason why it's disabled..
	if (NumSelObjs==0)
	{
		*UIDescription = String_256(_R(IDS_NO_OBJECTS_SELECTED));
	}

	// Never ticked, greyed if no selected objects
	return OpState(FALSE, NumSelObjs==0);
}


/********************************************************************************************
>	BOOL EditSelectionOp::Init()

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	23/01/2004
	Purpose:	Create an OpDescriptor for the Select All operation
********************************************************************************************/

BOOL EditSelectionOp::Init()
{
	BTNOP(EDIT_EDITSELECTION,EditSelectionOp,EDIT);
	return(TRUE);
}


