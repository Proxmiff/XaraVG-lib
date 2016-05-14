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


#ifndef INC_HTMLFILTERS
#define INC_HTMLFILTERS


// include
#include "textfltr.h"
//#include "pathname.h" - in camtypes.h [AUTOMATICALLY REMOVED]
//#include "selop.h" - in camtypes.h [AUTOMATICALLY REMOVED]
//#include "ccfile.h" - in camtypes.h [AUTOMATICALLY REMOVED]
#include "htmllist.h"


/********************************************************************************************
>	class HTMLFilter : public BaseTextFilter

	Author:		Olivier_Gascoin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	23/03/97
	Purpose:	A filter for html file, recognise only tags containing bitmaps
	SeeAlso:	BaseTextFilter
********************************************************************************************/

class HTMLFilter : public BaseTextFilter
{
	CC_DECLARE_DYNAMIC(HTMLFilter)

public:
	HTMLFilter();
	~HTMLFilter();

	INT32 Init();
	// Initialise some member variables
	//BOOL Init1();
	// free memory used by member variables
	//BOOL DeInit1();

	// Function called before HowCompatible so that filters have a chance to do
	// something, if required. Baseclass version is present but does nothing.
	// Therefore, only needs to be overridden by a filter or set of filters which requires it.
	virtual BOOL PreHowCompatible();

	// HowCompatible() returns value in range 0 to 10:
	//   0 => "Definitely Not"
	//  10 => "Yes Sir, that's my file format"
	virtual INT32 HowCompatible(	PathName& Filename, 
								ADDR  HeaderStart, 
								UINT32 HeaderSize,
								UINT32 FileSize );

	virtual BOOL DoImport(SelOperation*, CCLexFile*, Document*, 
						  BOOL AutoChosen = FALSE, ImportPosition *Pos = NULL,
						  KernelBitmap** ppImportedBitmap = NULL,
						  DocCoord* pPosTranslate = NULL, String_256* URL=NULL);

	//BOOL ScanListAndImport(HTMLFileList* HTMLList, CCLexFile* pFile, SelOperation *Op, Document *DestDoc);
	BOOL ScanListAndImport(HTMLFileList* HTMLList, String_256* Address, Document *DestDoc, SelOperation *Op, PathName pthImportFile);
	static BOOL DoImportBitmap(HTMLTAG_TYPE TagId, SelOperation *Op, Document *DestDoc, 
								PathName Path, INT32 FileNum, INT32 FileNumber, 
								DocCoord& PosTranslate, BOOL IsCentre, String_256* NewName);

	static BOOL ImportBitmap(SelOperation *Op, PathName &Path,
							KernelBitmap** ppImportedBitmap,
							DocCoord* pPosTranslate, INT32 FileNum, 
							INT32 FileNumber, String_256* NewName);

	BOOL ParseHTMLFile(CCLexFile* pFile, HTMLFileList* HTMLList);
	BOOL GetTag(TCHAR*, TCHAR*, const TCHAR* TokenBuffer);
	BOOL GetAttribute(const TCHAR* AttrName, const TCHAR*, TCHAR*);
	TCHAR* SearchStr(const char* A, const char* B);

	virtual BOOL IsDefaultDocRequired(const TCHAR* pcszPathName) { return TRUE; }

	// unused
	virtual BOOL DoExport(Operation*, CCLexFile*, PathName*, Document*, BOOL) { return TRUE; }

public:
	//This variable is set to TRUE when we import a bitmap as part of an HTML document
	static BOOL ImportingHTML;
	
protected:
	//Graham 5/6/97: Calculates the Nth position at which to import a bitmap
	DocCoord GetImportPosition(Document* pdocTarget, INT32 iPosNum);

protected:
	String_256 m_WarningsString;
};


#endif	//INC_HTMLFILTERS