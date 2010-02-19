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

#include <barsread.h>
#include <AvkonInternalCRKeys.h>
#include <e32property.h>
#include <PtiDefs.h>
#include <AknFep.rsg>
#include <AknFepGlobalEnums.h>
#include "PredictiveSettingList.h"
#include "PredictiveSettingData.h"
#include "PredictiveSettingDialog.h"

CGSMultiSelectionSettingItem::CGSMultiSelectionSettingItem ( 
        TInt aResourceId, CGSPredictiveSettingData &aData, CCoeEnv* aCoeEnv )
    : CAknBigSettingItemBase(aResourceId), iData(aData), iCoeEnv(aCoeEnv)
    {
    }

CGSMultiSelectionSettingItem::~CGSMultiSelectionSettingItem()
    {
    if(iSelection)
    	iSelection->ResetAndDestroy();
    delete iSelection;
    iSelection = NULL;
    }
        
void CGSMultiSelectionSettingItem::EditItemL( TBool /*aCalledFromMenu*/ )
    {
    if(!iSelection)
        {
        iSelection = CreateSelectionItemListL();
        }
    CAknCheckBoxSettingPage* settingPage = 
        new( ELeave) CAknCheckBoxSettingPage( R_AUTOWORDCOMPLMULTI_PAGE, iSelection);

    if( settingPage->ExecuteLD(CAknSettingPage::EUpdateWhenAccepted))
        {
        StoreL();
        }
    else
        {
	TInt physicalKeyboards = iData.PhysicalKeyboards();
        
        if(physicalKeyboards & EPhysicalKeyboard12key)
            {
            iSelection->At(iItutIndex)->SetSelectionStatus(iData.AWCBitmask() & EKeyboardStyle12Key);
            }
        if((physicalKeyboards & EPhysicalKeyboardQwerty4x10) || (physicalKeyboards & EPhysicalKeyboardQwerty4x12) || (physicalKeyboards & EPhysicalKeyboardQwerty3x11))
            {
            iSelection->At(iQwertyIndex)->SetSelectionStatus(iData.AWCBitmask() & EKeyboardStyleQwerty);
            }
        else if(physicalKeyboards & EPhysicalKeyboardHalfQwerty)
            {
            iSelection->At(iHalfQwertyIndex)->SetSelectionStatus(iData.AWCBitmask() & EKeyboardStyleHalfQwerty);
            }
        }
    SetSettingPage(0);
    }

void CGSMultiSelectionSettingItem::StoreL()
    {
    if(iSelection)
        {
        TInt physicalKeyboards = iData.PhysicalKeyboards();
        for(TUint i = 0x01; i <= 0xF0; i = i << 1)
            {
            if(physicalKeyboards & i)
                {
                switch(i)
                    {
                    case 0x01: // itut
                        if(iSelection->At(iItutIndex)->SelectionStatus())
                            {
                            iData.AWCBitmask() |= EKeyboardStyle12Key;
                            }
                        else
                            {
                            iData.AWCBitmask() &= ~EKeyboardStyle12Key;
                            }
                        break;
                    case 0x10: // half
                        if(iSelection->At(iHalfQwertyIndex)->SelectionStatus())
                            {
                            iData.AWCBitmask() |= EKeyboardStyleHalfQwerty;
                            }
                        else
                            {
                            iData.AWCBitmask() &= ~EKeyboardStyleHalfQwerty;
                            }
                        break;
    
                    case 0x02: // 4x12
                    case 0x04: // 4x10
                    case 0x08: // 3x11
                    case 0x20: // custom
                        if(iSelection->At(iQwertyIndex)->SelectionStatus())
                            {
                            iData.AWCBitmask() |= EKeyboardStyleQwerty;
                            }
                        else
                            {
                            iData.AWCBitmask() &= ~EKeyboardStyleQwerty;
                            }
                        i = 0x20; // Skip rest of qwerty checks
                        break;
                    default:
                        break;
                    }
                }
            }
        }
    }

void CGSMultiSelectionSettingItem::LoadL()
    {
    if(iSelection)
        {
        iSelection->ResetAndDestroy();
        delete iSelection;  
        iSelection = NULL;      
        }
    iSelection = CreateSelectionItemListL();
    }

CSelectionItemList* CGSMultiSelectionSettingItem::CreateSelectionItemListL()
    {
    CSelectionItemList* selection = new (ELeave) CSelectionItemList(2);
    TInt physicalKeyboards = iData.PhysicalKeyboards();
    TBool settingActive = EFalse;
    HBufC* selectionText = 0;
    TInt count = 0;
    for(TUint i = 0x01; i <= 0xF0; i = i << 1)
        {
        if(physicalKeyboards & i)
            {
            switch(i)
                {
                case 0x01: // itut
                    settingActive = ( iData.AWCBitmask() & EKeyboardStyle12Key ? 1 : 0 ); 
                    selectionText = iCoeEnv->AllocReadResourceLC(R_AKNFEP_PRED_SETTING_ITUT);
                    iItutIndex = count++;                    
                    break;
                case 0x10: // half
                    settingActive = ( iData.AWCBitmask() & EKeyboardStyleHalfQwerty ? 1 : 0 ); 
                    selectionText = iCoeEnv->AllocReadResourceLC( R_AKNFEP_PRED_SETTING_HALF_QWERTY );
                    iHalfQwertyIndex = count++;
                    break;
                case 0x02: // 4x12
                case 0x04: // 4x10
                case 0x08: // 3x11
                case 0x20: // custom
                    settingActive = ( iData.AWCBitmask() & EKeyboardStyleQwerty ? 1: 0);
                    selectionText = iCoeEnv->AllocReadResourceLC(R_AKNFEP_PRED_SETTING_QWERTY);
                    iQwertyIndex = count++;
                    i = 0x20; // Skip rest of qwerty checks
                    break;
                default:
                    break;
                }
            
            CSelectableItem* item = new (ELeave) CSelectableItem( *selectionText, settingActive );
            item->ConstructL();
            selection->AppendL(item);
            CleanupStack::PopAndDestroy(); // selectionText
            }
        }
    return selection;
    }

CGSPredictiveSettingList *CGSPredictiveSettingList::NewL(CGSPredictiveSettingData &aData, CGSPredictiveSettingDialog &aDialog)
	{
	CGSPredictiveSettingList* self = CGSPredictiveSettingList::NewLC(aData, aDialog);		
	CleanupStack::Pop(self);
	return self;
	}

CGSPredictiveSettingList *CGSPredictiveSettingList::NewLC(CGSPredictiveSettingData &aData, CGSPredictiveSettingDialog &aDialog)
	{
	CGSPredictiveSettingList* self = new (ELeave) CGSPredictiveSettingList(aData, aDialog);
	CleanupStack::PushL(self);
	return self;
	}

CGSPredictiveSettingList::CGSPredictiveSettingList(CGSPredictiveSettingData &aData, CGSPredictiveSettingDialog &aDialog) : 
	CAknSettingItemList(),
	iSettingsData(aData),
	iSettingsDialog(aDialog)	
	{
	}

CGSPredictiveSettingList::~CGSPredictiveSettingList()
	{
	// no specific destruction code required - no owned data
	}


void CGSPredictiveSettingList::SizeChanged()
    {
	// if size changes, make sure component takes whole available space
	CEikFormattedCellListBox *listbox = ListBox();

    if (listbox) 
        {
        listbox->SetRect(Rect());
        }
    }

void CGSPredictiveSettingList::HandleListBoxEventL(CEikListBox* aListBox, TListBoxEvent aEventType)
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

void CGSPredictiveSettingList::EditCurrentItemL(TBool aCalledFromMenu)
	{
	// invoke EditItemL on the current item
	TInt index = SettingItemArray()->ItemIndexFromVisibleIndex(ListBox()->CurrentItemIndex());
	
	EditItemL(index, aCalledFromMenu);	// invoked from menu,or not
			  
	StoreSettingsL();
	}

CAknSettingItem * CGSPredictiveSettingList::CreateSettingItemL (TInt aIdentifier) 
	{
	// method is used to create specific setting item as required at run-time.
	// aIdentifier is used to determine what kind of setting item should be 
	// created
	CAknSettingItem* settingItem = NULL;
	TInt keyboardLayout = 0;
	RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
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
            settingItem = new (ELeave) CGSMultiSelectionSettingItem (
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
            if((keyboardLayout == EPtiKeyboard12Key )||
               (keyboardLayout == EPtiKeyboardHalfQwerty)||
               (!DeviceHasQwertyKeyboard()))
                settingItem->SetHidden(ETrue);
            break;
        case EAknFepCmdPredTypingCorrectionSetting:
            settingItem = new (ELeave) CAknEnumeratedTextPopupSettingItem (
                                            aIdentifier, 
                                            iSettingsData.TypingCorrection());
            // Disabled for ITU-T
            if((keyboardLayout == EPtiKeyboard12Key )||
               (keyboardLayout == EPtiKeyboardHalfQwerty)||
               (!DeviceHasQwertyKeyboard()))
                settingItem->SetHidden(ETrue);
            break;
        case EAknFepCmdPredPrimaryCandidateSetting:
            settingItem = new (ELeave) CAknBinaryPopupSettingItem (
                                            aIdentifier, 
                                            iSettingsData.PrimaryCandidate());
            // Disabled for Half Qwerty and ITU-T
            if((keyboardLayout == EPtiKeyboard12Key )||
               (keyboardLayout == EPtiKeyboardHalfQwerty)||
               (!DeviceHasNonHalfQwertyKeyboard()))
                settingItem->SetHidden(ETrue);
            break;
        default:
            break;
        }

    return settingItem;
	}

TBool CGSPredictiveSettingList::DeviceHasMultipleKeyboards() 
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

TBool CGSPredictiveSettingList::DeviceHasQwertyKeyboard() 
    {
    TInt physicalKeyboards = iSettingsData.PhysicalKeyboards();
    if(physicalKeyboards & 0x02 || // 4x12
       physicalKeyboards & 0x04 || // 4x10
       physicalKeyboards & 0x08 || // 3x11
       physicalKeyboards & 0x10 || // half
       physicalKeyboards & 0x20)   // custom
        {
        return ETrue;
        }
    return EFalse;
    }

TBool CGSPredictiveSettingList::DeviceHasNonHalfQwertyKeyboard() 
    {
    TInt physicalKeyboards = iSettingsData.PhysicalKeyboards();
    if(physicalKeyboards & 0x02 || // 4x12
       physicalKeyboards & 0x04 || // 4x10
       physicalKeyboards & 0x08 || // 3x11
       physicalKeyboards & 0x20)   // custom
        {
        return ETrue;
        }
    return EFalse;
    }
// This part of code for error fixing multiple layout.Whenever you switch from one keyboard 
// Layout to another keyboard layout HandleResourceChange is getting called and displays or 
// hides the Input Option Settings information specific to keyboard layout
void CGSPredictiveSettingList::HandleResourceChange(TInt aType)
    {
    CAknSettingItemList::HandleResourceChange(aType);
    // Check for keyboard layout change
    // If layout is changed, close settings with information note
    if(aType==KEikDynamicLayoutVariantSwitch)
        {
        TInt keyboardLayout = 0;
        RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
        if(keyboardLayout == EPtiKeyboard12Key)
            {
	    //Returning as Hiding for Touch Input for Virtual Input is not required
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
	    return;
#else
            // Number candidate
            SettingItemArray()->At(2)->SetHidden(ETrue);
            // Typing correction
            SettingItemArray()->At(3)->SetHidden(ETrue);
            // Primary Candidate
            SettingItemArray()->At(4)->SetHidden(ETrue);
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
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
        TRAP_IGNORE(HandleChangeInItemArrayOrVisibilityL());
        }
    }

