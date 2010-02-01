/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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












#ifndef _AKNFEP_HASHKEY_MANAGER_H__
#define _AKNFEP_HASHKEY_MANAGER_H__

#include <e32base.h>
#include <coedef.h>
#include <frmtlay.h>

#include "AknFepGlobalEnums.h"

class CAknFepManager;
class CAknFepCaseManager;
enum TKeyResponse;
enum TKeyPressLength;

class CAknFepHashKeyManager : public CBase
    {
public:
    enum THashKeyStyle
        {
        EHashKeyStyleWestern,
        EHashKeyStyleChineseWithWestern,
        EHashKeyStyleJapanese,
        EHashKeyStyleKoreanWithWestern
        };

public:
    static CAknFepHashKeyManager* NewL(CAknFepManager& aFepMan, CAknFepCaseManager* aCaseMan);
    ~CAknFepHashKeyManager();
    TKeyResponse HandleKeyEventL(TKeyPressLength aLength); 
    void SetMode(TInt aMode, TBool aWesternPredictive);

    /**
    * Changes the hash key functionality.
    */
    void SetHashKeyStyle(THashKeyStyle aHashKeyStyle);

    /**
    * Cancels the hash key timer.
    */
    void CancelHashKeyTimer();
    
    inline void ClearSelectionModeHashKeyLoop();
    
	void SetPreviousSelectionStyleModeL();        
	 /**
     * To Handle the Indic Hash key Loop State
     * 
     * Set the New State or Mode based on the editor after Fep is configured from Editor
     * @since 3.2
     * @return nothing
     */ 	 
	void ResetIndicHashKeyStateL();     

     /**
     * Reset the flag of previous selection
     *
     * @since 3.2
     * @return nothing
     */
    void ResetPreviousSelectionStyleMode();
#ifdef RD_HINDI_PHONETIC_INPUT
   	 /**
     * To Handle SetIndicStateL method
     * 
     * helps in handling case changes
     * @since 5.0
     * @return nothing
     */ 	   
    void HandleSetIndicStateL();
#endif

private:
    CAknFepHashKeyManager(CAknFepManager& aFepMan, CAknFepCaseManager* aCaseMan);
    void ConstructL();
    static TInt HashKeyTimerTimeoutCallback(TAny* aObj);
    void HashKeyTimerTimeout();
	TKeyResponse HandleKeyWesternSelectionStyleL(TKeyPressLength aLength);    
	void SetSelectionStyleModeL();
	 /**
     * To Handle Indic Hash key Loop
     * 
     * Set the New State or Mode based on the existing mode
     * @since 3.2
     * @return EKeyWasConsumed
     */  
	TKeyResponse SetIndicStateL();
	
	 /**
     * To Handle Indic Hash key Loop
     * 
     * Change back to the Previous State or Mode 
     * @since 3.2
     * @return EKeyWasConsumed
     */  
	TKeyResponse SetIndicPreviousStateL(); 
	
	 /**
     * To Handle Indic Hash key Loop
     * 
     * To verify if Indic hash key Loop is required.
     * @since 3.2
     * @return ETrue if Shared Interface input Language is Indic and given editor is not Latin-Only
     */ 
	TBool IsIndicHashKeyStyleRequired();
 

	/**
     * To Handle Indic Hash key Loop
     * 
     * Set the Style for the State 
     * @since 3.2
     * @return nothing
     */ 
	void SetIndicSelectionStateL();
	

	/**
     * To Handle Indic Hash key Loop
     * 
     * Update the case information
     * @since 3.2
     * @return nothing
     */ 
	void UpdateCaseForIndicHashLoop();

    /**
    * Reverts predictive text setting change. This method can be used
    * if short key press of hash key changed predictive text
    * setting in non-case supported languages and actually long key press
    * action was handled. Not used with Chinese languages.
    */
    void RevertPredictiveTextSettingChangeL();

    TKeyResponse HandleKeyWesternL(TKeyPressLength aLength);
    TKeyResponse HandleKeyWesternWithChineseL(TKeyPressLength aLength);
    TKeyResponse HandleKeyJapaneseL(TKeyPressLength aLength);
    TKeyResponse HandleLongKeypressJapaneseL();
    TKeyResponse HandleSortKeypressJapaneseL();
    TKeyResponse HandleKeyWesternWithKoreanL(TKeyPressLength aLength);

private:
    THashKeyStyle iHashKeyStyle;
    TInt iMode;
    TBool iLastModeNumber;

    CPeriodic* iHashKeyTimer; //owned
    
    CAknFepManager& iFepMan; //not owned
    CAknFepCaseManager* iCaseMan;
    TBool iWesternPredictive;
    
    TInt iSelectionLoopIndex;
    TInt iIndicSelectionLoopIndex;
    TUint8 const * iSelectionLoop;
    TBool iLanguageChangedOrChangeFromNumberMode;
    TBool iIndicPredictiveState;
    TCursorSelection iPreviousIndicCursorPosition;
    TBool iPreviousSelection;       // used by Japanese
    };

inline void CAknFepHashKeyManager::ClearSelectionModeHashKeyLoop()
	{
	iSelectionLoop = NULL;
	iSelectionLoopIndex	= 0;
	}

#endif //_AKNFEP_HASHKEY_MANAGER_H__
