/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides CAknFepManager definition
*
*/












#ifdef _DEBUG
#include <eikenv.h>
#endif

#include <fepbase.h>
#include <avkon.hrh>
#include <aknedsts.h>               //CAknEdwinState

#include "AknFepFnKeyManager.h"
#include "AknFepManager.h"
#include <centralrepository.h>
#include <AknFepInternalCRKeys.h>
#include "PtiEngine.h"


/*this part is removed 
const   TUid    KPhoneUidAppPhone = { 0x100058B3 };
const   TUid    KUidCalc = { 0x10005902 };
const   TUid    KUidIdle = { 0x101fd64c };
*/
const   TStdScanCode stdFnKeyCode(EStdKeyRightFunc);    // Defines the std key code for Fn key

////////////////////////////////////////////////////////////
//   CAknFepFnKeyManager implementation
//////////////////////////////////////////////////////////// 
CAknFepFnKeyManager::CAknFepFnKeyManager( CAknFepManager& aFepMan, 
                    CAknFepSharedDataInterface* aSharedDataInterface)
:iFepMan( aFepMan ),
 iFnKeyState( EFnKeyNone ),
 iIsQwertyMode (EFalse),
 iSharedDataInterface(aSharedDataInterface),
 iShiftKeyPressed( EFalse )
    {
    iIsQwertyMode = iSharedDataInterface->QwertyInputMode();
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
    iWasPreviousModePredictive = iFepMan.WesternPredictive();
#endif // RD_INTELLIGENT_TEXT_INPUT
#else   
    iWasPreviousModePredictive = iSharedDataInterface->PredictiveTextOn();
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__     
    // The CR key KAknFepFnKeyState is reset the Fn key state NONE
    iSharedDataInterface->SetFnKeyState(0);
    }

CAknFepFnKeyManager* CAknFepFnKeyManager::NewL( CAknFepManager& aFepMan,
                                            CAknFepSharedDataInterface* aSharedDataInterface)
    {
    CAknFepFnKeyManager* self = new(ELeave) CAknFepFnKeyManager( aFepMan,aSharedDataInterface );
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop();
    return self;
    }

CAknFepFnKeyManager::~CAknFepFnKeyManager()
    {    
    iFnKeyState=EFnKeyNone;
    }
    
void CAknFepFnKeyManager::ConstructL()
    {    
    }

void CAknFepFnKeyManager::SetQwertyInputMode(TBool aQwertyState)
    {
    iIsQwertyMode = aQwertyState;
    }
void CAknFepFnKeyManager::ClearFnKeyState()
    {
    iFnKeyState = EFnKeyNone;
    iFepMan.UpdateIndicators();    
    }

CAknFepFnKeyManager::TFnKeyState CAknFepFnKeyManager::FnKeyState()
    {
    return iFnKeyState;
    }

void CAknFepFnKeyManager::SetFnKeyState(CAknFepFnKeyManager::TFnKeyState aState)
    {
    TPtiKeyboardType keyboardType = iFepMan.KeyboardLayout();
    //This function is only supported for EFnKeyNext and half Qwerty keypad at the moment.
    if(keyboardType == EPtiKeyboardHalfQwerty && EFnKeyNext == iFnKeyState &&
        aState == EFnKeyNone)
        {    
        
        //if(aState == EFnKeyNext && keyboardType == EPtiKeyboardHalfQwerty)
        //	{
        //	iFepMan.PtiEngine()->CancelTimerActivity();
        //	}
        iFnKeyState = aState;
        iFepMan.SetCase( (TCase)iPreviousCase );
        iFepMan.UpdateIndicators();
        }
#ifdef RD_INTELLIGENT_TEXT_INPUT  
    else if(keyboardType == EPtiKeyboardQwerty3x11 && EFnKeyNone == iFnKeyState &&
        aState == EFnKeyLock)
        {
        UpdatePreviousCase();
        iFnKeyState = aState;
        iFepMan.SetCase(EFnKeyLowerCase);
        iFepMan.UpdateIndicators();
        }
#endif // RD_INTELLIGENT_TEXT_INPUT 		
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__  
#ifdef RD_INTELLIGENT_TEXT_INPUT  
    //Half QWERTY state uses this to change from EFnKeyNext aFnKeyNone
 	   else if ( iFnKeyState == EFnKeyNone && aState == EFnKeyNext )
        {
        // QWERTY long press implementation uses this function to set the Fn state.
        UpdatePreviousCase();
        iFnKeyState=EFnKeyNext;
        }

#endif // RD_INTELLIGENT_TEXT_INPUT  
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
     // TODO: above piece of code should be simplify.    
     // Set the Fn Key state  
 	 else
 	    {
 	    iFnKeyState = aState;    
 	    }
    }
TKeyResponse CAknFepFnKeyManager::HandleFnKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType, 
                                                     TCoeInputCapabilities aInputCapabilities ) 
    {
    
    TKeyResponse keyResponse = EKeyWasNotConsumed;
    TPtiKeyboardType keyboardType = iFepMan.KeyboardLayout();

    if(!iIsQwertyMode)
        {
        return keyResponse;
        }
        
  	if(!iFepMan.IsFlagSet(CAknFepManager::EFlagShiftKeyDepressed) && 
      !iFepMan.IsFlagSet(CAknFepManager::EFlagLongShiftKeyPress)
      && iShiftKeyPressed )
        {
        iShiftKeyPressed = EFalse;
        }
                
    TInt caseFromFep = iFepMan.GetCurrentCase();
    
    TBool lockLater = EFalse;    
    if(  EEventKeyDown == aType )
        {
        if( stdFnKeyCode == aKeyEvent.iScanCode )
    		{
            iFepMan.PtiEngine()->CancelTimerActivity();
    		if( iPreviousKeyWasFnKey )
    			{
    			lockLater	= ETrue;
    			}
    		iPreviousKeyWasFnKey = ETrue; 			
    		}    
    	else
    		{
    		iPreviousKeyWasFnKey = EFalse; 				
    		}        
        }
    if(aKeyEvent.iRepeats && stdFnKeyCode != aKeyEvent.iScanCode &&
            iFnKeyState == EFnKeyNext)
        {
        // To fix the bug : Long press of number at the begin of editor. Then enter any character. 
        // The character is shown in upper case.
        if (ELowerCase == caseFromFep)
            {
            iFepMan.SetCase( (TCase)caseFromFep );
            }
        // To Fix Error: Character is NOT entered as per the input mode
        // when the character is entered after pressing Shift + Fn + Shift keys
        else if(!(EStdKeyRightShift == aKeyEvent.iScanCode ||
        		  EStdKeyLeftShift == aKeyEvent.iScanCode))
	        {
            iFepMan.SetCase( (TCase)iPreviousCase );	
            }
            
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT  
         // For QWERTY Predictive ---->
        if(iWasPreviousModePredictive)
        	{
        	// We usually end up here when using the long key press to enter Fn symbols
			iFepMan.SetWesternPredictive(ETrue); 
		   	iFepMan.TryChangeModeL(ELatin);               	
        	}
        // For QWERTY Predictive <----

#endif // RD_INTELLIGENT_TEXT_INPUT  
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__    
        iFnKeyState = EFnKeyNone;
        iFepMan.UpdateIndicators();
        }
     
    if( stdFnKeyCode == aKeyEvent.iScanCode )
        {
        // Consume the function key events.
        keyResponse = EKeyWasConsumed;
        }
    if (iFepMan.IsHybridAplhaEditor())
        keyResponse = EKeyWasNotConsumed;
 
    
    // check for the case when in Half Qwerty and both shift and fn keys are
    // pressed and fn key is released while shift is still down
    if( EEventKeyUp == aType && 
    	stdFnKeyCode == aKeyEvent.iScanCode &&
    	iShiftKeyPressed &&
    	EPtiKeyboardHalfQwerty == keyboardType )
    	{
    	ClearFnKeyState();
    	}
    	    
    if( EStdKeyRightShift == aKeyEvent.iScanCode ||  EStdKeyLeftShift == aKeyEvent.iScanCode )
        {
        if( EEventKeyDown == aType ) 
            {
            iShiftKeyPressed = ETrue;
            }
        if( EEventKeyUp == aType )
            {
            iShiftKeyPressed = EFalse;
            }
        }
        
    // For Shift key handling.
    // Shift Key should be ignored in locked state.
     
    if( ( EFnKeyNext == iFnKeyState ) 
    && EEventKeyDown == aType &&
    ( EStdKeyRightShift == aKeyEvent.iScanCode ||  EStdKeyLeftShift == aKeyEvent.iScanCode)) 
        {
        // Fnkey event+ shift key event. Change to FnKey Upper case
        iFepMan.SetCase( EFnKeyUpperCase );
        if( keyboardType == EPtiKeyboardHalfQwerty )
            {
            iFepMan.SetCase( EFnKeyLowerCase );
            }
        return EKeyWasNotConsumed;
        }
    //===================================================		
	
	// GQF clarification Fn+Shift
	// If shift key is hold on, So fn key state should be EFnKeyNext
	// So next text mode goes to function upper
	if( aType == EEventKeyUp && iShiftKeyPressed &&  stdFnKeyCode == aKeyEvent.iScanCode)	
		{
		if( EFnKeyPressed == iFnKeyState)
			{
			iFepMan.PtiEngine()->CancelTimerActivity();
			iFnKeyState=EFnKeyNext;
			iFepMan.SetCase( EFnKeyUpperCase );				
			}			
        else if (EFnKeyPressedAgain == iFnKeyState )
            {
            CEikonEnv::Static()->InfoMsg(_L("iFnKeyState = EFnKeyLock"));
            iFnKeyState = EFnKeyLock;	                
            UpdatePreviousCase();
            iFepMan.SetCase( EFnKeyUpperCase );
            }
        else if(EFnKeyDown == iFnKeyState)
            {
            iFnKeyState = EFnKeyNone;
            UpdatePreviousCase();
            iFepMan.SetCase( EUpperCase );	
            }
		iFepMan.UpdateIndicators();
		}
	// Holding shift key, press key just after releasing fn key,
	// Fn key state goes to EFnKeyNone mode.
	if( aType == EEventKeyUp && iShiftKeyPressed && iFnKeyState==EFnKeyNext && stdFnKeyCode != aKeyEvent.iScanCode &&  !(EStdKeyRightShift == aKeyEvent.iScanCode ||  EStdKeyLeftShift == aKeyEvent.iScanCode))
		{
		iFnKeyState=EFnKeyNone;		
		iFepMan.SetCase( EUpperCase );		
		iFepMan.UpdateIndicators();
		}
		
	//====================================================
		
    // Fnkey event+ shift key event -> when fn and shift key are kept pressed.
    if( (iShiftKeyPressed && EFnKeyNone != iFnKeyState ) )
        {
        if( (EFnKeyPressed == iFnKeyState) &&
        	(stdFnKeyCode != aKeyEvent.iScanCode) &&
        	!( EStdKeyRightShift == aKeyEvent.iScanCode ||  EStdKeyLeftShift == aKeyEvent.iScanCode ))
	        {
     	   	CEikonEnv::Static()->InfoMsg(_L("iFnKeyState = EFnKeyDown"));
          	iFnKeyState=EFnKeyDown;	      	
	        }
        
        // Thre is no Functin shift reverse mode
        // So, In shift key press if fn key state is not none
        // text case stte is function upper.
        if( aType == EEventKeyDown && stdFnKeyCode == aKeyEvent.iScanCode)
        	{
        	if ( EFnKeyNext == iFnKeyState )
        		{       	     	
       	     	if(lockLater)
       	     	    {
       	     	    //CEikonEnv::Static()->InfoMsg(_L("iFnKeyState = EFnKeyPressedAgain "));
       	     	    iFnKeyState=EFnKeyPressedAgain;
       	     	    } 					
        		}
        	else if ( EFnKeyLock == iFnKeyState )
	        	{
       	     	//CEikonEnv::Static()->InfoMsg(_L("iFnKeyState = EFnKeyDown"));
              	iFnKeyState=EFnKeyDown;
	        	}
        	}
        iFepMan.SetCase( EFnKeyUpperCase );
        if( keyboardType == EPtiKeyboardHalfQwerty )
            {
            iFepMan.SetCase( EFnKeyLowerCase );
            }
        iFepMan.UpdateIndicators();
        return EKeyWasNotConsumed;
        }

    // Check the input mode of the currently focused editor (when available)
    MCoeFepAwareTextEditor* fepAwareTextEditor = aInputCapabilities.FepAwareTextEditor();
    
    if ( fepAwareTextEditor && fepAwareTextEditor->Extension1() )
        {
        if( EAknEditorNumericInputMode == 
            ( static_cast<CAknEdwinState*>( fepAwareTextEditor->Extension1()->State( KNullUid ) ) )->CurrentInputMode() )
            // If number input mode is detected, handle it as number input
            // capability with greatest available variation of letters
            {
            // Use EDialableCharacters since it provides most flexibility for number input
            aInputCapabilities.SetCapabilities( TCoeInputCapabilities::EDialableCharacters );
            }            
        }
        
    if ( EFnKeyForced != iFnKeyState )
        {
        // Handling of Fn key events.
        // If Fn key is held down while characters are typed, all following key 
        // events are exchanged.
        // If Fn key is only pressed (key down + key up, not held down), only
        // the very next key event is exchanged.
        // If the next key event after Fn key is another Fn key event, the 
        // Fn lock mode is activated. Then all following key events are
        // exchanged until the Fn key is pressed again.       
         
        if( EEventKeyDown == aType && stdFnKeyCode == aKeyEvent.iScanCode )
            {
            if ( EFnKeyNext == iFnKeyState )
                {                
                if(lockLater)
            		{
            		CEikonEnv::Static()->InfoMsg(_L("iFnKeyState=EFnKeyPressedAgain"));	
            		iFnKeyState=EFnKeyPressedAgain;
          			}
              	else
              	    {
              	    iFnKeyState=EFnKeyPressed;
              	    }		              			
                }
            else if ( EFnKeyLock == iFnKeyState )
                {
                CEikonEnv::Static()->InfoMsg(_L("iFnKeyState=EFnKeyDown"));
                iFnKeyState=EFnKeyDown;
                }
            //else 
            // Sahoo
            else if ( EFnKeyDown != iFnKeyState )
                {
                CEikonEnv::Static()->InfoMsg(_L("iFnKeyState=EFnKeyPressed"));
                iFnKeyState=EFnKeyPressed;
                }
            iFepMan.UpdateIndicators();    
            return keyResponse;
            }
        // Sahoo            
        if( EEventKeyDown == aType && EFnKeyPressed == iFnKeyState )
            {
            iFnKeyState=EFnKeyDown;
            iFepMan.SetCase( EFnKeyLowerCase );
            }
#ifdef __WINS        
        // NOTE: This following HACK is to ensure working in emulator environment,
        // where Alt Gr key seems to send always two separate key events:
        // EStdKeyRightFunc and EStdKeyLeftCtrl
        if ( EStdKeyLeftCtrl == aKeyEvent.iScanCode )
            {
            return;
            }
        // HACK ends
#endif
        
        if( EEventKeyUp == aType && stdFnKeyCode == aKeyEvent.iScanCode )
            {
            if ( EFnKeyPressedAgain == iFnKeyState )
                {
               	if(! iFepMan.IsFlagSet(CAknFepManager::EFlagQwertyShiftMode))
	               	{
	                CEikonEnv::Static()->InfoMsg(_L("iFnKeyState = EFnKeyLock"));
	                iFnKeyState = EFnKeyLock;
		           	}
               	else
	               	{
	                CEikonEnv::Static()->InfoMsg(_L("iFnKeyState = EFnKeyNext"));
	                iFepMan.PtiEngine()->CancelTimerActivity();
	                iFnKeyState = EFnKeyNext;               		
	               	}
                UpdatePreviousCase();
                iFepMan.ClearFlag(CAknFepManager::EFlagQwertyShiftMode);             
                iFepMan.SetCase( EFnKeyLowerCase );
                }
            else if ( EFnKeyPressed == iFnKeyState )
                {
                CEikonEnv::Static()->InfoMsg(_L("iFnKeyState = EFnKeyNext"));
                iFepMan.PtiEngine()->CancelTimerActivity();
                iFnKeyState = EFnKeyNext;
                
                UpdatePreviousCase();
                
                // After pressing shift key Press Fn key,
                // So its goes to function upper mode
                if( iFepMan.IsFlagSet(CAknFepManager::EFlagQwertyShiftMode))
                	iFepMan.SetCase( EFnKeyUpperCase );
                else
                    iFepMan.SetCase( EFnKeyLowerCase );
                }
            else
                {
                CEikonEnv::Static()->InfoMsg(_L("iFnKeyState = EFnKeyNone"));
                
                // For Shift key handling.
                if( EFnKeyDown == iFnKeyState || EFnKeyNext == iFnKeyState )
                    {
                    iFepMan.SetCase( (TCase)iPreviousCase );
                    }
                iFnKeyState = EFnKeyNone;
                }
            if( EEventKeyUp == aType && iFnKeyState == EFnKeyNone )
                {
                // To fix the bug :Fn key lock. then type a number with short press at begin of 
                // editor. Now press Fn key to go to Fn none state. 
                // Press a character. The char is in upper case
                if (ELowerCase == caseFromFep)
		            {
		            iFepMan.SetCase( (TCase)caseFromFep );
		            }
		        // To Fix Error: Character is NOT entered as per the input mode
		        // when the character is entered after pressing Shift + Fn + Shift keys
		        else if(!(EStdKeyRightShift == aKeyEvent.iScanCode ||
		            	  EStdKeyLeftShift == aKeyEvent.iScanCode))
		            {
		            iFepMan.SetCase( (TCase)iPreviousCase );	
		            }
                }
            iFepMan.UpdateIndicators();
            return keyResponse;
            }
        }
                
    // Now detect the input mode for telephone and calculator apps
/*    if ( aInputCapabilities.IsNone() )
        {   // Idle and Calculator do not provide input capabilities at all.
        if ( KPhoneUidAppPhone == aCurrentTopAppUid || KUidIdle == aCurrentTopAppUid )
            {
            aInputCapabilities.SetCapabilities( TCoeInputCapabilities::EDialableCharacters );
            }
        if ( aCurrentTopAppUid == KUidCalc )
            {
            aInputCapabilities.SetCapabilities( TCoeInputCapabilities::EWesternNumericReal );
            }
        }        
*/        
    // Now handle the automatic number mode
    if ((iFnKeyState != EFnKeyForced) && 
        ( aInputCapabilities.SupportsWesternNumericIntegerPositive() ||
         aInputCapabilities.SupportsWesternNumericIntegerNegative() ||
         aInputCapabilities.SupportsWesternNumericReal() ||
         aInputCapabilities.SupportsDialableCharacters() )         
       )
        {
        // If input mode is numeric, Fn key is automated (=forced)
        iFnKeyState = EFnKeyForced;
        
        UpdatePreviousCase();
        
        iFepMan.SetCase( EFnKeyLowerCase );
        iFepMan.UpdateIndicators();
        //Forced input mode
        // The CR key KAknFepFnKeyState set to Locked
        iSharedDataInterface->SetFnKeyState(EFnKeyForced/*=6*/);        
        CEikonEnv::Static()->InfoMsg(_L("iFnKeyState = EFnKeyForced"));   
        }
#ifdef __REVERSE_FN_KEY_SUPPORTED
    //Reverse Fn Key mapping!
    //Just one of the cases for Reverse Fn key mapping!
    // There are manu more of these!
    if((EFnKeyForced == iFnKeyState && aKeyEvent.iModifiers & EModifierRightFunc ) )
    	{    	
    	iFepMan.SetCase( (TCase)iPreviousCase );    	
    	iFepMan.SetReverseFnkeyInputMode(ETrue);    	
    	}
    else if(iFepMan.IsReverseFnkeyInput())
    	{
    	iFepMan.SetReverseFnkeyInputMode(EFalse);           
        iFepMan.SetCase(EFnKeyLowerCase);
    	}
#endif //__REVERSE_FN_KEY_SUPPORTED
        
    // If any of the following states apply, the key event should be exchanged.        
    if ( EFnKeyPressed == iFnKeyState ||
         EFnKeyDown == iFnKeyState ||
         EFnKeyNext == iFnKeyState ||
         EFnKeyPressedAgain == iFnKeyState ||
         EFnKeyLock == iFnKeyState ||
         EFnKeyForced == iFnKeyState )
        {
        // For telephone number editor key events for w, p and + must be accepted as is
        if( aInputCapabilities.SupportsDialableCharacters() && ( 'W' == aKeyEvent.iScanCode ||
                                                                 'P' == aKeyEvent.iScanCode ||
                                                                 '+' == aKeyEvent.iScanCode ||
                                                                 'w' == aKeyEvent.iCode ||
                                                                 'p' == aKeyEvent.iCode ||
                                                                 '+' == aKeyEvent.iCode) )
            {
            return keyResponse;
            }
            
        if( EEventKeyUp == aType )
            {
            if ( EFnKeyPressed == iFnKeyState || EFnKeyPressedAgain == iFnKeyState )
                {
                CEikonEnv::Static()->InfoMsg(_L("iFnKeyState = EFnKeyDown"));
                iFnKeyState = EFnKeyDown;                
                UpdatePreviousCase();
                
                // Fn key hold on and press any key
                if( iFepMan.IsFlagSet(CAknFepManager::EFlagQwertyShiftMode))
                	iFepMan.SetCase( EFnKeyUpperCase );
                else
                	iFepMan.SetCase( EFnKeyLowerCase );
                }
                
             // Debasish MT error fixing
            else if( EFnKeyNext == iFnKeyState && iFepMan.IsFlagSet(CAknFepManager::EFlagQwertyShiftMode))
	            {
	            // keep the sate as it is
	            UpdatePreviousCase();	
	            iFepMan.SetCase( EFnKeyUpperCase );
	            
	            if( keyboardType == EPtiKeyboardHalfQwerty )
                    {
                    iFepMan.SetCase( EFnKeyLowerCase );
                    }
	            }
            else if ( EFnKeyNext == iFnKeyState )
                {
                //This has to be done if the keyboard type is not half Qwerty
                if(keyboardType == EPtiKeyboardHalfQwerty)
                    return keyResponse;
                CEikonEnv::Static()->InfoMsg(_L("iFnKeyState = EFnKeyNone"));
                
                // To fix the bug : At begin of editor, press Fn key. Now press a number.
                // Now state is FnKeyNone. Type any char. The case is upper.
                // For Shift key handling.
                if( EFnKeyDown == iFnKeyState || EFnKeyNext == iFnKeyState )
                    {
		 			if (ELowerCase == caseFromFep)
	            		{
	            		iFepMan.SetCase( (TCase)caseFromFep );
	            		}
			        // To Fix Error: Character is NOT entered as per the input mode
			        // when the character is entered after pressing Shift + Fn + Shift keys
		 			else if(!(EStdKeyRightShift == aKeyEvent.iScanCode ||  
		 						  EStdKeyLeftShift == aKeyEvent.iScanCode))
	            		{
	            		iFepMan.SetCase( (TCase)iPreviousCase );	
	            		}
                    }
                 iFnKeyState = EFnKeyNone;
                }
            iFepMan.UpdateIndicators();
            }                        
        }

    return keyResponse;
    }
  
void CAknFepFnKeyManager::UpdatePreviousCase()
    {
    iPreviousCase = iFepMan.GetCurrentCase();
    }
// End of File

