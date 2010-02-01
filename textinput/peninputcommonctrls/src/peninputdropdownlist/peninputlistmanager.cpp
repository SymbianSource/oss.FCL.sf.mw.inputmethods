/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0""
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  List manager,which is used by drop down list
*
*/


#include "peninputlistmanager.h"
#include "peninputlistonerowwithouticon.h"
#include "peninputlistonerowwithouticonwithbubble.h"
#include "peninputlistmultirowwithicon.h"
#include "peninputlistmultirowwithiconwithbubble.h"
#include "peninputlistonerowwithicon.h"
#include "peninputlistonerowwithiconwithbubble.h"
#include "peninputlistmultirowwithouticon.h"

#include "peninputlistmultirowrollwithiconwithbubble.h"
#include "peninputlistonerowrollwithiconwithbubble.h"

#include "peninputlistmultirowrollwithicon.h"
#include "peninputlistonerowrollwithicon.h"
#include "peninputlistexpandablemultirowwithouticon.h"
#include <peninputdropdownlistcontext.h>

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CListManager 

// -----------------------------------------------------------------------------
// CListManager::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListManager* CListManager::NewL(MFepCtrlDropdownListContext* aContext)
    {
    CListManager* self = new (ELeave) CListManager();
    CleanupStack::PushL(self);
    self->ConstructL(aContext);
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CListManager::~CListManager
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CListManager::~CListManager()    
    {
    delete iListOneRowWithoutIcon;
    delete iListOneRowWithoutIconWithBubble;
    delete iListOneRowWithIcon;
    delete iListOneRowWithIconWithBubble;
    delete iListMultiRowWithIcon;
    delete iListMultiRowWithIconWithBubble;
    delete iListMultiRowWithoutIcon;
    delete iListMultiRowRollWithIcon;
    delete iListOneRowRollWithIcon;
    delete iListExpandableMultiRowWithoutIcon;
    delete iListMultiRowRollWithIconWithBubble;
    delete iListOneRowRollWithIconWithBubble;
    }
        
// -----------------------------------------------------------------------------
// CListManager::NewL
// 2nd construct function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListManager::ConstructL(MFepCtrlDropdownListContext* aContext)
    {
    iContext = aContext;
    iListOneRowWithoutIcon = CListOneRowWithoutIcon::NewL(this, aContext);
    iListOneRowWithoutIconWithBubble = CListOneRowWithoutIconWithBubble::NewL(this, aContext);
    iListOneRowWithIcon = CListOneRowWithIcon::NewL(this, aContext);
    iListOneRowWithIconWithBubble = CListOneRowWithIconWithBubble::NewL(this, aContext);
    iListMultiRowWithIcon = CListMultiRowWithIcon::NewL(this, aContext);
    iListMultiRowWithIconWithBubble = CListMultiRowWithIconWithBubble::NewL(this, aContext);
    iListMultiRowWithoutIcon = CListMultiRowWithoutIcon::NewL(this, aContext);
    iListMultiRowRollWithIcon = CListMultiRowRollWithIcon::NewL(this, aContext);
    iListOneRowRollWithIcon = CListOneRowRollWithIcon::NewL(this, aContext);
    iListExpandableMultiRowWithoutIcon = CListExpandableMultiRowWithoutIcon::NewL(this, aContext);
    iListMultiRowRollWithIconWithBubble = CListMultiRowRollWithIconWithBubble::NewL(this, aContext);
    iListOneRowRollWithIconWithBubble = CListOneRowRollWithIconWithBubble::NewL(this, aContext);
    }

// -----------------------------------------------------------------------------
// CListManager::SizeChanged
// size change
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListManager::SizeChanged()
    {
    iListOneRowWithoutIcon->SizeChanged();    
    iListOneRowWithoutIconWithBubble->SizeChanged();   
    iListOneRowWithIcon->SizeChanged();
    iListOneRowWithIconWithBubble->SizeChanged();
    iListMultiRowWithIcon->SizeChanged();
    iListMultiRowWithIconWithBubble->SizeChanged();
    iListMultiRowWithoutIcon->SizeChanged();
    iListMultiRowRollWithIcon->SizeChanged();
    iListOneRowRollWithIcon->SizeChanged();
    iListExpandableMultiRowWithoutIcon->SizeChanged();
    iListMultiRowRollWithIconWithBubble->SizeChanged();
    iListOneRowRollWithIconWithBubble->SizeChanged();
    }

// -----------------------------------------------------------------------------
// CListManager::Move
// move the list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListManager::Move(const TPoint& aOffset)
    {
    iListOneRowWithoutIcon->Move(aOffset);
    iListOneRowWithoutIconWithBubble->Move(aOffset);
    iListOneRowWithIcon->Move(aOffset);
    iListOneRowWithIconWithBubble->Move(aOffset);
    iListMultiRowWithIcon->Move(aOffset);
    iListMultiRowWithIconWithBubble->Move(aOffset);
    iListMultiRowWithoutIcon->Move(aOffset);
    iListMultiRowRollWithIcon->Move(aOffset);
    iListOneRowRollWithIcon->Move(aOffset);
    iListExpandableMultiRowWithoutIcon->Move(aOffset);
    iListMultiRowRollWithIconWithBubble->Move(aOffset);
    iListOneRowRollWithIconWithBubble->Move(aOffset);
    
    // Move all candidates
    for ( TInt i=0; i<iContext->CandidateCount(); i++ )   
        {
        CCandidate* candidate = iContext->GetCandidate(i);
        if ( candidate )
            {
            candidate->Move(aOffset);        
            }       
        }    
    }
    
// -----------------------------------------------------------------------------
// CListManager::ActiveList
// get active list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//   
CList* CListManager::ActiveList() const
    {
    return iActiveList;    
    }
// -----------------------------------------------------------------------------
// CListManager::SetActiveList
// set active list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CListManager::SetActiveList(CList* aList)
    {
    iActiveList = aList;
    }

// -----------------------------------------------------------------------------
// CListManager::ListOneRowWithoutIcon
// get one row without icon
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
CList* CListManager::ListOneRowWithoutIcon() const
    {
    return iListOneRowWithoutIcon;
    }
    
// -----------------------------------------------------------------------------
// CListManager::ListOneRowWithoutIcon
// get one row without icon
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
CList* CListManager::ListOneRowWithoutIconWithBubble() const
    {
    return iListOneRowWithoutIconWithBubble;
    }
    

// -----------------------------------------------------------------------------
// CListManager::ListOneRowWithIcon
// get one row with icon
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
CList* CListManager::ListOneRowWithIcon() const
    {
    return iListOneRowWithIcon;
    }

// -----------------------------------------------------------------------------
// CListManager::ListOneRowWithIcon
// get one row with icon
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
CList* CListManager::ListOneRowWithIconWithBubble() const
    {
    return iListOneRowWithIconWithBubble;
    }

// -----------------------------------------------------------------------------
// CListManager::ListMultiRowWithIcon
// get multi row with icon
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
CList* CListManager::ListMultiRowWithIcon() const
    {
    return iListMultiRowWithIcon;
    }
    
CList* CListManager::ListMultiRowWithIconWithBubble() const
    {
    return iListMultiRowWithIconWithBubble;
    }    
    

// -----------------------------------------------------------------------------
// CListManager::ListMultiRowWithoutIcon
// get multi row without icon
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
CList* CListManager::ListMultiRowWithoutIcon() const
    {
    return iListMultiRowWithoutIcon;
    }    

CList* CListManager::ListMultiRowRollWithIcon() const
    {
    return iListMultiRowRollWithIcon;
    }

CList* CListManager::ListOneRowRollWithIcon() const
    {
    return iListOneRowRollWithIcon;
    }    

CList* CListManager::ListExpandableMultiRowWithoutIcon() const
    {
    return iListExpandableMultiRowWithoutIcon;
    } 

// -----------------------------------------------------------------------------
// CListManager::ListMultiRowRollWithIconWithBubble
// get one row roll with icon with bubble
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//        
CList* CListManager::ListOneRowRollWithIconWithBubble() const    
    {
    return iListOneRowRollWithIconWithBubble;
    }

// -----------------------------------------------------------------------------
// CListManager::ListMultiRowRollWithIconWithBubble
// get multi row roll with icon with bubble
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//        
CList* CListManager::ListMultiRowRollWithIconWithBubble() const
    {
    return iListMultiRowRollWithIconWithBubble;
    }   
    
void CListManager::ResetStatus()
	{
    ListOneRowWithoutIcon()->ResetStatus();
    ListOneRowWithoutIconWithBubble()->ResetStatus();
    ListMultiRowWithIcon()->ResetStatus();
    ListMultiRowWithIconWithBubble()->ResetStatus();
    ListOneRowWithIcon()->ResetStatus();		
    ListOneRowWithIconWithBubble()->ResetStatus();
    ListMultiRowRollWithIcon()->ResetStatus();
    ListOneRowRollWithIcon()->ResetStatus();
    ListOneRowRollWithIconWithBubble()->ResetStatus();
    ListMultiRowRollWithIconWithBubble()->ResetStatus();
	}

// End Of File
