/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the MAknFepUIManagerStateInterface definition.
*
*/












#ifndef __AKN_FEP_UI_MAN_STATE_INTERFACE_H__
#define __AKN_FEP_UI_MAN_STATE_INTERFACE_H__

// INCLUDES
#include <e32std.h>
#include <badesca.h> //CDesCArrayFlat

#include "AknFepGlobalEnums.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class MAknFepManagerUIInterface;
class CPtiEngine;
class CAknFepCaseManager;

//the follow class for phrase creation
class MZhuyinKeyHandler;
class CAknFepZhuyinAnalyser;

/**
 *  CAknFepUIManagerBase class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 *
 */
class MAknFepUIManagerStateInterface
    {
public: // New Functions
    /**
     * 
     * 
     * @since 2.6
     */
    virtual TUIState State() const = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual TUIState ChangeState(TUIState) = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual MAknFepManagerUIInterface* FepMan() const = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual CPtiEngine* PtiEngine() const = 0;

    /**
     * zhuyin phrase creation
     * 
     * @since 5.0
     */
    virtual MZhuyinKeyHandler* ZhuyinKeyHandler() = 0;

    /**
     * zhuyin phrase creation
     * 
     * @since 5.0
     */
    virtual CAknFepZhuyinAnalyser* ZhuyinAnalyser() = 0;

    
    /**
     * 
     * 
     * @since 2.6
     */
    virtual TBool IsValidChineseInputKey(TInt aKey) const = 0;

    /**
     * 
     * 
     * @since 3.0
     */
    virtual TBool IsValidChineseInputKeyQwerty(TInt aKey) const = 0;

    /**
     * 
     * 
     * @since 3.0
     */
    virtual TBool IsQwertyZhuyinToneMarkKey(TInt aKey) const = 0;
    
    /**
     * 
     * 
     * @since 2.6
     */
    virtual TLanguage GetLanguage() const = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual TPtr GetLatestDeliberateSelection() const = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual CAknFepCaseManager* CaseMan() const = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual TInt ThaiSCTResourceId(TUint aPrewChar, TInt aKey) = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual CDesCArrayFlat* CandidateArray() = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual TBool IsPredictive() const = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual void SetInlineEditUnderlineVisibility(TBool aFlag) = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual const TDesC& NumberModeKeyMapping() const = 0;

    /**
     * Check Qwerty keypad
     * 
     * @since 3.0
     */
    virtual TBool IsQwerty() const = 0;
    
    /**
     * Check chinese symbol
     */
    virtual TBool IsValidChineseSymbol(TInt aKey) const =0;

    };

#endif

// End of file
