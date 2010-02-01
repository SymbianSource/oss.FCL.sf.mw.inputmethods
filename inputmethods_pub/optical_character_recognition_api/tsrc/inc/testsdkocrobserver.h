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


#ifndef C_TESTSDKOCROBSERVER_H
#define C_TESTSDKOCROBSERVER_H

//  INCLUDES
#include <ocrsrv.h>

/**
 *  CTestNotesControl is used to add setting page control in its window
 */
class TTestObserver : public MOCREngineObserver
    {
public:
    /**
    *  fuction RecognizeBlockComplete from MOCREngineObserver
    */
    void RecognizeBlockComplete( TInt aError, 
                                 const TOCRTextRgnInfo* aBlocks, 
                                 const TInt aBlockCount );
    
    /**
    *  fuction RecognizeSpecialRegionComplete from MOCREngineObserver
    */
    void RecognizeSpecialRegionComplete( TInt aError, 
                                         const TOCRTextRgnInfo* aBlocks, 
                                         const TInt aBlockCount );
    
    /**
    *  fuction RecognizeComplete from MOCREngineObserver
    */
    void RecognizeComplete( TInt aError, 
                            const TOCRTextRgnInfo* aBlocks, 
                            const TInt aBlockCount );
    
    /**
    *  fuction LayoutComplete from MOCREngineObserver
    */
    void LayoutComplete( TInt aError, 
                         const TOCRBlockInfo* aBlocks, 
                         const TInt aBlockCount );
    
    /**
    *  fuction RecognizeProcess from MOCREngineObserver
    */
    void RecognizeProcess( const TUint aPercent );
    };

#endif /*C_TESTSDKOCROBSERVER_H*/

