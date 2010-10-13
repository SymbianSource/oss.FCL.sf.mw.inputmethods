/*
* Copyright (c) 2003 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepUIManagerBase definition.
*
*/












#ifndef __AKN_FEP_UI_MANAGER_BASE_H__
#define __AKN_FEP_UI_MANAGER_BASE_H__

// INCLUDES
#include <uikon.hrh>                        //TAknEditorNumericKeymap

#include "AknFepManagerUIInterface.h"
#include "AknFepManagerInterface.h"
#include "AknFepUIManagerStateInterface.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION
class CPtiEngine;
class TAknFepInputStateBase;

/**
 *  CAknFepUIManagerBase class.
 *
 *  @lib AknFep.lib
 *  @since 2.6
 *
 */
class CAknFepUIManagerBase : 
            public CBase,
            public MAknFepManagerInterface,
            public MAknFepUIManagerStateInterface
    {
private:
    /**
     * private constants
     */
    enum
        {
        EBaseLanguageNumericKeymapIndex = 0x00FF,
        EMaskShiftOffset                = 8 
        };

public: // Constructors and destructor
    /**
     * 
     * 
     * @since 2.6
     */
    static CAknFepUIManagerBase* NewL(MAknFepManagerUIInterface* aFepMan, 
                                      CAknFepCaseManager* aCaseMan, TLanguage aLanguage);

    /**
     * 
     * 
     * @since 2.6
     */
    virtual ~CAknFepUIManagerBase();

public: // Functions from base classes
    /**
     * From MAknFepManagerInterface
     * 
     * @since 2.6
     */
#ifdef RD_INTELLIGENT_TEXT_INPUT     
    void SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode, 
                 TInt aKeyboardType = EPtiKeyboardNone);
#else
    void SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode);
#endif    

    /**
     * From MAknFepManagerInterface
     * 
     * @since 2.6
     */
    void SetCase(TCase aCase);

    /**
     * From MAknFepManagerInterface
     * 
     * @since 2.6
     */
    void SetNumberModeKeyMappingL(TAknEditorNumericKeymap aAknEditorNumericKeymap);

    /**
     * From MAknFepManagerInterface
     * 
     * @since 2.6
     */
    const TDesC& NumberModeKeyMapping() const;

    /**
     * From MAknFepManagerInterface
     * 
     * @since 2.6
     */
    TBool HandleKeyL(TInt aKey, TKeyPressLength aLength, TEventCode aEventCode = EEventKey );

    /**
     * From MAknFepManagerInterface
     * this is in base class only
     * @since 2.6
     */
    void HandleCommandL(TInt aCommandId);
    void HandleCommandL(TInt aCommandId, TInt aParam);

    /**
     * From MAknFepManagerInterface
     * this should be implemented in each ui manager
     * 
     * @since 2.6
     */
    void CloseUI();

    /**
     * From MAknFepManagerInterface
     * this is in base class only
     *
     * @since 2.6
     */
    void ExpireMultitapTimer();

    /**
     * From MAknFepManagerInterface
     * this is in base class only
     * 
     * @since 2.6
     */
    TBool IsValidNumericLongKeyPress(TInt aKey) const;

    /**
     * From MAknFepManagerInterface
     * this api use in chinese and western
     * 
     * @since 2.6
     */
    void AddTextToUserDictionaryL(const TDesC& aText);

    /**
     * From MAknFepManagerInterface
     * this should be implemented in each ui manager
     * 
     * @since 2.6
     */
    void GetFormatOfFepInlineText(TCharFormat& aFormat, TInt& aNumberOfCharactersWithSameFormat, 
                                  TInt aPositionOfCharacter) const;

    /**
     * From MAknFepManagerInterface
     * 
     * @since 2.6
     */
    virtual TInt SupportLanguage(TInt aMode) const;

    /**
     * From MAknFepManagerInterface
     * 
     * @since 2.6
     */
    virtual void SetInputLanguageL(TLanguage aLanguage);

    /**
     * From MAknFepManagerInterface
     * 
     * @since 2.6
     */
    TBool IsValidShiftKeyPress() const;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    TBool IsValidFnKeyPress() const;

    virtual TBool IsValidLongChrKeyPress() const;

#endif

    /**
     * From MAknFepManagerInterface
     * this api use in chinese ui only
     * 
     * @since 2.6
     */
    void SetEditorContext(TInt aContext);

    /**
     * 
     * 
     * @since 2.6
     */
    void ActivateUI();

    /**
     * From MAknFepUIManagerStateInterface
     * this is in base class only
     *
     * @since 2.6
     */
    TUIState State() const;

    /**
     * From MAknFepUIManagerStateInterface
     * this should be implemented in each ui manager
     *
     * @since 2.6
     */
    TUIState ChangeState(TUIState aState);

    /**
     * From MAknFepUIManagerStateInterface
     * this is in base class only
     *
     * @since 2.6
     */
    MAknFepManagerUIInterface* FepMan() const;

    /**
     * From MAknFepUIManagerStateInterface
     * using instead of PtiEngine()
     *
     * @since 2.6
     */
    CPtiEngine* PtiEngine() const;
    
    // the follow function for phrase creation
    MZhuyinKeyHandler* ZhuyinKeyHandler();
    CAknFepZhuyinAnalyser* ZhuyinAnalyser();
    
    /**
     * From MAknFepUIManagerStateInterface
     * this api use in chinese input states only
     *
     * @since 2.6
     */
    TBool IsValidChineseInputKey(TInt aKey) const;

    /**
     * From MAknFepUIManagerStateInterface
     * this api use in chinese Qwerty input states only
     *
     * @since 3.0
     */
    TBool IsValidChineseInputKeyQwerty(TInt aKey) const;
    
    /**
     * From MAknFepUIManagerStateInterface
     * this api use in chinese Qwerty input states only
     *
     * @since 3.0
     */
    TBool IsQwertyZhuyinToneMarkKey(TInt aKey) const;

    /**
     * this api use in HK Qwerty input states only
     *
     * @since 3.0
     */
    virtual void UpdateCangJieState();
    
    /**
     * From MAknFepUIManagerStateInterface
     * this api use in chinese input states only
     *
     * @since 3.0
     */
    TBool IsValidChineseSymbol(TInt aKey) const;
    /**
     * From MAknFepUIManagerStateInterface
     * this is in base class only
     * TLanguage is Language identity enumeration defined in e32std.h
     *
     * @since 2.6
     */
    TLanguage GetLanguage() const;

    /**
     * From MAknFepUIManagerStateInterface
     * 
     * @since 2.6
     */
    CAknFepCaseManager* CaseMan() const;

    /**
     * From MAknFepUIManagerStateInterface
     * 
     * 
     * @since 2.6
     * this api use in chinese ui only
     */
    TPtr GetLatestDeliberateSelection() const;

    /**
     * From MAknFepUIManagerStateInterface
     * Thai only
     *
     * @since 2.6
     */
    TInt ThaiSCTResourceId(TUint aPrewChar, TInt aKey);

    /**
     * From MAknFepUIManagerStateInterface
     * Japanese only
     *
     * @since 2.6
     */
    CDesCArrayFlat* CandidateArray();

    /**
     * From MAknFepUIManagerStateInterface
     * Japanese only
     *
     * @since 2.6
     */
    TBool IsPredictive() const;

    /**
     * From MAknFepUIManagerStateInterface
     * Japanese only
     * 
     * @since 2.6
     */
    void SetInlineEditUnderlineVisibility(TBool aFlag);

    /**
     * From MAknFepUIManagerStateInterface
     *
     * @since 3.0
     */
    TBool IsQwerty() const;

    /**
     * From MAknFepUIManagerStateInterface
     *
     * @since 3.2
     */
    void SetFepAwareEditorText(const TFepInputContextFieldData& /*aIcfData*/);

    /**
     * From MAknFepUIManagerStateInterface
     *
     * @since 3.2
     */
    TInt EditorMaxLength();
    
    /**
     * From MAknFepUIManagerStateInterface
     *
     * @since 3.2
     */
    void ResourceChanged(TInt aType);

    /**
     * From MAknFepUIManagerStateInterface
     *
     * @since 3.2
     */
    void SetNextFepUI(MAknFepManagerInterface* /*aNextUi*/);

	/**
	 * Set the Star key flag
	 *
     * @param aSet TBool.
     * @return None
	 */
	void SetStarKeyFlag( TBool aSet );

protected: // New Functions
    /**
     * 
     * 
     * @since 2.6
     */
    void ConstructL(TLanguage aLanguage);

    /**
     * 
     * 
     * @since 2.6
     */
    CAknFepUIManagerBase(MAknFepManagerUIInterface* aFepMan, CAknFepCaseManager* aCaseMan);

    /**
     * 
     * 
     * @since 2.6
     */
    virtual TAknFepInputStateBase* FepUIState();
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
     * Removes the character produced by the initial short key press and substitutes
     * it with the character found behind Fn. If no no Fn-mapping is available, the
     * function does nothing. Also, in non-QWERTY modes the function does nothing because
     * ITU-T input states handle long press internally.
     * @return  ETrue   if key was consumed and no further handling should be made.
     *                  This is the case when editor is filled up and no more characters can be entered.
     *          EFalse  if handling of the key event may continue. Either the mode has
     *                  been changed to Fn mode for the next character or nothing has
     *                  been done and normal key repeat should happen.
     * @since 3.2
     */
    virtual TBool HandleLongKeyL( TInt aKey );
    
    /**
    * Removes the character produced by the last handled key press. Long press number
    * entry uses this to remove the character which was entered when the key was pressed
    * down ("the initially short press character") before the key press turned out to be
    * a long press.
    * @return   ETrue   if something was really removed
    *           EFalse  if nothing was removed (the previous key press had produced no charcters)
    * @since 3.2
    */
    virtual TBool RollbackPreviousCharL();
    
    	
	/**
    * Check whether the character mapped on given key
    * @param    aKey    The scancode of the key to be checked.
    * @param    aMode   The InputMode regarding ptiengine
	* @param    aAgainst The character to be checked.
	* @param    aCase    Text case.
	* @Return   ETrue : yes it is on the key; EFalse : No it is not on the Key
    */
    TBool MapAgainst(TInt aKey, TInt aMode, TInt16 aAgainst, TInt aCase) const;


#endif //RD_INTELLIGENT_TEXT_INPUT
#endif // __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__

protected: // Data
    TInt iMode;                         // 
    TInt iNumericKeymapResourceId;
    HBufC* iNumericKeymapBuffer;
    TLanguage iLanguage;                //

    CPtiEngine* iPtiEngine;             //owned

    TAknFepInputStateBase* iStatePtr;   // gcc workaround to force compiler to use vtable lookup.

    MAknFepManagerUIInterface* iFepMan; //not owned
    CAknFepCaseManager* iCaseMan;       //not owned
    TBool iPredictive;                  // ETrue: prediction is ON.
#ifdef RD_INTELLIGENT_TEXT_INPUT
    // predictive QWERTY changes (e.g. XT9) ---->
    TBool iQwertyInputMode;             // ETrue: QWERTY mode is ON
    // predictive QWERTY changes (e.g. XT9) <----
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
    TInt iPreviousInputSeqLen;
    TInt iPreviousEditorTextLen;
    TInt iInputSeqLenDeltaWithLastKey;
    TInt iEditorTextLenDeltaWithLastKey;
#endif //__ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__   
#endif //RD_INTELLIGENT_TEXT_INPUT
    TUIState iInitialFepUIState;        //
    TInt     iCaseBeforeLongKey;
    };

#endif //__AKN_FEP_UI_MANAGER_BASE_H__

// End of file
