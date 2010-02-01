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
/*
 * ============================================================================
 *  Name     : CGSPredictiveSettingList from AknFepPredictiveSettingList 
 * ============================================================================
 */
#include <CentralRepository.h>
#include <AknQueryDialog.h>
#include <AknFepInternalCRKeys.h>
#include <PtiDefs.h>
#include <e32property.h>
#include <AvkonInternalCRKeys.h>
#include <AknFepGlobalEnums.h>
#include "DualLanguageDialog.h"
#include <aknnotewrappers.h>
#include <AknFep.rsg>
#include "GSLangModel.h"
#include "DualLanguageData.h"
#include <GSCommon.hrh>

CGSDualLanguageDialog *CGSDualLanguageDialog::NewL(TInt aMenuResId, CGSLangModel* aModel)
    {
    CGSDualLanguageDialog* self = CGSDualLanguageDialog::NewLC(aMenuResId, aModel);      
    CleanupStack::Pop(self);
    return self;
    }

CGSDualLanguageDialog *CGSDualLanguageDialog::NewLC(TInt aMenuResId, CGSLangModel* aModel)
    {
    CGSDualLanguageDialog* self = new (ELeave) CGSDualLanguageDialog(aModel);
    CleanupStack::PushL(self);
    self->ConstructL(aMenuResId);
    return self;
    }

CGSDualLanguageDialog::CGSDualLanguageDialog(CGSLangModel* aModel)
    : iModel(aModel)
    {
    }
    
CGSDualLanguageDialog::~CGSDualLanguageDialog()
    {
    delete iLangData;
    delete iLanguages;
    delete iSecondaryLanguages;
    }

SEikControlInfo CGSDualLanguageDialog::CreateCustomControlL(TInt /*aControlType*/)
{
    // construct the data object the settings list will use
    ReadSettingsDataL();
        
    iSettingsList = CGSDualLanguageList::NewL(iLangData, this);
    
    SEikControlInfo controlInfo;
    controlInfo.iFlags = EEikControlHasEars;
    controlInfo.iTrailerTextId = 0;
    controlInfo.iControl = iSettingsList;
    STATIC_CAST(CGSDualLanguageList*, controlInfo.iControl)->SetContainerWindowL(*this);
    
    return controlInfo;
}

CGSDualLanguageDialog::TFormControlTypes CGSDualLanguageDialog::ConvertCustomControlTypeToBaseControlType(TInt /*aControlType*/) const
{
    return EPopfieldDerived;
}

TBool CGSDualLanguageDialog::OkToExitL(TInt aButtonId)
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

void CGSDualLanguageDialog::ProcessCommandL(TInt aCommandId)
    {
	// This is TSW eror fixing.
	// This function is called by dialog framework, whenever menu item is 
	// selected. we should have to hide existing display menu first to maitain the
	// control stack .
	// If user try to stop displaying menu after creating any control
	// In that situation whenever application goes to background framework remove the
	// control from the stack when try to remove the menubar from control stack.
	 CAknDialog::ProcessCommandL(aCommandId);
	
    switch(aCommandId) 
        {
        case EGSCmdAppChange:
            EditItemL(ETrue);
            break;
        case EGSCmdAppHelp:
            break;
        case EAknCmdExit:
			// Pass the EAknCmdExit command to Avkon 
			// Framework 
			iAvkonAppUi->ProcessCommandL(aCommandId);
            break;
        }

    }

void CGSDualLanguageDialog::ReadSettingsDataL()
    {
    if(iLangData == NULL)
        {
        iLangData = CGSDualLanguageData::NewL();
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
    
    iModel->GetLanguagesFromPtiL( NULL, iLanguages, items );
    iModel->GetSecondaryInputLanguagesL(iSecondaryLanguages, secondaryItems,
            iCoeEnv);
    
    TInt i;
    TInt currentItem = 0;
    TInt currentItemSecondary = 0;

    //fetch the current value from model:
    // current item is the decimal value of T9 language code from shared data
    TInt currentInputLanguage =
    iModel->LanguageL( CGSLangModel::EGSInputLanguage );

    TInt currentInputLanguageSecondary =
    iModel->LanguageL( CGSLangModel::EGSInputLanguageSecondary );
    
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

void CGSDualLanguageDialog::SaveSettingsDataL() const
    {
    TInt index = iSettingsList->SettingItemArray()->ItemIndexFromVisibleIndex(iSettingsList->ListBox()->CurrentItemIndex());
    TInt curLang = (*iLanguages)[iLangData->CurrentValue()]; 
    
    if(0 == index)
        {
        TInt previous = iModel->LanguageL( CGSLangModel::EGSInputLanguage );
        iModel->SetLanguageL( CGSLangModel::EGSInputLanguage, curLang );     
               
        if ((curLang == ELangPrcChinese || curLang == ELangHongKongChinese
            || curLang == ELangTaiwanChinese) && previous != curLang)
            {
            if (curLang == ELangPrcChinese )
                {
                iModel->SetDefaultInputMethodL( EPinyin );
                }
            else if ( curLang == ELangHongKongChinese )
                {
                iModel->SetDefaultInputMethodL( EStroke );
                }
            else if ( curLang == ELangTaiwanChinese )
                {
                iModel->SetDefaultInputMethodL( EZhuyin );
                }            
            }
        }
    else
        {        
        if(curLang == ELangTaiwanChinese ||
           curLang == ELangHongKongChinese ||
           curLang == ELangPrcChinese ||
           curLang == ELangJapanese ||
           curLang == ELangKorean )
            {
            iModel->SetLanguageL( CGSLangModel::EGSInputLanguageSecondary, 0);
            }
        else
            {
            iModel->SetLanguageL( CGSLangModel::EGSInputLanguageSecondary,
                    (*iSecondaryLanguages)[iLangData->CurrentSecondaryValue()]);
            }
        }
    }

void CGSDualLanguageDialog::EditItemL(TBool aCalledFromMenu)
    {
    iSettingsList->EditCurrentItemL(aCalledFromMenu);
    
    // Secondary language item shouldnt be displayed for Chinsese/Japanese/Korean languages
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
