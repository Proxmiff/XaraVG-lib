// $Id$
/* @@tag:xara-cn@@ DO NOT MODIFY THIS LINE
================================XARAHEADERSTART===========================
 
               Xara LX, a vector drawing and manipulation program.
                    Copyright (C) 1993-2006 Xara Group Ltd.
       Copyright on certain contributions may be held in joint with their
              respective authors. See AUTHORS file for details.

LICENSE TO USE AND MODIFY SOFTWARE
----------------------------------

This file is part of Xara LX.

Xara LX is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License version 2 as published
by the Free Software Foundation.

Xara LX and its component source files are distributed in the hope
that it will be useful, but WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with Xara LX (see the file GPL in the root directory of the
distribution); if not, write to the Free Software Foundation, Inc., 51
Franklin St, Fifth Floor, Boston, MA  02110-1301 USA


ADDITIONAL RIGHTS
-----------------

Conditional upon your continuing compliance with the GNU General Public
License described above, Xara Group Ltd grants to you certain additional
rights. 

The additional rights are to use, modify, and distribute the software
together with the wxWidgets library, the wxXtra library, and the "CDraw"
library and any other such library that any version of Xara LX relased
by Xara Group Ltd requires in order to compile and execute, including
the static linking of that library to XaraLX. In the case of the
"CDraw" library, you may satisfy obligation under the GNU General Public
License to provide source code by providing a binary copy of the library
concerned and a copy of the license accompanying it.

Nothing in this section restricts any of the rights you have under
the GNU General Public License.


SCOPE OF LICENSE
----------------

This license applies to this program (XaraLX) and its constituent source
files only, and does not necessarily apply to other Xara products which may
in part share the same code base, and are subject to their own licensing
terms.

This license does not apply to files in the wxXtra directory, which
are built into a separate library, and are subject to the wxWindows
license contained within that directory in the file "WXXTRA-LICENSE".

This license does not apply to the binary libraries (if any) within
the "libs" directory, which are subject to a separate license contained
within that directory in the file "LIBS-LICENSE".


ARRANGEMENTS FOR CONTRIBUTION OF MODIFICATIONS
----------------------------------------------

Subject to the terms of the GNU Public License (see above), you are
free to do whatever you like with your modifications. However, you may
(at your option) wish contribute them to Xara's source tree. You can
find details of how to do this at:
  http://www.xaraxtreme.org/developers/

Prior to contributing your modifications, you will need to complete our
contributor agreement. This can be found at:
  http://www.xaraxtreme.org/developers/contribute/

Please note that Xara will not accept modifications which modify any of
the text between the start and end of this header (marked
XARAHEADERSTART and XARAHEADEREND).


MARKS
-----

Xara, Xara LX, Xara X, Xara X/Xtreme, Xara Xtreme, the Xtreme and Xara
designs are registered or unregistered trademarks, design-marks, and/or
service marks of Xara Group Ltd. All rights in these marks are reserved.


      Xara Group Ltd, Gaddesden Place, Hemel Hempstead, HP2 6EX, UK.
                        http://www.xara.com/

=================================XARAHEADEREND============================
 */
// NodeSimpleShape class. This provides a base class for all simple primitve shapes
// eg, Rectangles and Ellipses.

#ifndef INC_NODESHAP
#define INC_NODESHAP

//#include "ink.h" - in camtypes.h [AUTOMATICALLY REMOVED]
//#include "tranform.h" - in camtypes.h [AUTOMATICALLY REMOVED]
//#include "paths.h" - in camtypes.h [AUTOMATICALLY REMOVED]
//#include "doccoord.h" - in camtypes.h [AUTOMATICALLY REMOVED]
//#include "docrect.h" - in camtypes.h [AUTOMATICALLY REMOVED]

class RenderRegion;
class Spread;
class BecomeA;


/***********************************************************************************************

>	class NodeSimpleShape : public NodeRenderableInk

	Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
	Created:	22/9/93
	Purpose:	A NodeSimpleShape is the base class for several primitive shapes that are
				bounded by a Parallelogram. This class contains 2 Pure Virtual functions that
				need to be re-written in base classes. These should be the only functions that
				need re-writting in most cases. They build a path for the shape given a
				bounding parallelogram.

***********************************************************************************************/

class NodeSimpleShape : public NodeRenderableInk
{
// Give my name out in memory dumps
CC_DECLARE_DYNCREATE(NodeSimpleShape);

public:
	NodeSimpleShape(); 
	
	NodeSimpleShape(Node* ContextNode,  
				AttachNodeDirection Direction,  
				BOOL Locked = FALSE, 
				BOOL Mangled = FALSE,  
				BOOL Marked = FALSE, 
				BOOL Selected = FALSE    
			  );

	// Initialise the path
	BOOL SetUpPath(INT32 RequiredSize=12, INT32 BlockSize=12);
	virtual Node* SimpleCopy();
	void CopyNodeContents(NodeSimpleShape* NodeCopy);
	virtual void PolyCopyNodeContents(NodeRenderable* pNodeCopy);

	// Functions to re-build the path from the parallelogram
	virtual void CreateShape(DocRect NewRect);
	virtual void UpdateShape();

	// virtual Functions to deal with bounding boxes
	virtual DocRect	GetBoundingRect(BOOL DontUseAttrs=FALSE, BOOL HitTest=FALSE);
	virtual DocRect GetBlobBoundingRect();

	// Rendering Functions
	virtual void Render( RenderRegion* pRender );
	virtual void RenderEorDrag( RenderRegion* );

	// Selection functions.
	virtual void RenderObjectBlobs(RenderRegion* pRender);
	virtual void RenderTinyBlobs(RenderRegion* pRender);

	// Functions to transform the shape
	virtual void Transform( TransformBase& );

	// Information functions
	virtual UINT32 GetNodeSize() const; 			// Returns size of node in bytes
 
	// Make Shapes functions
	virtual BOOL CanBecomeA(BecomeA* pBecomeA);
	virtual BOOL DoBecomeA(BecomeA* pBecomeA);

	// Debugging function
	#ifdef _DEBUG
		void	ShowDebugTreeDetails() const;
	#endif
	void GetDebugDetails( StringBase* Str );

	// Functions to deal with messages
	virtual BOOL OnClick( DocCoord, ClickType, ClickModifiers, Spread* );

	// Function to decide if a coord is close to any of the control points
	BOOL IsNearControlHandle(DocCoord Coord, INT32* CoordNum);
 
	// Decides what to do about a drag on a blob
	virtual void HandleBlobDrag( DocCoord&, Spread*, INT32 );

	// The natty Snap functions
	virtual BOOL Snap(DocCoord* pDocCoord);
	virtual BOOL Snap(DocRect* pDocRect,const DocCoord& PrevCoord,const DocCoord& CurCoord);
	virtual BOOL SnapToCoords(DocCoord* pDocCoord);

	virtual double GetRotationAngle();

	// Karim MacDonald 25/11/1999
	// Extend functions.
	// Note that if a sub-class returns TRUE from IsTypeExtendible, it must
	// also provide complete implementations of ValidateExtend and Extend,
	// which should usually check/extend its children.
public:
	virtual BOOL IsTypeExtendible() const { return TRUE; }
	virtual DocRect ValidateExtend(const ExtendParams& ExtParams);
	virtual void Extend(const ExtendParams& ExtParams);

	// Data vars
public:
	Path	 InkPath;			// The path that contains the shape
	DocCoord Parallel[4];		// 4 coords to represent the parallelogram that the shape fits in
}; 




#endif	// INC_NODESHAP




