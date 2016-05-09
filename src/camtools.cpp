

#include "camtypes.h"

#include "dll_lib.h"
#include "viewmod.h"

DECLARE_SOURCE("$Revision$");


// Base new DLLs on this file.

extern "C" void * MODULE_FN CreateModule(UINT32 Type)
{
	switch (Type)
	{
		case 1:
			return new ViewModule;
		// Add further moduless here...
	}
	
	return NULL;
}


// This function is used when getting info on all the modules that are around (app startup).

extern "C" UINT32 MODULE_FN GetNumberOfModules()
{
	return 1; // Increase this as you add more modules.
}


