/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Layout UI interface base class
*
*/











#ifndef C_PLUGINFEPMANAGERBASE_H
#define C_PLUGINFEPMANAGERBASE_H

// System includes
#include <e32base.h>
#include <frmtlay.h>
#include <eikon.hrh>
#include <AknFepManagerInterface.h>
#include <peninputcmd.h>

//Forward declaration
class CPenInputImePluginCn;
class RPeninputServer;
class CAknEdwinState;
class CRepository;
/**
 *  CPluginFepManagerBase is the layout UI interface base class. 
 *  It is something like a "proxy" class of layout UI plugin, sending
 *  commands from FEP to layout UI plugin.
 *
 *  @lib peninputimeplugincn.lib
 *  @since S60 v3.2
 */
class CPluginFepManagerBase: public CBase, public MAknFepManagerInterface
    {
public: 

    /**
     * Destructor.
     */
    ~CPluginFepManagerBase();

    /**
     * Initalize UI when first time starting up. 
     * The first time means when the instance created.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    virtual void OnInit();

    /**
     * Explicitly get ICF max length from layout UI and save the 
     * value to iMaxEditorLength.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    void RetrieveEditorMaxLength();

    /**
     * Update local copy of case mode when 
     * user change case in layout UI.
     * 
     * @since S60 v3.2
     *
     * @param aCaseMode The case mode to save locally.
     * @return None.
     */
    void UpdateCaseMode( TInt aCaseMode );

// From MAknFepManagerInterface

    /**
     * Set layout UI current number mode key mapping.
     * For example, 0-9 ; 0-9,*,# and etc.
     * 
     * @since S60 v3.2
     *
     * @param aAknEditorNumericKeymap Key mapping enum value.
     * @return None.
     */
    void SetNumberModeKeyMappingL( TAknEditorNumericKeymap aAknEditorNumericKeymap );

    /**
     * Handle key event.
     * Since most plugin layout UIs are based on animation, they receive 
     * key and process key events before this function. 
     * 
     * @since S60 v3.2
     *
     * @param aKey Key event code.
     * @param aLength Key press length.
     * @return ETrue if processed, EFalse otherwise.
     */
    TBool HandleKeyL( TInt aKey, TKeyPressLength aLength, TEventCode aEventCode = EEventKey );

    /**
     * Handle command come from FEP.
     * 
     * @since S60 v3.2
     *
     * @param aCommandId Command id.
     * @return None
     */
    void HandleCommandL( TInt aCommandId );

    /**
     * Handle command come from FEP.
     * 
     * @since S60 v3.2
     *
     * @param aCommandId Command id.
     * @param aParam  Command parameter.
     * @return None.
     */
    void HandleCommandL( TInt aCommandId, TInt aParam );

    /**
     * Close plugin layout UI.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    void CloseUI();

    /**
     * Activate plugin layout UI.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    void ActivateUI();

    /**
     * Get max text length of layout UI ICF control.
     * 
     * @since S60 v3.2
     * @return Max ICF text length.
     */
    TInt EditorMaxLength();

    /**
     * When screen size changes, SizeChanged is called by FEP.
     * 
     * @since S60 v3.2
     * @return None.
     */
    void ResourceChanged( TInt aType );

    /**
     * Set underlining UI.
     * Since plugin layout UI and HKB can be used at same time,
     * HKB UI layout need to be notified after plugin layout UI 
     * processes some events
     * 
     * @since S60 v3.2
     * @param aNextUI The underlining UI pointer.
     * @return None.
     */
    void SetNextFepUI( MAknFepManagerInterface* aNextUi );
    
    /**
     * Get support language in sepecfied mode.
     * The method leaves here only for compatibility.
     * 
     * @since S60 v3.2
     *
     * @param aMode Input mode.
     * @return Language supported.
     */
    TInt SupportLanguage( TInt aMode ) const;

    /**
     * Set current language, which is required by editor
     * 
     * @since S60 v3.2
     *
     * @param aLanguage Language to set.
     * @return None.
     */
    void SetInputLanguageL( TLanguage aLanguage );

    /**
     * Set editor text and cursor information to layout UI 
     * ICF control.
     * 
     * @since S60 v3.2
     * @param aTotalTextLen Total text length in eidtor.
     * @param aStartPos Offset of aText to the start of editor text.
     * @param aCursorSel Cursor position.
     * @param aText A part of editor text start from aStartPos.
     *
     * @return None.
     */
    void SetFepAwareEditorText( const TFepInputContextFieldData& aIcfData );

    /**
     * Set layout UI mode.
     * The method leaves here only for compatibility
     * 
     * @since S60 v3.2
     *
     * @param aMode Mode to set.
     * @param aPredictive Whether the current mode support predictive.
     * @param aWwertyInputMode Whether the current HKB is qwerty.
     * @return None.
     */
#ifdef RD_INTELLIGENT_TEXT_INPUT          
    void SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode, TInt aKeyboardType);
#else
    void SetMode(TInt aMode, TBool aPredictive, TBool aQwertyInputMode);
#endif   

#ifdef RD_INTELLIGENT_TEXT_INPUT
    virtual TBool IsValidFnKeyPress() const;
    virtual TBool IsValidLongChrKeyPress() const;
#endif

    /**
     * Set layout UI current case.
     * The method leaves here only for compatibility. Since plugin layout 
     * UI is different from HKB tranditional UI, they use something like 
     * "case mode" other than case which is set by a FEP command.
     * 
     * @since S60 v3.2
     *
     * @param aCase Case to set.
     * @return None.
     */
    void SetCase( TCase aCase );

    /**
     * The method leaves here only for compatibility.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    void ExpireMultitapTimer();

    /**
     * The method leaves here only for compatibility.
     * 
     * @since S60 v3.2
     * 
     * @param aKey The key presssed.
     * @return ETrue if long press, EFalse otherwise.
     */
    TBool IsValidNumericLongKeyPress( TInt aKey ) const;

    /**
     * The method leaves here only for compatibility.
     * 
     * @since S60 v3.2
     *
     * @param aText Text to add to user dictionary.
     * @return None.
     */
    void AddTextToUserDictionaryL( const TDesC& aText );

    /**
     * The method leaves here only for compatibility.
     * 
     * @since S60 v3.2
     *
     * @param aFormat On return, contains the formatting to 
     *        apply to the inline text (or to a portion of it). 
     * @param aNumberOfCharactersWithSameFormat  On return, 
              contains the number of characters in the inline 
              text (starting at aPositionOfCharacter) 
              which have the same formatting
     * @param aPositionOfCharacter Start position within the inline text
     * @return None.
     */
    void GetFormatOfFepInlineText( TCharFormat& aFormat, 
                                   TInt& aNumberOfCharactersWithSameFormat, 
                                   TInt aPositionOfCharacter ) const;

    /**
     * The method leaves here only for compatibility.
     * 
     * @since S60 v3.2
     *
     * @return ETrue if valid, EFalse otherwise.
     */
    TBool IsValidShiftKeyPress() const;

    /**
     * The method leaves here only for compatibility.
     * 
     * @since S60 v3.2
     *
     * @return None.
     */
    void SetEditorContext( TInt aContext );

	/**
	 * Set the Star key flag
	 *
     * @param aSet TBool.
     * @return None
	 */
	void SetStarKeyFlag( TBool aSet );
protected:
   
    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aOwner IME plugin pointer.
     * @param aPenInputServer Pen input server pointer
     */
    CPluginFepManagerBase(CPenInputImePluginCn& aOwner, 
                          RPeninputServer* aPenInputServer);

    /**
     * By default Symbian 2nd phase constructor.
     *
     * @since S60 v3.2
     * @return None
     */
    void BaseConstructL();

    /**
     * Set layout permited ranges.
     *
     * @param aPermittedMode Ranges of current editor allowed.
     * @return None
     */
    virtual void SetLayoutPermitedRanges( TInt aPermittedRanges );

    /**
     * Set layout primary range.
     *
     * @since S60 v3.2
     * @param aEditorState Current editor state.
     * @return None
     */
    virtual void SetLayoutRange( const CAknEdwinState* aEditorState );

    /**
     * Send command to pen input server utility function.
     *
     * @since S60 v3.2
     * @param aCommandId The command id to sent.
     * @return None
     */
    void SendCommandToServer( TInt aCommandId );

    /**
     * Send command to pen input server utility function.
     *
     * @since S60 v3.2
     * @param aCommandId The command id to sent.
     * @param aParam The command param.
     * @return None
     */
    void SendCommandToServer( TInt aCommandId, TInt aParam );

    /**
     * Send command to pen input server utility function.
     *
     * @since S60 v3.2
     * @param aCommandId The command id to sent.
     * @param aParam The command param.
     * @return None
     */
    void SendCommandToServer( TInt aCommandId, const TDesC8& aParam );

    /**
     * Get layout UI last used range.
     *
     * @since S60 v3.2
     * @return Range last used.
     */
    TInt LayoutLastUsedRange();

    /**
     * Set layout UI position.
     * Move layout UI not to overlap with some editors.
     *
     * @since S60 v3.2
     * @param aBottomRight The bottom right position of layout UI.
     *        The value is only a suggest value, layout UI may adjust it.
     * @return None.
     */
    void SetLayoutPosition( const TRect& aParam );

    /**
     * Get Editor's local language.
     *
     * @since S60 v3.2
     * @param aEditorState The editor's .CAknEdwinState pointer
     * @return Language code the editor requires.
     */
    TLanguage GetLocalLanguage( const CAknEdwinState* aEditorState ) const;
   
protected:

    /**
     * IME plugin reference.
     */
    CPenInputImePluginCn& iOwner;

    /**
     * Pen input server pointer. Not own.
     */
    RPeninputServer* iPenInputServer;

    /**
     * The premitted range of current editor.
     */
    TInt iPermittedRange;
    
    TInt iHkbMode;
    /**
     * The fep suggested range. The suggested value comes from
     * previous touch input method.
     */
    TInt iSuggestedRange;
    
    /**
     * Current language.
     */
    TLanguage iLanguage;   
    
private:

    /**
     * Repository object used to get last used range.
     */
    CRepository* iRepository;

    /**
     * Local copy of ICF max text length.
     */
    TInt iMaxEditorLength;

    /**
     * Local copy of layout current case mode
     */
    TInt iLastCase;

    /**
     * Underlineing UI interface
     */
    MAknFepManagerInterface* iHkbUi;
    };
#endif // C_PLUGINFEPMANAGERBASE_H

// End Of File
