
// Implementation file for operation to break paths at selected points

#include "camtypes.h"
#include "opbreak.h"
#include "nodepath.h"
#include "ndtxtpth.h"
#include "objchge.h"
#include "ndbldpth.h"
#include "ophist.h"

DECLARE_SOURCE("$Revision$");
CC_IMPLEMENT_DYNCREATE( OpBreakAtPoints, SelOperation )

#define new CAM_DEBUG_NEW

/* things to remember

(2) The GetState function is exactly the same as DeletePoints::GetState so
	we might be able to merge the two.
(4) Force DoStartSelOp to return FALSE to make sure we recover correctly.
(5) something has to be done about ExtraInfo
	ie in RotatePathLeft()
			if (ExtraInfo != NULL)
				ExtraInfo->ShiftDownExtraInfo(StartSlot, NumSlots, SlotsToMove);
	Ask Will!
(6) test both parts to rotatepathleft() by causing the tempbuff memory claim to fail

*/

/********************************************************************************************

>	OpBreakAtPoints::OpBreakPoints() 

	Author:		Mike_Kenny (Xara Group Ltd) <camelotdev@xara.com>
	Created:	30/8/94
	Purpose:	OpBreakAtPoints constructor
	SeeAlso:	-

********************************************************************************************/

OpBreakAtPoints::OpBreakAtPoints()
{
	// Dummy constructor
}
  
 
/********************************************************************************************

>	BOOL OpBreakAtPoints::Init()

	Author:		Mike_Kenny (Xara Group Ltd) <camelotdev@xara.com>
	Created:	26/7/94
	Inputs:		-
	Outputs:	-
	Returns:	TRUE if the operation could be successfully initialised 
				FALSE if no more memory could be allocated 
				
	Purpose:	OpDeletePoints initialiser method
	Errors:		ERROR will be called if there was insufficient memory to allocate the 
				operation.
	SeeAlso:	-

********************************************************************************************/

BOOL OpBreakAtPoints::Init()
{
	return (RegisterOpDescriptor(0,									// tool ID
 								_R(IDS_BREAKATPOINTSOP),				// string resource ID
								CC_RUNTIME_CLASS(OpBreakAtPoints),	// runtime class for Op
 								OPTOKEN_BREAKATPOINTS,				// Ptr to token string
 								OpBreakAtPoints::GetState,			// GetState function
 								0,									// help ID
 								_R(IDBBL_BREAKATPOINTSOP),				// bubble help ID
 								_R(IDD_BARCONTROLSTORE),				// resource ID
								_R(IDC_BTN_BREAKATPOINTSOP),			// control ID
								SYSTEMBAR_EDIT,						// Group bar ID
								TRUE, 								// Receive messages
								FALSE,								// Smart
								FALSE,								// Clean
								0,									// One open Instance ID
								0

 								)); 

}               



/********************************************************************************************

>	OpState	OpBreakAtPoints::GetState(String_256*, OpDescriptor*)

	Author:		Mike_Kenny (Xara Group Ltd) <camelotdev@xara.com>
	Created:	30/8/94
	Inputs:		-
	Outputs:	-
	Returns:	The state of the OpBreakAtPoints
	Purpose:	For finding the OpBreakAtPoints's state. 
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

OpState	OpBreakAtPoints::GetState(String_256* UIDescription, OpDescriptor*)
{

	OpState OpSt;
	String_256 DisableReason; 

   	OpSt.Greyed = FALSE;
	BOOL FoundSelected = FALSE;

	// Go through the selection until we find a selected point

	SelRange* Selected = GetApplication()->FindSelection();
	Node* pNode = Selected->FindFirst();

	while (pNode)
	{
		if (IS_A(pNode,NodePath) || IS_A(pNode,NodeBlendPath))
		{
			NodePath* pNodePath = (NodePath*)pNode;
			INT32 NumSplinters = pNodePath->InkPath.NumSplinters();

			if (NumSplinters > 0)
			{
				// We need to ask the effected nodes if they (and their parents) can handle this node being replaced
				ObjChangeFlags cFlags;

				if (NumSplinters > 1)
					cFlags.MultiReplaceNode = TRUE;	// Node will be replaced with more than one node.
				else
					cFlags.ReplaceNode = TRUE;		// Node will be replaced with one node only.

				String_32 optokenstring(OPTOKEN_BREAKATPOINTS);
				ObjChangeParamWithToken ObjChange(OBJCHANGE_STARTING,cFlags,pNodePath,NULL,&optokenstring);

				// Will the node allow this op to happen?
				if (pNodePath->AllowOp(&ObjChange,FALSE))
				{
					FoundSelected = TRUE;
					break;
				}
			}
		}
		pNode = Selected->FindNext(pNode);
	}

	// The operation is disabled if there are no complex paths selected

	if (!FoundSelected)
	{
		OpSt.Greyed = TRUE;
		DisableReason = String_256(_R(IDS_NEEDS_SELECTED_POINT));
		*UIDescription = DisableReason;
	}
	
	return(OpSt);   
}


/********************************************************************************************

>	void OpBreakAtPoints::GetOpName(String_256* OpName)

	Author:		Mike_Kenny (Xara Group Ltd) <camelotdev@xara.com>
	Created:	9/9/94
	Inputs:		-
	Outputs:	The undo string for the operation
	Returns:	
	Purpose:	The GetOpName fn is overridden so that we return back a description
				appropriate to the type of attribute that the operation applies.
					
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

void OpBreakAtPoints::GetOpName(String_256* OpName)
{
	*OpName = String_256(_R(IDS_UNDO_BREAKATPOINTSOP));
}







/********************************************************************************************

>	void OpBreakAtPoints::Do(OpDescriptor*)

	Author:		Mike_Kenny (Xara Group Ltd) <camelotdev@xara.com>
	Created:	30/8/94
	Inputs:		pointer to an OpDescriptor (unused)
	Outputs:	-
	Returns:	-
	Purpose:	This operation works on the selected objects in the current document. 
				It will scan through all path objects in the selection and if any contain
				selected points, break the path at those points, producing a number of
				output paths. These paths will be connected on the next sibling to the
				original. All attributes will be distributed to the output siblings.
	Errors:		failandexecute will be called if the operation fails in some way, most
				likely when no memory is available. 

********************************************************************************************/
	
void OpBreakAtPoints::Do(OpDescriptor*)
{   
	
	// Obtain the current selections 
	SelRange* Selected = GetApplication()->FindSelection();
	NodePath* pSplitNode;

	// Now, because we're going to be doing mad things to the selection, we have to make a list
	// of all the selected nodes, so that adding nodes into the tree won't confuse us

	List* NodeList = Selected->MakeListOfNodes();
	NodeListItem* CurItem = (NodeListItem*)(NodeList->GetHead());

	if (!CurItem)
		goto FailAndDeleteList;

	if (!DoStartSelOp(TRUE,TRUE))
		goto FailAndDeleteList;

	while (CurItem)
	{
		// get a pointer to the NodePath
		NodePath* pThisNode = (NodePath*)(CurItem->pNode);

		// Only interested in NodePaths that have a sub selection, and that will allow the op to happen
		if ((IS_A(pThisNode,NodePath) || IS_A(pThisNode,NodeBlendPath)) && pThisNode->InkPath.IsSubSelection())
		{
			// Find out how many nodes this op will reproduce
			INT32 NumSplinters = pThisNode->InkPath.NumSplinters();
			BOOL DoThisNode = FALSE;

			if (NumSplinters > 0)
			{
				// We need to ask the effected nodes if they (and their parents) can handle this node being replaced
				ObjChangeFlags cFlags;

				if (NumSplinters > 1)
					cFlags.MultiReplaceNode = TRUE;	// Node will be replaced with more than one node.
				else
					cFlags.ReplaceNode = TRUE;		// Node will be replaced with one node only.

				ObjChangeParam ObjChange(OBJCHANGE_STARTING,cFlags,NULL,this);
				DoThisNode = pThisNode->AllowOp(&ObjChange);
			}

			if (DoThisNode)
			{

				BOOL ok;
				Node* pnode;

				// Copy the nodepath and all its children, without placing the copy in the tree
				CALL_WITH_FAIL(pThisNode->NodeCopy(&pnode), this, ok);
				if (!ok) goto DeleteList;
				pSplitNode = (NodePath*)pnode;

				// remove the fill from this path as we're about to open it
				pSplitNode->InkPath.IsFilled = FALSE;

				// Now stick the new path into the tree
				CALL_WITH_FAIL
				(
					DoInsertNewNode(pSplitNode, pThisNode, NEXT, TRUE, FALSE),
					this,ok
				);
				
				if (!ok) 
					goto DeleteListAndPath;

				// Now breakup this copy of the path where necessary
				Path* pChildPath;
				INT32 split;

			    do
			    {
					// Create a new path, ready for split
					ALLOC_WITH_FAIL(pChildPath, new Path, this);
					if (!pChildPath) 
						goto DeleteList;

					// Now split the path, possibly into two pieces.
			    	split = pSplitNode->InkPath.BreakInTwo(pChildPath);

					if (split==-1) 
					{
						InformError(_R(IDS_OUT_OF_MEMORY), _R(IDS_OK));
						delete pChildPath;
						goto FailAndDeleteList;
					}

					/*	Karim 05/12/2000
						No longer required - see code addition at the bottom of this loop.

					if (split==1) 
			    	{
						delete pChildPath;
			    		continue;
					}

					*/

					if (split>1)
					{

						// update the split paths bounding rectangle
						pSplitNode->InvalidateBoundingRect();

						// Create a new nodepath.
						NodePath* pChildNode;
						ALLOC_WITH_FAIL(pChildNode, new NodePath(), this);
						if (!pChildNode)
						{
							delete pChildPath;
							goto DeleteList;
						}

						// make room for the new path in the node path.
						CALL_WITH_FAIL
						(
							pChildNode->SetUpPath(pChildPath->GetNumCoords(),12),
							this,ok
						);
				
						if (!ok)
						{
							delete pChildNode;
							delete pChildPath;
							goto DeleteList;
						}

						// now copy the path data in there.
						pChildNode->InkPath.CopyPathDataFrom(pChildPath);
						delete pChildPath;

						// Clear the selection flag from the first element in both the split
						// and child paths.  All others apart from the last will be	unselected
						// by definition.  Also select the last element in the child
						(pSplitNode->InkPath.GetFlagArray())[0].IsSelected = FALSE;
						(pChildNode->InkPath.GetFlagArray())[0].IsSelected = FALSE;
						(pChildNode->InkPath.GetFlagArray())[(pChildNode->InkPath.GetNumCoords()-1)].IsSelected = TRUE;
						pChildNode->InkPath.IsFilled = FALSE;

						// now, copy all attributes from the parent split to the child split
						Node* pAttr = pSplitNode->FindFirstChild();
						while (pAttr != NULL)
						{
							if (pAttr->IsKindOf(CC_RUNTIME_CLASS(NodeAttribute)))
							{
								Node* pAttrCopy;
								CALL_WITH_FAIL(pAttr->NodeCopy(&pAttrCopy), this,ok);
								if (!ok) 
								{
									pChildNode->CascadeDelete();
									delete pChildNode;
									goto DeleteList;
								}
								pAttrCopy->AttachNode(pChildNode, FIRSTCHILD);
							}
							pAttr = pAttr->FindNext();
						}

						for (INT32 loop = 0; loop < pChildNode->InkPath.GetNumCoords(); loop ++)
						{
							pChildNode->InkPath.GetVerbArray()[loop] = pChildNode->InkPath.GetVerbArray()[loop] & ~PT_CLOSEFIGURE;
						}

						// Now stick the new path into the tree
						CALL_WITH_FAIL
						(
							DoInsertNewNode(pChildNode, pSplitNode, NEXT, TRUE, FALSE),
							this,ok
						);

						if (!ok)
						{
							pChildNode->CascadeDelete();
							delete pChildNode;
							goto DeleteList;
						}

						pSplitNode = pChildNode;
					}

					// Karim 05/12/2000
					// Fix for memory leak.
					else
					{
						delete pChildPath;
					}
			    }
			    while (split);

				// Clear out any remaining closefigures on the last bit of the path
				for (INT32 loop = 0; loop < pSplitNode->InkPath.GetNumCoords(); loop ++)
				{
					pSplitNode->InkPath.GetVerbArray()[loop] = pSplitNode->InkPath.GetVerbArray()[loop] & ~PT_CLOSEFIGURE;
				}

				// Now we've broken up this path, let's hide it
				CALL_WITH_FAIL(DoHideNode(pThisNode,TRUE), this, ok)
				if (!ok) goto DeleteList;
			}
		}
		CurItem = (NodeListItem*)(NodeList->GetNext(CurItem));
	}

	// Update all the parents of this node
	{
		ObjChangeFlags cFlags;
		ObjChangeParam ObjChange(OBJCHANGE_FINISHED,cFlags,NULL,this);
		if (!UpdateChangedNodes(&ObjChange))
			goto FailAndDeleteList;
	}

 	End();

	// delete the nodelist (and all the list items)
	while (!NodeList->IsEmpty())
		delete (NodeListItem*)(NodeList->RemoveHead());
	delete NodeList;
	return;


FailAndDeleteList:
	while (!NodeList->IsEmpty())
		delete (NodeListItem*)(NodeList->RemoveHead());
	delete NodeList;
	FailAndExecute();
	End();
	return;

DeleteListAndPath:
	pSplitNode->CascadeDelete();
	delete pSplitNode;

DeleteList:
	while (!NodeList->IsEmpty())
		delete (NodeListItem*)(NodeList->RemoveHead());
	delete NodeList;
	End();
	return;

}


