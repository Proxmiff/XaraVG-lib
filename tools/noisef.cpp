

#include "camtypes.h"
#include "noisef.h"
#include "vector3d.h"
#include "bitmpinf.h"
#include "macros.h"
#include "noisebas.h"

CC_IMPLEMENT_DYNCREATE(NoiseFractalFill,CCObject);

/********************************************************************************************

	NoiseFractalFill::NoiseFractalFill()
	
	Author:	   Mike_Kenny (Xara Group Ltd) <camelotdev@xara.com>
	Created:   16/01/97
	Purpose:   A noise based fill. Really simply defined as the projection of a 3d noise
			   function onto a surface.
			   For documentation, see Docs\Specs\Textures\Phase1 Spec v1,2 and 3 .doc

********************************************************************************************/

NoiseFractalFill::NoiseFractalFill()
{
}


/********************************************************************************************

>	BOOL NoiseFractalFill::DoFill(double scale, 
								  double tuber, 
								  double sweepr, 
								  KernelBitmap *pBitmap) const
	
	Author:	   Mike_Kenny (Xara Group Ltd) <camelotdev@xara.com>
	Created:   16/01/97
	Purpose:   A noise based fill. Really simply defined as the projection of a 3d noise
			   function on a torus of given swept radius and tube radius.

********************************************************************************************/
/*
BOOL NoiseFractalFill::DoFill(double scale, 
							  double tuber, 
							  double sweepr, 
							  KernelBitmap *pBitmap) const
{
	// go get the noise manager
	NoiseMan* pNoiseMan = GetApplication()->GetNoiseManager();
	if (pNoiseMan==NULL)
		return FALSE;

	BitmapInfo Info;
	pBitmap->ActualBitmap->GetInfo(&Info);

	INT32 Width  = Info.PixelWidth;
	INT32 Height = Info.PixelHeight;
	
	double n;
	INT32 i,j;
	BYTE cn;
	Vector3D p;

	double theta,phi,sin_phi,cos_phi;

	double vstep = XS_2PI/Height;
	double ustep = XS_2PI/Width;
	double ls = 1.0/scale;

	for (j=0; j<Height; j++)
	{
		phi = j*vstep;
		
		sin_phi = sin(phi);
		cos_phi = cos(phi);

		for (i=0; i<Width; i++)
		{
			theta = i*ustep;

			p.x = sweepr + tuber*cos(theta);
			p.y =		   tuber*sin(theta);

			p.z = p.x*sin_phi;
			p.x = p.x*cos_phi;

			p.x *= 8.0;

			// translate into the positive octant
			p.x += sweepr+tuber;
			p.y += tuber;
			p.z += sweepr+tuber;

			// find the noise value at this position
			n = pNoiseMan->NOISE1D(p*ls);
			// turn this into a grey scale
			cn = (BYTE)(n*255.0+0.5);
			// and place it in the bitmap
			pBitmap->PlotPixel(i,j,cn);
		}
	}
	
	return TRUE;
}
*/

/*

BOOL NoiseFractalFill::DoFill(double scale, 
							  double tuber, 
							  double sweepr, 
							  KernelBitmap *pBitmap) const
{
	double		ustep,vstep,ls,n,s,c;
	Vector3D	p;
	BYTE		cn;
	BitmapInfo	Info;
	INT32		Width,Height,i,j;

	// go get the noise manager
	NoiseMan* pNoiseMan = GetApplication()->GetNoiseManager();
	if (pNoiseMan==NULL)
		return FALSE;

	pBitmap->ActualBitmap->GetInfo(&Info);

	Width  = Info.PixelWidth;
	Height = Info.PixelHeight;

	ustep = 1.0/(double)Width;
	vstep = 1.0/(double)Height;
	   ls = 1.0/scale;
	  p.z = 0.0;

	for (j=0; j<Height; j++)
	{
		p.y = j*vstep*100.0;
		s = sin(2.0*p.y*XS_2PI);
		for (i=0; i<Width; i++)
		{
			p.x = i*ustep*100.0;
			c = s*cos(2.0*p.x*XS_2PI);
			// find the noise value at this position
			n = pNoiseMan->NOISE1D(p*ls);
			// turn this into a grey scale
			cn = (BYTE)(128+n*c*127.0+0.5);
			// and place it in the bitmap
			pBitmap->PlotPixel(i,j,cn);
		}
	}

	return TRUE;
}

*/

/********************************************************************************************

>	BOOL NoiseFractalFill::DoFill(double scale, 
								  double tuber, 
								  double sweepr, 
								  KernelBitmap *pBitmap) const
	
	Author:	   Mike_Kenny (Xara Group Ltd) <camelotdev@xara.com>
	Created:   16/01/97
	Purpose:   A noise based fill. This version uses 2d noise built from an array of random
			   numbers which are spline interpolated and wrapped at the edges. This is
			   dirtier and quicker than the 3d version and is less succeptable to warping
			   yet still manages to tile.
	
********************************************************************************************/

BOOL NoiseFractalFill::DoFill(double scale, 
							  UINT32 seed,
							  KernelBitmap *pBitmap) const
{
	double		ustep,vstep,ustart,vstart;
	double      n;
	Vector3D	p;
	BYTE		cn;
	BitmapInfo	Info;
	INT32		Width,Height,i,j;

	// go get the noise manager
	NoiseMan* pNoiseMan = GetApplication()->GetNoiseManager();
	if (pNoiseMan==NULL)
		return FALSE;

	// set the random number generator.
	pNoiseMan->SEEDTABLE(seed);

	pBitmap->ActualBitmap->GetInfo(&Info);

	Width  = Info.PixelWidth;
	Height = Info.PixelHeight;

	ustep = 1.0/(double)Width;
	vstep = 1.0/(double)Height;
	vstart = vstep/2.0;

    p.z = 0.0;
	for (j=0; j<Height; j++)
	{
		p.y = vstart;
		ustart = ustep/2.0;
		for (i=0; i<Width; i++)
		{
			p.x = ustart;
			// find the noise value at this position
			n = pNoiseMan->NOISE1D(p, scale);
			// turn this into a grey scale
			cn = (BYTE)(n*255.0+0.5);
			// and place it in the bitmap
			pBitmap->PlotPixel(i,j,cn);
			ustart += ustep;
		}
		vstart += vstep;
	}

/*	double ng2 = ustep/2.0;
	double ng1 = 1.0 - ng2;

	double n0,n1;
	double t;
	
	vstart = vstep + vstep/2.0;
	for (j=1; j<Height-1; j++)
	{
		p.y = vstart;
		p.x = ng1;
		n0  = pNoiseMan->NOISE1D(p);
		p.x = ng2;
		n1  = pNoiseMan->NOISE1D(p);

		t = 1.0/3.0;
		n = n0 + t*(n1-n0);
		cn = (BYTE)(n*255.0+0.5);
		pBitmap->PlotPixel(Width-1,j,cn);

		t = 2.0/3.0;
		n = n0 + t*(n1-n0);
		cn = (BYTE)(n*255.0+0.5);
		pBitmap->PlotPixel(0,j,cn);
		
		vstart += vstep;
	}
*/
	return TRUE;
}
	


