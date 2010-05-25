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
 *  Name     : CAknFepPredictiveSettingList from AknFepPredictiveSettingList 
 * ============================================================================
 */

#include <barsread.h>
#include <AvkonInternalCRKeys.h>
#include <e32property.h>
#include <PtiDefs.h>
#include <aknfep.rsg>
#include <AknFepGlobalEnums.h>
#include <featmgr.h> 

#include "AknFepPredictiveSettingList.h"
#include "AknFepPredictiveSettingData.h"
#include "AknFepPredictiveSettingDialog.h"

CAknFepMultiSelectionSettingItem::CAknFepMultiSelectionSettingItem ( 
        TInt aResourceId, CAknFepPredictiveSettingData &aData, CCoeEnv* aCoeEnv )
    : CAknBigSettingItemBase(aResourceId), iData(aData), iCoeEnv(aCoeEnv)
    {
    iItutIndex = -1;
    iQwertyIndex = -1;
    iHalfQwertyIndex = -1;    
    }

CAknFepMultiSelectionSettingItem::~CAknFepMultiSelectionSettingItem()
    {
    if (iSelection)
        {
        iSelection->ResetAndDestroy();
        }
    delete iSelection;
    }
        
void CAknFepMultiSelectionSettingItem::EditItemL( TBool /*aCalledFromMenu*/ )
    {
    if(!iSelection)
        {
        iSelection = CreateSelectionItemListL();
        }
    CAknCheckBoxSettingPage* settingPage = 
        new( ELeave) CAknCheckBoxSettingPage( R_AUTOWORDCOMPLMULTI_PAGE, iSelection);

    SetSettingPage(settingPage);
    
    if( settingPage->ExecuteLD(CAknSettingPage::EUpdateWhenAccepted))
        {
        StoreL();
        }
    else
        {
        TInt physicalKeyboards = iData.PhysicalKeyboards();
        
        if( iItutIndex > -1 )
            {
            TBool selected = iData.AWCBitmask() & EKeyboardStyle12Key ? ETrue : EFalse;
            iSelection->At(iItutIndex)->SetSelectionStatus( selected );
            }
        
        if( iQwertyIndex > -1  )
            {
            TBool selected = iData.AWCBitmask() & EKeyboardStyleQwerty ? ETrue : EFalse;
            iSelection->At(iQwertyIndex)->SetSelectionStatus( selected );
            }
        
        if( iHalfQwertyIndex > -1 )
            {
            TBool selected = iData.AWCBitmask() & EKeyboardStyleHalfQwerty ? ETrue : EFalse;
            iSelection->At(iHalfQwertyIndex)->SetSelectionStatus( selected );
            }
        }
    SetSettingPage(0);
    }

void CAknFepMultiSelectionSettingItem::StoreL()
    {
    if(iSelection)
        {
        if ( iItutIndex > -1 )
            {
            if(iSelection->At(iItutIndex)->SelectionStatus())
                {
                iData.AWCBitmask() |= EKeyboardStyle12Key;
                }
            else
                {
                iData.AWCBitmask() &= ~EKeyboardStyle12Key;
                }
            }
        
        if( iHalfQwertyIndex > -1 )
            {                   
            if(iSelection->At(iHalfQwertyIndex)->SelectionStatus())
                {
                iData.AWCBitmask() |= EKeyboardStyleHalfQwerty;
                }
            else
                {
                iData.AWCBitmask() &= ~EKeyboardStyleHalfQwerty;
                }
            }
        if ( iQwertyIndex > -1 )
            {
            if(iSelection->At(iQwertyIndex)->SelectionStatus())
                {
                iData.AWCBitmask() |= EKeyboardStyleQwerty;
                }
            else
                {
                iData.AWCBitmask() &= ~EKeyboardStyleQwerty;
                }
            }
        }
    }

void CAknFepMultiSelectionSettingItem::LoadL()
    {
    if(iSelection)
        {
        iSelection->ResetAndDestroy();
        delete iSelection;   
        iSelection = NULL;
        }
    iSelection = CreateSelectionItemListL();
    }

CSelectionItemList* CAknFepMultiSelectionSettingItem::CreateSelectionItemListL()
    {
    iItutIndex = -1;
    iQwertyIndex = -1;
    iHalfQwertyIndex = -1;
    
    CSelectionItemList* selection = new (ELeave) CSelectionItemList(2);
    CleanupStack::PushL( selection );
    
    TInt physicalKeyboards = iData.PhysicalKeyboards();
    TInt count = 0;
    
    TInt qwertyMask = EPhysicalKeyboardQwerty4x12 | EPhysicalKeyboardQwerty4x10 | 
                      EPhysicalKeyboardQwerty3x11 | EPhysicalKeyboardCustomQwerty;
    
    TBool supportVFSQ = FeatureManager::FeatureSupported( KFeatureIdVirtualFullscrQwertyInput );
    
    if ( physicalKeyboards & EPhysicalKeyboard12key ) //itut
        {
        TBool active = iData.AWCBitmask() & EKeyboardStyle12Key ? ETrue : EFalse; 
        HBufC* text = iCoeEnv->AllocReadResourceLC( R_AKNFEP_PRED_SETTING_ITUT );
        iItutIndex = count++;
        
        CSelectableItem* item = new (ELeave) CSelectableItem( *text, active );
        CleanupStack::PushL( item );
        item->ConstructL();
        selection->AppendL( item );
        CleanupStack::Pop( item );
        CleanupStack::PopAndDestroy( text );        
        }
    
    if ( ( physicalKeyboards & qwertyMask ) || supportVFSQ ) //4x12,4x10,3x11,custom, FSQ
        {
        TBool active = iData.AWCBitmask() & EKeyboardStyleQwerty ? ETrue : EFalse;
        HBufC* text = iCoeEnv->AllocReadResourceLC( R_AKNFEP_PRED_SETTING_QWERTY );
        iQwertyIndex = count++;

        CSelectableItem* item = new (ELeave) CSelectableItem( *text, active );
        CleanupStack::PushL( item );
        item->ConstructL();
        selection->AppendL( item );
        CleanupStack::Pop( item );
        CleanupStack::PopAndDestroy( text );          
        }
  
    if ( physicalKeyboards & EPhysicalKeyboardHalfQwerty ) //half qwerty
        {
        TBool active = iData.AWCBitmask() & EKeyboardStyleHalfQwerty ? ETrue : EFalse; 
        HBufC* text = iCoeEnv->AllocReadResourceLC( R_AKNFEP_PRED_SETTING_HALF_QWERTY );
        iHalfQwertyIndex = count++;
        
        CSelectableItem* item = new (ELeave) CSelectableItem( *text, active );
        CleanupStack::PushL( item );
        item->ConstructL();
        selection->AppendL( item );
        CleanupStack::Pop( item );
        CleanupStack::PopAndDestroy( text );        
        }
    
    CleanupStack::Pop( selection );
    
    return selection;
    }

CAknFepPredictiveSettingList *CAknFepPredictiveSettingList::NewL(CAknFepPredictiveSettingData &aData, CAknFepPredictiveSettingDialog &aDialog)
	{
	CAknFepPredictiveSettingList* self = CAknFepPredictiveSettingList::NewLC(aData, aDialog);		
	CleanupStack::Pop(self);
	return self;
	}

CAknFepPredictiveSettingList *CAknFepPredictiveSettingList::NewLC(CAknFepPredictiveSettingData &aData, CAknFepPredictiveSettingDialog &aDialog)
	{
	CAknFepPredictiveSettingList* self = new (ELeave) CAknFepPredictiveSettingList(aData, aDialog);
	CleanupStack::PushL(self);
	return self;
	}

CAknFepPredictiveSettingList::CAknFepPredictiveSettingList(CAknFepPredictiveSettingData &aData, CAknFepPredictiveSettingDialog &aDialog) : 
	CAknSettingItemList(),
	iSettingsData(aData),
	iSettingsDialog(aDialog)	
	{
	}

CAknFepPredictiveSettingList::~CAknFepPredictiveSettingList()
	{
	// no specific destruction code required - no owned data
	}


void CAknFepPredictiveSettingList::SizeChanged()
    {
	// if size changes, make sure component takes whole available space
	CEikFormattedCellListBox *listbox = ListBox();

    if (listbox) 
        {
        listbox->SetRect(Rect());
        }
    }

void CAknFepPredictiveSettingList::HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType)
	{
	TBool isSingleClick = iAvkonAppUi->IsSingleClickCompatible();
	if( aEventType == EEventEnterKeyPressed || aEventType == EEventItemDoubleClicked 
	      || ( isSingleClick && ( aEventType == EEventItemSingleClicked )))
		{
		iSettingsDialog.EditItemL(EFalse);//EditCurrentItemL(EFalse);
		}
	else
	    {
	    CAknSettingItemList::HandleListBoxEventL( aListBox,aEventType );
	    }
	}

void CAknFepPredictiveSettingList::EditCurrentItemL(TBool aCalledFromMenu)
	{
	// invoke EditItemL on the current item
	TInt index = SettingItemArray()->ItemIndexFromVisibleIndex(ListBox()->CurrentItemIndex());
	
	EditItemL(index, aCalledFromMenu);	// invoked from menu,or not
			  
	StoreSettingsL();
	}

CAknSettingItem * CAknFepPredictiveSettingList::CreateSettingItemL (TInt aIdentifier) 
	{
	// method is used to create specific setting item as required at run-time.
	// aIdentifier is used to determine what kind of setting item should be 
	// created
	CAknSettingItem* settingItem = NULL;
    TPtiKeyboardType keyboardLayout = iSettingsData.OwnerKeyboardType();	
	
    switch (aIdentifier)
        {
        case EAknFepCmdPredAutoComplSetting:
            settingItem = new (ELeave) CAknBinaryPopupSettingItem (
                        aIdentifier, iSettingsData.AutoWordCompl());
            if(DeviceHasMultipleKeyboards())
                {
                settingItem->SetHidden(ETrue);
                }
            break;
        case EAknFepCmdPredAutoComplMultiSetting:
            settingItem = new (ELeave) CAknFepMultiSelectionSettingItem (
                    aIdentifier, iSettingsData, iCoeEnv ); 
            if(!DeviceHasMultipleKeyboards())
                {
                settingItem->SetHidden(ETrue);
                }
            break;
        case EAknFepCmdPredNumberCandidateSetting:
            settingItem = new (ELeave) CAknBinaryPopupSettingItem (
                                            aIdentifier, 
                                            iSettingsData.NumberCandidates());
            if(keyboardLayout == EPtiKeyboard12Key || keyboardLayout == EPtiKeyboardHalfQwerty)
                settingItem->SetHidden(ETrue);
            break;
        case EAknFepCmdPredTypingCorrectionSetting:
            settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem (
                                            aIdentifier, 
                                            iSettingsData.TypingCorrection());
            // Disabled for ITU-T
            if(keyboardLayout == EPtiKeyboard12Key || keyboardLayout == EPtiKeyboardHalfQwerty)
                settingItem->SetHidden(ETrue);
            break;
        case EAknFepCmdPredPrimaryCandidateSetting:
            settingItem = new (ELeave) CAknBinaryPopupSettingItem (
                                            aIdentifier, 
                                            iSettingsData.PrimaryCandidate());
            // Disabled for Half Qwerty and ITU-T
            if(keyboardLayout == EPtiKeyboardHalfQwerty ||
                    keyboardLayout == EPtiKeyboard12Key)
                settingItem->SetHidden(ETrue);
            break;
        default:
            break;
        }

    return settingItem;
	}

TBool CAknFepPredictiveSettingList::DeviceHasMultipleKeyboards() 
    {
    TInt physicalKeyboards = iSettingsData.PhysicalKeyboards();
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

void CAknFepPredictiveSettingList::HandleResourceChange(TInt aType)
    {
    CAknSettingItemList::HandleResourceChange(aType);
    // Check for keyboard layout change
    // If layout is changed, close settings with information note
    if(aType==KEikDynamicLayoutVariantSwitch)
        {
        TPtiKeyboardType keyboardLayout = iSettingsData.OwnerKeyboardType();
        if(keyboardLayout == EPtiKeyboard12Key)
            {
#ifndef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
			// Adding this under the flag would avoid flickering as settings page will be
			// dismissed if __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__ is defined
            // Number candidate
            SettingItemArray()->At(2)->SetHidden(ETrue);
            // Typing correction
            SettingItemArray()->At(3)->SetHidden(ETrue);
            // Primary Candidate
            SettingItemArray()->At(4)->SetHidden(ETrue);
#endif            
            }
        else if (keyboardLayout == EPtiKeyboardHalfQwerty)
            {
            // Number candidate
            SettingItemArray()->At(2)->SetHidden(ETrue);
            // Typing correction
            SettingItemArray()->At(3)->SetHidden(ETrue);
            // Primary Candidate
            SettingItemArray()->At(4)->SetHidden(ETrue);
            }
        else
            {
            // Number candidate
            SettingItemArray()->At(2)->SetHidden(EFalse);
            // Typing correction
            SettingItemArray()->At(3)->SetHidden(EFalse);
            // Primary Candidate
            SettingItemArray()->At(4)->SetHidden(EFalse);            
            }
            
        TRAP_IGNORE(
        { 								 
        HandleChangeInItemArrayOrVisibilityL();	
        });
        
        }
    }

