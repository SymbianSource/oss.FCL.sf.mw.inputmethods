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
* Description:  Implementation for arabic peninput finger hwr
*
*/

//SYSTEM INCLUDE
#include <centralrepository.h>
#include <settingsinternalcrkeys.h>
#include <CommonEngineDomainCRKeys.h>

//FEP INCLUDE
#include <aknfeppeninputenums.h>
#include <AknFepGlobalEnums.h>
#include <AknFepInternalCRKeys.h>
#include <peninputcommonlayoutglobalenum.h>

//USER INCLUDE
#include "peninputfingerhwrarlayout.h"
#include "peninputfingerhwrardatastore.h"
#include "peninputfingerhwrarengine.h"
#include "peninputpluginutils.h"
#include "peninputfingerhwrarstoreconstants.h"
#include <peninputfingerhwrarwnd.rsg>

//CONSTANT DEFINATION HEADER


// ----------------------------------------------------------------------------
// Symbian constructor
// ----------------------------------------------------------------------------
//
CPeninputFingerHwrArDataStore* CPeninputFingerHwrArDataStore::NewL( 
    CPtiEngine* aPtiEngine, CPeninputFingerHwrArLayout* aLayout )
    {
    CPeninputFingerHwrArDataStore* self = new ( ELeave ) 
        CPeninputFingerHwrArDataStore(aLayout);
    
    CleanupStack::PushL( self );
    self->ConstructL( aPtiEngine );
    CleanupStack::Pop( self );//self

    return self;
    }

// ----------------------------------------------------------------------------
// destructor
// ----------------------------------------------------------------------------
//
CPeninputFingerHwrArDataStore::~CPeninputFingerHwrArDataStore()
    {
    iCandidates.ResetAndDestroy();
    iCandidates.Close();


    iRangeStartPos.Close();
    iPermittedRanges.Close();
    iKeyMappingIdList.Close();
    
    iKeyMappingStringList.ResetAndDestroy();
    iKeyMappingStringList.Close();

    delete iRepositoryFep;
    delete iCommonEngineRepository;
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
TBool CPeninputFingerHwrArDataStore::IsValidCandidate( TInt aCandidateIndex ) const
    {
    return ( *iCandidates[aCandidateIndex] ).Length() > 0;
    }


// ----------------------------------------------------------------------------
// Is English letter
// ----------------------------------------------------------------------------
//
TBool CPeninputFingerHwrArDataStore::IsEnglishLetter( const TDesC& aLetter)
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
void CPeninputFingerHwrArDataStore::DoRecognizeL( const RArray<TPoint>& aTraceData )
    {
    iHwrEngine->DoRecognizeL( aTraceData, iCandidates );
    ReorderCandidates();
    }



 
// ----------------------------------------------------------------------------
// Set fixed Chinese symbols
// ----------------------------------------------------------------------------
//
 void CPeninputFingerHwrArDataStore::SetArabicSymbolL()
    {
    iCandidates.ResetAndDestroy();
    }

// ----------------------------------------------------------------------------
// Set permitted ranges
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrArDataStore::SetPermittedRanges( TInt aPermittedRanges )
    {
    if(aPermittedRanges == 0)
        return;
    
    iPermittedRanges.Reset();
    
    //order can not be changed
    if ( aPermittedRanges & ERangeNative )
        {
        iPermittedRanges.Append( ERangeNative );
        }
    if ( aPermittedRanges & ERangeNativeNumber )
        {
        iPermittedRanges.Append( ERangeNativeNumber );
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
void CPeninputFingerHwrArDataStore::SetPermittedCases( TInt aPermittedCases )
    {
    iPermittedCases = aPermittedCases;
    }
// ----------------------------------------------------------------------------
// get Primary range
// ----------------------------------------------------------------------------
//
TInt CPeninputFingerHwrArDataStore::PrimaryRange()
	{
	if( iPermittedRanges.Count() > 0 )
		{
	    return iPermittedRanges[0];			
		}
	return -1;
	}
	
// ----------------------------------------------------------------------------
// Set case
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrArDataStore::SetCase( TInt aCase )
    {
    iCase = aCase;
    iHwrEngine->SetCase( iCase );
    }

// ----------------------------------------------------------------------------
// Set language
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrArDataStore::SetLanguageL( TInt aLanguage )
    {
    if( iLanguage == aLanguage )
        {
        return;
        }
        
    if ( !IsArabicLanguage( aLanguage ) && ( aLanguage != ELangEnglish ) )
        {
        return;
        }
        
    if ( aLanguage == ELangEnglish )
        {
        TLanguage displayLang = User::Language();

        // set real language to a Arabic
        if ( !IsArabicLanguage( iLanguage ) )
            {
            // original language is not Chinese
            if ( IsArabicLanguage( displayLang ) )
                {
                iLanguage = displayLang;
                }
            else
                {
                iLanguage = ELangArabic;    
                }
                
            SetEngineLanguageL( iLanguage );
            }   
        
        }
    else
        {
        // aLanguage is arabic
        if ( iLanguage != aLanguage )
            {
            iLanguage = aLanguage;
            
            SetEngineLanguageL( iLanguage );
            }
        }
    }


// ----------------------------------------------------------------------------
// Get first stroke flag
// ----------------------------------------------------------------------------
//
TBool CPeninputFingerHwrArDataStore::FirstStroke() 
    {
    return iIsFirstStroke;
    }

// ----------------------------------------------------------------------------
// Set first stroke flag
// ----------------------------------------------------------------------------
// 
void CPeninputFingerHwrArDataStore::SetFirstStroke(const TBool aIsFirstStroke) 
    {
    iIsFirstStroke = aIsFirstStroke;
    }
      
// ----------------------------------------------------------------------------
// Get the candidate
// ----------------------------------------------------------------------------
//    
const RPointerArray<HBufC>& CPeninputFingerHwrArDataStore::Candidate() const
    {
    return iCandidates;
    }



// ----------------------------------------------------------------------------
// Set number mode for hwr engine
// ----------------------------------------------------------------------------
//
void CPeninputFingerHwrArDataStore::SetNumberMode( 
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
void CPeninputFingerHwrArDataStore::SetCustomNumberModeL( const TDesC& aCustomMode )
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
void CPeninputFingerHwrArDataStore::SaveRange( TInt aRange )
    {
    if ( iRepositoryFep && ( aRange == ERangeNative || aRange == ERangeEnglish ) )
        {
        iRepositoryFep->Set( KAknFepLastUsedRange, aRange );
        }
    }


// ------------------------------------------------------------------
// sync stroke end mark of writingbox with hwr engine.
// -------------------------------------------------------------------
//
void CPeninputFingerHwrArDataStore::SetStrokeEndMark()
    {
    iLayout->SyncHwrStrokeEndMark( iHwrEngine->StrokeEndMark() );
    }

// ----------------------------------------------------------------------------
// get stroke end mark from hwr engine
// ----------------------------------------------------------------------------
//
TPoint CPeninputFingerHwrArDataStore::StrokeEndMark()
    {
    return iHwrEngine->StrokeEndMark();
    }

// ----------------------------------------------------------------------------
// stroke end mark from control
// ----------------------------------------------------------------------------
//
TPoint CPeninputFingerHwrArDataStore::StrokeEndMarkFromControl()
    {
    return TPoint( KDefaultStrokeEndMarkX, KDefaultStrokeEndMarkY );
    }
    
// --------------------------------------------------------------------
// Get number keymapping string
// --------------------------------------------------------------------
//   
HBufC* CPeninputFingerHwrArDataStore::KeyMappingStringL() const
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
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::StartCharacter
// -----------------------------------------------------------------------------
//
TBool CPeninputFingerHwrArDataStore::StartCharacter()
    {
    return iStartCharacter;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::SetStartCharacter
// -----------------------------------------------------------------------------
//  
void CPeninputFingerHwrArDataStore::SetStartCharacter(const TBool aStartCharacter)
    {
    iStartCharacter = aStartCharacter;
    }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::SetInputAreaSize
// -----------------------------------------------------------------------------
//  
TInt CPeninputFingerHwrArDataStore::SetInputAreaSize(TSize& aSize)
    {
    return iHwrEngine->SetInputAreaSize(aSize);    
    }
    
// -----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::SetScreenSize
// -----------------------------------------------------------------------------
//      
TInt CPeninputFingerHwrArDataStore::SetScreenSize(TSize& aSize)
    {
    return iHwrEngine->SetScreenSize(aSize);    
    }    


// -----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::SetHighlight
// -----------------------------------------------------------------------------
//
void CPeninputFingerHwrArDataStore::SetHighlight(const TBool aIsHighlight)
   {
   iIsHighlightCell = aIsHighlight;
   }

// -----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::Highlight
// -----------------------------------------------------------------------------
//
TBool CPeninputFingerHwrArDataStore::Highlight()
   {
   return iIsHighlightCell;
   }
 
 // ----------------------------------------------------------------------------
 // C++ constructor
 // ----------------------------------------------------------------------------
 //
 CPeninputFingerHwrArDataStore::CPeninputFingerHwrArDataStore(CPeninputFingerHwrArLayout* aLayout)
     :iLanguage(ELangNone),
      iCurrentNumberMode(EAknEditorStandardNumberModeKeymap),
      iLayout( aLayout ),
      iIsHighlightCell(ETrue),
      iIsNativeNumMode(EFalse)
     {
     }

 // ----------------------------------------------------------------------------
 // Second phase constructor
 // ----------------------------------------------------------------------------
 //
 void CPeninputFingerHwrArDataStore::ConstructL( CPtiEngine* aPtiEngine )
     {
     //create the repository for fep settings    
     iRepositoryFep = CRepository::NewL( KCRUidAknFep );
     
     //create the trigger string
//     iTriggerStr = new ( ELeave ) CAknFepHwrArTriggerStr();
     
     //create the hwr engine
     iHwrEngine = CPeninputFingerHwrArEngine::NewL( aPtiEngine, this );
     
     //initialize the key mapping list
     InitKeyMappingListL();
     
     // read number mode flag.
     //create the repository for common engine.
     iCommonEngineRepository = CRepository::NewL(KCRUidCommonEngineKeys);
     TInt displayLang = 0;
     iCommonEngineRepository->Get(KGSDisplayTxtLang,displayLang);
     if( displayLang == ELangTest )
         {
         displayLang = User::Language();
         }     
     if(displayLang == ELangArabic)
         {
         iIsNativeNumMode = ETrue;
         }
     else
         {
         iIsNativeNumMode = EFalse;
         }
     iRepositoryFep->Set(KAknFepDefaultArabicNumberMode,iIsNativeNumMode);
     
     }

 // ----------------------------------------------------------------------------
 // Initialize the key mapping list
 // ----------------------------------------------------------------------------
 //   
 void CPeninputFingerHwrArDataStore::InitKeyMappingListL()
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
 // Judge if a language is Arabic
 // ----------------------------------------------------------------------------
 //
 TBool CPeninputFingerHwrArDataStore::IsArabicLanguage( TInt aLanguage )
     {
     if( aLanguage == ELangArabic)
         {
         return ETrue;
         }
     
     return EFalse;    
     }
 
 // ----------------------------------------------------------------------------
 // Set real engine language
 // ----------------------------------------------------------------------------
 //
 void CPeninputFingerHwrArDataStore::SetEngineLanguageL( TInt aLanguage )
     {
     SetLanguageShowText( aLanguage );       
     iHwrEngine->SetLanguageL( aLanguage );
     iHwrEngine->SetPrimaryCandidateNum( KCandidateCount );
     }

 // ----------------------------------------------------------------------------
 // Set language show text
 // ----------------------------------------------------------------------------
 //
 void CPeninputFingerHwrArDataStore::SetLanguageShowText( TInt aLanguage )
     {
     AknPenInputUtils::GetISOLanguageCode( TLanguage( aLanguage ), 
         iLanguageShowText );
     }

// ----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::GetTopGuideLinePos
// ----------------------------------------------------------------------------
//   
void CPeninputFingerHwrArDataStore::GetTopGuideLinePos(TInt& aPos)
    {
    iHwrEngine->GetTopGuideLinePos(aPos);
    }

// ----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::GetBottomGuideLinePos
// ----------------------------------------------------------------------------
//	
void CPeninputFingerHwrArDataStore::GetBottomGuideLinePos(TInt& aPos)
    {
    iHwrEngine->GetBottomGuideLinePos(aPos);
    }

// ----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::SetFirstCandidateType
// ----------------------------------------------------------------------------
//		
void CPeninputFingerHwrArDataStore::SetFirstCandidateType(TFirstCandidateType aFirstCandType)
    {
    iFirstCandidateType = aFirstCandType;
    }

// ----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::ReorderCandidates
// ----------------------------------------------------------------------------
//	
void CPeninputFingerHwrArDataStore::ReorderCandidates()
    {
    TInt candCount = iCandidates.Count();
    TInt insertPos = 0;
    if(iFirstCandidateType == ECandLatinCharFirst) 
        {
        for(TInt i = 0; i < candCount; i++)
            {
            TUint16 unicode = (*iCandidates[i])[0];
            if(IsLatinChar(unicode))
                {
                if(insertPos < i)
                    {
					iCandidates.Insert(iCandidates[i],insertPos);
                    iCandidates.Remove(i+1);
                    }
                insertPos++;
                }
            }
        }
    else if(iFirstCandidateType == ECandLatinNumFirst) 
        {
        for(TInt i = 0; i < candCount; i++)
            {
            TUint16 unicode = (*iCandidates[i])[0];
            if(IsLatinNumber(unicode))
                {
                if(insertPos < i)
                    {
                    iCandidates.Insert(iCandidates[i],insertPos);
                    iCandidates.Remove(i+1);
                    }
                insertPos++;
                }
            }
        }
    else if(iFirstCandidateType == ECandArabicIndicNumFirst) 
        {
        for(TInt i = 0; i < candCount; i++)
            {
            TUint16 unicode = (*iCandidates[i])[0];
            if(IsArabicNumber(unicode))
                {
                if(insertPos < i)
                    {
                    iCandidates.Insert(iCandidates[i],insertPos);
                    iCandidates.Remove(i+1);
                    }
                insertPos++;
                }
            }
        }
    }

// ----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::IsLatinNumber
// ----------------------------------------------------------------------------
//		
TBool CPeninputFingerHwrArDataStore::IsLatinNumber(TUint16 aChar)
    {
	if(aChar >= 0x30 && aChar <= 0x39)
	   {
	   return ETrue;
	   }
	
    return EFalse;	
	}

// ----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::IsLatinChar
// ----------------------------------------------------------------------------
//		
TBool CPeninputFingerHwrArDataStore::IsLatinChar(TUint16 aChar)
    {
	if(aChar >= 0x41 && aChar <= 0x5A 
	   || aChar >= 0x61 && aChar <= 0x7A)
	   {
	   return ETrue;
	   }
    
    return EFalse;	
	}

// ----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::IsArabicNumber
// ----------------------------------------------------------------------------
//	
TBool CPeninputFingerHwrArDataStore::IsArabicNumber(TUint16 aChar)
    {
	if(aChar >= 0x0660 && aChar <= 0x0669)
	    {
		return ETrue;
		}
	
    return EFalse;	
	}

// ----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::IsNumberOnlyMode
// ----------------------------------------------------------------------------
//  
TBool CPeninputFingerHwrArDataStore::IsNumberOnlyMode()
    {
    if(PrimaryRange() == ERangeNumber || PrimaryRange() == ERangeNativeNumber)
        {
        return ETrue;
        }
    return EFalse;
    }

// ----------------------------------------------------------------------------
// CPeninputFingerHwrArDataStore::IsNativeNumMode
// ----------------------------------------------------------------------------
//  
TBool CPeninputFingerHwrArDataStore::IsNativeNumMode()
    {
    return iIsNativeNumMode;
    }
// End Of File
