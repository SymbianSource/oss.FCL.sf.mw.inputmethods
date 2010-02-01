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
* Description:  test AknInputLanguageInfo.h
*
*/



// [INCLUDE FILES]
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>

#include "testpubinputlanguageinfo.h"

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestPubInputLanguageInfo::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::RunMethodL( CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
        		
        		// for AknInputLanguageInfo.h
        ENTRY( "TestTAknInputLanguageCapabilitiesL", CTestPubInputLanguageInfo::TestTAknInputLanguageCapabilitiesL ),
        ENTRY( "TestAssignCapabilityL", CTestPubInputLanguageInfo::TestAssignCapabilityL ),
        ENTRY( "TestHasCapabilityL", CTestPubInputLanguageInfo::TestHasCapabilityL ),
        ENTRY( "TestSetAllCapabilitiesL", CTestPubInputLanguageInfo::TestSetAllCapabilitiesL ),
        ENTRY( "TestHasAnySupportL", CTestPubInputLanguageInfo::TestHasAnySupportL ),
        ENTRY( "TestCAknInputLanguageListL", CTestPubInputLanguageInfo::TestCAknInputLanguageListL ),
        ENTRY( "TestMdcaCountL", CTestPubInputLanguageInfo::TestMdcaCountL ),
        ENTRY( "TestMdcaPointL", CTestPubInputLanguageInfo::TestMdcaPointL ),
        ENTRY( "TestCreateInputLanguageInfoLL", CTestPubInputLanguageInfo::TestCreateInputLanguageInfoLL ),
        ENTRY( "TestLanguageCodeL", CTestPubInputLanguageInfo::TestCreateInputLanguageInfoLL ),
        ENTRY( "TestLanguageNameL", CTestPubInputLanguageInfo::TestCreateInputLanguageInfoLL ),
        ENTRY( "TestCapabilitiesL", CTestPubInputLanguageInfo::TestCreateInputLanguageInfoLL ),

        // [test cases entries]

        };

    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

//  [End of File]
