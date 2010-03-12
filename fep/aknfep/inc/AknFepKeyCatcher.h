/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Provides the CAknFepKeyCatcher definition.
*
*/












#ifndef __AKN_INPUT_FEP_KEYCATCHER_H__
#define __AKN_INPUT_FEP_KEYCATCHER_H__

#include <coecntrl.h>       //CCoeControl
#include <coemain.h>
#include "AknFepGlobalEnums.h"

class CAknFepManager;

/**
 * Interface class to allow ability to switch into a test mode, where a different
 * key catcher is used, i.e. to print values of keys received on the screen.
 */
class CAknFepKeyCatcherInterface : public CCoeControl
    {
public:
    virtual void SetState(const TAknFepManagerState aState) = 0;
    virtual ~CAknFepKeyCatcherInterface();
    virtual void HandleChangeInFocus();
    };

/**
 * The Key Catcher sits sits on top of the control stack and does some basic, primary processing 
 * of all key presses. `
 */
class CAknFepKeyCatcher : 
						 public CAknFepKeyCatcherInterface
#ifdef RD_SCALABLE_UI_V2
						 ,public MCoeMessageMonitorObserver
#endif //RD_SCALABLE_UI_V2
                          
    {

private:
    /** 
     * values for the flags required to maintain the internal
     * state of the Key Catcher during operation
     */
    enum
        {
        EFlagLongKeyPressHandled            =0x00000001,
        EFlagBlockAllLongKeyPressEvents     =0x00000002 
        };

public:
    /**
     * First phase construction
     *
     * @param aFepMan the Fep Manager object which owns this key catcher object. 
     */
    static CAknFepKeyCatcher* NewL(CAknFepManager& aFepMan);

    /**
     * Destructor
     */
    virtual ~CAknFepKeyCatcher();
    

    /**
     * from CCoeControl  
     * Key Catcher. Ignores keys when Fep is in null mode (no editor focused)
     * and does first stage filtering
     */
    virtual TKeyResponse OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode);

    /**
     * interface for FepMan to set the Key Catcher's internal state object
     */
    virtual void SetState(const TAknFepManagerState aState);

private:    
    /**
     * C++ Constructor
     */
    CAknFepKeyCatcher(CAknFepManager& aFepMan);

    /**
     * Second phase construction
     */
    void ConstructL();

    TKeyResponse HandleHashKeyJapaneseL(const TKeyEvent& aKeyEvent, TEventCode aEventCode);
    
    /**
     * from CCoeControl  
     * Handles a change to the control's resources.
     * @param aType A message UID value
     */
    void HandleResourceChange(TInt aType);
#ifdef RD_SCALABLE_UI_V2
	void MonitorWsMessage(const TWsEvent& aEvent);
#endif
    inline void SetFlag(TInt aFlag);
    inline void ClearFlag(TInt aFlag);
    inline TBool IsFlagSet(TInt aFlag) const;
	TBool IsNaviKey(TUint aCode);
private:    
    // Data not owned
    CAknFepManager& iFepMan;

    TAknFepManagerState iKeyCatcherState;
    TUint iFlags;
    TKeyPressLength     iKeyPressLength;
    TInt iLongPressedScanCode;
    };

/**
 *  Sets aFlag bitfield in iFlags
 */
inline void CAknFepKeyCatcher::SetFlag(TInt aFlag)
    {
    iFlags|=aFlag;
    }

/**
 * Clears aFlag bitfield in iFlags
 */
inline void CAknFepKeyCatcher::ClearFlag(TInt aFlag)
    {
    iFlags&=~aFlag;
    }

/**
 * Returns ETrue if the aFlag bitfield in iFlags is set, EFalse if it
 * is clear
 */
inline TBool CAknFepKeyCatcher::IsFlagSet(TInt aFlag) const
    {
    return iFlags&aFlag;
    }

#endif

// End of file
