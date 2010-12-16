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
* Description:           
*       Provides the MAknFepManagerUIInterface definition.
*
*/












#ifndef __AKN_FEP_MANAGER_UI_INTERFACE_H__
#define __AKN_FEP_MANAGER_UI_INTERFACE_H__

// INCLUDES
#include <e32std.h>

#include <avkon.hrh>
#include <frmtlay.h>        // TCursorSelection
#include <aknextendedinputcapabilities.h>
#ifdef RD_INTELLIGENT_TEXT_INPUT
#include <PtiDefs.h>
#endif
#include "AknFepGlobalEnums.h"
#include "AknFepUiIndicEnums.h"

#ifdef RD_INTELLIGENT_TEXT_INPUT
#include "AknFepFnKeyManager.h"
#endif
// CLASS DECLARATION
class CPtiEngine;
class CAknFepUIInterface;
class CAknFepPluginManager;
// the follow 2 class for phrase creation 
class MZhuyinKeyHandler;
class CAknFepZhuyinAnalyser;

/**
  * This interface describes the FEP Manager functionality that is required by western 
  * Input States only. It could potentially be conditionally compiled out if western 
  * predictive text is not required in a product
  */
class MAknFepManagerUIInterfaceWestern
    {
public:
    /*
     *flags, all held in the FEP Manager
     */
    virtual TBool IsFlagSet(TInt aFlag) const = 0;
    virtual void SetFlag(TInt aFlag) = 0;
    virtual void ClearFlag(TInt aFlag) = 0;
    
	virtual void SetCcpuFlag(TInt aFlag) = 0;  
	virtual void ClearCcpuFlag(TInt aFlag) = 0;	  

    /*
     * Update the current inline edit
     * @param aUncommitedTextChange, the change to the length of the inline edit
     * i.e. 1 is a character added, 0 is no change, -1 is a character deleted
     */
    virtual void UpdateInlineEditL(const TDesC& aText, TInt aUncommitedTextChange) = 0;
    
    /*
     * Starts an inline edit
     */ 
    virtual void StartInlineEditL() = 0;

    /*
     * Starts an inline edit with a text string to uncommit
     */ 
    virtual void StartInlineEditL(const TDesC& aText) = 0;

    /*
    */
    virtual void StartInlineEditL(TCursorSelection aCursorSelection, 
                                  const TDesC& aInitialInlineText, 
                                  TInt aPositionOfInsertionPointInInlineText, 
                                  TBool aCursorVisibility) = 0;

    /*
     * Closes the UI backspace event have completely deleted a word
     */ 
    virtual TBool CloseUiIfWordDeletedL() = 0;

    /*
     * gets the text to uncommit in the engine when moving onto a word 
     *
     * @param aText the descriptor to fill with the text that should be uncommitted
     * @param aCode the keycode, must be backspace or left or right arrow
     * @param aComsumeKey ETrue if the key should be consumed
     * @return True if we have successfully found text which can be uncommited
     */     
    virtual TBool TryGetTextToUncommitL(TDes& aText, TInt aCode, TBool& aComsumeKey) = 0;
    
    /*
     * Update the CBA with a new resource
     */ 
    virtual void UpdateCbaL(TInt aResourceId) = 0;
    
    /*
     * Remove the character that is used to show to the user that the 
     * predictive text engine cannot match a word to the keys entered 
     * @return ETrue if the No Matches Indicator has been removed, EFalse
     * if it was not there.
     */ 
    virtual TBool TryRemoveNoMatchesIndicatorL() = 0;

    /*
     * @return ETrue if the new word length is one character longer than 
     * the current uncommited text
     */
    virtual TBool IsMoreGoodWordsComing(TInt aNewWordLength) const = 0;
    
    //the following 8 functions for creation   
    /**
     * Set the state to Pinyin phrase creation
     */
    virtual void PinyinPhraseCreation(TBool aPinyinPhraseCreation) = 0;

    /**
     * Is the state is in pinyin phrase creation
     *
     * @return if not enabled, return EFalse
     */
    virtual TBool IsPinyinPhraseCreation() const = 0;

    /**
     * Set the state to phrase creation
     */
    virtual void PhraseCreation(TBool aPhraseCreation) = 0;

    /**
     * Is the state is in phrase creation
     *
     * @return if not enabled, return EFalse
     */
    virtual TBool IsPhraseCreation() const = 0;

    /**
     * Set the state to input phrase
     */
    virtual void EntryPhrase(TBool aEntryPhrase) = 0;

    /**
     * Is the state is in input phrase
     *
     * @return if not enabled, return EFalse
     */
    virtual TBool IsEntryPhrase() const = 0;

    /**
     * Set the state to enable star key
     */
    virtual void EnableKeyStar(TBool aEnable) = 0;

    /**
     * Is the state is enable star key
     *
     * @return if not enabled, return EFalse
     */
    virtual TBool IsEnableKeyStar() const = 0;
#ifndef RD_INTELLIGENT_TEXT_INPUT
    /*
     * @return ETrue if inputmode is word completion and new word length is not equal to 
     * the current inputsequence length
     */
	virtual TBool IsMoreGoodAutoCompleteWordsComing(TInt aInputMode, TInt aPreviousWordLengh, 
	                                                TInt aNewWordLength) const =0;
#endif	// RD_INTELLIGENT_TEXT_INPUT

    /*
     * When starting a new word, checks the adjacent editor positions for 
     * valid inline editing characters. If these are present, we are starting a
     * compound word and the flag EFlagCompoundWord is set
     */
    virtual void TryStartCompoundWord() = 0;

    /*
     * Calculates the span of the compound word that is made up of the 
     * current uncommitted text and the adjacent word. 
     * Adds this word to the User Database
     */
    virtual void AddCompoundWordToUdbL() = 0;
    
    /**
 	*  Sets delayed commit on (predicitve word will be commited in 
 	*  next call to TryCloseUiL. This is needed because of Japanese ReadingTextL).
 	*
 	* @since 3.1
 	*/ 
    virtual void SetDelayedCommit() = 0;   
    
    /**
    * Returns a boolean value indicating whether the curesor is at the end of a word.
    */  
    virtual TBool IsCursorAtEndOfWord() = 0;       
    };

class MAknFepManagerUIInterface : public MAknFepManagerUIInterfaceWestern
    {
public:
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
    virtual TBool KeyMapsDifferentCharacterWithFn( TPtiKey aKey ) const = 0;
#endif
#endif
		/**
		* Check western predictive mode.
		* @param aMode input mode.
		* @return ETrue if western predictive mode.
		*/
		virtual TBool WesternPredictive(TInt aMode = 0) const=0;
		 /**
	     * Check Qwerty keypad
	     *
	     * @since 3.0
	     * @return ETrue is Qwerty
	     */
	    virtual TBool IsQwerty() const=0;
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
     * Launches the compact match candidate list which follows the inline editor.
     * @param   aFocusedIndex   The index of the word focused by default. 
     *                          If KErrNotFound is given, the currently active
     *                          word will be focused.
     */
    virtual void LaunchCandidatePopupListL( TInt aFocusedIndex = KErrNotFound )=0;
    /**
    Returns the Fn key state
    */
    virtual CAknFepFnKeyManager::TFnKeyState FnKeyState() = 0;
    virtual void SetFnKeyState(CAknFepFnKeyManager::TFnKeyState aState) = 0;
    /**
    Returns the current keyboard layout in use.
    */
    virtual TPtiKeyboardType KeyboardLayout() const = 0;
#ifdef __REVERSE_FN_KEY_SUPPORTED
    /**
    Returns if the input is in reverse Fn key input mode.
    */
    virtual TBool IsReverseFnkeyInput() = 0;
    
    /**
     * Returns true if the aChar is allowed in the numeric editor
     */
    virtual TBool IsValidCharInNumericEditorL( TChar aChar ) const = 0;
        
    /**
     * Filter the char for Fn reverse mode in case of
     * numeric editor. This function should call
     * in case of numeric editor reverse mapping.
     */
    virtual TBool IsValidCharForNumericInFnReverseL(TInt aKey, TPtiTextCase aCase)=0;

#endif //__REVERSE_FN_KEY_SUPPORTED
    virtual void AddOneSpaceOrMoveCursorL() = 0;
    
#endif
    virtual TBool IsHybridAplhaEditor() const = 0 ;
    virtual TBool IsHybridAlphaModeChangedtoAplhanumeric() const = 0;
	/**
	* Tells if the current editor is phone number editor
	* @return ETrue if the current editor is phone number editor
	*/
	virtual TBool IsPhoneNumberEditor() const = 0;

    /**
     * A request to close the UI. If the UI was not active (initial state)
     * multitap characters are commited by calling ExpireMultitapTimer()
     * on the ptiengine.
     * @return ETrue if the UI was active, EFalse otherwise
     */
    virtual TBool TryCloseUiL() = 0;

    /**
     * A request to send a char to the editing window
     */
    virtual void NewCharacterL(const TDesC& aChar) = 0;

    /**
     * A request to send text to the editing window
     */
    virtual void NewTextL(const TDesC& aChar) = 0;

    /**
      * A request to commit the current char
      */
    virtual void CommitInlineEditL() = 0;

    /**
     * Commits given string in the editing window. This function checks that
     * the editor has enough space to insert given string. If it not,
     * given string is cut, then it is committed.
     */
    virtual void CommitInlineEditL(const TDesC& aText, TInt aUncommitedTextChange) = 0;
    
    /**
      * A request to cancel inline input to wditing window.
      */
    virtual void CancelInlineEdit() =0;

    /** 
      * Called by Fep Manager when editing window draws inline input characters.
      */
    virtual void GetScreenCoordinatesL(TPoint& aLeftSideOfBaseLine,TInt& aHeight,
                                       TInt& aAscent,TInt aDocumentOffset) = 0;

    /** 
      * Called by Fep Manager when Chinese UI ask cursor baseline position.
      */
    virtual void GetScreenCoordinatesL(TPoint& aLeftSideOfBaseLine,
                                       TInt& aHeight,TInt& aAscent) = 0;

    /**
     * A notification to the Fep Manager that the UI has been activated..
     * Should always be called by an Initial State Object when initiating 
     * a change to an Entry State
     */
    virtual void SendUIActivatedNotification() = 0;

    /**
     * A notification to the Fep Manager that the UI has not been activated.
     */
    virtual void SendUIDeActivatedNotification() = 0;

    /**
     * calulates if an SCT can be launched in the current editor at the present
     * cursor position
     * @return ETrue if an SCT can be launched, EFalse if not
     */
    virtual TBool IsAbleToLaunchSCT() const = 0;

    /**
     * Launch the Special Character Table  
     */
    virtual void LaunchSpecialCharacterTableL(TInt aResourceId=0, 
                                              TBool aLaunchedByTouchWin=EFalse, 
                                              TBool aSmileyFirst=EFalse) = 0;

    /**
     * Play sound
     */
    virtual void PlaySound(TAvkonSystemSID aSound) const = 0;
    /**
     * get uncommitted text
     */
    virtual TCursorSelection UncommittedText() const = 0;

	/**
     * Gets the previous character depnding on wheather language is 
     * contxet sensitve or not
     * since 3.2: aContextSensitive not in use, since no real meaning found.
     */
    virtual TText PreviousChar(	TBool aContextSensitive = EFalse ) = 0;
    /**
     * Returns the next character to the current cursor position
     */
    virtual TText NextChar() = 0;
    virtual TBool IsZWSCharacterPresent( TBool aLigaturePresent = EFalse ) = 0;
    /**
     * Removes previous character to the current cursor position
     */
    virtual void RemovePreviousCharacterL() = 0;
    /**
     * Removes Zero Width Space character from the inputted text.
     */
    virtual void RemoveZWSCharacterL(TBool aIsViramaInputted, 
                                     TBool aIsInMultitappingHalant = EFalse,
                                     TBool aIsCharModifier = EFalse, 
                                     TBool aIsLigaturePresent = EFalse) = 0;
    /**
     * Adds text to the editor and commits the text.
     */
    virtual void NewCharacterSequenceL(const TDesC& aText, TIndicInputResponse aResponse) = 0;
    
    virtual TDigitType LocalDigit() = 0;
    virtual TWidthChar CharacterWidth() const = 0;
    virtual void SetInlineEditingCursorVisibilityL(TBool aCursorVisibility) = 0;
    virtual CPtiEngine* PtiEngine() const = 0;

    /**
     * checks that the editor the editor has free space to add another character
     * An editor of unlimited length will return its maximum size as zero
     */
    virtual TBool EditorHasFreeSpace( TInt aNumberOfCharacter = 0 ) const = 0;
    
    /**
     * Check whether the editor is a Java number editor
     * Return ETrue if the editor is a Java number editor 
     */
    virtual TBool IsSpecialNumericEditor() = 0;


    virtual void RedecorateAfterDeletionL( const TDesC& aTextThatWasDeleted ) = 0;

    /**
     * 
     *
     * @param aKeyCode the value of the key to simulate
     */
    virtual void SimulateKeyEventL(TUint aKeyCode, TBool aActiveObj=EFalse) = 0;

    /**
     * Queries supportting of ScrollLatinPredictive feature
     *
     * @since 2.6
     * @return ETrue if ScrollLatinPredictive was supported
     */
    virtual TBool IsAbleScrollLatinPredictive() const = 0;

    virtual TBool IsOnlyNumericPermitted() const = 0;
    
    virtual TInt EditorNumericKeymap() const = 0;

    /**
     * Queries supportting of SecretText
     *
     * @since 2.6
     * @return ETrue if SecretText was supported
     */
    virtual TBool IsSupportsSecretText() const = 0;
    
    /**
     * checks whether the current editor is a Java Secret editor.
     * return value: Etrue when it is a Java Secret editor.
     * Efalse when it is not.
     */
    virtual TBool IsJavaSecretEditor() = 0;
    

	/**
	* Returns current input mode.
	*
	* @since 3.0
	* @return Integer value indicating current editor input mode.
    */
    virtual TInt InputMode() const = 0;
    
	/**
	* Returns current CangJie input mode.
	*
	* @since 3.0
	* @return Integer value indicating current CangJie input mode.
    */    
    virtual TInt CangJieMode() const = 0;
    
 	/**
    * Returns boolean value indicating whether edit submenu functionality is in use.    
    *  
    * @since 3.1
    * @return  ETrue if edit submenu functionality is in use.
    *          EFalse otherwise.
    */        
    virtual TBool EditSubmenuInUse() const = 0;   
    
    /**
     * Launches the mode selection menu
     *
     * @since 3.1
     */
    virtual void LaunchSelectModeMenuL() = 0;                  

    /**
     * @since 3.2
     */
    virtual void SetCursorSelectionL(const TCursorSelection& aCurSel, TBool aSyncCursor) = 0;    
    
   /**                
    * Returns the status of arabic-indic digit mode setting.
    *
    * @since 3.0
    */
    virtual TBool ArabicIndicDigitsInUse() const = 0;        
    virtual TBool EasternArabicIndicDigitsInUse() const = 0;
#ifndef RD_INTELLIGENT_TEXT_INPUT
   
   /**                
    * Returns the status of auto word completion in the editor.
    *
    * @since 5.0
    */
    virtual TBool IsAutoCompleteOn() const = 0;
    
   /**                
    * Removes the suggested completion part
    *
    * @since 5.0
    */
    virtual void RemoveSuggestedCompletionL() = 0;
#endif	// RD_INTELLIGENT_TEXT_INPUT
    /**
     * Set current long clear key press
     *
     * @since 3.2
     */    
    virtual void SetLongClearAfterCloseUI(TBool aLongClear) = 0;

	/**
	 * Align the Logical and Visual cursor positions.
	 *
     * @since 3.2
     * @param aType Cursor type.
     * @param aToLeft ETrue if the position to the left is to be found,
     *			 	  EFalse if the position to the right is to be found.
     * @return None
	 */
	virtual void AlignLogicalAndVisualCursorL( TTmDocPosSpec::TType aType, 
											TBool aToLeft ) = 0;
	
	/**
	 * This method is called for removing the Repha character. The function 
	 * goes to the start of the consonant or ligature, checks if repha 
	 * character is present.
	 *
	 * @since 3.2
	 * @param None.
	 * @return None.
	 */							
	virtual void RemoveRephaCharacterL() = 0;
	
	/**
	 * Checks if the Repha character has been attached to the consonant or 
	 * ligature.
	 *
	 * @since 3.2
	 * @param None.
	 * @return ETrue if present else EFalse.
	 */	
	virtual TBool IsRephaPresent() = 0;
	
	/**
	 * Retrieve the Previous to previous character during multitapping.
	 *
	 * @since 3.2
	 * @param aContextSensitive EFalse is the default parameter.
	 *						    ETrue is used for requesting the function
	 *						    to check for extraction of the character
	 *						    two positions before the seleted text.
	 *
	 * @return The previous to previous character if any.
	 */
	virtual TText PreviousToPreviousChar( TBool aContextSensitive = EFalse ) = 0;

	 /**
	 * This method is called for removing the Rakar character. The function 
	 * checks if Rakar (Virama+Ra)  is present.
	 *
	 * @since 3.2
	 * @param None.
	 * @return None.
	 */
	virtual void RemoveRakarCharacterL() = 0;
	
	/**
     * Returns the free space left in the Editor.
     *
     * @param aUnlimit.	Set to EFalse in case of Editors with limited text 
	 *				  	size
	 *		  isToCountUncommittedTextLength. Pass ETrue if the lenght of
	 *					Uncommitted text is to be counted.
	 *
	 * @return TInt. Free space left in the Editor.
     */
    virtual TInt EditorFreeSpace(TBool& aUnlimit, 
                    TBool isToCountUncommittedTextLength = EFalse ) const = 0;
#ifdef RD_INTELLIGENT_TEXT_INPUT                        
	/**
     * Returns the free space left in the Editor. ITI specific implementation
     *
     * @param aUnlimit.	Set to EFalse in case of Editors with limited text 
	 *				  	size
	 *		  isToCountUncommittedTextLength. Pass ETrue if the lenght of
	 *					Uncommitted text is to be counted.
	 *
	 * @return TInt. Free space left in the Editor.
     */
	virtual TInt EditorFreeSpaceForAutoWordCompletion(TBool& aUnlimit, 
                     TBool isToCountUncommittedTextLength = EFalse ) const = 0;
#endif	
#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT   
    /**
    * Gives the amount of characters in the document.
    * @return   The number of characters in the active editor.
    */
    virtual TInt DocumentLength() const = 0;

#endif //RD_INTELLIGENT_TEXT_INPUT
#endif //__ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
    /**
     * from MAknFepManagerUIInterface
     * query whether the special char is valid in the current editor
     *
     * @since 3.2
     * @param aChar Checking a chracter
     * @return TBool. IF ETrue, the character is valid.
     */
    virtual TBool CharIsValidInEditor(TChar aChar) = 0;

   /**
    * Returns the status of predictive input.
    *
    * @since 3.2
     * @return TBool. IF ETrue, it is possible to change prediction ON.
    */
    virtual TBool IsAbleToChangePrediction() const = 0;

    /**
	 * This method is called for removing the N characters from the 
	 * specified position. If the text to be removed is not yet
	 * committed, it is updated with NULL descriptor and committed.
	 *
	 * @since 5.0
	 * @param TInt aPos position to start searching from. By default
	 *              Searching will start from the cursor position.
	 * @param TInt aNumOfCharacters Number of characters to remove.
	 * @param TBool Should cursor be positioned at the original location.
	 * @return None.
	 */
    virtual void RemoveTextFromEditorL( TInt aNumOfCharacters, TInt aPos = 0, 
                                        TBool aIsToAlignCursor = ETrue ) = 0;

   	/**
	 * Align the Logical and Visual cursor positions.
	 *
     * @since 5.0
     * @param None
     * @return None
	 */
	virtual void AlignLogicalAndVisualCursorL() = 0;

#ifdef RD_MARATHI
	/**
	 * This function checks if the previous two characters are
	 * the combination of Chandra-A
	 *
	 * @since 5.0
	 * @param None
	 * @return TBool.
	 */
	virtual TBool IsChandraAPresentL() = 0;
#endif // RD_MARATHI

    /**
    * Returns boolean value indicating whether hash key is in selection mode or
    * in traditional mode.
    *
    * @since 3.1
    * @return  ETrue if hash key is in text selection mode.
    *          EFalse otherwise.
    */
    virtual TBool HashKeySelectionInUse() const = 0;
    
    /**
    * Returns the UI interface implementation used in the current app.
    *
    * @since 3.2
    * @return  The UI interface object used in the current app.
    */
    virtual CAknFepUIInterface* UiInterface() = 0;
    
#ifdef RD_INTELLIGENT_TEXT_INPUT

    // Predictive QWERTY (XT9) changes ---->
        /**
     * Function to get the text direction for current language.
     * @return ETrue if language id RTL, EFalse if it is not
     */
    virtual TBool IsRightToLeftLanguage() = 0;
    
    /**
    * @return   ETrue   If the auto completion is On. EFalse otherwise.
    */
    virtual TBool IsAutoCompleteOn() const = 0;
    
    /**
    * @return	TInt 	Typing correction level.
    */
    virtual TInt AdvancedPredictiveTypingCorrectionLevel() const = 0;
    
    /**
    * @return   ETrue   If the number candidate shown. EFalse otherwise.
    */
    virtual TBool IsAdvancedPredictiveNumberCandidateShown() const = 0;
    
    /**
    * Cuts off the automatically completed tail of the suggested word candidate.
    */
    virtual void RemoveSuggestedAdvanceCompletionL() = 0;
    
     /**
    * @return   ETrue   If the primary candidate is suggested. EFalse otherwise.
    */
    virtual TBool AdvancedPredictivePrimaryCandidate() const = 0;

    /**
    * @Set the chr composition flag.
    */
    virtual void SetComposChrFlag( TBool aFlag ) = 0;
    
    /**
    * @return   ETrue   If it is chr composition key
    */
    virtual TBool GetComposChrFlag() = 0;
    
    // Predictive QWERTY (XT9) changs <----
#endif //RD_INTELLIGENT_TEXT_INPUT
    /**
    * Returns plugin ui manager
    *
    * @since 3.2
    * @return  The UI interface object used in the current app.
    */
    virtual CAknFepPluginManager* PluginUIManager() = 0;    
#ifdef RD_HINDI_PHONETIC_INPUT
    //hindi_phonetic_fixes
   	/*
    * This functions toggles the case from upper to lower
    * @since 5.0
    * @param None
    * @return  void
    */
    virtual void HandleIndicCaseL()=0;
#endif    

    // the follow 2 functions for phrase creation
    virtual MZhuyinKeyHandler* ZhuyinKeyHandler() = 0;
    virtual CAknFepZhuyinAnalyser* ZhuyinAnalyser() = 0;

#ifdef __ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
    * @return   ETrue   If the number entry with long key press is supported on QWERTY keyboard.
    */
    virtual TBool LongPressNumberEntryOnQwerty() const = 0;
#endif //RD_INTELLIGENT_TEXT_INPUT
#endif //__ITI_LONGPRESS_NUM_SHIFT_COPYPASTE__
#ifdef RD_INTELLIGENT_TEXT_INPUT    
    virtual void MultitapThroughSCTCharL(TCallBack aCallBack) = 0;
    virtual TBool IsLanguageSupportPrediction() = 0;           
    virtual TInt GetNumericSCTResID() = 0;
    /**
    * Hides the exact word pop up window.
    */ 
    virtual void HideExactWordPopUp() = 0;
    /**
    * @return   ETrue   If the exact word pop up is shown.
    */
    virtual TBool IsExactWordPopUpShown() = 0;
#endif //RD_INTELLIGENT_TEXT_INPUT   
	virtual CAknExtendedInputCapabilities::TEditorType EditorType() const = 0;
	virtual TBool IsAllowedKeymappingForNumberMode(TChar aChar) const = 0;
    };
#endif      // __AKN_FEP_MANAGER_UI_INTERFACE_H__

// End of file
