

#include "camtypes.h"

/**************************************************************************************
>	static BOOL StringVar::Init()

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	31st January 2006
	Inputs:-
	Outputs:	TRUE if this class initialises OK
				FALSE otherwise
	Purpose:	Test StringVar functionality in debug builds.
	Scope:		Public

***************************************************************************************/

BOOL StringVar::Init()
{
#if defined(_DEBUG) && 0
	StringVar t1(_T("Test string\n"));
	StringVar t2(_T("0123456789012345678901234567890123456789")	// 8*40 = 320 chars
				_T("0123456789012345678901234567890123456789")
				_T("0123456789012345678901234567890123456789")
				_T("0123456789012345678901234567890123456789")
				_T("0123456789012345678901234567890123456789")
				_T("0123456789012345678901234567890123456789")
				_T("0123456789012345678901234567890123456789")
				_T("0123456789012345678901234567890123456789")
				);
	String_32 st32(_T(" Woohoo!\n"));

	StringVar t3;
	t3 = st32;
	t3 += t1;
	t3 += t2;
	TRACEUSER("Phil", _T("StringVar Test 1 = %s"), (TCHAR*)t3);

	StringVar t4(st32);
	StringVar t5(_R(IDS_MANY));
	t4 += TCHAR('-') + t5;		// Implicit cast?
	TRACEUSER("Phil", _T("StringVar Test 2 = %s\n"), (TCHAR*)t4);
#endif

	return TRUE;
}



/**************************************************************************************
>	StringVar::StringVar(const TCHAR* psz = 0)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	30th January 2006
	Inputs:		An optional pointer to a (constant) Unicode character array - if
				omitted then the String becomes empty (NOT the null pointer).
	Purpose:	Constructs a string of the appropriate maximum length.
	Errors:		ASSERTion failure if the passed character array is too long for
				this length of string.
	Scope:		Public
***************************************************************************************/

StringVar::StringVar(const TCHAR* psz)
{
	text = NULL;
	length = 0;

	UINT32 newlen = camStrlen(psz);
	EnsureAlloc(newlen);
	SafeCopy(psz, newlen);
}



/**************************************************************************************
>	StringVar::StringVar(const StringBase& other)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	30th January 2006
	Inputs:		An optional pointer to a (constant) Unicode character array - if
				omitted then the String becomes empty (NOT the null pointer).
	Purpose:	Constructs a string of the appropriate maximum length.
	Errors:		ASSERTion failure if the passed character array is too long for
				this length of string, or if psz is NULL.
	Scope:		Public
***************************************************************************************/

StringVar::StringVar(const StringBase& other)
{
	text = NULL;
	length = 0;

	UINT32 newlen = other.Length();
	EnsureAlloc(newlen);
	SafeCopy((const TCHAR*)other, newlen);
}



/**************************************************************************************
>	StringVar::StringVar(UINT32 resID, UINT32 hinst = 0)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	30th January 2006
	Inputs:		An optional pointer to a (constant) Unicode character array - if
				omitted then the String becomes empty (NOT the null pointer).
	Purpose:	Constructs a string of the appropriate maximum length.
	Errors:		ASSERTion failure if the passed character array is too long for
				this length of string, or if psz is NULL.
	Scope:		Public
***************************************************************************************/

StringVar::StringVar(UINT32 resID, UINT32 hinst)
{
	text = NULL;
	length = 0;

	const TCHAR* pResText = CamResource::GetText(resID);
	UINT32 len = camStrlen(pResText);

	EnsureAlloc(len);
	SafeCopy(pResText, len);
}



/**************************************************************************************
>	StringVar::StringVar(const wxString& wxs)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	30th January 2006
	Inputs:		An optional pointer to a (constant) Unicode character array - if
				omitted then the String becomes empty (NOT the null pointer).
	Purpose:	Constructs a string of the appropriate maximum length.
	Errors:		ASSERTion failure if the passed character array is too long for
				this length of string, or if psz is NULL.
	Scope:		Public
***************************************************************************************/

StringVar::StringVar(const wxString& wxs)
{
	text = NULL;
	length = 0;

	UINT32 newlen = wxs.Len();
	EnsureAlloc(newlen);
	SafeCopy((const TCHAR*)wxs, newlen);
}



/**************************************************************************************
>	StringVar& StringVar::operator+=(const StringBase& rhs)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	31st January 2006
	Inputs:		The string to add to the end of this string.
	Returns:	A reference to the lhs, ie. this StringVar as an lvalue.
	Purpose:	Append string y onto the end of string x, eg.
					x = TEXT("hello");
					cout << (x += TEXT(" world!")) << endl;
***************************************************************************************/
inline StringVar& StringVar::operator+=(const StringBase& rhs)
{
	ERROR3IF(this==NULL, "StringVar this pointer is NULL!");
	ERROR3IF(text==NULL || (const TCHAR*)rhs==NULL, "Call to StringVar::operator+= for an unALLOCated String");

	if (text && (const TCHAR*)rhs)
		SafeCat((const TCHAR*)rhs);

	return *this;
}



/**************************************************************************************
>	StringVar& StringVar::operator+=(const TCHAR* s)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	31st January 2006
	Inputs:		The C-style character "string" to add to the end of this String.
	Returns:	A reference to the lhs, ie. this StringBase as an lvalue.
	Purpose:	Append string y onto the end of string x, eg.
					x = TEXT("hello");
					cout << (x += TEXT(" world!")) << endl;
***************************************************************************************/
inline StringVar& StringVar::operator+=(const TCHAR* s)
{
	ERROR3IF(this==NULL, "StringVar this pointer is NULL!");
	ERROR3IF(text==NULL || s==NULL, "Call to String::operator+= for an unALLOCated String");

	if (text && s)
		SafeCat(s);

	return *this;
}



/**************************************************************************************
>	StringVar& StringVar::operator+=(const TCHAR ch)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	31st January 2006
	Inputs:		A character to add to the end of this String.
	Returns:	A reference to the lhs, ie. this StringVar as an lvalue.
	Purpose:	Appends this character to the end of this string
***************************************************************************************/
StringVar& StringVar::operator+=(const TCHAR ch)
{
	ERROR3IF(this==NULL, "StringVar this pointer is NULL!");
	ERROR3IF(text==NULL, "Call to String::operator+= for an unALLOCated String");

	if (text && ch!=0)
	{
		TCHAR tch[2];
		tch[0] = ch;
		tch[1] = 0;
		SafeCat(tch);
	}	

	return *this;
}



/**************************************************************************************
>	StringVar& StringVar::operator=(const StringBase& rhs)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	1st February 2006
	Inputs:		A reference to a constant String.
	Returns:	A reference to this.
	Purpose:	Assignment (set equal to) operator, eg.
					StringBase x("rhubarb");
					x = "custard";
***************************************************************************************/
StringVar& StringVar::operator=(const StringBase& other)
{
	ERROR3IF(this==NULL, "StringVar this pointer is NULL!");
	ERROR3IF(text==NULL || (const TCHAR*)other==NULL, "Call to String::operator= for an unALLOCated String");

	if (text && (const TCHAR*)other)
	{
		UINT32 newlen = other.Length();
		EnsureAlloc(newlen);
		SafeCopy((const TCHAR*)other, newlen);
	}
	return *this;
}


	
/**************************************************************************************
>   StringVar& StringVar::operator=(const TCHAR* pcsz)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	1st February 2006
	Inputs:		A pointer to a constant C-style (possibly wide-character) string.
	Returns:	A reference to this.
	Purpose:	Assignment (set equal to) operator, eg.
					StringBase x("rhubarb");
					x = "custard";
***************************************************************************************/
StringVar& StringVar::operator=(const TCHAR* s)
{
	ERROR3IF(this==NULL, "StringVar this pointer is NULL!");
	ERROR3IF(text==NULL || s==NULL, "Call to String::operator= for an unALLOCated String or 0 char*");

	if (text && s)
	{
		UINT32 newlen = camStrlen(s);
		EnsureAlloc(newlen);
		SafeCopy(s, newlen);
	}

	return *this;
}

	
	
/**************************************************************************************
>   StringVar& StringVar::operator=(const TCHAR ch)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	1st February 2006
	Inputs:		ch - a single character
	Returns:	A reference to this.
	Purpose:	Assigns the single character to this string
***************************************************************************************/
inline StringVar& StringVar::operator=(const TCHAR ch)
{
	ERROR3IF(this==NULL, "StringVar this pointer is NULL!");
	ERROR3IF(text==NULL, "Call to String::operator= for an unALLOCated String");

	EnsureAlloc(1);
	if (text && length>=2)		// length includes term
	{
		text[0] = ch;
		text[1] = 0;
	}

	return *this;
}




/**************************************************************************************

>	void StringVar::SafeCat(const TCHAR* string)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	31st January 2006

	Inputs:		string		0-terminated source string
	Outputs:	-
	Returns:	-
	Purpose:	Appends string to the end of this
	SeeAlso:	StringBase::Alloc, StringVar::Alloc

***************************************************************************************/

void StringVar::SafeCat(const TCHAR* string)
{
	ERROR3IF(this==NULL || string==NULL, "StringBase::SafeCat given NULL params");
	if (string==NULL)
		return;

	// Are we going to overflow ?
	UINT32 newlen = camStrlen(text) + camStrlen(string);
	EnsureAlloc(newlen);

	// Now we can concatenate the specified string, if we allocated the buffer correcetly
	if (text && length>0 && (length-1) >= newlen)
		camStrcat(text, string);
}
	

	
/**************************************************************************************

>	void StringVar::SafeCopy(const TCHAR* string, UINT32 maxlen=0)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	1st February 2006

	Inputs:		string		0-terminated source string
	Outputs:	-
	Returns:	-
	Purpose:	Assigns string to this carefully avoiding buffer overflow

***************************************************************************************/

void StringVar::SafeCopy(const TCHAR* string, UINT32 maxlen)
{
	ERROR3IF(this==NULL || string==NULL, "StringBase::SafeCopy given NULL params");

	if (text!=NULL && length>0 && string!=NULL)
	{
		if (maxlen==0)
			maxlen = camStrlen(string);

		maxlen = ((length-1)<maxlen) ? length-1 : maxlen;		// length includes term

		camStrncpy(text, string, maxlen+1);		// Copy the string AND the terminating 0 character
	}
}
	

	
/**************************************************************************************

>	void StringVar::EnsureAlloc(UINT32 newlen)

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	31st January 2006

	Inputs:		string		0-terminated destination string
	Outputs:	-
	Returns:	-
	Purpose:	Appends string to the end of this
	SeeAlso:	StringBase::Alloc, StringVar::Alloc

***************************************************************************************/

void StringVar::EnsureAlloc(UINT32 newlen)
{
	// Note: length includes zero termination character
	if (length==0 || newlen > (length-1))		// length includes term
	{
		// We need to reallocate
		TCHAR* pOldText = text;
		text = NULL;

		// Allocate room for the new string plus extra room to avoid repeated reallocation
		// And round to a 64 char boundary so that allocations slot together more efficiently.
		// And ensure that we always allocate >0
		// (Remember Alloc adds 1 character for the terminator so we subtract 1 from newlen)
		newlen = (newlen/64 + 1)*64 - 1;
		Alloc(newlen);

		if (text!=NULL && pOldText!=NULL)
			camStrcpy(text, pOldText);
	}
}


#if !defined(EXCLUDE_FROM_XARLIB)
/**************************************************************************************
>	void StringVar::MakePercent(INT32 Value);
	void StringVar::MakePercent(double Value);
	void StringVar::MakePercent(TCHAR* Value);

	Author:		Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
	Created:	31st January 2006
	Inputs:		Takes a number / string
	Purpose:	Set the string equal to the given argument + a % character, or however
				the current locale wants it
	Example:	String_32 Str;
				Str.MakePercent(23);
				For UK, Str now equals "23%"
**************************************************************************************/

void StringVar::MakePercent(INT32 Value)
{	
	EnsureAlloc(32);
	StringBase::MakePercent(Value);
}

void StringVar::MakePercent(double Value)
{
	EnsureAlloc(32);
	StringBase::MakePercent(Value);
}

void StringVar::MakePercent(TCHAR* Value)
{
	EnsureAlloc(32);
	StringBase::MakePercent(Value);
}
#endif

