/*
* Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides CAknFepManager definition
*
*/












#ifndef _AKNFEP_FNKEY_MANAGER_H__
#define _AKNFEP_FNKEY_MANAGER_H__


#include <e32base.h>
#include <w32std.h>
#include <coeinput.h>
#include <PtiDefs.h>                   //TCoeInputCapabilities

#include <AknIndicatorContainer.h> 

#include "AknFepGlobalEnums.h"
#include "AknFepSharedDataInterface.h"


class CAknFepManager;
class CAknIndicatorContainer;
   
/**
 *  FnKey State machine
 *
 *  @since S60 v3.2
 */
class CAknFepFnKeyManager : public CBase
    {

public:

  /**  Fn key states */
    enum TFnKeyState
        {
        EFnKeyNone,
        EFnKeyPressed,
        EFnKeyDown,
        EFnKeyNext,
        EFnKeyPressedAgain,
        EFnKeyLock,
        EFnKeyForced,
        EFnKeyReversePressed,
        EFnKeyReverse
        };

public:
    static CAknFepFnKeyManager* NewL( CAknFepManager& aFepMan,CAknFepSharedDataInterface* 
                                        aSharedDataInterface );
    ~CAknFepFnKeyManager();

public:
    /*
    * Resets the Function Key State
    */
    void ClearFnKeyState();

    void SetQwertyInputMode(TBool aQwertyState);
    
    /* 
    * Handles the Fn key event to change the state appropriately.
    */
    TKeyResponse HandleFnKeyEventL( const TKeyEvent& aKeyEvent, 
                           TEventCode aEventCode,
                           TCoeInputCapabilities aInputCapabilities
                           );

    void SetCase( TCase aCase );
    
    TFnKeyState FnKeyState();
    void SetFnKeyState(CAknFepFnKeyManager::TFnKeyState aState);    
private:

    CAknFepFnKeyManager( CAknFepManager& aFepMan,
                        CAknFepSharedDataInterface* aSharedDataInterface);
    void ConstructL();
    
    // For Shift Key handling.
    void UpdatePreviousCase();
    

private: // data
    CAknFepManager &iFepMan; //not own
    TCoeInputCapabilities iInputCapabilities;
    TFnKeyState iFnKeyState;
    TBool iIsQwertyMode;
    
    /**
     * Pointer to the Indicator Container object containing editor indicators.
     * Not own.
     */
    CAknIndicatorContainer*     iIndicatorContainer;
    CAknFepSharedDataInterface* iSharedDataInterface;
    
    // For Shift key handling.
    TInt iPreviousCase;
    TBool iShiftKeyPressed;
    
    // For QWERTY Predictive
    TBool iWasPreviousModePredictive;
    TBool iPreviousKeyWasFnKey;
    };

#endif // ? _AKNFEP_FNKEY_MANAGER_H__
