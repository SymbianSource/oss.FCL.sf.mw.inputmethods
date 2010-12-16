/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation of HWR engine
*
*/

//FEP INCLUDES
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <PtiHwrRecognizer.h>
#include <e32property.h>
#include <AknFepInternalPSKeys.h>
#include <PtiDefs.h>

//USER INCLUDES
#include "peninputfingerhwrengine.h"
#include "peninputfingerhwrdatastore.h"

// CONSTANT DEFINITION HEADER
#include "peninputfingerhwrstoreconstants.h"

// ---------------------------------------------------------------------------
// SplitString()
// ---------------------------------------------------------------------------
//
static TInt SplitString( const TPtrC &aPtr, 
    TUint16 aSeperator, 
    RPointerArray<HBufC>& aStringArray )
	{
	// phrase input mode
    TInt start = 0;
    TInt length = 0;
	
    for ( TInt i = 0; i < aPtr.Length(); i++ )
        {
        if ( aPtr[i] == aSeperator )
            {
            TPtrC segment( aPtr.Ptr() + start, length );
            TRAP_IGNORE( aStringArray.AppendL( segment.AllocL() ) );
            start += ( length + 1 );
            length = 0;
            }
        else
            {
            length++;               
            }
        }

    if ( length )
        {
        TPtrC segm( aPtr.Ptr() + start, length );
        TRAP_IGNORE( aStringArray.AppendL( segm.AllocL() ) );
        }
        
    return aStringArray.Count();
	}

// ---------------------------------------------------------------------------
// Symbian constructor
// ---------------------------------------------------------------------------
//
CAknFepHwrEngine* CAknFepHwrEngine::NewL( CPtiEngine* aPtiEngine, 
    CPeninputFingerHwrDataStore* aOwner )
    {
    CAknFepHwrEngine* self = new ( ELeave ) CAknFepHwrEngine();
    
    CleanupStack::PushL( self );
    self->ConstructL( aPtiEngine, aOwner );
    CleanupStack::Pop( self );//self

    return self;
    }

// ---------------------------------------------------------------------------
// destructor
// ---------------------------------------------------------------------------
//
CAknFepHwrEngine::~CAknFepHwrEngine()
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
void CAknFepHwrEngine::DoRecognizeL( const RArray<TPoint>& aTraceData, 
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

    TInt primaryCount = iRecognizer->Recognize( aTraceData, aCandidates ); 

    // filter recognized candidate, set start position for all ranges    
    TPtrC ptr( KSeparator );
    
    // remove uncessary aux candidate, including range separator 
    for ( TInt i=0; i < aCandidates.Count(); i++ )
        {
        if ( aCandidates[i]->Compare( KGestureBackspace ) == 0 )
        	{
        	// convert backspace returned by engine, to make sure it display correctly.
            *aCandidates[i] = KDisplayBackspace;
            break;
        	}
        }
        
    // remove uncessary primary candidate
    TInt totallyCount = aCandidates.Count();
    TInt removePos = iTotalCandidateNum;        
    
    for ( TInt i = removePos; i < totallyCount; i++ )
         {
         delete aCandidates[removePos];
         aCandidates.Remove( removePos );
         } 
   
    // not allowing '-' to be the first char in chinese range 
    // not allowing '/' to be the first char in English and Number range 
    TPtrC dashPtr( KDash );
    TPtrC solidusPtr( KSolidus );
    if ( ( iPremaryRange == ERangeNative ) && 
        ( aCandidates[0]->Compare( dashPtr ) == 0 ) )
        {
        *aCandidates[0] = *aCandidates[1];
        *aCandidates[1] = dashPtr;
        }
    else if ( ( ( iPremaryRange == ERangeEnglish ) || 
        ( iPremaryRange == ERangeNumber ) ) 
        && ( aCandidates[0]->Compare( solidusPtr ) == 0 ) )
        {
        *aCandidates[0] = *aCandidates[1];
        *aCandidates[1] = solidusPtr;
        }
    }
    

// ---------------------------------------------------------------------------
// Do predictive using trigger string
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::DoPredictiveL( const TDesC& aTriggerStr, 
    RPointerArray<HBufC>& aPredictives,
    TBool aNextPage )
    {
    // predictive only valid for Chinese
    if ( ( iLanguage != ELangPrcChinese ) &&
        ( iLanguage != ELangTaiwanChinese ) && 
        ( iLanguage != ELangHongKongChinese ) )
    	{
    	return;    	
    	}

    // activate correct pti language according to given language
    if ( !aNextPage )
        {
    	iPtiEngine->SetCandidatePageLength( KPredictiveCountPerPage );
    	
    	aPredictives.ResetAndDestroy();
    	if( !iPtiEngine->SetPredictiveChineseChar( aTriggerStr ) )
    		{
    		return;
    		}
        }
    else
        {
        if ( !iPtiEngine->NextCandidatePage() )
            {
        	return;
            }
        }
        
    TPtrC ptr = iPtiEngine->CandidatePage();
    
    if ( ( iPtiEngine->InputMode() == EPtiEnginePinyinVkb ) || 
        ( iPtiEngine->InputMode() == EPtiEngineStrokeByPhrase ) ||
        ( iPtiEngine->InputMode() == EPtiEngineZhuyinVkb ) )
    	{
    	SplitString( ptr, KSegment, aPredictives );
    	}
    else
        {
        TInt predictiveCandidateNum = ptr.Length();

        for ( TInt i=0; i<predictiveCandidateNum; i++ )
            {
            aPredictives.Append( ptr.Mid( i,1 ).AllocL() );
            }
        }	
    }
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
// ---------------------------------------------------------------------------
// Do predictive for auto complete in English range
// ---------------------------------------------------------------------------
//
 void CAknFepHwrEngine::DoEngPredictiveL( const TDesC& aTriggerStr, 
    RPointerArray<HBufC>& aPredictives)
    {

    aPredictives.ResetAndDestroy();
    CDesCArray* cands = new (ELeave) CDesCArrayFlat(16);
    CleanupStack::PushL(cands);
    iPtiEngine->ClearCurrentWord();
	iPtiEngine->SetCurrentWord(aTriggerStr);
  
    // Get auto complete candidates
    iPtiEngine->GetCandidateListL(*cands);
    for ( TInt i = 1; i < cands->Count(); i++ )
        {
        // Don't show the first predictive
        aPredictives.Append( (*cands)[i].AllocL() );
        }
   
    CleanupStack::PopAndDestroy( cands ); //cands  	
    }
#endif
 
// ---------------------------------------------------------------------------
// Set primary and auxiliary ranges for hwr engine
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::SetRanges( const RArray<TInt>& aPermittedRanges )
    {
    TRAP_IGNORE( SetRangesL( aPermittedRanges ) );
    }
    
// ---------------------------------------------------------------------------
// Set primary and auxiliary ranges for hwr engine
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::SetRangesL( const RArray<TInt>& aPermittedRanges )
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
        
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
        if(iPremaryRange == ERangeEnglish)
            {
            iPtiEngine->ActivateLanguageL( ELangEnglish, EPtiEngineQwertyPredictive );
            iPtiEngine->HandleCommandL( EPtiCommandEnableAutoCompletion  );
            }
        else
            {
            iPtiEngine->HandleCommandL( EPtiCommandDisableAutoCompletion );
            
        	switch ( iLanguage )
                {
                case ELangPrcChinese:
                    {
                    if ( iPtiEngine->ActivateLanguageL( iLanguage, EPtiEnginePinyinVkb ) != KErrNone )
                    	{
                    	iPtiEngine->ActivateLanguageL( iLanguage, EPtiEnginePinyin );
                    	}
                    }
                    break;
                case ELangHongKongChinese:
                    {
                    if ( iPtiEngine->ActivateLanguageL( iLanguage, EPtiEngineStrokeByPhrase ) != KErrNone )
                    	{
                    	iPtiEngine->ActivateLanguageL( ELangHongKongChinese, EPtiEngineStroke );
                    	}
                    }
                    break;
                case ELangTaiwanChinese:
                    {
                    if ( iPtiEngine->ActivateLanguageL( iLanguage, EPtiEngineZhuyinVkb ) != KErrNone )
                    	{
                    	iPtiEngine->ActivateLanguageL( ELangTaiwanChinese, EPtiEngineZhuyin );
                    	}
                    }
                    break;
                default:
                    return;
                }            
            }         
#endif //RD_INTELLIGENT_TEXT_INPUT        
        
        iRecognizer->SetRange( range );
        SetCase( iCase );
    	}
    }

    
// ---------------------------------------------------------------------------------------------
// Set case
// ---------------------------------------------------------------------------------------------
//
void CAknFepHwrEngine::SetCase( const TInt aCase )
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
void CAknFepHwrEngine::SetNumberMode( const TAknEditorNumericKeymap& aNumberMode )
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
TPoint CAknFepHwrEngine::StrokeEndMark() const
    {
    if ( iRecognizer )
        {
        return iRecognizer->StrokeEndMark();
        }
    else
        {
        return TPoint( KDefaultStrokeEndMarkX, KDefaultStrokeEndMarkY );
        }
    }

// ---------------------------------------------------------------------------------------------
// Set primary candidate num for hwr engine
// ---------------------------------------------------------------------------------------------
//
TInt CAknFepHwrEngine::SetPrimaryCandidateNum( const TInt aNum )
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
void CAknFepHwrEngine::SetCandidatesMaxCount( const TInt aCount )
    {
    iTotalCandidateNum = aCount;
    }
    
// ---------------------------------------------------------------------------------------------
// Set language
// ---------------------------------------------------------------------------------------------
//
void CAknFepHwrEngine::SetLanguageL( const TInt aLanguage )
    {
    if ( ( iLanguage == aLanguage ) ||
         ( aLanguage != ELangPrcChinese && 
          aLanguage != ELangHongKongChinese && 
          aLanguage != ELangTaiwanChinese ) )
        {
        return;
        }
        
    iLanguage = aLanguage;
        
	switch ( iLanguage )
    {
    case ELangPrcChinese:
        {
        if ( iPtiEngine->ActivateLanguageL( iLanguage, EPtiEnginePinyinVkb ) != KErrNone )
        	{
        	iPtiEngine->ActivateLanguageL( iLanguage, EPtiEnginePinyin );
        	}
        }
        break;
    case ELangHongKongChinese:
        if ( iPtiEngine->ActivateLanguageL( iLanguage, EPtiEngineStrokeByPhrase ) != KErrNone )
        	{
        	iPtiEngine->ActivateLanguageL( ELangHongKongChinese, EPtiEngineStroke );
        	}
        break;
    case ELangTaiwanChinese:
        if ( iPtiEngine->ActivateLanguageL( iLanguage, EPtiEngineZhuyinVkb ) != KErrNone )
        	{
        	iPtiEngine->ActivateLanguageL( ELangTaiwanChinese, EPtiEngineZhuyin );
        	}
        break;
    default:
        return;
    }

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
void CAknFepHwrEngine::SetCustomKeymapL( const TDesC& aKeyMap )
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
void CAknFepHwrEngine::ResetCustomKeyMap()
    {
    delete iCustomKeymap;
    iCustomKeymap = NULL;
    }

// ---------------------------------------------------------------------------------------------
// CAknFepHwrEngine::BackgroundConstructL
// Do background construct.
// ---------------------------------------------------------------------------------------------
//
TBool CAknFepHwrEngine::BackgroundTaskL( TAny* aPtr )
    {
    CAknFepHwrEngine* self = static_cast<CAknFepHwrEngine*>( aPtr );
    self->DoIdleConstructL();
    return EFalse;
    }
    
// ---------------------------------------------------------------------------
// Set hand writing area size
// ---------------------------------------------------------------------------
//
TInt CAknFepHwrEngine::SetInputAreaSize(TSize& aSize)
    {
    return iRecognizer ? iRecognizer->SetInputAreaSize(aSize) : KErrNotFound;     
    }

// ---------------------------------------------------------------------------
// Set hand writing screen size
// ---------------------------------------------------------------------------
//
TInt CAknFepHwrEngine::SetScreenSize(TSize& aSize)    
    {
    return iRecognizer ? iRecognizer->SetScreenSize(aSize) : KErrNotFound;
    }

// ---------------------------------------------------------------------------
// C++ constructor
// ---------------------------------------------------------------------------
//
CAknFepHwrEngine::CAknFepHwrEngine()
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
void CAknFepHwrEngine::ConstructL( CPtiEngine* aPtiEngine, 
    CPeninputFingerHwrDataStore* aOwner )
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
void CAknFepHwrEngine::SetRecognitionRange( const TInt aRange, 
    TRecognitionRange& aRecognitionRange )
    {
    aRecognitionRange.iLanguage = TLanguage( iLanguage );

    switch ( aRange )
        {
        case ERangeNative:
			{
            if ( iLanguage == ELangPrcChinese )
                {
                aRecognitionRange.iSubRange = EPtiRangePRCChinese;
                }
            else if ( iLanguage == ELangHongKongChinese )
                {
                aRecognitionRange.iSubRange = EPtiRangeHKChinese;
                }
            else
            	{
                aRecognitionRange.iSubRange = EPtiRangeTWChinese;
                }
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
// CAknFepHwrEngine::DoIdleConstructL
// Do background construct.
// ---------------------------------------------------------------------------------------------
//
void CAknFepHwrEngine::DoIdleConstructL()
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
void CAknFepHwrEngine::ConvertStrokeEndMark( RArray<TPoint>& aTraceData, 
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

// ---------------------------------------------------------------------------
// Set Keyboard type to Qwerty
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::SetKeyboardToQwerty()
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT   
    RProperty::Set( KPSUidAknFep, KAknFepVirtualKeyboardType, EPtiKeyboardQwerty4x12 );
#endif
    }        
     
       
//End Of File
