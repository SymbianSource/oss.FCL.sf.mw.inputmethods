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
* Description:           Provides the TAknFepInputStateEntryIndicPredictive definition.
*
*/











#ifndef __AKN_FEP_UI_INPUT_STATE_ENTRY_INDIC_PREDICTIVE_H__
#define __AKN_FEP_UI_INPUT_STATE_ENTRY_INDIC_PREDICTIVE_H__

#include "AknFepUiInputStateEntryWesternPredictive.h"

/**
*  This class handles the entry predictive state for Indic languages.
*  @lib avkonfep.dll
*  @since Series 60 3.1
*/
class TAknFepInputStateEntryIndicPredictive : public TAknFepInputStateEntryWesternPredictive
    {
public:
    /**
    * 
    * Constructor for TAknFepInputStateEntryIndicPredictive.	
    * @since 3.1M
    * @return 
    */
    TAknFepInputStateEntryIndicPredictive(MAknFepUIManagerStateInterface* aOwner, 
                                          TLanguage aLanguage);
    void LastWordInSelectionList();
    };

#endif //__AKN_FEP_UI_INPUT_STATE_ENTRY_INDIC_PREDICTIVE_H__
// End of File
