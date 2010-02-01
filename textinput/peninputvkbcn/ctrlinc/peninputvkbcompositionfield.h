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
* Description:  CAknFepVkbCompositionField
*
*/


#ifndef C_CAKNFEPVKBCOMPOSITIONFIELD_H
#define C_CAKNFEPVKBCOMPOSITIONFIELD_H

#include <peninputlayouteditareabase.h>

class MAknFepVkbDataProvider;
class CAknFepVkbPinyinAnalyser;

// property of Cpi engine, Private protocol since hard to modify ptiengine.h
const TInt KCpiMaxInputPhraseLen = 6;
/**
 *  Composition Element Type.
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2 
 */
enum TCompositionElementType
    {
    EElementTypeCompositionChar = 0x0001, //normal char
    EElementTypeAutoSeparator = 0x0002,   //auto separator
    EElementTypeManualSeparator = 0x0004,  //manual separator
    EElementTypeSeparator = EElementTypeAutoSeparator | EElementTypeManualSeparator,
    EElementTypeTonemark = 0x0008,        //tomark
    EElementTypeAuxilian = EElementTypeSeparator | EElementTypeTonemark,
    EElementTypeConvertedChar = 0x0010    //converted char from engine
    };
        
class CAknFepVkbCompositionField;

/**
 *  CCompositionElement.
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2 
 */
class CCompositionElement: public CBase
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aKey Input key
     * @return Pointer to created CCompositionElement object
     */
    static CCompositionElement* NewLC(const TDesC& aKey);    

    /**
     * destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CCompositionElement();
    
    /**
     * Get the length of original input
     *
     * @since S60 v3.2
     * @return The length of original input
     */
    virtual TInt OriginalCount() const;
    
    /**
     * Undo convert
     *
     * @since S60 v3.2
     * @param aField The owner of unconverted components
     * @param aStartPos The insert position
     * @return Inserted elements count
     */
    virtual TInt UndoConvertL(CAknFepVkbCompositionField* aField, TInt aStartPos);

    /**
     * Get the key
     *
     * @since S60 v3.2
     * @return The key
     */
    inline const TDesC* Key() const;

    /**
     * Compare function
     *
     * @since S60 v3.2
     * @return ETrue if the key value is equal to aKeyValue,
     *         EFalse otherwise
     */
    inline TBool operator == (TUint aKeyValue) const;
    
    /**
     * Determine the type of element
     *
     * @since S60 v3.2
     * @param aType The type of element to judge
     * @return ETrue if it is, otherwise EFalse
     */
    inline TBool IsTypeOf(TCompositionElementType aType) const;

    /**
     * Is this element is a separator?
     *
     * @since S60 v3.2
     * @return ETrue if element is a separator; otherwise EFalse
     */
    inline TBool IsSeparator() const;
    
    /**
     * Is this element is an atuo separator?
     *
     * @since S60 v3.2
     * @return ETrue if element is an auto separator; otherwise EFalse
     */
    inline TBool IsAutoSeparator() const;
    
    /**
     * Is this element is a manul separator?
     *
     * @since S60 v3.2
     * @return ETrue if element is a manul separator; otherwise EFalse
     */
    inline TBool IsManualSeparator() const;
    

    /**
     * Is this element is a tone mark?
     *
     * @since S60 v3.2
     * @return ETrue if element is a tone mark; otherwise EFalse
     */
    inline TBool IsTonemark() const;
    
    /**
     * Is this element is a auxilian(separator or tone mark)
     *
     * @since S60 v3.2
     * @return ETrue if element is a manul separator; otherwise EFalse
     */
    inline TBool IsAuxilian() const;
    
    /**
     * Is this element is visible?
     *
     * @since S60 v3.2
     * @return ETrue if element is visible; otherwise EFalse
     */
    inline TBool IsVisible();
    
    /**
     * Show/hide the element
     *
     * @since S60 v3.2
     * @param aHide Hide flag,
     * @return None
     */
    inline void Hide(TBool aHide);
    
    /**
     * Get the CCompositionElement pointer of original input
     *
     * @since S60 v3.2
     * @return The length of original input
     */
    virtual const CCompositionElement* OriginalContent( TInt aIndex ) const;
    
    
protected:

    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @return None
     */            
    CCompositionElement(TInt aKeyType);
    
    /**
     * 2nd constructor
     *
     * @since S60 v3.2
     * @param aKey Input key
     * @return None
     */            
    void BaseConstructL(const TDesC& aKey);
    
private:  //data

    /**
     * The key type
     */
    const TInt iKeyType;
    /**
     * The key (Own)
     * The iKey->Length() must be > 0 
     */
    HBufC* iKey;
   
    TBool iHide; 
    };
    
/**
 *  CCompositionConvertedElement.
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2 
 */
class CCompositionConvertedElement: public CCompositionElement
    {
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aKey Input key
     * @return Pointer to created CCompositionElement object
     */
    static CCompositionConvertedElement* NewLC(const TDesC& aKey);    

    /**
     * destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CCompositionConvertedElement();

    /**
     * Append a element to this converted element
     *
     * @since S60 v3.2
     * @param aElement The element to append
     * @return None
     */
    void AppendOriginal(const CCompositionElement* aElement);

// from CCompositionElement
    
    /**
     * From CCompositionElement
     * Undo convert
     *
     * @since S60 v3.2
     * @param aField The owner of unconverted components
     * @param aStartPos The insert position
     * @return Inserted elements count
     */
    TInt UndoConvertL(CAknFepVkbCompositionField* aField, TInt aStartPos);
    
    /**
     * From CCompositionElement
     * Get the length of original input
     *
     * @since S60 v3.2
     * @return The length of original input
     */
    TInt OriginalCount() const;
    
    /**
     * From CCompositionElement
     * Get the CCompositionElement pointer of original input
     *
     * @since S60 v3.2
     * @return The length of original input
     */
    virtual const CCompositionElement* OriginalContent( TInt aIndex ) const;

private:
    
    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @return None
     */            
    CCompositionConvertedElement();
    
    /**
     * 2nd constructor
     *
     * @since S60 v3.2
     * @param aKey Input key
     * @return None
     */            
    void ConstructL(const TDesC& aKey);

private:  //data
    
    /**
     * Original elements before converted
     */
    RPointerArray<CCompositionElement> iOriginalElements;
    
    };
    
/**
 *  CAknFepVkbCompositionField.
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2 
 */
class CAknFepVkbCompositionField: public CFepLayoutEditAreaBase
    {

public:
    
    friend class CCompositionConvertedElement;
    
    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aMaxLength Length limitation, 0 means no limitation
     * @param aRect  The control rect
     * @param aUiLayout The layout
     * @param aControlId The control id.
     * @return Pointer to created CAknFepVkbCompositionField object
     */
    static CAknFepVkbCompositionField* NewL(TInt aMaxLength,
                                            TRect aRect,
                                            CFepUiLayout* aUiLayout,
                                            TInt aControlId,
                                            MAknFepVkbDataProvider* aDataProvider);    

    /**
     * destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepVkbCompositionField();

    /**
     * HandleControlEvent
     *
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrl  The sender
     * @param aEventData The event data.
     * @return None
     */
    void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData);
    
    /**
     * Get the length of current string in composition field
     *
     * @since S60 v3.2
     * @return The length of current string in composition field
     */
    TInt Length() const ;  
    
    /**
     * Set warning color
     *
     * @since S60 v3.2
     * @param aColor The color.
     * @return None
     */
    void SetWarningColor(const TRgb& aColor);
        
    /**
     * Set normal color
     *
     * @since S60 v3.2
     * @param aColor The color.
     * @return None
     */
    void SetNormalColor(const TRgb& aColor); 
    
    /**
     * Is there need to create new word
     *
     * @since S60 v3.2
     * @return ETrue if there is need to create new word, otherwise EFalse
     */
    TBool NeedCreateWord();
    
     /**
     * Return the spell for new created words.
     *
     * @since S60 v3.2
     * @return The buffer for the gernated spell.
     */ 
    TPtrC GetCreatedWordSpell();
    
     /**
     * Return whether the inputted spell is invalid
     *
     * @since S60 v3.2
     * @return Return whether the inputted spell is invalid.
     */ 
    TBool IsValidSpell();

	void ConstructFromResourceL();
	
	void SizeChanged(TRect aRect);	
private:

    /**
     * C++ constructor
     *
     * @since S60 v3.2
     * @param aMaxLength Length limitation, 0 means no limitation
     * @param aRect  The control rect
     * @param aUiLayout The layout
     * @param aControlId The control id.
     * @return None
     */            
    CAknFepVkbCompositionField(TInt aMaxLength,
                              TRect aRect,
                              CFepUiLayout* aUiLayout,
                              TInt aControlId,
                              MAknFepVkbDataProvider* aDataProvider);

    /**
     * Second phase constructor
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructL();

    /**
     * Insert a key
     *
     * @since S60 v3.2
     * @param aKey The key to insert.
     * @return None
     */
    void HandleInsertL(const TDesC& aKey);

    /**
     * Generate first segment to analysis
     *
     * @since S60 v3.2
     * @return None
     */
    void GetFirstSegmentL();

    /**
     * Insert an element
     *
     * @since S60 v3.2
     * @param aElement The element to insert.
     * @return None
     */
    void InsertElementL(const CCompositionElement* aElement);

    /**
     * clear text
     *
     * @since S60 v3.2
     * @return None
     */
    void ClearTextL();
    
    /**
     * Handle back key
     *
     * @since S60 v3.2
     * @return None
     */
    void HandleBackL();
    
    /**
     * Submit all text
     *
     * @since S60 v3.2
     * @return None
     */
    void HandleFlushL();

    /**
     * Handle Analysis Response
     *
     * @since S60 v3.2
     * @param aCompText Analysis result text.
     * @return None
     */
    void HandleAnalysisResponseL(const TDesC& aCompText);

    /**
     * Handle converted result
     *
     * @since S60 v3.2
     * @param aConvertedText Converted result text.
     * @return None
     */
    void HandleConvertedResultL(const TDesC& aConvertedText);

    /**
     * Elements operation: remove
     *
     * @since S60 v3.2
     * @param aStartPos Start position.
     * @param aLength Length.
     * @return None
     */
    void Remove(TInt aStartPos, TInt aLength, TBool aDestroy = ETrue);
    /**
     * Elements operation: insert
     *
     * @since S60 v3.2
     * @param aPos Insert position.
     * @param aElement The element.
     * @return None
     */
    void InsertL(TInt aPos, const CCompositionElement* aElement);
    
    /**
     * Elements operation: clear
     *
     * @since S60 v3.2
     * @return None
     */
    void Clear();
    
    /**
     * Submit the keys in elements and the cursor to editbase
     *
     * @since S60 v3.2
     * @param aPos Insert position.
     * @return None
     */
    void UpdateTextL();

    /**
     * Determine whether elment is allowed to insert
     *
     * @since S60 v3.2
     * @param aElement Element to insert
     * @return ETrue for allowed otherwise EFalse;
     */
    TBool IsAllowedInsert(const CCompositionElement* aElement);

    /**
     * Send Analysis Request
     *
     * @since S60 v3.2
     * @param aRequest The request
     * @return None;
     */
    void SendAnalysisRequestL();
    
    /**
     * To test whether the all input keys are converted
     * to chinese characters
     *
     * @since S60 v3.2
     * @return ETrue if finished, EFalse otherwise.
     */
    TBool IsFinishComposition();
    
    /**
     * To test whether current pos element should be replaced by input one
     *
     * @since S60 v3.2
     * @param aElement The input element
     * @param aPos The pos of element to test
     * @return ETrue if current element should be replaced, EFalse otherwise.
     */
    TBool IsReplacePosition(const CCompositionElement* aElement, TInt aPos);
    
    /**
     * To test whether the one before current pos element should be replaced 
     * by input one
     *
     * @since S60 v3.2
     * @param aElement The input element
     * @return ETrue if current element should be replaced, EFalse otherwise.
     */
    TBool IsReplaceFront(const CCompositionElement* aElement);
    
    /**
     * To test whether current pos element should be replaced by input one
     *
     * @since S60 v3.2
     * @param aElement The input element
     * @return ETrue if current element should be replaced, EFalse otherwise.
     */
    TBool IsReplaceBack(const CCompositionElement* aElement);
    
    /**
     * Redo the format after deletion.
     *
     * @since S60 v3.2
     * @return None.
     */
    void ReFomatAfterDelete();
    
    /**
     * Get display cursor positon by internal cursor position
     *
     * @since S60 v3.2
     * @param aInternalPos The internal position
     * @return The display cursor position.
     */
    TInt InternalPosToDisplayPos(TInt aInternalPos);
    
    /**
     * Get internal cursor positon by display cursor position
     *
     * @since S60 v3.2
     * @param aInternalPos The internal position
     * @return The display cursor position.
     */
    TInt DisplayPosToInternaPos(TInt aDisplayPos);
    
    /**
     * Remove the selected text
     *
     * @since S60 v3.2
     * @return None
     */
    void RemoveSelection();
    
// from CFepLayoutEditAreaBase

    /**
     * From CFepLayoutEditAreaBase
     * Adjust the cursor when pointer down or move
     *
     * @since S60 v3.2
     * @return None
     */
    void AdjustSelectedCompositionText(TInt& aPositionOfInsertionPointInBuffer);

    /**
     * From CFepLayoutEditAreaBase
     * Update editbase content when scrolling
     *
     * @since S60 v3.2
     * @param aCursorPos The cursor position
     * @param aAnchorPos The anchor position
     * @return None.     
     */                  
    void UpdateContent(const TCursorSelection& aCursorSel);
    
private:  //data

    /**
     * Length limitation
     */
    TInt iMaxLength;
    
    /**
     * segment to analysis (Own)
     */
    HBufC* iFirstSegment;
    
    /**
     * first segment start position in buffer 
     */
    TInt iFirstSegmentOffset;

    /**
     * first segment length in buffer 
     * iFirstSegmentLength may not equal to iFirstSegment.Length
     * Because all automatical separators are removed in iFirstSegment
     */
    TInt iFirstSegmentLength;
    
    /**
     * Warning color 
     */
    TRgb iWarningColor;
    
    /**
     * Normal color 
     */
    TRgb iNormalColor;
    
    /**
     * Elements
     */
    RPointerArray<CCompositionElement> iElements;
    
    /**
     * Current cursor position of 
     * internal data
     */
    TInt iCurrentPos;
    /**
     * composition text start with manul separator
     */
    //TInt iSavedCursorPos;
    
    /**
     * Flag of is there need to create new word
     */
    TBool iNeedCreateWord;
    
    /**
     * The count of converted element
     */
    TInt iConvertedElement;
    
    /**
     * The count of converted element
     */
    TBuf<KCpiMaxInputPhraseLen * 8 + 1> iCreateWordSpellBuffer;
    
    /**
     * The analyser handle for spell decomposition
     */
    CAknFepVkbPinyinAnalyser* iPinyinAnalyser;
    CAknFepVkbPinyinAnalyser* iZhuyinAnalyser;
    
    /**
     * The data provider
     * Not own
     */
    MAknFepVkbDataProvider* iDataProvider;
    
    /**
     * Specify wether the spell is valid.
     */
    TInt iIsValidSpells;

    };

#include "peninputvkbcompositionfield.inl"    

#endif //C_CAKNFEPVKBCOMPOSITIONFIELD_H

//  End Of File
