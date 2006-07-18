// $Id: pngfiltr.h 809 2006-04-13 11:28:42Z phil $
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
//

#ifndef INC_IMGMGKFT_H
#define INC_IMGMGKFT_H

#include "maskfilt.h"
#include "outptpng.h"
#include "bmpprefs.h"  	// SelectionType
//#include "filtrres.h"

class KernelBitmap;

/********************************************************************************************

>	class ImageMagickExportOptions : public MaskedFilterExportOptions

	Author:		Colin_Barfoot (Xara Group Ltd) <camelotdev@xara.com>
	Created:	30/10/96
	Purpose:   	Base class for storing information about the bitmap we're exporting.

********************************************************************************************/

// Need a type for the hash data or we get duplicate const error
typedef const TCHAR * IMFilterString;

// Declare the hash map from IMFilterString to UINT32
WX_DECLARE_HASH_MAP( IMFilterString, UINT32, wxStringHash, wxStringEqual, IMFilterStringToUINT32);

class ImageMagickExportOptions : public MaskedFilterExportOptions
{
CC_DECLARE_DYNCREATE(ImageMagickExportOptions)

public:
	ImageMagickExportOptions(const FilterType FilterID, const StringBase* pFilterName);
	ImageMagickExportOptions() {};

	virtual BOOL	GetSupportsImageMap() { return TRUE; }

	virtual BOOL	RetrieveDefaults();
	virtual BOOL	SetAsDefaults() const;
	
	virtual UINT32 GetFilterNameStrID() { return _R(IDS_FILTERNAME_ImageMagick);}

	virtual UINT32 * GetConfigPtr(const TCHAR * FilterName) const
	{
		if (!s_pHash)
			return NULL;
		IMFilterStringToUINT32::iterator i=s_pHash->find(FilterName);
		return (UINT32 *)((i==s_pHash->end())?NULL:&(i->second));
	}

	virtual UINT32 GetConfig() const
	{
		UINT32 * pConfig = GetConfigPtr(FilterName);
		return pConfig?(*pConfig):0;
	}

	virtual void SetConfig(UINT32 value) const
	{
		UINT32 * pConfig = GetConfigPtr(FilterName);
		if (pConfig)
			*pConfig = value;
	}

protected:
	String_256 FilterName;

	static	IMFilterStringToUINT32 * s_pHash;
};

/********************************************************************************************

>	class ImageMagickFilter : public MaskedFilter

	Author:		Neville_Humphrys (Xara Group Ltd) <camelotdev@xara.com>
	Created:	26/4/96
	Purpose:	Encapsulates a ImageMagick export filter.
				Derives off the masked filter which provides funcitonality for doing masked
				bitmap export and import.

********************************************************************************************/

class ImageMagickFilter : public MaskedFilter
{
	CC_DECLARE_DYNAMIC(ImageMagickFilter);
	
public:

	ImageMagickFilter();
	virtual ~ImageMagickFilter() {if (TempFile) delete (TempFile);}
	BOOL Init();

	virtual INT32 HowCompatible( PathName& Filename, ADDR HeaderStart, UINT32 HeaderSize,
		UINT32 FileSize );

	virtual FilterType GetFilterType() const { return IMAGEMAGICK; }

	// Import related functions
	virtual BOOL ReadFromFile(OILBitmap* pOilBitmap);
	// This is the function for native/web files to use
	virtual BOOL ReadFromFile(OILBitmap* pOilBitmap, BaseCamelotFilter* pFilter,
							CCLexFile* pFile, BOOL IsCompressed);

	// Function for web/native filters to use to write out the bitmap data to file
	virtual BOOL WriteBitmapToFile(KernelBitmap* pKernelBitmap, BaseCamelotFilter* pFilter,
								CCLexFile* pFile, INT32 Compression);

	// Check if this Bitmap filter can cope with saving at this Bpp/Colour depth
	virtual BOOL IsThisBppOk(UINT32 Bpp);

	// Public way of finding out how compatable the ImageMagick filter thought the file was
	// We might want to bring in the Accusoft filter to help out.
	virtual INT32 GetImageMagickCompatibility() { return ImageMagickHowCompatible; }

	// These get overridden in the derived classes
	virtual BOOL CanDoTransparency() { return TRUE; }
	virtual BOOL CanDoInterlace() { return FALSE; }
	virtual wxString GetTag() { return _T("mmif"); }
	virtual TCHAR * GetExtension() { return _T("mmif"); }
	virtual INT32 GetCompatibility() { return 10; }

	// Virtual overrides
	virtual UINT32 GetExportMsgID();
	virtual void PostGetExportOptions(BitmapExportOptions* pOptions);

	static BOOL CheckPath();

protected:

	// Invert the alpha channel.
	virtual void InvertAlpha ( LPBITMAPINFO	lpBitmapInfo,
							LPBYTE		lpBits );

	// Get the correct output DIB.
	virtual OutputDIB* GetOutputDIB ( void );

	virtual BitmapExportOptions*	CreateExportOptions() const;

	virtual BOOL GetExportOptions( BitmapExportOptions* pOptions );
	virtual void CleanUpAfterExport();
	virtual BOOL EndWriteToFile();

	virtual BOOL WriteFileHeader(void) { return TRUE; }
	virtual BOOL WritePreFrame(void);
	virtual BOOL WritePreSecondPass(void);
	virtual BOOL WritePostOptimisedPalette(void) { return TRUE; }
	virtual BOOL WriteFrame(void);
	virtual BOOL WritePostFrame(void);
	virtual BOOL WriteFileEnd(void);

	virtual BOOL WriteBitmapToFile(KernelBitmap* pKernelBitmap, double Dpi);

	// this is used for the actual writing of the file
	BOOL WriteDataToFile( BOOL End, UINT32 Bpp, UINT32 Compression);
	BOOL WriteToFile ( CCLexFile*, LPBITMAPINFO Info, LPBYTE Bits,
							String_64* ProgressString = NULL);

	// This is the form used for direct saving bitmaps into the native/web file format
	BOOL WriteToFile ( CCLexFile*, LPBITMAPINFO Info, LPBYTE Bits,
							BOOL Interlace, INT32 TransparentColour,
							BaseCamelotFilter* pFilter = NULL);

	void AlterPaletteContents( LPLOGPALETTE pPalette );//ap

	virtual BOOL CreateTempFile();
	virtual BOOL ConvertFromTempFile(CCLexFile * File);
	virtual BOOL ConvertToTempFile(CCLexFile * File);
	virtual BOOL TidyTempFile(BOOL Delete = TRUE);

#ifdef DO_EXPORT
	// The class we use for actually outputting the ImageMagick data and converting from 32 to n bpps
	static OutputPNG DestImageMagick;
#endif

	ResourceID FilterExtID;
	
	// This is so we can remember what we thought of the GIF file.
	INT32 ImageMagickHowCompatible;

	static FilterType s_FilterType;						// Type of filter in use i.e. ImageMagick

	// The string to display when exporting the second stage.
	UINT32 Export2ndStageMsgID;

	CCDiskFile * TempFile;
	wxString TempFileName;

	static String_256	s_ImageMagickPath;
	static BOOL s_HaveImageMagick;
	static BOOL s_HaveCheckedPath;
	static BOOL s_DoWarning;
	static BOOL s_Disable;

	// These allow us to use a single filter type
	static BOOL s_OutputTransparent;
	static BOOL s_OutputInterlaced;
};

/********************************************************************************************

>	class ImageMagickOILFilter : public OILFilter

	Author:		Alex Bligh <alex@alex.org.uk>
	Created:	18/07/2006
	Purpose:	The PNG File format Oil Filter (Extension etc)

********************************************************************************************/

class ImageMagickOILFilter : public OILFilter
{
public:
	ImageMagickOILFilter (Filter* pFilter, ResourceID FilterNameID, ResourceID FilterExtID);
};


#define DECLARE_IMAGEMAGICK_FILTER(IMType, IMCanInport, IMCanExport, IMTransparency, IMInterlace, IMTag, IMExtension, IMCompatibility)	\
class ImageMagickFilter ## IMType : public ImageMagickFilter					\
{																				\
public:																			\
	ImageMagickFilter ## IMType()												\
	{																			\
		Flags.CanImport 	= IMCanInport;										\
		Flags.CanExport 	= IMCanExport;										\
		FilterID			= IMAGEMAGICK_ ## IMType ;							\
		FilterNameID		= _R(IDS_IMAGEMAGICK_ ## IMType ## _FILTERNAME);	\
		FilterInfoID		= _R(IDS_IMAGEMAGICK_ ## IMType ## _FILTERINFO);	\
		FilterExtID			= _R(IDS_IMAGEMAGICK_ ## IMType ## _FILTEREXT);		\
		ImportMsgID			= _R(IDS_IMAGEMAGICK_ ## IMType ## _IMPORTMSG);		\
		ExportMsgID			= _R(IDS_IMAGEMAGICK_ ## IMType ## _PREPAREMSG);	\
		ExportingMsgID		= _R(IDS_IMAGEMAGICK_ ## IMType ## _EXPORTMSG);		\
		Export2ndStageMsgID = _R(IDS_IMAGEMAGICK_ ## IMType ## _MASKINGMSG);	\
	}																			\
																				\
	virtual BOOL CanDoTransparency()	{ return IMTransparency; }				\
	virtual BOOL CanDoInterlace() 		{ return IMInterlace; }					\
	virtual wxString GetTag() 			{ return IMTag; }						\
	virtual TCHAR * GetExtension() 		{ return IMExtension; }					\
	virtual INT32 GetCompatibility() 	{ return IMCompatibility; }				\
};

// Instructions on adding a new ImageMagickFilter
// ==============================================
//
// 1. Declare the filter below (let's say you give it the classname FOO)
// 2. Add "IMAGEMAGIK_FOO" to the enum in filter_types.h
// 3. Add "ADD_FILTER(ImageMagickFilterFOO)" to filters.cpp
// 4. Add appropriate resources to wxOil/xrc/EN/imagemagick-strings.xrc
//    These would be
//		IDS_IMAGEMAGICK_FOO_FILTERNAME
//		IDS_IMAGEMAGICK_FOO_FILTERINFO
//		IDS_IMAGEMAGICK_FOO_FILTEREXT
//		IDS_IMAGEMAGICK_FOO_IMPORTMSG
//		IDS_IMAGEMAGICK_FOO_PREPAREMSG
//		IDS_IMAGEMAGICK_FOO_EXPORTMSG
//		IDS_IMAGEMAGICK_FOO_MASKINGMSG
//

//						  ClassName	Import	Export	Transp	Int'lce	Tag			Extension	Compat
DECLARE_IMAGEMAGICK_FILTER(BMP,		TRUE,	TRUE,	TRUE,	FALSE,	_T("bmp"),	_T("bmp"),	10); // Microsoft Windows Bitmap
DECLARE_IMAGEMAGICK_FILTER(CUR,		TRUE,	FALSE,	TRUE,	FALSE,	_T("cur"),	_T("cur"),	10); // Microsoft Cursor Icon
DECLARE_IMAGEMAGICK_FILTER(CUT,		TRUE,	FALSE,	TRUE,	FALSE,	_T("cut"),	_T("cut"),	10); // DR Halo
DECLARE_IMAGEMAGICK_FILTER(DCM,		TRUE,	FALSE,	TRUE,	FALSE,	_T("dcm"),	_T("dcm"),	10); // DICOM Image
DECLARE_IMAGEMAGICK_FILTER(DCX,		TRUE,	TRUE,	TRUE,	FALSE,	_T("dcx"),	_T("dcx"),	10); // ZSoft Paintbrush
DECLARE_IMAGEMAGICK_FILTER(DIB,		TRUE,	TRUE,	TRUE,	FALSE,	_T("dib"),	_T("dib"),	10); // Microsoft Windows Device Independent Bitmap
DECLARE_IMAGEMAGICK_FILTER(DNG,		TRUE,	FALSE,	TRUE,	FALSE,	_T("dng"),	_T("dng"),	10); // Digital Negative
DECLARE_IMAGEMAGICK_FILTER(EPDF,	TRUE,	TRUE,	TRUE,	FALSE,	_T("epdf"),	_T("epdf"),	10); // Encapsulated Portable Document Format
DECLARE_IMAGEMAGICK_FILTER(EPI,		TRUE,	TRUE,	TRUE,	FALSE,	_T("epi"),	_T("epi"),	10); // Adobe Encapsulated PostScript Interchange format
DECLARE_IMAGEMAGICK_FILTER(EPS,		TRUE,	TRUE,	TRUE,	FALSE,	_T("eps"),	_T("eps"),	10); // Adobe Encapsulated PostScript
DECLARE_IMAGEMAGICK_FILTER(EPS2,	FALSE,	TRUE,	TRUE,	FALSE,	_T("eps2"),	_T("eps2"),	10); // Adobe Level II Encapsulated PostScript
DECLARE_IMAGEMAGICK_FILTER(EPS3,	FALSE,	TRUE,	TRUE,	FALSE,	_T("eps3"),	_T("eps3"),	10); // Adobe Level III Encapsulated PostScript
DECLARE_IMAGEMAGICK_FILTER(EPSF,	TRUE,	TRUE,	TRUE,	FALSE,	_T("epsf"),	_T("epsf"),	10); // Adobe Encapsulated PostScript
DECLARE_IMAGEMAGICK_FILTER(EPSI,	TRUE,	TRUE,	TRUE,	FALSE,	_T("epsi"),	_T("epsi"),	10); // Adobe Encapsulated PostScript Interchange format
DECLARE_IMAGEMAGICK_FILTER(EPT,		TRUE,	TRUE,	TRUE,	FALSE,	_T("ept"),	_T("ept"),	10); // Adobe Encapsulated PostScript Interchange format with TIFF preview
DECLARE_IMAGEMAGICK_FILTER(FAX,		TRUE,	TRUE,	TRUE,	FALSE,	_T("fax"),	_T("fax"),	10); // Group 3 TIFF
DECLARE_IMAGEMAGICK_FILTER(FITS,	TRUE,	TRUE,	TRUE,	FALSE,	_T("fits"),	_T("fits"),	10); // Flexible Image Transport System
DECLARE_IMAGEMAGICK_FILTER(ICO,		TRUE,	FALSE,	TRUE,	FALSE,	_T("ico"),	_T("ico"),	10); // Microsoft Icon
DECLARE_IMAGEMAGICK_FILTER(JNG,		TRUE,	TRUE,	TRUE,	FALSE,	_T("jng"),	_T("jng"),	10); // Multiple-image Network Graphics
DECLARE_IMAGEMAGICK_FILTER(MIFF,	TRUE,	TRUE,	TRUE,	FALSE,	_T("miff"),	_T("miff"),	10); // Magick image file format
DECLARE_IMAGEMAGICK_FILTER(MPC,		TRUE,	TRUE,	TRUE,	FALSE,	_T("mpc"),	_T("mpc"),	10); // Magick Persistent Cache image file format
DECLARE_IMAGEMAGICK_FILTER(OTB,		TRUE,	TRUE,	TRUE,	FALSE,	_T("otb"),	_T("otb"),	10); // On-the-air Bitmap
DECLARE_IMAGEMAGICK_FILTER(P7,		TRUE,	TRUE,	TRUE,	FALSE,	_T("p7"),	_T("p7"),	10); // Xv's Visual Schnauzer thumbnail format
DECLARE_IMAGEMAGICK_FILTER(PALM,	TRUE,	TRUE,	TRUE,	FALSE,	_T("palm"),	_T("palm"),	10); // Palm pixmap
DECLARE_IMAGEMAGICK_FILTER(PAM,		FALSE,	TRUE,	TRUE,	FALSE,	_T("pam"),	_T("pam"),	10); // Common 2-dimensional bitmap format
DECLARE_IMAGEMAGICK_FILTER(PBM,		TRUE,	TRUE,	TRUE,	FALSE,	_T("pbm"),	_T("pbm"),	10); // Portable bitmap format (black and white)
DECLARE_IMAGEMAGICK_FILTER(PCD,		TRUE,	TRUE,	TRUE,	FALSE,	_T("pcd"),	_T("pcd"),	10); // Photo CD
DECLARE_IMAGEMAGICK_FILTER(PCDS,	TRUE,	TRUE,	TRUE,	FALSE,	_T("pcds"),	_T("pcds"),	10); // Photo CD
DECLARE_IMAGEMAGICK_FILTER(PCL,		FALSE,	TRUE,	TRUE,	FALSE,	_T("pcl"),	_T("pcl"),	10); // HP Page Control Language
DECLARE_IMAGEMAGICK_FILTER(PCX,		TRUE,	TRUE,	TRUE,	FALSE,	_T("pcx"),	_T("pcx"),	10); // ZSoft IBM PC Paintbrush file
DECLARE_IMAGEMAGICK_FILTER(PDB,		TRUE,	TRUE,	TRUE,	FALSE,	_T("pdb"),	_T("pdb"),	10); // Palm Database ImageViewer Format
DECLARE_IMAGEMAGICK_FILTER(PDF,		TRUE,	TRUE,	TRUE,	FALSE,	_T("pdf"),	_T("pdf"),	10); // Portable Document Format
DECLARE_IMAGEMAGICK_FILTER(PGM,		TRUE,	TRUE,	TRUE,	FALSE,	_T("pgm"),	_T("pgm"),	10); // Portable graymap format (gray scale)
DECLARE_IMAGEMAGICK_FILTER(PICT,	TRUE,	TRUE,	TRUE,	FALSE,	_T("pict"),	_T("pict"),	10); // Apple Macintosh QuickDraw/PICT file
DECLARE_IMAGEMAGICK_FILTER(PIX,		TRUE,	FALSE,	TRUE,	FALSE,	_T("pix"),	_T("pix"),	10); // Alias/Wavefront RLE image format
DECLARE_IMAGEMAGICK_FILTER(PNM,		TRUE,	TRUE,	TRUE,	FALSE,	_T("pnm"),	_T("pnm"),	10); // Portable anymap
DECLARE_IMAGEMAGICK_FILTER(PPM,		TRUE,	TRUE,	TRUE,	FALSE,	_T("ppm"),	_T("ppm"),	10); // Portable pixmap format (color)
DECLARE_IMAGEMAGICK_FILTER(PS,		TRUE,	TRUE,	TRUE,	FALSE,	_T("ps"),	_T("ps"),	10); // Adobe PostScript file
DECLARE_IMAGEMAGICK_FILTER(PS2,		TRUE,	TRUE,	TRUE,	FALSE,	_T("ps2"),	_T("ps2"),	10); // Adobe Level II PostScript file
DECLARE_IMAGEMAGICK_FILTER(PS3,		TRUE,	TRUE,	TRUE,	FALSE,	_T("ps3"),	_T("ps3"),	10); // Adobe Level III PostScript file
DECLARE_IMAGEMAGICK_FILTER(PSD,		TRUE,	TRUE,	TRUE,	FALSE,	_T("psd"),	_T("psd"),	10); // Adobe Photoshop bitmap file
DECLARE_IMAGEMAGICK_FILTER(PTIF,	TRUE,	TRUE,	TRUE,	FALSE,	_T("ptif"),	_T("ptif"),	10); // Pyramid encoded TIFF
DECLARE_IMAGEMAGICK_FILTER(PWP,		TRUE,	FALSE,	TRUE,	FALSE,	_T("pwp"),	_T("pwp"),	10); // Seattle File Works multi-image file
DECLARE_IMAGEMAGICK_FILTER(RLA,		TRUE,	FALSE,	TRUE,	FALSE,	_T("rla"),	_T("rla"),	10); // Alias/Wavefront image file
DECLARE_IMAGEMAGICK_FILTER(RLE,		TRUE,	FALSE,	TRUE,	FALSE,	_T("rle"),	_T("rle"),	10); // Utah Run length encoded image file
DECLARE_IMAGEMAGICK_FILTER(SCT,		TRUE,	FALSE,	TRUE,	FALSE,	_T("sct"),	_T("sct"),	10); // Scitex Continuous Tone Picture
DECLARE_IMAGEMAGICK_FILTER(SFW,		TRUE,	FALSE,	TRUE,	FALSE,	_T("sfw"),	_T("sfw"),	10); // Seattle File Works image
DECLARE_IMAGEMAGICK_FILTER(SUN,		TRUE,	TRUE,	TRUE,	FALSE,	_T("sun"),	_T("sun"),	10); // SUN Rasterfile
DECLARE_IMAGEMAGICK_FILTER(SVG,		TRUE,	TRUE,	TRUE,	FALSE,	_T("svg"),	_T("svg"),	10); // Scalable Vector Graphics
DECLARE_IMAGEMAGICK_FILTER(TGA,		TRUE,	TRUE,	TRUE,	FALSE,	_T("tga"),	_T("tga"),	10); // Truevision Targa image
DECLARE_IMAGEMAGICK_FILTER(TIFF,	TRUE,	TRUE,	TRUE,	FALSE,	_T("tiff"),	_T("tiff"),	10); // Tagged Image File Format
DECLARE_IMAGEMAGICK_FILTER(TIM,		TRUE,	FALSE,	TRUE,	FALSE,	_T("tim"),	_T("tim"),	10); // PSX TIM file
DECLARE_IMAGEMAGICK_FILTER(TTF,		TRUE,	FALSE,	TRUE,	FALSE,	_T("ttf"),	_T("ttf"),	10); // TrueType font file
DECLARE_IMAGEMAGICK_FILTER(VICAR,	TRUE,	TRUE,	TRUE,	FALSE,	_T("vicar"),_T("vicar"),10); // VICAR rasterfile format
DECLARE_IMAGEMAGICK_FILTER(VIFF,	TRUE,	TRUE,	TRUE,	FALSE,	_T("viff"),	_T("viff"),	10); // Khoros Visualization Image File Format
DECLARE_IMAGEMAGICK_FILTER(WBMP,	TRUE,	TRUE,	TRUE,	FALSE,	_T("wbmp"),	_T("wbmp"),	10); // Wireless bitmap
DECLARE_IMAGEMAGICK_FILTER(WPG,		TRUE,	FALSE,	TRUE,	FALSE,	_T("wpg"),	_T("wpg"),	10); // Word Perfect Graphics File
DECLARE_IMAGEMAGICK_FILTER(XBM,		TRUE,	TRUE,	TRUE,	FALSE,	_T("xbm"),	_T("xbm"),	10); // X Windows system bitmap, black and white only
DECLARE_IMAGEMAGICK_FILTER(XCF,		TRUE,	FALSE,	TRUE,	FALSE,	_T("xcf"),	_T("xcf"),	10); // GIMP image
DECLARE_IMAGEMAGICK_FILTER(XPM,		TRUE,	TRUE,	TRUE,	FALSE,	_T("xpm"),	_T("xpm"),	10); // X Windows system pixmap
DECLARE_IMAGEMAGICK_FILTER(XWD,		TRUE,	TRUE,	TRUE,	FALSE,	_T("xwd"),	_T("xwd"),	10); // X Windows system window dump

#endif // INC_ImageMagickFILTR


