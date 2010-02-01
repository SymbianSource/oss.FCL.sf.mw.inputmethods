/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the MAknFepUICtrlEditPane definition.
*
*/













#ifndef M_AKNFEPUICTRLEDITPANE_H
#define M_AKNFEPUICTRLEDITPANE_H

/**
 * pane in which the user's edit is shown
 */
class MAknFepUICtrlEditPane
    {
public:

    /**
	 * Set the show info
	 * 	 
     * @since S60 v5.0
     * @param aDes the show info.
     * @return None.
     */
	virtual void SetText( const TDesC& aDes ) = 0;
	
    /**
     * Set the show info and cursor position
     *   
     * @since S60 v5.0
     * @param aDes the show info.
     * @param aCursorIndex the cursor position.
     * @return None.
     */
    virtual void SetText( const TDesC& aDes, TInt aCursorIndex, TBool aMiniQwertyZhuyin = EFalse ) = 0;
    
    /**
     * Set the show info
     *   
     * @since S60 v5.0
     * @param aDes the show info.
     * @param aCursorIndex the cursor position.
     * @param aChineseCount the chinese count.
     * @param aJudgeSeed the zhuyin symbols.
     * @param aKeystroke the index of symbols.
     * @param aRefreshOffset ETrue the show info only is zhuyin symbols.
     * @return None.
     */
    virtual void SetTextForZhuyin( 
                                  const TDesC& aDes, 
                                  TInt aCursorIndex, 
                                  TInt aChineseCount, 
                                  const TDesC& aJudgeSeed, 
                                  TInt aKeystroke, 
                                  TBool aRefreshOffset = EFalse,
                                  TBool aMiniQwertyZhuyin = EFalse
                                  ) = 0;
    
    /**
     * Set the cousor the index. 
     *   
     * @since S60 v5.0
     * @param aIndex the index of cousor.
     * @return None.
     */
	virtual void SetCursor( const TInt aIndex ) = 0;
	
    /**
     * Get the index of the cursor.
     *   
     * @since S60 v5.0
     * @return the index of cousor.
     */
	virtual TInt GetCursor() = 0;
	
    /**
     * Move cursor to the left.
     *   
     * @since S60 v5.0
     * @param aStride the cursor move offset.
     * @return ETrue if the cursor can be move,EFalse the others.
     */
	virtual TBool MoveCursorLeft( TInt aStride = 1 ) = 0;

    /**
     * Move cursor to the right.
     *   
     * @since S60 v5.0
     * @param aStride the cursor move offset.
     * @return ETrue if the cursor can be move,EFalse the others.
     */
	virtual TBool MoveCursorRight( TInt aStride = 1 ) = 0;
	
    /**
     * Active cursor. 
     *   
     * @since S60 v5.0
     * @return None.
     */
    virtual void ActiveCursor() = 0;

    /**
     * Deactive cursor. 
     *   
     * @since S60 v5.0
     * @return None.
     */
    virtual void DeactiveCursor() = 0;
    
    /**
     * Enable cursor, the cursor will be drawn.
     *   
     * @since S60 v5.0
     * @return None.
     */
    virtual void EnableCursor() = 0;
    
    /**
     * Disable cursor, the cursor will be not drawn.
     *   
     * @since S60 v5.0
     * @return None.
     */
    virtual void DisableCursor() = 0;
    
    /**
     * Set the show info underline.
     *   
     * @since S60 v5.0
     * @param aStartIndex the underline begine index.
     * @param aEndIndex the underline end index.
     * @return None.
     */
	virtual void SetUnderline( TInt aStartIndex, TInt aEndIndex ) = 0;
	
    /**
     * Get the show info underline.
     *   
     * @since S60 v5.0
     * @param aStartIndex the underline begine index.
     * @param aEndIndex the underline end index.
     * @return None.
     */
    virtual void GetUnderlineIndex(TInt& aStartIndex, TInt& aEndIndex) = 0;
	
    /**
     * Set the highlight text.
     *   
     * @since S60 v5.0
     * @param aStartIndex the high light begine index.
     * @param aEndIndex the high light end index.
     * @return None.
     */
    virtual void SetHighlight(TInt aStartIndex, TInt aEndIndex) = 0;

    /**
     * Get the highlight text.
     *   
     * @since S60 v5.0
     * @param aStartIndex the high light begine index.
     * @param aEndIndex the high light end index.
     * @return None.
     */
    virtual void GetHighlightIndex(TInt& aStartIndex, TInt& aEndIndex) = 0;
    
    /**
     * set index of keystroke array.
     *   
     * @since S60 v5.0
     * @param aStartIndex the index.
     * @return None.
     */
	virtual void SetCursorIndexOfKeystroke( const TInt aStartIndex ) = 0;
	
    /**
     * Get index of keystroke array.
     *   
     * @since S60 v5.0
     * @return the index.
     */
	virtual TInt GetCursorIndexOfKeystroke() = 0;

    /**
     * Enables the display of the control. Sets the control's 
     * window postion and visibility, but only if the value has
     * changed (so it's ok to call it excessively). 
     *
     * @since S60 v5.0
     * @param aEnable if ETrue, makes visible
     *                          if EFalse, makes non-visible
     * @return none.
     */
    virtual void Enable(TBool aEnable) = 0;	

    /**
     * Is the control enabled for display?
     *
     * @since S60 v5.0
     * @return if not enabled, return EFalse
     */
    virtual TBool IsEnabled() const = 0;
	
    /**
     * Get the text buffer of the pane
     *
     * @since S60 v5.0
     * @param aText the buffer is filled with the contents of the candidate
     *              of the pane, up to the size of the buffer
     * @return if not enabled, return EFalse
     */
    virtual void GetText(TDes& aText) = 0;
    
    /**
     * Get the show info before cursor.
     *
     * @since S60 v5.0
     * @param aOut the show info of the before cursor.
     * 
     * @return if not enabled, return EFalse
     */
    virtual TBool GetDesBeforeCursor ( TDes& aOut ) = 0;

    /**
     * This function sets the Tone Mark for the Pane.
     *
     * @since S60 v5.0
     * @param aToneMark descriptor containing the tone mark character
     * 
     * @return none.
     */
    virtual void SetToneMark(const TDesC& aToneMark) = 0;

    /**
     * This function sets the validity of the tonemark
     *
     * @since S60 v5.0
     * @param aValid if ETrue, tonemark will be displayed as valid
     *                          if EFalse, tonemark will be displayed as not valid
     * 
     * @return none.
     */
    virtual void SetToneMarkValidity(TBool aValid) = 0;
    
    /**
     * Reset private member viariable.
     * 
     * @since S60 v5.0
     * @return None.
     */
    virtual void Reset() = 0;
    
    /**
     * Sets the font to be used, which will override the font from LAF
     *
     * @since S60 v5.0
     * @param aFontId the new font id, pass in 0 to use the one from LAF
     * @return None.
     */
    virtual void SetOverrideFontId(TInt aFontId) = 0;
    
    /**
     * Sets flag
     *
     * @since S60 v5.0
     * @param aFlag the flag
     * @return None.
     */
    virtual void SetFlag(TInt aFlag) = 0;
    
    /**
     * Clear flag
     *
     * @since S60 v5.0
     * @param aFlag the flag
     * @return None.
     */
    virtual void ClearFlag(TInt aFlag) = 0;
    
    /**
     * check flag
     *
     * @since S60 v5.0
     * @param aFlag the flag
     * @return if not enabled, return EFalse.
     */
    virtual TBool IsFlagSet(TInt aFlag) const = 0;
    
    /**
     * Return kestroke array.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    virtual CDesCArrayFlat* KeystrokeArray() = 0;

    /**
     * Return show kestroke array. 
     * save keystroke that after Analyse when pinyin. 
     * save all valid keystroke when zhuyin. 
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    virtual CDesCArrayFlat* ShowKeystrokeArray() = 0;

    /**
     * Return choose chinese charater array. 
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    virtual CDesCArrayFlat* PhraseArray() = 0;

    /**
      * Return choose chinese charater show keystroke array. 
      * 
      * @since S60 v3.2.3
      * @param None.
      * @return pointer of array.
      */
    virtual CDesCArrayFlat* PhraseShowKeyStrokeArray() = 0;
    
    /**
     * Return temp array. 
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    virtual CDesCArrayFlat* TempArray() = 0;
    
    /**
     * Reset private member viariable array.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return None.
     */
    virtual void ResetAllArray() = 0;
    
    /**
     * change state flag.
     * 
     * @since S60 v3.2.3
     * @param aState, ETrue change state.
     * @return None.
     */
    virtual void SetChangeState( TBool aState = ETrue ) = 0;
    
    /**
     * check change state flag.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue if change state, EFalse others.
     */
    virtual TBool IsChangeState() = 0;

    /**
     * Set auto change state flag.
     * 
     * @since S60 v3.2.3
     * @param aState, ETrue auto change state, EFalse manual change state.
     * @return None.
     */
    virtual void SetAutoChangeStateFlag( TBool aState = ETrue ) = 0;
    
    /**
     * check change state flag.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue atuo change state, EFalse others.
     */
    virtual TBool GetAutoChangeStateFlag() = 0;
    
    
    /**
     * Set all the keystroke valid flag.
     * 
     * @since S60 v3.2.3
     * @param aState, ETrue all the keystroke is valid, EFalse others.
     * @return None.
     */
    virtual void SetAllValidFlag( TBool aState = ETrue ) = 0;
    
    /**
     * Get all the keystroke valid flag.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue all the keystroke is valid, EFalse others.
     */
    virtual TBool GetAllValidFlag() = 0;
    
    /**
     * Set the keystroke invalid index.
     * 
     * @since S60 v3.2.3
     * @param aIndex, the keystroke invalid index.
     * @return None.
     */
    virtual void SetInvalidIndex( TInt aIndex = 0 ) = 0;
    
    /**
     * Get all the invalid keystroke index.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return invalid keystroke index.
     */
    virtual TInt GetInvalidIndex() = 0;
    
    /**
     * Set Phrase Creation flag.
     * 
     * @since S60 v3.2.3
     * @param aState, ETrue is phrase creation, EFalse others.
     * @return None.
     */
    virtual void SetPhraseCreationFlag( TBool aState ) = 0;
    
    /**
     * Get Phrase Creation flag.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue is phrase creation, EFalse others.
     */
    virtual TBool GetPhraseCreationFlag() = 0;
    
    /**
     * Set Phrase Candidate state need refresh UI flag.
     * 
     * @since S60 v3.2.3
     * @param aState, ETrue is that Candidate state need refresh UI,other don't need.
     * @return None.
     */
    virtual void SetCandidateRefreshFlag( TBool aState ) = 0;
    
    /**
     * Get Phrase Candidate state need refresh UI flag.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue is that Candidate state need refresh UI,EFalse don't need.
     */
    virtual TBool GetCandidateRefreshFlag() = 0;
    
    /**
     * Set the delimiter count of keystroke.
     * 
     * @since S60 v3.2.3
     * @param aCount, the delimiter count of keystroke.
     * @return None.
     */
    virtual void SetDelimiterCount( TInt aCount ) = 0;
    
    /**
     * Get the delimiter count of keystroke. 
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return aCount, the delimiter count of keystroke.
     */
    virtual TInt GetDelimiterCount() = 0;
    
    /**
     * Get current cba resource id. 
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return current cba resource id.
     */
    virtual TInt GetCurrentCBAResID() = 0;
    
    /**
     * Set current cba resource id. 
     * 
     * @since S60 v3.2.3
     * @param aResourceId, current cba resource id.
     * @return None.
     */
    virtual void SetCurrentCBAResID( TInt aResourceId) = 0;
    
    virtual void SetEffictiveLength( TInt aLength ) = 0;
    
    virtual TInt GetEffictiveLength() = 0;
    
    virtual void DecEffictiveLength() = 0;
    
    virtual void AddEffictiveLength() = 0;
    
    virtual void SetNeedClearDeliberateSelection( TBool aNeedClearDeliberateSelection ) = 0;
    
    virtual TBool NeedClearDeliberateSelection() = 0;
	/**
     * Return keycode array.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    virtual RArray<TInt>* KeycodeArray() = 0;
    
	/**
     * Return phrase keycode array.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    virtual RArray<TInt>* PhraseKeycodeArray() = 0;
    };

#endif /*M_AKNFEPUICTRLEDITPANE_H*/

//End of File

