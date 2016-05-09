


#include "camtypes.h"

#include "plugop.h"			// useful plug-in realted items

#include "plugmngr.h"		// main plug-in manager

#include "pshpop.h"			// PhotoShopContextMenu
#include "bfxop.h"			// BfxContextMenu

// Place any IMPLEMENT type statements here
CC_IMPLEMENT_DYNAMIC(PlugInOpDescriptor, OpDescriptor);
CC_IMPLEMENT_DYNCREATE(PlugInOp, Operation);
CC_IMPLEMENT_MEMDUMP(PlugInsContextMenu, ContextMenu)

// We want better memory tracking
#define new CAM_DEBUG_NEW

/********************************************************************************************

>	PlugInOpDescriptor::PlugInOpDescriptor(const TCHAR* pcszToken, const String_64& NewMenuText,
										   CCRuntimeClass* pClass = CC_RUNTIME_CLASS(PlugInOp),
										   pfnGetState gs = PlugInOp::GetState)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/12/96
	Inputs:		pcszToken		the "OpToken" (OLE verb?) of the associated Operation
				NewMenuText		string resource of menu text
				pClass		the runtime class to use, defaults to PlugInOp.
				gs			the GetState function to use
	Outputs:	-
	Returns:	-
	Purpose:	Constructs the new PlugInOpDescriptor
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

PlugInOpDescriptor::PlugInOpDescriptor(const TCHAR* pcszToken, const String_64& NewMenuText,
									   CCRuntimeClass* pClass, pfnGetState gs)
  :	OpDescriptor(	0,							// tool ID
					_R(IDS_PHOTOSHOP_PLUGIN),		// String resource ID (use same for all)
					pClass, //CC_RUNTIME_CLASS(PlugInOp),	// Runtime class
					(TCHAR*) pcszToken,			// OpToken
					gs,							// GetState function
					NULL,						// help ID
					NULL,						// bubble help
					NULL,						// resource ID
					NULL,						// control ID
					TRUE,						// Recieve system messages
					FALSE,						// Smart duplicate operation
					TRUE,						// Clean operation
					NULL,						// String for one copy only error
					(DONT_GREY_WHEN_SELECT_INSIDE | GREY_WHEN_NO_CURRENT_DOC) // Auto state flags
				)
{
	m_MenuText = NewMenuText;
}

/********************************************************************************************

>	static OpState PlugInOpDescriptor::GetState(String_256* pDesc, OpDescriptor* pOpDesc)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/12/96
	Inputs:		pDesc       --- A pointer to a String. GetState fills this with the
								appropriate details for the conditions arising eg. why
								"Previous Zoom" is greyed out.
				pOpDesc		--- A pointer to the OpDescriptor whose state is being
								queried.
	Returns:	An OpState containing the flags that show what is valid.
	Purpose:	Returns the state that this operation should appear in the menus 
				or as a buttom, for example - greyed out, or ticked.

********************************************************************************************/

OpState PlugInOpDescriptor::GetState(String_256*, OpDescriptor* pOpDesc)
{
	// At present, this item is always available.
	OpState OpSt;
	return OpSt;
}

/********************************************************************************************

>	virtual void PlugInOpDescriptor::GetText(String_256* Description, OpTextFlags WhichText)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/12/96
	Inputs:		TextFlag - Identifies which text string to retrieve from the string resource
	Outputs:	Description - Operation description string if it is found or
				NULL otherwise.
	Returns:	TRUE if successfully retrieves the string and FALSE othersise.
	Purpose:	This function will use the TextID and ModuleID values to obtain a String
				resource describing an operation. String resources may have one or more 
				text descriptions in them, therefore, a TextFlag can be used to identify the
				the appropriate text required.
	SeeAlso:	OpDescriptor::GetText
	SeeAlso:	GetDescription

	Note:		Made virtual by Neville 11/12/96
				We do this so that we do not have to store a string with every OpDesciptor.
				This would be ok if we had dynamic strings but of course we don't. The overhead
				would be unacceptable. So we use this overriding class instead.

********************************************************************************************/

BOOL PlugInOpDescriptor::GetText(String_256* Description, OpTextFlags WhichText)
{
	String_256 ResourceText = m_MenuText; // ( TextID, ModuleID );
	// This string should contain various elements for each part of the apply, undo, redo text
	// each separated by a semi colon. So we will fake this up in the string using the
	// text that we will have set, which is the plug-in name on the menu, for the apply name. 
	ResourceText += ";";			// DO NOT internationlise this!
	ResourceText += m_MenuText;		// add undo text
	ResourceText += ";";			// DO NOT internationlise this!
	ResourceText += m_MenuText;		// add redo text

	TCHAR* ok = NULL;

	// The rest is the same as the main OpDescriptor...

	// Explicitly cast return value from GetDescription from a TCHAR* to a String_256
	ok = GetDescription((TCHAR*) ResourceText, WhichText);
	
	// if description is found then return true else return false
	if (ok)        
	{
		*Description = String_256(ok);
		return TRUE;
	}
	else
		return FALSE;
} 

/********************************************************************************************
/// PlugInOp
********************************************************************************************/

/********************************************************************************************

>	PlugInOp::PlugInOp()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/12/96
	Inputs:
	Outputs:
	Returns:
	Purpose:	Constructor for PlugInOp operation. It is not undoable.
	Errors:		None

********************************************************************************************/

PlugInOp::PlugInOp()
{
}

/********************************************************************************************

>	static BOOL PlugInOp::Init()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/12/96
	Returns:	TRUE if the Op was started ok
	Purpose:	Creates all the opDescriptors that call this operation.

********************************************************************************************/

BOOL PlugInOp::Init()
{
	BOOL ok = TRUE;
	// Set up some standard operations
	ok = ok && RegisterOpToken(OPTOKEN_PLUGINS, _R(IDS_PLUGINS));
//	ok = ok && RegisterOpToken(OPTOKEN_PHOTOSHOP_PLUGINS, _R(IDS_PHOTOSHOP_PLUGINS));
//	ok = ok && RegisterOpToken(OPTOKEN_PHOTOSHOP_APPLYLAST, _R(IDS_PHOTOSHOP_APPLYLAST));

//	ok = ok && RegisterOpToken(OPTOKEN_PLUGINONE, _R(IDS_PLUGIN_ONE));
//	ok = ok && RegisterOpToken(OPTOKEN_PLUGINTWO, _R(IDS_PLUGIN_TWO));
//	ok = ok && RegisterOpToken(OPTOKEN_PLUGINTHREE, _R(IDS_PLUGIN_THREE));
	return ok;
}

/********************************************************************************************

>	static BOOL PlugInOp::RegisterOpToken(TCHAR *OpToken, const String_64& NewMenuText,
										  CCRuntimeClass* pClass = CC_RUNTIME_CLASS(PlugInOp))

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/12/96
	Inputs:		OpToken		the OpToken for the operation
				NewMenuText	the text for the menu item for this operation
				pClass		the runtime class to use, defaults to PlugInOp.
				gs			the GetState function to use
	Returns:	TRUE if the Op was registered ok
	Purpose:	Allows OpDescriptors to be defined to the operation system.

********************************************************************************************/

BOOL PlugInOp::RegisterOpToken(TCHAR *OpToken, const String_64& NewMenuText,
							   CCRuntimeClass* pClass, pfnGetState gs)
{
	// Try to create the OpDescriptor
	PlugInOpDescriptor* pOpDesc = new PlugInOpDescriptor(OpToken, NewMenuText, pClass, gs);
	if (pOpDesc == NULL)
		return FALSE;

	return TRUE;
}

/********************************************************************************************

>	static BOOL PlugInOp::RegisterOpToken(TCHAR *OpToken, UINT32 MenuTextID,
										  CCRuntimeClass* pClass = CC_RUNTIME_CLASS(PlugInOp),
										  pfnGetState gs = PlugInOp::GetState)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/12/96
	Inputs:		OpToken		the OpToken for the operation
				MenuTextId	the text for the menu item for this operation
				pClass		the runtime class to use, defaults to PlugInOp.
				gs			the GetState function to use
	Returns:	TRUE if the Op was registered ok
	Purpose:	Allows OpDescriptors to be defined to the operation system.

********************************************************************************************/

BOOL PlugInOp::RegisterOpToken(TCHAR *OpToken, UINT32 MenuTextID, CCRuntimeClass* pClass,
							   pfnGetState gs)
{
	// register some option descriptors
	if (!Operation::RegisterOpDescriptor(
						0,							// Tool ID
						MenuTextID,					// String resource ID
						pClass, //CC_RUNTIME_CLASS(PlugInOp),	// Runtime class
						OpToken, 					// Token string
						gs,							// GetState function
						NULL,						// help ID
						NULL,						// bubble help
						NULL,						// resource ID
						NULL,						// control ID
						SYSTEMBAR_ILLEGAL,			// Bar ID
						TRUE,						// Recieve system messages
						FALSE,						// Smart duplicate operation
						TRUE,						// Clean operation
						NULL,						// No vertical counterpart
						NULL,						// String for one copy only error
						(DONT_GREY_WHEN_SELECT_INSIDE | GREY_WHEN_NO_CURRENT_DOC) // Auto state flags
						))
		return FALSE;

	return TRUE;
}

/********************************************************************************************

>	static OpState PlugInOp::GetState(String_256*, OpDescriptor*)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/12/96
	Inputs:		
	Outputs:	String - if MenuItem is enabled Then return the name of the operation to 
							be undone Else return the reason why it is disabled
	Returns:	OpState the state of the operation
	Purpose:	To provide greying and ticking functionality for the
				operation.
	Errors:		None

********************************************************************************************/

OpState PlugInOp::GetState(String_256*, OpDescriptor* OpDesc)
{
	// At present, this item is always available.
	OpState OpSt;
	return OpSt;
}

/********************************************************************************************

>	virtual void PlugInOp::Do(OpDescriptor*)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/12/96
	Inputs:		pOpDesc		the OpDescriptor of the item to invoke
	Outputs:	None
	Returns:	None
	Purpose:	Invokes the plug-in specified by the OpDescriptor.
	Errors:		if no OpDescriptor is supplied

********************************************************************************************/

void PlugInOp::Do(OpDescriptor* pOpDesc)
{
	if (pOpDesc == NULL)
	{
		ERROR3IF(pOpDesc == NULL,"PlugInOp::Do null OpDescriptor");
		return;
	}
	//ERROR3("PlugInOp - do");

	// Search the plug-ins list for the specified plug-in and invoke it
	PlugInManager* pManager = GetApplication()->GetPlugInManager();
	if (pManager == NULL)
		return;

	PlugInItem * pPlugIn = pManager->GetFirstPlugIn();
	String_32 OpToken;
	while (pPlugIn)
	{
		OpToken = pPlugIn->GetUniqueID();
		OpToken += pPlugIn->GetPlugInName();
		if (pOpDesc->Token == OpToken)
			pPlugIn->About();

		pPlugIn = pManager->GetNextPlugIn(pPlugIn);
	}

	// and finish
	End();
}


/********************************************************************************************
/// PlugInsContextMenu
********************************************************************************************/


/********************************************************************************************

>	PlugInsContextMenu::PlugInsContextMenu()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/12/96

	Inputs:		-
	Outputs:	-
	Purpose:	To construct ;-)

********************************************************************************************/

PlugInsContextMenu::PlugInsContextMenu()
{
	// As we are just about to invoke UI, check that the plug-in manager has its list
	// of available plug-ins otherwise, go and get it now as we are just about to need it.
	/* PlugInManager* pManager = GetApplication()->GetPlugInManager();
	if (pManager)
		pManager->CheckHaveDetailsOnPlugIns(); */
}

/********************************************************************************************

>	PlugInsContextMenu::PlugInsContextMenu()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	5/3/97
	Inputs:		pBitmap		the bitmap to apply the effect to
				pDocument	the document the bitmap is stored in
	Outputs:	-
	Purpose:	Default constructer.

********************************************************************************************/

PlugInsContextMenu::PlugInsContextMenu(KernelBitmap* pBitmap, Document * pDocument)
{
	// remember them for later use
	m_pBitmap = pBitmap;
	m_pDocument = pDocument;

	// As we are just about to invoke UI, check that the plug-in manager has its list
	// of available plug-ins otherwise, go and get it now as we are just about to need it.
	/* PlugInManager* pManager = GetApplication()->GetPlugInManager();
	if (pManager)
		pManager->CheckHaveDetailsOnPlugIns(); */
}

/********************************************************************************************

>	virtual BOOL PlugInsContextMenu::Build(void)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/12/96
	Returns:	TRUE if menu built OK
				FALSE (and sets error) otherwise
	Purpose:	To build the Plug-ins pop-up menu.

********************************************************************************************/

BOOL PlugInsContextMenu::Build()
{
	return PlugInsContextMenu::BuildMenu(m_pBitmap, m_pDocument, this);
}

/********************************************************************************************

>	static BOOL PlugInsContextMenu::BuildMenu(KernelBitmap * pBitmap, Document * pDocument,
											  ContextMenu * pContextMenu, MenuItem * pMainRoot = NULL,
											  BOOL AddSeparator = FALSE, BOOL UseUndoable = FALSE)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	4/3/97
	Inputs:		pBitmap			bitmap the effect is to be applied to
				pDocument		the document the bitmap is in
				pContextMenu	the menu we want to add to
				pMainRoot		the root menu to add this menu to, defaults to no parent
				AddSeparator	True if want a separator added to the last item on the menu
				UseUndoable		True if want the undoable operations instead of the non-undoable
								Defaults to False
	Returns:	TRUE if menu built OK
				FALSE (and sets error) otherwise
	Purpose:	To build the a plug-ins pop-up menu either directly or tacked on
				as a sub-menu item to the specified pMainRoot. At present, this is the pop-ups
				that appear over the bitmap gallery and so are very bitmap related.

********************************************************************************************/

BOOL PlugInsContextMenu::BuildMenu(KernelBitmap * pBitmap, Document * pDocument,
								   ContextMenu * pContextMenu, MenuItem * pMainRoot,
								   BOOL AddSeparator, BOOL UseUndoable)
{
	ERROR2IF(pContextMenu == NULL,FALSE,"PlugInsContextMenu::BuildMenu null ContextMenu to add to");

	BOOL ok = TRUE;

	// As we are just about to invoke UI, check that the plug-in manager has its list
	// of available plug-ins otherwise, go and get it now as we are just about to need it.
//	PlugInManager* pManager = GetApplication()->GetPlugInManager();
//	if (pManager)
//		pManager->CheckHaveDetailsOnPlugIns();

//#ifndef WEBSTER
//#ifndef EXCLUDE_BFX
//	ok = ok && BfxPlugInContextMenu::BuildMenu(pBitmap, pDocument, pContextMenu, pMainRoot, TRUE, UseUndoable);
//#endif
//#endif // WEBSTER
//	ok = ok && PhotoShopContextMenu::BuildMenu(pBitmap, pDocument, pContextMenu, pMainRoot, FALSE, UseUndoable);

	ok = ok && BfxPlugInContextMenu::BuildMenu(pBitmap, pDocument, pContextMenu, pMainRoot, TRUE, UseUndoable);
	
	return ok;
}

/********************************************************************************************
********************************************************************************************/
