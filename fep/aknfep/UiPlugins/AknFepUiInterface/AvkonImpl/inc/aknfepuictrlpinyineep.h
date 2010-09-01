/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
 *      Provides the CAknFepUICtrlPinyinEEP definition 
 *
*/












#ifndef C_AKNFEPUICTRLPINYINEEP_H
#define C_AKNFEPUICTRLPINYINEEP_H

#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>
#include <AknUtils.h>
#include <AknsFrameBackgroundControlContext.h> 

#include "aknfepuictrleditpane.h"
#include "aknfepuictrleepcontrol.h"

/**
 * The parent control of CAknFepUICtrlEEPControl.
 * 
 * @lib aknfepuiavkonplugin.dll
 * @since S60 v5.0
 */
const TInt KMaxBufLen = 100;
class CAknFepUICtrlPinyinEEP : public CCoeControl, public MAknFepUICtrlEditPane
    {
public:
    enum
        {
        EEditableEntryPane
        };
    enum
        {
        EZhuyinNULL,
        EZhuyinInsert,
        EZhuyinDelete,
        EZhuyinReplace,
        EZhuyinMoveCursor
        };
    /**
     * NewL function.
     * 
     * @since S60 v5.0
     * @param aParent. the RWindow object.
     * @return self object.
     */
    static CAknFepUICtrlPinyinEEP* NewL( RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr );

    /**
     * NewLC function.
     * 
     * @since S60 v5.0
     * @param aParent. the RWindow object.
     * @return self object.
     */
    static CAknFepUICtrlPinyinEEP* NewLC( RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr );

    /**
     * Destructor.
     * 
     * @return none.
     */
    virtual ~CAknFepUICtrlPinyinEEP();

    /**
     * From CCoeControl.
     * 
     * @return. count of controls.
     */
    virtual TInt CountComponentControls() const;

    /**
     * From CCoeControl.
     * 
     * @param aIndex.
     * @return. the pointer of sub-control
     */
    virtual CCoeControl* ComponentControl( TInt aIndex ) const;

    /**
     * From CCoeControl.
     * 
     * @param aRect.
     * @return None.
     */
    virtual void Draw( const TRect& aRect ) const;

    /**
     * From CCoeControl.
     * 
     * @param None.
     * @return None.
     */
    virtual void SizeChanged();

    /**
     * From MAknFepUICtrlEditPane.
     * Set the text buffer and maintain the current cursor position.
     * 
     * @since S60 v5.0
     * @param aDes. the content you set.
     * @return None.
     */
    void SetText( const TDesC& aDes );

    /**
     * From MAknFepUICtrlEditPane.
     * Set the text buffer and posit the cursor index.
     * 
     * @since S60 v5.0
     * @param aDes. the content you set.
     * @param aCursorPos. the position of cursor.
     * @param aMiniQwertyZhuyin 
     * @return None.
     */
    void SetText( const TDesC& aDes, TInt aCursorIndex, TBool aMiniQwertyZhuyin = EFalse );

    /**
     * From MAknFepUICtrlEditPane.
     * Set the text buffer and posit the cursor index for zhuyin.
     * 
     * @since S60 v5.0
     * @param aDes. the content you set.
     * @param aCursorPos. the position for cursor.
     * @param aChineseCount. the count for chinese characters.
     * @param aJudgeSeed. current zhuyin symbols' descriptor.
     * @param aKeystroke. current key stroke position.
     * @param aInvalid. whether aDes is invalid. ETrue for invalid, EFalse for valid.
     * @return None.
     */
    void SetTextForZhuyin( const TDesC& aDes, TInt aCursorIndex, TInt aChineseCount,
        const TDesC& aJudgeSeed, TInt aKeystroke, TBool aInvalid = EFalse, TBool aMiniQwertyZhuyin = EFalse );

    /**
     * From MAknFepUICtrlEditPane
     * Set the cursor position.
     * 
     * @since S60 v5.0
     * @param aIndex. the cursor position you set.
     * @return None.
     */
    void SetCursor( const TInt aIndex );

    /**
     * From MAknFepUICtrlEditPane
     * get the position index of cursor.The max cursor index must be larger
     * than the length of text buffer by 1.
     * 
     * @since S60 v5.0
     * @param None.
     * @return. the position index.
     */
    TInt GetCursor();

    /**
     * From MAknFepUICtrlEditPane
     * move cursor to the left by a 1 stride.
     * 
     * @since S60 v5.0
     * @param aStride. the stride for motion
     * @return. succeed or not.
     */
    TBool MoveCursorLeft( TInt aStride = 1 );

    /**
     * From MAknFepUICtrlEditPane
     * move cursor to the right by a 1 stride.
     * 
     * @since S60 v5.0
     * @param aStride. the stride for motion
     * @return. succeed or not.
     */
    TBool MoveCursorRight( TInt aStride = 1);

    /**
     * From MAknFepUICtrlEditPane.
     * active the cursor,the cursor will be black and with a wink.this cursor
     * state represent this control is focused.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void ActiveCursor();

    /**
     * From MAknFepUICtrlEditPane.
     * deactive the cursor, the cursor will be gray and no wink this cursor
     * state represent this control with no focus.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void DeactiveCursor();

    /**
     * From MAknFepUICtrlEditPane.
     * enable the cursor,the cursor will be drawn.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void EnableCursor();

    /**
     * From MAknFepUICtrlEditPane.
     * deactive the cursor, the cursor will not be drawn.
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void DisableCursor();

    /**
     * From MAknFepUICtrlEditPane.
     * set the beginning and ending index for the underline text.
     * 
     * @since S60 v5.0
     * @param aBeg. the beginning index for underlined text.
     * @param aEnd. the ending index for underlined text.
     * @return None.
     */
    void SetUnderline( TInt aBeg, TInt aEnd );

    /**
     * From MAknFepUICtrlEditPane.
     * get the indexes for text with underline.
     * 
     * @since S60 v5.0
     * @param aBeg. a output param for receiving beginning index for underlined text.
     * @param aEnd. a output param for receiving ending index for underlined text.
     * @return None.
     */
    void GetUnderlineIndex( TInt& aBeg, TInt& aEnd );

    /**
     * From MAknFepUICtrlEditPane.
     * set the indexes for the text with highlight
     * 
     * @since S60 v5.0
     * @Param aBeg. the beginning index for highlighted text.
     * @param aEnd. the ending index for highlighted text.
     * @return None.
     */
    void SetHighlight( TInt aBeg, TInt aEnd );

    /**
     * From MAknFepUICtrlEditPane.
     * get the indexes for the text with highlight
     * 
     * @since S60 v5.0
     * @param aBeg. a output parameter which will receive the beginning index.
     * @param aEnd. a output parameter which will receive the ending index.
     * @return None.
     */
    void GetHighlightIndex( TInt& aBeg, TInt& aEnd );

    /**
     * From MAknFepUICtrlEditPane.
     * set keystroke index. 
     * 
     * @since S60 v5.0
     * @param aStartIndex. the keystroke index.
     * @return None.
     */
    void SetCursorIndexOfKeystroke( const TInt aStartIndex );

    /**
     * From MAknFepUICtrlEditPane.
     * get the keystroke index
     * 
     * @since S60 v5.0
     * @param None.
     * @return. the keystroke index.
     */
    TInt GetCursorIndexOfKeystroke();

    /**
     * From MAknFepUICtrlEditPane.
     * whether this control will be actived.ETrue represent enable otherwise
     * disable.
     * 
     * @since S60 v5.0
     * @param aEnable.ETrue represent enable this control,otherwise disable.
     * @return None.
     */
    void Enable( TBool aEnable );

    /**
     * From MAknFepUICtrlEditPane.
     * return whether this control is enabled.
     * 
     * @since S60 v5.0
     * @param None.
     * @return.ETrue represent this control is enabled, otherwise disabled.
     */
    TBool IsEnabled() const;

    /**
     * Get the info before cursor.
     * 
     * @since S60 v5.0
     * @param aText the show info.
     * @return none.
     */
    void GetText( TDes& aText );

    /**
     * Get the info before cursor.
     * 
     * @since S60 v5.0
     * @param aOut the info before cursor.
     * @return ETrue if can get the info.
     */
    TBool GetDesBeforeCursor( TDes& aOut );

    /**
     * From MAknFepUICtrlEditPane.
     * Not used.
     */
    void SetToneMark( const TDesC& aToneMark );

    /**
     * From MAknFepUICtrlEditPane.
     * Not used.
     */
    void SetToneMarkValidity( TBool aValid );

    /**
     * From MAknFepUICtrlEditPane.
     * Not used.
     */
    void SetOverrideFontId( TInt aFontId );

    /**
     * From MAknFepUICtrlEditPane.
     * Not used.
     */
    void SetFlag( TInt aFlag );

    /**
     * From MAknFepUICtrlEditPane.
     * Not used.
     */
    void ClearFlag( TInt aFlag );

    /**
     * From MAknFepUICtrlEditPane.
     * Not used.
     */
    TBool IsFlagSet( TInt aFlag ) const;

    /**
     * Set the cursor index for moving to the left.
     * 
     * @since S60 v5.0
     * @param aIndex. the cursor index for moving left.
     * @return None.
     */
    void SetCursorForMoveLeft( const TInt aIndex );

    /**
     * Set the cursor index for moving to the right.
     * 
     * @since S60 v5.0
     * @param aIndex. the cursor index for moving right.
     * @return None.
     */
    void SetCursorForMoveRight( const TInt aIndex );
    /**
     * Add some text at the end of buffer.
     * 
     * @since S60 v5.0
     * @param aDes. the content you want to append.
     * @return. ETrue represent success, otherwise failure.
     */
    TBool Append( const TDesC& aDes );

    /**
     * delete the content begin with aBeg index, and with an aLen length.
     * 
     * @since S60 v5.0
     * @param aBeg. the beginning index for delete.
     * @param aLen. the length you delete.
     * @return ETrue for success, and EFalse for failure.
     */
    TBool Delete( TInt aBeg, TInt aLen = 1 );

    /**
     * Insert some text after aIndex character.
     * 
     * @since S60 v5.0
     * @param aIndex. the index for insert.
     * @param aDes. the content you insert.
     * @return ETrue for success, and EFalse for failure.
     */
    TBool Insert( TInt aIndex, const TDesC& aDes );

    /**
     * Replace text from aBeg index to aBeg + aLen with aDes.
     * 
     * @since S60 v5.0
     * @param aBeg. the beginning index for replace.
     * @param aLen. the length for replace.
     * @param aDes. the new content after replacing.
     * @return ETrue for success, and EFalse for failure.
     */
    TBool Replace( TInt aBeg, TInt aLen, const TDesC& aDes );

    /**
     * Reset private member viariable.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
	void Reset();

	/**
     * Return kestroke array.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    CDesCArrayFlat* KeystrokeArray();
    
	/**
     * Return keycode array.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    RArray<TInt>* KeycodeArray();

    /**
     * Return show kestroke array. 
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    CDesCArrayFlat* ShowKeystrokeArray();

    /**
     * Return choose chinese charater array. 
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    CDesCArrayFlat* PhraseArray();

    /**
     * Return choose chinese charater show keystroke array. 
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    CDesCArrayFlat* PhraseShowKeyStrokeArray();
    
	/**
     * Return phrase keycode array.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    RArray<TInt>* PhraseKeycodeArray();
    
    /**
     * Return temp array. 
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return pointer of array.
     */
    CDesCArrayFlat* TempArray();    
    
    /**
     * Reset private member viariable array.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return None.
     */
    void ResetAllArray();
    
    /**
     * change state flag.
     * 
     * @since S60 v3.2.3
     * @param aState, ETrue change state.
     * @return None.
     */
    void SetChangeState( TBool aState = ETrue );
    
    /**
     * check change state flag.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue if change state, EFalse others.
     */
    TBool IsChangeState();

    /**
     * Set auto change state flag.
     * 
     * @since S60 v3.2.3
     * @param aState, ETrue auto change state, EFalse manual change state.
     * @return None.
     */
    void SetAutoChangeStateFlag( TBool aState = ETrue );
    
    /**
     * Get the auto change state flag.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue atuo change state, EFalse others.
     */
    TBool GetAutoChangeStateFlag();
    
    /**
     * Set all the keystroke valid flag.
     * 
     * @since S60 v3.2.3
     * @param aState, ETrue all the keystroke is valid, EFalse others.
     * @return None.
     */
    void SetAllValidFlag( TBool aState = ETrue );
    
    /**
     * Get all the keystroke valid flag.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue all the keystroke is valid, EFalse others.
     */
    TBool GetAllValidFlag();
    
    /**
     * Set Phrase Creation flag.
     * 
     * @since S60 v3.2.3
     * @param aState, ETrue is phrase creation, EFalse others.
     * @return None.
     */
    void SetPhraseCreationFlag( TBool aState );
    
    /**
     * Get Phrase Creation flag.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue is phrase creation, EFalse others.
     */
    TBool GetPhraseCreationFlag();
    
    /**
     * Set the keystroke invalid index.
     * 
     * @since S60 v3.2.3
     * @param aIndex, the keystroke invalid index.
     * @return None.
     */
    void SetInvalidIndex( TInt aIndex = 0 );
    
    /**
     * Get all the invalid keystroke index.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return invalid keystroke index.
     */
    TInt GetInvalidIndex();
    
    /**
     * Set Phrase Candidate state need refresh UI flag.
     * 
     * @since S60 v3.2.3
     * @param aState, ETrue is that Candidate state need refresh UI,other don't need.
     * @return None.
     */
    void SetCandidateRefreshFlag( TBool aState );
    
    /**
     * Get Phrase Candidate state need refresh UI flag.
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return ETrue is that Candidate state need refresh UI,EFalse don't need.
     */
    TBool GetCandidateRefreshFlag();
    
    /**
     * Set the delimiter count of keystroke.
     * 
     * @since S60 v3.2.3
     * @param aCount, the delimiter count of keystroke.
     * @return None.
     */
    void SetDelimiterCount( TInt aCount );
    
    /**
     * Get the delimiter count of keystroke. 
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return aCount, the delimiter count of keystroke.
     */
    TInt GetDelimiterCount();
    
    /**
     * Get current cba resource id. 
     * 
     * @since S60 v3.2.3
     * @param None.
     * @return current cba resource id.
     */
    TInt GetCurrentCBAResID();
    
    /**
     * Set current cba resource id. 
     * 
     * @since S60 v3.2.3
     * @param aResourceId, current cba resource id.
     * @return None.
     */
    void SetCurrentCBAResID( TInt aResourceId );
    
    void SetEffictiveLength( TInt aLength );
    
    TInt GetEffictiveLength();
    
    void DecEffictiveLength();
    
    void AddEffictiveLength();
    
    void SetNeedClearDeliberateSelection( TBool aNeedClearDeliberateSelection );
    
    TBool NeedClearDeliberateSelection();
    
    CDesCArrayFlat* PhraseZhuYinSpellingArray();
protected:

    /**
     * Layout contained controls.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void LayoutContainedControls();

    /**
     * layout the background frames.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void LayoutFrames();

    /**
     * layout its rectangles
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void LayoutRects();

private:
    /**
     * C++ constructor.
     */
    CAknFepUICtrlPinyinEEP( CAknFepUiLayoutDataMgr* aLafDataMgr );

    /**
     * Two-phrase constructor.
     */
    void ConstructL( RWindow& aParent, CAknFepUiLayoutDataMgr* aLafDataMgr );

    /**
     * Calculate visual text for appending.
     * 
     * @since S60 v5.0
     * @param aDes.the content you want to append.
     * @return None.
     */
    void CalculateVisualTextForAppend( const TDesC& aDes );

    /**
     * Calculate visual text for deleting.
     * 
     * @since S60 v5.0
     * @param aBeg. the beginning index you want to delete.
     * @param aLen. the length you want to delete, 1 by default.
     * @return None.
     */
    void CalculateVisualTextForDelete( TInt aBeg, TInt aLen = 1 );

    /**
     * Calculate visual text for inserting.
     * 
     * @since S60 v5.0
     * @param aBeg. the beginning index for your insert.
     * @param aDes. the content you want to insert.
     * @return None.
     */
    void CalculateVisualTextForInsert( TInt aBeg, const TDesC& aDes );

    /**
     * Calculate visual text for replacing.
     * 
     * @since S60 v5.0
     * @param aBeg. the beginning index for you replace.
     * @param aLen. the length for your replace.
     * @param aDes. the new content after your replace.
     * @return None.
     */
    void CalculateVisualTextForReplace( TInt aBeg, TInt aLen,
        const TDesC& aDes );

    /**
     * Calculate visual text for moving to left.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void CalculateVisualTextForMoveLeft();

    /**
     * Calculate visual text for moving to the right.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void CalculateVisualTextForMoveRight();

    /**
     * Calculate the underline indexes and highlight indexes according to the
     * absolute value.
     * 
     * @since S60 v5.0
     * @param None.
     * @return None.
     */
    void CalculateVisualIndex();

    /**
     * layout the background frames.
     *
     * @since S60 v5.0
     * @param aOuterRect filled with outer rect coords
     * @param aInnerRect filled with inner rect coords
     * @return None.
     */
    void CalculateFrameRects( TRect& aOuterRect, TRect& aInnerRect ) const;

private:
    // Data.
	CAknFepUiLayoutDataMgr* iLafDataMgr;
    /**
     * the position of keystroke.
     */
    TInt iIndex;
    /**
     * Whether EEP control is enabled ( visible ) .
     */
    TBool iEnabled;
    /**
     * The background context for this control.
     */
    CAknsFrameBackgroundControlContext* iBgContext;

    TAknLayoutRect iRectCoverMainPaneInput;
    TAknLayoutRect iRectFirstShadowInput;
    TAknLayoutRect iRectOutlineFrameInput;
    TAknLayoutRect iRectInsideAreaInput;
    /**
     * Its contained control only for display.
     * own
     */
    CAknFepUICtrlEEPControl* iEditableEntryPane;
    /**
     * All the text its caller set.
     * own
     */
    HBufC* iAbsoluteBuffer;
    /**
     * The text displayed on the contained control.
     * own
     */
    HBufC* iVisualBuffer;
    /**
     * The cursor position relative to iAbsoluteBuffer which is set by its
     * caller.
     */
    TInt iAbsoluteCursorPos;
    /**
     * The offset between the beginning position of iVisualBuffer and
     * iAbsoluteBuffer.
     */
    TInt iOffset;
    /**
     * The beginning position of underlined text.
     */
    TInt iAbsoluteUnderlineBeg;
    /**
     * The end position of underlined text.
     */
    TInt iAbsoluteUnderlineEnd;
    /**
     * The beginning position of highlighted text.
     */
    TInt iAbsoluteHighlightBeg;
    /**
     * The end position of highlighted text.
     */
    TInt iAbsoluteHighlightEnd;
    /**
     * Store the highlight-setting&underline-setting sequence
     */
    RPointerArray<CAknFepUICtrlEEPControl::TSegment> iArray;

    /**
     * Only for zhuyin use
     * the seed descriptor used in zhuyin input method. 
     * used to store zhuyin symbols every time.
     */
    TBuf<KMaxBufLen> iJudgeSeed;
    /**
     * used to store count for chinese character.
     */
    TInt iChineseCount;
    
    /**
     * whether the last displayed text is valid.
     */
    TBool iLastValid;
    
    /**
     * keystroke arrray.
     */
    CDesCArrayFlat* iKeystrokeArray;
    
    /**
     * Keycode arrray.
     */
    RArray<TInt> iKeycodeArray;

    /**
     * show Keystroke array.
     */
    CDesCArrayFlat* iShowKeystrokeArray;
    
    /**
     * Chinese array.
     */
    CDesCArrayFlat* iPhraseArray;
    
    /**
     * phrase Keycode arrray.
     */
    RArray<TInt> iPhraseKeycodeArray;
    
    /**
     * KeyStroke array for the Chinese which have been selected.
     */
    CDesCArrayFlat* iPhraseShowKeyStrokeArray;
    
    /**
     * The temp array.
     */
    CDesCArrayFlat* iTempArray;
    
    /**
     * the change state flag.
     */
    TInt iChangeFlag;
    
    /**
     * the auto change state flag.
     */
    TInt iAutoChangeFlag;
    
    /**
     * The all kyestroke is valid.
     */
    TInt iAllValidFlag;
    
    /**
     * The invalid keystroke index.
     */
    TInt iInvalidKeystrokeIndex;
    
    /**
     * Is phrase creation.
     */
    TInt iPhraseCreation;
    
    /**
     * The candidate refresh flag.
     */
    TInt iCandidateRefreshFlag;
    
    /**
     * The delimiter count of keystroke.
     */
    TInt iDelimiterCount;
    
    /**
     * Current CBA resource id.
     */
    TInt iCurrentCBAResId;
    
    TInt iEffictiveLength;
    
    TBool iMiniQwertyZhuyin;
    
    TBool iNeedClearDeliberateSelection;
    
    /**
	 * store ZhuYin Spelling
	 */
    CDesCArrayFlat* iPhraseZhuYinSpellingArray;
    };

#endif /* C_AKNFEPUICTRLPINYINEEP_H */

// End of File

