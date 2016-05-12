
// Implemetation of low level noise function calls. See Texturing and Modelling
//	a procedural approach.


#include "camtypes.h"
#include "vector3d.h"
#include "noisebas.h"
#include "noise1.h"


/**************************************************************************************

BOOL NoiseMan::Construct()

***************************************************************************************/

NoiseMan::NoiseMan()
{
	pNoise=NULL;
}

NoiseMan::~NoiseMan()
{
	Destroy();
}

BOOL NoiseMan::Initialise()
{
	if (pNoise==NULL)
	{
		NoiseGen1 *pNewNoise = new NoiseGen1;
		if (pNewNoise!=NULL)
		{
			if (!pNewNoise->Initialise())
			{
				delete pNewNoise;
				return FALSE;
			}
		}
		pNoise = pNewNoise;
	}
	return TRUE;
}


void NoiseMan::Destroy()
{
	if (pNoise!=NULL)
	{
		delete pNoise;
		pNoise=NULL;
	}
}

NoiseBase* NoiseMan::NoiseGen()
{
	return pNoise;
}


/**************************************************************************************

NoiseBase::NoiseBase()

***************************************************************************************/

NoiseBase::NoiseBase()
{
}

NoiseBase::~NoiseBase()
{
}

