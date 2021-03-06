
// 
// Property.cpp
//
// This file implements the basic useful functionality to implement a property tab.
// This will be basically a tabbed dialog box but this linked list handles a lot
// of the mundane work required to handle a MODELESS property tabbed dialog box.
//
// Curent users of this system are:-
// - The layer roperties dialog box. Brought up from the layer gallery menu item.


#include "camtypes.h"
#include "property.h"
#include "layerprp.h"

// The property tabs themselves
#include "prpslyrs.h"	// Layer properties tab handler
#include "prpsgds.h"	// Guides properties tab handler

// This is not compulsory, but you may as well put it in so that the correct version
// of your file can be registered in the .exe
DECLARE_SOURCE("$Revision$");

// An implement to match the Declare in the .h file.
// If you have many classes, it is recommended to place them all together, here at the start of the file
CC_IMPLEMENT_DYNAMIC(PropertyTabs, ListItem)

// This will get Camelot to display the filename and linenumber of any memory allocations
// that are not released at program exit
#define new CAM_DEBUG_NEW

UnitType 	PropertyTabs::CurrentPageUnits	= NOTYPE;	// The units used to display page measurements in

Document	*PropertyTabs::pScopeDocument 	= NULL;		// Pointer to the current document
Document	*PropertyTabs::pDocument 		= NULL;		// Document we are supposed to be working on
Spread		*PropertyTabs::pSpread 			= NULL; 	// Spread we are supposed to be working on
Layer		*PropertyTabs::pActiveLayer 	= NULL; 	// Layer we are supposed to be working on
Layer		*PropertyTabs::pCurrentLayer 	= NULL; 	// Layer we are supposed to be working on

//List 		PropertyTabs::PropertyTabsList;				// The global list of installed options tabs.

/******************************************************************************************
******************************************************************************************/

// Code to handle a list of options tabs available to the system

/********************************************************************************************

>	PropertyTabs::PropertyTabs()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Purpose:	Constructor.
	Errors:
	SeeAlso:	PropertyTabsDlg;

********************************************************************************************/

PropertyTabs::PropertyTabs()
{
	// Set up some good default values.
//	pScopeDocument 		= NULL;
//	pDocument 			= NULL;
//	pSpread 			= NULL;
//	CurrentPageUnits	= NOTYPE; 
//	pActiveLayer		= NULL;
//	pCurrentLayer		= NULL;
}


/********************************************************************************************

>	PropertyTabs::Init()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Purpose:	Initialiser.
	Errors:
	SeeAlso:	PropertyTabsDlg;

********************************************************************************************/

BOOL PropertyTabs::Init()
{
	// Do nothing for now.
	return TRUE;
}



//********************************************************************************************
//
//>	static BOOL PropertyTabs::DeclarePropertyTab(PropertyTabs *pProperty)
//
//	Author:		Neville
//	Created:	5/12/95
//	Inputs:		A pointer to a new PropertyTabs class already declared into the system
//	Returns:	True if tab declared, False if something went wrong. 
//	Purpose:	A function to declare a new tab to the system.
//	SeeAlso:	LayerProperty
//
//********************************************************************************************/
//
//BOOL PropertyTabs::DeclarePropertyTab(PropertyTabs *pProperty)
//{
//	// First check to see if somebody has actually declared the pointer to be non null.
//	if (pProperty == NULL) return FALSE;
//
//	if (!pProperty->Init())
//	{
//		/* Error occured - report it and stop trying to initialise the options tab. */
//		InformError();
//		return TRUE;
//	}
//	PropertyTabsList.AddTail(pProperty);
//	
//	// All ok
//	return TRUE;
//}
//
//********************************************************************************************
//
//>	static BOOL PropertyTabs::UndeclarePropertyTab(PropertyTabs *pProperty)
//
//	Author:		Neville
//	Created:	5/12/95
//	Inputs:		A pointer to a new PropertyTabs class already declared into the system
//	Returns:	True if tab declared, False if something went wrong. 
//	Purpose:	A function to remove a tab handler that has been declared to the system.
//	SeeAlso:	LayerProperty
//
//********************************************************************************************/
//
//BOOL PropertyTabs::UndeclarePropertyTab(PropertyTabs *pProperty)
//{
//	// First check to see if somebody has actually declared the pointer to be non null.
//	if (pProperty == NULL) return FALSE;
//
//	BOOL Found = FALSE;
//	PropertyTabs * pProps = PropertyTabs::GetFirst();
//	while (pProps && !Found)
//	{
//		if (pProps == pProperty)
//		{
//			// First remove from the list
//			PropertyTabsList.RemoveItem(pProperty);
//			// Do not delete the class itself, as this list just stores the fact that
//			// this property tab is around.
//			pProps = NULL;
//
//			// Flag, we have done as required and deleted the item
//			Found = TRUE;
//		}
//		else
//			pProps = PropertyTabs::GetNext(pProps);
//	}
//
//	ERROR3IF(!Found,"PropertyTabs::UndeclarePropertyTab tab not found");
//	
//	// All ok
//	return TRUE;
//}
//
//********************************************************************************************
//
//>	static PropertyTabs *PropertyTabs::GetFirst()
//
//	Author:		Neville
//	Created:	28/9/95
//	Returns:	Pointer to the first PropertyTab, or NULL if none.
//	Purpose:	Iterating through the list of PropertyTabs known to Camelot.  This function
//				returns the first PropertyTabs.
//	SeeAlso:	PropertyTabs::GetNext()
//	Scope:		Static
//
//********************************************************************************************/
//
//PropertyTabs *PropertyTabs::GetFirst()
//{
//	return (PropertyTabs *) PropertyTabsList.GetHead();
//}
//
//********************************************************************************************
//
//>	static PropertyTabs *PropertyTabs::GetNext(PropertyTabs *pPropertyTabs)
//
//	Author:		Neville
//	Created:	5/12/95
//	Inputs:		pPropertyTabs - pointer to the PropertyTabs to be used to get the following
//				PropertyTabs.
//	Returns:	Pointer to the next PropertyTabs, or NULL if none.
//	Purpose:	Iterating through the list of PropertyTabs known to Camelot.  This function
//				returns the next PropertyTabs.
//	SeeAlso:	PropertyTabs::GetFirst
//
//********************************************************************************************/
//
//PropertyTabs *PropertyTabs::GetNext(PropertyTabs *pPropertyTabs)
//{
//	return (PropertyTabs *) PropertyTabsList.GetNext(pPropertyTabs);
//}



/******************************************************************************************

>	virtual void PropertyTabs::SetDefaultUnits()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Inputs:		-
	Outputs:	-
	Returns:	-
	Purpose:	Sets up the units class variables to the default values. 
	Errors:		-
	SeeAlso:	SetUpDocUnits;

******************************************************************************************/
void PropertyTabs::SetDefaultUnits()
{
	// Just to be on the safe side set up some default values 
	CurrentPageUnits = MILLIMETRES;			// Set up random default units.
}

/******************************************************************************************

>	virtual void PropertyTabs::SetUpDocUnits()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Inputs:		-
	Outputs:	-
	Returns:	TRUE if set up ok, FALSE otherwise.
	Purpose:	Sets up the units variable to the currently set document units 
	Errors:		-
	SeeAlso:	SetDefaultUnits;

******************************************************************************************/

BOOL PropertyTabs::SetUpDocUnits()
{
	BOOL SetUpOk = FALSE; 

	// Set up the default document measurement units that are used
	// Get the page units from the doc unit list of the current document unless there
	// is no document present.
	// Otherwise just pick a random default.
	pDocument = Document::GetSelected();
	if (pDocument == NULL)
	{
		// Just to be on the safe side set up some default values 
		CurrentPageUnits = MILLIMETRES;						
	}
	else
	{
		// Get the unit list attached to the current document
		DocUnitList* pDocUnitList = pDocument->GetDocUnitList();
		ERROR3IF(pDocUnitList == NULL,"PropertyTabs::SetUpDocUnits No doc unit list attached to this doc yet");
	
		if (pDocUnitList != NULL)
		{
			CurrentPageUnits = pDocUnitList->GetPageUnits(); 	// Set to the page units.
		
			// Flag that everything went ok
			SetUpOk = TRUE; 
		}
		else
		{
			// Just to be on the safe side set up some default values 
			CurrentPageUnits = MILLIMETRES;						// Set up random default units.
		}
	}

	return (SetUpOk);
}

/********************************************************************************************

>	virtual void PropertyTabs::SetCurrentDocAndSpread(const Document *pDoc)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Inputs:		Pointer to the document to use, can be NULL of course.
	Outputs:	-
	Returns:	- 
	Purpose:	Sets up the document and spread that we are currently supposed to be working on.
				This can then be used by the tabs instead of each one using their own method
				to work it out.
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

void PropertyTabs::SetCurrentDocAndSpread(Document *pDoc)
{
	// Note this new document in our class variable.
	pDocument = pDoc;

	// Set up what we consider to be the current spread using the current document
	// class variable. 
	if (pDocument)
	{
		pSpread = pDocument->FindFirstSpread();
		// We will get the active layer of this spread
		if (pSpread)
			pActiveLayer = pSpread->FindActiveLayer();		
	}
	else
	{
		pSpread = NULL;
		pActiveLayer = NULL;	
	}
	
}	

/********************************************************************************************

>	virtual void PropertyTabs::SetActiveLayer(Layer * pNewLayer)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	3/10/95
	Inputs:		Pointer to the new active layer to use, can be NULL of course.
	Outputs:	-
	Returns:	- 
	Purpose:	Sets up the active layer that we are currently supposed to be working on.
				This can then be used by the tabs instead of each one using their own method
				to work it out.
				This may be different to a current layer used by say the layer properties tab
				as this will be the one that the layer properties tabbed dialog box was
				brought up over using the adjust click menu on the layer gallery.
	Errors:		-
	SeeAlso:	PropertyTabs::GetActiveLayer; LayerPropertyTabs::GetCurrentLayer;

********************************************************************************************/

void PropertyTabs::SetActiveLayer(Layer * pNewLayer)
{
	pActiveLayer = pNewLayer;	
}

/********************************************************************************************

>	virtual Layer * PropertyTabs::GetActiveLayer()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	3/10/95
	Returns:	A pointer to the current layer that should be used.
	Purpose:	Sets up the layer that we are currently supposed to be working on.
				This can then be used by the tabs instead of each one using their own method
				to work it out.
				This may be different to a current layer used by say the layer properties tab
				as this will be the one that the layer properties tabbed dialog box was
				brought up over using the adjust click menu on the layer gallery.
	Errors:		-
	SeeAlso:	PropertyTabs::SetActiveLayer; LayerPropertyTabs::SetCurrentLayer;

********************************************************************************************/
Layer * PropertyTabs::GetActiveLayer()
{
	return pActiveLayer;		
}
	
/********************************************************************************************

>	virtual Layer * PropertyTabs::GetGuideLayer()

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	5/10/95
	Returns:	A pointer to the guide layer of the associated spread, or NULL
	Purpose:	Returns ptr to the guide layer to use, or NULL if it can't find one
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

Layer * PropertyTabs::GetGuideLayer()
{
	if (pSpread != NULL)
		return (pSpread->FindFirstGuideLayer());

	return NULL;		
}
	

/******************************************************************************************

>	virtual BOOL PropertyTabs::GreyApplyNow()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Inputs:		-
	Outputs:	-
	Returns:	TRUE if successful, else FALSE
	Purpose:	Makes the Apply Now button on the main page of the tabbed dialog grey. 
	Errors:		-
	SeeAlso:	-

******************************************************************************************/

BOOL PropertyTabs::GreyApplyNow()
{
	// First, get a pointer to the dialog box that we are going to use
	DialogTabOp * pPropertiesDlg = GetTabbedDlg();
	ERROR2IF(pPropertiesDlg == NULL, FALSE, "LayerPropertyTabs::GreyApplyNow called with no dialog pointer");

	// Grey out the apply now button on the main page
	// Must restore the state of the currently selected tab as otherwise you might start
	// switching to talking to the main page when we are in the middle of talking to controls
	// on one of the pages and hence give rise to unknown controls messages when we try to
	// talk to the pages controls again.
	CDlgResID PageID = pPropertiesDlg->GetCurrentPageID();	// Get currently selected Tab id
	pPropertiesDlg->TalkToPage(0);

	pPropertiesDlg->EnableGadget(_R(ID_APPLY_NOW), FALSE);
	pPropertiesDlg->EnableGadget(_R(wxID_OK), FALSE);
	pPropertiesDlg->SetStringGadgetValue(_R(wxID_CANCEL),_R(IDS_CLOSEDOCS));

	pPropertiesDlg->TalkToPage(PageID);						// Select the originally selected tab

	return TRUE;
}

/******************************************************************************************

>	virtual BOOL PropertyTabs::UngreyApplyNow()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Inputs:		-
	Outputs:	-
	Returns:	TRUE if successful, else FALSE
	Purpose:	Makes the Apply Now button on the main page of the tabbed dialog ungrey. 
	Errors:		-
	SeeAlso:	-

******************************************************************************************/

BOOL PropertyTabs::UngreyApplyNow()
{
	// First, get a pointer to the dialog box that we are going to use
	DialogTabOp * pPropertiesDlg = GetTabbedDlg();
	ERROR2IF(pPropertiesDlg == NULL,FALSE,"LayerPropertyTabs::UngreyApplyNow called with no dialog pointer");
	
	// Ungrey the apply now button on the main page
	// Must restore the state of the currently selected tab as otherwise you might start
	// switching to talking to the main page when we are in the middle of talking to controls
	// on one of the pages and hence give rise to unknown controls messages when we try to
	// talk to the pages controls again.
	CDlgResID PageID = pPropertiesDlg->GetCurrentPageID();	// Get currently selected Tab id
	pPropertiesDlg->TalkToPage(0);						// Select the main tab

	pPropertiesDlg->EnableGadget(_R(ID_APPLY_NOW), TRUE);		// ungrey button on main tab
	pPropertiesDlg->EnableGadget(_R(wxID_OK), TRUE);
	pPropertiesDlg->SetStringGadgetValue(_R(wxID_CANCEL),_R(IDS_CANCEL));

	pPropertiesDlg->TalkToPage(PageID);						// Select the originally selected tab

	return TRUE;
}


/******************************************************************************************

>	virtual BOOL PropertyTabs::NewUnitsInSection()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Inputs:		-
	Outputs:	-
	Returns:	TRUE if successful, else FALSE
	Purpose:	Called when new default units chosen. Gives any tabbed dialog a chance to
				update any fields with new units in.
				This is the base class version and so will be used for any tabs which have
				no specified handling code. 
	Errors:		-
	SeeAlso:	-

******************************************************************************************/

BOOL PropertyTabs::NewUnitsInSection()
{
	// Called when new default units chosen
	// Would normally be overriden by a a particular tab
	return TRUE;
}

/******************************************************************************************

>	BOOL virtual PropertyTabs::GreySection()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Inputs:		-
	Outputs:	-
	Returns:	TRUE if successful, else FALSE
	Purpose:	Called when the user has closed all documents. Gives any tabbed dialog a
				chance to grey any fields which are document specific.
				This is the base class version and so will be used for any tabs which have
				no specified handling code.  
	Errors:		-
	SeeAlso:	PropertyTabs::UngreySection(); PropertyTabs::UpdateSection();

******************************************************************************************/

BOOL PropertyTabs::GreySection()
{
	// Called when the user has closed all documents
	// Would normally be overriden by a a particular tab
	return TRUE;
}

/******************************************************************************************

>	virtual BOOL PropertyTabs::UngreySection()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Inputs:		-
	Outputs:	-
	Returns:	TRUE if successful, else FALSE
	Purpose:	Called when the user has opened a document. Gives any tabbed dialog a
				chance to update any fields which may have been greyed when the last
				document was closed.
				This is the base class version and so will be used for any tabs which have
				no specified handling code.   
	Errors:		-
	SeeAlso:	 PropertyTabs::GreySection(); PropertyTabs::UpdateSection();

******************************************************************************************/

BOOL PropertyTabs::UngreySection()
{
	// Called when the user has opened a document
	// Would normally be overriden by a a particular tab
	return TRUE;
}

/******************************************************************************************

>	virtual BOOL PropertyTabs::UpdateSection()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Returns:	TRUE if successful, else FALSE
	Purpose:	Called when the user has clicked on a new document or opened a new document.
				Gives any tabbed dialog a chance to update any fields which are used to
				reflect the current document's status.
				This is the base class version and so will be used for any tabs which have
				no specified handling code.   
	Errors:		-
	SeeAlso:	PropertyTabs::UngreySection(); PropertyTabs::GreySection();

******************************************************************************************/

BOOL PropertyTabs::UpdateSection()
{
	// Called when the user has opened a document
	// Would normally be overriden by a a particular tab
	return TRUE;
}

/******************************************************************************************

>	virtual BOOL PropertyTabs::UpdateSpreadSection()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	3/10/95
	Returns:	TRUE if successful, else FALSE
	Purpose:	Called when the user has clicked or done something to change the selected 
				spread.
				Gives any tabbed dialog a chance to update any fields which are used to
				reflect the current spread's status.
				This is the base class version and so will be used for any tabs which have
				no specified handling code.   
	Errors:		-
	SeeAlso:	PropertyTabs::UngreySection(); PropertyTabs::GreySection();
	SeeAlso:	PropertyTabs::UpdateSection(); PropertyTabs::UpdateLayerSection();

******************************************************************************************/

BOOL PropertyTabs::UpdateSpreadSection()
{
	// Called when the user has changed the selected spread
	// Would normally be overriden by a a particular tab
	return TRUE;
}

/******************************************************************************************

>	virtual BOOL PropertyTabs::UpdateLayerSection()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	3/10/95
	Returns:	TRUE if successful, else FALSE
	Purpose:	Called when the user has clicked or done something to change the selected 
				layer.
				Gives any tabbed dialog a chance to update any fields which are used to
				reflect the current layer's status.
				This is the base class version and so will be used for any tabs which have
				no specified handling code.   
	Errors:		-
	SeeAlso:	PropertyTabs::UngreySection(); PropertyTabs::GreySection();
	SeeAlso:	PropertyTabs::UpdateSection(); PropertyTabs::UpdateSpreadSection();

******************************************************************************************/

BOOL PropertyTabs::UpdateLayerSection()
{
	// Called when the user has changed the selected/active layer
	// Would normally be overriden by a a particular tab
	return TRUE;
}


/******************************************************************************************

>	virtual BOOL PropertyTabs::UpdateGuidelineSection()

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	4/10/95
	Returns:	TRUE if successful, else FALSE
	Purpose:	Called when the user has clicked or done something to change the guidelines
				of the *active* layer.
				Gives any tabbed dialog a chance to update any fields which are used to
				reflect the current layer's status.
				This is the base class version and so will be used for any tabs which have
				no specified handling code.   
	Errors:		-
	SeeAlso:	PropertyTabs::UngreySection(); PropertyTabs::GreySection();
	SeeAlso:	PropertyTabs::UpdateSection(); PropertyTabs::UpdateSpreadSection();

******************************************************************************************/

BOOL PropertyTabs::UpdateGuidelineSection()
{
	// Called when the user has changed the selected/active layer
	// Would normally be overriden by a a particular tab
	return TRUE;
}

/******************************************************************************************

>	virtual BOOL PropertyTabs::ColourListChanged(Document* pDoc)

	Author:		Mark_Neves (Xara Group Ltd) <camelotdev@xara.com>
	Created:	9/10/95
	inputs:		pDoc = ptr to document that contains the changed list, or NULL if there's no longer a list
	Returns:	TRUE if successful, else FALSE
	Purpose:	Called when the colour list changes in some way.
				pDoc points to the doc that contains the list that's changed.
				If pDoc is NULL, then there is no longer a colour list (e.g. all docs closed)
	Errors:		-
	SeeAlso:	-

******************************************************************************************/

BOOL PropertyTabs::ColourListChanged(Document* pDoc)
{
	return TRUE;
}

/******************************************************************************************

>	virtual BOOL PropertyTabs::IsPropertyRequired()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/9/95
	Inputs:		-
	Outputs:	-
	Returns:	Returns True if this tab is required.	  
	Purpose:	Allows the layer properties options dialog code to determine if this tab
				belongs is required at this point.
				This base class version allow the majority of tabs to answer yes to this
				question and hence only needs overiding if the tab is not required.
	Errors:		-

******************************************************************************************/

BOOL PropertyTabs::IsPropertyRequired()
{
	return FALSE;
}


/********************************************************************************************

>	virtual void PropertyTabs::SetCurrentLayer(Layer * pNewLayer)

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	3/10/95
	Inputs:		Pointer to the layer to use, can be NULL of course.
	Outputs:	-
	Returns:	- 
	Purpose:	Sets up the layer that we are currently supposed to be working on.
				This can then be used by the tabs instead of each one using their own method
				to work it out.
				This may be different to the active layer set up in the baseclass. The current
				one as far as we are concerned is the one that the layer properties tabbed
				dialog box was brought up over using the adjust click menu on the layer gallery.
	Errors:		-
	SeeAlso:	PropertyTabs::GetActiveLayer; PropertyTabs::GetCurrentLayer;

********************************************************************************************/

void PropertyTabs::SetCurrentLayer(Layer * pNewLayer)
{
	PropertyTabs::pCurrentLayer = pNewLayer;	
}

/********************************************************************************************

>	virtual Layer * PropertyTabs::GetCurrentLayer()

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	3/10/95
	Returns:	A pointer to the current layer that should be used.
	Purpose:	Sets up the layer that we are currently supposed to be working on.
				This can then be used by the tabs instead of each one using their own method
				to work it out.
				This may be different to the active layer set up in the baseclass. The current
				one as far as we are concerned is the one that the layer properties tabbed
				dialog box was brought up over using the adjust click menu on the layer gallery.
	Errors:		-
	SeeAlso:	PropertyTabs::SetActiveLayer; PropertyTabs::SetCurrentLayer;

********************************************************************************************/

Layer * PropertyTabs::GetCurrentLayer()
{
	return PropertyTabs::pCurrentLayer;		
}

/******************************************************************************************

>	virtual BOOL PropertyTabs::SetOkState(BOOL Val)

	Author:		Ranbir_Rana (Xara Group Ltd) <camelotdev@xara.com>
	Created:	16/07/97
	Inputs:		Val - BOOLEAN Value to determine whether to grey/ungrey the OK button.
	Outputs:	-
	Returns:	TRUE if successful, else FALSE
	Purpose:	Greys/Ungreys  the OK button on the main page of the tabbed dialog.
	
******************************************************************************************/
BOOL PropertyTabs::SetOkState(BOOL Val)
{
	// Get a pointer to the dialog box that we are going to use.
	DialogTabOp * pPropertiesDlg = GetTabbedDlg();
	ERROR2IF(pPropertiesDlg == NULL,FALSE,"LayerPropertyTabs::UngreyApplyNow called with no dialog pointer");
	
	// Grey/Ungrey the OK button on the main page.
	// Get currently selected Tab id
	CDlgResID PageID = pPropertiesDlg->GetCurrentPageID();	

	// Select the main tab
	pPropertiesDlg->TalkToPage(0);					

	// ungrey button on main tab
	pPropertiesDlg->EnableGadget(_R(ID_OK), Val);		
		
	// Select the originally selected tab
	pPropertiesDlg->TalkToPage(PageID);						

	return TRUE;
}


	

/******************************************************************************************
********************************************************************************************/
