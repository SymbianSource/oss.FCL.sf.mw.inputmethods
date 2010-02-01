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
* Description:            Provides the TAknFepInputStateInitialIndicMultitap definition.
*
*/











#ifndef __AKN_FEP_UI_INPUT_STATE_INIT_INDIC_MULTITAP_H__
#define __AKN_FEP_UI_INPUT_STATE_INIT_INDIC_MULTITAP_H__

#include "AknFepUiInputStateInitialMultitapBase.h"
#include "AknFepUiIndicEnums.h"
/**
*  This class  handles the multitap input for Indic languages.
*  @lib avkonfep.dll
*  @since Series 60 3.1
*/
class TAknFepInputStateInitialIndicMultitap : public TAknFepInputStateInitialMultitapBase
    {
public:
    /**
    * 
    * Constructor for TAknFepInputStateInitialIndicMultitap.	
    * @since 3.1M
    * @return 
    */
    TAknFepInputStateInitialIndicMultitap(MAknFepUIManagerStateInterface* aOwner, 
                                          TLanguage aLanguage); 
public: //from TAknFepInputStateBase    
    /**
    * 
    * Returns the virama character depending on the input language.	
    * @since 3.1M
    * @return Etrue if the Key is handled. 
    */

    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    
public: //from MptiObserver
    /**
    * 
    * Gets called when PTI Engine's timer expires.	
    * @since 3.1M
    * @return 
    */

	void KeyTimerExpired();

    /**
	 * This function handles Indic key response codes
	 *
	 * @since 3.2
	 * @param TInt The key code.
	 * @param TIndicInputResponse the key response.
	 * @param TPtrC Text obtained from PtiEngine
	 * @return TBool
	 */
    TBool HandleIndicKeyResponseL( TInt aKey, TIndicInputResponse aKeyResponse, TPtrC aPtiText );

    /**
	 * This function checks if space is left in a limited space editor.
	 *
	 * @since 3.2
     * @param TIndicInputResponse the key response.
	 * @return TBool
	 */
    TBool IsFreeSpaceAvailable( TIndicInputResponse aKeyResponse );

#ifdef RD_MARATHI
    /**
	 * This function handles Marathi specific code.
	 *
	 * @since 5.0
	 * @return void
	 */
    void HandleKeyMarathiL();
#endif // RD_MARATHI	
	};


#endif //__AKN_FEP_UI_INPUT_STATE_INIT_INDIC_MULTITAP_H__

// End of File
