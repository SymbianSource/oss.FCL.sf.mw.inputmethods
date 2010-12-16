/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implementation for chinese peninput finger hwr
*
*/

#ifndef C_PENINPUTFINGERHWRDATASTORE_H
#define C_PENINPUTFINGERHWRDATASTORE_H

//SYSTEM INCLUDES
#include <e32base.h>
#include <e32def.h>
#include <w32std.h>
#include <eikon.hrh>
#include <AknUtils.h>

//FEP INCLUDES
#include <PtiEngine.h>
#include <peninputpluginutils.h>


//USER INCLUDES
#include "peninputfingerhwrstoreconstants.h"

//CONSTANTS
const TInt KLanguageStringLength = 10;

//FOWWARD CLASS
class CPeninputFingerHwrLayout;
class CAknFepHwrTriggerStr;
class CRepository;
class CAknFepHwrEngine;
class CPtiEngine;


/**
 *  CPeninputFingerHwrDataStore
 *
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
class CPeninputFingerHwrDataStore : public CBase
    {
public:
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @param aPtiEngine The PtiEngine
     * @param aLayout The Finger HWR layout
     * @return Pointer to created CPeninputFingerHwrDataStore object
     */
    static CPeninputFingerHwrDataStore* NewL( CPtiEngine* aPtiEngine, 
        CPeninputFingerHwrLayout* aLayout );

    /**
     * destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputFingerHwrDataStore();


public:    
    /**
     * Is valid candidate
     *
     * @since S60 v5.0
     * @param aCandidateIndex The index the candidate in list.
     * @return ETrue if candidate is not empty; otherwise EFalse
     */    
    TBool IsValidCandidate( TInt aCandidateIndex ) const;
    

    /**
     * Is English letter
     *
     * @since S60 v5.0
     * @param aLetter The recognize character.
     * @return ETrue if the character is English letter; otherwise EFalse
     */   
    TBool IsEnglishLetter( const TDesC& aLetter);
         
    /**
     * Do recoginize by engine
     *
     * @since S60 v5.0
     * @param aTraceData The trace data.
     * @return None
     */            
    void DoChineseRecognizeL( const RArray<TPoint>& aTraceData );

    /**
     * Do recoginize in English range
     *
     * @since S60 v5.0
     * @param aTraceData The trace data.
     * @return None
     */
    void DoEngRecognizeL( const RArray<TPoint>& aTraceData);

    /**
     * Do predictive using trigger string
     *
     * @since S60 v5.0
     * @return None
     */          
    void DoPredictiveL();
    
    /**
     * Set Chinese symbol to writing candidate list
     *
     * @since S60 v5.0
     * @return None
     */      
    void SetChineseSymbolL();
    
    /**
     * Set fixed Chinese predictives to predictive list
     *
     * @since S60 v5.0
     * @return None
     */    
    void SetFixChinesePredictiveL();
    
    /**
     * Set fixed English symbol
     *
     * @since S60 v5.0
     * @param aRowCount The flag to confirm which row
     * @return None
     */   
    void SetFixEnglishSymbolL(TRowCount aRowCount = ERowOne);
    
       
    /**
     * Get the predictive count
     *
     * @since S60 v5.0
     * @return The count of predictives
     */            
    TInt PredictiveCount() const;

    /**
     * Get the trigger string
     *
     * @since S60 v5.0
     * @return Pointer to CAknFepHwrTriggerStr object
     */            
    CAknFepHwrTriggerStr* TriggerStr() const;

    /**
     * Set permitted ranges
     *
     * @since S60 v5.0
     * @param aPermittedRanges The permitted ranges.
     * @return None
     */            
    void SetPermittedRanges( TInt aPermittedRanges );  
    
    /**
     * Set permitted ranges
     *
     * @since S60 v5.0
     * @param aPermittedRanges The permitted ranges.
     * @return None
     */            
    void SetPermittedCases( TInt aPermittedCases );
    
   
    /**
     * Set Primary range
     *
     * @since S60 v5.0
     * @param aPrimaryRange The Primary range
     * @return None
     */            
    void SetPrimaryRange( TInt aPrimaryRange );
    
    /**
     * Get Primary range
     *
     * @since S60 v5.0
     * @return Primary range.
     */
    TInt PrimaryRange();
    
    /**
     * Get current range
     *
     * @since S60 v5.0
     * @return Current range.
     */   
    TFingerHwrRange CurrentRange();
    
     
    /**
     * Set case
     *
     * @since S60 v5.0
     * @param aCase The case
     * @return None
     */            
    void SetCase( TInt aCase );
    
    /**
     * Set language
     *
     * @since S60 v5.0
     * @param aLanguage The language
     * @return None
     */            
    void SetLanguageL( TInt aLanguage );
         
    /**
     * Set the candidate list.
     *
     * @since S60 v5.0
     * @return None
     */
    void SetCandidatesL();
    

    /**
     * Set auto complete candidate list.
     *
     * @since S60 v5.0
     * @return None
     */
    void SetAutoCompleteCandidatesL();
    
    /**
     * Set auto complete candidate list.
     *
     * @since S60 v5.0
     * @param aIsFirstStroke The first stroke flag
     * @return None
     */
    void SetFirstStroke( const TBool aIsFirstStroke);
    
    /**
     * Set auto complete candidate list.
     *
     * @since S60 v5.0
     * @return ETrue if it's the first stroke
     */
    TBool FirstStroke();   
 
    
    /**
     * Get the candidate.
     *
     * @since S60 v5.0
     * @param aIndex The index of candidate
     * @param aCharCode The result candidate
     * @return Candidate char code
     */
    const RPointerArray<HBufC>& Candidate() const;
    
    /**
     * Get the predictive.
     *
     * @since S60 v5.0
     * @param aIndex The index of predictive
     * @param aCharCode The result predictive
     * @return Predictive char code
     */
    const RPointerArray<HBufC>& Predictive();  
    
    /**
     * Set number mode
     *
     * @since S60 v5.0
     * @param aNumberMode Given number mode
     * @return None
     */
    void SetNumberMode( const TAknEditorNumericKeymap aNumberMode );
    
    /**
     * Set number mode
     *
     * @since S60 v5.0
     * @param aCustomMode The customized number key mapping, 0-9
     *                    are not included.
     * @return None
     */
    void SetCustomNumberModeL( const TDesC& aCustomMode );

    /**
     * Set the screen mode.
     *
     * @since S60 v5.0
     * @param aRange The range
     * @return None
     */
    void SaveRange( TInt aRange );
    

    
    /**
     * Get next predictive page
     *
     * @since S60 v5.0
     * @return None
     */            
    void GetNextPredictivePageL();
    
    /**
     * Set stroke end mark to stroke end mark control
     *
     * @since S60 v5.0
     * @return None
     */
    void SetStrokeEndMark();

    /**
     * Get the stroke end mark.
     *
     * @since S60 v5.0
     * @return TPoint
     */
    TPoint StrokeEndMark();
    
    /**
     * Get stroke end mark from control which handle it
     *
     * @since S60 v5.0
     * @return None
     */
    TPoint StrokeEndMarkFromControl();

    /**
     * Get number keymapping string
     *
     * @since S60 v5.0
     * @param aKeyMappingId Keymapping id
     * @return Pointer to keymapping string 
     */    
    HBufC* KeyMappingStringL() const;
    
    /**
     * Check if the char is the special display char
     * 
     * @since S60 v5.0
     * @param aChar The checked char
     * @return TBool Wheater or not the input char is the special display char
     */
    TBool IsSpecialDisplayChars( const TDesC& aChar ) const;
     
    /**
     * Check if the char is need to be sent to app directly which not need to be shown  in candidate window.
     * 
     * @since S60 v5.0
     * @param aChar The checked char
     * @return TBool Wheater or not the input char is the directly sent char
     */
    TBool IsDirectlySentCandidate( const TDesC& aChar ) const;
    
    /**
     * Convert the special display char to the keycode.
     *
     * @since S60 v5.0
     * @param aChar The special display char.
     * @return HBufC* The converted keycode char.
     */
    HBufC* ConvertDisplayChars( const TDesC& aChar ) const;
    
    /**
     * Get start writing char flag
     *
     * @since S60 v5.0
     * @return ETrue If begine writing.
     */   
    TBool StartCharacter();

    /**
     * Set start writing char flag
     *
     * @since S60 v5.0
     * @param aStartCharacter The start writing flag
     * @return ETrue If begine writing.
     */   
    void SetStartCharacter(const TBool aStartCharacter);
  
    /**
     * Set input area size to engine for recognize
     *
     * @since S60 v5.0
     * @param aStartCharacter The start writing flag
     * @return ETrue If begine writing.
     */   
    TInt SetInputAreaSize(TSize& aSize);
     
    /**
     * Set screen size to engine for recognize
     *
     * @since S60 v5.0
     * @param aStartCharacter The start writing flag
     * @return ETrue If begine writing.
     */        
    TInt SetScreenSize(TSize& aSize);
         
    /**
     * Clear predictive array
     *
     * @since S60 v5.0
     * @return None
     */    
    void ClearPredictive();
    
    /**
     * Set high light flag for candidate list
     *
     * @since S60 v5.0
     * @param aIsHighlight The flag to decide highlight default cell or not
     * @return None
     */   
    void SetHighlight(const TBool aIsHighlight);

    /**
     * Get high light flag for candidate list
     *
     * @since S60 v5.0
     * @return ETrue If highlight the default cell to candidate list
     */   
    TBool Highlight();
    
    /**
     * Remove duplicate candidate from predictive candidate list
     *
     * @since S60 v5.0
     * @param aSrc The punctuation candidate list
     * @param aTgt The predicative candidate list
     * @param aSrcStartIdx 
     * @param aTgtStartIdx 
     * @return last overlap index
     */   
    TInt RemoveDuplicateCand(const RPointerArray<HBufC>& aSrc,
                             RPointerArray<HBufC>& aTgt,
                             TInt aSrcStartIdx,
                             TInt aTgtStartIdx);
    
    /**
     * Set key board type to qwerty
     *
     * @since S60 v5.0
     */   
    void SetKeyboardToQwerty();

private:  

    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return None
     */            
    CPeninputFingerHwrDataStore(CPeninputFingerHwrLayout* aLayout);
    
    /**
     * Second phase constructor
     *
     * @since S60 v5.0
     * @return None
     */
    void ConstructL( CPtiEngine* aPtiEngine );
   
   /**
     * Initialize the key mapping list
     *
     * @since S60 v5.0
     * @return None
     */  
    void InitKeyMappingListL();    
    
    /**
     * Judge if a language is Chinese
     *
     * @since S60 v5.0
     * @param aLanguage Given language code
     * @return ETrue if aLanguage is Chinese, otherwise EFalse
     */
    TBool LanguageIsChinese( TInt aLanguage );
    
    /**
     * Set engine language
     *
     * @since S60 v5.0
     * @param aLanguage Given language code
     * @return None
     */
    void SetEngineLanguageL( TInt aLanguage );    
    
    /**
     * Set language show text
     *
     * @since S60 v5.0
     * @param aLanguage language code
     * @return None
     */
    void SetLanguageShowText( TInt aLanguage );   
    
    /**
     * Remove accent character
     *
     * @since S60 v5.0
     * @return None
     */
    void RemoveAccent();
      
private:// data
        
    /**
     * Trigger string (Own)
     */
    CAknFepHwrTriggerStr* iTriggerStr;
    
    /**
     * predictive list (Own)
     */
    RPointerArray<HBufC> iPredictives;

    /**
     * candidate list (Own)
     */
    RPointerArray<HBufC> iCandidates;
    
    /**
     * The candidate range start index
     */
    RArray<TInt> iRangeStartPos;

    /**
     * Permitted ranges
     */
    RArray<TInt> iPermittedRanges;
    
    /**
     * Current range
     */
    TFingerHwrRange iCurrentRange;
    
    /**
     * Permitted range in int format
     */
    TInt iIntRange;
    
    /**
     * Current case
     */
    TInt iCase;

    /**
     * Current language
     */
    TInt iLanguage;
    
    /**
     * Pen size
     */
    TSize iPenSize;

    /**
     * Pen color
     */
    TRgb iPenColor;
    
    /**
     * The repository (Own);
     */
    CRepository* iRepositoryFep;

    /**
     * The engine (Own);
     */
    CAknFepHwrEngine* iHwrEngine;
    
    /**
     * The permited cases
     */
    TInt iPermittedCases;   
    
    /**
     * The current number key map
     */   
    TAknEditorNumericKeymap iCurrentNumberMode;
   
    /**
     * Keymapping id array
     */  
    RArray<TInt> iKeyMappingIdList;
    
    /**
     * Keymapping string array
     */          
    RPointerArray<HBufC> iKeyMappingStringList;

    /**
     * Language show text
     */
    TBuf<KLanguageStringLength> iLanguageShowText;
    
    /**
     * User defined key mapping (Own)
     */
    HBufC* iUserDefinedResource;
    
    /**
     * Finger hwr layout (not own)
     */
    CPeninputFingerHwrLayout* iLayout;
    
    /**
     * Start writing flag
     */
    TBool iStartCharacter;
    
    /**
     * Highlight default writing candidate flag
     */
    TBool iIsHighlightCell;
    
    /**
     * The first stroke flag
     */
    TBool iIsFirstStroke;
    
    /**
     * default key mapping for normal editor(not number-only).
     * Own
     */
    HBufC* iDefaultNumberMapping;
 };


#endif	//C_PENINPUTFINGERHWRDATASTORE_H
