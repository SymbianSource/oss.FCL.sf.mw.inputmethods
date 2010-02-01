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
* Description:  dialog for gspeninputimple
*
*/


#include <e32base.h>
#include <aknsettingitemlist.h>
#include <AknColourSelectionGrid.h>
#include <aknradiobuttonsettingpage.h>
#include <gdi.h>
#include <AknUtils.h>
#include <avkon.rsg>
#include <avkon.hrh>
#include <gspeninputpluginrsc.rsg>
#include <StringLoader.h>
#include <gscommon.hrh>
#include <aknlayoutscalable_avkon.cdl.h>
#include <avkon.mbg>
#include <AknsDrawUtils.h>
#include <AknsFrameBackgroundControlContext.h>
#include <skinlayout.cdl.h>
#include <aknconsts.h>
#include <aknslider.h>
#include <settingsinternalcrkeys.h>
#include <AknFepInternalCRKeys.h>
#include <aknappui.h>
#include <hlplch.h> // HlpLauncher
#include <peninputpluginutils.h>
//#include <csxhelp\cp.hlp.hrh>
#include <gsfwviewuids.h>
#include <AknNullService.h>

using namespace SkinLayout;

#include "gspeninputplugin.hrh"
#include "gssettingdialog.h"
#include "gspeninputimple.h"
#include "gspeninputmodel.h"

_LIT(KSingleSpaceText, " ");

const TUid KUidCalibrationApp = { 0x102828BC };


// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSColorSettingItem::CGSColorSettingItem(const TInt aIdentifier, 
                                               CGSPenInputImple* aImple)
    :CAknSettingItem(aIdentifier), iImple(aImple)
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CGSColorSettingItem::~CGSColorSettingItem()
    {
    }

// ---------------------------------------------------------
// This launches the setting page
// ---------------------------------------------------------
//
void CGSColorSettingItem::EditItemL(TBool /*aCalledFromMenu*/)
    {
    if (iImple->ShowTrailColourPageL())
        {
        UpdateListBoxTextL();
        }
    }

// ---------------------------------------------------------
// This method obtains the current value's setting text
// ---------------------------------------------------------
//
const TDesC& CGSColorSettingItem::SettingTextL()
    {
    return KSingleSpaceText;
    }
    
// ---------------------------------------------------------
// Method to obtain a new bitmap for the setting content
// ---------------------------------------------------------
//
CFbsBitmap* CGSColorSettingItem::CreateBitmapL()
    {
    TRect origin;
    
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, origin);
    
    TAknLayoutRect layoutRect;
    
    layoutRect.LayoutRect(origin, 
                          AknLayoutScalable_Avkon::listscroll_gen_pane(0));
    layoutRect.LayoutRect(layoutRect.Rect(), 
                          AknLayoutScalable_Avkon::list_gen_pane(0));
    layoutRect.LayoutRect(layoutRect.Rect(), 
                          AknLayoutScalable_Avkon::list_setting_number_pane(0));
    layoutRect.LayoutRect(layoutRect.Rect(), 
                          AknLayoutScalable_Avkon::set_value_pane(0));
                        
    TRect mainRect(layoutRect.Rect());

    // Move to 0,0
    mainRect.Move(-mainRect.iTl.iX, -mainRect.iTl.iY);  
    
    CFbsBitmap* bitmap = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(bitmap);
    
    bitmap->Create(mainRect.Size(), CCoeEnv::Static()->ScreenDevice()->DisplayMode());

    CFbsBitGc* fbsBitGc = CFbsBitGc::NewL();
    CleanupStack::PushL(fbsBitGc);

    CFbsBitmapDevice* bmpDevice = CFbsBitmapDevice::NewL(bitmap);
    CleanupStack::PushL(bmpDevice);

    fbsBitGc->Activate(bmpDevice);
    
    TRect outerRect;
    TRect innerRect;
    TAknLayoutRect backGrandRect;
    
    backGrandRect.LayoutRect(origin, 
                             AknLayoutScalable_Avkon::listscroll_gen_pane(0));
    backGrandRect.LayoutRect(backGrandRect.Rect(), 
                             AknLayoutScalable_Avkon::list_gen_pane(0));
    backGrandRect.LayoutRect(backGrandRect.Rect(), 
                             AknLayoutScalable_Avkon::list_setting_number_pane(0));
                             
    TRect listSettingPaneRect(backGrandRect.Rect());

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

    TPoint dstPos(0, 0);

    AknsDrawUtils::DrawBackground(AknsUtils::SkinInstance(), 
                                  cc, NULL, *fbsBitGc, dstPos, 
                                  outerRect, KAknsDrawParamDefault);

    fbsBitGc->SetPenColor(TRgb(iImple->Model()->TrailColour()));
    fbsBitGc->SetPenStyle(CGraphicsContext::ESolidPen);
    fbsBitGc->SetBrushColor(iImple->Model()->TrailColour());
    fbsBitGc->SetBrushStyle(CGraphicsContext::ESolidBrush);

    const TInt clolorWidth = outerRect.Width() / 3; 
    const TInt clolorHight = (outerRect.Height() * 2) / 3;  
    
    dstPos.iX = (outerRect.Width() / 2) - (clolorWidth / 2);
    dstPos.iY = (outerRect.Height() / 2) - (clolorHight / 2);
    
    fbsBitGc->DrawRect(TRect(dstPos.iX,
                             dstPos.iY,
                             dstPos.iX + clolorWidth,
                             dstPos.iY + clolorHight));
 
    CleanupStack::PopAndDestroy(cc);
    CleanupStack::PopAndDestroy(bmpDevice); 
    CleanupStack::PopAndDestroy(fbsBitGc); 
    CleanupStack::Pop(bitmap); 
    
    CreateMaskL();

    return bitmap;
    }

// ---------------------------------------------------------
// Create a mask for item mask
// ---------------------------------------------------------
//
void CGSColorSettingItem::CreateMaskL()
    {
    TRect origin;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, origin);
    
    TAknLayoutRect layoutRect;
    layoutRect.LayoutRect(origin, 
                          AknLayoutScalable_Avkon::listscroll_gen_pane(0));
    layoutRect.LayoutRect(layoutRect.Rect(), 
                          AknLayoutScalable_Avkon::list_gen_pane(0));
    layoutRect.LayoutRect(layoutRect.Rect(), 
                          AknLayoutScalable_Avkon::list_setting_number_pane(0));
    layoutRect.LayoutRect(layoutRect.Rect(), 
                          AknLayoutScalable_Avkon::set_value_pane(0));
    TRect mainRect(layoutRect.Rect());

    // Move to 0,0
    mainRect.Move(-mainRect.iTl.iX,-mainRect.iTl.iY);     
    
    TRect outerRect;
    TRect innerRect;
     
    TAknLayoutRect backGrandRect;
    backGrandRect.LayoutRect(origin, 
                             AknLayoutScalable_Avkon::listscroll_gen_pane(0));
    backGrandRect.LayoutRect(backGrandRect.Rect(), 
                             AknLayoutScalable_Avkon::list_gen_pane(0));
    backGrandRect.LayoutRect(backGrandRect.Rect(), 
                             AknLayoutScalable_Avkon::list_setting_number_pane(0));
    TRect listSettingPaneRect(backGrandRect.Rect());


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
    
    CFbsBitmap* mask = new (ELeave) CFbsBitmap;
    CleanupStack::PushL(mask);
        
    mask->Create(mainRect.Size(), EGray256);
    CFbsBitGc* fbsBitGc = CFbsBitGc::NewL();
    CleanupStack::PushL(fbsBitGc);
    CFbsBitmapDevice* bmpDevice = CFbsBitmapDevice::NewL(mask);
    CleanupStack::PushL(bmpDevice);
    fbsBitGc->Activate(bmpDevice);
    fbsBitGc->SetPenStyle(CGraphicsContext::ENullPen);
    fbsBitGc->SetBrushStyle(CGraphicsContext::ESolidBrush);
    fbsBitGc->SetBrushColor(KRgbWhite);
    fbsBitGc->DrawRect(TRect(mainRect.Size()));
    
    AknsDrawUtils::DrawFrame(AknsUtils::SkinInstance(), *fbsBitGc, outerRect,
                             innerRect, KAknsIIDQsnFrSetOptFoc, 
                             KAknsIIDQsnFrSetOptFocCenter, KAknsSDMAlphaOnly);    
    
    CleanupStack::PopAndDestroy(bmpDevice); 
    CleanupStack::PopAndDestroy(fbsBitGc); 

    if (SetIconMask(mask))
        {
        CleanupStack::Pop(mask);    
        }
    else
        {
        CleanupStack::PopAndDestroy(mask);
        }
    }
    
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSTextSettingItem::CGSTextSettingItem(TInt aItemResourceId,
                                             TDes &aText,
                                             CGSPenInputImple* aImple,
                                             CGSSettingItemList* aParent)
    :CAknTextSettingItem(aItemResourceId, aText), 
     iImple(aImple), 
     iItemId(aItemResourceId),
     iSettingList(aParent)
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CGSTextSettingItem::~CGSTextSettingItem()
    {
    iInputChineseFindMethod.Close();
    }

// ---------------------------------------------------------
// This method obtains the current value's setting text
// ---------------------------------------------------------
//
const TDesC& CGSTextSettingItem::SettingTextL()
    {
    switch ( iItemId )
        {
        /*case EGSInputpenIdHwrTraining:
            {
            TBuf<10> buf;
            buf.Append(_L("trui"));
            return buf;
            }*/
        case EGSInputpenIdInputLanguage:
            {
            iImple->Model()->InputLanguage(iInputLanguage);
            return iInputLanguage;
            }
        case EGSDefaultChineseOnscreenKeyboard:
            {
            iImple->InputModeL(iInputMode);
            return iInputMode;
            }
        case EGSInputpenIdWritingSpeed:
            {
            iImple->WritingSpeed(iWritingSpeed);
            return iWritingSpeed;
            }
        case EGSInputpenIdGuidLine:
            {
            iImple->GuideLine(iGuideLine);
            return iGuideLine;
            }
        case EGSInputpenIdRecognitionWithDictionary:
            {
            iImple->RecognitionWithDictionary(iRecognitionWithDictionary);
            return iRecognitionWithDictionary;
            }             
        case EGSInputpenIdPenTrailWidth:
            {
            iImple->TrailWidth(iTrailWidth);
            return iTrailWidth;
            }
        case EGSInputpenIdInputMethodForFind:
           {
           iImple->InputMethodForFind(iInputMethodForFind);
           return iInputMethodForFind;
           }    
        case EGSInputpenIdChineseFindMethod:
           {
           if ( !iInputChineseFindMethod.MaxLength() )
               {
               iInputChineseFindMethod.CreateL( KItemTextSize );
               }           
           iImple->GetCurrentChineseFineMethodItem(iInputChineseFindMethod);
           return iInputChineseFindMethod;
           }    
        }
           
    return KSingleSpaceText;
    }

// ---------------------------------------------------------
// This launches the setting page
// ---------------------------------------------------------
//
void CGSTextSettingItem::EditItemL(TBool /*aCalledFromMenu*/)
    {
    switch(iItemId)
        {
        case EGSInputpenIdInputLanguage:
            {
            if (iImple->ShowInputLanguageSettingPageL())
                {
                //iSettingList->ResetSettingListL();                
                UpdateListBoxTextL();
                }
            }
            break;
        case EGSDefaultChineseOnscreenKeyboard:
            {
            if (iImple->ShowInputModePageL())
                {
                UpdateListBoxTextL();    
                }
            }
            break;
        case EGSInputpenIdWritingSpeed:
            {
            if (iImple->ShowWritingSpeedPageL())
                {
                UpdateListBoxTextL();    
                }
            }
            break;
        case EGSInputpenIdGuidLine:
            {
            if (iImple->ShowGuideLinePageL())
                {
                UpdateListBoxTextL();    
                }
            }
            break;            
        case EGSInputpenIdRecognitionWithDictionary:
            {
            if (iImple->ShowRecognitionWithDictionaryL())
                {
                UpdateListBoxTextL();    
                }
            }
            break;            
        case EGSInputpenIdPenTrailWidth:
            {
            if (iImple->ShowTrailWidthPageL())
                {
                UpdateListBoxTextL();          
                }
            }
            break;
         case EGSInputpenIdInputMethodForFind:
            {
            if (iImple->ShowInputMethodForFindPageL())
                {
                UpdateListBoxTextL();          
                }
            }
            break;    
         case EGSInputpenIdChineseFindMethod:
            {
            if (iImple->ShowChineseFindMethodPageL())
                {
                UpdateListBoxTextL();          
                }
            }
            break;    
        default:
            break;
        }
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSSettingItemList::CGSSettingItemList(CGSPenInputImple* aImple)
    :CAknSettingItemList(), iImple(aImple)
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CGSSettingItemList::~CGSSettingItemList()
    {
    iInputChineseFindMethod.Close();
    }

// ---------------------------------------------------------
// Reset Setting List
// ---------------------------------------------------------
//
void CGSSettingItemList::ResetSettingListL()
    {
    CAknSettingItemArray* listArray = SettingItemArray();
    for( TInt i = EGSInputpenIdHwrTraining; i <= EGSInputpenIdTouchScrCalib; ++i )
        {
        CAknSettingItem* item = listArray->At(i);
        item->SetHidden(!(iImple->Model()->IsSettingItemAvailable(i) ));
        }
    ListBox()->SetCurrentItemIndex(0);
    //HandleChangeInItemArrayOrVisibilityL();
    }

// ---------------------------------------------------------
// Handle listbox events
// ---------------------------------------------------------
//
void CGSSettingItemList::HandleListBoxEventL( CEikListBox* aListBox, TListBoxEvent aEventType )
    {
            
    if ( aEventType == EEventEnterKeyPressed ||
    	aEventType == EEventItemDoubleClicked )
        {
        
        TInt index = aListBox->CurrentItemIndex();
        TInt itemID = SettingItemArray()->ItemIndexFromVisibleIndex(index);
        switch ( itemID )
            {
            case EGSInputpenIdTouchScrCalib:
                {
                iImple->StartTouchScreenCalibL(); 
                return;
                }
            case EGSInputpenIdHwrTraining:
                {
                _LIT(KPeninputHwrTrainingUI, "z:\\sys\\bin\\trui.exe" );
                AknPenInputUtils::StartAppL( KPeninputHwrTrainingUI );
                return;
                }
            }
        }
                     
    CAknSettingItemList::HandleListBoxEventL( aListBox, aEventType );   
    }
    
// ---------------------------------------------------------
// Framework method to create a setting item based upon the user id aSettingId
// ---------------------------------------------------------
//
CAknSettingItem* CGSSettingItemList::CreateSettingItemL(TInt identifier)
    {
    CAknSettingItem* settingItem = NULL;

    switch (identifier)
    {
    case EGSInputpenIdHwrTraining:
        {
        settingItem = new (ELeave) CAknBigSettingItemBase( EGSInputpenIdHwrTraining);
        }
        break;
    case EGSInputpenIdInputLanguage:
        {
        iImple->Model()->InputLanguage(iInputLanguage);
        settingItem = new (ELeave) CGSTextSettingItem(EGSInputpenIdInputLanguage,
                                                      iInputLanguage, 
                                                      iImple,
                                                      this);
        }
        break;
    case EGSDefaultChineseOnscreenKeyboard:
        {
            iImple->InputModeL(iInputMode);
            settingItem = new (ELeave) CGSTextSettingItem(
                                       EGSDefaultChineseOnscreenKeyboard,
                                       iInputMode, iImple, this);
        }
        break;
    case EGSInputpenIdWritingSpeed:
        {
        iImple->WritingSpeed(iWritingSpeed);
        settingItem = new (ELeave) CGSTextSettingItem(EGSInputpenIdWritingSpeed,
                                                      iWritingSpeed, 
                                                      iImple,
                                                      this);
        }
        break;
    case EGSInputpenIdGuidLine:
        {
            iImple->GuideLine(iGuideLine);
            settingItem = new (ELeave) CGSTextSettingItem(EGSInputpenIdGuidLine,
                                                          iGuideLine, 
                                                          iImple,
                                                          this);
        }
        break;
    case EGSInputpenIdRecognitionWithDictionary:
        {
            iImple->RecognitionWithDictionary(iRecognitionWithDictionary);
            settingItem = new (ELeave) CGSTextSettingItem(EGSInputpenIdRecognitionWithDictionary,
                                                          iRecognitionWithDictionary, 
                                                          iImple,
                                                          this);
        }
        break;        
    case EGSInputpenIdPenTrailWidth:
        {
        iImple->TrailWidth(iTrailWidth);
        settingItem = new (ELeave) CGSTextSettingItem(EGSInputpenIdPenTrailWidth,
                                                      iTrailWidth, 
                                                      iImple,
                                                      this);
        }
        break;
    case EGSInputpenIdPenTrailColour:
        {
        settingItem = new (ELeave) CGSColorSettingItem(EGSInputpenIdPenTrailColour, 
                                                       iImple);
        }
        break; 
    case EGSInputpenIdInputMethodForFind:
        {
        iImple->InputMethodForFind(iInputMethodForFind);
        settingItem = new (ELeave) CGSTextSettingItem(EGSInputpenIdInputMethodForFind, 
                                                      iInputMethodForFind, 
                                                      iImple,
                                                      this);
        }
        break;    
    case EGSInputpenIdChineseFindMethod:
        {        
        iImple->LoadChineseFindMethodItemsL();
        if ( !iInputChineseFindMethod.MaxLength() )
           {
           iInputChineseFindMethod.CreateL( KItemTextSize );
           }
        iImple->GetCurrentChineseFineMethodItem( iInputChineseFindMethod );
        iInputChineseFindMethod.Copy( KNullDesC );
        settingItem = new (ELeave) CGSTextSettingItem(EGSInputpenIdChineseFindMethod, 
                                                      iInputChineseFindMethod, 
                                                      iImple,
                                                      this);
        }
        break;    
    case EGSInputpenIdTouchScrCalib:
        {
        settingItem = new (ELeave) CAknBigSettingItemBase( EGSInputpenIdTouchScrCalib );
        }
    default:
        break;
        }

    if( settingItem  )
        {
        settingItem->SetHidden(
                    !(iImple->Model()->IsSettingItemAvailable(identifier) ));
        }

    return settingItem;
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
CGSSettingDialog::CGSSettingDialog(CGSPenInputImple* aImple)
    : iImple(aImple)
    {
    }

// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
CGSSettingDialog::~CGSSettingDialog()
    {
    }
    
// ---------------------------------------------------------
// Framework method to create a control based upon control type
// ---------------------------------------------------------
//
SEikControlInfo CGSSettingDialog::CreateCustomControlL(TInt aControlType)
	{
    SEikControlInfo controlInfo;
    
    controlInfo.iControl = NULL;
    controlInfo.iTrailerTextId = 0;
    controlInfo.iFlags = 0;

    switch (aControlType)
        {
        case EGSSettingItemListBox:
            {
            iSettingList = new (ELeave) CGSSettingItemList(iImple);
            controlInfo.iControl = iSettingList;
            }
            break;
        default:
            break;
        }

    return controlInfo;
    }

// ---------------------------------------------------------
// This callback method is used to notify the client about
// key changing
// ---------------------------------------------------------
//
void CGSSettingDialog::HandleNotifyInt(TUint32 aId, TInt /*aNewValue*/)
    {
    CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
    TInt index = 0;
    switch (aId)
        {
        case KAknFepInputTxtLang:
           {
            TRAP_IGNORE(itemsArray->At(EGSInputpenIdInputLanguage)->UpdateListBoxTextL());
            //TRAP_IGNORE(iSettingList->ResetSettingListL());
            index = EGSDefaultChineseOnscreenKeyboard;
            if (iImple->Model()->ChineseLanguage())
                {
                TRAP_IGNORE(itemsArray->At(index)->UpdateListBoxTextL());    
                }
            TRAP_IGNORE( iSettingList->HandleChangeInItemArrayOrVisibilityL() );
            }
            break;
        case KSettingsWritingSpeed:
            {
            index = EGSInputpenIdWritingSpeed;
            TRAP_IGNORE(itemsArray->At(index)->UpdateListBoxTextL());    
            }
            break;
        case KSettingsPenInputGuideLine:
            {
            index = EGSInputpenIdGuidLine;
            TRAP_IGNORE(itemsArray->At(index)->UpdateListBoxTextL());    
            }
            break;
        case KSettingRecognitionwithdictionary:
            {
            TRAP_IGNORE(iSettingList->ResetSettingListL());
            index = EGSInputpenIdRecognitionWithDictionary;
            TRAP_IGNORE(itemsArray->At(index)->UpdateListBoxTextL());    
            }
            break;            
        case KSettingsPenWidth:
            {
            TRAP_IGNORE(itemsArray->At(EGSInputpenIdPenTrailWidth)->UpdateListBoxTextL());    
            }
            break;
        case KSettingsPenColor:
            {
            index = EGSInputpenIdPenTrailColour;
            TRAP_IGNORE(itemsArray->At(index)->UpdateListBoxTextL());    
            }
            break;
        case KSettingsDefaultVkbPrc:
        case KSettingsDefaultVkbTw:
        case KSettingsDefaultVkbHk:
            {
            index = EGSDefaultChineseOnscreenKeyboard;
            if (iImple->Model()->ChineseLanguage())
               {
               TRAP_IGNORE(itemsArray->At(index)->UpdateListBoxTextL());    
               }
            }
            break;
        default:
            break;
        }
    
    TRAP_IGNORE(iSettingList->HandleChangeInItemArrayOrVisibilityL());
    }

// ---------------------------------------------------------
// This callback method is used to notify the client about
// key changing
// ---------------------------------------------------------
//
void CGSSettingDialog::HandleNotifyInt
          (TUid aRepositoryUid, TUint32 aId, TInt aNewValue)
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
        iSettingList->HandleChangeInItemArrayOrVisibilityL();
        )
    }

// ---------------------------------------------------------
// Handle notifier from KCRUidPersonalizationSettings 
// in CRepository
// ---------------------------------------------------------
//
void CGSSettingDialog::HandleNotifyIntFromPersonalizationL
                         ( TUint32 aId, TInt /*aNewValue*/ )
    {
    CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
    TInt index = 0;    
    switch ( aId )
        {
        case KSettingsWritingSpeed:
            {
            index = EGSInputpenIdWritingSpeed;
            itemsArray->At(index)->UpdateListBoxTextL();
            break;
            }
        case KSettingsPenInputGuideLine:
            {
            index = EGSInputpenIdGuidLine;
            itemsArray->At(index)->UpdateListBoxTextL();
            break;
            }
        case KSettingsPenWidth:
            {
            itemsArray->At(EGSInputpenIdPenTrailWidth)->UpdateListBoxTextL();
            break;
            }
        case KSettingsPenColor:
            {
            index = EGSInputpenIdPenTrailColour;
            itemsArray->At(index)->UpdateListBoxTextL();
            break;
            }
        case KSettingsDefaultVkbPrc:
        case KSettingsDefaultVkbHk:
        case KSettingsDefaultVkbTw:
            {
            index = EGSDefaultChineseOnscreenKeyboard;
            if (iImple->Model()->ChineseLanguage())
               {
               itemsArray->At(index)->UpdateListBoxTextL();    
               }
            break;
            }
        case KSettingRecognitionwithdictionary:
            {
            index = EGSInputpenIdRecognitionWithDictionary;
            itemsArray->At(index)->UpdateListBoxTextL();
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
void CGSSettingDialog::HandleNotifyIntFromAknFepL
                         ( TUint32 aId, TInt /*aNewValue*/ )
    {
    CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
    TInt index = 0;    
    switch ( aId )
        {
        case KAknFepInputTxtLang:
            {
            itemsArray->At(EGSInputpenIdInputLanguage)->UpdateListBoxTextL();
            iSettingList->ResetSettingListL();
            if (iImple->Model()->ChineseLanguage())
                {
                // Reload default on-screen vkb
                iImple->LoadDefaultOnScreenVkbItemsL();
                index = EGSDefaultChineseOnscreenKeyboard;
                itemsArray->At(index)->UpdateListBoxTextL();
                // Reload chinese find items because language changed
                iImple->LoadChineseFindMethodItemsL();
                index = EGSInputpenIdChineseFindMethod;
                itemsArray->At(index)->UpdateListBoxTextL();
                }           
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
void CGSSettingDialog::HandleNotifyIntFromAvkonL
                         ( TUint32 aId, TInt /*aNewValue*/ )
    {
    CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
    TInt index = 0;    
    switch ( aId )
        {
        case KAknAvkonAdaptiveSearchEnabled:
            {
            index = EGSInputpenIdInputMethodForFind;
            itemsArray->At(index)->UpdateListBoxTextL();
            break;
            }
        case KAknAdaptiveSearchChinesePRC:
        case KAknAdaptiveSearchChineseTW:
        case KAknAdaptiveSearchChineseHongkong:
            {            
            index = EGSInputpenIdChineseFindMethod;
            itemsArray->At(index)->UpdateListBoxTextL();            
            break;
            }
        default:
            break;
        }    
    }

// ---------------------------------------------------------
//  Launch other application;
// ---------------------------------------------------------
//
void CGSSettingDialog::LaunchAppL( TUid aAppUid )
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
        if ( iNullService )
            {
            delete iNullService;
            iNullService = NULL;
            }
        iNullService = CAknNullService::NewL( aAppUid, NULL );
        }    
    }

// ---------------------------------------------------------
// Process menu selection event
// ---------------------------------------------------------
//
void CGSSettingDialog::ProcessCommandL(TInt aCommandId)
    {
    HideMenu();
    
    switch (aCommandId)
        {
        case EGSCmdAppChange:
            {
            ShowSettingPageL();
            }
            break;
        case EAknCmdExit:
            {
            TryExitL(EAknCmdExit);            
            }
            break;
        case EAknCmdHelp:
            {
            TCoeHelpContext context;
            context.iMajor = KUidGS;
            context.iContext = HLP_GS_PEN_INPUT;// temp fix for helps integration build break;
            CArrayFixFlat<TCoeHelpContext>* array 
                = new (ELeave) CArrayFixFlat
                                    <TCoeHelpContext>( 1 );
            CleanupStack::PushL( array );
            array->AppendL( context );
            CleanupStack::Pop( array );
            HlpLauncher::LaunchHelpApplicationL(
                    iEikonEnv->WsSession(), array );             
            }
            break;
        default:
            break;
        }
    }
    
// ---------------------------------------------------------
// Process button pressed event
// ---------------------------------------------------------
//
TBool CGSSettingDialog::OkToExitL(TInt aButtonId)
    {
    TInt ret = EFalse;
    switch (aButtonId)
        {
        case EGSMSKCmdAppChange:
            {
            ShowSettingPageL();
            }
            break;
        case EAknCmdExit:
        case EAknSoftkeyBack:
            {
            // Add to restore status pane before closing dialog
            if ( iImple )
               {
               iImple->RestoreStatusPaneVisibility();
               }
            ret = ETrue;
            }
            break;
        case EAknSoftkeyOptions:
            {
            DisplayMenuL();
            }
        default:
            break;
        }

    return ret;
    }
    
// ---------------------------------------------------------
// Show setting page when user select item in listbox
// ---------------------------------------------------------
//
void CGSSettingDialog::ShowSettingPageL()
    {
    CEikFormattedCellListBox* listbox = iSettingList->ListBox();
    const TInt index = listbox->CurrentItemIndex();  
    
    TInt itemID = iSettingList->SettingItemArray()->ItemIndexFromVisibleIndex(index);
    
    switch (itemID)
       {
       case EGSInputpenIdHwrTraining:
           {
           _LIT(KPeninputHwrTrainingUI, "z:\\sys\\bin\\trui.exe" );
           AknPenInputUtils::StartAppL( KPeninputHwrTrainingUI );           
           }
           break;
       case EGSInputpenIdInputLanguage:
           {
           iImple->ShowInputLanguagePageL();
           CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
           itemsArray->At(itemID)->UpdateListBoxTextL();
           //iSettingList->ResetSettingListL();
           //itemsArray->At(itemID + 1 )->UpdateListBoxTextL();    
           }
           break;
       case EGSDefaultChineseOnscreenKeyboard:
           {
           iImple->ShowInputModePageL();
           CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
           itemsArray->At(itemID)->UpdateListBoxTextL();
           } 
           break;           
       case EGSInputpenIdWritingSpeed:
           {
           iImple->ShowWritingSpeedPageL();
           CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
           itemsArray->At(itemID)->UpdateListBoxTextL();
           }
           break;
       case EGSInputpenIdGuidLine:
           {
           iImple->ShowGuideLinePageL();
           CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
           itemsArray->At(itemID)->UpdateListBoxTextL();
           }
           break;
       case EGSInputpenIdRecognitionWithDictionary:
           {
           iImple->ShowRecognitionWithDictionaryL();
           CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
           itemsArray->At(itemID)->UpdateListBoxTextL();
           }
           break;               
       case EGSInputpenIdPenTrailWidth:
           {
           iImple->ShowTrailWidthPageL();             
           CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
           itemsArray->At(itemID)->UpdateListBoxTextL();
           }
           break;
       case EGSInputpenIdPenTrailColour:
           {
           iImple->ShowTrailColourPageL();
           CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
           itemsArray->At(itemID)->UpdateListBoxTextL();
           }
           break;
       case EGSInputpenIdInputMethodForFind:
           {
           iImple->ShowInputMethodForFindPageL();
           CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
           itemsArray->At(itemID)->UpdateListBoxTextL();
           }
           break; 
       case EGSInputpenIdChineseFindMethod:
           {
           iImple->ShowChineseFindMethodPageL();
           CAknSettingItemArray* itemsArray = iSettingList->SettingItemArray();
           itemsArray->At(itemID)->UpdateListBoxTextL();
           }
           break; 
       case EGSInputpenIdTouchScrCalib:
           {                   
           LaunchAppL( KUidCalibrationApp );
           }
           break;
       default:
           break;
       }
    }

void CGSSettingDialog::DynInitMenuPaneL(TInt aResourceId, CEikMenuPane *aMenuPane)
    {
    if ( aResourceId != R_GS_INPUT_ITEM_CHANGE )
    	{
    	return;
    	}
    	
    CEikFormattedCellListBox* listbox = iSettingList->ListBox();
    const TInt index = listbox->CurrentItemIndex();  
    TInt itemID = iSettingList->SettingItemArray()->ItemIndexFromVisibleIndex(index);
    
    if ( itemID == EGSInputpenIdHwrTraining || itemID == EGSInputpenIdTouchScrCalib )
    	{
    	aMenuPane->SetItemTextL( EGSCmdAppChange, R_GS_INPUT_ITEM_TXT_OPEN );
    	}
    else
    	{
        aMenuPane->SetItemTextL( EGSCmdAppChange, R_GS_INPUT_ITEM_TXT_CHANGE );
    	}
    }    
// End Of File 
