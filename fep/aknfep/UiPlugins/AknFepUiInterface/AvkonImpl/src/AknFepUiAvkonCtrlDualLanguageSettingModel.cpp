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
* Description:          
*
*/











#include    <PtiEngine.h>
#include <AknAppUi.h> //For CAknAppUi
#include <akntitle.h> //CAknTitlePane
#include    <aknfep.rsg>  //For R_SECONDARY_WRITING_LANG_NONE
#include <StringLoader.h>
#include <AknFepInternalCRKeys.h>
#include    <aknnotedialog.h>
#include    <avkon.rsg>
#include    <featmgr.h>
#include <e32property.h>
#include    <AvkonInternalCRKeys.h>
#include <aknenv.h>  
#include <AknSettingCache.h>
#include "AknFepUIAvkonCtrlDualLanguageSettingModel.h"

const TInt KAkFepDualLanguageMask = 0x03FF;
const TInt KAkFepDualLanguageBufSize128 = 128;

CAknFepDualLanguageSettingModel* CAknFepDualLanguageSettingModel::NewL()
    {
    CAknFepDualLanguageSettingModel* self = new( ELeave ) CAknFepDualLanguageSettingModel();
    CleanupStack::PushL( self );
    self->ConstructL();
    CleanupStack::Pop( self );
    return self;
    }

void CAknFepDualLanguageSettingModel::ConstructL()
    {
    iAknFepRepository = CRepository::NewL( KCRUidAknFep );
    }

CAknFepDualLanguageSettingModel::~CAknFepDualLanguageSettingModel()
    {
    if(iAknFepRepository)
        delete iAknFepRepository;
    }
void CAknFepDualLanguageSettingModel::GetSecondaryInputLanguagesL(
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
    
    HBufC* buffer = HBufC::NewLC( KAkFepDualLanguageBufSize128 );
    for ( TInt index = 1; index < count; ++index )
        {
        const TInt langId = languageCodeArray->At( index );

        TPtr ptrToBuffer( buffer->Des() );
        ptiEngine->GetLocalizedLanguageName( langId & KAkFepDualLanguageMask,
                                             ptrToBuffer );

        aLanguageNames->AppendL( ptrToBuffer );

        ptrToBuffer.Zero();
        }

    CleanupStack::PopAndDestroy( buffer );
    CleanupStack::PopAndDestroy( ptiEngine );
    }
void CAknFepDualLanguageSettingModel::ShowDualInputLanguageSettingListL(CCoeEnv* aCoeEnv)
    {
    CAknAppUi* appUi = static_cast<CAknAppUi*>(aCoeEnv->AppUi());

    CAknTitlePane* titlePane = static_cast<CAknTitlePane*>( appUi->StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );

    const TDesC* appTitle = titlePane->Text();
    HBufC* oldTitle = appTitle->AllocL();
    CleanupStack::PushL(oldTitle);
    HBufC* title = StringLoader::LoadLC( R_INPUT_LANGUAGE_SETTINGS_TITLE, aCoeEnv);
    titlePane->SetTextL( *title );
    CleanupStack::PopAndDestroy(title);
    
    //Launch dialog

    titlePane->SetTextL( *oldTitle );
    CleanupStack::PopAndDestroy(oldTitle);
    }


// ================= DATA ACCESS FUNCTIONS ==================

// ----------------------------------------------------------------------------
// CAknFepDualLanguageSettingModel::GetLanguagesFromPtiL
//
// Utility routine to get available languages with Symbian OS codes
// ----------------------------------------------------------------------------
//
void CAknFepDualLanguageSettingModel::GetLanguagesFromPtiL(
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

    HBufC* buffer = HBufC::NewLC( KAkFepDualLanguageBufSize128 );
    const TInt count = languageCodeArray->Count();
    for ( TInt index = 0; index < count; ++index )
        {
        const TInt langId = languageCodeArray->At( index );

        TPtr ptrToBuffer( buffer->Des() );
        ptiEngine->GetLocalizedLanguageName( langId & KAkFepDualLanguageMask,
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

TInt CAknFepDualLanguageSettingModel::LanguageL( TLanguageType aType )
    {
    TInt Language = KErrNone;
    switch ( aType )
        {
        case EAknFepDualLanguageSettingInputLanguage:
            {
            User::LeaveIfError(
                iAknFepRepository->Get( KAknFepInputTxtLang, Language ) );
            }
            break;
        case EAknFepDualLanguageSettingInputLanguageSecondary:
            {
            User::LeaveIfError(
                iAknFepRepository->Get( KAknFepInputTxtLangSecondary, Language ) );
            }
            break;
        default:
            break;
        }
    return Language;

    }


TBool CAknFepDualLanguageSettingModel::SetLanguageL( TLanguageType aType, TInt aLanguage )
    {
    TBool ret = ETrue;

    switch( aType )
        {
        case EAknFepDualLanguageSettingInputLanguage:
            {
            if( iAknFepRepository->Set( KAknFepInputTxtLang,
                                        aLanguage ) != KErrNone )
                {
                ret = EFalse;
                }
            
            if(ret)
                {
                // now inform all open apps of the switch
                TWsEvent event;
                event.SetType(KEikInputLanguageChange);
                CEikonEnv::Static()->WsSession().SendEventToAllWindowGroups(event);
                CAknEnv::Static()->SettingCache().Update(KEikInputLanguageChange);
                }
            
            // See if it cannot do predictive text entry
            if( !CheckDictionaryFromPtiL( aLanguage ) )
                {
                CAknNoteDialog* dlg = new (ELeave) CAknNoteDialog( );
                dlg->ExecuteDlgLD(
                    CAknNoteDialog::EShortTimeout,
                    CAknNoteDialog::ENoTone,
                    R_AVKON_T9_NOTE_NO_AVAILABLE );                                   
              
                }
            }
            break;
        case EAknFepDualLanguageSettingInputLanguageSecondary:
            {
            if( iAknFepRepository->Set( KAknFepInputTxtLangSecondary,
                                        aLanguage ) != KErrNone )
                {
                ret = EFalse;
                }
            if(ret)
                {
                // now inform all open apps of the switch
                // same event as primary input language change
                TWsEvent event;
                event.SetType(KEikInputLanguageChange);
                CEikonEnv::Static()->WsSession().SendEventToAllWindowGroups(event);
				CAknEnv::Static()->SettingCache().Update(KEikInputLanguageChange);
                }
            // Secondary input language does not need prediction
            // so we don't check for it
            }
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
TBool CAknFepDualLanguageSettingModel::CheckDictionaryFromPtiL( TInt aLangCode )
    {
    TBool langSupport = EFalse;

    if ( FeatureManager::FeatureSupported( KFeatureIdChinese ) ||
        FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
        {
        langSupport = ETrue;
        }
    else
        {
        CPtiEngine* ptiEngine = CPtiEngine::NewL();
        CleanupStack::PushL( ptiEngine );

        MPtiLanguage* ptiLang = ptiEngine->GetLanguage( aLangCode );
        if ( ptiLang )
            {
            // the check here should be based on the keyboard      
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

            }
        CleanupStack::PopAndDestroy( ptiEngine );
        ptiLang = NULL;
        }

    return langSupport;
    }
