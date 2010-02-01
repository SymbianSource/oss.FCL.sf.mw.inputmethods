/*
* Copyright (c) 2002 - 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  test ocrsrv.h
 *
*/


// [INCLUDE FILES]

#include "testsdkocrobserver.h"

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// TTestObserver::RecognizeBlockComplete
// -----------------------------------------------------------------------------
//
void TTestObserver::RecognizeBlockComplete( TInt /*aError*/, 
                                            const TOCRTextRgnInfo* /*aBlocks*/, 
                                            const TInt /*aBlockCount*/ )
    {
    
    }
    
// -----------------------------------------------------------------------------
// TTestObserver::RecognizeBlockComplete
// -----------------------------------------------------------------------------
//
void TTestObserver::RecognizeSpecialRegionComplete( TInt /*aError*/, 
                                                    const TOCRTextRgnInfo* /*aBlocks*/, 
                                                    const TInt /*aBlockCount*/ )
    {
    
    }
    
// -----------------------------------------------------------------------------
// TTestObserver::RecognizeBlockComplete
// -----------------------------------------------------------------------------
//
void TTestObserver::RecognizeComplete( TInt /*aError*/, 
                                       const TOCRTextRgnInfo* /*aBlocks*/, 
                                       const TInt /*aBlockCount*/ )
    {
    
    }
    
// -----------------------------------------------------------------------------
// TTestObserver::RecognizeBlockComplete
// -----------------------------------------------------------------------------
//
void TTestObserver::LayoutComplete( TInt /*aError*/, 
                                    const TOCRBlockInfo* /*aBlocks*/, 
                                    const TInt /*aBlockCount*/ )
    {
    
    }
    
// -----------------------------------------------------------------------------
// TTestObserver::RecognizeBlockComplete
// -----------------------------------------------------------------------------
//
void TTestObserver::RecognizeProcess( const TUint /*aPercent*/ )
    {
    
    }
    
