/*
* Copyright (c) 2005-2010 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation of HWR engine
*
*/


//FEP INCLUDES
#include <aknfepglobalenums.h>
#include <aknfeppeninputenums.h>
#include <ptihwrrecognizer.h>
#include <e32property.h>
#include "ptidefs.h"

//USER INCLUDES
#include "peninputfingerhwrarengine.h"
#include "peninputfingerhwrardatastore.h"

// CONSTANT DEFINITION HEADER
#include "peninputfingerhwrarstoreconstants.h"

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArEngine* CPeninputFingerHwrArEngine::NewL( CPtiEngine* aPtiEngine, 
    CPeninputFingerHwrArDataStore* aOwner )
    {
    CPeninputFingerHwrArEngine* self = new ( ELeave ) CPeninputFingerHwrArEngine();
    
    CleanupStack::PushL( self );
    self->ConstructL( aPtiEngine, aOwner );
    CleanupStack::Pop( self );//self

    return self;
    }

// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArEngine::~CPeninputFingerHwrArEngine()
    {
    if( iOwnPtiEngine )
        {
        delete iPtiEngine;
        }
    delete iCustomKeymap;
    
    delete iIdle;
    
    iNeedPermittedRanges.Close();
    }

// ---------------------------------------------------------------------------
// Do recoginize by engine
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::DoRecognizeL( const RArray<TPoint>& aTraceData, 
    RPointerArray<HBufC>& aCandidates )
    {
    if ( !iRecognizer)
        {
        DoIdleConstructL();
        }
        
    TPoint ctrlStrokeEndMark = iOwner->StrokeEndMarkFromControl();
        
    if ( ctrlStrokeEndMark != iRecognizer->StrokeEndMark() )
        {
        ConvertStrokeEndMark( CONST_CAST( RArray<TPoint>&, aTraceData ), 
            iOwner->StrokeEndMarkFromControl(), 
            iRecognizer->StrokeEndMark() );
        iOwner->SetStrokeEndMark(); // make control's stroke end mark info same to engine
        }
    
    aCandidates.ResetAndDestroy();

    iRecognizer->Recognize( aTraceData, aCandidates ); 
    }
    

 
// ---------------------------------------------------------------------------
// Set primary and auxiliary ranges for hwr engine
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::SetRanges( const RArray<TInt>& aPermittedRanges )
    {
    TRAP_IGNORE( SetRangesL( aPermittedRanges ) );
    }
    
// ---------------------------------------------------------------------------
// Set primary and auxiliary ranges for hwr engine
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::SetRangesL( const RArray<TInt>& aPermittedRanges )
    {
   	ASSERT( aPermittedRanges.Count() > 0 );

    if ( !iRecognizer )
        {
    	iNeedPermittedRanges.Reset();
    	
    	for ( TInt i = 0; i < aPermittedRanges.Count(); i++ )
    	    {
            iNeedPermittedRanges.Append( aPermittedRanges[i] );
    	    }
    	
    	iNeedSetRange = ETrue;
    	return;    
        }
    else 
    	{
        iPremaryRange = aPermittedRanges[0];
        iRangeCount = aPermittedRanges.Count();

        SetCandidatesMaxCount( KCandidateCount );


        TRecognitionRange range;
        SetRecognitionRange( aPermittedRanges[0], range );
        
    
       
        iRecognizer->SetRange( range );
        SetCase( iCase );
    	}
    }

    
// ---------------------------------------------------------------------------------------------
// Set case
// ---------------------------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::SetCase( const TInt aCase )
    {
    iCase = aCase;
    
    if ( !iRecognizer )
        {
    	iNeedSetCase = ETrue;
        }
    else
        {
        // set letter to lower first when LowerCase
	    // set letter to upper first when UpperCase and TextCase
        if ( aCase == ECaseLower )
            {
            iRecognizer->SetFirstLetterOrder( ELowerFirst );
            }
        else
            {
            iRecognizer->SetFirstLetterOrder( EUpperFirst );
            }
        }
    }

// ---------------------------------------------------------------------------------------------
// Set number mode for hwr engine
// ---------------------------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::SetNumberMode( const TAknEditorNumericKeymap& aNumberMode )
    {
    iNumberMode = aNumberMode;
    
    if ( !iRecognizer )
        {
        iNeedSetNumberMode = ETrue;
        }
    else
        {
        iRecognizer->SetNumberMode( aNumberMode );
        if( aNumberMode !=  EKeymapFromResource )
            {
            ResetCustomKeyMap();
            }
        }
    }

// ---------------------------------------------------------------------------------------------
// Get stroke end mark from hwr engine
// ---------------------------------------------------------------------------------------------
//
TPoint CPeninputFingerHwrArEngine::StrokeEndMark() const
    {
	TPoint strokeEndMark( KDefaultStrokeEndMarkX, KDefaultStrokeEndMarkY );
	
    if ( iRecognizer )
        {
        strokeEndMark = iRecognizer->StrokeEndMark();
        }
    
	return strokeEndMark;
    }

// ---------------------------------------------------------------------------------------------
// Set primary candidate num for hwr engine
// ---------------------------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArEngine::SetPrimaryCandidateNum( const TInt aNum )
    {
    if ( iRecognizer )
        {
        return iRecognizer->SetCandidateNum( aNum );
        }
    else
        {
        return KErrGeneral;
        }
    }
        
// ---------------------------------------------------------------------------------------------
// Set total candidate num that should be shown
// ---------------------------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::SetCandidatesMaxCount( const TInt aCount )
    {
    iTotalCandidateNum = aCount;
    }
    
// ---------------------------------------------------------------------------------------------
// Set language
// ---------------------------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::SetLanguageL( const TInt aLanguage )
    {
    if ( ( iLanguage == aLanguage ) ||
         ( aLanguage != ELangArabic ) )
        {
        return;
        }
        
    iLanguage = aLanguage;
        

	iRecognizer = NULL;
	
	if( !iIdle->IsActive() )
	    {
	    iIdle->Start( TCallBack( BackgroundTaskL, this ) );
	    }
    }

// ---------------------------------------------------------------------------
// Set recognition range for hwr engine
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::SetCustomKeymapL( const TDesC& aKeyMap )
    {
    ResetCustomKeyMap();
    
    iCustomKeymap = HBufC::NewL( aKeyMap.Length() + KNumberString().Length() );
    iCustomKeymap->Des().Copy( KNumberString() );
    iCustomKeymap->Des().Append( aKeyMap );
    }

// ---------------------------------------------------------------------------
// Set recognition range for hwr engine
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::ResetCustomKeyMap()
    {
    delete iCustomKeymap;
    iCustomKeymap = NULL;
    }

// ---------------------------------------------------------------------------------------------
// CPeninputFingerHwrArEngine::BackgroundConstructL
// Do background construct.
// ---------------------------------------------------------------------------------------------
//
TBool CPeninputFingerHwrArEngine::BackgroundTaskL( TAny* aPtr )
    {
    CPeninputFingerHwrArEngine* self = static_cast<CPeninputFingerHwrArEngine*>( aPtr );
    self->DoIdleConstructL();
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// Set hand writing area size
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArEngine::SetInputAreaSize(TSize& aSize)
    {
    return iRecognizer ? iRecognizer->SetInputAreaSize(aSize) : KErrNotFound;     
    }

// ---------------------------------------------------------------------------
// Set hand writing screen size
// ---------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArEngine::SetScreenSize(TSize& aSize)    
    {
    return iRecognizer ? iRecognizer->SetScreenSize(aSize) : KErrNotFound;
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CPeninputFingerHwrArEngine::CPeninputFingerHwrArEngine()
    {
    iNeedSetNumberMode = EFalse;
    iNeedSetCase = EFalse;
    iNeedSetRange = EFalse;
    iRecognizer = NULL;    
    iOwnPtiEngine = EFalse;
    }

// ---------------------------------------------------------------------------
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::ConstructL( CPtiEngine* aPtiEngine, 
    CPeninputFingerHwrArDataStore* aOwner )
    {
    if( !aPtiEngine )
        {
        iPtiEngine = CPtiEngine::NewL( ETrue );
        iOwnPtiEngine = ETrue;
        }
    else
    	{
    	iPtiEngine = aPtiEngine;
    	}

    iIdle = CIdle::NewL( CActive::EPriorityIdle );
    iOwner = aOwner;
    }

// ---------------------------------------------------------------------------
// Set recognition range for hwr engine
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::SetRecognitionRange( const TInt aRange, 
    TRecognitionRange& aRecognitionRange )
    {
    aRecognitionRange.iLanguage = TLanguage( iLanguage );

    switch ( aRange )
        {
        case ERangeNative:
			{
			aRecognitionRange.iSubRange = EPtiRangeNative;
			}
            break;
        case ERangeEnglish:
            {
            aRecognitionRange.iLanguage = ELangEnglish;
            aRecognitionRange.iSubRange = EPtiRangeLatin;  
            }
            break;
        case ERangeNumber:
            {
            aRecognitionRange.iSubRange = EPtiRangeNumber;
            }
            break;
        default:
            break;
        }
    }
    
// ---------------------------------------------------------------------------------------------
// CPeninputFingerHwrArEngine::DoIdleConstructL
// Do background construct.
// ---------------------------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::DoIdleConstructL()
    {
    if ( iRecognizer )
        {
        return;
        }

	iRecognizer = iPtiEngine->GetHwrRecognizerL( TLanguage( iLanguage ) );
    
	iOwner->SetStrokeEndMark();
	SetPrimaryCandidateNum( KPremaryCandidateCount );
	
    if ( iNeedSetRange )
        {
        SetRanges( iNeedPermittedRanges );
        iNeedPermittedRanges.Reset();
        iNeedSetRange = EFalse;        
        }
    
    if ( iNeedSetCase )
        {
    	SetCase( iCase );
    	iNeedSetCase = EFalse;
        }
    
    if ( iNeedSetNumberMode )
        {
    	SetNumberMode( TAknEditorNumericKeymap( iNumberMode ) );
    	iNeedSetNumberMode = EFalse;
        }
    }

// ---------------------------------------------------------------------------
// Convert stroke end mark
// ---------------------------------------------------------------------------
//
void CPeninputFingerHwrArEngine::ConvertStrokeEndMark( RArray<TPoint>& aTraceData, 
    TPoint aPnt1, TPoint aPnt2 )
    {
    TInt count = aTraceData.Count();

    for ( TInt i = 0; i < count; i++ )
        {
    	if ( aTraceData[i] == aPnt1 )
    	    {
    		aTraceData.Remove( i );
    		aTraceData.Insert( aPnt2, i );
    	    }
        }
    }    
    
void CPeninputFingerHwrArEngine::GetTopGuideLinePos(TInt& aPos)
    {
    if(iRecognizer)
        {
        iRecognizer->GetTopGuideLinePos(aPos);
        }
    }

void CPeninputFingerHwrArEngine::GetBottomGuideLinePos(TInt& aPos)
    {
    if(iRecognizer)
        {
        iRecognizer->GetBottomGuideLinePos(aPos);
        }
    }
   
//End Of File
