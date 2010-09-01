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
* Description:  Implementation of HWR engine
*
*/


//FEP INCLUDES
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <PtiHwrRecognizer.h>

//USER INCLUDES
#include "peninputhwrengine.h"
#include "peninputhwrfscndatastore.h"

// CONSTANT DEFINITION HEADER
#include "peninputhwrfscnstoreconstants.h"

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
    CPeninputHwrfscnDataStore* aOwner )
    {
    CAknFepHwrEngine* self = new ( ELeave ) CAknFepHwrEngine();
    
    CleanupStack::PushL( self );
    self->ConstructL( aPtiEngine, aOwner );
    CleanupStack::Pop( self );//self

    return self;
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
// Second phase constructor
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::ConstructL( CPtiEngine* aPtiEngine, 
    CPeninputHwrfscnDataStore* aOwner )
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
// Do recoginize by engine
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::DoRecognizeL( const RArray<TPoint>& aTraceData, 
    RPointerArray<HBufC>& aCandidates )
    {
    if ( !iRecognizer )
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
    
    //Do recognition for number only    
    if ( iOwner->IsNumberOnly() )
        {     
        DoRecognizeOfNumberOnly( aTraceData,aCandidates );
        return;
        }

    TInt primaryCount = iRecognizer->Recognize( aTraceData, aCandidates ); 

    // filter recognized candidate, set start position for all ranges    
    TPtrC ptr( KSeparator );
    
    // remove uncessary aux candidate, including range separator 
    for ( TInt i=0; i<aCandidates.Count(); i++ )
        {
        if ( aCandidates[i]->Compare( KGestureBackspace ) == 0 )
        	{
        	// convert backspace returned by engine, to make sure it display correctly.
            *aCandidates[i] = KDisplayBackspace;
            break;
        	}
        }
    
    //reorder the secondary range
    ReorderSecondaryRange( aCandidates );
    
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
// Do recognition when the range is only number
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::DoRecognizeOfNumberOnly( const RArray<TPoint>& aTraceData, 
    RPointerArray<HBufC>& aCandidates )
    {
    if ( iNumberMode == EAknEditorPlainNumberModeKeymap )
        {
        TBuf<KPlainNumSize> buf;
        buf.Append( KNumberString() );
        buf.Append( KGestureBackspace );
        iRecognizer->RecognizeWithCharSet( aTraceData, aCandidates, buf );
        }
    else
        {
        iRecognizer->Recognize( aTraceData, aCandidates );
        }
        
    //find special char and do special handling
    TInt count = aCandidates.Count();
    for ( TInt i = count - 1; i >= 0; i-- )
        {
         //the space and enter are not allowed to be appeared in candidates
         if( ( *aCandidates[i] )[0] == KGestureSpace()[0] || 
             ( *aCandidates[i] )[0] == KGestureEnter()[0] )
             {
             delete aCandidates[i];
             aCandidates.Remove( i );
             }
         else if( aCandidates[i]->Compare( KGestureBackspace ) == 0 )
             {
             *aCandidates[i] = KDisplayBackspace; 
             }                    
        }
    
    if ( iCustomKeymap )
        {
        count = aCandidates.Count();
        for( TInt i = count - 1; i >= 0; i-- )
            {
            if( iCustomKeymap->Find( *aCandidates[i] ) == KErrNotFound )
                {
                delete aCandidates[i];
    	    	aCandidates.Remove( i );
                }
            }
        }
        
    // remove uncessary primary candidate
    count = aCandidates.Count() - iTotalCandidateNum;
    for ( TInt i = 0; i < count; i++ )
        {
        delete aCandidates[iTotalCandidateNum];
        aCandidates.Remove( iTotalCandidateNum );
        }
    }

// ---------------------------------------------------------------------------
// Reorder secondary range in candidate list
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::ReorderSecondaryRange( RPointerArray<HBufC>& aCandidates )
    {
    if( iRangeCount <= 1 )
        {
        return;
        }

    RPointerArray<HBufC> secondary;
    TInt auxCandidateNum = GetAuxCandidateNum();
    
    HBufC* secCandidate = NULL;          
    TInt candiateCount = aCandidates.Count();
   
    TInt primaryCountFromEng = 0;
    
    // Get all secondary candidates
    for( TInt i = 0; i < candiateCount; i++ )// "i" is the index of separater 
        {
        if( aCandidates[i]->CompareC( KSeparator ) == 0 )
            {
            if(primaryCountFromEng == 0)
                {
                // Save the primary candidates count from ptiengine
                primaryCountFromEng = i;
                }
            TInt auxCountFromEng = 0;
            while(i < candiateCount - 1 && aCandidates[++i]->CompareC( KSeparator ) != 0)
                {
                // calculate the secondary range candidate count
                auxCountFromEng++;
                
                // Copy all the secondary candidate to an array
                secCandidate = aCandidates[i]->Alloc();
                secondary.Append( secCandidate );
                }
            
            for(TInt j = 0; j < auxCandidateNum - auxCountFromEng ; j++ )
                {
                // supply empty secondary candidate when engine not return
                // enough cell
                secondary.Append( KNullDesC().Alloc() );
                }
            i--; // go back to the separator index
            }
        }

   
    // caculate the primary candidate count
    TInt primaryCount = 0;
    TInt rowCount = 0;
    
    if( iPremaryRange == ERangeNumber )
        {
        primaryCount = KCandidateCountNumMode - iRangeCount + 1;
        rowCount = 1;
        }
    else
        {
        primaryCount = KCandidateCountPerRow - iRangeCount + 1;
        rowCount = KCandidateRowCount;
        }

    // supply empty primary candidate when engine not return
    // enough cell
    if(primaryCountFromEng < primaryCount * rowCount)
        {
        for(TInt y = 0; y < primaryCount * rowCount - primaryCountFromEng; y++)
            {
            aCandidates.Insert( KNullDesC().Alloc(), primaryCountFromEng );
            }
        }   
    
    // insert secondary candidates into the end of each row
   	for( ; rowCount > 0; rowCount-- )
        {
        for( TInt m = 0; m < iRangeCount - 1; m++ )
            {
            TInt secondaryIndex = rowCount - 1 + m * auxCandidateNum;
            TInt insertPos = primaryCount * rowCount + m;
            aCandidates.Insert( secondary[secondaryIndex], insertPos );
            }
        }
    secondary.Reset();
    secondary.Close();
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
 
    
// ---------------------------------------------------------------------------
// Do homophonic using trigger string
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::DoHomophonicL( const TDesC& aTriggerStr, 
                                      RPointerArray<HBufC>& aHomophonic )
    {
    // homophonic only valid for PRC Chinese and Taiwan Chinese
    if ( ( iLanguage == ELangPrcChinese ) || 
        ( iLanguage == ELangTaiwanChinese ) )
        {
        iPtiEngine->SetCandidatePageLength( KMaxHomophonicCandidateCount );
        aHomophonic.ResetAndDestroy();
        
        // get the spelling of the character
        TBuf<KMaxSpellingsLettersNum> spelling;
        TPtiSpelling spellingType = EPtiPinyin;
        
        if ( iPtiEngine->InputMode() == EPtiEnginePinyinVkb )
            {
            spellingType = EPtiPinyin;
            }
        else if ( iPtiEngine->InputMode() == EPtiEngineZhuyinVkb )
            {
            spellingType = EPtiZhuyin;
            }
        
        TInt result = iPtiEngine->GetSpelling( aTriggerStr[0], 
            spelling, spellingType );    
        if( KErrNone != result )
            {
            return;
            }
            
        // get homophonic or polyphonic candidates by spelling    
        GetHomophonicAndPolyphonic( spelling, aHomophonic );  
           
        // delete the repeated characters                                        
        TInt count = aHomophonic.Count();
        for ( TInt i = count - 1; i >= 0; i-- )
            {
            if ( aTriggerStr == *( aHomophonic[i] ) )
                {
                delete aHomophonic[i];
                aHomophonic.Remove( i );
                }
            }
        // remove the seleted character to the first cell    
        aHomophonic.Insert( aTriggerStr.AllocL(), 0 );       
        }
    }

// ---------------------------------------------------------------------------
// Get homophonic or polyphonic candidates
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::GetHomophonicAndPolyphonic( TDes& aSpelling, 
    RPointerArray<HBufC>& aHomophonic )
    {
    TInt index = KErrNotFound;
    TBuf<KMaxOneSpellingLettersNum> tmpSpelling;
    
    while( aSpelling != KNullDesC16 )
        {
        tmpSpelling.Zero();    
		// find the separator
        index = aSpelling.Find( KPolyphonicSeparator );
        if( index != KErrNotFound )
            {
			// get one spelling in the polyphonic string
            tmpSpelling = aSpelling.Left( index );

			// remove the left spelling and a separator
            aSpelling = aSpelling.Right( aSpelling.Length() - index - 1 );
            }
        else // only one spelling
            {
            tmpSpelling = aSpelling;
            aSpelling = KNullDesC16;
            }
           
		// get candidate for each spelling
        RPointerArray<HBufC> candidates;
        HBufC* formatString = iPtiEngine->GetCandidatesByInputString( 
            tmpSpelling, 
            candidates, 
            EFalse );
                                                
        for( TInt i = 0; i < candidates.Count(); i++ )
            {
            aHomophonic.Append( candidates[i] );        
            }
        delete formatString;
        candidates.Reset();
        }
    }
 

// ---------------------------------------------------------------------------
// Set primary and auxiliary ranges for hwr engine
// ---------------------------------------------------------------------------
//
void CAknFepHwrEngine::SetRanges( const RArray<TInt>& aPermittedRanges )
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
        
        if( iPremaryRange == ERangeNumber )
            {
            SetCandidatesMaxCount( KCandidateCountNumMode );
            SetAuxCandidateNum( KAuxCandidateCountNumMode );
            }
        else
            {
            SetCandidatesMaxCount( KCandidateCount );
            SetAuxCandidateNum( KAuxCandidateCount );
            }

        TRecognitionRange range;

        SetRecognitionRange( aPermittedRanges[0], range );
        iRecognizer->SetRange( range );

        // set auxiliary ranges for hwr engine
        for ( TInt i=1; i<aPermittedRanges.Count(); i++ )
            {
            SetRecognitionRange( aPermittedRanges[i], range );

            iRecognizer->AddAuxiliaryRange( range );
            }
        
        SetCase( iCase );
    	}
    }

    
// ---------------------------------------------------------------------------------------------
// Set case
// ---------------------------------------------------------------------------------------------
//
void CAknFepHwrEngine::SetCase( TInt aCase )
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
TInt CAknFepHwrEngine::SetPrimaryCandidateNum( TInt aNum )
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
// Set aux candidate num that should be shown
// ---------------------------------------------------------------------------------------------
//
TInt CAknFepHwrEngine::SetAuxCandidateNum( TInt aNum )
    {
    if ( iRecognizer )
        {
        return iRecognizer->SetAuxCandidateNum( aNum );
        }
    else
        {
        return KErrGeneral;
        }
    }


// ---------------------------------------------------------------------------------------------
// Get aux candidate num that should be shown
// ---------------------------------------------------------------------------------------------
//
TInt CAknFepHwrEngine::GetAuxCandidateNum()
    {
    if ( iRecognizer )
        {
        return iRecognizer->GetAuxCandidateNum();
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
TInt CAknFepHwrEngine::SetCandidatesMaxCount( TInt aCount )
    {
    if ( aCount > 0 )
        {
        iTotalCandidateNum = aCount;
        return KErrNone;
        }
    else
        {
        return KErrGeneral;
        }    
    }
    
// ---------------------------------------------------------------------------------------------
// Set language
// ---------------------------------------------------------------------------------------------
//
void CAknFepHwrEngine::SetLanguageL( TInt aLanguage )
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
	SetAuxCandidateNum( KAuxCandidateCount );
	
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
            break;
			}
        case ERangeEnglish:
            aRecognitionRange.iLanguage = ELangEnglish;
            aRecognitionRange.iSubRange = EPtiRangeLatin;

            break;
        case ERangeNumber:
            aRecognitionRange.iSubRange = EPtiRangeNumber;

            break;
        case ERangeSymbol:
            // The symbol mode is associated with iLanguage instead of iPremaryRange
            aRecognitionRange.iLanguage = TLanguage( iLanguage );
            aRecognitionRange.iSubRange = EPtiRangeSymbol;

            break;
        default:
            break;
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
TInt CAknFepHwrEngine::SetInputAreaSize(TSize& aSize)
    {
    if(iRecognizer)
        {
        return iRecognizer->SetInputAreaSize(aSize);        
        }
    else
        {
        return KErrNotFound;
        } 
    }    
TInt CAknFepHwrEngine::SetScreenSize(TSize& aSize)    
    {
      if(iRecognizer)
        {
        return iRecognizer->SetScreenSize(aSize);        
        }
    else
        {
        return KErrNotFound;
        }  
    }    
//End Of File
