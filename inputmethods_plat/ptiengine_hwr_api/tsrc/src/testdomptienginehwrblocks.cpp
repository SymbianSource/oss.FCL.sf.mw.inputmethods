/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               test PtiHwrRecognizer.h
*
*/




// [INCLUDE FILES]
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include <PtiHwrRecognizer.h>
#include <PtiDefs.h>

#include "testdomptienginehwr.h"
#include "testdomrecognizer.h"

const TUid KUid =  {0xE00040a9};

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestDOMPtiengineHWR::RunMethodL( CStifItemParser& aItem ) 
    {
    static TStifFunctionInfo const KFunctions[] =
        {  
        ENTRY( "TestHRSetRangeL", CTestDOMPtiengineHWR::TestHRSetRangeL ),
        ENTRY( "TestHRGetRangeL", CTestDOMPtiengineHWR::TestHRGetRangeL ),
        ENTRY( "TestHRAddAuxiliaryRangeL", CTestDOMPtiengineHWR::TestHRAddAuxiliaryRangeL ),
        ENTRY( "TestHRRemoveAllAuxRangeL", CTestDOMPtiengineHWR::TestHRRemoveAllAuxRangeL ),
        ENTRY( "TestHRRemoveAuxiliaryRangeL", CTestDOMPtiengineHWR::TestHRRemoveAuxiliaryRangeL ),
        ENTRY( "TestHRSetCandidateNumL", CTestDOMPtiengineHWR::TestHRSetCandidateNumL ),
        ENTRY( "TestHRGetCandidateNumL", CTestDOMPtiengineHWR::TestHRGetCandidateNumL ),
        ENTRY( "TestHRStrokeEndMarkL", CTestDOMPtiengineHWR::TestHRStrokeEndMarkL ),
        ENTRY( "TestHRRecognizeL", CTestDOMPtiengineHWR::TestHRRecognizeL ),
        ENTRY( "TestHRSetInputAreaSizeL", CTestDOMPtiengineHWR::TestHRSetInputAreaSizeL ),
        ENTRY( "TestHRGetEngineVersionL", CTestDOMPtiengineHWR::TestHRGetEngineVersionL ),
        ENTRY( "TestHRGetDictVersionL", CTestDOMPtiengineHWR::TestHRGetDictVersionL ),
        ENTRY( "TestHREnableWordRecognitionL", CTestDOMPtiengineHWR::TestHREnableWordRecognitionL ),
        ENTRY( "TestHRSetFirstLetterOrderL", CTestDOMPtiengineHWR::TestHRSetFirstLetterOrderL ),
        ENTRY( "TestHRSetNumberModeL", CTestDOMPtiengineHWR::TestHRSetNumberModeL ),
        ENTRY( "TestHRGetTopGuideLinePosL", CTestDOMPtiengineHWR::TestHRGetTopGuideLinePosL ),
        ENTRY( "TestHRGetBottomGuideLinePosL", CTestDOMPtiengineHWR::TestHRGetBottomGuideLinePosL ),
        ENTRY( "TestHRSetPermittedSymbolSetL", CTestDOMPtiengineHWR::TestHRSetPermittedSymbolSetL ),
        ENTRY( "TestHRRecognizeWithCharSetL", CTestDOMPtiengineHWR::TestHRRecognizeWithCharSetL ),
        ENTRY( "TestHRSetAuxCandidateNumL", CTestDOMPtiengineHWR::TestHRSetAuxCandidateNumL ),
        ENTRY( "TestHRGetAuxCandidateNumL", CTestDOMPtiengineHWR::TestHRGetAuxCandidateNumL ),
        ENTRY( "TestHRQueryUdmInterfaceL", CTestDOMPtiengineHWR::TestHRQueryUdmInterfaceL ),
        ENTRY( "TestHRMcrBeginSessionL", CTestDOMPtiengineHWR::TestHRMcrBeginSessionL ),
        ENTRY( "TestHRMcrAddStrokesL", CTestDOMPtiengineHWR::TestHRMcrAddStrokesL ),
        ENTRY( "TestHRMcrEndSessionL", CTestDOMPtiengineHWR::TestHRMcrEndSessionL ),
        ENTRY( "TestHRSymbolModelValidL", CTestDOMPtiengineHWR::TestHRSymbolModelValidL ),
        ENTRY( "TestHRIsSupportUdmL", CTestDOMPtiengineHWR::TestHRIsSupportUdmL ),
        ENTRY( "TestHRLoadUdmL", CTestDOMPtiengineHWR::TestHRLoadUdmL ),
        ENTRY( "TestHRUnLoadUdmL", CTestDOMPtiengineHWR::TestHRUnLoadUdmL ),
        ENTRY( "TestHRGetModelIndexListL", CTestDOMPtiengineHWR::TestHRGetModelIndexListL ),
        ENTRY( "TestHRGetSymbolModelL", CTestDOMPtiengineHWR::TestHRGetSymbolModelL ),
        ENTRY( "TestHRNewL", CTestDOMPtiengineHWR::TestHRNewL ),
        ENTRY( "TestHRStaticFuncL", CTestDOMPtiengineHWR::TestHRStaticFuncL ),
        };

    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// ============================ MEMBER FUNCTIONS ===============================
// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRNewL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRNewL( CStifItemParser& /*aItem*/ )
    {
    TUid uid;
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( ELangTest, uid );
    CleanupStack::PushL( recognize );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRStaticFuncL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRStaticFuncL( CStifItemParser& /*aItem*/ )
    {
    RArray<TUid> uidArray;
    RArray<TInt> langArray;
    CHwrRecognizer::ListAvailableLanguagesL( langArray );
    if ( langArray.Count() <= 0 )
        {
        return -1;
        }
    CHwrRecognizer::ListImplementationsL( ELangTest, uidArray );
    if ( uidArray.Count() <= 0 )
        {
        return -1;
        }
    langArray.Reset();
    CHwrRecognizer::ListGivenAvailableLanguagesL( uidArray[0], langArray );

    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRSetRangeL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRSetRangeL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );
    
    TRecognitionRange range;
    range.iLanguage = ELangTest;
    recognize->SetRange( range );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRGetRangeL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRGetRangeL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );
    
    TRecognitionRange ra = recognize->GetRange();
    STIF_ASSERT_TRUE( ra.iLanguage == ELangNone );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRAddAuxiliaryRangeL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRAddAuxiliaryRangeL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );
    
    TRecognitionRange ra;
    ra.iLanguage = ELangEnglish;
    recognize->AddAuxiliaryRange( ra );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRRemoveAllAuxRangeL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRRemoveAllAuxRangeL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    recognize->RemoveAllAuxRange();
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRRemoveAuxiliaryRangeL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRRemoveAuxiliaryRangeL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    recognize->RemoveAuxiliaryRange( 0 );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRSetCandidateNumL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRSetCandidateNumL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    recognize->SetCandidateNum( 0 );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRGetCandidateNumL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRGetCandidateNumL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    recognize->GetCandidateNum();
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRStrokeEndMarkL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRStrokeEndMarkL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    const TInt num = -65536;
    TPoint point = TPoint( num, num );
    STIF_ASSERT_TRUE( recognize->StrokeEndMark() == point );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRRecognizeL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRRecognizeL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    RArray<TPoint> pointArray;
    CleanupClosePushL( pointArray );
    RPointerArray<HBufC> bufArray;
    CleanupClosePushL( bufArray );
    
    recognize->Recognize( pointArray, bufArray );
    
    CleanupStack::PopAndDestroy( &bufArray );
    CleanupStack::PopAndDestroy( &pointArray );
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRSetInputAreaSizeL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRSetInputAreaSizeL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    TSize size( 100, 100 );
    recognize->SetInputAreaSize( size );
    
    recognize->SetScreenSize( size );
    recognize->EnableGuideline( EFalse );
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRGetEngineVersionL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRGetEngineVersionL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    TFileName buf;
    recognize->GetEngineVersion( buf );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRGetDictVersionL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRGetDictVersionL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    TFileName buf;
    recognize->GetDictVersion( buf );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHREnableWordRecognitionL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHREnableWordRecognitionL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    recognize->EnableWordRecognition( ETrue );
    recognize->EnableRecognitionDictionary( ETrue );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRSetFirstLetterOrderL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRSetFirstLetterOrderL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    recognize->SetFirstLetterOrder( EUpperFirst );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRSetNumberModeL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRSetNumberModeL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    recognize->SetNumberMode( EAknEditorStandardNumberModeKeymap );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRGetTopGuideLinePosL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRGetTopGuideLinePosL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    TInt pos;
    recognize->GetTopGuideLinePos( pos );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRGetBottomGuideLinePosL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRGetBottomGuideLinePosL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    TInt pos;
    recognize->GetBottomGuideLinePos( pos );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRSetPermittedSymbolSetL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRSetPermittedSymbolSetL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    TFileName buf;
    recognize->SetPermittedSymbolSet( buf );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRRecognizeWithCharSetL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRRecognizeWithCharSetL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    RArray<TPoint> pointArray;
    CleanupClosePushL( pointArray );
    RPointerArray<HBufC> bufArray;
    CleanupClosePushL( bufArray );
    TFileName buf;
    recognize->RecognizeWithCharSet( pointArray, bufArray, buf );
    
    CleanupStack::PopAndDestroy( &bufArray );
    CleanupStack::PopAndDestroy( &pointArray );
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRSetAuxCandidateNumL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRSetAuxCandidateNumL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    TInt num = 0;
    recognize->SetAuxCandidateNum( num );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRGetAuxCandidateNumL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRGetAuxCandidateNumL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    TInt num = recognize->GetAuxCandidateNum();
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRQueryUdmInterfaceL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRQueryUdmInterfaceL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    TInt err = 0;
    TRAP( err, MTruiEngine* engine = recognize->QueryUdmInterfaceL() );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRMcrBeginSessionL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRMcrBeginSessionL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    recognize->McrBeginSessionL();
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRMcrAddStrokesL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRMcrAddStrokesL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    RArray<TPoint> pointArray;
    CleanupClosePushL( pointArray );
    RPointerArray<HBufC> bufArray;
    CleanupClosePushL( bufArray );
    
    recognize->McrAddStrokesL( pointArray, bufArray );
    
    CleanupStack::PopAndDestroy( &bufArray );
    CleanupStack::PopAndDestroy( &pointArray );
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRMcrEndSessionL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRMcrEndSessionL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    RPointerArray<HBufC> bufArray;
    CleanupClosePushL( bufArray );
    
    recognize->McrEndSessionL( bufArray );
    
    CleanupStack::PopAndDestroy( &bufArray );
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRSymbolModelValidL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRSymbolModelValidL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CHwrRecognizer* recognize = CHwrRecognizer::NewL( uid.iUid );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    TFileName symbolName;
    RArray<TPoint> pointArray;
    CleanupClosePushL( pointArray );
    THwrUdmRange scriptType;
    scriptType.iRange = 0;
    scriptType.iScript = 0;
    TFileName similarSymbol;
    
    recognize->SymbolModelValid( symbolName, pointArray, scriptType, similarSymbol );
    
    CleanupStack::PopAndDestroy( &pointArray );
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRIsSupportUdmL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRIsSupportUdmL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CTestDomHwrRecognizer* recognize = static_cast<CTestDomHwrRecognizer*>( CHwrRecognizer::NewL( uid.iUid ) );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );
    
    STIF_ASSERT_FALSE( recognize->IsSupportUdm() );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRLoadUdmL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRLoadUdmL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CTestDomHwrRecognizer* recognize = static_cast<CTestDomHwrRecognizer*>( CHwrRecognizer::NewL( uid.iUid ) );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );
    
    recognize->LoadUdmL( EUdmSymbol );
    recognize->UnLoadUdmL( EUdmSymbol );
    
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRUnLoadUdmL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRUnLoadUdmL( CStifItemParser& aItem )
    {
    return TestHRLoadUdmL( aItem );
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRGetModelIndexListL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRGetModelIndexListL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CTestDomHwrRecognizer* recognize = static_cast<CTestDomHwrRecognizer*>( CHwrRecognizer::NewL( uid.iUid ) );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );

    RArray<TInt> array;
    CleanupClosePushL( array );
    THwrUdmRange scriptType;
    scriptType.iScript = 0;
    scriptType.iRange = 0;
    
    recognize->LoadUdmL( EUdmSymbol );
    recognize->UnLoadUdmL( EUdmSymbol );
    recognize->GetModelIndexListL( EUdmSymbol, array, scriptType );
    
    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestDOMPtiengineHWR::TestHRGetSymbolModelL
// -----------------------------------------------------------------------------
TInt CTestDOMPtiengineHWR::TestHRGetSymbolModelL( CStifItemParser& /*aItem*/ )
    {
    TUid uid( KUid );
    CTestDomHwrRecognizer* recognize = static_cast<CTestDomHwrRecognizer*>( CHwrRecognizer::NewL( uid.iUid ) );
    CleanupStack::PushL( recognize );
    STIF_ASSERT_NOT_NULL( recognize );
    
    RArray<TPoint> array;
    CleanupClosePushL( array );
    TInt helplLine = 0;
    TInt baseLine = 0;
    TPtrC buf;
    
    recognize->LoadUdmL( EUdmSymbol );
    recognize->GetSymbolModelL( EUdmSymbol, 0, buf, array, helplLine, baseLine );
    
    CleanupStack::PopAndDestroy( &array );
    CleanupStack::PopAndDestroy( recognize );
    
    return KErrNone;
    
    }

//  [End of File]


