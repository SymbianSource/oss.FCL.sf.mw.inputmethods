/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepKeyCatcher methods.
*
*/












#include <eikenv.h>
#include <eikappui.h>               //iEikonEnv
#include <e32keys.h>                //keys
#include <uikon.hrh>                //keys

#include "AknFepKeyCatcher.h"
#include "AknFepManager.h"
#include "AknFepGlobalEnums.h"
#include "AknFepPanic.h"
#include "AknFepPluginManager.h"
#include <AknDef.h>
#include <PtiEngine.h>
#include <PtiDefs.h>                //keys
#include <featmgr.h>                //FeatureManager
#include <AvkonInternalCRKeys.h>

LOCAL_C void FepObserverHandleCompletionOfTransactionL(MCoeFepObserver& aFepObserver)
    {
    aFepObserver.HandleCompletionOfTransactionL();
    }

CAknFepKeyCatcherInterface::~CAknFepKeyCatcherInterface()
    {
    }

void CAknFepKeyCatcherInterface::HandleChangeInFocus()
    {
    }

CAknFepKeyCatcher* CAknFepKeyCatcher::NewL(CAknFepManager& aFepMan)
    {
    CAknFepKeyCatcher* self=new(ELeave) CAknFepKeyCatcher(aFepMan);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(); //self
    return self;
    }

CAknFepKeyCatcher::~CAknFepKeyCatcher()
    {
    CloseWindow();
    if(NULL != iEikonEnv->EikAppUi())
        {
        iEikonEnv->EikAppUi()->RemoveFromStack(this);
#ifdef RD_SCALABLE_UI_V2
        (CEikonEnv::Static())->RemoveMessageMonitorObserver(*this); 
#endif //RD_SCALABLE_UI_V2
        }
    }

TKeyResponse CAknFepKeyCatcher::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode)
    {
   
#ifdef RD_SCALABLE_UI_V2     
    if (aEventCode == EEventKeyUp && 
        aKeyEvent.iScanCode == EStdKeyNo)
        {
        iFepMan.HandleEndKeyL();
        }
#endif //RD_SCALABLE_UI_V2 
    // Temporary solution for Disable key tone when press key in Virtual ITUT
    iFepMan.NeedDisableKeySound(aKeyEvent, aEventCode);
    // sort out if we're doing long or short presses...
    TKeyPressLength length = aKeyEvent.iRepeats ? ELongKeyPress : EShortKeyPress;
    if (iKeyCatcherState == EAknFepStateNull)
        {
        if (aEventCode == EEventKeyUp && iLongPressedScanCode == aKeyEvent.iScanCode)
            {
            // An editor has lost focus during long key press (for example SCT popup 
            // is shown during long keypress of star key).
            // EFlagLongKeyPressHandled needs to be reseted also in this case.
            ClearFlag(EFlagLongKeyPressHandled);
            iLongPressedScanCode = 0;
            }
        if((aEventCode == EEventKeyDown) && (aKeyEvent.iScanCode==EStdKeyRightShift) 
                && (iFepMan.KeyboardLayout()== EPtiKeyboard12Key) && !iFepMan.Japanese() 
                &&  !iFepMan.IsChineseInputLanguage()  )
            {
            iFepMan.SetCcpuFlag(CAknFepManager::ECcpuStateShiftkeyWasPressedBeforeLosingFocus);
            }
        else if ((aEventCode == EEventKeyUp) && (aKeyEvent.iScanCode==EStdKeyRightShift) 
                && (iFepMan.KeyboardLayout()== EPtiKeyboard12Key) 
                && iFepMan.IsCcpuFlagSet(CAknFepManager::ECcpuStateShiftkeyWasPressedBeforeLosingFocus))
            {
            iFepMan.ClearCcpuFlag(CAknFepManager::ECcpuStateShiftkeyWasPressedBeforeLosingFocus);
            }
        
        TInt keyCode = aKeyEvent.iCode;
        if (keyCode == EKeyF19 || keyCode == EKeyF22 ||
            keyCode == EKeyF23 || keyCode == EKeyF24 )
            {
            // Internal FEP key events that are simulated by FEP are always 
            // also consumed by FEP. In this situation FEP has simulated the
            // key event but the editor is unfocused right after.
            return EKeyWasConsumed;
            }
        // FEP does not handle any keyevents if there are not any FEP aware editor focused, except shift key. 
        // Notice that shift key is handled only if CAknFepKeyCatcher is constructed by focusing some editor.
        
        if (iFepMan.HashKeySelectionInUse())
        	{        	
        	// Hash key selection should work also with uneditable editors, therefore
        	// we need to fake it as a shift key press here.
	        if (aKeyEvent.iScanCode == EStdKeyHash)
    	    	{
    	    	if (aEventCode == EEventKeyUp)
    	    		{		
    	    		if (iFepMan.IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress))	   			    	    		
    	    			{    	    			    	    		
    	    			// We need to generate real shift up event for editor to
    	    			// cancel ccpu-mode.
		   				TKeyEvent ccpuStart = {0, EStdKeyLeftShift, 0, 0};
    	 				CCoeEnv::Static()->SimulateKeyEventL(ccpuStart, aEventCode);
    	    			}
    	    		else
    	    			{
    	    			// CCpu-mode was not yet activated. Just cancel the timer.
    	    			iFepMan.CancelShiftKeyTimer();
    	    			}		
    	 			return EKeyWasNotConsumed;       	    		
    	    		}
				else if (aEventCode == EEventKeyDown)   	    		
					{
					return iFepMan.HandleShiftKeyEventL(aEventCode);        							
					}
        		}
    	    }
    	
    	if( keyCode == EKeyEscape )
    		{
    		return iFepMan.HandleKeyEventL(keyCode, length);	
    		}
    		
        if ((aKeyEvent.iScanCode==EStdKeyRightShift || aKeyEvent.iScanCode==EStdKeyLeftShift))
            {
            if (aEventCode == EEventKeyUp)
                {
                iFepMan.SetExtendedFlag(CAknFepManager::EExtendedFlagShiftReleasedOnPopup);
                }
            else
                {
                iFepMan.ClearExtendedFlag(CAknFepManager::EExtendedFlagShiftReleasedOnPopup);
                }
            }
        else
            {
            return EKeyWasNotConsumed;
            }
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT
     if(iKeyCatcherState != EAknFepStateNull)
        {
            if(iFepMan.IsFnKeyMapped() &&
                (iFepMan.HandleFnKeyEventL( aKeyEvent, aEventCode ) == EKeyWasConsumed))
            {
            return EKeyWasConsumed;        
            }

        }
#endif
	
     if ( aEventCode == EEventKeyUp && 
          aKeyEvent.iScanCode == EStdKeyHash &&
          iFepMan.IsExtendedFlagSet(CAknFepManager::EExtendedFlagShortPressHashKey) )
         {
         // We are here because of short press of has key. Since, we had consumed
         // short press of hash key, we re-simulate is again, so that the respective
         // application can handle it as per their need.
         
         TKeyEvent keyHash = {EPtiKeyHash, EStdKeyHash, 0, 0};
         CCoeEnv::Static()->SimulateKeyEventL(keyHash, EEventKey);
         iFepMan.ClearExtendedFlag(CAknFepManager::EExtendedFlagShortPressHashKey);
         }
    // Check for the Wireless Keyboard Modifier Flag.
    const TUint KModifierExternalKeyboard = 0x00200000;
    if ( aKeyEvent.iModifiers & KModifierExternalKeyboard )
        {        
        if ( iFepMan.IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction |
             CAknFepManager::EFlagInsideMultitapInlineEditingTransaction) ) 
            {
            if (iFepMan.IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction))
                {
                // Remove any no matching word indicator.
                iFepMan.TryRemoveNoMatchesIndicatorL();
                }
            // Ensure the CBA is in the default state.
            iFepMan.UpdateCbaL(NULL);
            // Close FEP UI, commit the inline edit and show the cursor.            
            iFepMan.TryCloseUiL(); 
            }        
        // Allow the key through to the underlying control.
        return EKeyWasNotConsumed;
        }
    
    TKeyResponse retCode;
	if (iFepMan.HandleCcpuModeKeyEventL(aKeyEvent, aEventCode,
	                                   retCode, IsFlagSet(EFlagLongKeyPressHandled)))
		{
		return retCode;	
		}

#ifdef RD_SCALABLE_UI_V2
    // Set text selection when tap arrow keys
	if(iFepMan.HandleSelModeArrowKeyEventL(aKeyEvent, aEventCode, retCode))
        {
        return retCode;
        }        
#endif //RD_SCALABLE_UI_V2	

    
  
    if ( ( aKeyEvent.iModifiers & EModifierCtrl
#ifdef RD_INTELLIGENT_TEXT_INPUT     
           || aKeyEvent.iScanCode==EStdKeyRightCtrl
           || aKeyEvent.iScanCode==EStdKeyLeftCtrl
#endif
		 )
        && aKeyEvent.iScanCode != EStdKeyBackspace  
        && aKeyEvent.iScanCode != EStdKeyDelete )
        {// Chinese qwerty increment input mode by Ctrl+Shift
		return iFepMan.HandleQwertyControlKeyEventL(aKeyEvent, aEventCode);        
        }

    // handling hash key event in japanese input
    if (iFepMan.IsFeatureSupportedJapanese()
     && aKeyEvent.iScanCode==EStdKeyHash && !iFepMan.IsOnlyNumericPermitted())
        {
        return HandleHashKeyJapaneseL(aKeyEvent, aEventCode);
        }
    
    // Let FepMan handle chr key
    if (aKeyEvent.iScanCode==EStdKeyLeftFunc)
        {
        if (iFepMan.HandleQwertyChrKeyEventL(aEventCode))
            {
            return EKeyWasConsumed;
            }
        }
    //let FepMan handle shifts
    if ( aKeyEvent.iScanCode==EStdKeyRightShift || aKeyEvent.iScanCode
            ==EStdKeyLeftShift )
        {
        // phrase creation
        if ( iFepMan.IsPinyinPhraseCreation() )
            {
            return EKeyWasConsumed;
            }
        else
            {
            return iFepMan.HandleShiftKeyEventL( aEventCode );
            }
        }
    
    if (iFepMan.Japanese())
        {
        if ((aKeyEvent.iScanCode==EStdKeyDevice0) || (aKeyEvent.iScanCode==EStdKeyDevice1))
            {
            if (aEventCode != EEventKey)
                {
                return EKeyWasNotConsumed;
                }
            }
        }

    if (aEventCode==EEventKeyUp)
        {
        if ( iLongPressedScanCode == aKeyEvent.iScanCode )
            {
            ClearFlag(EFlagLongKeyPressHandled);
            iLongPressedScanCode = 0;
            }
        
        if ( (EStdKeyDelete == aKeyEvent.iScanCode || 
              EStdKeyBackspace == aKeyEvent.iScanCode))  
                {
                iFepMan.SetLongClearAfterCloseUI(EFalse);               
                }
            
        iFepMan.ClearFlag(CAknFepManager::EFlagCharacterAdded);
        }


    //let FepMan to try to handle Thai 0 key
    TBool thai0Keyhandling = EFalse;

    if (aKeyEvent.iScanCode==EPtiKey0 
        && (!(aKeyEvent.iModifiers & EModifierSpecial)  // If the key is produced by hardware key
          // In VKB and FSQ latin range 0 could be input with virtual keyboard
          || (iFepMan.PluginInputMode() != EPluginInputModeFSQ  
             && iFepMan.PluginInputMode() != EPluginInputModeVkb)))
        {
        if (iFepMan.HandleThai0KeyEvent(aEventCode, length, thai0Keyhandling) == EKeyWasConsumed)
            {
            return (EKeyWasConsumed); // Eats some key events to find out when o key goes up
            }
        if (thai0Keyhandling)
            {
            if(length == ELongKeyPress)
                {
                SetFlag(EFlagLongKeyPressHandled);
                iLongPressedScanCode = aKeyEvent.iScanCode;
                }
            return (iFepMan.HandleKeyEventL(EPtiKey0, length));
            }
        }
    TInt keyCode = aKeyEvent.iCode;
    TKeyResponse response = EKeyWasNotConsumed;
    
#ifdef RD_INTELLIGENT_TEXT_INPUT
    if ((aKeyEvent.iScanCode == EStdKeyNkpAsterisk ||aKeyEvent.iScanCode == EPtiKeyStar )&& iFepMan.KeyboardLayout() == EPtiKeyboard12Key)
    	{
    	//For Special Editors like Java numeric editor, EventKeyUp is allowed to flow through FEP.
    	//All other events for Special Editor is not consumed by FEP.
    	if(iFepMan.IsSpecialNumericEditor() && iFepMan.IsOnlyNumericPermitted() && aEventCode != EEventKeyUp)
    		{
    		return EKeyWasNotConsumed;
    		}
#ifdef FF_HOME_SCREEN_EASY_DIALING
				// If Easy Dialing Feature in Phone Idle is turned on then,
				// phoneIdleinEasyDialingNumericMode is set to True
        TBool phoneIdleinEasyDialingNumericMode = EFalse;
        phoneIdleinEasyDialingNumericMode = 
        	(iFepMan.EditorType() == CAknExtendedInputCapabilities::EPhoneNumberEditor) &&
        	(iFepMan.InputMode() == ENumber || iFepMan.InputMode() == ENativeNumber);
#endif           
    	// For all Numeric Editors, only Event Key is allowed to flow through
    	// When Easy Dialing is on, the editor in not numeric, even though in this scenario only Event key is allowed throug FEP
    	if ( (iFepMan.IsOnlyNumericPermitted()
#ifdef FF_HOME_SCREEN_EASY_DIALING    	
    		 || phoneIdleinEasyDialingNumericMode 
#endif    		 
    		 )&& 
    	     aEventCode != EEventKey)
    		{
    		return EKeyWasConsumed;
    		}
    	
    	/* 
    	For Long Key press:
    		We only have Event Key, and FEP handles it for launching SCT.
    	For short key presses:
    		For Non numeric editors:
    			EventDown and Eventkey:
    				FEP consumes it - if FEP is in Inline edit or it can launch SCT. It is consumed by below check and does not flow further.
    				Else, these events flow through FEP. Later decision is made to consume the event or not.
    			EventUpKey:
    				FEP consumes it - if FEP is in Inline edit or it can launch SCT. Desired functionality is achieved after flowing through FEP.
    				Else, these events flow through FEP. Later decision is made to consume the event or not.
    		For Numeric Editors:
    			FEP handles EventKey for star key looping or launching sct. Other events does not reach here.
    	*/
    	if ( length != ELongKeyPress && 
    	     aEventCode != EEventKeyUp && 
    	     (
    	      iFepMan.IsFlagSet(CAknFepManager::EFlagInsideInlineEditingTransaction) ||
    	      !iFepMan.IsAbleToLaunchSCT() 
    	     ) &&
    	     (!iFepMan.IsOnlyNumericPermitted()
#ifdef FF_HOME_SCREEN_EASY_DIALING    	
    		  && !phoneIdleinEasyDialingNumericMode 
#endif    		     	     
    	     )
    	   )
    		{
            return EKeyWasConsumed;
    		}
    	keyCode = EPtiKeyStar;
    	}
    else
#endif
    //only interested in EventKeys apart from for the shift key
    if (aEventCode!=EEventKey)
        {
        // For Japanese, don't pass EEventKeyUp and EEventKeyDown
        // event to application side if we are inputting Hiragana/Kanji.
        if (iFepMan.Japanese() && iKeyCatcherState == EAknFepStateUIActive)
            {
            // arrow keys
            if (aKeyEvent.iScanCode == EStdKeyDownArrow
                  || aKeyEvent.iScanCode == EStdKeyRightArrow
                  || aKeyEvent.iScanCode == EStdKeyLeftArrow
                  || aKeyEvent.iScanCode == EStdKeyUpArrow)
                {
                return EKeyWasConsumed;
                }
            }

        if ((iKeyCatcherState == EAknFepStateInitial)
            && (aEventCode == EEventKeyUp) && (aKeyEvent.iScanCode == EStdKeyBackspace))
            {
            CCoeEnv::Static()->ForEachFepObserverCall(FepObserverHandleCompletionOfTransactionL);
            }

        return EKeyWasNotConsumed;  
        }

    //if we've done a long press, and we get repeats, don't pass it on to the fep man.

    if (!(iFepMan.Japanese()&&
          (aKeyEvent.iCode == EKeyLeftArrow
        || aKeyEvent.iCode == EKeyRightArrow
        || aKeyEvent.iCode == EKeyDownArrow
        || aKeyEvent.iCode == EKeyUpArrow)))
        {
        if (!(aKeyEvent.iCode == EKeyPrevious || aKeyEvent.iCode == EKeyNext))
            {                	
            if (IsFlagSet(EFlagLongKeyPressHandled)&&(aKeyEvent.iRepeats))
                {
                return EKeyWasConsumed;         
                }
            }
        }

#ifdef RD_INTELLIGENT_TEXT_INPUT
    CAknFepFnKeyManager::TFnKeyState fnKeyStateBeforeKey = iFepMan.FnKeyState();

	// the short key press was blocked, don't allow the long key press to go through
	if ( IsFlagSet(EFlagBlockAllLongKeyPressEvents) && length == ELongKeyPress 
								&& !IsNaviKey(aKeyEvent.iCode) )
		{
		return EKeyWasNotConsumed;
		}
	// if the editor has no free space	
	// the key press is a valid one 
	// (it is not for eg. a simulated key press for case change)
	// we are not in between multitapping
	// block the subsequent long key press also	
	if ( length == EShortKeyPress && !iFepMan.EditorHasFreeSpace() 
		&& iFepMan.PtiEngine()->IsValidKey((TPtiKey)aKeyEvent.iScanCode)   
		&& !iFepMan.IsFlagSet(CAknFepManager::EFlagInsideMultitapInlineEditingTransaction) ) 
    	{
    	SetFlag(EFlagBlockAllLongKeyPressEvents); // block the subsequent long key press also
    	return EKeyWasNotConsumed;
    	}
    else
    	{
    	ClearFlag(EFlagBlockAllLongKeyPressEvents);
    	}	
#endif // RD_INTELLIGENT_TEXT_INPUT 
    if (iFepMan.HandleQwertyKeyEventL(aKeyEvent, response))
        {

#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__ 
		ClearFlag(EFlagBlockAllLongKeyPressEvents);	
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__    
			TPtiKeyboardType keyboardType = iFepMan.KeyboardLayout();
	        if( (length == ELongKeyPress) &&
            (
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__ 
            iFepMan.LongPressNumberEntryOnQwerty()||
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__          
            (keyboardType == EPtiKeyboardHalfQwerty)) &&
            iFepMan.KeyMapsDifferentCharacterWithFn( (TPtiKey)aKeyEvent.iScanCode ) && 
            fnKeyStateBeforeKey == CAknFepFnKeyManager::EFnKeyNone )
            {
            SetFlag(EFlagLongKeyPressHandled); // don't allow any more long presses to go through.
            iLongPressedScanCode = aKeyEvent.iScanCode;
            }
#endif // RD_INTELLIGENT_TEXT_INPUT      
        return response;
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT		
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__ 
        // key was not handled because there is no space in editor
    else if( !iFepMan.EditorHasFreeSpace() && ( iFepMan.KeyboardLayout() != EPtiKeyboard12Key))
    	{		
    	SetFlag(EFlagBlockAllLongKeyPressEvents);
    	}
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__          			
#endif // RD_INTELLIGENT_TEXT_INPUT    
	if (iFepMan.IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed))
		{
		// If shift (ie. edit-key) is pressed in itu-t mode, block all the numeric keys, 
		// otherwise they will be routed to edwin and prodec unwanted character.
		if ((aKeyEvent.iScanCode >= EPtiKey1 && aKeyEvent.iScanCode <= EPtiKey9) ||
		    aKeyEvent.iScanCode == EPtiKey0 || aKeyEvent.iScanCode == EStdKeyFullStop )
			{
			return EKeyWasConsumed;
			}		
		}        
		
	if (iFepMan.IsFlagSet(CAknFepManager::EFlagQwertyChrKeyDepressed))
		{
		if ((aKeyEvent.iScanCode >= EPtiKeyQwertyA && aKeyEvent.iScanCode <= EPtiKeyQwertyZ) ||
		     aKeyEvent.iScanCode == EStdKeyFullStop )
			{
			return EKeyWasConsumed;
			}		
		}        
        
    // bulk of the selection.
    switch(keyCode)
        {
    case EKeyLeftArrow: //fall through
    case EKeyRightArrow:
    case EKeyDownArrow:
    case EKeyUpArrow:
        response = iFepMan.HandleKeyEventL(keyCode, length);
        break;
    case EKeyCBA1:
        {
        TInt inputMode = iFepMan.InputMode();
        if ( iKeyCatcherState == EAknFepStateUIActive && 
            inputMode >= ECangJie && inputMode <= EStroke )
            {
            // Left softkey does not have functionality in Chinese input modes when 
            // FEP UI is active.
            // phrase creation
            iFepMan.HandleKeyEventL( keyCode, length );
            response = EKeyWasConsumed;
            break;
            }
        }
        //fall through
    case EKeyOK:
    case EKeyCBA2:
    case EPtiKey0:
    case EPtiKey1:
    case EPtiKey2:
    case EPtiKey3:
    case EPtiKey4:
    case EPtiKey5:
    case EPtiKey6:
    case EPtiKey7:
    case EPtiKey8:
    case EPtiKey9:
    case EPtiKeyStar:
    case EPtiKeyHash:
    case EKeyEscape:
        if(length == ELongKeyPress)
            {
            SetFlag(EFlagLongKeyPressHandled); // don't allow any more long presses to go through.
            iLongPressedScanCode = aKeyEvent.iScanCode;
            }
        //fall through
    case EKeyBackspace:
    case EKeyDelete:
        
        // This check is added to handle the case when user enters a consonant
        // followed by a halant, follwed by digit using long key press.
        // When backspace is done, first time, the digit gets deleted.
        // Second time, only halant should be deleted. But since ZWS is
        // present, the entire syllable gets deleted. Hence we forcefully
        // remove the ZWS.
        
        if( keyCode == EKeyBackspace && 
            iFepMan.PreviousChar( ETrue ) == ( ZERO_WIDTH_SPACE ) )
            {
            // Remove the Zero Width Space
	    	iFepMan.RemovePreviousCharacterL();
            }

#ifdef RD_MARATHI
		if( ( keyCode == EKeyBackspace ) && 
			iFepMan.PreviousChar( ETrue ) == ( ZERO_WIDTH_JOINER ))
			{
			// Remove the Zero Width Joiner
			iFepMan.RemovePreviousCharacterL();
			}
#endif // RD_MARATHI
#ifdef RD_INTELLIGENT_TEXT_INPUT
		if( keyCode == EKeyBackspace && 
			( (aKeyEvent.iModifiers & EModifierLeftShift) ||
			  (aKeyEvent.iModifiers & EModifierRightShift) ||
			  (aKeyEvent.iModifiers & EModifierShift) ))
			{
			keyCode = EKeyDelete;
			}
#endif //RD_INTELLIGENT_TEXT_INPUT			
        response = iFepMan.HandleKeyEventL(keyCode, length, aEventCode );
        break;
    case EKeyF19: // Fep simulated event to asynchronously update the case.
    case EKeyF22:
    case EKeyF23:
    case EKeyF24:
        response = iFepMan.HandleKeyEventL(keyCode, length);
        break;
    case EKeyPowerOff:
    case EKeyPhoneEnd:
    case EKeyApplication:
#ifndef RD_INTELLIGENT_TEXT_INPUT
    		// To rip off suggested word completion when user press END / APPLICATION
    			if(iFepMan.IsAutoCompleteOn())
    			{
    			iFepMan.RemoveSuggestedCompletionL();	
    			}
#endif //RD_INTELLIGENT_TEXT_INPUT
#ifdef RD_INTELLIGENT_TEXT_INPUT
		response = iFepMan.HandleKeyEventL(keyCode, length);
#endif //RD_INTELLIGENT_TEXT_INPUT
        break;
    case EKeyPhoneSend:
        // handling phone send key event in japanese input
        if (iFepMan.IsFeatureSupportedJapanese())
            {
            if (iFepMan.Japanese() && iKeyCatcherState == EAknFepStateUIActive)
                {
                // Ensure the CBA is in the default state.
                iFepMan.UpdateCbaL(NULL);
                // Close FEP UI, commit the inline edit and show the cursor.            
                iFepMan.TryCloseUiL();
                }
            }
        break;
    default:
        break;
        }
   
    return response;
    }

void CAknFepKeyCatcher::SetState(const enum TAknFepManagerState aState)
    {
    iKeyCatcherState=aState;
    }

CAknFepKeyCatcher::CAknFepKeyCatcher(CAknFepManager& aFepMan)
    : iFepMan(aFepMan),
      iKeyCatcherState(EAknFepStateNull)
    {
    }

void CAknFepKeyCatcher::ConstructL()
    {
    // Set up fep key catching control - front window, null size, non-focusing
    CreateWindowL();
    SetFocusing(EFalse);    
    RWindow& window=Window();
    window.SetOrdinalPosition(0, ECoeWinPriorityFep);
    TPoint fepControlPos(0, 0);
    SetExtent(fepControlPos, TSize(0,0));               //null size
    window.SetExtent(fepControlPos, TSize(0,0));
    window.SetNonFading(ETrue);
    if(NULL == iEikonEnv->EikAppUi())
        {
        User::Leave(KErrCancel);
        }
    iEikonEnv->EikAppUi()->AddToStackL(this, ECoeStackPriorityFep, 
                                       ECoeStackFlagRefusesFocus|ECoeStackFlagSharable);
#ifdef RD_SCALABLE_UI_V2
    (CEikonEnv::Static())->AddMessageMonitorObserverL(*this); 
#endif  //RD_SCALABLE_UI_V2
    SetBlank();
    }

TKeyResponse CAknFepKeyCatcher::HandleHashKeyJapaneseL(const TKeyEvent& aKeyEvent, 
                                                       TEventCode aEventCode)
    {
    // handling hash keypress event only
    __ASSERT_DEBUG(aKeyEvent.iScanCode==EStdKeyHash, AknFepPanic(EAknFepPanicNotSupportKey));

    // No handling case
    if (aEventCode == EEventKeyDown
     || iKeyCatcherState == EAknFepStateNull)
        {
        if (aEventCode == EEventKeyUp)
            {
            ClearFlag(EFlagLongKeyPressHandled);
            }
        return EKeyWasNotConsumed;
        }
    
    // sort out if we're doing long or short presses...
    TKeyPressLength length = aKeyEvent.iRepeats ? ELongKeyPress : EShortKeyPress;

    // long keypress is handled once, after key event always is consumed.
    if (aEventCode == EEventKey && IsFlagSet(EFlagLongKeyPressHandled) )
        {
        return EKeyWasConsumed;
        }
    
    if (length == ELongKeyPress && !IsFlagSet(EFlagLongKeyPressHandled))
        {
        SetFlag(EFlagLongKeyPressHandled);
        }
    
    // Always handling case
    if (aEventCode != EEventKeyUp && IsFlagSet(EFlagLongKeyPressHandled)
        || aEventCode == EEventKeyUp && !IsFlagSet(EFlagLongKeyPressHandled))
        {
        TKeyResponse response = iFepMan.HandleKeyEventL(EPtiKeyHash, length);
        __ASSERT_DEBUG(response != EKeyWasNotConsumed, AknFepPanic(EAknFepPanicNotHandleHashKey));
        }
    else if (aEventCode == EEventKey
          && iKeyCatcherState == EAknFepStateUIActive)
        {
        // Ensure the CBA is in the default state.
        iFepMan.UpdateCbaL(NULL);
        // Close FEP UI, commit the inline edit and show the cursor.
        iFepMan.TryCloseUiL();
        }

    if (aEventCode == EEventKeyUp)
        {
        ClearFlag(EFlagLongKeyPressHandled);
        }
    
    return EKeyWasConsumed;
    }

void CAknFepKeyCatcher::HandleResourceChange(TInt aType)
    {
    if(aType == KEikDynamicLayoutVariantSwitch)  //If the layout has been changed, notify fep manager
        {
        iFepMan.HandleResourceChange(aType);
        }

    CCoeControl::HandleResourceChange(aType);
    }
TBool CAknFepKeyCatcher::IsNaviKey(TUint aCode)
	{
	switch(aCode)
		{
			case EKeyBackspace:
			case EKeyLeftArrow:
			case EKeyRightArrow:
			case EKeyUpArrow:
			case EKeyDownArrow:
			case EKeyDelete:
				return ETrue;
			default:
				return EFalse;	
		}
	}
#ifdef RD_SCALABLE_UI_V2

void CAknFepKeyCatcher::MonitorWsMessage(const TWsEvent& aEvent)
    {
	switch (aEvent.Type())
		{
	    case EEventPointer:
            TRAP_IGNORE(iFepMan.HandlePointerEventL(*aEvent.Pointer()));
		    break;
		default:
		    break;
		}
    }

#endif //RD_SCALABLE_UI_V2 

// End of file
