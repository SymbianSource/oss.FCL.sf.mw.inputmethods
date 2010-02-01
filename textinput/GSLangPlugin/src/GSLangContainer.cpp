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
* Description:  Container for GSLangPlugin.
*
*/


// INCLUDE FILES
#include    "GSLangContainer.h"
#include    "GSLangPlugin.h"
#include    "GSLangPlugin.hrh"
#include    "GSLangModel.h"
#include    <gslistbox.h> // CGSListBoxItemTextArray
#include    <gscommon.hrh>
#include    <GSLangPluginRsc.rsg>

#include    <bldvariant.hrh>

#include    <aknappui.h>
#include    <AknFepGlobalEnums.h>
#include    <AknFepInternalCRKeys.h>
#include    <aknlists.h>
#include    <akntitle.h> 
//#include    <ApProtHandler.h> // CApProtHandler
//#include    <aputils.h> // CApUtils
#include    <commdb.h> // CCommsDatabase
//#include    <csxhelp/cp.hlp.hrh>
#include    <gsfwviewuids.h>
#include    <eikspane.h>
#include    <featmgr.h>
#include    <syslangutil.h>
#include    <AknFepInternalCRKeys.h>
#include    <AvkonInternalCRKeys.h>

// CONSTANTS

// ========================= MEMBER FUNCTIONS ================================

// -----------------------------------------------------------------------------
// CGSLangContainer::CGSLangContainer
// C++ default constructor can NOT contain any code, that
// might leave.
// -----------------------------------------------------------------------------
//
CGSLangContainer::CGSLangContainer()
    {
    }

// ---------------------------------------------------------------------------
// CGSLangContainer::ConstructL(const TRect& aRect)
// Symbian OS two phased constructor
//
// ---------------------------------------------------------------------------
//
void CGSLangContainer::ConstructL( const TRect& aRect )
    {
    iModel = CGSLangModel::NewL( this );
    iUpdateITL = EFalse;
    iListBox = new( ELeave ) CAknSettingStyleListBox;
    BaseConstructL( aRect, R_GS_LANG_PLUGIN_TITLE, R_LANG_LBX );
    }


// ---------------------------------------------------------------------------
// CGSLangContainer::ConstructListBoxL(TInt aResLbxId)
// Construct the listbox items
//
// ---------------------------------------------------------------------------
//
void CGSLangContainer::ConstructListBoxL( TInt aResLbxId )
    {
    iListBox->ConstructL( this, EAknListBoxSelectionList );

    iListboxItemArray = CGSListBoxItemTextArray::NewL( aResLbxId,
        *iListBox, *iCoeEnv );
    iListBox->Model()->SetItemTextArray( iListboxItemArray );
    iListBox->Model()->SetOwnershipType( ELbmDoesNotOwnItemArray );

    // Set up UI language arrays - Display Text Language Construction
    CArrayFixFlat<TInt>* systemEpocLanguageCodes = 0;
    // This creates the list itself
    TInt error = SysLangUtil::GetInstalledLanguages( systemEpocLanguageCodes );
    if ( error != KErrNone )
        {
        delete systemEpocLanguageCodes;
        User::Leave( error );
        }
    CleanupStack::PushL( systemEpocLanguageCodes );

    delete iDtLanguagesCodes;
    iDtLanguagesCodes = 0;
    iDtLanguagesCodes = new (ELeave)CArrayFixFlat<TInt>(10);
    iDtLanguagesCodes->AppendL(0); // This is the code for "Automatic"

    delete iDtLanguageItems;
    iDtLanguageItems = 0;
    iDtLanguageItems = iCoeEnv->ReadDesC16ArrayResourceL(
        R_DISPLAY_TEXT_LANGUAGE_SETTING_PAGE_LBX);

    //This is for display text language
    iModel->GetLanguagesFromPtiL( systemEpocLanguageCodes,
                                  iDtLanguagesCodes, iDtLanguageItems );

    CleanupStack::PopAndDestroy(); // systemEpocLanguageCode

    // Input Text Language Constrution
    iWtLanguagesCodes = new(ELeave)CArrayFixFlat<TInt>(10);
    iWtLanguageItems = new(ELeave)CDesCArrayFlat(10);

    //This is for input text language
    iModel->GetLanguagesFromPtiL( NULL, iWtLanguagesCodes, iWtLanguageItems );

    //Number modes
    iAHNumberModeItems = iCoeEnv->ReadDesC16ArrayResourceL(
        R_AH_DEFAULT_INPUT_METHOD_LBX );

    iHindiNumberModeItems = iCoeEnv->ReadDesC16ArrayResourceL(
        R_HI_DEFAULT_INPUT_METHOD_LBX );
        
    // Predictive Text Language Construction
    iOnOffItems = iCoeEnv->ReadDesC16ArrayResourceL(
        R_T9_ON_OFF_SETTING_PAGE_LBX );
    iOnOffNotAvailable =
        iCoeEnv->AllocReadResourceL( R_T9_ONOFF_NOT_AVAILABLE );

    //Japanese specific features
    iJapPredTxtOnOffItems = iCoeEnv->ReadDesC16ArrayResourceL(
        R_T9_ON_OFF_SETTING_PAGE_LBX );        
    iKeypressTimeoutItems = iCoeEnv->ReadDesC16ArrayResourceL( 
        R_KEYPRESS_TIMEOUT_SETTING_PAGE_LBX );
    iJapQwertyCommaItems = iCoeEnv->ReadDesC16ArrayResourceL( 
        R_JAP_QWERTY_COMMA_SETTING_PAGE_LBX );
    iJapQwertyPeriodItems = iCoeEnv->ReadDesC16ArrayResourceL( 
        R_JAP_QWERTY_PERIOD_SETTING_PAGE_LBX );
    iJapQwertyWidthOfSpaceItems = iCoeEnv->ReadDesC16ArrayResourceL( 
        R_JAP_QWERTY_WIDTH_FULL_HALF_SETTING_PAGE_LBX );
    
    iChiCangJieItems = iCoeEnv->ReadDesC16ArrayResourceL( 
        R_CHI_CANGJIE_SETTING_PAGE_LBX );
	// Word Autocompletion
	iPredictiveModeItems = iCoeEnv->ReadDesC16ArrayResourceL( 
        R_PREDICTIVE_MODE_SETTING_PAGE_LBX );
    iInputMethodItems = 0;

    CreateListBoxItemsL();
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::CreateListBoxItemsL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::CreateListBoxItemsL()
    {
    MakeDTLItemL();

    const TInt uiLang = iModel->LanguageL( CGSLangModel::EGSUiLanguage );
    if (( uiLang == ELangArabic || User::Language() == ELangArabic ) ||
        ( uiLang == ELangUrdu || User::Language() == ELangUrdu ) || 
        (uiLang == ELangFarsi || User::Language() == ELangFarsi) && iElaf )
        {
        MakeAHNumberModeItemL();
        }
    if ( uiLang == ELangHindi || User::Language() == ELangHindi ||
         uiLang == ELangMarathi || User::Language() == ELangMarathi )
        {
        MakeHindiNumberModeItemL();
        }
    // if Japanese language variant is flashed
    if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
        {
        MakeEngPredTxtItemL();
        MakeResetATOKDictionaryItemL();
        MakeJnpPredTxtItemL();
        MakeKeypressTimeoutItemL();
        if ( FeatureManager::FeatureSupported( KFeatureIdQwertyInput ) )
            {
            MakeJapQwertyItemL( EGSLangIdJQC );
            MakeJapQwertyItemL( EGSLangIdJQP );
            MakeJapQwertyItemL( EGSLangIdJQWSP );
            }
        }
    else
        {
        MakeITLItemL();    //because we don't need this item in japanese
        MakeT9LItemL();
#ifndef RD_INTELLIGENT_TEXT_INPUT
        MakePredictiveModeItemL();
#else
        MakePredictiveOptionsItemL();
#endif
        }

    const TInt inputLang = iModel->LanguageL( CGSLangModel::EGSInputLanguage );
    if( FeatureManager::FeatureSupported( KFeatureIdChinese ) &&
        ( inputLang == ELangPrcChinese || inputLang == ELangHongKongChinese ||
        inputLang == ELangTaiwanChinese ) )
        {
        MakeDIMItemL();
        }
    
    // check for input language even if it is done also in CheckDictionary...
    // CangJie is only supported for HK Chinese variant
    if ( iModel->DefaultInputMethodL() == ECangJie && 
            iModel->CheckCangjieSupportL( inputLang ) )
        {
        MakeCangJieItemL();
        }
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::~CGSLangContainer
// Destructor.
//
// -----------------------------------------------------------------------------
//
CGSLangContainer::~CGSLangContainer()
    {
    if( iModel )
        {
        delete iModel;
        }
    if ( iDtLanguageItems )
        {
        delete iDtLanguageItems;
        }
    if ( iDtLanguagesCodes )
        {
        delete iDtLanguagesCodes;
        }
    if ( iWtLanguageItems )
        {
        delete iWtLanguageItems;
        }
    if ( iWtLanguagesCodes )
        {
        delete iWtLanguagesCodes;
        }
    if ( iAHNumberModeItems )
        {
        delete iAHNumberModeItems;
        }
    if ( iHindiNumberModeItems )
        {
        delete iHindiNumberModeItems;
        }
    if ( iInputMethodItems )
        {
        delete iInputMethodItems;
        }
    if ( iOnOffItems )
        {
        delete iOnOffItems;
        }
    if ( iOnOffNotAvailable )
        {
        delete iOnOffNotAvailable;
        }
    if ( iListboxItemArray )
        {
        delete iListboxItemArray;
        }
    if ( iJapPredTxtOnOffItems )
        {
        delete iJapPredTxtOnOffItems;
        }
    if ( iKeypressTimeoutItems )
        {
        delete iKeypressTimeoutItems;
        }
    if ( iJapQwertyCommaItems )
        {
        delete iJapQwertyCommaItems;
        }
    if ( iJapQwertyPeriodItems )
        {
        delete iJapQwertyPeriodItems;
        }
    if ( iJapQwertyWidthOfSpaceItems )
        {
        delete iJapQwertyWidthOfSpaceItems;
        }
    if ( iChiCangJieItems )
        {
        delete iChiCangJieItems;
        }
    delete iPredictiveModeItems;
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::CountComponentControls
//
//
// -----------------------------------------------------------------------------
//
TInt CGSLangContainer::CountComponentControls() const
    {
    return 1;
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::ComponentControl
//
//
// -----------------------------------------------------------------------------
//
CCoeControl* CGSLangContainer::ComponentControl( TInt /*aIndex*/ ) const
    {
    return iListBox;
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::UpdateListBoxL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::UpdateListBoxL( TInt aFeatureId )
    {
    const TInt uiLang = iModel->LanguageL( CGSLangModel::EGSUiLanguage );
    switch ( aFeatureId )
        {
        case EGSLangIdDTL:
            {
            MakeDTLItemL();
            if ( uiLang != 0 )
                {
                iUpdateITL = ETrue;
                }
            }
            break;
        case EGSLangIdITL:
            if ( !FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
                MakeITLItemL();
                }
            break;
        case EGSLangAHNumberMode:
            if( iElaf )
                {
                MakeAHNumberModeItemL();
                }
            break;
        case EGSLangHindiNumberMode:
            if ( uiLang == ELangHindi || User::Language() == ELangHindi ||
                 uiLang == ELangMarathi || User::Language() == ELangMarathi )
                {
                MakeHindiNumberModeItemL();
                }
            break;
        case EGSLangIdT9L:
            if ( !FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
                MakeT9LItemL();
#ifdef RD_INTELLIGENT_TEXT_INPUT
                MakePredictiveOptionsItemL();
#else
                MakePredictiveModeItemL();
#endif
                }
            break;
        case EGSLangIdEPT:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
                MakeEngPredTxtItemL();
                }
            break;
        case EGSLangIdResetAtokDict:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
                MakeResetATOKDictionaryItemL();
                }
            break;
        case EGSLangIdJPT:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
                MakeJnpPredTxtItemL();
                }
            break;
        case EGSLangIdKT:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
                MakeKeypressTimeoutItemL();
                }
            break;
        case EGSLangIdJQC:
        case EGSLangIdJQP:
        case EGSLangIdJQWSP:
            if ( FeatureManager::FeatureSupported( KFeatureIdJapanese ) &&
                 FeatureManager::FeatureSupported( KFeatureIdQwertyInput ) )
                {
                MakeJapQwertyItemL( aFeatureId );
                }
            break;
        case EGSLangIdDIM:
            if ( FeatureManager::FeatureSupported( KFeatureIdChinese ) )
                {
                MakeDIMItemL();
                }
            break;
        case EGSLangIdCangJie:
            //Check for support is not done here assuming it is checked
            //before calling this method with this ID.
            if ( iModel->DefaultInputMethodL() == ECangJie )
                {
                MakeCangJieItemL();
                }
            break;
        case EGSLangIdPredictiveMode:
        	if ( !FeatureManager::FeatureSupported( KFeatureIdJapanese ) )
                {
#ifndef RD_INTELLIGENT_TEXT_INPUT
        		MakePredictiveModeItemL();
#else
        		MakePredictiveOptionsItemL();
#endif
        		}
        default:
            break;
        }

    // Currently selected feature id
    const TInt currentlySelectedFeature = iListboxItemArray->CurrentFeature();
    
    //Make a list item visible based on feature ID
    MakeListItemVisibleL( aFeatureId, currentlySelectedFeature );
    
    // Update the listbox
    iListBox->HandleItemAdditionL();
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::MakeDTLItemL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakeDTLItemL()
    {
    // Get the item from the listbox model and then dynamically set
    // its caption
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );
    const TInt uiLanguage = iModel->LanguageL( CGSLangModel::EGSUiLanguage );
    TInt count = iDtLanguagesCodes->Count();

    for ( TInt index=0; index<count; ++index )
        {
        if ( ( *iDtLanguagesCodes )[ index ] == uiLanguage )
            {
            ptrBuffer = ( *iDtLanguageItems )[ index ];
            break;
            }
        }

    if ( !ptrBuffer.Length() && iDtLanguageItems->Count() > 0 )
        {
        ptrBuffer = ( *iDtLanguageItems )[0];
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSLangIdDTL, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLangIdDTL,
        CGSListBoxItemTextArray::EVisible );
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::MakeITLItemL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakeITLItemL()
    {
    // Dynamic text not wanted in case we have dual language suppport
#ifndef FF_DUAL_LANGUAGE_SUPPORT
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    // Get Input Language code
    const TInt inputLanguage = iModel->LanguageL( CGSLangModel::EGSInputLanguage );
    const TInt uiLanguage = iModel->LanguageL( CGSLangModel::EGSUiLanguage );
    TInt count = iWtLanguagesCodes->Count();
    for ( TInt index=0; index<count; ++index )
        {
        if ( !iUpdateITL )
            {
            if ( ( *iWtLanguagesCodes )[index] == inputLanguage )
                {
                ptrBuffer = ( *iWtLanguageItems )[index];
                break;
                }
            }
        else
            {
            if ( ( *iWtLanguagesCodes )[index] == uiLanguage &&
                 uiLanguage != 0 ) // 'Automatic' = 0
                {
                iModel->SetLanguageL( CGSLangModel::EGSInputLanguage,
                                    ( *iWtLanguagesCodes )[index] );
                ptrBuffer = ( *iWtLanguageItems )[index];
                break;
                }
            }
        }
    iUpdateITL = EFalse;
    if ( !ptrBuffer.Length() && iWtLanguageItems->Count() > 0 )
        {
        ptrBuffer = ( *iWtLanguageItems )[0];
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSLangIdITL, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );
#endif

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLangIdITL,
        CGSListBoxItemTextArray::EVisible );
        
    //Set visibility to the input method item, according to input language
    MakeListItemVisibleL( EGSLangIdDIM, EGSLangIdITL );
    MakeListItemVisibleL( EGSLangIdCangJie, EGSLangIdDIM );
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::MakeT9LItemL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakeT9LItemL()
    {
    const TInt mode = iModel->PredictiveOnOff( CGSLangModel::EGSLangOther );
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );
    //When input language is Chinese, it is check if English is 
    //supporting predictive                            
    TInt langCode = iModel->LanguageL( CGSLangModel::EGSInputLanguage);
    TBool checkEngine = EFalse;
    if ( FeatureManager::FeatureSupported( KFeatureIdChinese ) )
        {
        langCode = ELangEnglish;
        checkEngine = ETrue;   
        }
  
    if ( !iModel->CheckDictionaryFromPtiL(
            langCode ,checkEngine ) )
        {
        ptrBuffer = iOnOffNotAvailable->Des();
        }
    else 
        {
#ifdef RD_INTELLIGENT_TEXT_INPUT            
        if(!iModel->DeviceHasMultipleKeyboardsL())
#endif 
            {
            ptrBuffer = ( *iOnOffItems )[1-mode];
            }
        }
       
    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSLangIdT9L, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );
    
    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLangIdT9L,
        CGSListBoxItemTextArray::EVisible );
    if( !mode )
		{
		iListboxItemArray->SetItemVisibilityL( EGSLangIdPredictiveMode,
		    CGSListBoxItemTextArray::EInvisible );
		}
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::SetT9ItemVisibilityL
//
// Sets predictive text item's visibility
// -----------------------------------------------------------------------------
//
void CGSLangContainer::SetT9ItemVisibilityL( TBool aVisibility )
    {
    if( aVisibility )
        {
        iListboxItemArray->SetItemVisibilityL( EGSLangIdT9L,
        CGSListBoxItemTextArray::EVisible );
        }
    else
        {
        iListboxItemArray->SetItemVisibilityL( EGSLangIdT9L,
        CGSListBoxItemTextArray::EInvisible );
        }
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::MakeEngPredTxtItemL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakeEngPredTxtItemL()
    {
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );
    const TInt mode = iModel->PredictiveOnOff( CGSLangModel::EGSLangOther );
    if ( iModel->CheckDictionaryFromPtiL(
                 iModel->LanguageL( CGSLangModel::EGSInputLanguage ) ) )
        {
        ptrBuffer = ( *iOnOffItems )[1-mode];
        }
    else
        {
        ptrBuffer = iOnOffNotAvailable->Des();
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSLangIdEPT, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLangIdEPT,
        CGSListBoxItemTextArray::EVisible );
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::MakeResetATOKDictionaryItemL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakeResetATOKDictionaryItemL()
    {
    iListboxItemArray->SetItemVisibilityL(
        EGSLangIdResetAtokDict,
        CGSListBoxItemTextArray::EVisible );
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::MakeJnpPredTxtItemL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakeJnpPredTxtItemL()
    {
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    const TInt mode = iModel->PredictiveOnOff( CGSLangModel::EGSLangJapanese );
    if ( iModel->CheckDictionaryFromPtiL(
                 iModel->LanguageL( CGSLangModel::EGSInputLanguage ) ) )
        {
        ptrBuffer = ( *iJapPredTxtOnOffItems )[1-mode];
        }
    else
        {
        ptrBuffer = iOnOffNotAvailable->Des();
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSLangIdJPT, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLangIdJPT,
        CGSListBoxItemTextArray::EVisible );
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::MakeAHNumberModeItemL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakeAHNumberModeItemL()
    {
    const TInt defaultNumberModeIndex = 
               iModel->DefaultNumberMode( EGSNbrModeTypeArabic );
    
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );
    
    ptrBuffer = ( *iAHNumberModeItems )[defaultNumberModeIndex];

    // Set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSLangAHNumberMode, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLangAHNumberMode,
                                           CGSListBoxItemTextArray::EVisible );
    }

// -----------------------------------------------------------------------------
// CGSLangContainer::MakeHindiNumberModeItemL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakeHindiNumberModeItemL()
    {
    const TInt defaultNumberModeIndex = 
               iModel->DefaultNumberMode( EGSNbrModeTypeIndic );
    
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );
    
    ptrBuffer = ( *iHindiNumberModeItems )[defaultNumberModeIndex];

    // Set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSLangHindiNumberMode, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLangHindiNumberMode,
                                           CGSListBoxItemTextArray::EVisible );
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::MakeDIMItemL
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakeDIMItemL()
    {
    //fetch the current value from iModel:
    // current item is the Hexadecimal value of Input method code from CenRep
    TInt lang = iModel->LanguageL( CGSLangModel::EGSInputLanguage );
    TInt resId = 0;
     
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
	
	TBool isCangjieSupported = EFalse;   
    
    CGSRadioButtonSettingPageItemTextArray* items = 
        CGSRadioButtonSettingPageItemTextArray::NewL( resId, *iCoeEnv, NULL );
        
    
    CleanupStack::PushL(items);
    
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
				if (!(layout == EPtiKeyboard12Key || layout == EPtiKeyboardHalfQwerty))
					{
					isCangjieSupported = ETrue;
					}
					
                if ( iModel->CheckCangjieSupportL( ELangHongKongChinese ) && isCangjieSupported )
                    {
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
                                   
    TInt oldInputMethodCode =iModel->DefaultInputMethodL();
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
    
    TInt itemsNum = KGSMapSize;
    TMapArray* methodMap = NULL;
    if ( FeatureManager::FeatureSupported( KFeatureIdQwertyInput )
       || (FeatureManager::FeatureSupported(KFeatureIdVirtualKeyboardInput) &&
           FeatureManager::FeatureSupported(KFeatureIdChinese)))
        {
		    if ( isCangjieSupported )
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
  
    TInt featureId = items->FeatureIdByIndex( currentItem );
    HBufC* dynamicText = NULL;
    dynamicText = CGSItemTextArray::GetItemCaptionFromFeatureIdLC( 
                                                 resId, 
                                                 featureId, 
                                                 *iCoeEnv );
                                                 
    iListboxItemArray->SetDynamicTextL( EGSLangIdDIM, *dynamicText );
    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLangIdDIM,
        CGSListBoxItemTextArray::EVisible );
    
    CleanupStack::PopAndDestroy( dynamicText );
    CleanupStack::PopAndDestroy( items );
    }


// ---------------------------------------------------------------------------
// CGSLangContainer::MakeKeypressTimeoutItemL()
// 
// Creates the Keypress Timeout item  
// ---------------------------------------------------------------------------
//
void CGSLangContainer::MakeKeypressTimeoutItemL()
    {
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );

    // Get current data
    const TInt currentTime = iModel->KeypressTimeout();
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
    ptrBuffer = ( *iKeypressTimeoutItems )[ currentIndex ];

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSLangIdKT, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLangIdKT, 
        CGSListBoxItemTextArray::EVisible );
    }

// ---------------------------------------------------------------------------
// CGSLangContainer::MakeJapQwertyItemL()
// 
// Creates the Japanese Qwerty setting item  
// ---------------------------------------------------------------------------
//
void CGSLangContainer::MakeJapQwertyItemL( TInt aFeatureId )
    {
    TGSJapQwertyFlags flag = EGSJapQwertyFlagNone;
    CDesCArrayFlat* arrary = NULL;

    switch ( aFeatureId )
        {
        case EGSLangIdJQC:
            flag = EGSJapQwertyFlagComma;
            arrary = iJapQwertyCommaItems;
            break;
        case EGSLangIdJQP:
            flag = EGSJapQwertyFlagPeriod;
            arrary = iJapQwertyPeriodItems;
            break;
        case EGSLangIdJQWSP:
            flag = EGSJapQwertyFlagSpaceFullWidth;
            arrary = iJapQwertyWidthOfSpaceItems;
            break;
        default:
            break;
        }

    if ( flag != EGSJapQwertyFlagNone )
        {
        HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
        TPtr ptrBuffer ( dynamicText->Des() );
        const TBool currentValue = iModel->JapanQwertyFlags( flag );
        // If current value is ETrue, display index is 0.
        TInt dispIndex = ( currentValue )? 0 : 1;
        // get current index string
        ptrBuffer = ( *arrary )[ dispIndex ];  

        // Finally, set the dynamic text
        iListboxItemArray->SetDynamicTextL( aFeatureId, ptrBuffer );
        CleanupStack::PopAndDestroy( dynamicText );

        // And add to listbox
        iListboxItemArray->SetItemVisibilityL( aFeatureId, 
            CGSListBoxItemTextArray::EVisible );
        }
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::CurrentFeatureId
//
//
// -----------------------------------------------------------------------------
//
TInt CGSLangContainer::CurrentFeatureId() const
    {
    return iListboxItemArray->CurrentFeature( );
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::GetHelpContext
//
//
// -----------------------------------------------------------------------------
//
void CGSLangContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = HLP_GS_LANGUAGE_PLUGIN;
    }   


// -----------------------------------------------------------------------------
// CGSLangContainer::Model
//
//
// -----------------------------------------------------------------------------
//
CGSLangModel* CGSLangContainer::Model()
    {
    return iModel;
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::NumberModeItems
//
//
// -----------------------------------------------------------------------------
//
const MDesCArray* CGSLangContainer::NumberModeItems( TInt aNbrModeType )
    {
    if ( aNbrModeType == EGSNbrModeTypeArabic )
        {
        return iAHNumberModeItems;
        }
    else
        {
        return iHindiNumberModeItems;
        }
    }
    

// -----------------------------------------------------------------------------
// CGSLangContainer::MakeListItemVisibleL
//
// Make a list item visible, when needed
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakeListItemVisibleL( TInt aFeatureId, 
                                             TInt aCurrentlySelectedFeature )
    {
    switch ( aFeatureId )
        {
        case EGSLangAHNumberMode:
            // UI language was changed
            if ( iElaf && aCurrentlySelectedFeature == EGSLangIdDTL )
                {
                CGSListBoxItemTextArray::TVisibility digitTypeVisibility =
                    CGSListBoxItemTextArray::EInvisible;
                TInt uiLang = iModel->LanguageL( CGSLangModel::EGSUiLanguage );
                if ( ( uiLang == ELangArabic && User::Language() == ELangArabic )
                   || ( uiLang == ELangUrdu && User::Language() == ELangUrdu ) ||
                   ( uiLang == ELangFarsi && User::Language() == ELangFarsi ) )
                    {
                    digitTypeVisibility = CGSListBoxItemTextArray::EVisible;
                    }

                // Set the visibility
                iListboxItemArray->SetItemVisibilityL( aFeatureId,
                    digitTypeVisibility );
                }
            break;
        case EGSLangHindiNumberMode:
            // UI language was changed
            if ( aCurrentlySelectedFeature == EGSLangIdDTL )
                {
                CGSListBoxItemTextArray::TVisibility digitTypeVisibility =
                    CGSListBoxItemTextArray::EInvisible;
                if ( ( iModel->LanguageL( CGSLangModel::EGSUiLanguage ) == 
                     ELangHindi && User::Language() == ELangHindi ) ||
                     ( iModel->LanguageL( CGSLangModel::EGSUiLanguage ) == 
                     ELangMarathi && User::Language() == ELangMarathi ) )
                    {
                    digitTypeVisibility = CGSListBoxItemTextArray::EVisible;
                    }
    
                // Set the visibility
                iListboxItemArray->SetItemVisibilityL( aFeatureId,
                    digitTypeVisibility );
                }
            break;
        case EGSLangIdDIM:
            // Input language was changed
            if( FeatureManager::FeatureSupported( KFeatureIdChinese ) && !iElaf )
                {
                if ( aCurrentlySelectedFeature == EGSLangIdITL )
                    {
                    CGSListBoxItemTextArray::TVisibility 
                    defInputMethodVisibility = CGSListBoxItemTextArray::EVisible;
                    TInt inputLanguage = 
                         iModel->LanguageL( CGSLangModel::EGSInputLanguage );
                    if ( inputLanguage != ELangPrcChinese &&
                         inputLanguage != ELangHongKongChinese &&
                         inputLanguage != ELangTaiwanChinese )
                        {
                        defInputMethodVisibility = 
                            CGSListBoxItemTextArray::EInvisible;
                        }
    
                // Set the visibility
                iListboxItemArray->SetItemVisibilityL( aFeatureId,
                    defInputMethodVisibility );
                }
            }
            break;
        case EGSLangIdCangJie:
            // Default Input Language/Method was changed
            if ( aCurrentlySelectedFeature == EGSLangIdDIM )
                {
                CGSListBoxItemTextArray::TVisibility 
                        cangJieItemVisibility = CGSListBoxItemTextArray::EInvisible;            
                TInt defaultInputMethod = 
                             iModel->DefaultInputMethodL();
                TInt inputLanguage = iModel->LanguageL( CGSLangModel::EGSInputLanguage );
                            
                if ( defaultInputMethod == ECangJie && 
                        iModel->CheckCangjieSupportL( inputLanguage ) )
                    {
                    cangJieItemVisibility = CGSListBoxItemTextArray::EVisible;
                    }
        
                // Set the visibility
                iListboxItemArray->SetItemVisibilityL( aFeatureId,
                    cangJieItemVisibility );
                }
            break;
        default:
            break;
        }    
    
    }
    
// -----------------------------------------------------------------------------
// CGSLangContainer::MakeCangJieItemL
//
// Should be visible only if Input language is Chinese HK and this supports
// CangJie input mode. Checked from PtiEngine.
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakeCangJieItemL()
    {
    HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    TPtr ptrBuffer ( dynamicText->Des() );
    
    const TInt selectedItem = iModel->ChiCangJieInputL();
    const TInt inputLang = iModel->LanguageL( CGSLangModel::EGSInputLanguage );
    if ( iModel->CheckDictionaryFromPtiL( inputLang ) )
        {
        ptrBuffer = ( *iChiCangJieItems )[ selectedItem ];
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL( EGSLangIdCangJie, ptrBuffer );
    CleanupStack::PopAndDestroy( dynamicText );

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL( EGSLangIdCangJie,
        CGSListBoxItemTextArray::EVisible );
    }


// -----------------------------------------------------------------------------
// CGSLangContainer::MakePredictiveModeItemL
// Towards Word Autocompletion Req from 4.0
// Create Predictvie mode setting values
// 0   --->  Normal
// 1   --->  Autocompletion
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakePredictiveModeItemL()
    {
    if ( iModel->PredictiveOnOff( CGSLangModel::EGSLangOther ) )
    	{
    	HBufC* dynamicText = HBufC::NewLC( KGSBufSize128 );
    	TPtr ptrBuffer ( dynamicText->Des() );
    
    	const TInt selectedItem = iModel->PredictiveModeL();
    	if ( iModel->CheckDictionaryFromPtiL(
		             iModel->LanguageL( CGSLangModel::EGSInputLanguage ) ) )
		    {
		    ptrBuffer = ( *iPredictiveModeItems )[ selectedItem ];
		    }
		else
		    {
		    ptrBuffer = iOnOffNotAvailable->Des();
		    }
    	
    	#ifndef RD_SCALABLE_UI_V2
    	if( iModel->CheckAutoWordCompletionSupportL() )
    		{    			 
    		// Finally, set the dynamic text
    		iListboxItemArray->SetDynamicTextL( EGSLangIdPredictiveMode, ptrBuffer );
    	
    		// And add to listbox
    		iListboxItemArray->SetItemVisibilityL( EGSLangIdPredictiveMode,
        						   CGSListBoxItemTextArray::EVisible );        	
        	}
      	#endif // RD_SCALABLE_UI_V2  		
      				   
      	CleanupStack::PopAndDestroy( dynamicText );    	
    	   	       
        }
    }
#ifdef RD_INTELLIGENT_TEXT_INPUT
// -----------------------------------------------------------------------------
// CGSLangContainer::MakePredictiveSettingListL
// -----------------------------------------------------------------------------
//
void CGSLangContainer::MakePredictiveOptionsItemL()
    {    
    // Check whether the language supports prediction or not 
    // if not disable input options from language menu
    TInt keyboardLayout = 0;
    RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
    TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
    TBool isPredictionSupport = EFalse;
    switch(layout)
        {
        case EPtiKeyboardHalfQwerty:
            isPredictionSupport = iModel->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEngineHalfQwertyPredictive);
            break;
        case EPtiKeyboardQwerty4x12:
        case EPtiKeyboardQwerty4x10:  
        case EPtiKeyboardQwerty3x11:
        case EPtiKeyboardCustomQwerty:
            isPredictionSupport = iModel->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEngineQwertyPredictive);
            break;
        case EPtiKeyboardNone:
        case EPtiKeyboard12Key:
#ifndef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
            isPredictionSupport = iModel->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEnginePredictive);
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__            
            break;
        default:
            isPredictionSupport = iModel->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEnginePredictive);
            break;     
        }
    if (isPredictionSupport == EFalse)
        {
        iListboxItemArray->SetItemVisibilityL( EGSLangIdPredictiveOptions,
                CGSListBoxItemTextArray::EInvisible );
        }
    else
        {
        iListboxItemArray->SetItemVisibilityL( EGSLangIdPredictiveOptions,
                CGSListBoxItemTextArray::EVisible );
        }
    }

void CGSLangContainer::HandleResourceChange( TInt aType )
    {
    CGSBaseContainer::HandleResourceChange(aType);
    TInt keyboardLayout = 0;
        RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
        TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
        TBool isPredictionSupport = EFalse;
        switch(layout)
            {
            case EPtiKeyboardHalfQwerty:
                isPredictionSupport = iModel->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEngineHalfQwertyPredictive);
                break;
            case EPtiKeyboardQwerty4x12:
            case EPtiKeyboardQwerty4x10:  
            case EPtiKeyboardQwerty3x11:
            case EPtiKeyboardCustomQwerty:
                isPredictionSupport = iModel->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEngineQwertyPredictive);
                break;
            case EPtiKeyboardNone:
            case EPtiKeyboard12Key:
                // Commenting out the below function call as ITI is not supported for Touch Input.
    #ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
                //isPredictionSupport = iModel->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEnginePredictive);
    #else
                isPredictionSupport = iModel->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEnginePredictive);
    #endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
                break;
            default:
                isPredictionSupport = iModel->CheckT9FromPtiLForPredictionL((TPtiEngineInputMode)EPtiEnginePredictive);
                break;     
            }
        if (isPredictionSupport == EFalse)
            {
            iListboxItemArray->SetItemVisibilityL( EGSLangIdPredictiveOptions,
                    CGSListBoxItemTextArray::EInvisible );
            }
        else
            {
            iListboxItemArray->SetItemVisibilityL( EGSLangIdPredictiveOptions,
                    CGSListBoxItemTextArray::EVisible );
            }
        iListBox->HandleItemAdditionL();
        MakeT9LItemL();
    }
#endif
// End of File
