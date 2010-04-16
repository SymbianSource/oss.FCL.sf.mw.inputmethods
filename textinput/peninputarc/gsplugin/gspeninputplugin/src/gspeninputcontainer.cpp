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
* Description:  Container for GSPenInputPlugin.
*
*/


#include <gscommon.hrh>
#include <gspeninputpluginrsc.rsg>
#include <aknlists.h>
#include <gslistbox.h>     
#include <featmgr.h>
#include <aknradiobuttonsettingpage.h> 
#include <AknColourSelectionGrid.h> 
#include <aknlayoutscalable_avkon.cdl.h>
#include <avkon.mbg>
#include <AknsDrawUtils.h>
#include <AknsFrameBackgroundControlContext.h>
#include <skinlayout.cdl.h>
#include <aknconsts.h>
#include <aknslider.h>
#include <settingsinternalcrkeys.h>
#include <AknFepInternalCRKeys.h>
#include <AvkonInternalCRKeys.h>
//#include <csxhelp\cp.hlp.hrh> //Temp fix for helps integration build break;
#include <AknNullService.h>

using namespace SkinLayout;

#include "gspeninputcontainer.h"
#include "gspeninputplugin.h"
#include "gspeninputplugin.hrh"
#include "gspeninputmodel.h"
#include "peninputpluginutils.h"
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSPenInputContainer::CGSPenInputContainer()
    {
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
void CGSPenInputContainer::ConstructL(const TRect& aRect)
    {
    iModel = CGSPenInputModel::NewL(this);
    iListBox = new (ELeave) CAknSettingStyleListBox;
    
    iModel->InputLanguage();
    iModel->InputMode();
    iModel->TrailColour();
    iModel->TrailWidth();
    iModel->GuideLine();
    iModel->InputMethodForFind();
    iModel->ChineseFindMethod();
    iModel->RecognitionWithDictionary();

    BaseConstructL(aRect, R_CP_INPUTPEN_PLUGIN_TITLE, R_INPUTPEN_LBX);
    iChinese = iModel->ChineseLanguage();

    iJapanese = iModel->JapaneseLanguage();
    }

// ---------------------------------------------------------
// This callback method is used to notify the client about
// key changing
// ---------------------------------------------------------
//
void CGSPenInputContainer::HandleNotifyInt(TUint32 aId, TInt /*aNewValue*/)
    {
    switch (aId)
        {
        case KAknFepInputTxtLang:
            {
            TRAP_IGNORE(UpdateListBoxL(EGSInputpenIdInputLanguage));
            }
            break;
        case KSettingsWritingSpeed:
            {
            TRAP_IGNORE(UpdateListBoxL(EGSInputpenIdWritingSpeed));
            }
            break;
        case KSettingsPenInputGuideLine:
            {
            TRAP_IGNORE(UpdateListBoxL(EGSInputpenIdGuidLine));
            }
            break;
        case KSettingRecognitionwithdictionary:
            {
            TRAP_IGNORE(UpdateListBoxL(EGSInputpenIdRecognitionWithDictionary));
            }
            break;            
        case KSettingsPenWidth:
            {
            TRAP_IGNORE(UpdateListBoxL(EGSInputpenIdPenTrailWidth));
            }
            break;
        case KSettingsPenColor:
            {
            TRAP_IGNORE(UpdateListBoxL(EGSInputpenIdPenTrailColour));
            }
            break;
        case KSettingsDefaultVkbPrc:
        case KSettingsDefaultVkbTw:
        case KSettingsDefaultVkbHk:
            {
            TRAP_IGNORE(UpdateListBoxL(EGSDefaultChineseOnscreenKeyboard));
            }
            break;
        default:
            break;
        }
    }
// ---------------------------------------------------------
// This callback method is used to notify the client about 
// key changing
// ---------------------------------------------------------
//
void CGSPenInputContainer::HandleNotifyInt( TUid aRepositoryUid, TUint32 aId, TInt aNewValue )
    {
    TRAP_IGNORE
        (
        if ( aRepositoryUid == KCRUidPersonalizationSettings )
            {
            HandleNotifyIntFromPersonalizationL( aId, aNewValue );
            }
        else if ( aRepositoryUid == KCRUidAknFep )
            {
            HandleNotifyIntFromAknFepL( aId, aNewValue );
            }
        else if ( aRepositoryUid == KCRUidAvkon )
            {
            HandleNotifyIntFromAvkonL( aId, aNewValue );
            }
        )
    }
// ---------------------------------------------------------
// Handle notifier from KCRUidPersonalizationSettings 
// in CRepository
// ---------------------------------------------------------
//
void CGSPenInputContainer::HandleNotifyIntFromPersonalizationL
                         ( TUint32 aId, TInt /*aNewValue*/ )
    {
    switch ( aId )
        {
        case KSettingsWritingSpeed:
            {
            UpdateListBoxL(EGSInputpenIdWritingSpeed);
            break;
            }
        case KSettingsPenInputGuideLine:
            {
            UpdateListBoxL(EGSInputpenIdGuidLine);
            break;
            }
        case KSettingsPenWidth:
            {
            UpdateListBoxL(EGSInputpenIdPenTrailWidth);
            break;
            }
        case KSettingsPenColor:
            {
            UpdateListBoxL(EGSInputpenIdPenTrailColour);
            break;
            }
        case KSettingsDefaultVkbPrc:
        case KSettingsDefaultVkbHk:
        case KSettingsDefaultVkbTw:
            {
            UpdateListBoxL(EGSDefaultChineseOnscreenKeyboard);
            break;
            }
        case KSettingRecognitionwithdictionary:
            {
            UpdateListBoxL(EGSInputpenIdRecognitionWithDictionary);
            break;
            }
        default:
            break;
        }                            
    }
    
// ---------------------------------------------------------
// Handle notifier from KCRUidAknFep in CRepository
// ---------------------------------------------------------
//
void CGSPenInputContainer::HandleNotifyIntFromAknFepL
                         ( TUint32 aId, TInt /*aNewValue*/ )
    {
    switch ( aId )
        {
        case KAknFepInputTxtLang:
            {
            UpdateListBoxL(EGSInputpenIdInputLanguage);
            break;
            }
        default:
            break;
        }        
    }

// ---------------------------------------------------------
// Handle notifier from KCRUidAvkon in CRepository
// ---------------------------------------------------------
//
void CGSPenInputContainer::HandleNotifyIntFromAvkonL
                         ( TUint32 aId, TInt /*aNewValue*/ )
    {
    switch ( aId )
        {
        case KAknAvkonAdaptiveSearchEnabled:
            {
            UpdateListBoxL(EGSInputpenIdInputMethodForFind);
            break;
            }
        case KAknAdaptiveSearchChinesePRC:
        case KAknAdaptiveSearchChineseTW:
        case KAknAdaptiveSearchChineseHongkong:
            {
            UpdateListBoxL(EGSInputpenIdChineseFindMethod);
            break;            
            }
        default:
            break;
        }            
    }
            
// ---------------------------------------------------------
// Constructs listbox 
// ---------------------------------------------------------
//
void CGSPenInputContainer::ConstructListBoxL(TInt aResLbxId)
    {
    iListBox->ConstructL(this, EAknListBoxSelectionList);
    
    iListboxItemArray = CGSListBoxItemTextArray::NewL(aResLbxId,
                                                      *iListBox,
                                                      *iCoeEnv);
    
    iListBox->Model()->SetItemTextArray(iListboxItemArray);
    iListBox->Model()->SetOwnershipType(ELbmDoesNotOwnItemArray);

    delete iWritingSpeedItems;
    iWritingSpeedItems = NULL;
    
    iWritingSpeedItems = iCoeEnv->ReadDesC16ArrayResourceL(
                                  R_GS_WRITINGSPEED_SETTING_PAGE_LBX);

    delete iPenTrailWidthItems;
    iPenTrailWidthItems = NULL;
    
    iPenTrailWidthItems = iCoeEnv->ReadDesC16ArrayResourceL(
                                   R_GS_PENTRAIL_SETTING_PAGE_LBX);
                                   
    delete iGuideLineItems;
    iGuideLineItems = NULL;
    
    iGuideLineItems = iCoeEnv->ReadDesC16ArrayResourceL(
                                   R_GS_GUIDLINE_SETTING_PAGE_LBX);
    
    delete iRecognitionWithDictionaryItems;
    iRecognitionWithDictionaryItems = NULL;
    
    iRecognitionWithDictionaryItems = iCoeEnv->ReadDesC16ArrayResourceL(
            R_GS_RECOGNIZEWITHDICTIONARY_SETTING_PAGE_LBX);    

    delete iInputMethodForFindItems;
    iInputMethodForFindItems = NULL;

    iInputMethodForFindItems = iCoeEnv->ReadDesC16ArrayResourceL(
                                   R_GS_INPUTMETHODFORFIND_SETTING_PAGE_LBX);

    delete iChineseFindMethodItems;
    iChineseFindMethodItems = NULL;

    iChineseFindMethodResourceID = iModel->ChineseFindMethodResourceID();
    if (iChineseFindMethodResourceID != -1)
        {
        iChineseFindMethodItems = iCoeEnv->ReadDesC16ArrayResourceL(
                                   iChineseFindMethodResourceID);
        }

    delete iInputModeItems;
    iInputModeItems = NULL;
    
    if (iModel->InputModeResourceID() != -1)
        {
        iInputModeItems = iCoeEnv->ReadDesC16ArrayResourceL(
                                   iModel->InputModeResourceID());
        iModel->ResetInputModeArray(iInputModeItems);	
        }
        
    CArrayPtr<CGulIcon>* icons = new (ELeave) CArrayPtrFlat<CGulIcon>(2);
    CleanupStack::PushL(icons);
    CGulIcon* icon = CGulIcon::NewL();
    CleanupStack::PushL(icon);
    
    icons->AppendL(icon);

    CEikFormattedCellListBox* listbox = static_cast<CEikFormattedCellListBox*>(iListBox);
    
    listbox->ItemDrawer()->FormattedCellData()->SetIconArray(icons);// icons owned by List box
    
    CleanupStack::Pop(icon);
    CleanupStack::Pop(icons);
    
    iIconArray = listbox->ItemDrawer()->FormattedCellData()->IconArray();
     
    CreateListBoxItemsL();
    }

// ---------------------------------------------------------
// Constructs listbox items
// ---------------------------------------------------------
//
void CGSPenInputContainer::CreateListBoxItemsL()
    {
    MakeHwrTruiItemL();
    MakeInputLanguageL();
    MakeInputModeL();
    MakeSpeedItemL();
    MakeGuideItemL();
    MakeTrailWidthItemL();
    MakeRecognitionWithDictionaryL();
    MakeTrailColourItemL();
    MakeTouchScrCalibItemL();
    MakeInputMethodForFindItemL();
    MakeChineseFindMethodItemL();
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CGSPenInputContainer::~CGSPenInputContainer()
    {
    delete iModel;
    delete iWritingSpeedItems;
    delete iGuideLineItems;
    delete iRecognitionWithDictionaryItems;
    delete iPenTrailWidthItems;
    delete iInputMethodForFindItems;
    delete iChineseFindMethodItems;
    delete iInputModeItems;
    delete iListboxItemArray;        
    delete iNullService;
    }

// ---------------------------------------------------------
// Updates listbox item's value
// ---------------------------------------------------------
//
void CGSPenInputContainer::UpdateListBoxL(TInt aFeatureId)
    {
    switch (aFeatureId)
        {
        case EGSInputpenIdInputLanguage:
            {
            MakeInputLanguageL();
            }
            break;
        case EGSDefaultChineseOnscreenKeyboard:
            {
            MakeInputModeL();
            }
            break;
        case EGSInputpenIdWritingSpeed:
            {
            MakeSpeedItemL();
            }
            break;
        case EGSInputpenIdGuidLine:
            {
            MakeGuideItemL();
            }
            break; 
        case EGSInputpenIdRecognitionWithDictionary:
            {
            MakeRecognitionWithDictionaryL();
            }
            break;            
        case EGSInputpenIdPenTrailWidth:
            {
            MakeTrailWidthItemL();
            }
            break;
        case EGSInputpenIdPenTrailColour:
            {
            MakeTrailColourItemL();
            }
            break;
        case EGSInputpenIdInputMethodForFind:
            {
            MakeInputMethodForFindItemL();
            }
        case EGSInputpenIdChineseFindMethod:
            {
            MakeChineseFindMethodItemL();
            }
            break;            
        default:
            break;  
        }
    // Update the listbox
    iListBox->HandleItemAdditionL();    
    }

// ---------------------------------------------------------
// Retrieves the currently selected listbox id
// ---------------------------------------------------------
//
TInt CGSPenInputContainer::CurrentSelectedId() const
    {
    return iListboxItemArray->CurrentFeature();
    }

// ---------------------------------------------------------
// Create listbox input language item 
// ---------------------------------------------------------
//
void CGSPenInputContainer::MakeInputLanguageL()
    {
    HBufC* dynamicText = HBufC::NewLC(KGSBufSize);
    TPtr ptrBuffer(dynamicText->Des());
    
    iModel->InputLanguage(ptrBuffer); 

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL(EGSInputpenIdInputLanguage, ptrBuffer);
    
    CleanupStack::PopAndDestroy(dynamicText);

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL(EGSInputpenIdInputLanguage,
                                          CGSListBoxItemTextArray::EVisible);
                                          
    // Decide handwritting training visible
    MakeHwrTruiItemL();
    // Decide if show Default chinese on-screen keyboard
    MakeInputModeL();
    // Decide which chinese find method should be shown
    MakeChineseFindMethodItemL();
    // Decide which guideline should be shown
    MakeGuideItemL();
    
    MakeTrailColourItemL();
    
    MakeTrailWidthItemL();
    
    MakeSpeedItemL();
    
    MakeRecognitionWithDictionaryL();
    }

// ---------------------------------------------------------
// Create  input mode item 
// ---------------------------------------------------------
//
void CGSPenInputContainer::MakeInputModeL()
    {
    TBool bVisible = iModel->IsSettingItemAvailable(EGSDefaultChineseOnscreenKeyboard);       

    delete iInputModeItems;
    iInputModeItems = NULL;
    // if current input language is not Chinese, 
    // Hide default chinese on-screen keyboard
    if ( iListboxItemArray && !iModel->ChineseLanguage() )
        {        
        iListboxItemArray->SetItemVisibilityL(
                EGSDefaultChineseOnscreenKeyboard, 
                CGSItemTextArray::EInvisible);
        } 

    if (iModel->InputModeResourceID() != -1)
        {
        iInputModeItems = iCoeEnv->ReadDesC16ArrayResourceL(
                                   iModel->InputModeResourceID());
        iModel->ResetInputModeArray(iInputModeItems);	
        }
    else
        {
        return;
        }
        
    HBufC* dynamicText = HBufC::NewLC(KGSBufSize);
    TPtr ptrBuffer(dynamicText->Des());
    iModel->InputMode();
    const TInt index = iModel->InputModeInSettingPage();
    
    if ( iInputModeItems && index < iInputModeItems->Count() )
        {
        ptrBuffer = (*iInputModeItems)[index];
        }    

    // Finally, set the dynamic text
    
    if( iListboxItemArray != NULL )
        {
        iListboxItemArray->SetDynamicTextL(EGSDefaultChineseOnscreenKeyboard, ptrBuffer);
        }
    CleanupStack::PopAndDestroy(dynamicText);

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL(
            EGSDefaultChineseOnscreenKeyboard, 
            bVisible ? CGSItemTextArray::EVisible : CGSItemTextArray::EInvisible);
    }

// ---------------------------------------------------------
// Create listbox writing speed item 
// ---------------------------------------------------------
//
void CGSPenInputContainer::MakeGuideItemL()
    {

    TBool bVisible = iModel->IsSettingItemAvailable(EGSInputpenIdGuidLine);       

    
    HBufC* dynamicText = HBufC::NewLC(KGSBufSize);
    TPtr ptrBuffer(dynamicText->Des());
    const TInt guideLine = iModel->GuideLine();

    ptrBuffer = (*iGuideLineItems)[guideLine == 0 ? 1:0];


    if (!ptrBuffer.Length() && iGuideLineItems->Count() > 0)
        {
        ptrBuffer = (*iGuideLineItems)[0];
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL(EGSInputpenIdGuidLine, ptrBuffer);
    CleanupStack::PopAndDestroy(dynamicText);

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL(
            EGSInputpenIdGuidLine, 
            bVisible ? CGSItemTextArray::EVisible : CGSItemTextArray::EInvisible);
    }
// ---------------------------------------------------------
// Create listbox for the item of recognition with dictionary 
// ---------------------------------------------------------
//
void CGSPenInputContainer::MakeRecognitionWithDictionaryL()
    {

    TBool bVisible = iModel->IsSettingItemAvailable(EGSInputpenIdRecognitionWithDictionary);       

    
    HBufC* dynamicText = HBufC::NewLC(KGSBufSize);
    TPtr ptrBuffer(dynamicText->Des());
    const TInt guideLine = iModel->RecognitionWithDictionary();

    ptrBuffer = (*iRecognitionWithDictionaryItems)[guideLine == 0 ? 1:0];


    if (!ptrBuffer.Length() && iRecognitionWithDictionaryItems->Count() > 0)
        {
        ptrBuffer = (*iRecognitionWithDictionaryItems)[0];
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL(EGSInputpenIdRecognitionWithDictionary, ptrBuffer);
    CleanupStack::PopAndDestroy(dynamicText);

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL(
            EGSInputpenIdRecognitionWithDictionary, 
            bVisible ? CGSItemTextArray::EVisible : CGSItemTextArray::EInvisible);
    }

// ---------------------------------------------------------
// Create listbox writing speed item 
// ---------------------------------------------------------
//
void CGSPenInputContainer::MakeSpeedItemL()
    {
    TBool bVisible = iModel->IsSettingItemAvailable(EGSInputpenIdWritingSpeed);
    
    if(bVisible)
        {
        HBufC* dynamicText = HBufC::NewLC(KGSBufSize);
        TPtr ptrBuffer(dynamicText->Des());
        const TInt writingSpeed = iModel->WritingSpeed();
        const TInt count = iWritingSpeedItems->Count();

        for (TInt index = 0; index < count; ++index)
            {
            if (index == writingSpeed)
                {
                ptrBuffer = (*iWritingSpeedItems)[index];
                break;
                }
            }

        if (!ptrBuffer.Length() && iWritingSpeedItems->Count() > 0)
            {
            ptrBuffer = (*iWritingSpeedItems)[0];
            }

        // Finally, set the dynamic text
        iListboxItemArray->SetDynamicTextL(EGSInputpenIdWritingSpeed, ptrBuffer);
        CleanupStack::PopAndDestroy(dynamicText);
        }

    // Enable or disable to listbox
    iListboxItemArray->SetItemVisibilityL(EGSInputpenIdWritingSpeed,
        bVisible ? CGSListBoxItemTextArray::EVisible : CGSListBoxItemTextArray::EInvisible);    
    }
    
// ---------------------------------------------------------
// Create listbox pen trail width item
// ---------------------------------------------------------
//
void CGSPenInputContainer::MakeTrailWidthItemL()
    {
    TBool bVisible = EFalse;
    if( !(FeatureManager::FeatureSupported( KFeatureIdFfCapacitiveDisplay )) )
        {
        bVisible = iModel->IsSettingItemAvailable(EGSInputpenIdPenTrailWidth);
        }
    
    if(bVisible)
        {
        HBufC* dynamicText = HBufC::NewLC(KGSBufSize);
        TPtr ptrBuffer(dynamicText->Des());
        const TInt trailwidth = iModel->TrailWidth();
        const TInt count = iPenTrailWidthItems->Count();

        for (TInt index = 0; index < count; ++index)
            {
            if (index == trailwidth)
                {
                ptrBuffer = (*iPenTrailWidthItems)[index];
                break;
                }
            }

        if (!ptrBuffer.Length() && iPenTrailWidthItems->Count() > 0)
            {
            ptrBuffer = (*iPenTrailWidthItems)[0];
            }

        // Finally, set the dynamic text
        iListboxItemArray->SetDynamicTextL(EGSInputpenIdPenTrailWidth, ptrBuffer);
        CleanupStack::PopAndDestroy(dynamicText);
        }

    // enable or disable to listbox
    iListboxItemArray->SetItemVisibilityL(EGSInputpenIdPenTrailWidth,
        bVisible ? CGSListBoxItemTextArray::EVisible : CGSListBoxItemTextArray::EInvisible);       
    }
    
// ---------------------------------------------------------
// Create listbox pen trail colour item
// ---------------------------------------------------------
//
void CGSPenInputContainer::MakeTrailColourItemL()
    {
    TBool bVisible = iModel->IsSettingItemAvailable(EGSInputpenIdPenTrailColour);
    if(bVisible)
        {
        CreateIconL();
        }
    
    iListboxItemArray->SetItemVisibilityL(EGSInputpenIdPenTrailColour, 
            bVisible ? CGSListBoxItemTextArray::EVisible : CGSListBoxItemTextArray::EInvisible);
    }
    
// ---------------------------------------------------------
// Create listbox touch screen calibration item
// ---------------------------------------------------------
//
void CGSPenInputContainer::MakeTouchScrCalibItemL()
    {
    if( FeatureManager::FeatureSupported( KFeatureIdPenSupportCalibration ) )
        {
        // And add to listbox
        iListboxItemArray->SetItemVisibilityL(EGSInputpenIdTouchScrCalib, 
                                              CGSListBoxItemTextArray::EVisible);
        }
    else
        {
        iListboxItemArray->SetItemVisibilityL(EGSInputpenIdTouchScrCalib, 
                                              CGSListBoxItemTextArray::EInvisible);
        }
    }

// ---------------------------------------------------------
// Create listbox input method for find item 
// ---------------------------------------------------------
//
void CGSPenInputContainer::MakeInputMethodForFindItemL()
    {
    HBufC* dynamicText = HBufC::NewLC(KGSBufSize);
    TPtr ptrBuffer(dynamicText->Des());
    const TInt inputMethodForFind = iModel->InputMethodForFind();

    ptrBuffer = (*iInputMethodForFindItems)[inputMethodForFind];


    if (!ptrBuffer.Length() && iInputMethodForFindItems->Count() > 0)
        {
        ptrBuffer = (*iInputMethodForFindItems)[0];
        }

    // Finally, set the dynamic text
    iListboxItemArray->SetDynamicTextL(EGSInputpenIdInputMethodForFind, ptrBuffer);
    CleanupStack::PopAndDestroy(dynamicText);

    // And add to listbox
    iListboxItemArray->SetItemVisibilityL(EGSInputpenIdInputMethodForFind,
                                          CGSListBoxItemTextArray::EVisible);
    }

// ---------------------------------------------------------
// Create listbox chinese find method item 
// ---------------------------------------------------------
//
void CGSPenInputContainer::MakeChineseFindMethodItemL()
    {
    if(iChineseFindMethodResourceID != iModel->ChineseFindMethodResourceID())
        {
        iChineseFindMethodResourceID = iModel->ChineseFindMethodResourceID();
        delete iChineseFindMethodItems;
        iChineseFindMethodItems = NULL;

        if (iChineseFindMethodResourceID != -1)
            {
            iChineseFindMethodItems = iCoeEnv->ReadDesC16ArrayResourceL(
                                       iChineseFindMethodResourceID);
            }
        }

    if (iModel->IsSettingItemAvailable(EGSInputpenIdChineseFindMethod))
        {
        HBufC* dynamicText = HBufC::NewLC(KGSBufSize);
        TPtr ptrBuffer(dynamicText->Des());
        const TInt chineseFindMethod = iModel->ChineseFindMethod();
        
        if( iChineseFindMethodItems != NULL )
            {
            ptrBuffer = (*iChineseFindMethodItems)[chineseFindMethod];
            }

        if (!ptrBuffer.Length() && iChineseFindMethodItems->Count() > 0)
            {
            ptrBuffer = (*iChineseFindMethodItems)[0];
            }

        // Finally, set the dynamic text
        iListboxItemArray->SetDynamicTextL(EGSInputpenIdChineseFindMethod, ptrBuffer);
        CleanupStack::PopAndDestroy(dynamicText);

        // Show listbox item
        iListboxItemArray->SetItemVisibilityL(EGSInputpenIdChineseFindMethod,
                CGSItemTextArray::EVisible);
        }
    else
        {
        // Hide listbox item
        iListboxItemArray->SetItemVisibilityL(EGSInputpenIdChineseFindMethod,
                CGSItemTextArray::EInvisible);
        }
    }
    
// ---------------------------------------------------------
// Create listbox hwr trui item
// ---------------------------------------------------------
//
void CGSPenInputContainer::MakeHwrTruiItemL()
    {
    // And add to listbox
    TBool bVisible = iModel->IsSettingItemAvailable(EGSInputpenIdHwrTraining);       
    iListboxItemArray->SetItemVisibilityL(
            EGSInputpenIdHwrTraining, 
            bVisible ? CGSItemTextArray::EVisible : CGSItemTextArray::EInvisible);
    }    

// ---------------------------------------------------------
// Get model instance
// ---------------------------------------------------------
//
CGSPenInputModel* CGSPenInputContainer::Model()
    {
    return iModel;     
    }
    
// ---------------------------------------------------------
// Create a icon for pen trail colour item
// ---------------------------------------------------------
//
void CGSPenInputContainer::CreateIconL()
    {
    CGulIcon* icon = CreateNewIconL();
    
    delete iIconArray->At(0);
    iIconArray->At(0) = icon;
    }
 
// ---------------------------------------------------------
// Create a new icon
// ---------------------------------------------------------
//
CGulIcon* CGSPenInputContainer::CreateNewIconL()
    {
    CGulIcon* icon = CGulIcon::NewLC();
    
    TRect origin;
    
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, origin);
    
    TAknLayoutRect layoutRect;
    
    layoutRect.LayoutRect(origin, 
                          AknLayoutScalable_Avkon::listscroll_gen_pane(0));
    layoutRect.LayoutRect(layoutRect.Rect(), 
                          AknLayoutScalable_Avkon::list_gen_pane(0));
    layoutRect.LayoutRect(layoutRect.Rect(), 
                           AknLayoutScalable_Avkon::list_setting_number_pane(0));
                           
    TRect listSettingPaneRect(layoutRect.Rect());
    
    layoutRect.LayoutRect(layoutRect.Rect(), 
                          AknLayoutScalable_Avkon::set_value_pane(0));
                          
    TRect setValuePaneRect(layoutRect.Rect());
    TRect rect(setValuePaneRect);

    // Move to 0,0
    rect.Move(-rect.iTl.iX, -rect.iTl.iY);
    
    DrawIconL(rect, icon);
    
    CleanupStack::Pop(icon); // icon - not owned anymore, do not destroy
    
    return icon; // icon owned by listbox
    }

// ---------------------------------------------------------
// Draw icon bitmap 
// ---------------------------------------------------------
//
void CGSPenInputContainer::DrawIconL(const TRect& aRect, CGulIcon* aIcon)
    {
    CFbsBitmap* bitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(bitmap);
    
    bitmap->Create(aRect.Size(), CCoeEnv::Static()->ScreenDevice()->DisplayMode());
    
    CFbsBitGc* fbsBitGc = CFbsBitGc::NewL();
    CleanupStack::PushL(fbsBitGc);
    
    CFbsBitmapDevice* bmpDevice = CFbsBitmapDevice::NewL(bitmap);
    CleanupStack::PushL(bmpDevice);
    
    fbsBitGc->Activate(bmpDevice);
    
    TRect outerRect;
    TRect innerRect;
    TRect origin;
    
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, origin);
    
    TAknLayoutRect layoutRect;
    
    layoutRect.LayoutRect(origin, 
                          AknLayoutScalable_Avkon::listscroll_gen_pane(0));
    layoutRect.LayoutRect(layoutRect.Rect(), 
                          AknLayoutScalable_Avkon::list_gen_pane(0));
    layoutRect.LayoutRect(layoutRect.Rect(), 
                          AknLayoutScalable_Avkon::list_setting_number_pane(0));
    TRect listSettingPaneRect(layoutRect.Rect());

    // Background skinning
    TAknLayoutRect topLeft;
    TAknLayoutRect bottomRight;

    topLeft.LayoutRect(listSettingPaneRect, 
                       Setting_list_item_skin_elements__value_background__Line_2());
    bottomRight.LayoutRect(listSettingPaneRect, 
                           Setting_list_item_skin_elements__value_background__Line_5());

    // Move to 0,0
    TRect topLeftRect = topLeft.Rect();
    TRect bottomRightRect = bottomRight.Rect();

    TPoint skinOffset(-topLeftRect.iTl.iX, -topLeftRect.iTl.iY);
    bottomRightRect.Move(skinOffset);
    topLeftRect.Move(skinOffset);

    outerRect = TRect(topLeftRect.iTl, bottomRightRect.iBr);
    innerRect = TRect(topLeftRect.iBr, bottomRightRect.iTl);

    // Frame IID for qsn_fr_set_opt_foc_xxxxx
    const TAknsItemID *frameId = &KAknsIIDQsnFrSetOptFoc;

    // Make a control context:
    CAknsFrameBackgroundControlContext* cc = CAknsFrameBackgroundControlContext::NewL(
        *frameId, outerRect, innerRect, EFalse);
    CleanupStack::PushL(cc);

    TPoint dstPos(0,0);

    AknsDrawUtils::DrawBackground(AknsUtils::SkinInstance(), cc, NULL, *fbsBitGc,
                                  dstPos, outerRect, KAknsDrawParamDefault);

    fbsBitGc->SetPenColor(TRgb(iModel->TrailColour()));
    fbsBitGc->SetPenStyle(CGraphicsContext::ESolidPen);
    fbsBitGc->SetBrushColor(iModel->TrailColour());
    fbsBitGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
   

    TInt clolorWidth = outerRect.Width() / 3;
    TInt clolorHight = (outerRect.Height() * 2) / 3;
    
    dstPos.iX = (outerRect.Width() / 2) - (clolorWidth / 2);
    dstPos.iY = (outerRect.Height() / 2) - (clolorHight / 2);
    
    fbsBitGc->DrawRect(TRect(dstPos.iX,
                             dstPos.iY,
                             dstPos.iX + clolorWidth, 
                             dstPos.iY + clolorHight));
 
    CleanupStack::PopAndDestroy(cc);
    CleanupStack::PopAndDestroy(bmpDevice); 
    CleanupStack::PopAndDestroy(fbsBitGc); 
    
    aIcon->SetBitmap(bitmap); // Transfers ownership, so all we need to do is to..
    CleanupStack::Pop(bitmap);  
    
    // For mask    
    CFbsBitmap* mask = new (ELeave) CFbsBitmap;
    CleanupStack::PushL( mask );
    
    mask->Create(aRect.Size(), EGray256);
    fbsBitGc = CFbsBitGc::NewL();
    CleanupStack::PushL(fbsBitGc);
    bmpDevice = CFbsBitmapDevice::NewL(mask);
    CleanupStack::PushL(bmpDevice);
    fbsBitGc->Activate(bmpDevice);
    fbsBitGc->SetPenStyle(CGraphicsContext::ENullPen);
    fbsBitGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
    fbsBitGc->SetBrushColor(KRgbWhite);
    fbsBitGc->DrawRect(TRect(aRect.Size()));
    
    AknsDrawUtils::DrawFrame(AknsUtils::SkinInstance(), *fbsBitGc, outerRect,
                             innerRect, KAknsIIDQsnFrSetOptFoc, 
                             KAknsIIDQsnFrSetOptFocCenter, KAknsSDMAlphaOnly);
    
    CleanupStack::PopAndDestroy(bmpDevice); 
    CleanupStack::PopAndDestroy(fbsBitGc); 
    aIcon->SetMask(mask); // mask owned by listbox
    CleanupStack::Pop(mask);   
    }
    
// ---------------------------------------------------------
// Display input language setting page
// ---------------------------------------------------------
//
void CGSPenInputContainer::ShowInputLanguagePageL()
    {
    CDesCArrayFlat* languageItems = new(ELeave)CDesCArrayFlat(10);
    CleanupStack::PushL(languageItems);
    
    iModel->GetLanguagesFromPtiL(EFalse, languageItems);

    TInt currentItem = iModel->InputLanguageIndex();

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                      R_GS_INPUTLANGUAGE_TEXT_SETTING_PAGE, 
                                      currentItem,
                                      languageItems);

    CleanupStack::PushL(dlg);
    
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if (currentItem != iModel->InputLanguageIndex())
            {
            iModel->SetInputLanguageIndex(currentItem);
            iModel->SetInputLanguageL();
            UpdateListBoxL(EGSInputpenIdInputLanguage);
            if (iModel->ChineseLanguage() && iChinese)
                {
                iModel->ResetFepDefaultInputMode();
                UpdateListBoxL(EGSDefaultChineseOnscreenKeyboard);
                UpdateListBoxL(EGSInputpenIdChineseFindMethod);
                }
            }

        if ((iChinese != iModel->ChineseLanguage()) ||
            (iJapanese !=  iModel->JapaneseLanguage()))
            {
            ReConstructListBoxL();    
            }
        }

    CleanupStack::Pop(dlg);
    CleanupStack::PopAndDestroy(languageItems);   
    }

// ---------------------------------------------------------
// Display input mode setting page
// ---------------------------------------------------------
//
void CGSPenInputContainer::ShowInputModePageL()
    {
    delete iInputModeItems;
    iInputModeItems = NULL;
    
    if (iModel->InputModeResourceID() != -1)
        {
        iInputModeItems = iCoeEnv->ReadDesC16ArrayResourceL(
                                   iModel->InputModeResourceID());
        iModel->ResetInputModeArray(iInputModeItems);	
        }
    else
        {
        return;
        }    
    
    TInt currentItem = iModel->InputModeInSettingPage();

    if ( currentItem < iInputModeItems->Count() )
        {
        CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                          R_GS_VKB_DEFAULTCHINESE_OSK_TEXT_SETTING_PAGE, 
                                          currentItem, 
                                          iInputModeItems);

        CleanupStack::PushL(dlg);
        
        if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
            {
            if(currentItem != iModel->InputModeInSettingPage())
                {
                iModel->SetInputModeBySettingPage(currentItem);
                UpdateListBoxL(EGSDefaultChineseOnscreenKeyboard);
                }        
            }
        CleanupStack::Pop(dlg);        
        }        
    }
    
// ---------------------------------------------------------
// Display writing speed setting page
// ---------------------------------------------------------
//
void CGSPenInputContainer::ShowWritingSpeedPageL()
    {
    TInt currentItem = iModel->WritingSpeed();

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                      R_GS_WRITINGSPEED_TEXT_SETTING_PAGE, 
                                      currentItem, 
                                      iWritingSpeedItems);

    CleanupStack::PushL(dlg);
    
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem != iModel->WritingSpeed())
            {
            iModel->SetWritingSpeed(currentItem);
            UpdateListBoxL(EGSInputpenIdWritingSpeed);
            }
        } 
        
    CleanupStack::Pop(dlg);        
    }

// ---------------------------------------------------------
// Display pen trail width setting page
// ---------------------------------------------------------
//
void CGSPenInputContainer::ShowTrailWidthPageL()
    {
    TInt currentItem = iModel->TrailWidth();

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                      R_GS_TRAILWIDTH_TEXT_SETTING_PAGE, 
                                      currentItem, 
                                      iPenTrailWidthItems);

    CleanupStack::PushL(dlg);
    
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem != iModel->TrailWidth())
            {
            iModel->SetTrailWidth(currentItem);
            UpdateListBoxL(EGSInputpenIdPenTrailWidth);
            }     
        }
    
    CleanupStack::Pop(dlg);          
    }

// ---------------------------------------------------------
// Display guide line setting page
// ---------------------------------------------------------
//
void CGSPenInputContainer::ShowGuideLinePageL()
    {
    TInt currentItem = iModel->GuideLine();
    
    if (currentItem == 1)
        {
        currentItem = 0;
        }
    else
        {
        currentItem = 1;
        }

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                      R_GS_GUIDLINE_TEXT_SETTING_PAGE, 
                                      currentItem, 
                                      iGuideLineItems);

    CleanupStack::PushL(dlg);
    
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem == iModel->GuideLine())
            {
            iModel->SetGuideLine(currentItem == 0? 1:0);
            UpdateListBoxL(EGSInputpenIdGuidLine);
            }
        }
 
    CleanupStack::Pop(dlg);  
    }

// ---------------------------------------------------------
// Display recognition with dictionary setting page
// ---------------------------------------------------------
//
void CGSPenInputContainer::ShowRecognitionWithDictionaryL()
    {
    TInt currentItem = iModel->RecognitionWithDictionary();
    
    if (currentItem == 1)
        {
        currentItem = 0;
        }
    else
        {
        currentItem = 1;
        }

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
            R_GS_RECOGNITIONWITHDICTIONARY_TEXT_SETTING_PAGE,  
                                      currentItem, 
                                      iRecognitionWithDictionaryItems);

    CleanupStack::PushL(dlg);
    
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem == iModel->RecognitionWithDictionary())
            {
            iModel->SetRecognitionWithDictionary(currentItem == 0? 1:0);
            UpdateListBoxL(EGSInputpenIdRecognitionWithDictionary);
            }
        }
 
    CleanupStack::Pop(dlg);  
    }

// ---------------------------------------------------------
// Display input method for find setting page
// ---------------------------------------------------------
//
void CGSPenInputContainer::ShowInputMethodForFindPageL()
    {
    TInt currentItem = iModel->InputMethodForFind();

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                      R_GS_INPUTMETHODFORFIND_TEXT_SETTING_PAGE, 
                                      currentItem, 
                                      iInputMethodForFindItems);

    CleanupStack::PushL(dlg);
    
    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem != iModel->InputMethodForFind())
            {
            iModel->SetInputMethodForFind(currentItem);
            UpdateListBoxL(EGSInputpenIdInputMethodForFind);
            }
        }
 
    CleanupStack::Pop(dlg);
    }

// ---------------------------------------------------------
// Display chinese find method setting page
// ---------------------------------------------------------
//
void CGSPenInputContainer::ShowChineseFindMethodPageL()
    {
    TInt currentItem = iModel->ChineseFindMethod();

    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage(
                                      R_GS_CHINESEFINDMETHOD_TEXT_SETTING_PAGE,
                                      currentItem,
                                      iChineseFindMethodItems);

    CleanupStack::PushL(dlg);

    if (dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged))
        {
        if(currentItem != iModel->ChineseFindMethod())
            {
            iModel->SetChineseFindMethod(currentItem);
            UpdateListBoxL(EGSInputpenIdChineseFindMethod);
            }
        }

    CleanupStack::Pop(dlg);
    }

// ---------------------------------------------------------
// Display pen trail colour setting page
// ---------------------------------------------------------
//
void CGSPenInputContainer::ShowTrailColourPageL()
    {
    CArrayFixFlat<TRgb>* colours = new (ELeave) CArrayFixFlat<TRgb>(8);
    CleanupStack::PushL(colours);

    for(TInt i = 0; i < AknPenInputTrailColorUtils::ColorCount(); i++)
        {
        colours->AppendL(AknPenInputTrailColorUtils::ColorAt(i));
        }
//    colours->AppendL(KRgbBlack);
//    colours->AppendL(KRgbDarkGray);
//    colours->AppendL(KRgbDarkRed);
//    colours->AppendL(KRgbDarkGreen);
//    colours->AppendL(KRgbDarkYellow);
//    colours->AppendL(KRgbDarkBlue);
//    colours->AppendL(KRgbDarkMagenta);
//    colours->AppendL(KRgbDarkCyan);
//    colours->AppendL(KRgbRed);
//    colours->AppendL(KRgbGreen);
//    colours->AppendL(KRgbYellow);
//    colours->AppendL(KRgbBlue);
//    colours->AppendL(KRgbMagenta);
//    colours->AppendL(KRgbCyan);
//    colours->AppendL(KRgbGray);
//    colours->AppendL(KRgbWhite);

    TBool noneChosen = EFalse; 
    TRgb colourChosen = TRgb(iModel->TrailColour());
                            
    CAknColourSelectionGrid* dlg = CAknColourSelectionGrid::NewL(colours, 
                                                                 EFalse, 
                                                                 noneChosen, 
                                                                 colourChosen);
    
    CleanupStack::PushL(dlg);
    
    if (dlg->ExecuteLD())
        {
        iModel->SetTrailColour(colourChosen.Value());
        UpdateListBoxL(EGSInputpenIdPenTrailColour);
        }
    
    CleanupStack::Pop(dlg);         
    CleanupStack::PopAndDestroy(colours);
    }

// ---------------------------------------------------------
// Display touch screen calibration setting page
// ---------------------------------------------------------
//
void CGSPenInputContainer::StartTouchScreenCalibL()
    {
    _LIT( KTouchScreenCalibFileName, "z:TouchScreenCalib.exe" );
    TInt err = KErrNone;
 
    TParse parse;
    parse.Set( KTouchScreenCalibFileName, 
                   &KDC_PROGRAMS_DIR, NULL );
    TFileName fileName( parse.FullName() );
 
    RProcess process;
    err = process.Create( fileName, KNullDesC );
    User::LeaveIfError(err);
 
    CApaCommandLine* commandLine = CApaCommandLine::NewLC();
    commandLine->SetDocumentNameL( KNullDesC );
    commandLine->SetExecutableNameL( fileName );
    commandLine->SetProcessEnvironmentL( process );
    CleanupStack::PopAndDestroy( commandLine );
 
    process.Resume();
    process.Close();
    }
    
// ---------------------------------------------------------
// Re-Construct List box
// ---------------------------------------------------------
//
void CGSPenInputContainer::ReConstructListBoxL() 
    {
    iChinese = iModel->ChineseLanguage();
    iJapanese = iModel->JapaneseLanguage();        

    delete iListboxItemArray;
    iListboxItemArray = NULL;
    
    iListboxItemArray = CGSListBoxItemTextArray::NewL(R_INPUTPEN_LBX,
                                                      *iListBox,
                                                      *iCoeEnv);
    iListBox->Model()->SetItemTextArray(iListboxItemArray);
    iListBox->Model()->SetOwnershipType(ELbmDoesNotOwnItemArray);
    
    if (!iIconArray)
        {
        CArrayPtr<CGulIcon>* icons = new (ELeave) CArrayPtrFlat<CGulIcon>(2);
        CleanupStack::PushL(icons);
        CGulIcon* icon = CGulIcon::NewL();
        CleanupStack::PushL(icon);
    
        icons->AppendL(icon);

        CEikFormattedCellListBox* listbox = static_cast<CEikFormattedCellListBox*>(iListBox);
    
        listbox->ItemDrawer()->FormattedCellData()->SetIconArray(icons);// icons owned by List box
    
        iIconArray = listbox->ItemDrawer()->FormattedCellData()->IconArray();
    
        CleanupStack::Pop(icon);
        CleanupStack::Pop(icons);
    }

    CreateListBoxItemsL();
    iListBox->HandleItemAdditionL();
    }
    

// ---------------------------------------------------------
//  Launch other application;
// ---------------------------------------------------------
//
void CGSPenInputContainer::LaunchAppL( TUid aAppUid )
    {
    // Find the task with uid
    TApaTaskList taskList( iEikonEnv->WsSession() );    
    TApaTask task = taskList.FindApp( aAppUid );

    if ( task.Exists() )
        {
        task.BringToForeground();
        }
    else
        {
        // Launch peninputsetting application as embedded.
        if ( iNullService )
            {
            delete iNullService;
            iNullService = NULL;
            }
        iNullService = CAknNullService::NewL( aAppUid, NULL );
        }    
    }

// ---------------------------------------------------------
//  Identify the help context so that the framework can look up
//  the corresponding help topic
// ---------------------------------------------------------
//
void CGSPenInputContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {
    aContext.iMajor = KUidGS;
    aContext.iContext = HLP_GS_PEN_INPUT; // temp fix for helps integration build break;
    }

// End Of File
