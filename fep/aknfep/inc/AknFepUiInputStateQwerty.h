/*
* Copyright (c) 2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateQwerty definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_QWERTY_H__
#define __AKN_FEP_UI_INPUT_STATE_QWERTY_H__

#include <w32std.h>
#include "AknFepUiInputStateBase.h"

class TAknFepInputStateQwerty : public TAknFepInputStateBase
    {
public:
    TAknFepInputStateQwerty(MAknFepUIManagerStateInterface* aOwner); 

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    TBool IsNumberKey(const TInt aKey);

public: // Functions from MPtiObserver interface
    void KeyTimerExpired();

protected:
    TBool IsCharacterKey(const TInt aKey);
    TBool IsQwertyKey(const TInt aKey);
    
//	TBool IsAllowedKeymappingForNumberMode(TChar aChar);    
	void NumericCharacterForQwertyKey(TInt aInScanCode,
                                      TKeyEvent& aEvent,
                                      TBool aShifted,
                                      TChar& aResult);     
	TBool IsExceptionChar(TUint16 aChar);
                                         
                                      
	/**
	* There are currently handled as a herdcoded special cases. 
	* This method is used for collecting and handling those cases.
	*/                                      
	void CustomFilter(TPtrC aInput, TDes& aOutput, TInt aLanguage, 
                      TInt aCurrentCase, TBool aShiftDown);                                      
	void HandleCaseChange(TInt aKey, TBool aIsCharacter);
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TBool IsCharacterNumber(TChar aChar);
    TBool TryGetValidCharForKey(const TInt& aKey, TChar& aChar);
    TBool TryGetValidCharForKeyAndCase(const TInt& aKey, TChar& aChar, TPtiTextCase aCase);
#endif
    TBool CheckForValidNumberEntryForqwerty(TInt aKey);  	  	                                        
private:
	//TBool iFakeDelete;  	                                        
	//TBool iCharKeyException;	 
#ifdef RD_INTELLIGENT_TEXT_INPUT
    
    //IsLanguageSupportsCaseChanges() should return ETrue if the current input language
    // does not support case changes. Eg - Arabic, Urdu, Farsi, Hindi, etc. 
    //It should return EFalse for other languages (eg - latin based languages)
    TBool IsLanguageWithoutCaseChanges();
    
    //IsSpecialNonLatinScript() should return ETrue if the current input language uses
    // either Greek, Cyrillic or Hebrew script. Currently supported languages with Cyrillic 
    // script are Russian, Ukrainian and Bulgarian.
    TBool IsSpecialNonLatinScript();
    
#endif
    };

#endif //__AKN_FEP_UI_INPUT_STATE_QWERTY_H__

// End of file
