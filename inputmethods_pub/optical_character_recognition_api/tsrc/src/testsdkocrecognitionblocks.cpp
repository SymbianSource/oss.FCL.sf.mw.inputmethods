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
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include <ocrsrv.h>
#include <ocrcommon.h>

#include "testsdkocrecognition.h"
#include "testsdkocrobserver.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestSDKOCRecognition::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestSDKOCRecognition::RunMethodL( CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        ENTRY( "TestORCCreateOCREngineL", CTestSDKOCRecognition::TestORCCreateOCREngineL ),
        ENTRY( "TestORCReleaseOCREngineL", CTestSDKOCRecognition::TestORCReleaseOCREngineL ),

        };

    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CTestSDKOCRecognition::TestORCCreateOCREngineL
// -----------------------------------------------------------------------------
TInt CTestSDKOCRecognition::TestORCCreateOCREngineL( CStifItemParser& /*aItem*/ )
    {
    TTestObserver observer;
    TOcrEngineEnv env;
    env.iPriority = EPriorityNormal;
    env.iMaxHeapSize = 1200 * KMinHeapGrowBy + 1;
    MOCREngineInterface* interface = NULL;
    TInt err = 0;
    TRAP( err, interface = OCREngineFactory::CreateOCREngineL( observer, env, 
            OCREngineFactory::EEngineLayoutRecognize ) );
    OCREngineFactory::ReleaseOCREngine( interface );
    if ( err != 0 )
        {
        err = 0;
        }
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestSDKOCRecognition::TestORCReleaseOCREngineL
// -----------------------------------------------------------------------------
TInt CTestSDKOCRecognition::TestORCReleaseOCREngineL( CStifItemParser& aItem )
    {
    return TestORCCreateOCREngineL( aItem );
    
    }

//  [End of File]
