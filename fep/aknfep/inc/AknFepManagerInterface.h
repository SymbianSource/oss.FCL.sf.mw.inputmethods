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
* Description:            Declaration of core part of CAknFepLanguageManager class.
*
*/












#ifndef __AKN_FEP_MANAGER_INTERFACE_H__
#define __AKN_FEP_MANAGER_INTERFACE_H__

// INCLUDES
#include <e32std.h>
#include <w32std.h>
#include <badesca.h>
#include <txtfrmat.h>
#include <eikon.hrh>                    //TAknEditorNumericKeymap
#include <peninputcmdparam.h>
#include "AknFepGlobalEnums.h"

#ifdef RD_INTELLIGENT_TEXT_INPUT
#include <PtiDefs.h>
#endif
// CONSTANTS

// CLASS DECLARATION

/**
* MAknFepManagerInterface is a part of Japanese FEP.
* 
* 
* @see CAknFepManager, other application
*/
class MAknFepManagerInterface
    {
    public: // Constructors and destructor
        virtual ~MAknFepManagerInterface(){}

public: // New Functions
    /**
     * 
     * 
     * @since 2.6
     */
#ifdef RD_INTELLIGENT_TEXT_INPUT          
    virtual void SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode, 
                         TInt aKeyboardType = EPtiKeyboardNone) = 0;
#else
    virtual void SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode) = 0;
#endif    

    /**
     * 
     * 
     * @since 2.6
     */
    virtual void SetCase(TCase aCase) = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual void SetNumberModeKeyMappingL(TAknEditorNumericKeymap aAknEditorNumericKeymap) = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual TBool HandleKeyL(TInt aKey, TKeyPressLength aLength, TEventCode aEventCode = EEventKey) = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual void HandleCommandL(TInt aCommandId) = 0;
    virtual void HandleCommandL(TInt aCommandId, TInt aParam) = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual void CloseUI() = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual void ExpireMultitapTimer() = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual TBool IsValidNumericLongKeyPress(TInt aKey) const = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual void AddTextToUserDictionaryL(const TDesC& aText) = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual void GetFormatOfFepInlineText(TCharFormat& aFormat, 
                                          TInt& aNumberOfCharactersWithSameFormat, 
                                          TInt aPositionOfCharacter) const = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual TInt SupportLanguage(TInt aMode) const = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual void SetInputLanguageL(TLanguage aLanguage) = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual TBool IsValidShiftKeyPress() const = 0;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    virtual TBool IsValidFnKeyPress() const = 0;
    virtual TBool IsValidLongChrKeyPress() const = 0;
#endif

    /**
     * 
     * 
     * @since 2.6
     */
    virtual void SetEditorContext(TInt aContext) = 0;

    /**
     * 
     * 
     * @since 2.6
     */
    virtual void ActivateUI() = 0;

    /**
     * Set text to Fep aware editor
     * 
     * @since 3.2
     */
    virtual void SetFepAwareEditorText(const TFepInputContextFieldData& /*aIcfData*/) = 0;

    /**
     * Get max text length that Fep aware editor can contain
     * 
     * @since 3.2
     */
    virtual TInt EditorMaxLength() = 0;
    
    /**
     * Screen resolution or layout change notification
     * 
     * @since 3.2
     */
    virtual void ResourceChanged(TInt aType) = 0;
        
    /**
     * Set underlining FepUI interface
     * 
     * @since 3.2
     */
    virtual void SetNextFepUI(MAknFepManagerInterface* /*aNextUi*/) = 0;

	/**
	 * Set the Star key flag
	 *
     * @param aSet TBool.
     * @return None
	 */
	virtual void SetStarKeyFlag( TBool aSet ) = 0;

    };
#endif // __AKN_FEP_MANAGER_INTERFACE_H__
// End of file
