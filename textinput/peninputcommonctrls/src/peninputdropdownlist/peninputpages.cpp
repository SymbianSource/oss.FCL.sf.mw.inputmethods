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
* Description:  page manager,which is used by drop down list
*
*/


#include "peninputpages.h"

// ======== MEMBER FUNCTIONS ========

// Implementation of Class CPages 

// -----------------------------------------------------------------------------
// CPages::NewL
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPages* CPages::NewL()
    {
    CPages* self = new (ELeave) CPages();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

// -----------------------------------------------------------------------------
// CPages::~CPages
// factory function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPages::~CPages()    
    {
    iFirstIndexList.Close();
    iLastIndexList.Close();
    }
        
// -----------------------------------------------------------------------------
// CPages::NewL
// 2nd construct function
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPages::ConstructL()
    {
    Reset();
    }

// -----------------------------------------------------------------------------
// CPages::Reset
// reset page in the page list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPages::Reset()
    {
    iFirstIndexList.Reset();
    iLastIndexList.Reset();
    }

// -----------------------------------------------------------------------------
// CPages::PageCount
// get page count
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CPages::PageCount() const
    {
    return iLastIndexList.Count(); 
    }
       
// -----------------------------------------------------------------------------
// CPages::AddPage
// add one page into the page list
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CPages::AddPage(const TInt aFirstIndex, const TInt aLastIndex)
    {
    if ( aFirstIndex == aLastIndex )  
        {
        return;
        }        
    
    const TInt index = iLastIndexList.Find(aLastIndex);
    
    if ( index == KErrNotFound)
        {
        iLastIndexList.Append(aLastIndex);  
        iFirstIndexList.Append(aFirstIndex);  
        }     
    }
    
// -----------------------------------------------------------------------------
// CPages::GetPreviousIndex
// get the previous page
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPages::GetPreviousIndex(const TInt aLastIndex, 
                               TInt& aFirstIndexOfPrevious, 
                               TInt& aLastIndexOfPrevious) const
    {
    const TInt index = iLastIndexList.Find( aLastIndex );
    
    if ( index != KErrNotFound ) // If found it
        {         
        if ( index > 0 ) // If not the first page
            {
            aFirstIndexOfPrevious = iFirstIndexList[index-1];
            aLastIndexOfPrevious = iLastIndexList[index-1];
            }
        else // If is the first page
            {
            aFirstIndexOfPrevious = iFirstIndexList[0];
            aLastIndexOfPrevious = iLastIndexList[0];                        
            }
        return ETrue;         
        }
    else
        {
        return EFalse;
        }        
    }
        
// -----------------------------------------------------------------------------
// CPages::GetPageIndexByPos
// get the page of assigned position
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CPages::GetPageIndexByPos(const TInt aPos, 
                                TInt& aFirstIndex, 
                                TInt& aLastIndex) const
    {
    if ( ( aPos < 0 ) || ( aPos >= iLastIndexList.Count() ) )
        {
        return EFalse;        
        }

    aLastIndex = iLastIndexList[aPos];    
    aFirstIndex = iFirstIndexList[aPos];
    
    return ETrue;
    }

// End Of File
