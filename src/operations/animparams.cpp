
#include "camtypes.h"

CC_IMPLEMENT_DYNAMIC(AnimPropertiesParam, OpParam)

// This line mustn't go before any CC_IMPLEMENT_... macros
#define new CAM_DEBUG_NEW


/********************************************************************************************

>	FramePropertiesParam::FramePropertiesParam()

	Author:		Ranbir_Rana (Xara Group Ltd) <camelotdev@xara.com>
	Created:	08/05/97
	Scope:		public

********************************************************************************************/

FramePropertiesParam::FramePropertiesParam()
{
	m_FrameName = "";
	m_FrameDelay = 50;
	m_Overlay = FALSE;
	m_Solid = FALSE;
};




/********************************************************************************************

>	AnimPropertiesParam::AnimPropertiesParam()

	Author:		Ranbir_Rana (Xara Group Ltd) <camelotdev@xara.com>
	Created:	08/05/97
	Scope:		public

********************************************************************************************/

AnimPropertiesParam::AnimPropertiesParam()
{
	m_Animloop			=	0;
	m_GlobalAnimDelay	=	50;
	m_Dither			=	XARADITHER_NONE;
	m_Palette			=	PAL_GLOBAL;
	m_PaletteColours	=	PALCOL_OPTIMIZED;
	m_NumColsInPalette	=	256;
	m_UseSystemColours	=	FALSE;
	m_BackGroundIsTransp =  FALSE;
};




/********************************************************************************************

>	AnimPropertiesParam& AnimPropertiesParam::operator=(const AnimPropertiesParam& obj)

	Author:		Ranbir_Rana (Xara Group Ltd) <camelotdev@xara.com>
	Created:	08/05/97
	Scope:		public

********************************************************************************************/

AnimPropertiesParam& AnimPropertiesParam::operator=(const AnimPropertiesParam& obj)
{
	if (this == &obj)	return *this;

	m_Animloop			=	obj.m_Animloop;
	m_GlobalAnimDelay	=	obj.m_GlobalAnimDelay;
	m_Dither			=	obj.m_Dither;
	m_Palette			=	obj.m_Palette;
	m_PaletteColours	=	obj.m_PaletteColours;
	m_NumColsInPalette	=	obj.m_NumColsInPalette;
	m_UseSystemColours	=	obj.m_UseSystemColours;

	m_BoundingRect		=	obj.m_BoundingRect;

	return *this;
};




