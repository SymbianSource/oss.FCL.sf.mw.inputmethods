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

private:    // Data
    MAknFepManagerUIInterface* iFepMan;     // not own
    TInt iToneIndex;                        // Index of tone mark list
    TInt iVowelIndex;                       // Index of vowel list
    TBool iIsLooping;                       // Judge if input is in looping state
    TText iPrevVowel;                       // Prev Vowel, used for undo the backdeletion
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

#endif // __AKN_FEP_VIETNAMESE_TONE_MANAGER__

// End of file
