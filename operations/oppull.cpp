
// Implementation of the "Pull onto grid" Op


#include "camtypes.h"
#include "oppull.h"


DECLARE_SOURCE("$Revision$");

CC_IMPLEMENT_DYNCREATE(OpPullOntoGrid,OpTranslateTrans)

#define new CAM_DEBUG_NEW



/********************************************************************************************

>	static BOOL OpPullOntoGrid::Init()

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	30/03/95
	Inputs:		-
	Outputs:	-
	Returns:	TRUE if the operation could be successfully initialised 
				FALSE if no more memory could be allocated 
	Purpose:	OpPullOntoGrid initialiser method.
	Errors:		ERROR will be called if there was insufficient memory to allocate the 
				operation.
	SeeAlso:	-

********************************************************************************************/

BOOL OpPullOntoGrid::Init()
{
	BOOL registered = RegisterOpDescriptor(
			 								0,
			 								_R(IDS_PULLONTOGRID),
											CC_RUNTIME_CLASS(OpPullOntoGrid),
			 								OPTOKEN_PULLONTOGRID,
			 								OpPullOntoGrid::GetState,
											0,
											_R(IDBBL_PULLONTOGRID)
										  );
	return registered;
}               




/********************************************************************************************

>	static OpState OpPullOntoGrid::GetState(String_256*, OpDescriptor*)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	30/03/95
	Inputs:		ptr to a string to place a description of what went wrong (if it did!)
	Outputs:	-
	Returns:	The state of the OpPullOntoGrid operation
	Purpose:	For finding OpPullOntoGrid's state. 
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

OpState	OpPullOntoGrid::GetState(String_256* UIDescription, OpDescriptor*)
{
	OpState OpSt;

	SelRange* pSelRange = GetApplication()->FindSelection();
	if (pSelRange != NULL && pSelRange->FindFirst() != NULL)
		OpSt.Greyed = FALSE;
	else
		OpSt.Greyed = TRUE;

	return (OpSt);   
}




/********************************************************************************************

>	void OpPullOntoGrid::Do(OpDescriptor* pOpDesc)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	30/03/95
	Inputs:		pOpDesc = ptr to the op descriptor
	Outputs:	-
	Returns:	-
	Purpose:	The pull onto grid op's Do() function.
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

void OpPullOntoGrid::Do(OpDescriptor* pOpDesc)
{  
	DocCoord 		Offset;
	TransformData 	TransData;

	// Set up the transform data
	TransData.CentreOfTrans.x 	= 0;
	TransData.CentreOfTrans.y 	= 0;
	TransData.StartBlob 	  	= 0;
	TransData.LockAspect 		= TRUE;
	TransData.LeaveCopy  		= FALSE;
	TransData.ScaleLines 		= FALSE;
	TransData.TransFills 		= TRUE;
	TransData.pRange = 0;

	SelRange* pSel = GetApplication()->FindSelection();
	if (pSel)
	{
		// Set up the Offset DocCoord to contain the X and Y translation values
		DocRect Bounds = pSel->GetBoundingRect();
		DocCoord Offset = Bounds.lo;
		DocView::ForceSnapToGrid(Document::GetSelectedSpread(),&Offset);
		Offset = Offset - Bounds.lo;

		// Call OpTranslateTrans::DoWithParams() with a ptr to the transform data and a ptr to a DocCoord
		// that specs the X and Y offsets of the translation
		OpParam temp((void *)&TransData, (void *)&Offset);
		DoWithParam(pOpDesc, &temp);
	}
}

