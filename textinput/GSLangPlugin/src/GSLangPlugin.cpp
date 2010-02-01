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
* Description:  GSLangPlugin implementation.
*
*/


// Includes
#include "GSLangPlugin.h"
#include "GSLangContainer.h"
#include "GSLangModel.h"
#include <gsfwviewuids.h>
#include <GSLangPluginRsc.rsg>
#include <gslistbox.h>
#include <gsprivatepluginproviderids.h>

#include <AknDateFormatUtils.h>
#include <AknFepGlobalEnums.h>  // EPinyin
#include <aknnotewrappers.h>
#include <aknradiobuttonsettingpage.h>
#include <aknsettingpage.h>
#include <akntextsettingpage.h>
#include <aknViewAppUi.h>
#include <AknWaitDialog.h>
#include <eikapp.h>
#include <featmgr.h>
#include <hlplch.h> // HlpLauncher
#include <PSVariables.h>
#include <PtiEngine.h>
#include <StringLoader.h>
#include <syslangutil.h>        // Display UI language
#include <ctsydomainpskeys.h>
#include <SATDomainPSKeys.h>
#include <e32property.h>
#ifdef RD_INTELLIGENT_TEXT_INPUT
#include "PredictiveSettingDialog.h"
#include <AknFep.rsg>
#include <akntitle.h>
#ifdef FF_DUAL_LANGUAGE_SUPPORT
#include "DualLanguageDialog.h"
#endif
#include <aknnotedialog.h>          //CAknNoteDialog 
#include <AvkonInternalCRKeys.h> 
#endif
#include <gslangplugin.mbg>

#ifndef RD_STARTUP_CHANGE
    #include <sysstartup.h>         // Startup reasons
#else
    #include <starterclient.h>      // Startup reasons
#endif // RD_STARTUP_CHANGE

// Constants

// ========================= MEMBER FUNCTIONS ================================

// ---------------------------------------------------------------------------
// CGSLangPlugin::CGSLangPlugin()
// Constructor
//
// ---------------------------------------------------------------------------
//
CGSLangPlugin::CGSLangPlugin():
    iCascadeOptionsOpen(EFalse),
    iDialog(NULL)
    {
    }


// ---------------------------------------------------------------------------
// CGSLangPlugin::~CGSLangPlugin()
// Destructor
//
// ---------------------------------------------------------------------------
//
CGSLangPlugin::~CGSLangPlugin()
    {
    FeatureManager::UnInitializeLib();
    if( iContainer )
        {
        AppUi()->RemoveFromViewStack( *this, iContainer );
        delete iContainer;
        }
    if(iDialog)
        {
       delete iDialog;
        }
    
#ifdef RD_INTELLIGENT_TEXT_INPUT    
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__       
    if (iKeyboardLayoutStatusSubscriber)
        {
        iKeyboardLayoutStatusSubscriber->StopSubscribe();
        }
    iKeyboardLayoutStatusProperty.Close();
    delete iKeyboardLayoutStatusSubscriber;     
#endif    
#endif    
    }


// ---------------------------------------------------------------------------
// CGSLangPlugin::ConstructL(const TRect& aRect)
// Symbian OS two-phased constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CGSLangPlugin::ConstructL()
    {
    FeatureManager::InitializeLibL();
    OpenLocalizedResourceFileL( KGSLangPluginResourceFileName,
                                iResourceLoader );
    BaseConstructL( R_GS_LANG_VIEW );
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__   
    //Attach the P&S property to the keyboardlayout and subscribe to the P&S framework
    // for any keyboard layout change.
    User::LeaveIfError(iKeyboardLayoutStatusProperty.Attach(KCRUidAvkon, KAknKeyBoardLayout));    
    iKeyboardLayoutStatusSubscriber = new (ELeave) CSubscriber(
                                         TCallBack(HandleKeyboardLayoutChangeNotification, this), 
                                         iKeyboardLayoutStatusProperty);    
    iKeyboardLayoutStatusSubscriber->SubscribeL();
#endif
#endif 
    }


// ---------------------------------------------------------------------------
// CGSLangPlugin::NewL()
// Static constructor
//
// ---------------------------------------------------------------------------
//
CGSLangPlugin* CGSLangPlugin::NewL( TAny* /*aInitParams*/ )
    {
    CGSLangPlugin* self = new(ELeave) CGSLangPlugin();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::Id
//
//
// -----------------------------------------------------------------------------
//
TUid CGSLangPlugin::Id() const
    {
    return KGSLangPluginUID;
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::HandleClientRectChange
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::HandleClientRectChange()
    {
    if ( iContainer )
        {
        iContainer->SetRect( ClientRect() );
        }
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::DoActivateL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::DoActivateL( const TVwsViewId& aPrevViewId,
                                 TUid aCustomMessageId,
                                 const TDesC8& aCustomMessage )
    {
    CGSBaseView::DoActivateL( aPrevViewId, aCustomMessageId, aCustomMessage );
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::DoDeactivate
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::DoDeactivate()
    {
    CGSBaseView::DoDeactivate();
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::HandleCommandL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::HandleCommandL( TInt aCommand )
    {
    switch ( aCommand )
        {
        case EGSMSKCmdAppChange:
            HandleListBoxSelectionL();
            break;
        case EGSCmdAppChange:
            if ( Container()->CurrentFeatureId() == EGSLangIdT9L )
                {
#ifdef RD_INTELLIGENT_TEXT_INPUT
                // Launch Predictive mode setting page if >1 keyboards available on device
                // Otherwise just change the setting value for the only keyboard
                ShowITIMultiSettingPageL(ETrue);
#else
                // If EGSLangIdT9L is selected, change T9
                // setting using setting page.
                Container()->Model()->ChangePredictiveOnOffSettingL(
                    ETrue,
                    CGSLangModel::EGSLangOther );
#endif
                }
            else if( Container()->CurrentFeatureId() == EGSLangIdEPT )
                {
                // If EGSLangIdEPT is selected, change T9
                // setting using setting page.
                Container()->Model()->ChangePredictiveOnOffSettingL(
                    ETrue,
                    CGSLangModel::EGSLangOther );
                }
            else if( Container()->CurrentFeatureId() == EGSLangIdJPT )
                {
                Container()->Model()->ChangePredictiveOnOffSettingL(
                    ETrue,
                    CGSLangModel::EGSLangJapanese );
                }
            else if( Container()->CurrentFeatureId() == EGSLangIdJQC )
                {
                ChangeJapQwertySettingL( EGSJapQwertyFlagComma,
                                         EGSLangIdJQC, ETrue );
                }
            else if( Container()->CurrentFeatureId() == EGSLangIdJQP )
                {
                ChangeJapQwertySettingL( EGSJapQwertyFlagPeriod,
                                         EGSLangIdJQP, ETrue );
                }
            else if( Container()->CurrentFeatureId() == EGSLangIdJQWSP )
                {
                ChangeJapQwertySettingL( EGSJapQwertyFlagSpaceFullWidth,
                                         EGSLangIdJQWSP, ETrue );
                }
            else if ( Container()->CurrentFeatureId() == EGSLangIdPredictiveMode )
            	{
#ifndef RD_INTELLIGENT_TEXT_INPUT
               if( Container()->Model()->CheckAutoWordCompletionSupportL() )
                   {
            	   ShowPredictiveModeSettingPageL( ETrue );
            	   }
#endif 
           	 	}
#ifdef RD_INTELLIGENT_TEXT_INPUT
            else if ( Container()->CurrentFeatureId() == EGSLangIdPredictiveOptions )
                {
                // Show full ITI predictive settings list
                ShowPredictiveSettingListL();
                }
#endif // RD_INTELLIGENT_TEXT_INPUT
           	else
                {
                HandleListBoxSelectionL();
                }
            break;
        case EAknSoftkeyBack:
            //In 5.0 "Language" settings were moved from Psln to Device view
            #ifdef RD_CONTROL_PANEL //For S60 5.0
		        AppUi()->ActivateLocalViewL( KGSGenPluginUid );
            #else //RD_CONTROL_PANEL 
            AppUi()->ActivateLocalViewL( KGSPrslnPluginUid );
			#endif //RD_CONTROL_PANEL
            break;
        case EAknCmdHelp:
            {
            if( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
                {
                HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), AppUi()->AppHelpContextL() );
                }
            break;
            }
        default:
            AppUi()->HandleCommandL( aCommand );
            break;
        }
    }

// ========================= From CGSPluginInterface ==================


// -----------------------------------------------------------------------------
// CGSLangPlugin::GetCaptionL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::GetCaptionL( TDes& aCaption ) const
    {
    HBufC* result = StringLoader::LoadL( R_GS_LANG_PLUGIN_CAPTION );
    aCaption.Copy( *result );
    delete result;
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::PluginProviderCategory
//
//
// -----------------------------------------------------------------------------
//
TInt CGSLangPlugin::PluginProviderCategory() const
    {
    //This plugin is created by 3rd party.
    return KGSPluginProviderInternal;
    }

// ========================= From CGSBaseView ==================


// -----------------------------------------------------------------------------
// CGSLangPlugin::NewContainerL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::NewContainerL()
    {
    iContainer = new(ELeave) CGSLangContainer;
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::HandleListBoxSelectionL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::HandleListBoxSelectionL()
    {
    const TInt currentFeatureId = Container()->CurrentFeatureId();

    switch ( currentFeatureId )
        {
        case EGSLangIdDTL:
            ShowLanguageSelectionSettingPageL();
            break;
        case EGSLangIdITL:

#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT
            TInt keyboardLayout = 0;
            RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
            TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;            
            // Currently dual language feature would work correctly for ITUT and Half Qwerty products 
            // Hence the dual language settings should also be displayed only for ITUT and Halfqwerty products
            if(layout == EPtiKeyboard12Key || layout == EPtiKeyboardHalfQwerty )
                {
	            // Show Dual language selection setting list
	            /* In Chinese/Japanese language variant,if there are more than one languages supporting dual language feature,
	            display writing language setting view in which Primary and Secondary Languages can be selected from the list.
            
	            In Chinese language variant,if there is only one language supporting dual language feature, 
	            no need to display the Writing language view which contains Primary and Secondary Language. 
	            Instead we can display the Writing language list directly to the user;from which the user can select the writing language*/
		            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) ||  FeatureManager::FeatureSupported( KFeatureIdChinese ) )
		                {		                
		                CPtiEngine* ptiEngine = CPtiEngine::NewL();
		                CleanupStack::PushL( ptiEngine );
		                CArrayFixFlat<TInt>* languages = new( ELeave )CArrayFixFlat<TInt>( 10 );
		                CleanupStack::PushL( languages );
		                ptiEngine->GetAvailableLanguagesL( languages );
	                
		                TInt count = languages->Count();  
		                TInt noLangSupportWesternPred = count; 
	                
		                for (TInt index = 0; index < count; index++)
		                    {      
		                    const TInt langCode = languages->At(index);
		                    if (langCode == ELangTaiwanChinese ||
		                       langCode == ELangHongKongChinese ||
		                       langCode == ELangPrcChinese ||
		                       langCode == ELangJapanese ||
		                       langCode == ELangKorean )
		                        {
		                        noLangSupportWesternPred--;
		                        }            
		                    }                               
	             
		                CleanupStack::PopAndDestroy(languages);
		                CleanupStack::PopAndDestroy(ptiEngine);
	            
		                if( noLangSupportWesternPred > 1 )
		                    {
		                    ShowDualInputLanguageSettingListL();
		                    }
		                else
	                    ShowInputSelectionSettingPageL();
	                  }
	            	else            
	                	ShowDualInputLanguageSettingListL();
								}
	        	else 
            		ShowInputSelectionSettingPageL();
#else //FF_DUAL_LANGUAGE_SUPPORT
            ShowInputSelectionSettingPageL();
#endif //FF_DUAL_LANGUAGE_SUPPORT
#else //RD_INTELLIGENT_TEXT_INPUT
			ShowInputSelectionSettingPageL();
#endif //RD_INTELLIGENT_TEXT_INPUT
            break;
        case EGSLangAHNumberMode:
            if( iElaf )
                {
                ShowAHNumberModeSettingPageL();
                }
            break;
        case EGSLangHindiNumberMode:
            ShowHindiNumberModeSettingPageL();
            break;
        case EGSLangIdT9L:
            if ( !FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
#ifdef RD_INTELLIGENT_TEXT_INPUT
                // Launch Predictive mode setting page if >1 keyboards available on device
                // Otherwise just change the setting value for the only keyboard
                TInt keyboardLayout = 0;
                RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
                TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
                TBool isPredictionSupport = EFalse;
                switch(layout)
                {
                case EPtiKeyboardHalfQwerty:
                	isPredictionSupport = Container()->Model()->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEngineHalfQwertyPredictive);
                  break;
                case EPtiKeyboardQwerty4x12:
                case EPtiKeyboardQwerty4x10:  
                case EPtiKeyboardQwerty3x11:
                case EPtiKeyboardCustomQwerty:
                	isPredictionSupport = Container()->Model()->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEngineQwertyPredictive);
                  break;
                case EPtiKeyboardNone:
                case EPtiKeyboard12Key:
                	isPredictionSupport = Container()->Model()->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEnginePredictive);
                  break;
                default:
                	isPredictionSupport = Container()->Model()->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEnginePredictive);
                  break;    
                  }
                if (isPredictionSupport)
                	ShowITIMultiSettingPageL(EFalse);
#else
                //change setting without opening the setting page
                Container()->Model()->ChangePredictiveOnOffSettingL(
                    EFalse,
                    CGSLangModel::EGSLangOther );
#endif
                }
            break;
        case EGSLangIdEPT:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
                Container()->Model()->ChangePredictiveOnOffSettingL(
                    EFalse,
                    CGSLangModel::EGSLangOther );
                }
            break;
        case EGSLangIdResetAtokDict:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
                ResetATOKDictionaryL();
                }
            break;
        case EGSLangIdJPT:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
                Container()->Model()->ChangePredictiveOnOffSettingL(
                    EFalse,
                    CGSLangModel::EGSLangJapanese );
                }
            break;
        case EGSLangIdKT:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
                ShowKeypressTimeoutSettingPageL();
                }
            break;
        case EGSLangIdJQC:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) &&
                 FeatureManager::FeatureSupported( KFeatureIdQwertyInput ) )
                {
                ChangeJapQwertySettingL( EGSJapQwertyFlagComma,
                                         currentFeatureId, EFalse );
                }
            break;
        case EGSLangIdJQP:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) &&
                 FeatureManager::FeatureSupported( KFeatureIdQwertyInput ) )
                {
                ChangeJapQwertySettingL( EGSJapQwertyFlagPeriod,
                                         currentFeatureId, EFalse );
                }
            break;
        case EGSLangIdJQWSP:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) &&
                 FeatureManager::FeatureSupported( KFeatureIdQwertyInput ) )
                {
                ChangeJapQwertySettingL( EGSJapQwertyFlagSpaceFullWidth,
                                         currentFeatureId, EFalse );
                }
            break;
        case EGSLangIdDIM:
            if ( FeatureManager::FeatureSupported( KFeatureIdChinese ) )
                {
                ShowDefaultInputMethodSettingPageL();
                }
            break;
        case EGSLangIdCangJie:
            if ( FeatureManager::FeatureSupported( KFeatureIdChinese ) &&
                 Container()->Model()->DefaultInputMethodL() == ECangJie )
                {
                ShowChiCangJieInputModeSettingPageL();
                }
            break;
        case EGSLangIdPredictiveMode:
#ifndef RD_INTELLIGENT_TEXT_INPUT
           if( Container()->Model()->CheckAutoWordCompletionSupportL() )
               {
        	   ShowPredictiveModeSettingPageL( EFalse );
        	   }
#endif
            break;
#ifdef RD_INTELLIGENT_TEXT_INPUT
        case EGSLangIdPredictiveOptions:
            // Show full ITI predictive settings list
            ShowPredictiveSettingListL();
            break;
#endif
        default:
            break;
        }
    }

// ========================= New ======================================


// -----------------------------------------------------------------------------
// CGSLangPlugin::CGSLangPlugin
//
//
// -----------------------------------------------------------------------------
//
CGSLangContainer* CGSLangPlugin::Container()
    {
    CGSLangContainer* container = static_cast<CGSLangContainer*>( iContainer );
    return container;
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::ShowLanguageSelectionSettingPageL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::ShowLanguageSelectionSettingPageL()
    {
    // If the setting page has a list create array which contains all
    // the items of the list in the setting page:

    // Set up UI language arrays
    CArrayFixFlat<TInt>* systemEpocLanguageCodes = 0;
    // This creates the list itself
    User::LeaveIfError( SysLangUtil::GetInstalledLanguages(
                                     systemEpocLanguageCodes ) );
    CleanupStack::PushL( systemEpocLanguageCodes );

    CArrayFixFlat<TInt>* dtlanguages = new(ELeave)CArrayFixFlat<TInt>(4);
    CleanupStack::PushL( dtlanguages );
    // Append "automatic" language into dtitems array
    dtlanguages->AppendL( 0 );
    // and get text for automatic from resource...
    CDesCArrayFlat* dtitems = iCoeEnv->ReadDesC16ArrayResourceL(
                            R_DISPLAY_TEXT_LANGUAGE_SETTING_PAGE_LBX );
    CleanupStack::PushL( dtitems );

    Container()->Model()->GetLanguagesFromPtiL(
                           systemEpocLanguageCodes,
                           dtlanguages, dtitems );

    TInt count = dtlanguages->Count();
    TInt currentItem = 0; // default value is Automatic

    //fetch the current value from model:
    TInt uiLanguage = Container()->Model()->LanguageL( CGSLangModel::EGSUiLanguage );

    // find the match epoc language code and set to UI array
    TInt i=0;
    for ( ; i < count; i++ )
        {
        if ( ( *dtlanguages )[i] == uiLanguage )
            {
            currentItem = i;
            break;
            }
        }
    // create setting page:
    TInt oldItem = currentItem;
    CAknSettingPage* dlg =
        new( ELeave ) CAknRadioButtonSettingPage(
                       R_DISPLAY_TEXT_LANGUAGE_SETTING_PAGE,
                       currentItem, dtitems );

    // Execute the setting page and check if the setting value was changed:
    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        if ( oldItem != currentItem )
            {
            // Check if there is enough space in disk
            if ( Container()->Model()->CheckDiskSpaceL() )
                {
                // Show information note
                HBufC* prompt = iCoeEnv->AllocReadResourceLC( R_GS_NOT_ENOUGH_MEMORY );

                CAknInformationNote* note = 
                     new( ELeave ) CAknInformationNote( ETrue );
                note->ExecuteLD( *prompt );
                
                CleanupStack::PopAndDestroy( prompt );
                }
            else
                {
                ConfirmRebootDTLL( ( *dtlanguages )[ currentItem ],
                               ( *dtitems )[ currentItem ] );
                }
            }
        }
    // Delete items:
    //dtitems, dtlanguages & systemEpocLanguageCodes
    CleanupStack::PopAndDestroy(3);

    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::ShowInputSelectionSettingPageL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::ShowInputSelectionSettingPageL()
    {
    // If the setting page has a list create array which contains all
    // the items of the list in the setting page:

    //some dummy size of 10
    CArrayFixFlat<TInt>* languages =
               new( ELeave )CArrayFixFlat<TInt>(10);
    CleanupStack::PushL( languages );

    //some dummy size of 10
    CDesCArrayFlat* items = new( ELeave )CDesCArrayFlat(10);
    CleanupStack::PushL( items );

    Container()->Model()->GetLanguagesFromPtiL( NULL,
                                                languages, items );
    TInt i;
    TInt currentItem = 0;

    //fetch the current value from model:
    // current item is the decimal value of T9 language code from shared data
    TInt currentInputLanguage =
         Container()->Model()->LanguageL( CGSLangModel::EGSInputLanguage );

    // total sum of available T9 dictionaries
    TInt count = languages->Count();

    // compare sharedata languageCode with PtiLanguageCode array
    // Ptilanguage <-> listindex converter

    for ( i=0;i<count;i++ )
        {
        if ((*languages)[i] == currentInputLanguage)
        {
        currentItem = i;
        break;
        }
    }
    // create setting page:
    CAknSettingPage* dlg =
        new( ELeave ) CAknRadioButtonSettingPage(
                         R_INPUT_TEXT_LANGUAGE_SETTING_PAGE,
                         currentItem, items );
    TInt previous = Container()->Model()->LanguageL( CGSLangModel::EGSInputLanguage );
    // Execute the setting page and check if the setting value was changed:
    if ( dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged ) )
        {
        //if the value was changed set the value to model
        Container()->Model()->SetLanguageL( CGSLangModel::EGSInputLanguage,
                                      ( *languages )[ currentItem ] );
        //inform all other apps about language change.
        TWsEvent event;
        event.SetType( KEikInputLanguageChange  );
        iEikonEnv->WsSession().SendEventToAllWindowGroups( event );

        //if updated
        if ( previous !=
                Container()->Model()->LanguageL( CGSLangModel::EGSInputLanguage ) )
            {
            //Update setting list:
            Container()->UpdateListBoxL( EGSLangIdITL );
            Container()->UpdateListBoxL( EGSLangIdT9L );
            Container()->UpdateListBoxL( EGSLangIdPredictiveMode );
            }
        TInt current = Container()->Model()->LanguageL( CGSLangModel::EGSInputLanguage );
        if ( ( (current == ELangPrcChinese || current == ELangHongKongChinese
            || current == ELangTaiwanChinese) && previous != current)
            || ( (current != ELangPrcChinese || current != ELangHongKongChinese
            || current != ELangTaiwanChinese) && previous != current) )
            {
            if (current == ELangPrcChinese )
                {
                Container()->Model()->SetDefaultInputMethodL( EPinyin );
                }
            else if ( current == ELangHongKongChinese )
                {
                Container()->Model()->SetDefaultInputMethodL( EStroke );
                }
            else if ( current == ELangTaiwanChinese )
                {
                Container()->Model()->SetDefaultInputMethodL( EZhuyin );
                }
            Container()->UpdateListBoxL( EGSLangIdDIM );
            }
        }
    // Delete items:
    CleanupStack::PopAndDestroy(2);    // items and languages
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::ShowDefaultInputMethodSettingPageL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::ShowDefaultInputMethodSettingPageL()
    {
    // If the setting page has a list create array which contains all
    // the items of the list in the setting page:

    // Find correct Language resource among PRC, HK and TW.
    TInt resId = 0;
    TInt lang = Container()->Model()->LanguageL( CGSLangModel::EGSInputLanguage );
    
    if ( FeatureManager::FeatureSupported( KFeatureIdQwertyInput ) 
        || (FeatureManager::FeatureSupported(KFeatureIdVirtualKeyboardInput) &&
           FeatureManager::FeatureSupported(KFeatureIdChinese)))
        {
        resId = R_DIM_QWERTY_SETTING_PAGE_LBX;
        }
    else
        {
        resId = R_DIM_SETTING_PAGE_LBX;
        }
      
    TInt keyboardLayout = 0;
    RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
    TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
    
    // Get the static items here
    CGSRadioButtonSettingPageItemTextArray* items = 
        CGSRadioButtonSettingPageItemTextArray::NewL( resId, *iCoeEnv, NULL );
        
    
    CleanupStack::PushL(items);
	
	TBool isCangjieSupported = EFalse;

    
    switch( lang )
        {
        case ELangPrcChinese:
            if ( FeatureManager::FeatureSupported( KFeatureIdQwertyInput )
               || FeatureManager::FeatureSupported(KFeatureIdVirtualKeyboardInput))
                {
                items->SetItemVisibilityL( EGSLangSPIdQwPinyin, 
                                   CGSListBoxItemTextArray::EVisible );
                }
            else
                {
                items->SetItemVisibilityL( EGSLangSPIdPinyin, 
                                   CGSListBoxItemTextArray::EVisible );
                }
            break;
        case ELangHongKongChinese:
            if ( FeatureManager::FeatureSupported( KFeatureIdQwertyInput )
               || FeatureManager::FeatureSupported(KFeatureIdVirtualKeyboardInput))
                {
				if(!(layout == EPtiKeyboard12Key || layout == EPtiKeyboardHalfQwerty))
                    {
					isCangjieSupported = ETrue;
					}
                if ( Container()->Model()->CheckCangjieSupportL( ELangHongKongChinese ) && isCangjieSupported )
                    {
                    
                    //Cangjie input mode is only valid for QWERTY input and Virtual keyboard Input 
                    //Putting condition for 12 key and half QWERTY keyboard layout.
                    
                        items->SetItemVisibilityL( EGSLangSPIdQwCangJie, 
                                       CGSListBoxItemTextArray::EVisible );
                        
                    
                    }
                }
            break;
        case ELangTaiwanChinese:
            if ( FeatureManager::FeatureSupported( KFeatureIdQwertyInput ) 
               || FeatureManager::FeatureSupported(KFeatureIdVirtualKeyboardInput))
                {
                items->SetItemVisibilityL( EGSLangSPIdQwZhuyin, 
                                   CGSListBoxItemTextArray::EVisible );
                }
            else
                {
                items->SetItemVisibilityL( EGSLangSPIdZhuyin, 
                                   CGSListBoxItemTextArray::EVisible );
                }
            break;
        default:
            break;
        }
    
    //fetch the current value from model:
    // current item is the Hexadecimal value of Input method code from CenRep
    TInt oldInputMethodCode =
             Container()->Model()->DefaultInputMethodL();
    TInt currentItem = 0;

    // Construct a map for PRC,HK and TW for Input Method codes
    struct TMapArray { TInt iInputMethodCode; TInt index0, index1, index2; };
     //Note: The values 0, 1, 2, 3 have been used to align with the setting page indices
	 
	TMapArray qwertyMethodMap_Cangjie[] = {
        //            PRC, HK, TW
        { EPinyin     , 0, -1, -1 },
        { EZhuyin     ,-1, -1,  0 },
        { EStroke     , 1,  0,  1 },
        { ECangJie    ,-1,  1, -1 },
        { ELatin      , 2,  2,  2 }
     };
     
     TMapArray qwertyMethodMap_NoCangjie[] = {
        //            PRC, HK, TW
        { EPinyin     , 0, -1, -1 },
        { EZhuyin     ,-1, -1,  0 },
        { EStroke     , 1,  0,  1 },
        { ECangJie    ,-1, -1, -1 },
        { ELatin      , 2,  1,  2 }
        };
        
    TMapArray itutMethodMap[] = {
        //            PRC, HK, TW
        { EPinyin     , 0, -1, -1 },
        { EZhuyin     ,-1, -1,  0 },
        { EStroke     , 1,  0,  1 },
        { ELatinText  , 2,  1,  2 },
        { ELatinUpper , 3,  2,  3 },
        { ELatinLower , 4,  3,  4 }
     }; 
    
    TMapArray halfQwertyMethodMap[] = {
        //            PRC, HK, TW
        { EPinyin     , 0, -1, -1 },
        { EZhuyin     ,-1, -1,  0 },
        { EStroke     , 1,  0,  1 },
        { ECangJie    ,-1, -1, -1 },
        { ELatin      , 2,  1,  2 }
     };
    
    TInt itemsNum = KGSMapSize;
    TMapArray* methodMap = NULL;
    if ( layout == EPtiKeyboardHalfQwerty )
        {
        methodMap = halfQwertyMethodMap;
        itemsNum = sizeof( halfQwertyMethodMap ) / sizeof( TMapArray );
        }
    else if ( FeatureManager::FeatureSupported( KFeatureIdQwertyInput ) 
            || (FeatureManager::FeatureSupported(KFeatureIdVirtualKeyboardInput) &&
                    FeatureManager::FeatureSupported(KFeatureIdChinese)))
        {
		    if (isCangjieSupported)
		        {
			      methodMap = qwertyMethodMap_Cangjie;
			      itemsNum = sizeof( qwertyMethodMap_Cangjie ) / sizeof( TMapArray );
			      }
		    else
		        {
			      methodMap = qwertyMethodMap_NoCangjie;
			      itemsNum = sizeof( qwertyMethodMap_NoCangjie ) / sizeof( TMapArray );
			      }
        }
    else
        {
        methodMap = itutMethodMap;
        itemsNum = sizeof( itutMethodMap ) / sizeof( TMapArray );
        }
    for(TInt i = 0; i < itemsNum && i < KGSMapSize; i++)
        {
        if (oldInputMethodCode == methodMap[i].iInputMethodCode)
            {
            TInt val = 0;
            if ( lang == ELangPrcChinese )      { val = methodMap[i].index0; }
            if ( lang == ELangHongKongChinese ) { val = methodMap[i].index1; }
            if ( lang == ELangTaiwanChinese )   { val = methodMap[i].index2; }
            currentItem = val;
            break;
            }
        }
    if ( currentItem == -1 ) 
        { 
        currentItem = 0;
        }

    // create setting page:
    CAknSettingPage* dlg =new( ELeave ) CAknRadioButtonSettingPage(
        R_CH_DEFAULT_INPUT_METHOD_SETTING_PAGE,
        currentItem,
        items );

    // Execute the setting page and check if the setting value was changed:
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        TInt newInputMethodCode=EPinyin;

        for (TInt j = 0; j < KGSMapSize; j++)
            {
            TInt val = 0;
            if ( lang == ELangPrcChinese )      { val = methodMap[j].index0; }
            if ( lang == ELangHongKongChinese ) { val = methodMap[j].index1; }
            if ( lang == ELangTaiwanChinese )   { val = methodMap[j].index2; }

            if ( currentItem == val )
                {
                newInputMethodCode = methodMap[j].iInputMethodCode;
                break;
                }
            }
        //if the value was changed set the value to model
        Container()->Model()->SetDefaultInputMethodL( newInputMethodCode );

        //Update setting list:
        Container()->UpdateListBoxL( EGSLangIdDIM );
        // If CangJie input is selected, then make the CangJie input list
        // item visible in the UI.
        if ( newInputMethodCode == ECangJie )
            {
            Container()->UpdateListBoxL( EGSLangIdCangJie );
            }
        Container()->MakeListItemVisibleL( EGSLangIdCangJie, EGSLangIdDIM );      
        }
    // Delete items:
    CleanupStack::PopAndDestroy( items );
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::ShowAHNumberModeSettingPageL
//
// Show A&H number mode setting page
// -----------------------------------------------------------------------------
//
TBool CGSLangPlugin::ShowAHNumberModeSettingPageL()
    {
    // This updates also the iAHNumberModeItems which should be checked before
    // using them.
    Container()->UpdateListBoxL( EGSLangAHNumberMode );

    TInt currentItem = 
        Container()->Model()->DefaultNumberMode( EGSNbrModeTypeArabic );

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
        R_AH_NUMBER_MODE_SETTINGS_PAGE,
        currentItem,
        Container()->NumberModeItems( EGSNbrModeTypeArabic ) );

    TBool ret = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );

    // Code to obtain the visual appearance of the date.
    TAknVisualDateFormat visualDateFormat;
        {
            TLocale locale;
            visualDateFormat =
            AknDateFormatUtils::MapSymbianDateFormatToVisualDateFormat(
                                locale.DateFormat(), NULL );
            }
    
    const TInt uiLang = 
          Container()->Model()->LanguageL( CGSLangModel::EGSUiLanguage );
    if ( ( uiLang == ELangUrdu || User::Language() == ELangUrdu ) || 
         ( uiLang == ELangFarsi || User::Language() == ELangFarsi) )
        {
        Container()->Model()->SetDefaultNumberMode( currentItem, 
                                                EGSNbrModeTypeEasternArabic );
        }
    else
        {
        Container()->Model()->SetDefaultNumberMode( currentItem, 
                                                EGSNbrModeTypeArabic );
        }

    // Code to set ensure the visual appearance stays stable.
    TLocale locale;
    TDateFormat newFormat =
        AknDateFormatUtils::MapVisualDateFormatToSymbianDateFormat(
                            visualDateFormat, NULL );
    if ( newFormat != locale.DateFormat())
        {
        locale.SetDateFormat( newFormat );
        locale.Set();
        }

    TWsEvent event;
    event.SetType( KEikInputLanguageChange );
    iEikonEnv->WsSession().SendEventToAllWindowGroups( event );

    // value should really be being updated, so update listbox
    Container()->UpdateListBoxL( EGSLangAHNumberMode );

    return ret;
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::ShowHindiNumberModeSettingPageL
//
// Show Hindi number mode setting page
// -----------------------------------------------------------------------------
//
TBool CGSLangPlugin::ShowHindiNumberModeSettingPageL()
    {
    // This updates also the iHindiNumberModeItems which should be checked before
    // using them.
    Container()->UpdateListBoxL( EGSLangHindiNumberMode );

    TInt currentItem = 
        Container()->Model()->DefaultNumberMode( EGSNbrModeTypeIndic );

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
        R_HI_NUMBER_MODE_SETTINGS_PAGE,
        currentItem,
        Container()->NumberModeItems( EGSNbrModeTypeIndic ) );

    TBool ret = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );

    // Code to obtain the visual appearance of the date.
    TAknVisualDateFormat visualDateFormat;
        {
        TLocale locale;
        visualDateFormat =
        AknDateFormatUtils::MapSymbianDateFormatToVisualDateFormat(
                            locale.DateFormat(), NULL );
        }
        
    Container()->Model()->SetDefaultNumberMode( currentItem, 
                                                EGSNbrModeTypeIndic );

    // Code to set ensure the visual appearance stays stable.
    TLocale locale;
    TDateFormat newFormat =
        AknDateFormatUtils::MapVisualDateFormatToSymbianDateFormat(
                            visualDateFormat, NULL );
    if ( newFormat != locale.DateFormat())
        {
        locale.SetDateFormat( newFormat );
        locale.Set();
        }

    TWsEvent event;
    event.SetType( KEikInputLanguageChange );
    iEikonEnv->WsSession().SendEventToAllWindowGroups( event );

    // value should really be being updated, so update listbox
    Container()->UpdateListBoxL( EGSLangHindiNumberMode );

    return ret;
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::ConfirmRebootDTLL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::ConfirmRebootDTLL(   TInt aCurrentLanguage,
                                        const TDesC& aLanguageName )
    {
    //Get information of if there are open connections
    TInt callState = CheckCallState();

    // get information of whether there are open connections or not
    if ( callState != EPSCTsyCallStateUninitialized &&
         callState != EPSCTsyCallStateNone )
        {
        HBufC* prompt = iCoeEnv->AllocReadResourceLC( R_ACTIVE_CALLS );

        CAknInformationNote* note =
            new( ELeave ) CAknInformationNote( ETrue );

        note->ExecuteLD( *prompt );

        CleanupStack::PopAndDestroy( prompt );

        return;
        }

    // The activity starts here.
    // First, show the confirmation query.
    HBufC* buf =
        StringLoader::LoadL(R_CONFIRM_DTL_STRING, aLanguageName, iEikonEnv );
    CleanupStack::PushL(buf);

    CAknQueryDialog* query =
        new( ELeave ) CAknQueryDialog( CAknQueryDialog::ENoTone );
    query->PrepareLC( R_CONFIRM_DTL ); // pushes query to cleanupstack
    query->SetPromptL( *buf );

    TInt ret = query->RunLD(); // deletes query

    CleanupStack::PopAndDestroy( buf );

    if ( ret )
        {
        // Code to obtain the visual appearance of the date.
        TAknVisualDateFormat visualDateFormat;
            {
            //This locale is scoped so that it does we get a totally new
            //instance later on
            TLocale locale;
            visualDateFormat =
                AknDateFormatUtils::MapSymbianDateFormatToVisualDateFormat(
                                    locale.DateFormat(), NULL );
            }
        // if the value was changed set the value to model
        Container()->Model()->SetLanguageL( CGSLangModel::EGSUiLanguage,
                                            aCurrentLanguage );

        // Code to set ensure the visual appearance stays stable.
        // Note that we force the language here because User::Language will
        // not return the new language
        TLocale locale;
        TDateFormat newFormat =
            AknDateFormatUtils::MapVisualDateFormatToSymbianDateFormat(
                             visualDateFormat, (TLanguage*)&aCurrentLanguage );
        if ( newFormat != locale.DateFormat() )
            {
            locale.SetDateFormat( newFormat );
            locale.Set();
            }
        // Update setting list:
        Container()->UpdateListBoxL( EGSLangIdDTL );
        Container()->UpdateListBoxL( EGSLangIdITL );

        //Updating default input method based on input text language
        //Chinese specific feature
        if ( aCurrentLanguage == ELangPrcChinese )
            {
            Container()->Model()->SetDefaultInputMethodL( EPinyin );
            }
        else if ( aCurrentLanguage == ELangHongKongChinese )
            {
            Container()->Model()->SetDefaultInputMethodL( EStroke );
            }
        else if ( aCurrentLanguage == ELangTaiwanChinese )
            {
            Container()->Model()->SetDefaultInputMethodL( EZhuyin );
            }

        TInt satLanguageSelectionTime( 0 );
        RProperty::Get( KPSUidSatServer,
                        KSatLanguageSelectionTimeBeforeReboot,
                        satLanguageSelectionTime );
                    
        if ( 0 != satLanguageSelectionTime )
            {
            User::After( satLanguageSelectionTime * 1000 );
            }

        // Reboot with Language Switch reason
#ifndef RD_STARTUP_CHANGE
        SysStartup::ShutdownAndRestart( AppUi()->Application()->AppDllUid(),
                                        ESWLangSwitch );
#else
        RStarterSession startSession;
        if ( startSession.Connect() == KErrNone )
            {
            startSession.Reset( RStarterSession::ELanguageSwitchReset );
            startSession.Close();
            }
#endif // RD_STARTUP_CHANGE
        }
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::CheckCallState
//
//
// -----------------------------------------------------------------------------
//
TInt CGSLangPlugin::CheckCallState()
    {
    TInt callState = KErrNone;

    //here are open connections or not
    RProperty::Get( KPSUidCtsyCallInformation,
                    KCTsyCallState,
                    callState );
    return callState;
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::ResetATOKDictionaryL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::ResetATOKDictionaryL()
    {
    HBufC* buf = StringLoader::LoadL( R_CONFIRM_RESET_ATOK_STRING, iEikonEnv );
    CleanupStack::PushL( buf );

    CAknQueryDialog* query =
        new( ELeave ) CAknQueryDialog( CAknQueryDialog::ENoTone );
    query->PrepareLC( R_CONFIRM_RESET_ATOK );
    query->SetPromptL( *buf );

    TInt ret = query->RunLD(); // deletes query
    CleanupStack::PopAndDestroy( buf );

    if ( ret )
        {
        CPtiEngine* ptiEngine = CPtiEngine::NewL();
        CleanupStack::PushL( ptiEngine );

        ptiEngine->ActivateLanguageL( ELangJapanese, EPtiEngineHiraganaKanji );
        ptiEngine->HandleCommandL( EPtiCommandResetLearningInfo );

        CleanupStack::PopAndDestroy( ptiEngine );
        }
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::ShowKeypressTimeoutSettingPageL()
// 
// Create a Keypress timeout setting page
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::ShowKeypressTimeoutSettingPageL()
    {
    // Get current data
    const TInt currentTime = Container()->Model()->KeypressTimeout();
    TInt currentIndex;
    if ( currentTime == 0 )
        {
        currentIndex = EGSKeypressTimeoutItemNone;
        }
    else if ( currentTime > KGSMultiTapTimerDefault )
        {
        currentIndex = EGSKeypressTimeoutItemLong;
        }
    else if ( currentTime < KGSMultiTapTimerDefault )
        {
        currentIndex = EGSKeypressTimeoutItemShort;
        }
    else
        {
        currentIndex = EGSKeypressTimeoutItemMedium;
        }

    // and get text for automatic from resource...
    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL( 
                              R_KEYPRESS_TIMEOUT_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );

    // create setting page:
    TInt oldIndex = currentIndex;
    CAknSettingPage* dlg = new( ELeave ) CAknRadioButtonSettingPage
                          ( R_KEYPRESS_TIMEOUT_SETTING_PAGE,
                            currentIndex, items );

    // Execute the setting page and check if the setting value was changed:
    if ( dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged ) )
        {
        if ( oldIndex != currentIndex )
            {
            TInt newTime = KGSMultiTapTimerDefault;
            const TInt rate = 40;       // rate is 40%.
            switch ( currentIndex )
                {
                case EGSKeypressTimeoutItemNone:
                    newTime = 0;
                    break;
                case EGSKeypressTimeoutItemShort:
                    newTime = 
                    KGSMultiTapTimerDefault - KGSMultiTapTimerDefault * rate / 100;
                    break;
                case EGSKeypressTimeoutItemLong:
                    newTime = 
                    KGSMultiTapTimerDefault + KGSMultiTapTimerDefault * rate / 100;
                    break;
                case EGSKeypressTimeoutItemMedium:
                default:
                    newTime = KGSMultiTapTimerDefault;
                    break;
                }
            Container()->Model()->SetKeypressTimeout( newTime );
            // value should really be being updated, so update listbox        
            Container()->UpdateListBoxL( EGSLangIdKT );
            }
        }   
    // Delete items:
    CleanupStack::PopAndDestroy( items );
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::ChangeJapQwertySettingL
// Change Japanese Qwerty settings
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::ChangeJapQwertySettingL( const TGSJapQwertyFlags aFlag,
                                     const TInt aItemId,
                                     const TBool aShowSettingPage )
    {
    TBool currentValue = Container()->Model()->JapanQwertyFlags( aFlag );
    TBool updateValue = ETrue;
    
    TInt settingPageResourceId = 0;
    TInt itemArraryResourceId = 0;    
    switch ( aItemId )
        {
        case EGSLangIdJQC:
            settingPageResourceId = R_JAP_QWERTY_COMMA_SETTING_PAGE;
            itemArraryResourceId = R_JAP_QWERTY_COMMA_SETTING_PAGE_LBX;
            break;
        case EGSLangIdJQP:
            settingPageResourceId = R_JAP_QWERTY_PERIOD_SETTING_PAGE;
            itemArraryResourceId = R_JAP_QWERTY_PERIOD_SETTING_PAGE_LBX;
            break;
        case EGSLangIdJQWSP:
            settingPageResourceId = R_JAP_QWERTY_WIDTH_SPACE_SETTING_PAGE;
            itemArraryResourceId = R_JAP_QWERTY_WIDTH_FULL_HALF_SETTING_PAGE_LBX;
            break;
        default:
            break;
        }

    // Following code potentially changes the value of currrent value 
    if ( aShowSettingPage )
        {
        updateValue = ShowJapQwertySettingPageL( settingPageResourceId,
                                                 itemArraryResourceId,
                                                 currentValue );
        }
    else
        {
        updateValue = !( currentValue );
        }

    if ( currentValue != updateValue )
        {
        Container()->Model()->SetJapanQwertyFlags( aFlag, updateValue );
        // value should really be being updated, so update listbox
        Container()->UpdateListBoxL( aItemId );
        }
    }

// -----------------------------------------------------------------------------
// CGSLangPlugin::ShowJapQwertySettingPageL
// Display Japanese Qwerty ON/OFF setting page.
// -----------------------------------------------------------------------------
//
TBool CGSLangPlugin::ShowJapQwertySettingPageL( 
                                        const TInt aSettingPageResourceId,
                                        const TInt aItemArraryResourceId,
                                        const TBool aCurrentValue )
    {
    // Default is to act as if the user cancelled the setting page
    TBool ret = EFalse;

    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
                                        aItemArraryResourceId );
    CleanupStack::PushL( items );
    // If current value is ETrue, display index is 0.
    TInt dispIndex = ( aCurrentValue )? 0 : 1;

    CAknRadioButtonSettingPage* dlg = new ( ELeave ) CAknRadioButtonSettingPage
                                     ( aSettingPageResourceId, 
                                       dispIndex, items );
    ret = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
    if ( ret )
        {
        ret = ( dispIndex )? EFalse : ETrue;
        }
    else
        {
        ret = aCurrentValue;
        }
    CleanupStack::PopAndDestroy( items );
    return ret;
    }


// -----------------------------------------------------------------------------
// CGSLangPlugin::ShowChiCangJieInputModeSettingPageL
//
// Display Chinese CangJie Input mode setting page.
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::ShowChiCangJieInputModeSettingPageL()
    {
    // Default is to act as if the user cancelled the setting page
    TBool ret = EFalse;

    TInt selection = Container()->Model()->ChiCangJieInputL();
    CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
                                        R_CHI_CANGJIE_SETTING_PAGE_LBX );
    CleanupStack::PushL( items );
    
    CAknRadioButtonSettingPage* dlg = new ( ELeave ) CAknRadioButtonSettingPage
               ( R_CHI_CANGJIE_INPUT_MODE_SETTING_PAGE, selection, items );
    ret = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
    CleanupStack::PopAndDestroy( items );
    
    if ( ret )
        {
        //User has changed the value. Update the CenRep key and the UI
        Container()->Model()->SetChiCangJieInputL( selection );
        Container()->UpdateListBoxL( EGSLangIdCangJie );
        }
    }

// -----------------------------------------------------------------------------
// CGSLangPlugin::ShowPredictiveModeSettingPageL
// Word Autocompletion Requirement
// Display Predictive mode setting page.
// -----------------------------------------------------------------------------
//
void CGSLangPlugin::ShowPredictiveModeSettingPageL( const TBool aShowSettingPage )
    {
    TBool updateValue = ETrue;
    TInt selection = Container()->Model()->PredictiveModeL();
    if ( aShowSettingPage )
    	{
    	// Default is to act as if the user cancelled the setting page
     	CDesCArrayFlat* items = iCoeEnv->ReadDesC16ArrayResourceL(
                                        R_PREDICTIVE_MODE_SETTING_PAGE_LBX );
    	CleanupStack::PushL( items );
    	CAknRadioButtonSettingPage* dlg = new ( ELeave ) CAknRadioButtonSettingPage
        	       ( R_PREDICTIVE_MODE_SETTING_PAGE, selection, items );
    	updateValue = dlg->ExecuteLD( CAknSettingPage::EUpdateWhenChanged );
    	CleanupStack::PopAndDestroy( items );
    	}
    else // just switch the value
    	{
    	// 0 --> Normal 
		// 1 --> Autocompletion
    	if ( selection == EGSPredictiveModeNormal )
    		{
    		selection = EGSPredictiveModeAutocompletion;
    		}
    	else if ( selection == EGSPredictiveModeAutocompletion )
    	    {
    	    selection = EGSPredictiveModeNormal;
    	    }
    	}

    if ( updateValue )
        {
        //User has changed the value. Update the CenRep key and the UI
        // Treat on and off separately for clarity
        if ( selection )
            {
            // See if it can do predictive text entry
            TInt langCode = Container()->Model()->LanguageL
            							( CGSLangModel::EGSInputLanguage );
            if( Container()->Model()->CheckDictionaryFromPtiL( langCode ) )
                {
                Container()->Model()->SetPredictiveModeL( selection );
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
            Container()->Model()->SetPredictiveModeL( selection );
            }
        Container()->UpdateListBoxL( EGSLangIdPredictiveMode );
        }
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef FF_DUAL_LANGUAGE_SUPPORT
// ----------------------------------------------------------------------------
// CGSLangPlugin::ShowDualInputLanguageSettingListL()
// 
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSLangPlugin::ShowDualInputLanguageSettingListL()
    {
    CAknAppUi* appUi = static_cast<CAknAppUi*>(iCoeEnv->AppUi());

    CAknTitlePane* titlePane = static_cast<CAknTitlePane*>( appUi->StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );

    const TDesC* appTitle = titlePane->Text();
    HBufC* oldTitle = appTitle->AllocL();
    CleanupStack::PushL(oldTitle);
    HBufC* title = StringLoader::LoadLC( R_INPUT_LANGUAGE_SETTINGS_TITLE, iCoeEnv);
    titlePane->SetTextL( *title );
    CleanupStack::PopAndDestroy(title);

    CGSDualLanguageDialog* dlg = CGSDualLanguageDialog::NewL(R_GS_MENUBAR_CHANGE_EXIT, Container()->Model());
    dlg->ExecuteLD(R_INPUTLANGUAGE_DIALOG);

    titlePane->SetTextL( *oldTitle );
    CleanupStack::PopAndDestroy(oldTitle);
    }

#endif //FF_DUAL_LANGUAGE_SUPPORT

void DestroySelectionItemList(TAny* aPtr)
    {
    CSelectionItemList* list = static_cast<CSelectionItemList*>(aPtr);
    list->ResetAndDestroy();
    delete list;
    }

// ----------------------------------------------------------------------------
// CGSLangPlugin::ShowITIMultiSettingPageL(TBool aShowOnOffSettingPage)
// 
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSLangPlugin::ShowITIMultiSettingPageL(TBool aShowOnOffSettingPage)
    {
    if( Container()->Model()->DeviceHasMultipleKeyboardsL() )
        {
        CSelectionItemList* selection = new (ELeave) CSelectionItemList(2);
        CleanupStack::PushL(TCleanupItem(DestroySelectionItemList, selection));
        TInt physicalKeyboards = Container()->Model()->PhysicalKeyboardsL();
        TInt predictionBitmask = Container()->Model()->PredictionBitMaskL();
        TBool settingActive = EFalse;
        HBufC* selectionText = 0;
        TInt count = 0;
        TInt itutIndex = 0;
        TInt halfQwertyIndex = 0;
        TInt qwertyIndex = 0;
        for(TUint i = 0x01; i <= 0xF0; i = i << 1)
            {
            if(physicalKeyboards & i)
                {
                switch(i)
                    {
                    case 0x01: // itut
                        settingActive = ( predictionBitmask & EKeyboardStyle12Key ? 1 : 0 ); 
                        selectionText = iCoeEnv->AllocReadResourceLC(R_AKNFEP_PRED_SETTING_ITUT);
                        itutIndex = count;
                        break;
                    case 0x10: // half
                        settingActive = ( predictionBitmask & EKeyboardStyleHalfQwerty ? 1 : 0 ); 
                        selectionText = iCoeEnv->AllocReadResourceLC(R_AKNFEP_PRED_SETTING_HALF_QWERTY);
                        halfQwertyIndex = count;
                        break;                        
                    case 0x02: // 4x12
                    case 0x04: // 4x10
                    case 0x08: // 3x11                    
                    case 0x20: // custom
                        settingActive = ( predictionBitmask & EKeyboardStyleQwerty ? 1: 0);
                        selectionText = iCoeEnv->AllocReadResourceLC(R_AKNFEP_PRED_SETTING_QWERTY);
                        qwertyIndex = count;
                        i = 0x20; // Skip rest of qwerty checks
                        break;
                    default:
                        break;
                    }
                count++;
                CSelectableItem* item = new (ELeave) CSelectableItem( *selectionText, settingActive );
                item->ConstructL();
                selection->AppendL(item);
                CleanupStack::PopAndDestroy(); //selectionText 
                }
            }
        
        CAknCheckBoxSettingPage* settingPage = 
            new( ELeave) CAknCheckBoxSettingPage( R_PREDICTIVE_TEXT_MULTI_SETTING_PAGE, selection);

        if( settingPage->ExecuteLD(CAknSettingPage::EUpdateWhenAccepted))
            {
            for(TUint i = 0x01; i <= 0xF0; i = i << 1)
                {
                if(physicalKeyboards & i)
                    {
                    switch(i)
                        {
                        case 0x01: // itut
                            if( (selection->At(itutIndex))->SelectionStatus() )
                                {
                                predictionBitmask = predictionBitmask | EKeyboardStyle12Key ;
                                }
                            else
                                {
                                predictionBitmask = predictionBitmask & ~EKeyboardStyle12Key ;
                                }                                                 
                            break;
                        case 0x10: // half
                            if( (selection->At(halfQwertyIndex))->SelectionStatus() )
                                {
                                predictionBitmask = predictionBitmask | EKeyboardStyleHalfQwerty ;
                                }
                            else
                                {
                                predictionBitmask = predictionBitmask & ~EKeyboardStyleHalfQwerty ;
                                }                                                 
                            break;
                        case 0x02: // 4x12
                        case 0x04: // 4x10
                        case 0x08: // 3x11                    
                        case 0x20: // custom
                            if( (selection->At(qwertyIndex))->SelectionStatus() )
                                {
                                predictionBitmask = predictionBitmask | EKeyboardStyleQwerty ;
                                }
                            else
                                {
                                predictionBitmask = predictionBitmask & ~EKeyboardStyleQwerty ;
                                }                                                 
                            i = 0x20; // Skip rest of qwerty checks
                            break;
                        default:
                            break;
                        }
                    }
                }
            Container()->Model()->SetPredictionBitMaskL(predictionBitmask);
            }
        CleanupStack::PopAndDestroy(); // selection
        }
    else
        {
        Container()->Model()->ChangePredictiveOnOffSettingL(
                aShowOnOffSettingPage,
                CGSLangModel::EGSLangOther );
        }
    }

// ----------------------------------------------------------------------------
// CGSLangPlugin::ShowPredictiveSettingListL()
// 
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSLangPlugin::ShowPredictiveSettingListL()
    {
    if (iCascadeOptionsOpen)
        {
        // Return if already open
        return;
        }
    
    CAknAppUi* appUi = static_cast<CAknAppUi*>(iCoeEnv->AppUi());

    CAknTitlePane* titlePane = static_cast<CAknTitlePane*>( appUi->StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );

    const TDesC* appTitle = titlePane->Text();
    HBufC* oldTitle = appTitle->AllocL();
    CleanupStack::PushL(oldTitle);
    HBufC* title = StringLoader::LoadLC( R_AKNFEP_PRED_INPUT_SETTINGS_TITLE, iCoeEnv);
    titlePane->SetTextL( *title );
    CleanupStack::PopAndDestroy(title);

    CGSPredictiveSettingDialog* dlg = CGSPredictiveSettingDialog::NewL(R_PREDICTIVESETTINGDIALOG_MENUBAR, R_PREDICTIVETEXTOFF_CONFIRMATION_QUERY);
    iDialog = dlg;
    iCascadeOptionsOpen = ETrue;
    dlg->ExecuteLD(R_PREDICTIVESETTING_DIALOG);
    iCascadeOptionsOpen = EFalse;
    iDialog = NULL;
    titlePane->SetTextL( *oldTitle );
    CleanupStack::PopAndDestroy(oldTitle);
    }
#endif

// ----------------------------------------------------------------------------
// CGSLangPlugin::DynInitMenuPaneL()
// 
// Display the dynamic menu
// ----------------------------------------------------------------------------
void CGSLangPlugin::DynInitMenuPaneL( TInt aResourceId,
                                      CEikMenuPane* aMenuPane )
    {
    // show or hide the 'help' menu item when supported
    if( aResourceId == R_GS_MENU_ITEM_HELP )
        {
        User::LeaveIfNull( aMenuPane );
            
        if ( FeatureManager::FeatureSupported( KFeatureIdHelp ) )
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, EFalse );
            }
        else
            {
            aMenuPane->SetItemDimmed( EAknCmdHelp, ETrue );
            }
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    // show or hide the change menu option when supported
    if(aResourceId == R_GS_MENU_ITEM_CHANGE)
        {
        User::LeaveIfNull( aMenuPane );
        TInt keyboardLayout = 0;
        RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
        TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
        TBool isPredictionSupport = EFalse;
        switch(layout)
            {
            case EPtiKeyboardHalfQwerty:
                isPredictionSupport = Container()->Model()->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEngineHalfQwertyPredictive);
                break;
            case EPtiKeyboardQwerty4x12:
            case EPtiKeyboardQwerty4x10:  
            case EPtiKeyboardQwerty3x11:
            case EPtiKeyboardCustomQwerty:
                isPredictionSupport = Container()->Model()->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEngineQwertyPredictive);
                break;
            case EPtiKeyboardNone:
            case EPtiKeyboard12Key:
                isPredictionSupport = Container()->Model()->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEnginePredictive);
                break;
            default:
                isPredictionSupport = Container()->Model()->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEnginePredictive);
                break;    
            }
        if (isPredictionSupport == EFalse)
            {
            aMenuPane->SetItemDimmed(EGSCmdAppChange, ETrue );
            }
        }
#endif    
        
    }
// -----------------------------------------------------------------------------
// CGSLangPlugin::CreateIconL()
//
//
// -----------------------------------------------------------------------------
//
CGulIcon* CGSLangPlugin::CreateIconL( const TUid aIconType )
    {
    //EMbm<Mbm_file_name><Bitmap_name>
    CGulIcon* icon;

    if( aIconType == KGSIconTypeLbxItem )
        {
        icon = AknsUtils::CreateGulIconL(
        AknsUtils::SkinInstance(),
        KAknsIIDQgnPropCpPersoLang,
        KGSDefaultLangIconFileName,
        EMbmGslangpluginQgn_prop_cp_perso_lang,
        EMbmGslangpluginQgn_prop_cp_perso_lang_mask );
        }
       else
        {
        icon = CGSPluginInterface::CreateIconL( aIconType );
        }

    return icon;
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT 
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
TInt CGSLangPlugin::HandleKeyboardLayoutChangeNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CGSLangPlugin*>(aObj)->HandleKeyboardLayoutChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

void CGSLangPlugin::HandleKeyboardLayoutChange()
    {
    if(iDialog)
        {
        iDialog->HandleResourceChange(KEikDynamicLayoutVariantSwitch);
        }
    if(iContainer)
		{
        Container()->HandleResourceChange(KEikDynamicLayoutVariantSwitch);
		}
    }

CGSLangPlugin::CSubscriber::CSubscriber(TCallBack aCallBack, RProperty& aProperty)
    :
    CActive(EPriorityNormal), iCallBack(aCallBack), iProperty(aProperty)
    {
    CActiveScheduler::Add(this);
    }

CGSLangPlugin::CSubscriber::~CSubscriber()
    {
    Cancel();
    }

void CGSLangPlugin::CSubscriber::SubscribeL()
    {
    if (!IsActive())
        {
        iProperty.Subscribe(iStatus);
        SetActive();
        }
    }

void CGSLangPlugin::CSubscriber::StopSubscribe()
    {
    Cancel();
    }

void CGSLangPlugin::CSubscriber::RunL()
    {
    if (iStatus.Int() == KErrNone)
        {
        iCallBack.CallBack();
        SubscribeL();
        }
    }

void CGSLangPlugin::CSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }
    
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
#endif
// End of File
