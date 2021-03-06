
// Implements click modifiers class


#include "camtypes.h"

#include "pen.h"


DECLARE_SOURCE($Revision$);

CC_IMPLEMENT_MEMDUMP(ClickModifiers,CCObject);

// Declare smart memory handling in Debug builds
#define new CAM_DEBUG_NEW

UINT32 ClickModifiers::ButtFuncNames[]=	{
						_R(IDS_BUTTFUNC_NORMAL),
						_R(IDS_BUTTFUNC_SHIFT),
						_R(IDS_BUTTFUNC_CONSTRAIN),
						_R(IDS_BUTTFUNC_ALTERNATIVE),
						_R(IDS_BUTTFUNC_MENU),
//						_R(IDS_BUTTFUNC_EDITOBJECT),
						_R(IDS_BUTTFUNC_FULLSCREEN),
						_R(IDS_BUTTFUNC_ZOOMIN),
						_R(IDS_BUTTFUNC_PUSHCENTRE)
						};



/********************************************************************************************

	Preference:	LeftButtonFunction
	Section:	Mouse
	Range:		ButtonFunction enum
	Purpose:	Determine the action of the left mouse button.

********************************************************************************************/

ButtonFunction	ClickModifiers::LeftButtonFunction = BUTTFUNC_NORMAL;




/********************************************************************************************

	Preference:	MiddleButtonFunction
	Section:	Mouse
	Range:		ButtonFunction enum
	Purpose:	Determine the action of the middle mouse button.

********************************************************************************************/

ButtonFunction	ClickModifiers::MiddleButtonFunction = BUTTFUNC_PUSHCENTRE;




/********************************************************************************************

	Preference:	RightButtonFunction
	Section:	Mouse
	Range:		ButtonFunction enum
	Purpose:	Determine the action of the right mouse button.

********************************************************************************************/

#ifndef STANDALONE
ButtonFunction	ClickModifiers::RightButtonFunction = BUTTFUNC_MENU;
#else
ButtonFunction	ClickModifiers::RightButtonFunction = BUTTFUNC_SHIFT;
#endif



/********************************************************************************************

>	static BOOL ClickModifiers::DeclarePrefs()

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	09/03/95
	Returns:	TRUE if it worked, FALSE if it failed
	Purpose:	Declares any preferences that the ClickModifiers class needs to declare

********************************************************************************************/

BOOL ClickModifiers::DeclarePrefs()
{
	GetApplication()->DeclareSection( wxT("Mouse"), 5);
	GetApplication()->DeclarePref( wxT("Mouse"), wxT("Left Button"), (INT32*)&LeftButtonFunction);
	GetApplication()->DeclarePref( wxT("Mouse"), wxT("Middle Button"), (INT32*)&MiddleButtonFunction);
	GetApplication()->DeclarePref( wxT("Mouse"), wxT("Right Button"), (INT32*)&RightButtonFunction);

	return TRUE;
}




/********************************************************************************************

>	static void ClickModifiers::GetButtonFuncName(ButtonFunction bf, String_32* pString)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/03/95
	Inputs:		ButtonFunction enum member
	Outputs:	Name of ButtonFunction enum member
	Returns:	-
	Purpose:	Get the name of a button function to show to the user.

********************************************************************************************/

void ClickModifiers::GetButtonFuncName(ButtonFunction bf, String_32* pString)
{
	*pString = String_32( ButtFuncNames[bf] );
}




/********************************************************************************************

>	static void ClickModifiers::RestoreDefaults()

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/03/95
	Inputs:		-
	Outputs:	-
	Returns:	-
	Purpose:	Set the three button function prefs to their default values...

********************************************************************************************/

void ClickModifiers::RestoreDefaults()
{
	LeftButtonFunction = BUTTFUNC_NORMAL;
	MiddleButtonFunction = BUTTFUNC_PUSHCENTRE;
	RightButtonFunction = BUTTFUNC_MENU;
}




/********************************************************************************************

>	static ButtonFunction ClickModifiers::GetButtonFunc(UINT32 ButtonID)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/03/95
	Inputs:		0, 1 or 2 indicating Left MIddle or Right mouse button
	Outputs:	-
	Returns:	Button function ID currently assigned to that button
	Purpose:	Get the function linked to a mouse button.

********************************************************************************************/

ButtonFunction ClickModifiers::GetButtonFunc(UINT32 ButtonID)
{
	switch (ButtonID)
	{
		case 0:
			return LeftButtonFunction;
		case 1:
			return MiddleButtonFunction;
		case 2:
			return RightButtonFunction;
		default:
			ERROR3("GetButtonFunc asked for the function assigned to an unknown button!");
			return BUTTFUNC_NORMAL;
	}
}




/********************************************************************************************

>	static void ClickModifiers::SetButtonFunc(UINT32 ButtonID, ButtonFunction bf)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/03/95
	Inputs:		0, 1 or 2 indicating Left Middle or Right mouse button
	Outputs:	-
	Returns:	-
	Purpose:	Set the function linked to a mouse button.

********************************************************************************************/

void ClickModifiers::SetButtonFunc(UINT32 ButtonID, ButtonFunction bf)
{
	switch (ButtonID)
	{
		case 0:
			LeftButtonFunction = bf;
			return;
		case 1:
			MiddleButtonFunction = bf;
			return;
		case 2:
			RightButtonFunction = bf;
			return;
		default:
			ERROR3("GetButtonFunc asked for the function assigned to an unknown button!");
	}
}




/********************************************************************************************
> 	ClickModifiers::ClickModifiers()

    Author: 	Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
    Created:	07/03/95
	Inputs:		-
	Outputs:	-
	Returns:	-
    Purpose:    Construct a ClickModifiers object and initialise all its member variables.

********************************************************************************************/

ClickModifiers::ClickModifiers()
{
	Adjust 			= FALSE;	// Shift key (Also configurable to a mouse button)
	Menu 			= FALSE;	// User wants a context sensitive menu (Configurable)
	Constrain 		= FALSE;	// Control key (Also configurable to a mouse button)
	Alternative1	= FALSE;	// Left-Alt key (Also configurable to a mouse button)
	Alternative2	= FALSE;	// Right-Alt key ???

	EditObject		= FALSE;	// User wants the best editor for this object (Configurable)
	FullScreen		= FALSE;	// User wants to toggle full-screen mode (Configurable)
	ZoomIn			= FALSE;	// User wants to zoom in one stage (Configurable)

	Pressure		= 0;		// Pen Pressure

	ClickWhileDrag = FALSE;		// TRUE if the click was made while a drag was in progress
	PushCentreTool = FALSE;		// User wants to drag move the view or auto-centre on current mouse pos
}




/********************************************************************************************
> 	ClickModifiers::ClickModifiers(const ClickModifiers& CopyMods)

    Author: 	Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
    Created:	07/03/95
	Inputs:		-
	Outputs:	-
	Returns:	-
    Purpose:    Construct a ClickModifiers object and initialise all its member variables.

********************************************************************************************/

ClickModifiers::ClickModifiers(const ClickModifiers& CopyMods)
{
	Adjust 			= CopyMods.Adjust;			// Shift key (Also configurable to a mouse button)
	Menu 			= CopyMods.Menu;			// User wants a context sensitive menu (Configurable)
	Constrain 		= CopyMods.Constrain;		// Control key (Also configurable to a mouse button)
	Alternative1	= CopyMods.Alternative1;	// Left-Alt key (Also configurable to a mouse button)
	Alternative2	= CopyMods.Alternative2;	// Right-Alt key ???

	EditObject		= CopyMods.EditObject;		// User wants the best editor for this object (Configurable)
	FullScreen		= CopyMods.FullScreen;		// User wants to toggle full-screen mode (Configurable)
	ZoomIn			= CopyMods.ZoomIn;			// User wants to zoom in one stage (Configurable)

	Pressure		= CopyMods.Pressure;		// Pen Pressure

	ClickWhileDrag	= CopyMods.ClickWhileDrag;	// TRUE if the click was made while a drag was in progress
	PushCentreTool	= CopyMods.PushCentreTool;	// TRUE if user wants to drag move view or auto-centre to mouse pos
}




/********************************************************************************************
> 	ClickModifiers& ClickModifiers::operator=(const ClickModifiers& CopyMods)

    Author: 	Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
    Created:	07/03/95
	Inputs:		-
	Outputs:	-
	Returns:	-
    Purpose:    Overload the = operator to copy all the member vars of one to another.

********************************************************************************************/

ClickModifiers& ClickModifiers::operator=(const ClickModifiers& CopyMods)
{
	Adjust 			= CopyMods.Adjust;			// Shift key (Also configurable to a mouse button)
	Menu 			= CopyMods.Menu;			// User wants a context sensitive menu (Configurable)
	Constrain 		= CopyMods.Constrain;		// Control key (Also configurable to a mouse button)
	Alternative1	= CopyMods.Alternative1;	// Left-Alt key (Also configurable to a mouse button)
	Alternative2	= CopyMods.Alternative2;	// Right-Alt key ???

	EditObject		= CopyMods.EditObject;		// User wants the best editor for this object (Configurable)
	FullScreen		= CopyMods.FullScreen;		// User wants to toggle full-screen mode (Configurable)
	ZoomIn			= CopyMods.ZoomIn;			// User wants to zoom in one stage (Configurable)

	Pressure		= CopyMods.Pressure;		// Pen Pressure

	ClickWhileDrag	= CopyMods.ClickWhileDrag;	// TRUE if the click was made while a drag was in progress
	PushCentreTool	= CopyMods.PushCentreTool;	// TRUE if user wants to drag move view or auto-centre to mouse pos

	return *this;
}




/********************************************************************************************
> 	BOOL ClickModifiers::IsHandledByTool()

    Author: 	Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
    Created:	07/03/95
	Inputs:		-
	Outputs:	-
	Returns:	TRUE if this ClickModifier should be passed on to tools
				FALSE otherwise
    Purpose:    Decide whether a tool should get a click modified with this modifier or not.

********************************************************************************************/

BOOL ClickModifiers::IsHandledByTool()
{
	//Graham 19/6/96: Altered to handle clicks while dragging.
	//If the click took place while dragging, the Drag operation should always handle
	//the click (i.e. IF ClickWhileDrag, return TRUE).

	//Otherwise, return TRUE
	//UNLESS any of Edit Object, FullScreen or ZoomIn are TRUE, in which case
	//return FALSE.

	return ClickWhileDrag || !(EditObject || FullScreen || ZoomIn);
}




/********************************************************************************************
> 	static UINT32 ClickModifiers::SynthesizeMouseEventFlags()

    Author: 	Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
    Created:	13/03/95
	Inputs:		-
	Outputs:	-
	Returns:	nFlags in the same format as received from MFC during OnClick, OnMouseMove
				events, etc.
    Purpose:    Create nFlags bitfield by interrogating the state of keys and mouse buttons

********************************************************************************************/

UINT32 ClickModifiers::SynthesizeMouseEventFlags()
{
	UINT32 nFlags = 0;

	// The OS swaps the mouse buttons over before putting them in the nFlags
	// bitfield if a user pref is set so we must do the same...
PORTNOTE("other", "Check use of wxSYS_SWAP_BUTTONS")
//	BOOL swapped = wxSystemSettings::GetMetric( wxSYS_SWAP_BUTTONS );
	BOOL swapped = FALSE;

	wxMouseState state = ::wxGetMouseState();

	nFlags |= (state.ControlDown()		? MK_CONTROL : 0);
	nFlags |= (state.ShiftDown()		? MK_SHIFT : 0);
	nFlags |= (state.AltDown()			? MK_ALT : 0);

	// Note: wxMouseState.LeftDown is equivalent to wxMouseEvent.LeftIsDown
	nFlags |= (state.LeftDown() 		? (swapped ? MK_RBUTTON : MK_LBUTTON) : 0);
	nFlags |= (state.MiddleDown() 		? MK_MBUTTON : 0);
	nFlags |= (state.RightDown() 		? (swapped ? MK_LBUTTON : MK_RBUTTON) : 0);

	return nFlags;
}




/********************************************************************************************
> 	static UINT32 ClickModifiers::SynthesizeMouseEventFlags(wxMouseEvent &event)

    Author: 	Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
    Created:	13/03/95
	Inputs:		wxMouseEvent	event	The event that contains button state
	Outputs:	-
	Returns:	nFlags in the same format as received from MFC during OnClick, OnMouseMove
				events, etc.
    Purpose:    Create nFlags bitfield by interrogating the state of keys and mouse buttons

********************************************************************************************/

UINT32 ClickModifiers::SynthesizeMouseEventFlags(wxMouseEvent &event)
{
	UINT32 nFlags = 0;

	// The OS swaps the mouse buttons over before putting them in the nFlags
	// bitfield if a user pref is set so we must do the same...
PORTNOTE("other", "Check use of wxSYS_SWAP_BUTTONS")
//	BOOL swapped = wxSystemSettings::GetMetric( wxSYS_SWAP_BUTTONS );
	BOOL swapped = FALSE;

	nFlags |= (event.ControlDown() 		? MK_CONTROL : 0);
	nFlags |= (event.ShiftDown()		? MK_SHIFT : 0);
	nFlags |= (event.AltDown()			? MK_ALT : 0);

	// Note: wxMouseState.LeftDown is equivalent to wxMouseEvent.LeftIsDown
	nFlags |= (event.LeftIsDown()		? (swapped ? MK_RBUTTON : MK_LBUTTON) : 0);
	nFlags |= (event.MiddleIsDown()		? MK_MBUTTON : 0);
	nFlags |= (event.RightIsDown()		? (swapped ? MK_LBUTTON : MK_RBUTTON) : 0);

	return nFlags;
}




/*****************************************************************************
>	static ClickModifiers ClickModifiers::GetClickModifiers();

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	18/Jan/2006
	Returns:	struct holding current modifiers for any mouse clicks
	Purpose:	Construct ClickModifiers from current system info
*****************************************************************************/

ClickModifiers ClickModifiers::GetClickModifiers()
{
	UINT32 nFlags = SynthesizeMouseEventFlags();
	return GetClickModifiers(nFlags);
}




/*****************************************************************************
>	static ClickModifiers ClickModifiers::GetClickModifiers(wxMouseEvent& event);

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	18/Jan/2006
	Returns:	struct holding current modifiers for any mouse clicks
	Purpose:	Construct ClickModifiers from current event
*****************************************************************************/

ClickModifiers ClickModifiers::GetClickModifiers(wxMouseEvent& event)
{
	UINT32 nFlags = SynthesizeMouseEventFlags(event);
	return GetClickModifiers(nFlags);
}




/*****************************************************************************
>	static ClickModifiers ClickModifiers::GetClickModifiers(UINT32 nFlags);

	Author:		Ed_Cornes (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/11/94
	Returns:	struct holding current modifiers for any mouse clicks
*****************************************************************************/

ClickModifiers ClickModifiers::GetClickModifiers(UINT32 nFlags)
{
	ClickModifiers	ClickMods;

	// Read keyboard modifier states and reflect them in ClickModifiers...
	ClickMods.Adjust		= ((nFlags & MK_SHIFT)!=0);
	ClickMods.Constrain		= ((nFlags & MK_CONTROL)!=0);
	ClickMods.Alternative1	= ((nFlags & MK_ALT)!=0);

	// Check mouse button function prefs to see if they supply modifiers...
	if ((nFlags & MK_LBUTTON)!=0)
		ClickMods.GetModsFromPrefs(LeftButtonFunction);

	if ((nFlags & MK_MBUTTON)!=0)
		ClickMods.GetModsFromPrefs(MiddleButtonFunction);

	if ((nFlags & MK_RBUTTON)!=0)
		ClickMods.GetModsFromPrefs(RightButtonFunction);

	//Graham 18/6/96: Set ClickWhileDrag if there is a drag in progress
	//That means that this click was made while another mouse button was
	//being used to drag something
	if (Operation::GetCurrentDragOp()!=NULL)
		ClickMods.ClickWhileDrag=TRUE;

// WEBSTER - markn 25/4/97
// No pen stuff required in Webster
// Taken out by vector stroking code Neville 2/10/97
#ifdef VECTOR_STROKING
	// Get pressure information...
	CCPen* pPressurePen    = Camelot.GetPressurePen();
	if (pPressurePen)
		ClickMods.Pressure = pPressurePen->GetPenPressure();
	else
	{
// don't warn in ralph it hasn't got one...
#if !defined(EXCLUDE_FROM_RALPH) && !defined(EXCLUDE_FROM_XARALX)
		ERROR3("CCamApp::GetClickModifiers() - pPressurePen==NULL");
#endif
		ClickMods.Pressure = 0;
	}
#endif // VECTOR_STROKING

	return ClickMods;
}




/********************************************************************************************
> 	void ClickModifiers::GetModsFromPrefs(ButtonFunction buttfunc)

    Author: 	Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
    Created:	07/03/95
	Inputs:		-
	Outputs:	-
	Returns:	ClickModifier object based on inputs and prefs
    Purpose:    Create a click modifier object

********************************************************************************************/

void ClickModifiers::GetModsFromPrefs(ButtonFunction buttfunc)
{
	switch (buttfunc)
	{
		case BUTTFUNC_NORMAL:
			break;

		case BUTTFUNC_SHIFT:
			Adjust = TRUE;
			break;

		case BUTTFUNC_CONSTRAIN:
			Constrain = TRUE;
			break;

		case BUTTFUNC_ALTERNATIVE:
			Alternative1 = TRUE;
			break;

		case BUTTFUNC_MENU:
			Menu = TRUE;
			break;

//		case BUTTFUNC_EDITOBJECT:
//			EditObject = TRUE;
//			break;
//
		case BUTTFUNC_FULLSCREEN:
			FullScreen = TRUE;
			break;

		case BUTTFUNC_ZOOMIN:
			ZoomIn = TRUE;
			break;

		case BUTTFUNC_PUSHCENTRE:
			PushCentreTool = TRUE;
			break;

		default:
			ERROR3("Shouldn't ever get here, but there's no harm if it does");
	}
}



