/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
#include <aknutils.h>

//FEP INCLUDES
#include <ptiengine.h>
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
 *  @since Symbian TB9.2
 */
class CPeninputFingerHwrArDataStore : public CBase
    {
public:
    /**
     * Symbian constructor.
     *
     * @since Symbian TB9.2
     * @param aPtiEngine The PtiEngine
     * @param aLayout The Finger HWR layout
     * @return Pointer to created CPeninputFingerHwrArDataStore object
     */
    static CPeninputFingerHwrArDataStore* NewL( CPtiEngine* aPtiEngine, 
        CPeninputFingerHwrArLayout* aLayout );

    /**
     * destructor
     *
     * @since Symbian TB9.2
     * @return None
     */
    virtual ~CPeninputFingerHwrArDataStore();


public:    
    /**
     * Is valid candidate
     *
     * @since Symbian TB9.2
     * @param aCandidateIndex The index the candidate in list.
     * @return ETrue if candidate is not empty; otherwise EFalse
     */    
    TBool IsValidCandidate( TInt aCandidateIndex ) const;
    

    /**
     * Is English letter
     *
     * @since Symbian TB9.2
     * @param aLetter The recognize character.
     * @return ETrue if the character is English letter; otherwise EFalse
     */   
    TBool IsEnglishLetter( const TDesC& aLetter);
         
    /**
     * Do recoginize by engine
     *
     * @since Symbian TB9.2
     * @param aTraceData The trace data.
     * @return None
     */            
    void DoRecognizeL( const RArray<TPoint>& aTraceData );

    
    /**
     * Set Chinese symbol to writing candidate list
     *
     * @since Symbian TB9.2
     * @return None
     */      
    void SetArabicSymbolL();
    
    /**
     * Set permitted ranges
     *
     * @since Symbian TB9.2
     * @param aPermittedRanges The permitted ranges.
     * @return None
     */            
    void SetPermittedRanges( TInt aPermittedRanges );  
    
    /**
     * Set permitted ranges
     *
     * @since Symbian TB9.2
     * @param aPermittedRanges The permitted ranges.
     * @return None
     */            
    void SetPermittedCases( TInt aPermittedCases );
    
       
    /**
     * Get Primary range
     *
     * @since Symbian TB9.2
     * @return Primary range.
     */
    TInt PrimaryRange();
    
    /**
     * Get current range
     *
     * @since Symbian TB9.2
     * @return Current range.
     */   
    TFingerHwrRange CurrentRange();
    
     
    /**
     * Set case
     *
     * @since Symbian TB9.2
     * @param aCase The case
     * @return None
     */            
    void SetCase( TInt aCase );
    
    /**
     * Set language
     *
     * @since Symbian TB9.2
     * @param aLanguage The language
     * @return None
     */            
    void SetLanguageL( TInt aLanguage );
         
    
    /**
     * Set auto complete candidate list.
     *
     * @since Symbian TB9.2
     * @param aIsFirstStroke The first stroke flag
     * @return None
     */
    void SetFirstStroke( const TBool aIsFirstStroke);
    
    /**
     * Set auto complete candidate list.
     *
     * @since Symbian TB9.2
     * @return ETrue if it's the first stroke
     */
    TBool FirstStroke();   
 
    
    /**
     * Get the candidate.
     *
     * @since Symbian TB9.2
     * @param aIndex The index of candidate
     * @param aCharCode The result candidate
     * @return Candidate char code
     */
    const RPointerArray<HBufC>& Candidate() const;
    
    
    /**
     * Set number mode
     *
     * @since Symbian TB9.2
     * @param aNumberMode Given number mode
     * @return None
     */
    void SetNumberMode( const TAknEditorNumericKeymap aNumberMode );
    
    /**
     * Set number mode
     *
     * @since Symbian TB9.2
     * @param aCustomMode The customized number key mapping, 0-9
     *                    are not included.
     * @return None
     */
    void SetCustomNumberModeL( const TDesC& aCustomMode );

    /**
     * Set the screen mode.
     *
     * @since Symbian TB9.2
     * @param aRange The range
     * @return None
     */
    void SaveRange( TInt aRange );
    

    
    
    /**
     * Set stroke end mark to stroke end mark control
     *
     * @since Symbian TB9.2
     * @return None
     */
    void SetStrokeEndMark();

    /**
     * Get the stroke end mark.
     *
     * @since Symbian TB9.2
     * @return TPoint
     */
    TPoint StrokeEndMark();
    
    /**
     * Get stroke end mark from control which handle it
     *
     * @since Symbian TB9.2
     * @return None
     */
    TPoint StrokeEndMarkFromControl();

    /**
     * Get number keymapping string
     *
     * @since Symbian TB9.2
     * @param aKeyMappingId Keymapping id
     * @return Pointer to keymapping string 
     */    
    HBufC* KeyMappingStringL() const;
    
    /**
     * Get start writing char flag
     *
     * @since Symbian TB9.2
     * @return ETrue If begine writing.
     */   
    TBool StartCharacter();

    /**
     * Set start writing char flag
     *
     * @since Symbian TB9.2
     * @param aStartCharacter The start writing flag
     * @return ETrue If begine writing.
     */   
    void SetStartCharacter(const TBool aStartCharacter);
  
    /**
     * Set input area size to engine for recognize
     *
     * @since Symbian TB9.2
     * @param aStartCharacter The start writing flag
     * @return ETrue If begine writing.
     */   
    TInt SetInputAreaSize(TSize& aSize);
     
    /**
     * Set screen size to engine for recognize
     *
     * @since Symbian TB9.2
     * @param aStartCharacter The start writing flag
     * @return ETrue If begine writing.
     */        
    TInt SetScreenSize(TSize& aSize);
         
    
    /**
     * Set high light flag for candidate list
     *
     * @since Symbian TB9.2
     * @param aIsHighlight The flag to decide highlight default cell or not
     * @return None
     */   
    void SetHighlight(const TBool aIsHighlight);

    /**
     * Get high light flag for candidate list
     *
     * @since Symbian TB9.2
     * @return ETrue If highlight the default cell to candidate list
     */   
    TBool Highlight();
    
    /**
     * Remove duplicate candidate from predictive candidate list
     *
     * @since Symbian TB9.2
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
     * Get top guide line from hwr engine
     *
     * @since Symbian TB9.2
     */
    void GetTopGuideLinePos(TInt& aPos);
    
	/**
     * Get bottom guide line from hwr engine
     *
     * @since Symbian TB9.2
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
private:  

    /**
     * C++ constructor
     *
     * @since Symbian TB9.2
     * @return None
     */            
    CPeninputFingerHwrArDataStore(CPeninputFingerHwrArLayout* aLayout);
    
    /**
     * Second phase constructor
     *
     * @since Symbian TB9.2
     * @return None
     */
    void ConstructL( CPtiEngine* aPtiEngine );
   
   /**
     * Initialize the key mapping list
     *
     * @since Symbian TB9.2
     * @return None
     */  
    void InitKeyMappingListL();    
    
    
    /**
     * Judge if a language is Arabic
     *
     * @since Symbian TB9.2
     * @param aLanguage Given language code
     * @return ETrue if aLanguage is Chinese, otherwise EFalse
     */
    TBool IsArabicLanguage( TInt aLanguage );

    /**
     * Set engine language
     *
     * @since Symbian TB9.2
     * @param aLanguage Given language code
     * @return None
     */
    void SetEngineLanguageL( TInt aLanguage );    
    
    /**
     * Set language show text
     *
     * @since Symbian TB9.2
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
