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










#include <CentralRepository.h>
#include <AknQueryDialog.h>
#include <AknFepInternalCRKeys.h>
#include <PtiDefs.h>
#include <e32property.h>
#include <AvkonInternalCRKeys.h>
#include <AknFepGlobalEnums.h>
#include <aknnotewrappers.h>
#include <AknFep.rsg>
#include "AknFepUIAvkonCtrlDualLanguageSettingDialog.h"
#include "AknFepUIAvkonCtrlDualLanguageSettingModel.h"
#include "AknFepUIAvkonCtrlDualLanguageSettingData.h"

CAknFepDualLanguageSettingDialog *CAknFepDualLanguageSettingDialog::NewL(TInt aMenuResId)
    {
    CAknFepDualLanguageSettingDialog* self = CAknFepDualLanguageSettingDialog::NewLC(aMenuResId);      
    CleanupStack::Pop(self);
    return self;
    }

CAknFepDualLanguageSettingDialog *CAknFepDualLanguageSettingDialog::NewLC(TInt aMenuResId)
    {
    CAknFepDualLanguageSettingDialog* self = new (ELeave) CAknFepDualLanguageSettingDialog();
    CleanupStack::PushL(self);
    self->ConstructL(aMenuResId);
    return self;
    }

CAknFepDualLanguageSettingDialog::CAknFepDualLanguageSettingDialog()    
    {
    }
    
CAknFepDualLanguageSettingDialog::~CAknFepDualLanguageSettingDialog()
    {
    delete iLangData;
    delete iLanguages;
    delete iSecondaryLanguages;
    delete iModel;
    }

SEikControlInfo CAknFepDualLanguageSettingDialog::CreateCustomControlL(TInt /*aControlType*/)
{
    iModel = CAknFepDualLanguageSettingModel::NewL();
    // construct the data object the settings list will use
    ReadSettingsDataL();
        
    iSettingsList = CAknFepDualLanguageSettingList::NewL(iLangData, this);
    
    SEikControlInfo controlInfo;
    controlInfo.iFlags = EEikControlHasEars;
    controlInfo.iTrailerTextId = 0;
    controlInfo.iControl = iSettingsList;
    STATIC_CAST(CAknFepDualLanguageSettingList*, controlInfo.iControl)->SetContainerWindowL(*this);
    
    return controlInfo;
}

CAknFepDualLanguageSettingDialog::TFormControlTypes CAknFepDualLanguageSettingDialog::ConvertCustomControlTypeToBaseControlType(TInt /*aControlType*/) const
{
    return EPopfieldDerived;
}

TBool CAknFepDualLanguageSettingDialog::OkToExitL(TInt aButtonId)
{
    TBool retValue = EFalse;
    switch(aButtonId)
    {
    case EAknSoftkeyBack:        
        retValue = ETrue;
        break;
    
    case EAknSoftkeyOptions:
        DisplayMenuL();
        break;
    
    case EAknSoftkeyChange:
        EditItemL(EFalse);
        break;
    }
    return retValue;
}


void CAknFepDualLanguageSettingDialog::ProcessCommandL(TInt aCommandId)
    {
    if(MenuShowing())
        HideMenu();   
    
    switch(aCommandId) 
        {
        case EAknFepCmdPredSettingChange:
            EditItemL(ETrue);
            break;
        case EAknCmdHelp:
            break;
        case EAknCmdExit:
			// Pass the EAknCmdExit command to Avkon 
			// Framework 
			iAvkonAppUi->ProcessCommandL(aCommandId);
            break;
        }
    }

void CAknFepDualLanguageSettingDialog::ReadSettingsDataL()
    {
    if(iLangData == NULL)
        {
        iLangData = CAknFepDualLanguageSettingData::NewL();
        }
    
    // If the setting page has a list create array which contains all
    // the items of the list in the setting page:

    //some dummy size of 10
    iLanguages = new( ELeave )CArrayFixFlat<TInt>(10);

    iSecondaryLanguages = new( ELeave )CArrayFixFlat<TInt>(10);

    //some dummy size of 10
    CDesCArrayFlat* items = new( ELeave )CDesCArrayFlat(10);
    CleanupStack::PushL( items );

    CDesCArrayFlat* secondaryItems = new( ELeave )CDesCArrayFlat(10);
    CleanupStack::PushL( secondaryItems );
    
    //TODO: Adding the GetLanguage and GetSecondaryLanguage APIs
    iModel->GetLanguagesFromPtiL( NULL, iLanguages, items );
    iModel->GetSecondaryInputLanguagesL(iSecondaryLanguages, secondaryItems,
            iCoeEnv);
    
    TInt i;
    TInt currentItem = 0;
    TInt currentItemSecondary = 0;

    //fetch the current value from model:
    // current item is the decimal value of T9 language code from shared data
    TInt currentInputLanguage =
    iModel->LanguageL( CAknFepDualLanguageSettingModel::EAknFepDualLanguageSettingInputLanguage );

    TInt currentInputLanguageSecondary =
    iModel->LanguageL( CAknFepDualLanguageSettingModel::EAknFepDualLanguageSettingInputLanguageSecondary );
    
    // total sum of available T9 dictionaries
    TInt count = iLanguages->Count();

    // compare sharedata languageCode with PtiLanguageCode array
    // Ptilanguage <-> listindex converter

    for ( i=0;i<count;i++ )
        {
        if ((*iLanguages)[i] == currentInputLanguage)
            {
            currentItem = i;
            break;
            }
        }
    
    count = iSecondaryLanguages->Count();

    // compare sharedata languageCode with PtiLanguageCode array
    // Ptilanguage <-> listindex converter

    for ( i=0;i<count;i++ )
        {
        if ((*iSecondaryLanguages)[i] == currentInputLanguageSecondary)
            {
            currentItemSecondary = i;
            break;
            }
        }

    iLangData->LoadL(items, secondaryItems, currentItem, currentItemSecondary);
    
    CleanupStack::Pop(secondaryItems);
    CleanupStack::Pop(items);
    }

void CAknFepDualLanguageSettingDialog::SaveSettingsDataL() const
    {    
    TInt index = iSettingsList->SettingItemArray()->ItemIndexFromVisibleIndex(iSettingsList->ListBox()->CurrentItemIndex());
    TInt curLang = (*iLanguages)[iLangData->CurrentValue()]; 
       
    if(0 == index)
        iModel->SetLanguageL( CAknFepDualLanguageSettingModel::EAknFepDualLanguageSettingInputLanguage, curLang );
    else
        {        
         if(curLang == ELangTaiwanChinese ||
         curLang == ELangHongKongChinese ||
         curLang == ELangPrcChinese ||
         curLang == ELangJapanese ||
         curLang == ELangKorean )
             {
             iModel->SetLanguageL( CAknFepDualLanguageSettingModel::EAknFepDualLanguageSettingInputLanguageSecondary, 0);
             }
         else
             {
             iModel->SetLanguageL( CAknFepDualLanguageSettingModel::EAknFepDualLanguageSettingInputLanguageSecondary,
                               (*iSecondaryLanguages)[iLangData->CurrentSecondaryValue()]);
             }
         }
    }
    
void CAknFepDualLanguageSettingDialog::EditItemL(TBool aCalledFromMenu)
    {
    iSettingsList->EditCurrentItemL(aCalledFromMenu);
    TInt curLang = (*iLanguages)[iLangData->CurrentValue()]; 
    if(curLang == ELangTaiwanChinese ||
       curLang == ELangHongKongChinese ||
       curLang == ELangPrcChinese ||
       curLang == ELangJapanese ||
       curLang == ELangKorean )
        {
        iSettingsList->HideSecondaryInputL(ETrue);
        }
    else
        {
        iSettingsList->HideSecondaryInputL(EFalse);
        }
    
    iSettingsList->StoreSettingsL();
    SaveSettingsDataL();
    }