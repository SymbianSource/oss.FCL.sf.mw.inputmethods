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










#include <barsread.h>
#include <AvkonInternalCRKeys.h>
#include <AknFepInternalCRKeys.h>
#include <e32property.h>
#include <PtiDefs.h>
#include <AknFepGlobalEnums.h>
#include <avkon.hrh>
#include <aknsettingitemlist.h> 
#include <aknfep.rsg>
#include "AknFepUIAvkonCtrlDualLanguageSettingList.h"


CAknFepDualLanguageSettingItem::CAknFepDualLanguageSettingItem( TInt aResourceId, TBool aPrimary, CAknFepDualLanguageSettingData* aData)
    : CAknEnumeratedTextPopupSettingItem(aResourceId, (aPrimary?aData->iCurrentValue:aData->iCurrentSecondaryValue))
      , iData(aData), iPrimary(aPrimary)
    {
    }

CAknFepDualLanguageSettingItem::~CAknFepDualLanguageSettingItem()
    {
    }

void CAknFepDualLanguageSettingItem::CompleteConstructionL()
    {
    CAknEnumeratedTextPopupSettingItem::CompleteConstructionL();
    CArrayPtrFlat<HBufC>* dummyPtr = new (ELeave) CArrayPtrFlat<HBufC>(1);
    CleanupStack::PushL(dummyPtr);
    CArrayPtrFlat<CAknEnumeratedText>* items = new (ELeave) CArrayPtrFlat<CAknEnumeratedText>(10);
    CleanupStack::PushL(items);
    if(iPrimary)
        {
        for(TInt i = 0; i < iData->Items()->Count(); i++)
            {
            HBufC* buffer = HBufC::NewLC( 128 );
            (*buffer) = (*iData->Items())[i];
            CAknEnumeratedText* newItem = new (ELeave) CAknEnumeratedText(i, buffer);
            CleanupStack::PushL(newItem);
            items->AppendL(newItem);
            CleanupStack::Pop(newItem);
            CleanupStack::Pop(buffer);
            }
        SetEnumeratedTextArrays(items, dummyPtr);
        SetInternalValue(iData->iCurrentValue);
        SetExternalValue(iData->iCurrentValue);
        }
    else
        {
        for(TInt i = 0; i < iData->SecondaryItems()->Count(); i++)
            {
            HBufC* buffer = HBufC::NewLC( 128 );
            (*buffer) = (*iData->SecondaryItems())[i];
            CAknEnumeratedText* newItem = new (ELeave) CAknEnumeratedText(i, buffer);
            CleanupStack::PushL(newItem);
            items->AppendL(newItem);
            CleanupStack::Pop(newItem);
            CleanupStack::Pop(buffer);
            }
        SetEnumeratedTextArrays(items, dummyPtr);
        SetInternalValue(iData->iCurrentSecondaryValue);
        SetExternalValue(iData->iCurrentSecondaryValue);
        }
    CleanupStack::Pop(items);
    CleanupStack::Pop(dummyPtr);
    }

CAknFepDualLanguageSettingList *CAknFepDualLanguageSettingList::NewL(CAknFepDualLanguageSettingData* aData, CAknFepDualLanguageSettingDialog* aDialog)
    {
    CAknFepDualLanguageSettingList* self = CAknFepDualLanguageSettingList::NewLC(aData, aDialog);        
    CleanupStack::Pop(self);
    return self;
    }

CAknFepDualLanguageSettingList *CAknFepDualLanguageSettingList::NewLC(CAknFepDualLanguageSettingData* aData, CAknFepDualLanguageSettingDialog* aDialog)
    {
    CAknFepDualLanguageSettingList* self = new (ELeave) CAknFepDualLanguageSettingList(aData, aDialog);
    CleanupStack::PushL(self);
    return self;
    }

CAknFepDualLanguageSettingList::CAknFepDualLanguageSettingList(CAknFepDualLanguageSettingData* aData, CAknFepDualLanguageSettingDialog* aDialog) : 
    CAknSettingItemList(), iData(aData), iDialog(aDialog)
    {
    }

CAknFepDualLanguageSettingList::~CAknFepDualLanguageSettingList()
    {
    // no specific destruction code required - no owned data
    }


void CAknFepDualLanguageSettingList::SizeChanged()
    {
    // if size changes, make sure component takes whole available space
    CEikFormattedCellListBox *listbox = ListBox();

    if (listbox) 
        {
        listbox->SetRect(Rect());
        }
    }

void CAknFepDualLanguageSettingList::HandleListBoxEventL(CEikListBox* /*aListBox*/, TListBoxEvent aEventType)
    {
    if(aEventType == EEventEnterKeyPressed)
        {
        EditCurrentItemL(EFalse);
        }
    }

void CAknFepDualLanguageSettingList::EditCurrentItemL(TBool aCalledFromMenu)
    {
    // invoke EditItemL on the current item
    TInt index = SettingItemArray()->ItemIndexFromVisibleIndex(ListBox()->CurrentItemIndex());
    
    EditItemL(index, aCalledFromMenu);  // invoked from menu,or not
              
    StoreSettingsL();
    }

CAknSettingItem * CAknFepDualLanguageSettingList::CreateSettingItemL (TInt aIdentifier) 
    {
    // method is used to create specific setting item as required at run-time.
    // aIdentifier is used to determine what kind of setting item should be 
    // created

    CAknFepDualLanguageSettingItem* settingItem = NULL;
    
    TInt language = 0;
    CRepository* aknFepRepository = CRepository::NewLC(KCRUidAknFep);
    aknFepRepository->Get( KAknFepInputTxtLang, language );
    
    switch (aIdentifier)
        {
        case EAknFepDualLanguageSettingDualInputPrimary:
            settingItem = new (ELeave) CAknFepDualLanguageSettingItem(
                    R_PRIMARY_INPUT_LANGUAGE_SETTING_PAGE, ETrue, iData);
            break;
        case EAknFepDualLanguageSettingDualInputSecondary:
            settingItem = new (ELeave) CAknFepDualLanguageSettingItem(
                    R_SECONDARY_INPUT_LANGUAGE_SETTING_PAGE, EFalse, iData);
            if(language == ELangTaiwanChinese ||
               language == ELangHongKongChinese ||
               language == ELangPrcChinese ||
               language == ELangJapanese ||
               language == ELangKorean )
                {
                settingItem->SetHidden(ETrue);
                }
            break;
        default:
            break;
        }
    CleanupStack::PopAndDestroy( aknFepRepository );
    
    return (CAknSettingItem*)settingItem;
    }

void CAknFepDualLanguageSettingList::HideSecondaryInputL(TBool aHidden)
    {
    SettingItemArray()->At(1)->SetHidden(aHidden);
    HandleChangeInItemArrayOrVisibilityL();
    }
