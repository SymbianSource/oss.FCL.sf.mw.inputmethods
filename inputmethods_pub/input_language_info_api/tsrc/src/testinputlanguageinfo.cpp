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
#include "testpubinputlanguageinfo.h"

// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestTAknInputLanguageCapabilitiesL
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::TestTAknInputLanguageCapabilitiesL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestTAknInputLanguageCapabilitiesL, "In TestTAknInputLanguageCapabilities" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestTAknInputLanguageCapabilitiesL );
    // Print to log file
    iLog->Log( KTestTAknInputLanguageCapabilitiesL );
    
    TAknInputLanguageCapabilities* inputLanguageCapa = new TAknInputLanguageCapabilities();
    CleanupStack::PushL( inputLanguageCapa );
    STIF_ASSERT_NOT_NULL( inputLanguageCapa );
    CleanupStack::PopAndDestroy( inputLanguageCapa );
    
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestAssignCapabilityL
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::TestAssignCapabilityL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestAssignCapabilityL, "In TestAssignCapabilityL" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestAssignCapabilityL );
    // Print to log file
    iLog->Log( KTestAssignCapabilityL );
    
    TAknInputLanguageCapabilities* inputLanguageCapa = new TAknInputLanguageCapabilities();
    CleanupStack::PushL( inputLanguageCapa );
    STIF_ASSERT_NOT_NULL( inputLanguageCapa );
    inputLanguageCapa->AssignCapability( 0, ETrue );
    inputLanguageCapa->AssignCapability( 0, EFalse );
    CleanupStack::PopAndDestroy( inputLanguageCapa );
    
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestHasCapabilityL
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::TestHasCapabilityL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestHasCapabilityL, "In TestHasCapability" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestHasCapabilityL );
    // Print to log file
    iLog->Log( KTestHasCapabilityL );
    
    TAknInputLanguageCapabilities* inputLanguageCapa = new TAknInputLanguageCapabilities();
    CleanupStack::PushL( inputLanguageCapa );
    STIF_ASSERT_NOT_NULL( inputLanguageCapa );
    inputLanguageCapa->HasCapability( 0 );
    CleanupStack::PopAndDestroy( inputLanguageCapa );
    
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestSetAllCapabilitiesL
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::TestSetAllCapabilitiesL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestSetAllCapabilitiesL, "In TestSetAllCapabilitiesL" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestSetAllCapabilitiesL );
    // Print to log file
    iLog->Log( KTestSetAllCapabilitiesL );
    
    TAknInputLanguageCapabilities* inputLanguageCapa = new TAknInputLanguageCapabilities();
    CleanupStack::PushL( inputLanguageCapa );
    STIF_ASSERT_NOT_NULL( inputLanguageCapa );
    inputLanguageCapa->SetAllCapabilities();
    CleanupStack::PopAndDestroy( inputLanguageCapa );
    
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestHasAnySupportL
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::TestHasAnySupportL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestHasAnySupportL, "In TestHasAnySupportL" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestHasAnySupportL );
    // Print to log file
    iLog->Log( KTestHasAnySupportL );
    
    TAknInputLanguageCapabilities* inputLanguageCapa = new TAknInputLanguageCapabilities();
    CleanupStack::PushL( inputLanguageCapa );
    STIF_ASSERT_NOT_NULL( inputLanguageCapa );
    inputLanguageCapa->HasAnySupport();
    CleanupStack::PopAndDestroy( inputLanguageCapa );
    
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestCAknInputLanguageListL
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::TestCAknInputLanguageListL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestCAknInputLanguageListL, "In TestCAknInputLanguageList" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestCAknInputLanguageListL );
    // Print to log file
    iLog->Log( KTestCAknInputLanguageListL );
    
    CAknInputLanguageList* inputLanguageList = new CAknInputLanguageList( 1 );
    CleanupStack::PushL( inputLanguageList );
    STIF_ASSERT_NOT_NULL( inputLanguageList );
    CleanupStack::PopAndDestroy( inputLanguageList );
    
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestMdcaCountL
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::TestMdcaCountL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestMdcaCountL, "In TestMdcaCountL" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestMdcaCountL );
    // Print to log file
    iLog->Log( KTestMdcaCountL );
    
    CAknInputLanguageList* inputLanguageList = new CAknInputLanguageList( 1 );
    CleanupStack::PushL( inputLanguageList );
    STIF_ASSERT_NOT_NULL( inputLanguageList );
    inputLanguageList->MdcaCount();
    CleanupStack::PopAndDestroy( inputLanguageList );
    
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestMdcaPointL
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::TestMdcaPointL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestMdcaPointL, "In TestMdcaPointL" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestMdcaPointL );
    // Print to log file
    iLog->Log( KTestMdcaPointL );
    
    CAknInputLanguageList* inputLanguageList = new CAknInputLanguageList( 1 );
    CleanupStack::PushL( inputLanguageList );
    STIF_ASSERT_NOT_NULL( inputLanguageList );
    CAknInputLanguageInfo* inputLanguageInfo = AknInputLanguageInfoFactory::CreateInputLanguageInfoL();
    CleanupStack::PushL( inputLanguageInfo );
    STIF_ASSERT_NOT_NULL( inputLanguageInfo );
    inputLanguageInfo->AppendAvailableLanguagesL( inputLanguageList );
    inputLanguageList->MdcaPoint( 0 );
    CleanupStack::PopAndDestroy( inputLanguageInfo );
    CleanupStack::PopAndDestroy( inputLanguageList );
    
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestCreateInputLanguageInfoLL
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::TestCreateInputLanguageInfoLL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestCreateInputLanguageInfoLL, "In TestCreateInputLanguageInfoLL" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestCreateInputLanguageInfoLL );
    // Print to log file
    iLog->Log( KTestCreateInputLanguageInfoLL );
    
    CAknInputLanguageInfo* inputLanguageInfo = AknInputLanguageInfoFactory::CreateInputLanguageInfoL();
    CleanupStack::PushL( inputLanguageInfo );
    STIF_ASSERT_NOT_NULL( inputLanguageInfo );
    CleanupStack::PopAndDestroy( inputLanguageInfo );
    
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestLanguageCodeL
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::TestLanguageCodeL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestLanguageCodeL, "In TestLanguageCodeL" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestLanguageCodeL );
    // Print to log file
    iLog->Log( KTestLanguageCodeL );
    
    CAknInputLanguageList* inputLanguageList = new CAknInputLanguageList( 1 );
    CleanupStack::PushL( inputLanguageList );
    STIF_ASSERT_NOT_NULL( inputLanguageList );
    CAknInputLanguageInfo* inputLanguageInfo = AknInputLanguageInfoFactory::CreateInputLanguageInfoL();
    CleanupStack::PushL( inputLanguageInfo );
    STIF_ASSERT_NOT_NULL( inputLanguageInfo );
    inputLanguageInfo->AppendAvailableLanguagesL( inputLanguageList );
    CAknInputLanguageItem* item = inputLanguageList->At( 1 );
    item->LanguageCode();
    CleanupStack::PopAndDestroy( inputLanguageInfo );
    CleanupStack::PopAndDestroy( inputLanguageList );
    
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestLanguageNameL
// -----------------------------------------------------------------------------
//
TInt CTestPubInputLanguageInfo::TestLanguageNameL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestLanguageNameL, "In TestLanguageNameL" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestLanguageNameL );
    // Print to log file
    iLog->Log( KTestLanguageNameL );
    
    CAknInputLanguageList* inputLanguageList = new CAknInputLanguageList( 1 );
    CleanupStack::PushL( inputLanguageList );
    STIF_ASSERT_NOT_NULL( inputLanguageList );
    CAknInputLanguageInfo* inputLanguageInfo = AknInputLanguageInfoFactory::CreateInputLanguageInfoL();
    CleanupStack::PushL( inputLanguageInfo );
    STIF_ASSERT_NOT_NULL( inputLanguageInfo );
    inputLanguageInfo->AppendAvailableLanguagesL( inputLanguageList );
    CAknInputLanguageItem* item = inputLanguageList->At( 1 );
    item->LanguageName();
    CleanupStack::PopAndDestroy( inputLanguageInfo );
    CleanupStack::PopAndDestroy( inputLanguageList );
    
    return KErrNone;
    }
// -----------------------------------------------------------------------------
// CTestUiAifCoreToolkit::TestCapabilitiesL
// -----------------------------------------------------------------------------
//    
TInt CTestPubInputLanguageInfo::TestCapabilitiesL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( Ktestinputlanguageinfo, "TestInputLanguageInfo" );
    _LIT( KTestCapabilitiesL, "In TestCapabilitiesL" );
    TestModuleIf().Printf( 0, Ktestinputlanguageinfo, KTestCapabilitiesL );
    // Print to log file
    iLog->Log( KTestCapabilitiesL );
    
    CAknInputLanguageList* inputLanguageList = new CAknInputLanguageList( 1 );
    CleanupStack::PushL( inputLanguageList );
    STIF_ASSERT_NOT_NULL( inputLanguageList );
    CAknInputLanguageInfo* inputLanguageInfo = AknInputLanguageInfoFactory::CreateInputLanguageInfoL();
    CleanupStack::PushL( inputLanguageInfo );
    STIF_ASSERT_NOT_NULL( inputLanguageInfo );
    inputLanguageInfo->AppendAvailableLanguagesL( inputLanguageList );
    CAknInputLanguageItem* item = inputLanguageList->At( 1 );
    item->Capabilities();
    delete item;
    CleanupStack::PopAndDestroy( inputLanguageInfo );
    CleanupStack::PopAndDestroy( inputLanguageList );
    
    return KErrNone;
    }
// End of file
