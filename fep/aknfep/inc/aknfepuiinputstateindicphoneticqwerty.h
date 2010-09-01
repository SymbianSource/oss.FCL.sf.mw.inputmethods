/*
* Copyright (c) 2006 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the TAknFepInputStateIndicPhoneticQwerty definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INDIC_PHONETIC_QWERTY_H__
#define __AKN_FEP_UI_INPUT_STATE_INDIC_PHONETIC_QWERTY_H__

#include <w32std.h>
#include <aknfepuiinputstateqwerty.h>
class TAknFepInputStateIndicPhoneticQwerty : public TAknFepInputStateQwerty
    {
public:
	/**
         * Class Constructor.
         * 
         * @since S60 4.0
         * @param UI Manager.
         * Engine.
         * @return Nothing
    */
         
    TAknFepInputStateIndicPhoneticQwerty(MAknFepUIManagerStateInterface* aOwner); 

	/**
         * Interface fuction from MAknFepManagerInterface, Used for handling events passes by FEP manager.
         * 
         * @since S60 4.0
         * @param aKey- key code of the event.
         * @param aLength- Signifies short key press or long key press.
         * Engine.
         * @return returns EKeyWasConsumed if the key event is handled else EKeyWasNotConsumed
    */

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);

	/**
         * Removes Halant added by Phonetic PtiENgine for North Indian Languages.
         * 
         * @since S60 4.0
         * @param aLanguage- Current Phonetic input language is passed to this function.
         * Engine.
         * @return Nothing
    */
    
	void RemoveHalantL(TLanguage aLanguage);
	
	/**
         * Checks wheather the current Phonetic Input Language follows North Indian style of editing. This is necessary for removal
         * of Halant.
         * @since S60 4.0
         * @param aLanguage- Current Phonetic input language is passed to this function.
         * Engine.
         * @return Returns True if the current language comes under North Indian style of writting.
    */
	TBool IsToRemoveHalantForLanguage(TInt aLanguage);

private:
	
	/**
         * Checks wheather key code is valid QWERTY key for Phonetic FEP.
         * 
         * @since S60 4.0
         * @param aKey- key code of the event.
         * Engine.
         * @return Returns ETrue if the key is valid Phonetic QWERTY Key.
    */
    
    TBool IsQwertyKey(const TInt aKey);

    };

#endif __AKN_FEP_UI_INPUT_STATE_INDIC_PHONETIC_QWERTY_H__

// End of file
