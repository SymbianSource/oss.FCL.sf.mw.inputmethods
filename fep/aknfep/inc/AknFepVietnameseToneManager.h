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
*
*/












#ifndef __AKN_FEP_VIETNAMESE_TONE_MANAGER__
#define __AKN_FEP_VIETNAMESE_TONE_MANAGER__

#include <e32def.h>
#include <e32base.h>

#include "AknFepManager.h"
#include "AknFepVietnameseInputTables.h"

// CONSTANTS

// MACROS

// DATA TYPES

// FUNCTION PROTOTYPES

// FORWARD DECLARATIONS

// CLASS DECLARATION

/**
 * CAknFepVietnameseToneManager class.
 * This class is mounted based on CAknFepUIManagerWestern.
 *
 *  @lib AknFep.lib
 *  @since 2.8
 */
class CAknFepVietnameseToneManager : public CBase
    {
private:
     /**
     * Construct
     *
     * @param aFepManager fep manager
     *
     */
    void ConstructL();

     /**
     * private constructor
     */
    CAknFepVietnameseToneManager();     
    
public:
     /**
     * NewL first phase construction
     *
     * @param aFepManager fep manager
     *
     */
    static CAknFepVietnameseToneManager* NewL();

     /**
     * Destructor
     */
    virtual ~CAknFepVietnameseToneManager();
    
public:
    /**
     * Start the tone mark looping 
     */
    TBool StartToneMarkLooping();

    /**
     * first phase construction
     *
     * @param aParent parent window
     * @param aPaneLayout the required layout
     *
     */
    TText GetVowelWithToneMark() const;

    /**
     * Increase the tone mark
     */
    inline void IncrementToneMark();

    /**
     * stop tone marking looping, counterpart to StartToneMarkLooping
     */
    void StopToneMarkLooping();

    /**
     * Set the fep manager pointer
     *
     * @param aFepManager Pointer to the fep manager
     *
     */
    inline void SetFepManager(MAknFepManagerUIInterface* aFepManager);

    /**
     * Judge if the current state is looping or not
     */
    inline TBool IsLooping() const;

    /**
     * Judge if the current state is looping or not
     */
    inline void SetDeletedVowel(TText aDeletedVowel);
    
    /**
     * Judge if the current state is looping or not
     */
    inline TText GetDeletedVowel() const;
    
    /**
     * Start key looping 
     * 
     * @param aKey current looping key
     * 
     */    
    void StartKeyLooping(TInt aKey);
    
    /**
     * stop key looping, counterpart to StartKeyLooping
     */
    void StopKeyLooping();

    /**
     * Get current mark index in key looping
     * 
     */      
    TInt ToneMarkIndex() const;
    
    /**
     * Get tone mark vowel in current key looping
     * 
     * @param aText return the tone mark vowel
     * 
     */     
    TBool GetLoopingToneMarkVowel( TText& aText ) const;
    
    /**
     * Judge if the current state is key looping or not
     * 
     * @param aKey current looping key
     * 
     */    
    inline TBool IsKeyLooping(TInt aKey) const;
    
    /**
     * Judge if the current state is key looping or not
     */      
    inline TBool IsLoopingCombined() const;
    
    /**
     * Set whether tone mark is combined in previous looping
     * 
     * @param aCombined tone mark is combined
     * 
     */ 
    inline void SetLoopingCombined(TBool aCombined);

private:    // Data
    MAknFepManagerUIInterface* iFepMan;     // not own
    TInt iToneIndex;                        // Index of tone mark list
    TInt iVowelIndex;                       // Index of vowel list
    TBool iIsLooping;                       // Judge if input is in looping state
    TText iPrevVowel;                       // Prev Vowel, used for undo the backdeletion
    
    TBool iIsKeyLooping;                    // Judge if input is in loop state, 
                                            // iIsLooping is only used for star key
    TInt iLoopingKey;                       // Current looping key
    TBool iIsLoopingCombined;               // Judge if tone mark is combined in previous looping 
    };

// Inline function implementation
inline TBool CAknFepVietnameseToneManager::IsLooping() const
    {
    return iIsLooping;
    }

inline void CAknFepVietnameseToneManager::SetFepManager(MAknFepManagerUIInterface* aFepManager)
    {
    iFepMan = aFepManager;
    }

inline void CAknFepVietnameseToneManager::IncrementToneMark()
    {
    if (iToneIndex+1 == KNumberOfToneMarks)
        iToneIndex = 0;
    else
        ++iToneIndex;
    }

inline void CAknFepVietnameseToneManager::SetDeletedVowel(TText aDeletedVowel)
    {
    iPrevVowel = aDeletedVowel;
    }

inline TText CAknFepVietnameseToneManager::GetDeletedVowel() const
    {
    return iPrevVowel;
    }

inline TBool CAknFepVietnameseToneManager::IsKeyLooping(TInt aKey) const
    {
    return iIsKeyLooping && (iLoopingKey == aKey) ;
    }

inline TBool CAknFepVietnameseToneManager::IsLoopingCombined() const
    {
    return iIsLoopingCombined ;
    }

inline void CAknFepVietnameseToneManager::SetLoopingCombined(TBool aCombined)
    {
    iIsLoopingCombined = aCombined ;
    }

#endif // __AKN_FEP_VIETNAMESE_TONE_MANAGER__

// End of file
