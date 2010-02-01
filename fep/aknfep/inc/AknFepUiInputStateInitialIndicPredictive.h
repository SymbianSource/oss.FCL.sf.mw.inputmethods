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
* Description:            Provides the TAknFepInputStateInitialIndicPredictive definition.
*
*/












#ifndef __AKN_FEP_UI_INPUT_STATE_INITIAL_INDIC_PREDICTIVE_H__
#define __AKN_FEP_UI_INPUT_STATE_INITIAL_INDIC_PREDICTIVE_H__

#include "AknFepUiInputStateInitialMultitapBase.h"


/**
*  This class handles the initial predictive state for Indic languages.
*  @lib avkonfep.dll
*  @since Series 60 3.1
*/

class TAknFepInputStateInitialIndicPredictive : public TAknFepInputStateInitialMultitapBase
    {
public:
	/**
    * 
    * Constructor for TAknFepInputStateInitialIndicPredictive.	
    * @since 3.1M
    * @return 
    */
    TAknFepInputStateInitialIndicPredictive(MAknFepUIManagerStateInterface* aOwner, 
                                            TLanguage aLanguage);

public: // from TAknFepInputStateBase 
    /**
    * 
    * Returns the virama character depending on the input language.	
    * @since 3.1M
    * @return Etrue if the Key is handled. 
    */
  
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength);
    };

#endif //__AKN_FEP_UI_INPUT_STATE_INITIAL_INDIC_PREDICTIVE_H__
//End of File