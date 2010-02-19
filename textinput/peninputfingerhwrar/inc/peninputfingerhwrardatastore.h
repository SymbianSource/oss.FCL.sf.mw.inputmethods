/*
* Copyright (c) 2005-2008 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of "Eclipse Public License v1.0"
* which accompanies this distribution, and is available
* at the URL "http://www.eclipse.org/legal/epl-v10.html".
*
* Initial Contributors:
* Nokia Corporation - initial contribution.
*
* Contributors:
*
* Description:  Implementation for arabic peninput finger hwr
*
*/


#ifndef C_PENINPUTFINGERHWRARDATASTORE_H
#define C_PENINPUTFINGERHWRARDATASTORE_H

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
#include "peninputfingerhwrarstoreconstants.h"
#include "peninputfingerhwrar.hrh"
//CONSTANTS
const TInt KLanguageStringLength = 10;

//FOWWARD CLASS
class CPeninputFingerHwrArLayout;
class CRepository;
class CPeninputFingerHwrArEngine;
class CPtiEngine;


/**
 *  CPeninputFingerHwrArDataStore
 *
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
class CPeninputFingerHwrArDataStore : public CBase
    {
public:
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @param aPtiEngine The PtiEngine
     * @param aLayout The Finger HWR layout
     * @return Pointer to created CPeninputFingerHwrArDataStore object
     */
    static CPeninputFingerHwrArDataStore* NewL( CPtiEngine* aPtiEngine, 
        CPeninputFingerHwrArLayout* aLayout );

    /**
     * destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputFingerHwrArDataStore();


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
    void DoRecognizeL( const RArray<TPoint>& aTraceData );

    
    /**
     * Set Chinese symbol to writing candidate list
     *
     * @since S60 v5.0
     * @return None
     */      
    void SetArabicSymbolL();
    
    
    /**
     * Set fixed English symbol
     *
     * @since S60 v5.0
     * @param aRowCount The flag to confirm which row
     * @return None
     */   
    void SetFixEnglishSymbolL(TRowCount aRowCount = ERowOne);
    
       

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
     * Reset key board type
     *
     * @since S60 v5.0
     */   
    void ResetKeyboardType();
    
    /**
     * Set key board type to qwerty
     *
     * @since S60 v5.0
     */   
    void SetKeyboardToQwerty();
    
    /**
     * Get key board type
     *
     * @since S60 v5.0
     */   
    void GetKeyboardType();
    
	/**
     * Get top guide line from hwr engine
     *
     * @since S60 v5.0
     */
    void GetTopGuideLinePos(TInt& aPos);
    
	/**
     * Get bottom guide line from hwr engine
     *
     * @since S60 v5.0
     */
	void GetBottomGuideLinePos(TInt& aPos);
	
    /**
     * Set first candidate type according to editor's text in cursor.
     * @since s60 v5.2
     */
    void SetFirstCandidateType(TFirstCandidateType aFirstCandType);
    
    /**
     * Check if the passed char is latin number
     *
     * @since s60 v5.2
     * @param aChar The passed unicode
     * @return ETrue if it is, EFalse otherwise
     */
    TBool IsLatinNumber(TUint16 aChar);
    
    /**
     * Check if the passed char is latin char
     *
     * @since s60 v5.2
     * @param aChar The passed unicode
     * @return ETrue if it is, EFalse otherwise
     */
    TBool IsLatinChar(TUint16 aChar);
    
    /**
     * Check if the passed char is arabic number
     *
     * @since s60 v5.2
     * @param aChar The passed unicode
     * @return ETrue if it is, EFalse otherwise
     */
    TBool IsArabicNumber(TUint16 aChar);

    /**
     * Check if the passed char is arabic character
     *
     * @since s60 v5.2
     * @param aChar The passed unicode
     * @return ETrue if it is, EFalse otherwise
     */
    TBool IsArabicChar(TUint16 aChar);

    /**
     * Check if the passed char is arabic symbols
     *
     * @since s60 v5.2
     * @param aChar The passed unicode
     * @return ETrue if it is, EFalse otherwise
     */
    TBool IsArabicSymbol(TUint16 aChar);
    
private:  

    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return None
     */            
    CPeninputFingerHwrArDataStore(CPeninputFingerHwrArLayout* aLayout);
    
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
     * Judge if a language is Arabic
     *
     * @since S60 v5.0
     * @param aLanguage Given language code
     * @return ETrue if aLanguage is Chinese, otherwise EFalse
     */
    TBool IsArabicLanguage( TInt aLanguage );

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
     * Reorder candidate list according to pre character type in editor.
     * @since s60 v5.2
     * @param None.
     * @return None.
     */
    void ReorderCandidates();
	
private:// data
        

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
    CPeninputFingerHwrArEngine* iHwrEngine;
    
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
    CPeninputFingerHwrArLayout* iLayout;
    
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
    
    /**
     * First candidate type.
     */
    TFirstCandidateType iFirstCandidateType;
 };


#endif	//C_PENINPUTFINGERHWRARDATASTORE_H
