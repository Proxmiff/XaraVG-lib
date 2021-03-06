
//
// Fixed-point primitive routines
//


#include "camtypes.h"


/********************************************************************************************

>	FIXED16 Fixed16Mul(FIXED16 arg1, FIXED16 arg2)

	Author: 	Andy_Pennell (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/5/93
	Inputs:		two fixed16s
	Outputs:	None
	Returns:	FIXED16 result (arg1 * arg2)
	Scope:		Only to be used in the FIXED16 class.
	Purpose:	signed multiplication
	Errors:		None (no overflow checks). Rounds

********************************************************************************************/

fixed16 Fixed16Mul(fixed16 a,fixed16 b)
{
	fixed16 result ;
//	result.all = INT32(Int32x32To64(a.all,b.all)+0x8000>>16) ;
	result.all = INT32(((xlong)a.all*b.all+0x8000) >> 16) ;
	return result ;
}


/********************************************************************************************

>	FIXED16 Fixed16Div(FIXED16 arg1, FIXED16 arg2)

	Author: 	Andy_Pennell (Xara Group Ltd) <camelotdev@xara.com>
	Created:	10/5/93
	Inputs:		two fixed16s
	Outputs:	None
	Returns:	FIXED16 result (arg1 / arg2)
	Scope:		Only to be used in the FIXED16 class.
	Purpose:	signed division
	Errors:		None (no overflow checks). No rounding either (should it?)
			Overflow will cause a DIV0 exception, as will divide by zero

********************************************************************************************/

fixed16 Fixed16Div(fixed16 a,fixed16 b)
{
	fixed16 result ;
//	result.all = MulDiv(a.all,1<<16,b.all) ;
	result.all = INT32(((xlong)a.all<<16)/b.all) ;
	return result ;
}





/********************************************************************************************

>	NOTREALLYAFIXED16 InternalDiv32by32( INT32 arg1, INT32 arg2 )

	Author: 	Andy_Pennell (Xara Group Ltd) <camelotdev@xara.com>
	Created:	11/5/93
	Inputs:		two signed 32-bit numbers
	Outputs:	None
	Returns:	FIXED16 result of arg1/arg2
	Scope:		Private to the CCMATHS library.
	Purpose:	Obtain a fixed-point representation of the ratio between two signed
			numbers.
	Errors:		None. Div0 and overflow will create a div0 exception.

Awooga! Awooga! Do not call this function directly - it returns a INT32 and is meant to be
used only in the C++ wrapper (i.e. it returns the value << 16)

This is done by multiplying arg1 by 65536, then doing a 64-bit/32-bit division

Curiously enough, this function is 100% the same as Fixed16Div.  When you think about it,
its not really that curious as both args are 65536 times bigger than the FIXED16 versions,
and x/y is identical to x*65536/y*65536.

********************************************************************************************/

fixed16 InternalDiv32by32( INT32 a, INT32 b )
{
	fixed16 result ;
//	result.all = MulDiv(a.all,1<<16,b.all) ;
	result.all = INT32( ( (xlong)a << 16 ) / b );
	return result ;
}



/********************************************************************************************

>	double Fixed16ToDouble( FIXED16 arg )

	Author: 	Andy_Pennell (Xara Group Ltd) <camelotdev@xara.com>
	Created:	13/5/93
	Inputs:		FIXED16 argument
	Outputs:	None
	Returns:	double-precision value
	Scope:		Only to be used in the FIXED16 class.
	Purpose:	General type conversion
	Errors:		None

This code relies on the compiler efficiently replacing the division by a multiplication
of the reciprical.

********************************************************************************************/

double Fixed16ToDouble( fixed16 a )
{
	return (double)a.all/(1<<16) ;
}


/********************************************************************************************

>	fixed16 DoubleToFixed16( double arg )

	Author: 	Andy_Pennell (Xara Group Ltd) <camelotdev@xara.com>
	Created:	14/5/93
	Inputs:		double-precision argument
	Outputs:	None
	Returns:	FIXED16 value
	Purpose:	General type conversion
	Errors:		None (overflow will cause exception)

This is done by taking the arg and multiplying it by 65536, then converting to integer.

********************************************************************************************/

fixed16 DoubleToFixed16( double arg )
{
	fixed16 result ;
#if 0//defined(_M_IX86)
	//
	// This avoids the slow fix operation. Add a large number so that
	// the double always has the same exponent and the mantissa becomes,
	// in affect, a fixed point value with the value we want in the
	// bottom 32 bits.
	//
	// Note that we are relying on the compiler to convert this to
	// efficient code.
	//
	// TODO: Test!
	//
	static const double fFix = (XLONG)3<<(DBL_MANT_DIG-16-2);
	double F = arg+fFix;
	result.all = (INT32&)F ;
#else
	result.all = INT32(arg*(1<<16)) ;
#endif
	return result ;
}


/********************************************************************************************

>	FIXED16 Fixed16Sin( FIXED16 arg )

	Author: 	Andy_Pennell (Xara Group Ltd) <camelotdev@xara.com>
	Created:	13/5/93
	Inputs:		FIXED16 argument in degrees
	Outputs:	None
	Returns:	FIXED16 result
	Scope:		Only to be used in the FIXED16 class.
	Purpose:	General trigonometry
	Errors:		None

********************************************************************************************/

/********************************************************************************************

>	FIXED16 Fixed16Cos( FIXED16 arg )

	Author: 	Andy_Pennell (Xara Group Ltd) <camelotdev@xara.com>
	Created:	13/5/93
	Inputs:		FIXED16 argument in degrees
	Outputs:	None
	Returns:	FIXED16 result
	Scope:		Only to be used in the FIXED16 class.
	Purpose:	General trigonometry
	Errors:		None

********************************************************************************************/
//
// If n is the input arg in degrees, then from a INT32 it is arg/65536.
// In radians, this is arg/65536*pi/180 which is arg*pi/11796480 (=approx 2.66e-7)
// The result then needs multiplying by 65536 before returning as a INT32.
//
fixed16	Fixed16Sin( fixed16 arg )
{
	return DoubleToFixed16( sin( double(2.6631610900792382460383465095346e-7) * arg.MakeDouble() ) );
}

fixed16	Fixed16Cos( fixed16 arg )
{
	return DoubleToFixed16( cos( double(2.6631610900792382460383465095346e-7) * arg.MakeDouble() ) );
}



/********************************************************************************************

>	INT32 MatrixCalc( FIXED16 fix1, INT32 l1, FIXED16 fix2, INT32 l2)

	Author: 	Andy_Pennell (Xara Group Ltd) <camelotdev@xara.com>
	Created:	14/5/93
	Inputs:		two FIXED16s, two INT32s
	Outputs:	None
	Returns:	INT32 result of fix1*l1 + fix2*l2
	Scope:		Only to be used in the Matrix classes.
	Purpose:	Matrix multiply primitive done using intermediate 64-bit results
			so accuracy is maintained. Special cases are when fix1 or fix2
			are 0.0 or 1.0 as multiplications may be avoided.
			Note that the result is rounded to the nearest integer.
	Errors:		None (overflow ignored).

********************************************************************************************/

INT32 MatrixCalc( fixed16 fix1, INT32 l1, fixed16 fix2, INT32 l2)
{
	return INT32( ( (xlong)fix1.all * l1 + (xlong)fix2.all * l2 ) >> 16 );
}



/********************************************************************************************

>	INT32 LongMulFixed16( INT32 arg, FIXED16 fix )

	Author: 	Andy_Pennell (Xara Group Ltd) <camelotdev@xara.com>
	Created:	14/5/93
	Inputs:		one FIXED16, one INT32
	Outputs:	None
	Returns:	INT32 result of fix * arg
	Scope:		Only to be used in the Matrix class.
	Purpose:	Method of multiplying a INT32 by a fixed point number keeping
			as much accuracy as possible.
	Errors:		None (overflow ignored).

********************************************************************************************/

INT32 LongMulFixed16( INT32 arg, fixed16 fix )
{
	return INT32( ((xlong)arg * fix.all + 0x8000) >> 16 );
}


/********************************************************************************************

>	INT32 LongDivFixed16( INT32 arg, FIXED16 fix )

	Author: 	Andy_Pennell (Xara Group Ltd) <camelotdev@xara.com>
	Created:	13/7/93
	Inputs:		one FIXED16, one INT32
	Outputs:	None
	Returns:	INT32 result of arg / fix
	Purpose:	Method of dividing an INT32 by a fixed point number keeping
			as much accuracy as possible.
	Errors:		None (overflow ignored). Div0 and Overflow will take exception.

********************************************************************************************/

INT32 LongDivFixed16( INT32 arg, fixed16 fix )
{
	return INT32(((xlong)arg<<16)/fix.all) ;
}



/********************************************************************************************

>	INT32 MPtoPixel( INT32 arg, FIXED16 fix )

	Author: 	Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	14/9/93
	Inputs:		one FIXED16, one INT32
	Outputs:	None
	Returns:	INT32 result of arg / fix
	Purpose:	Method of dividing a INT32 by a fixed point number keeping
				as much accuracy as possible.
				This version will Round any fractional result to the NEAREST whole value.
				It is used for scaling Millipoints to Whole Pixels.
	Assumptions:Fix is +ve.
	Errors:		None (overflow ignored). Div0 and Overflow will take exception.

********************************************************************************************/

INT32 MPtoPixel( INT32 arg, fixed16 fix )
{
	if ( arg<0 )
		return INT32((((xlong)arg<<16)-(fix.all>>1))/fix.all) ;
	else
		return INT32((((xlong)arg<<16)+(fix.all>>1))/fix.all) ;

// Gavin says (07/03/2006) the following would be more accurate so long as
// we could guarantee that fix.all<<1 does not overflow:
//	if ( arg<0 )
//		return INT32((((xlong)arg<<17)-fix.all))/(fix.all<<1)) ;
//	else
//		return INT32((((xlong)arg<<17)+fix.all))/(fix.all<<1)) ;
}


/********************************************************************************************

>	INT32 MPtoOS256( INT32 arg, FIXED16 fix )

	Author: 	Will_Cowling (Xara Group Ltd) <camelotdev@xara.com>
	Created:	14/9/93
	Inputs:		one FIXED16, one INT32
	Outputs:	None
	Returns:	INT32 result of arg / fix
	Purpose:	Method of dividing a INT32 by a fixed point number keeping
				as much accuracy as possible.
				This version will convert the MP value to 256ths of a Pixel.
	Errors:		None (overflow ignored). Div0 and Overflow will take exception.

********************************************************************************************/


INT32 MPtoOS256( INT32 arg, fixed16 fix )
{
	return INT32(((xlong)arg<<24)/fix.all) ;
}
