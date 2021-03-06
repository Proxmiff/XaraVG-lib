
// Contains general initialisation of the kernel, before Tool::InitTools() is called


#include "camtypes.h"
#include "csrstack.h"
#include "bfxbase.h"
#include "module.h"
#include "pathedit.h"
#include "opbreak.h"
#include "opsmooth.h"
#include "moldedit.h"
#include "layer.h"
#include "ngsetop.h"

#ifdef RALPH

// Ralph builds only
BOOL InitPreTools()
{
	return
	(
		   InitMaths()					// Init Maths class
		&& InitHandles()				// Init Handles Manager
		&& InitMemory()					// Init Memory Manager
		&& CursorStack::Init()			// Must come before tool initialisation.
		&& Module::InitModules()
	);
}

#else

// Normal exe's and standalones
BOOL InitPreTools()
{
	return
	(
		   InitMaths()					// Init Maths class
		&& InitHandles()				// Init Handles Manager
		&& InitMemory()					// Init Memory Manager
		&& CursorStack::Init()			// Must come before tool initialisation.
PORTNOTE("other","Removed BFX")
#if !defined(EXCLUDE_BFX)
		&& BitmapEffectBase::Init()		// must be before anything using this subsystem
#endif
		&& Module::InitModules()
		&& Layer::Init()				// Init the layer system
		&& Operation::Initialise()		// Init the operations system
PORTNOTE("other","Removed PluginOILFilter, various Op initializers, Layer system, operation system, dibutils, namingsystem usage")
#ifndef STANDALONE
		&& OpDeletePoints::Init()		// Deletes selected points
		&& OpBreakAtPoints::Init()		// Init the break at points operator
		&& OpSmoothSelection::Init()	// Init the smooth selection operator
		&& OpDragOrigin::Init() 		// Init the ability to drag origin blobs
#endif
#ifndef EXCLUDE_FROM_XARALX
#ifndef STANDALONE
		&& DIBUtil::Init()				// Init the dibutil stuff (here as needs prefs set up)
		&& InitNamingSystem()			// must be before tool info-bars are loaded
#endif
		&& PluginOILFilter::AutoRegisterFilters()
#endif
		&& StringVar::Init()
	);
}

#endif
