/*
* Copyright (c) 2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  Data model for GSLangPlugin.
*
*/

// INCLUDE FILES

#include    "GSLangModel.h"
#include    "GSLangContainer.h"
#include    "GSLangPlugin.h"
#include    <gslangpluginrsc.rsg>

#include    <AknFepInternalCRKeys.h>
#include    <aknnotedialog.h>
#include    <aknradiobuttonsettingpage.h>
#include    <aknViewAppUi.h>
#include    <avkon.rsg>
#include    <CommonEngineDomainCRKeys.h> //Ui language
#include    <e32math.h>
#include    <featmgr.h>
#include    <hal.h>
#include    <PtiEngine.h>
#include    <ScreensaverInternalCRKeys.h>  //double declaration of KScreenSaverPluginName
#include    <settingsinternalcrkeys.h>
#include    <startupdomaincrkeys.h>
#include    <ctsydomainpskeys.h>
#include    <languages.hrh>

#include <AknFepGlobalEnums.h> //For chinese input modes

// For Phonetic Input of Hindi
#include <PtiIndicDefs.h>
#include <sysutil.h>
#ifdef RD_INTELLIGENT_TEXT_INPUT
#include    <AvkonInternalCRKeys.h>
#include <StringLoader.h>
#include <coemain.h>
#endif

#include <aknenv.h>  
#include <AknSettingCache.h>


//CONSTANTS
const TInt KGSDiskSpaceLimitForLanguageChange = 300000;

const TInt KFepChineseInputModeLength = 10;

// ================= MEMBER FUNCTIONS =======================

// ----------------------------------------------------------------------------
// CGSLangModel::NewL
//
// EPOC two-phased constructor
// ----------------------------------------------------------------------------
//
CGSLangModel* CGSLangModel::NewL( CGSLangContainer* aContainer )
    {
    CGSLangModel* self = new( ELeave ) CGSLangModel( aContainer );
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }


// ----------------------------------------------------------------------------
// CGSLangModel::CGSLangModel
//
// C++ default constructor can NOT contain any code, that
// might leave.
// ----------------------------------------------------------------------------
//
CGSLangModel::CGSLangModel( CGSLangContainer* aContainer )
    : iContainer( aContainer )
    {

    }


// ----------------------------------------------------------------------------
// CGSLangModel::ConstructL
//
// EPOC default constructor can leave.
// ----------------------------------------------------------------------------
//
void CGSLangModel::ConstructL()
    {
    InitializeCentralRepositoriesL();
    
    // start listening to CenRep key changes for ITL
    iNotifyHandlerForITL = CCenRepNotifyHandler::NewL( *this, 
                           *iAknFepRepository,
                           CCenRepNotifyHandler::EIntKey, 
                           KAknFepInputTxtLang );
    iNotifyHandlerForITL->StartListeningL();

    // start listening to CenRep key changes for T9 on/off
    iNotifyHandlerForT9 = CCenRepNotifyHandler::NewL( *this, 
                           *iAknFepRepository,
                           CCenRepNotifyHandler::EIntKey, 
                           KAknFepPredTxtFlag );
    iNotifyHandlerForT9->StartListeningL();
    
    if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
        {
        // start listening to CenRep key changes for Japanese predictive mode
        iNotifyHandlerForJapanesePredTxt = CCenRepNotifyHandler::NewL( *this, 
                               *iAknFepRepository,
                               CCenRepNotifyHandler::EIntKey, 
                               KAknFepJapanesePredTxtFlag );
        iNotifyHandlerForJapanesePredTxt->StartListeningL();
        }
    

#ifdef RD_INTELLIGENT_TEXT_INPUT
    // start listening to CenRep key changes for T9 on/off
    iNotifyHandlerForPredExt = CCenRepNotifyHandler::NewL( *this, 
                           *iAknFepRepository,
                           CCenRepNotifyHandler::EIntKey, 
                           KAknFepPredTxtFlagExtension );
    iNotifyHandlerForPredExt->StartListeningL();
#endif

    }


// ----------------------------------------------------------------------------
// CGSLangModel::~CGSLangModel
//
// Destructor
// ----------------------------------------------------------------------------
//
CGSLangModel::~CGSLangModel()
    {
    if ( iNotifyHandlerForITL )
        {
        iNotifyHandlerForITL->StopListening();
        }
    delete iNotifyHandlerForITL;
    
    if ( iNotifyHandlerForT9 )
        {
        iNotifyHandlerForT9->StopListening();
        }
    delete iNotifyHandlerForT9;
    
    if ( iNotifyHandlerForJapanesePredTxt )
        {
        iNotifyHandlerForJapanesePredTxt->StopListening();
        }
    delete iNotifyHandlerForJapanesePredTxt;
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if ( iNotifyHandlerForPredExt )
        {
        iNotifyHandlerForPredExt->StopListening();
        }
    delete iNotifyHandlerForPredExt;
#endif

    
    CloseCentralRepositories();
    }


// ----------------------------------------------------------------------------
// CGSLangModel::InitializeCentralRepositoriesL
//
// Creating and setting keys for the Central Repository
// ----------------------------------------------------------------------------
//
void CGSLangModel::InitializeCentralRepositoriesL()
    {

    iPersonalizationRepository =
        CRepository::NewL( KCRUidPersonalizationSettings );
    iLocaleRepository = CRepository::NewL( KCRUidLocaleSettings );
    iStartupConfRepository = CRepository::NewL( KCRUidStartupConf );
    iAknFepRepository = CRepository::NewL( KCRUidAknFep );
    iCommonEngineRepository = CRepository::NewL( KCRUidCommonEngineKeys );
    iScreensaverRepository = CRepository::NewL( KCRUidScreenSaver );

    }

// ----------------------------------------------------------------------------
// CGSLangModel::CloseCentralRepositoriesL
//
// Creating and setting keys for the Central Repository
// ----------------------------------------------------------------------------
//
void CGSLangModel::CloseCentralRepositories()
    {
    if ( iPersonalizationRepository )
        {
        delete iPersonalizationRepository;
        iPersonalizationRepository = NULL;
        }
    if ( iLocaleRepository )
        {
        delete iLocaleRepository;
        iLocaleRepository = NULL;
        }
    if ( iStartupConfRepository )
        {
        delete iStartupConfRepository;
        iStartupConfRepository = NULL;
        }
    if ( iAknFepRepository )
        {
        delete iAknFepRepository;
        iAknFepRepository = NULL;
        }
    if ( iCommonEngineRepository )
        {
        delete iCommonEngineRepository;
        iCommonEngineRepository = NULL;
        }
    if ( iScreensaverRepository )
        {
        delete iScreensaverRepository;
        iScreensaverRepository = NULL;
        }
    }

// ================= DATA ACCESS FUNCTIONS ==================

// ----------------------------------------------------------------------------
// CGSLangModel::GetLanguagesFromPtiL
//
// Utility routine to get available languages with Symbian OS codes
// ----------------------------------------------------------------------------
//
void CGSLangModel::GetLanguagesFromPtiL(
    CArrayFix<TInt>* aInstalledLangCodes,
    CArrayFix<TInt>* aSeedLanguages,
    CDesCArray* aLanguageNames )
    {
    CPtiEngine* ptiEngine = CPtiEngine::NewL();
    CleanupStack::PushL( ptiEngine );

    CArrayFix<TInt>* languageCodeArray = aSeedLanguages;
    if ( aInstalledLangCodes )
        {
        languageCodeArray = aInstalledLangCodes;
        }
    else
        {
        ptiEngine->GetAvailableLanguagesL( languageCodeArray );
        }

    HBufC* buffer = HBufC::NewLC( KGSBufSize128 );
    const TInt count = languageCodeArray->Count();
    for ( TInt index = 0; index < count; ++index )
        {
        const TInt langId = languageCodeArray->At( index );

        TPtr ptrToBuffer( buffer->Des() );
        ptiEngine->GetLocalizedLanguageName( langId & KGSLanguageMask,
                                             ptrToBuffer );

        if ( aInstalledLangCodes )
            {
            aSeedLanguages->AppendL( langId );
            }
        aLanguageNames->AppendL( ptrToBuffer );

        ptrToBuffer.Zero();
        }

    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PopAndDestroy( ptiEngine );
    }


// -----------------------------------------------------------------------------
// CGSLangModel::LanguageL
//
//
// -----------------------------------------------------------------------------
//
TInt CGSLangModel::LanguageL( TLanguage aType )
    {
    TInt Language = KErrNone;

    switch ( aType )
        {
        case EGSUiLanguage:
            {
            User::LeaveIfError(
                iCommonEngineRepository->Get( KGSDisplayTxtLang, Language ) );
            }
            break;
        case EGSInputLanguage:
            {
            User::LeaveIfError(
                iAknFepRepository->Get( KAknFepInputTxtLang, Language ) );
            }
            break;
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT
        case EGSInputLanguageSecondary:
            {
            User::LeaveIfError(
                iAknFepRepository->Get( KAknFepInputTxtLangSecondary, Language ) );
            }
            break;
#endif
#endif
        default:
            break;
        }

    return Language;
    }


// -----------------------------------------------------------------------------
// CGSLangModel::SetLanguageL
//
//
// -----------------------------------------------------------------------------
//
TBool CGSLangModel::SetLanguageL( TLanguage aType, TInt aLanguage )
    {
    TBool ret = ETrue;

    switch( aType )
        {
        case EGSUiLanguage:
            {
            // Never set Language code 0 to HAL
            if ( aLanguage !=0 )
                {
                if ( HAL::Set( HAL::ELanguageIndex, aLanguage ) != KErrNone )
                    {
                    ret = EFalse;
                    }
                }

            if ( iCommonEngineRepository->
                    Set( KGSDisplayTxtLang, aLanguage ) != KErrNone )
                {
                ret = EFalse;
                }
            
            TBool NbrModeSaved = EFalse;
            if ( aLanguage == ELangArabic || User::Language() == ELangArabic )
                {
                //numberMode = EGSNumberModeArabicIndic;
                SetDefaultNumberMode( EGSNbrModeArabic, 
                                      EGSNbrModeTypeArabic );
                NbrModeSaved = ETrue;
                }
            else if ( ( aLanguage == ELangUrdu || User::Language() == ELangUrdu ) || 
                      ( aLanguage == ELangFarsi || User::Language() == ELangFarsi ) )
                {
                //numberMode = EGSNumberModeEasternArabicIndic;
                SetDefaultNumberMode( EGSNbrModeLatin, 
                                      EGSNbrModeTypeEasternArabic );
                NbrModeSaved = ETrue;
                }
            else if ( aLanguage == ELangHindi || User::Language() == ELangHindi ||
                      aLanguage == ELangMarathi || User::Language() == ELangMarathi )
                {
                //numberMode = EGSNumberModeIndic;
                SetDefaultNumberMode( EGSNbrModeLatin, 
                                      EGSNbrModeTypeIndic );
                NbrModeSaved = ETrue;
                }
            
            //if number mode is not set above, then set it to Latin with respective
            //number mode types. This part might be executed when Automatic is
            //selected and the SIM card does not support the language.
            if ( !NbrModeSaved )
                {
                TInt nbrModeType = EGSNbrModeTypeIndic;
                if ( aLanguage == ELangArabic || User::Language() == ELangArabic )
                    {
                    nbrModeType = EGSNbrModeTypeArabic;
                    }
                else if ( ( aLanguage == ELangUrdu || User::Language() == ELangUrdu ) 
                     || ( aLanguage == ELangFarsi || User::Language() == ELangFarsi ) )
                    {
                    nbrModeType = EGSNbrModeTypeEasternArabic;
                    }
                    
                //EGSNumberModeLatin is true in both cases;
                SetDefaultNumberMode( EGSNbrModeLatin,
                                      nbrModeType );
                }
            }
            break;
        case EGSInputLanguage:
            {
            if( iAknFepRepository->Set( KAknFepInputTxtLang,
                                        aLanguage ) != KErrNone )
                {
                ret = EFalse;
                }
				
			if(ret)
                {
                //inform all other apps about language change.
                TWsEvent event;
                event.SetType( KEikInputLanguageChange  );
                CEikonEnv::Static()->WsSession().SendEventToAllWindowGroups(event);
                CAknEnv::Static()->SettingCache().Update(KEikInputLanguageChange);
                }

            // See if it cannot do predictive text entry
    		TBool checkEngine = EFalse;
    		
    		
    		if (!( aLanguage == ELangPrcChinese ||
         	       aLanguage == ELangHongKongChinese || 
         	       aLanguage == ELangTaiwanChinese ) 
         	   )
    			{
	    		if ( FeatureManager::FeatureSupported( KFeatureIdChinese ) )
		    		{
        			aLanguage = ELangEnglish;
        			checkEngine = ETrue;   			
    				}
	   			if (!CheckDictionaryFromPtiL( aLanguage, checkEngine))
    	            {
        	        CAknNoteDialog* dlg = new (ELeave) CAknNoteDialog( );
            	    dlg->ExecuteDlgLD(
                	    CAknNoteDialog::EShortTimeout,
                    	CAknNoteDialog::ENoTone,
	                    R_AVKON_T9_NOTE_NO_AVAILABLE );
                                    
    	             //SetT9OnOffMode(0);
        	         SetPredictiveOnOff( EGSLangOther, 0 );
            	    }
    			}
            }
            break;
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT
        case EGSInputLanguageSecondary:
            {
            if( iAknFepRepository->Set( KAknFepInputTxtLangSecondary,
                                        aLanguage ) != KErrNone )
                {
                ret = EFalse;
                }
            
            if(ret)
                {
                //inform all other apps about language change.
                TWsEvent event;
                event.SetType( KEikInputLanguageChange  );
                CEikonEnv::Static()->WsSession().SendEventToAllWindowGroups(event);
                CAknEnv::Static()->SettingCache().Update(KEikInputLanguageChange);
                }
            
            // Secondary input language does not need prediction
            // so we don't check for it
            }
            break;
#endif            
#endif
        default:
            break;
        }

    return ret;
    }


// -----------------------------------------------------------------------------
// CGSLangModel::DefaultNumberMode
//
//
// -----------------------------------------------------------------------------
//
TInt CGSLangModel::DefaultNumberMode( TInt aNbrModeType )
    {
    TInt value = KGSSettingOff;
    
    switch ( aNbrModeType )
        {
        case EGSNbrModeTypeArabic:
        case EGSNbrModeTypeEasternArabic:
            iLocaleRepository->Get( KSettingsDefaultNumberMode, value );
            break;
        case EGSNbrModeTypeIndic:
            iLocaleRepository->Get( KSettingsIndicDefaultNumberMode, value );
            break;
        default:
            break;
        }
      
    return value;
    }


// -----------------------------------------------------------------------------
// CGSLangModel::SetDefaultNumberMode
//
// Set default number mode for non-latin languagues
// -----------------------------------------------------------------------------
//
void CGSLangModel::SetDefaultNumberMode( TInt aMode, TInt aNbrModeType )
    {
    if ( aNbrModeType == EGSNbrModeTypeArabic ||
         aNbrModeType == EGSNbrModeTypeEasternArabic )
        {
        iLocaleRepository->Set( KSettingsDefaultNumberMode, aMode );
        }
    else
        {
        iLocaleRepository->Set( KSettingsIndicDefaultNumberMode, aMode );
        }

    TLocale locale;
    if ( aMode == EGSNbrModeLatin )
        {
        locale.SetDigitType( EDigitTypeWestern );
        }
    else
        {
        //if aMode <> EGSNbrModeLatin, then it should be either latin or arabic. However
        //as EGSNbrModeArabic and EGsNbrModeIndic both have a value = 1, we can't use
        //that constant for below comparison. Hence, need to depend on the 2nd param.
        switch ( aNbrModeType )
            {
            case EGSNbrModeTypeArabic:
                locale.SetDigitType( EDigitTypeArabicIndic );
                break;
            case EGSNbrModeTypeIndic:
                locale.SetDigitType( EDigitTypeDevanagari );
                break;
            case EGSNbrModeTypeEasternArabic:
                locale.SetDigitType( EDigitTypeEasternArabicIndic );
                break;
            default:
                break;
            }
        }

    locale.Set();
    }


// -----------------------------------------------------------------------------
// CGSLangModel::PredictiveOnOff
//
//
// -----------------------------------------------------------------------------
//
TInt CGSLangModel::PredictiveOnOff( TPredictiveType aPredictive )
    {
    TInt onOffMode = KGSPredTxtFlagDefault;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TInt onOffBitMask = KGSPredTxtBitMaskDefault;
#endif

    switch( aPredictive )
        {
        case EGSLangJapanese:
            {
            if( iAknFepRepository->Get( KAknFepJapanesePredTxtFlag,
                                        onOffMode ) != KErrNone )
                {
                iAknFepRepository->Set( KAknFepJapanesePredTxtFlag,
                                        onOffMode );
                }
            }
            break;
        case EGSLangOther:
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT
            if ( iAknFepRepository->Get( KAknFepPredTxtFlagExtension,
                    onOffBitMask ) != KErrNone )
                {
                iAknFepRepository->Set( KAknFepPredTxtFlagExtension, onOffBitMask );
                }

            // Parse keyboard dependent settings for current keyboard from bitmask
            // We'll only use this function to get prediction value if we have one
            // keyboard on the device, so we can just check the current keyb. layout
            TInt keyboardLayout = 0;
            RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
            TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
            TInt predTmp = onOffBitMask;
            switch(layout)
                {
                case EPtiKeyboardNone:
                    break;
                case EPtiKeyboard12Key:
                    predTmp &= EKeyboardStyle12Key;
                    break;
                case EPtiKeyboardHalfQwerty:
                    predTmp &= EKeyboardStyleHalfQwerty;
                    break;
                case EPtiKeyboardQwerty4x12:
                case EPtiKeyboardQwerty4x10:
                case EPtiKeyboardQwerty3x11:
                case EPtiKeyboardCustomQwerty:
                    predTmp &= EKeyboardStyleQwerty;
                    break;
                default:
                    break;
                }
            onOffMode = ( predTmp ? 1 : 0);

#else
            if ( iAknFepRepository->Get( KAknFepPredTxtFlag,
                                         onOffMode ) != KErrNone )
                {
                iAknFepRepository->Set( KAknFepPredTxtFlag, onOffMode );
                }
#endif
            }
            break;
        default:
            break;
        }

    return onOffMode;
    }


// -----------------------------------------------------------------------------
// CGSLangModel::SetPredictiveOnOff
//
//
// -----------------------------------------------------------------------------
//
TBool CGSLangModel::SetPredictiveOnOff( TPredictiveType aPredictive,
                                        TInt aMode )
    {
    TBool ret = EFalse;
    switch( aPredictive )
        {
        case EGSLangJapanese:
            {
            if ( iAknFepRepository->Set( KAknFepJapanesePredTxtFlag,
                                         aMode ) != KErrNone )
                {
                ret = ETrue;
                }
            }
            break;
        case EGSLangOther:
            {
#ifdef RD_INTELLIGENT_TEXT_INPUT
            TInt predFlag = 0;
            iAknFepRepository->Get(KAknFepPredTxtFlagExtension, predFlag);

            // Parse keyboard dependent settings for current keyboard from bitmask
            // We'll only use this function to set prediction value if we have one
            // keyboard on the device, so we can just check the current keyb. layout
            TInt keyboardLayout = 0;
            RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
            TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
            switch(layout)
                {
                case EPtiKeyboardNone:
                    break;
                case EPtiKeyboard12Key:
                    if ( aMode )
                        {
                        predFlag |= EKeyboardStyle12Key;
                        }
                    else
                        {
                        predFlag &= ~EKeyboardStyle12Key;
                        }
                    break;
                case EPtiKeyboardHalfQwerty:
					if ( aMode )
                        {
                        predFlag |= EKeyboardStyleHalfQwerty;
                        }
                    else
                        {
                        predFlag &= ~EKeyboardStyleHalfQwerty;
                        }
                    break;
                    
                case EPtiKeyboardQwerty4x12:
                case EPtiKeyboardQwerty4x10:
                case EPtiKeyboardQwerty3x11:
                
                case EPtiKeyboardCustomQwerty:
                    if (aMode)
                        {
                        predFlag |= EKeyboardStyleQwerty;
                        }
                    else
                        {
                        predFlag &= ~EKeyboardStyleQwerty;
                        }
                    break;
                default:
                    break;
                }
            iAknFepRepository->Set(KAknFepPredTxtFlagExtension, predFlag);
            iAknFepRepository->Set(KAknFepPredTxtFlag, aMode); // For backwards compatibility
            }
#else
            if( iAknFepRepository->Set( KAknFepPredTxtFlag,
                                        aMode ) != KErrNone )
                {
                ret = ETrue;
                }
            }
#endif
            break;
        default:
            break;
        }

    return ret;
    }


// ----------------------------------------------------------------------------
// CGSLangModel::CheckDictionaryFromPtiL
//
// Checks if the selected language supports predictive text input.
// In case this returns EFalse, the UI display's "Not available"
// for Predictive text input.
// ----------------------------------------------------------------------------
//
TBool CGSLangModel::CheckDictionaryFromPtiL( TInt aLangCode, TBool aCheckEngine)
    {
    TBool langSupport = EFalse;

    if (!aCheckEngine && (FeatureManager::FeatureSupported( KFeatureIdChinese ) ||
        FeatureManager::FeatureSupported( KFeatureIdJapanese ) ))
        {
        langSupport = ETrue;
        }
    else
        {
        CPtiEngine* ptiEngine = CPtiEngine::NewL();
        CleanupStack::PushL( ptiEngine );

        MPtiLanguage* ptiLang = ptiEngine->GetLanguage( aLangCode & KGSLanguageMask );
        if ( ptiLang )
			{
	        // the check here should be based on the keyboard
#ifdef RD_INTELLIGENT_TEXT_INPUT	        
	        TInt keyboardLayout = 0;
	        RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
	        TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
	        switch(layout)
	            {
	            case EPtiKeyboardHalfQwerty:
	                langSupport = ptiLang->HasInputMode( (TPtiEngineInputMode)EPtiEngineHalfQwertyPredictive);
	                break;
	            case EPtiKeyboardQwerty4x12:
	            case EPtiKeyboardQwerty4x10:  
	            case EPtiKeyboardQwerty3x11:
	            case EPtiKeyboardCustomQwerty:
	                langSupport = ptiLang->HasInputMode( (TPtiEngineInputMode)EPtiEngineQwertyPredictive);
	                break;
	            case EPtiKeyboardNone:
	            case EPtiKeyboard12Key:
	                langSupport = ptiLang->HasInputMode( (TPtiEngineInputMode)EPtiEnginePredictive);
	                break;
	            default:
	                langSupport = ptiLang->HasInputMode( (TPtiEngineInputMode)EPtiEnginePredictive);
	                break;    
	            }
#else
	       langSupport = ptiLang->HasInputMode( (TPtiEngineInputMode)EPtiEnginePredictive);     
#endif	       
			}
        CleanupStack::PopAndDestroy( ptiEngine );
        ptiLang = NULL;
        }

    return langSupport;
    }

// ----------------------------------------------------------------------------
// CGSLangModel::CheckT9FromPtiLForPrediction
//
// Checks if the selected language supports predictive text input.
// In case this returns EFalse, the UI display's "Predictive text Not available 
// for selected language" for Predictive text input.
// ----------------------------------------------------------------------------
//

#ifdef RD_INTELLIGENT_TEXT_INPUT
TBool CGSLangModel::CheckT9FromPtiLForPredictionL(TPtiEngineInputMode inputMode, TInt aLangCode)
    {
    TBool langSupport=EFalse;
    
    CPtiEngine* ptiEngine = CPtiEngine::NewL();
    CleanupStack::PushL( ptiEngine );
    
    MPtiLanguage* ptiLang = NULL;
    if(0 == aLangCode)
    iAknFepRepository->Get( KAknFepInputTxtLang, aLangCode );    
    if ( aLangCode == ELangPrcChinese || 
	     aLangCode == ELangHongKongChinese || 
		 aLangCode == ELangTaiwanChinese )
        {
        aLangCode = ELangEnglish;
        }
        
    ptiLang = ptiEngine->GetLanguage( aLangCode );
    if ( ptiLang )
       {
         langSupport = ptiLang->HasInputMode( inputMode );
       }
    
    CleanupStack::PopAndDestroy( ptiEngine );
    ptiLang = NULL;

    return langSupport;
	}
#endif


// -----------------------------------------------------------------------------
// CGSLangModel::DefaultInputMethodL
//
//
// -----------------------------------------------------------------------------
//
TInt CGSLangModel::DefaultInputMethodL()
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdChinese ) )
        {
        // fetch the setting value from shared data etc.
        // return the value
        TUint MethodItem;
        _LIT(Kx, "x");
        // This conversion is needed because KAknFepChineseInputMode cenrep key original type was 16bit int.
        // now type is changed to string, so that it can accommodate bigger values like EHangul 0x16000. 
        TBuf<KFepChineseInputModeLength> conversion;
        User::LeaveIfError( iAknFepRepository->
                    Get( KAknFepChineseInputMode, conversion ) );
        TInt len = conversion.Find(Kx);
        TLex lex;
        
        if(len)
            {
            TPtrC ptr = conversion.Mid(len +1);
            lex.Assign(ptr);
            }
        else
            {
            lex.Assign(conversion);
            }
        
        lex.Val(MethodItem, EHex);
        return MethodItem;
        }
    else
        {
        return KErrNotSupported;
        }
    }


// -----------------------------------------------------------------------------
// CGSLangModel::SetDefaultInputMethodL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangModel::SetDefaultInputMethodL( TInt aMethod )
    {
    if ( FeatureManager::FeatureSupported( KFeatureIdChinese ) )
        {
        TBuf<KFepChineseInputModeLength> conversion;
        conversion.Num(aMethod, EHex);

        User::LeaveIfError(
                    iAknFepRepository->Set( KAknFepChineseInputMode, conversion ) );

        // See if it cannot do predictive text entry
        if( !CheckDictionaryFromPtiL( aMethod ) )
            {
            SetPredictiveOnOff( EGSLangOther, 0 );
            }
        }
    }


// -----------------------------------------------------------------------------
// CGSLangModel::ChangePredictiveOnOffSettingL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangModel::ChangePredictiveOnOffSettingL( TBool aShowOnOffSettingPage,
                                                  TPredictiveType aPredictive )
    {
    TInt currentValue = PredictiveOnOff( aPredictive );
    TBool updateValue = ETrue;

    // Following code potentially changes the value of currrent value
    if ( aShowOnOffSettingPage )
        {
        updateValue = ShowPredictiveOnOffSettingPageL( currentValue );        
        }
    else
        {
        currentValue = !PredictiveOnOff( aPredictive );
        }

    if ( updateValue )
        {
        // Treat on and off separately for clarity
        if ( currentValue )
            {
            // See if it can do predictive text entry
            if( CheckDictionaryFromPtiL( LanguageL( EGSInputLanguage ) ) )
                {
                SetPredictiveOnOff( aPredictive, currentValue );
                }
            else
                {
                CAknNoteDialog* dlg = new (ELeave) CAknNoteDialog( );
                dlg->ExecuteDlgLD(
                    CAknNoteDialog::EShortTimeout,
                    CAknNoteDialog::ENoTone,
                    R_AVKON_T9_NOTE_NO_AVAILABLE );
                }
            }
        else
            {
            // We are always able to turn T9 off:
            SetPredictiveOnOff( aPredictive, currentValue );
            }
        // value should really be being updated, so update listbox
        if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
            {
            // Update English Predictive list item
            if ( aPredictive == CGSLangModel::EGSLangJapanese )
                {
                iContainer->UpdateListBoxL( EGSLangIdJPT );
                }
            else
                {
                iContainer->UpdateListBoxL( EGSLangIdEPT );
                }
            }
        else
            {
            //EGSLangIdT9L
            iContainer->UpdateListBoxL( iContainer->CurrentFeatureId() );
            }
        }
    }


// -----------------------------------------------------------------------------
// CGSLangModel::ShowPredictiveOnOffSettingPageL
//
//
// -----------------------------------------------------------------------------
//
TBool CGSLangModel::ShowPredictiveOnOffSettingPageL( TInt& aOnOffcurrent )
    {
    // Default is to act as if the user cancelled the setting page
    TBool ret = EFalse;

    if ( CheckDictionaryFromPtiL( LanguageL( EGSInputLanguage ) ) )
        {
        CDesCArrayFlat* items =
            CCoeEnv::Static()->ReadDesC16ArrayResourceL(
                               R_T9_ON_OFF_SETTING_PAGE_LBX );

        CleanupStack::PushL( items );

        TInt dispOnOff = !aOnOffcurrent;

        CAknRadioButtonSettingPage* dlg =
            new ( ELeave ) CAknRadioButtonSettingPage(
                           R_T9_ON_OFF_SETTING_PAGE, dispOnOff, items );
        ret = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
        if ( ret )
            {
            aOnOffcurrent = !dispOnOff;
            }
        CleanupStack::PopAndDestroy( items );
        }
    else
        {
        CAknNoteDialog* dlg = new( ELeave ) CAknNoteDialog();
        dlg->ExecuteDlgLD( CAknNoteDialog::EShortTimeout,
                           CAknNoteDialog::ENoTone,
                           R_AVKON_T9_NOTE_NO_AVAILABLE );
        }
    return ret;
    }


// -----------------------------------------------------------------------------
// CGSLangModel::CheckCallStateL
//
//
// -----------------------------------------------------------------------------
//
TInt CGSLangModel::CheckCallStateL()
    {
    TInt callState = KErrNone;

    //here are open connections or not
    RProperty::Get( KPSUidCtsyCallInformation,
                    KCTsyCallState,
                    callState );
    return callState;
    }


// -----------------------------------------------------------------------------
// CGSLangModel::HandleNotifyInt
//
//
// -----------------------------------------------------------------------------
//
void CGSLangModel::HandleNotifyInt( TUint32 aId, TInt /*aNewValue*/ )
    {
    TRAP_IGNORE( HandleCenrepValueChangedL( aId ) );
    }


// -----------------------------------------------------------------------------
// CGSLangModel::HandleCenrepValueChangedL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangModel::HandleCenrepValueChangedL( TUint32 aId )
    {
    if ( aId == KAknFepInputTxtLang )
        {
        iContainer->UpdateListBoxL( EGSLangIdITL );
        iContainer->UpdateListBoxL( EGSLangIdT9L );
        
        TInt inputLanguage;
        iAknFepRepository->Get( KAknFepInputTxtLang, inputLanguage );
        //Updating default input method based on input text language
	      //Chinese specific feature 
	      TBool listboxUpdateReqd(EFalse);
	      if ( inputLanguage == ELangPrcChinese )
	          {
	          SetDefaultInputMethodL( EPinyin );
	          listboxUpdateReqd = ETrue;
	          }
	      else if ( inputLanguage == ELangHongKongChinese )
	          {
	          SetDefaultInputMethodL( EStroke );
	          listboxUpdateReqd = ETrue;
	          }
	      else if ( inputLanguage == ELangTaiwanChinese )
	          {
	          SetDefaultInputMethodL( EZhuyin );
	          listboxUpdateReqd = ETrue;
	          }
	      if (listboxUpdateReqd)
	          {
	          iContainer->UpdateListBoxL( EGSLangIdDIM );
	          }
#ifdef RD_HINDI_PHONETIC_INPUT
        //check if this is Hindi Phonetic - hide T9 item if it is.
        if( inputLanguage == KLangHindiPhonetic )
            {
            iContainer->SetT9ItemVisibilityL( EFalse );
            }
        else
            {
            iContainer->SetT9ItemVisibilityL( ETrue );
            }
#endif //RD_HINDI_PHONETIC_INPUT 
        }
    else if ( aId == KAknFepPredTxtFlag
#ifdef RD_INTELLIGENT_TEXT_INPUT
         || aId == KAknFepPredTxtFlagExtension
#endif
        )
        {
        iContainer->UpdateListBoxL( EGSLangIdT9L );
        iContainer->UpdateListBoxL( EGSLangIdEPT );
        }
    
    else if ( aId == KAknFepJapanesePredTxtFlag)
        {
        iContainer->UpdateListBoxL( EGSLangIdJPT);
        }
    
    else if ( aId == KSettingsIndicDefaultNumberMode )
        {
        iContainer->UpdateListBoxL( EGSLangHindiNumberMode );
        }
    }


// ----------------------------------------------------------------------------
// CGSLangModel::KeypressTimeout
// 
// Get Keypress timeout (means Multitap timer).
// ----------------------------------------------------------------------------
//
TInt CGSLangModel::KeypressTimeout()
    {
    TInt msec;    
    if ( iAknFepRepository->Get( KAknFepMultiTapTimer, msec ) != KErrNone )
        {
        msec = KGSMultiTapTimerDefault;
        iAknFepRepository->Set( KAknFepMultiTapTimer, msec );
        }
    
    return msec;
    }

// ----------------------------------------------------------------------------
// CGSLangModel::SetKeypressTimeout
// 
// Writes Keypress timeout (means Multitap timer).
// ----------------------------------------------------------------------------
//
TBool CGSLangModel::SetKeypressTimeout( TInt aMilliSecond )
    {
    TBool ret = EFalse;
    if ( iAknFepRepository->Set( KAknFepMultiTapTimer, aMilliSecond ) 
         == KErrNone )
        {
        ret = ETrue;
        }
    
    return ret;
    }

// ----------------------------------------------------------------------------
// CGSLangModel::JapanQwertyFlags
// 
// Get setting value of each apanese qwerty specific flags
// ----------------------------------------------------------------------------
//
TBool CGSLangModel::JapanQwertyFlags( const TGSJapQwertyFlags aFlag )
    {
    TInt value;
    if ( iAknFepRepository->Get( KAknFepJapaneseSpecialCharFlag, value )
        != KErrNone )
        {
        value = KGSJapaneseSpecialCharFlagDefault;
        iAknFepRepository->Set( KAknFepJapaneseSpecialCharFlag, value );
        }
    
    return ( value&aFlag );
    }

// ----------------------------------------------------------------------------
// CGSLangModel::SetJapanQwertyFlags
// 
// Set value of each Japanese qwerty specific flags
// ----------------------------------------------------------------------------
//
TBool CGSLangModel::SetJapanQwertyFlags( const TGSJapQwertyFlags aFlag,
                                         TBool aValue )
    {
    TBool ret = EFalse;
    TInt value;
    if ( iAknFepRepository->Get( KAknFepJapaneseSpecialCharFlag, value )
        == KErrNone )
        {
        if ( aValue )
            {
            value |= aFlag;
            }
        else
            {
            value &= ~( aFlag );
            }
        if ( iAknFepRepository->Set( KAknFepJapaneseSpecialCharFlag, value ) 
             == KErrNone )
            {
            ret = ETrue;
            }
        }
    
    return ret;
    }


// ----------------------------------------------------------------------------
// CGSLangModel::ChiCangJieInput
// 
// Get Chinese CangJie input method value
// ----------------------------------------------------------------------------
//
TInt CGSLangModel::ChiCangJieInputL()
    {
    TInt value;
    User::LeaveIfError( iAknFepRepository->
            Get( KAknFepCangJieMode, value ) );
    
    return value;
    }

// ----------------------------------------------------------------------------
// CGSLangModel::SetChiCangJieInput
// 
// Set Chinese CangJie input method value
// ----------------------------------------------------------------------------
//    
void CGSLangModel::SetChiCangJieInputL( TInt aMode )
    {
    User::LeaveIfError(
            iAknFepRepository->Set( KAknFepCangJieMode, aMode ) );
    }


// ----------------------------------------------------------------------------
// CGSLangModel::CheckCangjieSupportL
// 
// Check if the Cangjie is supported by the language 
// @param aLangCode i.e. Code for the language
// ----------------------------------------------------------------------------
//    
TBool CGSLangModel:: CheckCangjieSupportL( TInt aLangCode )
    {
    TBool langSupport=EFalse;
    CPtiEngine* ptiEngine = CPtiEngine::NewL();
    CleanupStack::PushL( ptiEngine );

    MPtiLanguage* ptiLang = NULL;
    //Check if HK variant support CangJie input mode
    TPtiEngineInputMode inputMode = EPtiEngineNormalCangjieQwerty;
        
    //Chinese languages do not need a mask.
    ptiLang = ptiEngine->GetLanguage( aLangCode );
    if ( ptiLang )
        {
        langSupport = ptiLang->HasInputMode( inputMode );
        }

    CleanupStack::PopAndDestroy( ptiEngine );
    ptiLang = NULL;
    return langSupport;
    }

// ----------------------------------------------------------------------------
// CGSLangModel::PredictiveModeL
// 
// Get Predictive mode value 
// 0 --> Normal 
// 1 --> Autocompletion
// ----------------------------------------------------------------------------
//
TInt CGSLangModel::PredictiveModeL()
    {
    TInt value = 0;
   	#ifndef RD_INTELLIGENT_TEXT_INPUT
   		User::LeaveIfError( iAknFepRepository->
       		    Get(  KAknFepAutoCompleteFlag, value ) );
   	#endif
    return value;
    }

// ----------------------------------------------------------------------------
// CGSLangModel::SetPredictiveModeL
// 
// Get Predictive mode value 
// 0 --> Normal 
// 1 --> Autocompletion
// ----------------------------------------------------------------------------
//
void CGSLangModel::SetPredictiveModeL( TInt aMode )
    {
   	#ifndef RD_INTELLIGENT_TEXT_INPUT
   		User::LeaveIfError( iAknFepRepository->
       		    Set(  KAknFepAutoCompleteFlag, aMode ) );
    #endif
    }

// ----------------------------------------------------------------------------
// CGSLangModel::CheckAutoWordCompletionSupportL
// 
// Checking autoword completion feature support
// ----------------------------------------------------------------------------
//
TBool CGSLangModel::CheckAutoWordCompletionSupportL()
    {
    TBool langSupport=EFalse;
    
   	#ifndef RD_INTELLIGENT_TEXT_INPUT
        CPtiEngine* ptiEngine = CPtiEngine::NewL();
        CleanupStack::PushL( ptiEngine );
    
        MPtiLanguage* ptiLang = NULL;
        TPtiEngineInputMode inputMode = EPtiEngineWordCompletion;
        
        TInt aLangCode = 0;
        User::LeaveIfError( iAknFepRepository->Get( KAknFepInputTxtLang, aLangCode ) );
        
        ptiLang = ptiEngine->GetLanguage( aLangCode );
        if ( ptiLang )
            {
            langSupport = ptiLang->HasInputMode( inputMode );
            }
    
        CleanupStack::PopAndDestroy( ptiEngine );
        ptiLang = NULL;
    #endif //RD_INTELLIGENT_TEXT_INPUT
    
    return langSupport;
    }

// ----------------------------------------------------------------------------
// CGSLangModel::CheckDiskSpaceL
// 
// Checking if there is enough free space for language switch
// ----------------------------------------------------------------------------
//
TBool CGSLangModel::CheckDiskSpaceL()
    {
    // Check if there is enough space in disk
    if ( SysUtil::FFSSpaceBelowCriticalLevelL( NULL, 
                                  KGSDiskSpaceLimitForLanguageChange ) )
        {
        return ETrue;
        }
    else
        {
        return EFalse;
        }
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
TBool CGSLangModel::DeviceHasMultipleKeyboardsL() 
    {
    TInt physicalKeyboards = 0;
    User::LeaveIfError( iAknFepRepository->Get(  KAknFepPhysicalKeyboards, physicalKeyboards ));
    // Check virtual keyboards
    if ( FeatureManager::FeatureSupported(KFeatureIdVirtualItutInput) )
        {
        physicalKeyboards = physicalKeyboards | 0x01;
        }
    if ( FeatureManager::FeatureSupported(KFeatureIdVirtualFullscrQwertyInput) )
        {
        physicalKeyboards = physicalKeyboards | 0x02;
        }
    // Check virtual keyboards end
    TBool firstFound = EFalse;
    for(TUint i = 0x01; i <= 0xF0; i = i << 1)
        {
        if(physicalKeyboards & i)
            {
            if (firstFound)
                {
                return ETrue;
                }
            else
                {
                firstFound = ETrue;
                }
            }
        }
    return EFalse;
    }

TInt CGSLangModel::PredictionBitMaskL()
    {
    TInt value = 0;
    User::LeaveIfError( iAknFepRepository->Get( KAknFepPredTxtFlagExtension, value ) );
    return value;
    }

void CGSLangModel::SetPredictionBitMaskL(TInt aBitMask)
    {
    User::LeaveIfError( iAknFepRepository->Set( KAknFepPredTxtFlagExtension, aBitMask ) );
    }

TInt CGSLangModel::PhysicalKeyboardsL()
    {
    TInt physicalKeyboards = 0;
    User::LeaveIfError( iAknFepRepository->Get(  KAknFepPhysicalKeyboards, physicalKeyboards ));
    // Check virtual keyboards
    if ( FeatureManager::FeatureSupported(KFeatureIdVirtualItutInput) )
        {
        physicalKeyboards = physicalKeyboards | 0x01;
        }
    if ( FeatureManager::FeatureSupported(KFeatureIdVirtualFullscrQwertyInput) )
        {
        physicalKeyboards = physicalKeyboards | 0x02;
        }
    // Check virtual keyboards end
    return physicalKeyboards;
    }

void CGSLangModel::GetSecondaryInputLanguagesL(
              CArrayFix<TInt>* aSeedLanguages,
              CDesCArray* aLanguageNames,
              CCoeEnv* aCoeEnv)
    {
    CPtiEngine* ptiEngine = CPtiEngine::NewL();
    CleanupStack::PushL( ptiEngine );

    CArrayFix<TInt>* languageCodeArray = aSeedLanguages;
    ptiEngine->GetAvailableLanguagesL( languageCodeArray );
    languageCodeArray->InsertL(0,0); // No secondary input language as the first item
    
    TInt count = languageCodeArray->Count();
    for ( TInt index = 0; index < count; ++index )
        {
        const TInt langCode = languageCodeArray->At(index);
        if(langCode == ELangTaiwanChinese ||
           langCode == ELangHongKongChinese ||
           langCode == ELangPrcChinese ||
           langCode == ELangJapanese ||
           langCode == ELangKorean )
            {
            languageCodeArray->Delete(index);
            count--;
            index--;
            }
        }

    HBufC* noneString = StringLoader::LoadLC( R_SECONDARY_WRITING_LANG_NONE, aCoeEnv);
    TPtr nonePtr( noneString->Des() );
    aLanguageNames->AppendL(nonePtr);
    CleanupStack::PopAndDestroy( noneString );
    
    HBufC* buffer = HBufC::NewLC( KGSBufSize128 );
    for ( TInt index = 1; index < count; ++index )
        {
        const TInt langId = languageCodeArray->At( index );

        TPtr ptrToBuffer( buffer->Des() );
        ptiEngine->GetLocalizedLanguageName( langId & KGSLanguageMask,
                                             ptrToBuffer );

        aLanguageNames->AppendL( ptrToBuffer );

        ptrToBuffer.Zero();
        }

    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PopAndDestroy( ptiEngine );
    }
#endif

//  End of File
