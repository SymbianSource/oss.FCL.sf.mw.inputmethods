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
* Description:  Implementation for chinese peninput finger hwr
*
*/
//SYSTEM INCLUDE
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>

//FEP INCLUDE
#include <aknfeppeninputenums.h>
#include <AknFepGlobalEnums.h>
#include <AknFepInternalCRKeys.h>

//USER INCLUDE
#include "peninputfingerhwrlayout.h"
#include "peninputfingerhwrdatastore.h"
#include "peninputfingerhwrtriggerstr.h"
#include "peninputfingerhwrengine.h"
#include "peninputpluginutils.h"
#include "peninputfingerhwrstoreconstants.h"
#include <peninputfingerhwrcn.rsg>

//CONSTANT DEFINATION HEADER


// ----------------------------------------------------------------------------
// Symbian constructor
// ----------------------------------------------------------------------------
//
CPeninputFingerHwrDataStore* CPeninputFingerHwrDataStore::NewL( 
    CPtiEngine* aPtiEngine, CPeninputFingerHwrLayout* aLayout )
    {
    CPeninputFingerHwrDataStore* self = new ( ELeave ) 
        CPeninputFingerHwrDataStore(aLayout);
    
    CleanupStack::PushL( self );
    self->ConstructL( aPtiEngine );
    CleanupStack::Pop( self );//self

    return self;
    }

// ----------------------------------------------------------------------------
// destructor
// ----------------------------------------------------------------------------
//
CPeninputFingerHwrDataStore::~CPeninputFingerHwrDataStore()
    {
    iCandidates.ResetAndDestroy();
    iCandidates.Close();

    iPredictives.ResetAndDestroy();
    iPredictives.Close();
    
    iRangeStartPos.Close();
    iPermittedRanges.Close();
    iKeyMappingIdList.Close();
    
    iKeyMappingStringList.ResetAndDestroy();
    iKeyMappingStringList.Close();

    delete iRepositoryFep;
    delete iTriggerStr;   
    delete iHwrEngine;
    delete iUserDefinedResource;
    
    if ( iDefaultNumberMapping )
        {
        delete iDefaultNumberMapping;
        }
    iDefaultNumberMapping = NULL;
    
    }


// ----------------------------------------------------------------------------
// Is valid candidate
// ----------------------------------------------------------------------------
//
TBool CPeninputFingerHwrDataStore::IsValidCandidate( TInt aCandidateIndex ) const
    {
    return ( *iCandidates[aCandidateIndex] ).Length() > 0;
    }


// ----------------------------------------------------------------------------
// Is English letter
// ----------------------------------------------------------------------------
//
TBool CPeninputFingerHwrDataStore::IsEnglishLetter( const TDesC& aLetter)
    {
    TUint unicode =  *((TUint16*) aLetter.Ptr());
    
    const TUint ACode = 0x0041;   // Letter "A"
    const TUint ZCode = 0x005A;   // Letter "Z"
    const TUint aCode = 0x0061;   // Letter "a"
    const TUint zCode = 0x007A;   // Letter "z" 
       
    if( ((ACode <= unicode) && (unicode <= ZCode))|| 
         ((aCode <= unicode) && (unicode <= zCode)) )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

// ----------------------------------------------------------------------------
// Do recoginize by engine for Chinese
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::DoChineseRecognizeL( const RArray<TPoint>& aTraceData )
    {
    iHwrEngine->DoRecognizeL( aTraceData, iCandidates );
    
    // To set candidate and do predictive
    SetCandidatesL();
    }


// ----------------------------------------------------------------------------
// Do recoginize by engine for English
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::DoEngRecognizeL( const RArray<TPoint>& aTraceData )
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
    iHwrEngine->DoRecognizeL( aTraceData, iCandidates );
    // To set candiate and do auto complete
    SetAutoCompleteCandidatesL(); 
#else
    iHwrEngine->DoRecognizeL( aTraceData, iPredictives );
#endif // RD_INTELLIGENT_TEXT_INPUT
    }
    

// ----------------------------------------------------------------------------
// Do predictive using trigger string
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::DoPredictiveL()
    {
    // clear the predictives
    iPredictives.ResetAndDestroy();
    
    TInt primaryRange = iPermittedRanges[0];
    TPtrC string = TriggerStr()->GetString();    
    
    if( primaryRange == ERangeNative )
        {
        iHwrEngine->DoPredictiveL( string, iPredictives );      
        }
    
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    if( primaryRange == ERangeEnglish )
        { // only for auto complete
        if(!IsEnglishLetter(string.Right(1)))
            {
            // clear the predictives when input non-letter.
            iPredictives.ResetAndDestroy();
            }
        else
            {
            iHwrEngine->DoEngPredictiveL( string, iPredictives );
            // Remove all the accent symbol from auto complete candidate
            RemoveAccent();
            }
        }
#endif  // No predictive candidate when auto complete is off     
    }
    
 
// ----------------------------------------------------------------------------
// Set fixed Chinese symbols
// ----------------------------------------------------------------------------
//
 void CPeninputFingerHwrDataStore::SetChineseSymbolL()
    {
    iCandidates.ResetAndDestroy();
    TInt symCount = sizeof( ChineseSymbolCode )/sizeof( TUint16 ); 	
    
    for( TInt i = 0; i < symCount; i++ )
        {
        TBuf<1> charBuf;
        charBuf.Append( ChineseSymbolCode[i] );

        iCandidates.AppendL( charBuf.AllocL() );
        }    
    
    }

// ----------------------------------------------------------------------------
// Set fixed Chinese predictives
// ----------------------------------------------------------------------------
//
 void CPeninputFingerHwrDataStore::SetFixChinesePredictiveL()
    {
    iPredictives.ResetAndDestroy();
    TInt symCount = sizeof( ChinesePredictCode )/sizeof( TUint16 ); 	
    
    for( TInt i = 0; i < symCount; i++ )
        {
        TBuf<1> charBuf;
        charBuf.Append( ChinesePredictCode[i] );

        iPredictives.AppendL( charBuf.AllocL() );
        }
    }

// ----------------------------------------------------------------------------
// Set fixed English symbols
// ----------------------------------------------------------------------------
//
  void CPeninputFingerHwrDataStore::SetFixEnglishSymbolL(TRowCount aRowCount)
    {
    TInt symCount = 0;
    
    switch(aRowCount)
        {
        case ERowOne: // only set one row dropdownlist
            {
            iCandidates.ResetAndDestroy();
            symCount = sizeof( EnglishSymbolCodeOneRow )/sizeof( TUint16 );
            for( TInt i = 0; i < symCount; i++ )
                {
                TBuf<1> charBuf;            
                charBuf.Append( EnglishSymbolCodeOneRow[i] );
                iCandidates.AppendL( charBuf.AllocL() );
                }
            }
            break;
        case ERowTwo: // Set two rows dropdownlist
            {
            iCandidates.ResetAndDestroy();
            iPredictives.ResetAndDestroy();
            symCount = sizeof( EnglishSymbolCodeTwoRow )/sizeof( TUint16 );
            
            for( TInt i = 0; i < symCount; i++ )
                {
                TBuf<1> charBuf1; // the first row
                TBuf<2> charBuf2; // the second row           
                
                charBuf1.Append( EnglishSymbolCodeOneRow[i] );
                iCandidates.AppendL( charBuf1.AllocL() );
                
                if(i > 2) // for smile symbol
                    {
                    charBuf2.Append(ColonSymbol);
                    } 
                charBuf2.Append( EnglishSymbolCodeTwoRow[i] );
               
                iPredictives.AppendL( charBuf2.AllocL() );
                } 
            }
            break;
        default:
            break;
        }
    }   

// ----------------------------------------------------------------------------
// Get the predictive count
// ----------------------------------------------------------------------------
//
TInt CPeninputFingerHwrDataStore::PredictiveCount() const
    {
    return iPredictives.Count();
    }

// ----------------------------------------------------------------------------
// Get the trigger string
// ----------------------------------------------------------------------------
//
CAknFepHwrTriggerStr* CPeninputFingerHwrDataStore::TriggerStr() const
    {
    return iTriggerStr;
    }

// ----------------------------------------------------------------------------
// Set permitted ranges
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SetPermittedRanges( TInt aPermittedRanges )
    {
    if(aPermittedRanges == 0)
        return;
    
    iPermittedRanges.Reset();
    
    //order can not be changed
    if ( aPermittedRanges & ERangeNative )
        {
        iPermittedRanges.Append( ERangeNative );
        }
    if ( aPermittedRanges & ERangeEnglish )
        {
        iPermittedRanges.Append( ERangeEnglish );
        }
    if ( aPermittedRanges & ERangeNumber )
        {
        iPermittedRanges.Append( ERangeNumber );
        }
    if ( aPermittedRanges & ERangeSymbol )
        {
        iPermittedRanges.Append( ERangeSymbol );
        }
    
    iIntRange = aPermittedRanges;
    
    // set primary and auxiliary ranges for hwr engine
    iHwrEngine->SetRanges( iPermittedRanges );
    }

    
// ----------------------------------------------------------------------------
// Set permitted cases
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SetPermittedCases( TInt aPermittedCases )
    {
    iPermittedCases = aPermittedCases;
    }

// ----------------------------------------------------------------------------
// Set Primary range
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SetPrimaryRange( TInt aPrimaryRange )
    {
    const TInt oldIndex = iPermittedRanges.Find( aPrimaryRange );
    
    // store the current range
    switch(aPrimaryRange)
        {
        case ERangeNative:
            {
            iCurrentRange = EFingerHwrNativeRange;
            }
            break;
        case ERangeEnglish:
            {
            iCurrentRange = EFingerHwrEnglishRange;
            }
            break;
        case ERangeNumber:
            {
            iCurrentRange = EFingerHwrNumberRange;
            }
            break;
        default:
            break;
        }
        
    if ( oldIndex > 0 )
        {
        iPermittedRanges.Sort();
        const TInt index = iPermittedRanges.Find( aPrimaryRange );
        
        iPermittedRanges.Remove( index );
        iPermittedRanges.Insert( aPrimaryRange, 0 );

        // set primary range for hwr engine
        iHwrEngine->SetRanges( iPermittedRanges );
        }
    }

// ----------------------------------------------------------------------------
// get Primary range
// ----------------------------------------------------------------------------
//
TInt CPeninputFingerHwrDataStore::PrimaryRange()
	{
	if( iPermittedRanges.Count() > 0 )
		{
	    return iPermittedRanges[0];			
		}
	return -1;
	}
	
// ----------------------------------------------------------------------------
// get current range
// ----------------------------------------------------------------------------
//
TFingerHwrRange CPeninputFingerHwrDataStore::CurrentRange()
	{
	return iCurrentRange;
	}

// ----------------------------------------------------------------------------
// Set case
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SetCase( TInt aCase )
    {
    iCase = aCase;
    iHwrEngine->SetCase( iCase );
    }

// ----------------------------------------------------------------------------
// Set language
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SetLanguageL( TInt aLanguage )
    {
    if( iLanguage == aLanguage )
        {
        return;
        }
        
    if ( !LanguageIsChinese( aLanguage ) && ( aLanguage != ELangEnglish ) )
        {
        return;
        }
        
    if ( aLanguage == ELangEnglish )
        {
        TLanguage displayLang = User::Language();
        
        // set real language to a Chinese
        if ( !LanguageIsChinese( iLanguage ) )
            {
            // original language is not Chinese
            if ( LanguageIsChinese( displayLang ) )
                {
                iLanguage = displayLang;
                }
            else
                {
                iLanguage = ELangPrcChinese;    
                }
                
            SetEngineLanguageL( iLanguage );
            }   
        }
    else
        {
        // aLanguage is Chinese
        if ( iLanguage != aLanguage )
            {
            iLanguage = aLanguage;
            
            SetEngineLanguageL( iLanguage );
            }
        }
    }

// ----------------------------------------------------------------------------
// Set the candidate list
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SetCandidatesL()
    {        
    if( !IsDirectlySentCandidate( *iCandidates[0] ) )
        {
        iTriggerStr->Set( *iCandidates[0] );       
        
        DoPredictiveL();
        }
    else
        {
        iTriggerStr->DeleteAll();
        SetFixChinesePredictiveL();
        }
    }
    
// ----------------------------------------------------------------------------
// Set the candidate list
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SetAutoCompleteCandidatesL()
    {        
#ifdef RD_INTELLIGENT_TEXT_INPUT
    //those special chars: backspace and enter char need sent to
     //app directly, not need to display candidate window
    if ( iCandidates.Count()<1 || !iCandidates[0]  )
        {
        return;
        }
    
    if ( !IsDirectlySentCandidate( *iCandidates[0] ) )
        {
        TPtrC string = iTriggerStr->GetString();
        if(FirstStroke())
            {
            if(string.Length() == 0 || 
               (string.Length() != 0 && !IsEnglishLetter(string.Right(1))))
                {
                // reset trigger string when got a punctuation
                iLayout->SubmitStartPosToFep(ETrue);
                iTriggerStr->Set( *iCandidates[0] );
                }
            else
                {
                // just add the new stroke to trigger string,
                // when write a new char
                iTriggerStr->Append( *iCandidates[0] );
                }
            }
        else
            {
            // replace previous stroke
            iTriggerStr->DeleteLast();
            iTriggerStr->Append( *iCandidates[0] );
            }
        DoPredictiveL();
        }
#endif
    }
 
// ----------------------------------------------------------------------------
// Get first stroke flag
// ----------------------------------------------------------------------------
//
TBool CPeninputFingerHwrDataStore::FirstStroke() 
    {
    return iIsFirstStroke;
    }

// ----------------------------------------------------------------------------
// Set first stroke flag
// ----------------------------------------------------------------------------
// 
void CPeninputFingerHwrDataStore::SetFirstStroke(const TBool aIsFirstStroke) 
    {
    iIsFirstStroke = aIsFirstStroke;
    }
      
// ----------------------------------------------------------------------------
// Get the candidate
// ----------------------------------------------------------------------------
//    
const RPointerArray<HBufC>& CPeninputFingerHwrDataStore::Candidate() const
    {
    return iCandidates;
    }

// ----------------------------------------------------------------------------
// Get the predictive
// ----------------------------------------------------------------------------
//
const RPointerArray<HBufC>& CPeninputFingerHwrDataStore::Predictive()
    {
    return iPredictives;
    }

// ----------------------------------------------------------------------------
// Set number mode for hwr engine
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SetNumberMode( 
    const TAknEditorNumericKeymap aNumberMode )
    {
    iCurrentNumberMode = aNumberMode;
    iHwrEngine->SetNumberMode( aNumberMode );
    iHwrEngine->SetRanges( iPermittedRanges );       
    }

// ----------------------------------------------------------------------------
// Set number mode for hwr engine
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SetCustomNumberModeL( const TDesC& aCustomMode )
    {
    iHwrEngine->SetCustomKeymapL( aCustomMode );
    
    delete iUserDefinedResource;
    iUserDefinedResource = NULL;
    iUserDefinedResource = HBufC::NewL( aCustomMode.Length() );
    iUserDefinedResource->Des().Append( aCustomMode );
    }

// ----------------------------------------------------------------------------
// save range to repository if english or chinese
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SaveRange( TInt aRange )
    {
    if ( iRepositoryFep && ( aRange == ERangeNative || aRange == ERangeEnglish ) )
        {
        iRepositoryFep->Set( KAknFepLastUsedRange, aRange );
        }
    }

// ------------------------------------------------------------------
// Get next predictive candidate page
// -------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::GetNextPredictivePageL()
    {
    iHwrEngine->DoPredictiveL( KNullDesC, iPredictives, ETrue );
 
    TInt predictCount = PredictiveCount();
    if ( predictCount <= 0 )
        {
        SetFixChinesePredictiveL();
        }
    }

// ------------------------------------------------------------------
// sync stroke end mark of writingbox with hwr engine.
// -------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SetStrokeEndMark()
    {
    iLayout->SyncHwrStrokeEndMark( iHwrEngine->StrokeEndMark() );
    }

// ----------------------------------------------------------------------------
// get stroke end mark from hwr engine
// ----------------------------------------------------------------------------
//
TPoint CPeninputFingerHwrDataStore::StrokeEndMark()
    {
    return iHwrEngine->StrokeEndMark();
    }

// ----------------------------------------------------------------------------
// stroke end mark from control
// ----------------------------------------------------------------------------
//
TPoint CPeninputFingerHwrDataStore::StrokeEndMarkFromControl()
    {
    return TPoint( KDefaultStrokeEndMarkX, KDefaultStrokeEndMarkY );
    }
    
// --------------------------------------------------------------------
// Get number keymapping string
// --------------------------------------------------------------------
//   
HBufC* CPeninputFingerHwrDataStore::KeyMappingStringL() const
    {
    TLocale locale;
    TBuf<KMaxNumericString> rs = KNumberString(); // copy "0123456789"
    const TInt index = iKeyMappingIdList.Find( iCurrentNumberMode );

    if ( !( iPermittedRanges.Count() == 1 && iPermittedRanges[0] == ERangeNumber ) )
        {
        rs.Append( *iDefaultNumberMapping ); //normal editor
        }
    else if ( iCurrentNumberMode == EKeymapFromResource )
        {
        rs.Append( *iUserDefinedResource );
        }  
    else if ( index != KErrNotFound )
        {
        HBufC* buf = iKeyMappingStringList[index];
        
        rs.Append( *buf );
        
        if ( ( iCurrentNumberMode == EAknEditorCalculatorNumberModeKeymap ) ||
             ( iCurrentNumberMode == EAknEditorConverterNumberModeKeymap ) )
            {
            rs.Append( locale.DecimalSeparator() ); 
            }       
        }
        
    return rs.AllocL();          
    }

// ------------------------------------------------------------------------
// CPeninputFingerHwrDataStore::IsSpecialDisplayChars
// ------------------------------------------------------------------------
//       
TBool CPeninputFingerHwrDataStore::IsSpecialDisplayChars( const TDesC& aChar ) const
    { 
    if( aChar.Compare( KGestureEnter ) == 0 
       || aChar.Compare( KDisplayBackspace ) == 0
       || aChar.Compare( KGestureSpace )== 0 )
        {
        return ETrue;
        }
            
    return EFalse;
    }    

// -----------------------------------------------------------------------------
// CPeninputFingerHwrDataStore::IsDirectlySentCandidate
// -----------------------------------------------------------------------------
//  
TBool CPeninputFingerHwrDataStore::IsDirectlySentCandidate( const TDesC& aChar ) const
    {
    
    if( aChar.Compare( KGestureEnter ) == 0 ||
        aChar.Compare( KDisplayBackspace ) == 0)
       {
       return ETrue;
       }
           
    return EFalse;
    }    
    
// ------------------------------------------------------------------------
// CPeninputFingerHwrDataStore::ConvertDisplayChars
// ------------------------------------------------------------------------
//   
HBufC* CPeninputFingerHwrDataStore::ConvertDisplayChars( const TDesC& aChar ) const   
    {
    HBufC* convertedCan = NULL;
    TBuf<KSpecialConvertedCharCount> str;
    if( aChar.Compare( KGestureEnter ) == 0 )
        {
        str.Append( EKeyEnter );
        } 
    else if( aChar.Compare( KDisplayBackspace ) == 0 )
        {
        str.Append( EKeyBackspace );
        }
    else if( aChar.Compare( KGestureSpace )== 0 )
        {
        str.Append( EKeySpace );
        }
                 
    if( str.Length() > 0 )
    	{
    	convertedCan = str.Alloc();	
    	}
    	
    return convertedCan;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrDataStore::StartCharacter
// -----------------------------------------------------------------------------
//
TBool CPeninputFingerHwrDataStore::StartCharacter()
    {
    return iStartCharacter;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrDataStore::SetStartCharacter
// -----------------------------------------------------------------------------
//  
void CPeninputFingerHwrDataStore::SetStartCharacter(const TBool aStartCharacter)
    {
    iStartCharacter = aStartCharacter;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrDataStore::SetInputAreaSize
// -----------------------------------------------------------------------------
//  
TInt CPeninputFingerHwrDataStore::SetInputAreaSize(TSize& aSize)
    {
    return iHwrEngine->SetInputAreaSize(aSize);    
    }
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrDataStore::SetScreenSize
// -----------------------------------------------------------------------------
//      
TInt CPeninputFingerHwrDataStore::SetScreenSize(TSize& aSize)
    {
    return iHwrEngine->SetScreenSize(aSize);    
    }    

// -----------------------------------------------------------------------------
// CPeninputFingerHwrDataStore::ClearPredictive
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::ClearPredictive()
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
    iPredictives.ResetAndDestroy();
#endif    
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrDataStore::SetHighlight
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrDataStore::SetHighlight(const TBool aIsHighlight)
   {
   iIsHighlightCell = aIsHighlight;
   }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrDataStore::Highlight
// -----------------------------------------------------------------------------
//
TBool CPeninputFingerHwrDataStore::Highlight()
   {
   return iIsHighlightCell;
   }
 
// -----------------------------------------------------------------------------
// CPeninputFingerHwrDataStore::RemoveDuplicateCand
// -----------------------------------------------------------------------------
//
TInt CPeninputFingerHwrDataStore::RemoveDuplicateCand(const RPointerArray<HBufC>& aSrc,
                                                 RPointerArray<HBufC>& aTgt,
                                                 TInt aSrcStartIdx,
                                                 TInt aTgtStartIdx)
    {
    TInt lastOverlapIdx = KInvalidIndex;

    // on the assumption that if candidate overlap,
    // it is overlap only once
    TInt srccandcnt = aSrc.Count();

    for (TInt i = aSrcStartIdx; i < srccandcnt; i++)
        {
        for (TInt j = aTgtStartIdx; j < aTgt.Count(); j++)
            {
            if (aTgt[j]->Compare(*(aSrc[i])) == 0)
                {
                lastOverlapIdx = i;
                delete aTgt[j];
                aTgt.Remove(j);
                j--;
                break;
                }
            }
        }

    return lastOverlapIdx;
    }        
       
 // ----------------------------------------------------------------------------
 // C++ constructor
 // ----------------------------------------------------------------------------
 //
 CPeninputFingerHwrDataStore::CPeninputFingerHwrDataStore(CPeninputFingerHwrLayout* aLayout)
     :iLayout( aLayout )
     {
     iLanguage = ELangNone;
     iCurrentNumberMode = EAknEditorStandardNumberModeKeymap;
     iIsHighlightCell = ETrue;
     }

 // ----------------------------------------------------------------------------
 // Second phase constructor
 // ----------------------------------------------------------------------------
 //
 void CPeninputFingerHwrDataStore::ConstructL( CPtiEngine* aPtiEngine )
     {
     //create the repository for fep settings    
     iRepositoryFep = CRepository::NewL( KCRUidAknFep );
     
     //create the trigger string
     iTriggerStr = new ( ELeave ) CAknFepHwrTriggerStr();
     
     //create the hwr engine
     iHwrEngine = CAknFepHwrEngine::NewL( aPtiEngine, this );
     
     //initialize the key mapping list
     InitKeyMappingListL();
     }

 // ----------------------------------------------------------------------------
 // Initialize the key mapping list
 // ----------------------------------------------------------------------------
 //   
 void CPeninputFingerHwrDataStore::InitKeyMappingListL()
     {
     CCoeEnv* env = CCoeEnv::Static();
         
     //read default key mapping string from resource
     if ( iDefaultNumberMapping )
         {
         delete iDefaultNumberMapping;
         iDefaultNumberMapping = NULL;
         }
     iDefaultNumberMapping = env->AllocReadResourceL( R_AKN_FEP_HWR_KEY_MAPPING_DEFAULT );
         
      //read the key maping string from resource
     TResourceReader reader;
     env->CreateResourceReaderLC( reader, R_AKN_FEP_HWR_KEY_MAPPING_STRING_LIST ); 
     
     const TInt count = reader.ReadInt16();
    
     for ( TInt i = 0; i < count; i++ )
         {
         iKeyMappingIdList.Append( reader.ReadInt16() );
         TPtrC ptr = reader.ReadTPtrC();
         iKeyMappingStringList.Append( ptr.Alloc() );
         }
     iCurrentNumberMode = EAknEditorStandardNumberModeKeymap;
     
     CleanupStack::PopAndDestroy(); //reader
     }

 // ----------------------------------------------------------------------------
 // Judge if a language is Chinese
 // ----------------------------------------------------------------------------
 //
 TBool CPeninputFingerHwrDataStore::LanguageIsChinese( TInt aLanguage )
     {
     if ( ( aLanguage == ELangPrcChinese ) ||
         ( aLanguage == ELangHongKongChinese ) ||
         ( aLanguage == ELangTaiwanChinese ) )
         {
         return ETrue;
         }
     
     return EFalse;    
     }
 
 // ----------------------------------------------------------------------------
 // Set real engine language
 // ----------------------------------------------------------------------------
 //
 void CPeninputFingerHwrDataStore::SetEngineLanguageL( TInt aLanguage )
     {
     SetLanguageShowText( aLanguage );       
     iHwrEngine->SetLanguageL( aLanguage );
     iHwrEngine->SetPrimaryCandidateNum( KCandidateCount );
     }

 // ----------------------------------------------------------------------------
 // Set language show text
 // ----------------------------------------------------------------------------
 //
 void CPeninputFingerHwrDataStore::SetLanguageShowText( TInt aLanguage )
     {
     AknPenInputUtils::GetISOLanguageCode( TLanguage( aLanguage ), 
         iLanguageShowText );
     }

 // ----------------------------------------------------------------------------
 // Remove accent characters
 // ----------------------------------------------------------------------------
 //   
void CPeninputFingerHwrDataStore::RemoveAccent()
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if(TriggerStr()->GetString().Length() != 1)
        {
        return;
        }
        
    TInt count = PredictiveCount();
    for(TInt i = count; i > 0; i--)
        {
        if(!IsEnglishLetter(iPredictives[i - 1]->Des()))
            {
            delete iPredictives[i-1];
            iPredictives.Remove(i-1);
            count--;
            }
        }

#endif               
    }
    
 // ----------------------------------------------------------------------------
 // Set key board type to Qwerty
 // ----------------------------------------------------------------------------
 //   
void CPeninputFingerHwrDataStore::SetKeyboardToQwerty()
    {
    iHwrEngine->SetKeyboardToQwerty(); 
    }
    
// End Of File
