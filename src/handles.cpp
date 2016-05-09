
// Keeps track of handles onto memory blocks and pointers



#include "camtypes.h"



/*==========================================================================================*/


// The Handle Array

typedef struct HandleTableStruct {
    ADDR Address;									// The address of the block
	#ifdef USE_UNIQUE_IDS
    	WORD UniqueID;								// Helps to keep Handles unique
  //		char Kludge[8-sizeof(ADDR)-sizeof(WORD)];	// Make the struct a power of 2 in length
		char Kludge[(2*sizeof(ADDR))-sizeof(ADDR)-sizeof(WORD)];	// Make the struct a power of 2 in length
	#endif
} HandleTableElement;

#define HandleTableSize			500					// Number of handles per table
#define HandleTableElementSize	(sizeof( HandleTableElement ))
													// The size of each element in the table
													
#define HandleTableByteSize		(HandleTableSize * HandleTableElementSize)

MemoryBlock HandlesMemory;					// actual memory for handles array

HandleTableElement* 	HandlesTable;		// Array of Handles + Other info
static UINT32	HandleTableTotalSize;			// Total size of the table to date
static UINT32	ElementsInHandleTable;			// The total number of elements in the table
static UINT32	HandlesInUse;					// The Number of Handles currently in use
#ifdef USE_UNIQUE_IDS
	static WORD	CurrUniqueId = 1;			// The Current ID is incremented each time
											// a handle is issued.
#endif



/*==========================================================================================*/

/*********************************************************************************************

>		BOOL InitHandles()

		Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
		Created: 	27/04/93		
		Returns: 	TRUE if all went OK, FALSE otherwise
		Purpose:	Starts up the Handles stuff. It sets all the handles in the table to 
					sensible starting values and inits a count of the number of handles
		
*********************************************************************************************/

BOOL InitHandles()
{
    // Try and get some memory from the OS for my lovely table
	HandleTableTotalSize  = HandleTableByteSize;
	ElementsInHandleTable = HandleTableSize;
	HandlesInUse = 0;
	HandlesTable = (HandleTableElement*)
					HandlesMemory.Init( HandleTableTotalSize, TRUE, MEMORYBLOCK_RELOCHEAP);	

	// check to see if we got the ram needed
	if (HandlesTable==NULL)
		return FALSE;

	// init all the elements in the table to a non-existent handle
	for (UINT32 n=0; n<ElementsInHandleTable; n++)
	{
		// Set the Address to Bad
		HandlesTable[n].Address  = PBYTE(BAD_MHANDLE);
		
		// if we are using unique IDs, set it to zero
		#ifdef USE_UNIQUE_IDS
			HandlesTable[n].UniqueID = 0;
		#endif
	}

	// All worked
	return TRUE;	
}






/*********************************************************************************************

>		void DeinitHandles()

		Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
		Created:	27/04/93
		Purpose:	Closes down the Handles manager. Should free up any space allocated
					by the Handle Manager during it life
		
*********************************************************************************************/

void DeinitHandles()
{   
#ifdef _DEBUG
	// Tell me how many handles were in use at its peak
	TRACE( wxT("DeinitHandles: Max size of handle table = %ld"), ElementsInHandleTable );

	// Tell everyone about any handles that were not released
	UINT32 Waste = 0;
	for (UINT32 n=1; n<ElementsInHandleTable; n++)
	{	
		if (HandlesTable[n].Address != BAD_MHANDLE )
		{
			TRACE( wxT("Handle %d not released (0x%08x)"), n, HandlesTable[n].Address);
			Waste++;
		}
	}
	TRACE( wxT("DeinitHandles: %ld handles not released"), Waste );
#endif
	
	// dealloc the memory we now have from the OS
	HandlesMemory.DeInit();
}




/*********************************************************************************************

>		ADDR DescribeHandle( MHANDLE Handle )

		Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
		Created:	27/04/93
		Inputs:		A Handle to a memory location
		Returns:	The address associated with the handle supplied
		Purpose:	To get an up to date location of the memory location
		
*********************************************************************************************/

ADDR DescribeHandle( MHANDLE Handle )
{
	#ifdef USE_UNIQUE_IDS
		ENSURE( (Handle & 0xFFFF) > BAD_MHANDLE, "DescribeHandle: Handle was Bad" );
		ENSURE( (Handle & 0xFFFF) < ElementsInHandleTable, "DescribeHandle: Handle out of range" );
	#else
		ENSURE(Handle > BAD_MHANDLE, "DescribeHandle: Handle was Bad" );
		ENSURE(Handle < ElementsInHandleTable, "DescribeHandle: Handle out of range" );
	#endif
	
	// passed all the assertions, but make no checks that the addr being returned
	// is valid. If it is not then they will be passed BAD_MHANDLE

	// Must also check that the Unique ID in the Handle is the same as the one in the
	// Handle table, but only if USE_UNIQUE_IDS is defined
	
	
    #ifdef USE_UNIQUE_IDS
    	// Have to decode the handles
		if ((HIWORD(Handle)) != (HandlesTable[LOWORD(Handle)].UniqueID))
			return BAD_MHANDLE;
		else
			return HandlesTable[LOWORD(Handle)].Address;
			
	#else
		// the more efficent version, with no unique ID passing
		return HandlesTable[Handle].Address;		
	#endif
}




/*********************************************************************************************

>		BOOL ReleaseHandle( MHANDLE Handle )

		Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
		Created:	27/04/93
		Inputs:		A Handle that is currently in use
		Returns:	A BOOL to say if the release of the Handle was succesful
		Purpose:	To free up a handle that is no longer required by its user
		Notes:		There are 2 versions of this function below - one is used if
					USE_UNIQUE_IDS is defined and the other if it is not
		
*********************************************************************************************/

#ifdef USE_UNIQUE_IDS
BOOL ReleaseHandle( MHANDLE Handle )
{
//	TRACEUSER("Gerry", _T("ReleaseHandle - %d"), Handle);

	// make sure its not garbage
	ENSURE( (Handle & 0xFFFF) > BAD_MHANDLE, "ReleaseHandle: Handle was bad" );
	ENSURE( (Handle & 0xFFFF) < ElementsInHandleTable, "ReleaseHandle: Handle was bad" );

    WORD LowWord = LOWORD(Handle);

	if ( HIWORD(Handle) != HandlesTable[LowWord].UniqueID )
	{
		TRACE( _T("ReleaseHandle - Someone tried to release a handle that is not theirs!\n") );
		return FALSE;								// Not your handle to release
	}
	else
	{
		if (HandlesTable[LowWord].Address != BAD_MHANDLE)
		{
			// mark it as available
			HandlesTable[LowWord].Address   = BAD_MHANDLE;
			HandlesTable[LowWord].BlockSize = 0;
			HandlesTable[LowWord].UniqueID  = 0;
			HandlesInUse --;
			return TRUE;
		}

		return FALSE;								// if we got here, return false
	}
}

#else

BOOL ReleaseHandle( MHANDLE Handle )
{
//	TRACEUSER("Gerry", _T("ReleaseHandle - %d"), Handle);

	// make sure its not garbage
	ENSURE( Handle > BAD_MHANDLE, "ReleaseHandle: Handle was BAD_MHANDLE" );
	ENSURE( Handle < ElementsInHandleTable, "ReleaseHandle: Handle was too big" );
                
	if ( HandlesTable[Handle].Address != BAD_MHANDLE )
	{
		// mark it as available
		HandlesTable[Handle].Address = PBYTE(BAD_MHANDLE);
		HandlesInUse --;
		return TRUE;
	}

	return FALSE;								// if we got here, return false
}
#endif







/*********************************************************************************************

>		ADDR AlterHandle( MHANDLE Handle, ADDR Address )

		Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
		Created:	27/04/93
		Inputs:		Handle  - A handle currently in use by the caller
					Address - The new memory location to associate with the handle
					Size	  - The new size of the the block				  
		Returns:	The old address that used to be associated with the handle
		Purpose:	The change the location associated with the handle without having to 
					release and then re-claim a new handle
		
*********************************************************************************************/

ADDR AlterHandle( MHANDLE Handle, ADDR Address )
{
	ADDR OldAddr;
	
	#ifdef USE_UNIQUE_IDS
		// Make sure its not garbage
		ENSURE( LOWORD(Handle) > BAD_MHANDLE, "AlterHandle: Handle was bad" );
		ENSURE( LOWORD(Handle) < ElementsInHandleTable, "AlterHandle: Handle too big" );
	
		// note old value and change it to the new one
		OldAddr = HandlesTable[LOWORD(Handle)].Address;
		HandlesTable[ LOWORD(Handle) ].Address = Address;
	#else
		// make sure its not garbage
		ENSURE( Handle > BAD_MHANDLE, "AlterHandle: Handle was bad" );
		ENSURE( Handle < ElementsInHandleTable, "AlterHandle: Handle was too big" );
	
		// note old value and change it to the new one
		OldAddr = HandlesTable[Handle].Address;
		HandlesTable[Handle].Address = Address;
	#endif
	
	// retrun back the old value
	return OldAddr;
}





/*********************************************************************************************

>		BOOL RelocateHandles( ADDR LowAddr, ADDR HighAddr, INT32 Shift )

		Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
		Created:	27/04/93
		Inputs:		LowAddr  - The low address in the region supplied
					HighAddr - The high address in the region supplied
					Shift    - The amount to alter all the address in the region by.
		Returns:	TRUE
		Purpose:	Finds all the memory locations in the supplied region that have 
					handles associated with them and modifies the address. Usually called
					by the memory manager when it shifts a block of memory to a different 
					location
		
*********************************************************************************************/

BOOL RelocateHandles( ADDR LowAddr, ADDR HighAddr, INT32 Shift )
{
	// Make sure that the params are not rubbish
	ENSURE( HighAddr > LowAddr, "RelocateHandles: Low Addr is Higher the High Addr" );

	// Check if nothing needs to be done	
	if (Shift == 0)
		return TRUE;

	// Loop through the handles
	ADDR Address;		
    for (UINT32 n=1; n<ElementsInHandleTable; n++)
    {
    	// First out get the address from the array    	
    	Address = HandlesTable[n].Address;

    	if (Address != BAD_MHANDLE)
    	{
    		if (Address >= LowAddr  &&  Address < HighAddr)
    			// If the address is in the range, shift it and store it back in the array
    			HandlesTable[n].Address = Address + Shift;
    	}
    }
    
    return TRUE;
}




/*********************************************************************************************

>		BOOL ReleaseRangeOfHandles( ADDR LowAddr, ADDR HighAddr )

		Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
		Created:	28/04/93
		Inputs:		LowAddr  - The Low address in the range
					HighAddr - The High address in the range to be searched
		Returns:	TRUE
		Purpose:	Find all the handles that refer to address in the the given range
					and release them
		
*********************************************************************************************/

BOOL ReleaseRangeOfHandles( ADDR LowAddr, ADDR HighAddr )
{
//	TRACEUSER("Gerry", _T("ReleaseRange - 0x%08x to 0x%08x"), LowAddr, HighAddr);

	// Make sure the params are not rubbish
	ENSURE( HighAddr > LowAddr, "ReleaseRangeOfHandles: Low Addr is Higher the High Addr" );
	
	// loop through the table
	ADDR Address;	
    for (UINT32 n=1; n<ElementsInHandleTable; n++)
    {
    	// First out get the address from the array
    	Address = HandlesTable[ n ].Address;

		// if it is ok
		if (Address != BAD_MHANDLE)
		{
			if( Address >= LowAddr &&
				Address < HighAddr )
			{
				// If the address is in the range, release it
//				TRACEUSER("Gerry", _T("ReleaseRange - releasing %d"), n);
				HandlesTable[n].Address = PBYTE(BAD_MHANDLE);
				HandlesInUse --;
			}
		}
	}

    return TRUE;
}


/*********************************************************************************************

>		MHANDLE ClaimHandle( ADDR Address )

		Author:		Rik_Heywood (Xara Group Ltd) <camelotdev@xara.com>
		Created:	27/04/93
		Inputs:		Address - The address of a block or somewhere in a block of allocated
							  memory.
					Size	- The size of the block of memory being stored. If zero then just
							  a pointer stored. If this parameter is not passed, then zero 
							  assumed
		Returns:	A handle to the address or BAD_MHANDLE
		Purpose:	Allocates a handle to the supplied address. The handle should be used
					as the reference to the memory location from then on to allow the
					memory manager to move the allocation around
		
*********************************************************************************************/

MHANDLE ClaimHandle( ADDR Address )
{
	// Need to scan along the list of handles to find an unused one
	if (HandlesInUse == (ElementsInHandleTable-1))
	{
		// we have run out of handles
		TRACEUSER( "Rik", wxT("ClaimHandle - No more handles available. Trying to get some more\n") );
	
		// First off we had better try to extend our allocation of memory to the table
		// If it fails, return saying there are no more handles left!
		if (!HandlesMemory.Grow(HandleTableTotalSize, 
								HandleTableTotalSize+HandleTableByteSize, (LPVOID*)&HandlesTable ))
		{
			TRACE( wxT("ClaimHandle - Failed to get more handles!!!!   P A N I C\n") );
			return BAD_MHANDLE;
		}

		// grew handle table OK, Set all the new bits of mem to defaults
		for (INT32 i=0; i<HandleTableSize; i++)
			HandlesTable[ElementsInHandleTable+i].Address = PBYTE(BAD_MHANDLE);

		// increase the counters
		HandleTableTotalSize += HandleTableByteSize;
		ElementsInHandleTable += HandleTableSize;
	}
	
	// Ok, we must have some handles by now, so lets try and find one	
	UINT32 Handle = ElementsInHandleTable - 1;
	while (Handle>0)
	{
		if (HandlesTable[Handle].Address == BAD_MHANDLE )
		{ 
			// Store the Addr
			HandlesTable[Handle].Address = Address;
			HandlesInUse ++;

			// if we are using unique IDs, do this
			#ifdef USE_UNIQUE_IDS
				// Using Unique IDS, so get the next id and store it away
				WORD UniqueID = CurrUniqueId++;
				HandlesTable[ Handle ].UniqueID = UniqueID;
				return MAKEINT32( Handle, UniqueID );
			#else
//				TRACEUSER("Gerry", _T("ClaimHandle - %d"), Handle);
				return Handle;
			#endif
		}
		
		Handle --;
	}
		
	// Should never get here
	ENSURE( FALSE, "ClaimHandle: We Should never have reached this bit!" );
	return BAD_MHANDLE;
}

