/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for chinese peninput full screen hwr
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
#include "peninputhwrfscndatastore.h"
#include "peninputhwrtriggerstr.h"
#include "peninputhwrengine.h"

//CONSTANT DEFINATION HEADER
#include "peninputhwrfscnstoreconstants.h"
#include <peninputhwrfscreencn.rsg>
#include "peninputpluginutils.h"

// ----------------------------------------------------------------------------
// Symbian constructor
// ----------------------------------------------------------------------------
//
CPeninputHwrfscnDataStore* CPeninputHwrfscnDataStore::NewL( 
    CPtiEngine* aPtiEngine )
    {
    CPeninputHwrfscnDataStore* self = new ( ELeave ) 
        CPeninputHwrfscnDataStore();
    
    CleanupStack::PushL( self );
    self->ConstructL( aPtiEngine );
    CleanupStack::Pop( self );//self

    return self;
    }

// ----------------------------------------------------------------------------
// C++ constructor
// ----------------------------------------------------------------------------
//
CPeninputHwrfscnDataStore::CPeninputHwrfscnDataStore()
    {
    iLanguage = ELangNone;
    iCurrentNumberMode = EAknEditorStandardNumberModeKeymap;
    }

// ----------------------------------------------------------------------------
// destructor
// ----------------------------------------------------------------------------
//
CPeninputHwrfscnDataStore::~CPeninputHwrfscnDataStore()
    {
    iSubscriberList.Close();
    
    iCandidates.ResetAndDestroy();
    iCandidates.Close();

    iPredictives.ResetAndDestroy();
    iPredictives.Close();
    
    iRangeStartPos.Close();
    
    iPermittedRanges.Close();

    iKeyMappingIdList.Close();
    
    iKeyMappingStringList.ResetAndDestroy();
    iKeyMappingStringList.Close();
  
    delete iGSRepositoryWatcher;

    delete iRepositoryFep;
    delete iRepositorySetting;

    delete iTriggerStr;
    
    delete iHwrEngine;
    
    delete iUserDefinedResource;
    }

// ----------------------------------------------------------------------------
// Second phase constructor
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::ConstructL( CPtiEngine* aPtiEngine )
    {
    //create the repository for fep settings    
    iRepositoryFep = CRepository::NewL( KCRUidAknFep );

    //create the repository for gereral settings
    iRepositorySetting = CRepository::NewL( KCRUidPersonalizationSettings );

    //create the repository watcher for general settings
    iGSRepositoryWatcher = CAknFepRepositoryWatcher::NewL( 
        KCRUidPersonalizationSettings, 
        TCallBack( HandleGSRepositoryCallBack, this ),
        iRepositorySetting );
    
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
void CPeninputHwrfscnDataStore::InitKeyMappingListL()
    {
     //read the key maping string from resource
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, 
        R_AKN_FEP_HWR_KEY_MAPPING_STRING_LIST ); 
    
    const TInt count = reader.ReadInt16();
   
    for ( TInt i = 0; i < count; i++ )
        {
        iKeyMappingIdList.Append( reader.ReadInt16() );
        TPtrC ptr = reader.ReadTPtrC();
        iKeyMappingStringList.Append( ptr.Alloc() );
        }
    iCurrentNumberMode = EAknEditorStandardNumberModeKeymap;
    CleanupStack::PopAndDestroy();
    }
    
// ----------------------------------------------------------------------------
// Is candidate in Chinese range
// ----------------------------------------------------------------------------
//
TBool CPeninputHwrfscnDataStore::IsChinese( TInt aCandidateIndex ) const
    {
    TUint16 candidate =  ( *iCandidates[aCandidateIndex] )[0];
    if( candidate >= 0x4E00 && candidate <= 0x9FA5 )
        {
        return ETrue;
        }

    return EFalse;
    }

// ----------------------------------------------------------------------------
// Is candidate in Chinese range
// ----------------------------------------------------------------------------
//
TBool CPeninputHwrfscnDataStore::IsValid( TInt aCandidateIndex ) const
    {
    return ( *iCandidates[aCandidateIndex] ).Length() > 0;
    }

// ----------------------------------------------------------------------------
// Is the language which has the homephonic effect.
// ----------------------------------------------------------------------------
//
TBool CPeninputHwrfscnDataStore::IsHomephonicLanguage()
	{
	if ( iLanguage == ELangPrcChinese || iLanguage == ELangTaiwanChinese )
	    {
	    return ETrue;
	    }
	
	return EFalse;   	
	}
	
// ----------------------------------------------------------------------------
// Do recoginize by engine
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::DoRecognizeL( const RArray<TPoint>& aTraceData )
    {
    iHwrEngine->DoRecognizeL( aTraceData, iCandidates );
    SetCandidatesL();
    }

// ----------------------------------------------------------------------------
// Do predictive using trigger string
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::DoPredictiveL()
    {
    // clear the predictives
    iPredictives.ResetAndDestroy();
    
    //if the Primary range is chinese
    TInt primaryRange = iPermittedRanges[0];
    
    if( primaryRange == ERangeNative )
    	{
        GetPredictivesOfPriRangeAsChineseL();		
        }
    //if the Primary range is english
    else if( primaryRange == ERangeEnglish )
        {
        GetPredictivesOfPriRangeAsEnglishL();
        }
    //if the Primary range is number        
    else if( primaryRange == ERangeNumber )
        {
        GetPredictivesOfPriRangeAsNumberL();
        }                  
    }

// ----------------------------------------------------------------------------
// Get predictives of primary range as chinese
// ----------------------------------------------------------------------------
//    
void CPeninputHwrfscnDataStore::GetPredictivesOfPriRangeAsChineseL()
    {
    TPtrC string = TriggerStr()->GetString();
    
    iHwrEngine->DoPredictiveL( string, iPredictives );
    if( PredictiveCount() == 0 )
    	{
    	TriggerStr()->SetLast();
    	iHwrEngine->DoPredictiveL( TriggerStr()->GetString(), iPredictives );            	
    	}
    
    SetPredictives();
    }

// ----------------------------------------------------------------------------
// Get predictives of primary range as Number
// ----------------------------------------------------------------------------
//    
void CPeninputHwrfscnDataStore::GetPredictivesOfPriRangeAsNumberL()
    {
    if( iPermittedRanges.Count() == 1 )
        {
        if( iCurrentNumberMode != EAknEditorPlainNumberModeKeymap )
            {
            HBufC* keyMappingBuf = KeyMappingStringL();
            CleanupStack::PushL( keyMappingBuf );
            TInt strLen = keyMappingBuf->Length();
            if( strLen > KNumberCount )
                {  
                const TUint16* keyMapingPtr = keyMappingBuf->Ptr();
                //keyMappingBuf always contains 0~9 number symbol
                //move the pointer to 11th char, right after 0~9 numbers, 
                //because we just need symbol not number
                keyMapingPtr += KNumberCount;
                
                TInt length = strLen - KNumberCount;
                for( TInt i = 0; i < length; i++ )
                    {
	                TBuf<1> charBuf;
                    charBuf.Append( ( keyMapingPtr + i ), 1 );
                
                    iPredictives.AppendL( charBuf.AllocL() );
                    }
                
                SetPredictives();
                }
            CleanupStack::PopAndDestroy( keyMappingBuf );
            }
        }
    else
        {
        TInt symCount = sizeof( NumberPredictSymCode )/sizeof( TUint16 ); 	
        
        for( TInt i = 0; i < symCount; i++ )
            {
	        TBuf<1> charBuf;
            charBuf.Append( NumberPredictSymCode[i] );

            iPredictives.AppendL( charBuf.AllocL() );
            }    
                    
        SetPredictives();
        }    
    } 

// ----------------------------------------------------------------------------
// Get predictives of primary range as English
// ----------------------------------------------------------------------------
//    
void CPeninputHwrfscnDataStore::GetPredictivesOfPriRangeAsEnglishL()
    {
    TInt symCount = sizeof( EnglishPredictSymCode )/sizeof( TUint16 );
    
    for( TInt i = 0; i < symCount; i++ )
        {
	    TBuf<1> charBuf;
        charBuf.Append( EnglishPredictSymCode[i] );

        iPredictives.AppendL( charBuf.AllocL() );
        }
    
    if( iPredictives.Count() > 0 )
    	{
	    SetPredictives();    
    	}
    } 
                
// ----------------------------------------------------------------------------
// Do homophonic using trigger string
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::DoHomophonicL( const TDesC& aString )
    {
    iHwrEngine->DoHomophonicL( aString, iCandidates );
    SetHomephonicCandidatesL();
    }
    
    
// ----------------------------------------------------------------------------
// Get the predictive count
// ----------------------------------------------------------------------------
//
TInt CPeninputHwrfscnDataStore::PredictiveCount() const
    {
    return iPredictives.Count();
    }

// ----------------------------------------------------------------------------
// Add property subscriber
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::AddSubscriber( 
    MPeninputHwrfscnPropertySubscriber* aSubscriber )
    {
    if ( aSubscriber )
        {
        const TInt index = iSubscriberList.Find( aSubscriber );
      
        if ( index == KErrNotFound )
            {
            iSubscriberList.Append( aSubscriber );
            }
        }
    }

// ----------------------------------------------------------------------------
// Get the trigger string
// ----------------------------------------------------------------------------
//
CAknFepHwrTriggerStr* CPeninputHwrfscnDataStore::TriggerStr() const
    {
    return iTriggerStr;
    }

// ----------------------------------------------------------------------------
// Set permitted ranges
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetPermittedRanges( TInt aPermittedRanges )
    {
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
    
    NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        EPeninputHwrfscnPropertyPermittedRange );
    
    NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        EPeninputHwrfscnPropertyRange );
    // set primary and auxiliary ranges for hwr engine
    iHwrEngine->SetRanges( iPermittedRanges );
    }

// ----------------------------------------------------------------------------
// Get the permitted ranges
// ----------------------------------------------------------------------------
//
const RArray<TInt>& CPeninputHwrfscnDataStore::PermittedRanges() const
    {
    return iPermittedRanges;
    }
    
// ----------------------------------------------------------------------------
// Set permitted cases
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetPermittedCases( TInt aPermittedCases )
    {
    iPermittedCases = aPermittedCases;
    NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        EPeninputHwrfscnPropertyPermittedCases );
    }

// ----------------------------------------------------------------------------
// Set Primary range
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetPrimaryRange( TInt aPrimaryRange )
    {
    const TInt oldIndex = iPermittedRanges.Find( aPrimaryRange );

    if ( oldIndex > 0 )
        {
        iPermittedRanges.Sort();
        const TInt index = iPermittedRanges.Find( aPrimaryRange );
        
        iPermittedRanges.Remove( index );
        iPermittedRanges.Insert( aPrimaryRange, 0 );

        NotifySubscriber( 
            MPeninputHwrfscnPropertySubscriber::
            EPeninputHwrfscnPropertyRange );
        
        // set primary and auxiliary ranges for hwr engine
        iHwrEngine->SetRanges( iPermittedRanges );
        }
    }

// ----------------------------------------------------------------------------
// get Primary range
// ----------------------------------------------------------------------------
//
TInt CPeninputHwrfscnDataStore::PrimaryRange()
	{
	if( iPermittedRanges.Count() > 0 )
		{
	    return iPermittedRanges[0];			
		}
	return -1;
	}
	
// ----------------------------------------------------------------------------
// get Primary range
// ----------------------------------------------------------------------------
//	
TBool CPeninputHwrfscnDataStore::IsChinesePrimaryRange()
    {
    return ( iPermittedRanges[0] == ERangeNative );
    }
    
// ----------------------------------------------------------------------------
// Set case
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetCase( TInt aCase )
    {
    iCase = aCase;
    NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        EPeninputHwrfscnPropertyCase );
    iHwrEngine->SetCase( iCase );
    }

// ----------------------------------------------------------------------------
// Judge if a language is Chinese
// ----------------------------------------------------------------------------
//
TBool CPeninputHwrfscnDataStore::LanguageIsChinese( TInt aLanguage )
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
void CPeninputHwrfscnDataStore::SetEngineLanguageL( TInt aLanguage )
    {
    SetLanguageShowText( aLanguage );       
    iHwrEngine->SetLanguageL( aLanguage );
    iHwrEngine->SetPrimaryCandidateNum( KCandidateCount );
    }

// ----------------------------------------------------------------------------
// Set language show text
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetLanguageShowText( TInt aLanguage )
	{
    AknPenInputUtils::GetISOLanguageCode( TLanguage( aLanguage ), 
        iLanguageShowText );
    }
    
// ----------------------------------------------------------------------------
// Set stroke end mark
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetStrokeEndMark()
    {
    NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        EPeninputHwrfscnPropertyStrokeEndMark );
    }

// ----------------------------------------------------------------------------
// get stroke end mark from hwr engine
// ----------------------------------------------------------------------------
//
TPoint CPeninputHwrfscnDataStore::StrokeEndMark()
    {
    return iHwrEngine->StrokeEndMark();
    }
    
// ----------------------------------------------------------------------------
// stroke end mark from control
// ----------------------------------------------------------------------------
//
TPoint CPeninputHwrfscnDataStore::StrokeEndMarkFromControl()
    {
	return TPoint( KDefaultStrokeEndMarkX, KDefaultStrokeEndMarkY );
    }

// ----------------------------------------------------------------------------
// Set language
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetLanguageL( TInt aLanguage )
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

    // make sure whole window is visible
    NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        EPeninputHwrfscnPropertyLanguageChanged );
    }

// ----------------------------------------------------------------------------
// Get language
// ----------------------------------------------------------------------------
//
TInt CPeninputHwrfscnDataStore::CurLanguage()
    {
    return iLanguage;
    }
    
// ----------------------------------------------------------------------------
// Set writting speed
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetWritingSpeed( TInt aWritingSpeed )
    {
    iWritingSpeed = aWritingSpeed;
    
    TTimeIntervalMicroSeconds32 speed;
    switch ( iWritingSpeed )
        {
        case EWritingSpeedVeryFast: 
            speed = EHwrWritingSpeedVeryFast;
            break;
        
        case EWritingSpeedFast: 
            speed = EHwrWritingSpeedFast;
            break;
        
        case EWritingSpeedNormal: 
            speed = EHwrWritingSpeedNormal;
            break;
        
        case EWritingSpeedSlow: 
            speed = EHwrWritingSpeedSlow;
            break;
        
        case EWritingSpeedVerySlow: 
            speed = EHwrWritingSpeedVerySlow;
            break;
        
        default:
            speed = EHwrWritingSpeedNormal;
            break;
        }

    NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        EPeninputHwrfscnPropertyWriteSpeed );
    }

// ----------------------------------------------------------------------------
// get writting speed
// ----------------------------------------------------------------------------
//
TTimeIntervalMicroSeconds32 CPeninputHwrfscnDataStore::WritingSpeed()
	{
    TTimeIntervalMicroSeconds32 speed;
    switch ( iWritingSpeed )
        {
        case EWritingSpeedVeryFast: 
            speed = EHwrWritingSpeedVeryFast;
            break;
       
        case EWritingSpeedFast: 
            speed = EHwrWritingSpeedFast;
            break;
        
        case EWritingSpeedNormal: 
            speed = EHwrWritingSpeedNormal;
            break;
        
        case EWritingSpeedSlow: 
            speed = EHwrWritingSpeedSlow;
            break;
        
        case EWritingSpeedVerySlow: 
            speed = EHwrWritingSpeedVerySlow;
            break;
        
        default:
            speed = EHwrWritingSpeedNormal;
            break;
        }	    
	return speed;	
	}
	
// ----------------------------------------------------------------------------
// Set trail drawing pen size
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetBoxPenSize( const TSize& aSize )
    {
    iPenSize = aSize;

    NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        EPeninputHwrfscnPropertyPenSize );        
    }

// ----------------------------------------------------------------------------
// Set trail drawing pen size
// ----------------------------------------------------------------------------
//
TSize CPeninputHwrfscnDataStore::PenSize()
	{
	return iPenSize;	
	}
	
// ----------------------------------------------------------------------------
// Set trail drawing pen color
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetBoxPenColor( const TRgb& aCol )
    {
    iPenColor = aCol;

    NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        EPeninputHwrfscnPropertyPenColor );        
    }

// ----------------------------------------------------------------------------
// get trail drawing pen color
// ----------------------------------------------------------------------------
//

TRgb& CPeninputHwrfscnDataStore::PenColor()
	{
	return iPenColor;	
	}

// ----------------------------------------------------------------------------
// Set the candidate list
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetCandidatesL()
    {        
     //those special chars: backspace and enter char need sent to
     //app directly, not need to display candidate window
    
    if( !IsDirectlySentCandidate( *iCandidates[0] ) )
        {
        NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
            EPeninputHwrfscnPropertyCandidates );
        iTriggerStr->Set( *iCandidates[0] );
        DoPredictiveL();	
        }
    }

// ----------------------------------------------------------------------------
// Set homephonic candidate list
// ----------------------------------------------------------------------------
//

void CPeninputHwrfscnDataStore::SetHomephonicCandidatesL()
    {
    if ( iCandidates.Count() > 0 )
        {
        NotifySubscriber( MPeninputHwrfscnPropertySubscriber
                        ::EPeninputHwrfscnPropertyHomephonicCandidates ); 
        TriggerStr()->Set( *iCandidates[0] );
        DoPredictiveL();                 
        }
    }
// ----------------------------------------------------------------------------
// Set the predictive list
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetPredictives()
    {
    if( PredictiveCount() > 0 )
        {
        NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
            EPeninputHwrfscnPropertyPredictives );
        }
    }
    
// ----------------------------------------------------------------------------
// Add new predictives to list
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::AddPredictives()
    {
    NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        EPeninputHwrfscnPropertyAddPredictives );
    }
    
// ----------------------------------------------------------------------------
// Get the candidate
// ----------------------------------------------------------------------------
//    
const RPointerArray<HBufC>& CPeninputHwrfscnDataStore::Candidate() 
    {
    return iCandidates;
    }
    
// ----------------------------------------------------------------------------
// Get the predictive
// ----------------------------------------------------------------------------
//
const RPointerArray<HBufC>& CPeninputHwrfscnDataStore::Predictive()
    {
    return iPredictives;
    }
    
// ----------------------------------------------------------------------------
// Notify subscriber
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::NotifySubscriber( 
    const MPeninputHwrfscnPropertySubscriber::
    TPeninputHwrfscnProperty aPropertyName )
    {
    for ( TInt i = 0; i < iSubscriberList.Count(); i++ )
        {
        TRAP_IGNORE( iSubscriberList[i]->PropertyChangedL( aPropertyName ) );
        }
    }

// ----------------------------------------------------------------------------
// Callback method to notify repository change
// ----------------------------------------------------------------------------
//
TInt CPeninputHwrfscnDataStore::HandleGSRepositoryCallBack( TAny* aPtr )
    {
    CPeninputHwrfscnDataStore *self = 
        static_cast<CPeninputHwrfscnDataStore*>( aPtr );
    
    CAknFepRepositoryWatcher* watcher = self->iGSRepositoryWatcher;
    if ( watcher )
        {
        TInt newValue = self->GetNewValue( watcher, watcher->ChangedKey() );
        
        switch ( watcher->ChangedKey() )
            {
            case KSettingsWritingSpeed:
                self->SetWritingSpeed( newValue );
                break;
            case KSettingsPenWidth:
                self->SetBoxPenSize( TSize( newValue,newValue ) );
                break;
            case KSettingsPenColor:
                self->SetBoxPenColor( newValue );
                break;
            default:
                break;
            }
        }
    return KErrNone;
    }

// ----------------------------------------------------------------------------
// Set number mode for hwr engine
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetNumberMode( 
    const TAknEditorNumericKeymap aNumberMode )
    {
    iCurrentNumberMode = aNumberMode;
    NotifySubscriber( MPeninputHwrfscnPropertySubscriber::
        EPeninputHwrfscnPropertyNumberMode );

    iHwrEngine->SetNumberMode( aNumberMode );
    iHwrEngine->SetRanges( iPermittedRanges );       
    }

// ----------------------------------------------------------------------------
// Set number mode for hwr engine
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SetNumberModeL( const TDesC& aCustomMode )
    {
    iHwrEngine->SetCustomKeymapL( aCustomMode );
    
    delete iUserDefinedResource;
    iUserDefinedResource = NULL;
    iUserDefinedResource = HBufC::NewL( aCustomMode.Length() );
    iUserDefinedResource->Des().Append( aCustomMode );
    }

// ----------------------------------------------------------------------------
// Get pen trail color
// ----------------------------------------------------------------------------
//
TInt CPeninputHwrfscnDataStore::GetPenTrailColor()
    {
    TInt newValue = 0; 
    iRepositorySetting->Get( KSettingsPenColor, newValue );
    
    if (!AknPenInputTrailColorUtils::CheckColorInColorTable(newValue))
        {
        return AknPenInputTrailColorUtils::GetTrailColorByTheme();
        }
    return newValue;
    }
// ----------------------------------------------------------------------------
// LoadAndPublishDefault
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::LoadAndPublishDefaultL()
    {
    if ( iRepositorySetting )
        {
        TInt newValue;

        iRepositorySetting->Get( KSettingsWritingSpeed, newValue );
        SetWritingSpeed( newValue );
        
        iRepositorySetting->Get( KSettingsPenWidth, newValue );
        TInt width = newValue;
        TInt height = newValue;
        SetBoxPenSize( TSize( width,height ) );

        newValue = GetPenTrailColor();        
        SetBoxPenColor( newValue );
        
        iRepositorySetting->Get( KSettingsPenInputLang, newValue );
        SetLanguageL( newValue );
        }
    }

// ----------------------------------------------------------------------------
// save range to repository if english or chinese
// ----------------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::SaveRange( TInt aRange )
    {
    if ( iRepositoryFep && ( aRange == ERangeNative || aRange == ERangeEnglish ) )
        {
        iRepositoryFep->Set( KAknFepLastUsedRange, aRange );
        }
    }

// ----------------------------------------------------------------------------
// Get rep value
// ----------------------------------------------------------------------------
//
TInt CPeninputHwrfscnDataStore::GetNewValue( 
    CAknFepRepositoryWatcher* aWatcher, TInt aId )
    {
    TInt newValue = -1;
    
    if ( aWatcher == iGSRepositoryWatcher )
        {
        iRepositorySetting->Get( aId, newValue );
        }
        
    return newValue;
    }

// ------------------------------------------------------------------
// Get next predictive candidate page
// -------------------------------------------------------------------
//
void CPeninputHwrfscnDataStore::GetNextPredictivePageL()
    {
    iHwrEngine->DoPredictiveL( KNullDesC, iPredictives, ETrue );
 
    TInt predictCount = PredictiveCount();
    if ( predictCount> 0 && predictCount < KMaxPredictiveCount )
        {
        AddPredictives();
        }
    }
    
// --------------------------------------------------------------------
// IsNumberOnly()
// -------------------------------------------------------------------
//  
TBool CPeninputHwrfscnDataStore::IsNumberOnly()
    {
    return ( ( iPermittedRanges.Count()==1 ) && 
        ( iPermittedRanges[0]==ERangeNumber ) );
    }
    
// --------------------------------------------------------------------
// Get number keymapping string
// --------------------------------------------------------------------
//   
HBufC* CPeninputHwrfscnDataStore::KeyMappingStringL() const
    {
    TLocale locale;
    TBuf<KMaxNumericString> rs = KNumberString();
    const TInt index = iKeyMappingIdList.Find( iCurrentNumberMode );
    
    if ( iCurrentNumberMode == EKeymapFromResource )
        {
        rs.Append( *iUserDefinedResource );
        }
    
    else if ( index != KErrNotFound )
        {
        HBufC* buf = iKeyMappingStringList[index];
        
        rs.Append( *buf );
        
        if ( ( iCurrentNumberMode == 
            EAknEditorCalculatorNumberModeKeymap ) ||
            ( iCurrentNumberMode == EAknEditorConverterNumberModeKeymap ) )
            {
            rs.Append( locale.DecimalSeparator() ); 
            }       
        }
        
    return rs.AllocL();          
    }

// ------------------------------------------------------------------------
// CPeninputHwrfscnDataStore::IsSpecialDisplayChars
// ------------------------------------------------------------------------
//       
TBool CPeninputHwrfscnDataStore::IsSpecialDisplayChars( const TDesC& aChar )
    {
    
    if( aChar.Compare( KGestureEnter ) == 0 
       || aChar.Compare( KDisplayBackspace ) == 0
       || aChar.Compare( KGestureSpace )== 0
       || aChar.Compare( KSmileFontCode ) == 0
       || aChar.Compare( KCryFontCode ) == 0 ) 
        {
        return ETrue;
        }
            
    return EFalse;
    }
    
// ------------------------------------------------------------------------
// CPeninputHwrfscnDataStore::ConvertDisplayChars
// ------------------------------------------------------------------------
//   
HBufC* CPeninputHwrfscnDataStore::ConvertDisplayChars( const TDesC& aChar )    
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
    else if( aChar.Compare( KSmileFontCode ) == 0 )
        {
        str.Append( KColon );
        str.Append( KHyphen );
        str.Append( KRightParenthesis );
        }
    else if( aChar.Compare( KCryFontCode ) == 0 )
        {
        str.Append( KColon );
        str.Append( KHyphen );
        str.Append( KLeftParenthesis );
        }
                 
    if( str.Length() > 0 )
    	{
    	convertedCan = str.Alloc();	
    	}
    	
    return convertedCan;
    }

// ----------------------------------------------------------------------------
// Get language show text
// ----------------------------------------------------------------------------
//
const TDesC& CPeninputHwrfscnDataStore::LanguageShowText()
    {
    return iLanguageShowText;
    }

// -----------------------------------------------------------------------------
// CPeninputHwrfscnDataStore::IsDirectlySentCandidate
// -----------------------------------------------------------------------------
//  
TBool CPeninputHwrfscnDataStore::IsDirectlySentCandidate( const TDesC& aChar )
    {
    
    if( aChar.Compare( KGestureEnter ) == 0 
      || aChar.Compare( KDisplayBackspace ) == 0 )
       {
       return ETrue;
       }
           
    return EFalse;
    }
TInt CPeninputHwrfscnDataStore::SetInputAreaSize(TSize& aSize)
    {
    return iHwrEngine->SetInputAreaSize(aSize);    
    }      
TInt CPeninputHwrfscnDataStore::SetScreenSize(TSize& aSize)
    {
    return iHwrEngine->SetScreenSize(aSize);    
    }      
// End Of File
