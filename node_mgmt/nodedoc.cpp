

// class NodeRenderableDocument which holds the whole document tree


#include "camtypes.h"
#include "nodedoc.h"
#include "chapter.h"
  
CC_IMPLEMENT_DYNAMIC(NodeDocument, NodeRenderablePaper)          

// Declare smart memory handling in Debug builds
#define new CAM_DEBUG_NEW
 
/***********************************************************************************************

>    NodeDocument::NodeDocument() 

     Author:    Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
     Created:   13/5/93
     Inputs:    -
     Outputs:   
     Returns:   -
			  
     Purpose: This constructor creates a NodeDocument linked to no other nodes, with all status
		 	  flags false, and NULL bounding and pasteboard rectangles.            
			
     Errors:    

***********************************************************************************************/
 

NodeDocument::NodeDocument(): NodeRenderablePaper()
{   
	pParentDoc = NULL;
}                    
 
                                                                      
/***********************************************************************************************

>   NodeDocument::NodeDocument
	(
		Node* ContextNode,  
		AttachNodeDirection Direction,  
		BOOL Locked = FALSE, 
		BOOL Mangled = FALSE,  
		BOOL Marked = FALSE, 
		BOOL Selected = FALSE, 
	)

    Author:  Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
    Created: 26/4/93             
    
    Inputs: ContextNode: Pointer to a node which this node is to be attached to.     
    
			Direction: 
			
				Specifies the direction in which this node is to be attached to the 
				ContextNode. The values this variable can take are as follows: 
								  
				PREV      : Attach node as a previous sibling of the context node
				NEXT      : Attach node as a next sibling of the context node
				FIRSTCHILD: Attach node as the first child of the context node
				LASTCHILD : Attach node as a last child of the context node                               
					      
			The remaining inputs specify the status of the node: 
			
			Locked:     Is node locked ?
			Mangled:    Is node mangled ?
			Marked:     Is node marked ?
			Selected:   Is node selected ?
			
    Outputs:   -
    Returns:   - 
    Purpose: This constructor initialises the node and links it to ContextNode in the
			 direction specified by Direction. All neccesary tree links are
			 updated.     
			 
    Errors:  An assertion error will occur if ContextNode is NULL


***********************************************************************************************/

NodeDocument::NodeDocument(Node* ContextNode,  
						   AttachNodeDirection Direction, 
						   BOOL Locked, 
						   BOOL Mangled,  
					       BOOL Marked, 
						   BOOL Selected):NodeRenderablePaper(ContextNode, Direction, Locked, 
						   										Mangled,Marked, Selected)
{
	pParentDoc = NULL;
}



/***********************************************************************************************

>    NodeDocument::~NodeDocument() 

     Author:    Jason_Williams (Xara Group Ltd) <camelotdev@xara.com>
     Created:   5/5/94
     Inputs:    -
     Outputs:   -
     Returns:   -
			  
     Purpose:	NodeDocument destructor.			
     Errors:
	 Notes:		OK, so this does nothing. But it used to. I've just ripped out all
	 			the code again 3 months later...

***********************************************************************************************/

NodeDocument::~NodeDocument()
{
}


/********************************************************************************************

>	void NodeDocument::SetParentDoc(BaseDocument *pNewDoc)

	Author:		Tim_Browse (Xara Group Ltd) <camelotdev@xara.com>
	Created:	14/02/95
	Inputs:		pNewDoc - the Document to attach to, or NULL if detaching.
	Purpose:	Specify which Document object a NodeDocument object is attached to.
				This cannot be done again, unless this function is first called with
				NULL as the document to attachto.
	Errors:		Already attached to a document => ERROR2
	SeeAlso:	NodeDocument::GetParentDoc

********************************************************************************************/

void NodeDocument::SetParentDoc(BaseDocument *pNewDoc)
{
    if (pParentDoc != NULL)
	{
		// Error
		ERROR2RAW("NodeDocument is already attached to a Document!");
	}
	else
	{
		// Attach document.
		pParentDoc = pNewDoc;
	}
}

/********************************************************************************************

>	BaseDocument *NodeDocument::GetParentDoc()

	Author:		Tim_Browse (Xara Group Ltd) <camelotdev@xara.com>
	Created:	14/02/95
	Returns: 	The Document that this node is attached to, or NULL if not attached.
	Purpose:	Find out which Document object a NodeDocument object is attached to.
	SeeAlso:	NodeDocument::SetParentDoc

********************************************************************************************/

/********************************************************************************************

>	BOOL NodeDocument::IsNodeDocument() const

	Author:		Tim_Browse (Xara Group Ltd) <camelotdev@xara.com>
	Created:	15/02/95
	Returns:	TRUE - this is a NodeDocument.
	Purpose:	Tell the caller that this is a NodeDocument.

********************************************************************************************/

BOOL NodeDocument::IsNodeDocument() const
{
	// Yes, it's a NodeDocument
	return TRUE;
}


/***********************************************************************************************
> Node* NodeDocument::SimpleCopy()  

    Author:     Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
    Created:    28/5/93
	
	Inputs:         - 
    Outputs:     
    Returns:    A copy of the node, or NULL if memory runs out

    Purpose:    This method returns a shallow copy of the node with all Node pointers NULL.
				The function is virtual, and must be defined for all derived classes.
	
   	Errors:     If memory runs out when trying to copy, then ERROR is called with an out of memory
				error and the function returns NULL.

	Scope:      protected
***********************************************************************************************/

Node* NodeDocument::SimpleCopy()
{
	NodeDocument* NodeCopy; 
	NodeCopy = new NodeDocument(); 
	ERRORIF(NodeCopy == NULL, _R(IDE_NOMORE_MEMORY), NULL); 
	CopyNodeContents(NodeCopy);   
	return (NodeCopy);
} 


/***********************************************************************************************
>   void NodeDocument::CopyNodeContents(NodeDocument* NodeCopy)

    Author:     Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
    Created:    07/6/93
	
	Inputs:        
    Outputs:    A copy of this node
 
   	Returns:    -
		 
    Purpose:    This method copies the node's contents to the node pointed to by NodeCopy.
		      
	Errors:     An assertion failure will occur if NodeCopy is NULL
	
    scope:      protected
								     
***********************************************************************************************/

void NodeDocument::CopyNodeContents(NodeDocument* NodeCopy)
{                         
	ENSURE(NodeCopy != NULL, "Trying to copy a node's contents to a NULL node");  
	NodeRenderablePaper::CopyNodeContents(NodeCopy); 
 	NodeCopy->LowExtent = LowExtent; 
 	NodeCopy->HighExtent = HighExtent;         
}


/***********************************************************************************************
>   void NodeDocument::PolyCopyNodeContents(NodeRenderable* pNodeCopy)

    Author:     Phil_Martin (Xara Group Ltd) <camelotdev@xara.com>
    Created:    18/12/2003
    Outputs:    -
    Purpose:    Polymorphically copies the contents of this node to another
	Errors:     An assertion failure will occur if NodeCopy is NULL
    Scope:      protected
								     
***********************************************************************************************/

void NodeDocument::PolyCopyNodeContents(NodeRenderable* pNodeCopy)
{
	ENSURE(pNodeCopy, "Trying to copy a node's contents into a NULL node");
	ENSURE(IS_A(pNodeCopy, NodeDocument), "PolyCopyNodeContents given wrong dest node type");

	if (IS_A(pNodeCopy, NodeDocument))
		CopyNodeContents((NodeDocument*)pNodeCopy);
}



/***********************************************************************************************

>	DocCoord NodeDocument::LoExtent() const

	Author:		Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/5/93
	Inputs:		-
	Outputs:	-
	Returns:	The documents low extent i.e. the low corner of the final chapters
				pasteboard rectangle.                                    
				
	Purpose:    For finding the low extent of the document
	Errors:		-
	SeeAlso:	-

***********************************************************************************************/
    
DocCoord NodeDocument::LoExtent() const
{
	return (LowExtent); 
}
 
/***********************************************************************************************

>	DocCoord NodeDocument::HiExtent() const

	Author:		Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/5/93
	Inputs:		-
	Outputs:	-
	Returns:	The documents high extent, i.e. the high corner of the first chapters
				pasteboard rectangle.     
				
	Purpose:    For finding the high extent of the document
	Errors:		-
	SeeAlso:	-

***********************************************************************************************/

DocCoord NodeDocument::HiExtent() const
{ 
	return (HighExtent); 
}                                   

/***********************************************************************************************

>	DocCoord NodeDocument::DescribeExtents(DocCoord* LoExtent, DocCoord* HiExtent) const

	Author:		Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
	Created:	02/06/93
	Outputs:	LoExtent: The documents low extent i.e. the low corner of the final chapters
						  pasteboard rectangle. 
				HiExtent: The documents high extent i.e. the high corner of the first chapters
						  pasteboard rectangle. 
	Returns:	- 
	Purpose:    For finding the extents of the document.
	Errors:		-
	SeeAlso:	-

***********************************************************************************************/

void NodeDocument::DescribeExtents(DocCoord* LoExtent, DocCoord* HiExtent) const
{
	*LoExtent = LowExtent; 
	*HiExtent = HighExtent; 	
}

/***********************************************************************************************

>	DocCoord NodeDocument::SetExtents()

	Author:		Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
	Created:	28/4/93
	Inputs:		-
	Outputs:	-
	Returns:	-  
	Purpose:    For setting the document's extents in the NodeDocument, and then updating the 
				extents in the current document. The NodeDocument finds its extents by 
				looking at its first and last chapter's pasteboard rectangles. 
				
				This method relies on the current document having been set.    
	
	Errors:		An assertion failure will occur if the document has no chapters

***********************************************************************************************/
/* Technical notes:

The NodeDocuments LowExtent is set to the low corner of the final chapters pasteboard rectangle
The NodeDocuments HighExtent is set to the high corner of the first chapters pasteboard rectangle 
   
*/                                                                                
                                                                               
void NodeDocument::SetExtents()
{   
	BOOL FoundChapter = FALSE; // Flag indicating if we have found a chapter beneath the doc

	Node* CurrentNode = FindFirstChild();   

	// Find the first chapter of a document
	while ((CurrentNode != NULL) && (!FoundChapter)) 
	{ 
		if (CurrentNode->IsKindOf(CC_RUNTIME_CLASS(Chapter)))
		{
			FoundChapter = TRUE; 
			HighExtent = ((NodeRenderablePaper*)CurrentNode)->GetPasteboardRect(FALSE).HighCorner(); 
			// Low extent will change if we find another chapter 
			LowExtent =  ((NodeRenderablePaper*)CurrentNode)->GetPasteboardRect(FALSE).LowCorner(); 
		}	  
		CurrentNode = CurrentNode->FindNext(); 
	}
	
	// A document should have at least one chapter when setting the extents
	ENSURE(FoundChapter,"Trying to set the extents of a document with no chapters"); 
	
	// Find the last chapter which may be the first
	while(CurrentNode != NULL)
	{
		if (CurrentNode->IsKindOf(CC_RUNTIME_CLASS(Chapter)))
		{
			LowExtent = ((NodeRenderablePaper*)CurrentNode)->GetPasteboardRect().LowCorner();  
		}	  
		CurrentNode = CurrentNode->FindNext(); 
	}        
	 
	// Inform the document that its extents have changed (only do it for 'real' documents,
	// and not e.g. the clipboard.
	if (IS_A(pParentDoc, Document))
		((Document *) pParentDoc)->UpdateExtents(LowExtent, HighExtent); 
}
 
#ifdef _DEBUG
 
void NodeDocument::ShowDebugTreeDetails() const
{                                 
	TRACE( _T("Node Document ") );
	Node::ShowDebugTreeDetails(); 
}  

#endif
 
/********************************************************************************************

>	void* NodeDocument::GetDebugDetails(StringBase* Str) 

	Author:		Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
	Created:	21/9/93
	Inputs:		-
	Outputs:	Str: String giving debug info about the node
	Returns:	-
	Purpose:	For obtaining debug information about the Node
	Errors:		-
	SeeAlso:	-

********************************************************************************************/

     
void NodeDocument::GetDebugDetails(StringBase* Str) 
{          
#ifdef _DEBUG
	NodeRenderablePaper::GetDebugDetails(Str); 
	String_256 TempStr; 
	TempStr._MakeMsg(TEXT("\r\nLowExtent = (#1%ld, #2%ld)\r\nHighExtent = (#3%ld, #4%ld)\r\n"), 
					 LowExtent.x, LowExtent.y, HighExtent.x, HighExtent.y); 
	(*Str)+=TempStr;
#endif
}   

/********************************************************************************************

>	virtual UINT32 NodeDocument::GetNodeSize() const

	Author:		Simon_Maneggio (Xara Group Ltd) <camelotdev@xara.com>
	Created:	6/10/93
	Inputs:		-
	Outputs:	-
	Returns:	The size of the node in bytes
	Purpose:	For finding the size of the node 
				
	SeeAlso:	Node::GetSubtreeSize

********************************************************************************************/

UINT32 NodeDocument::GetNodeSize() const 
{     
	return (sizeof(NodeDocument)); 
}  

