/*
* Copyright (c) 2005-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of layout timer
*
*/


#ifndef C_LAYOUTTIMER_H
#define C_LAYOUTTIMER_H

#include <e32base.h>
#include <peninputlayoutminterface.h>

/**
 *  CLayoutTimer
 *
 *  Layout timer class. Several timers are used in hwr layout. Refer to TTimeType.
 *
 * @lib peninputlayoutcontrol.lib  
 * @since S60 v4.0
 */
NONSHARABLE_CLASS(CLayoutTimer) : public CActive
    {
public:     
    /** Layout timer type */
    enum TTimeType
        {
         ECharacterTimer,
         EStrokeTimer,
         ECandidateClearTimer,
         EDragbarMoveTimer,
         ELongPressTimer,
         EOthers
        };

    // Constructors and destructor
    /**
     * Two-phased constructor.
     *
     * @since S60 v4.0
     * @param aTimerHandler The callback timer function
     * @param aType Timer type. Default value is stroke timer
     * @param aAutoFalg Flag tells whether the timer will be active again
     *         ETrue, if it's automatically set active after RunL. Default is EFalse
     * @return The created timer
     */
    static CLayoutTimer* NewL(MTimerHandler* aTimerHandler , 
                TTimeType aMode = EStrokeTimer , TBool aAutoFlag = EFalse);
    /** 
     * Destructor. Frees all used resources. 
     *
     * @since S60 V4.0
     */
    ~CLayoutTimer();

    /**
     * sets the timer
     * 
     * @since S60 v4.0
     * @param aDelay Timer interval
     */
    void SetTimer(TTimeIntervalMicroSeconds32  aDelay);

private:
    /**
     * Constructor
     *
     * @since S60 v4.0
     * @param aTimerHandler The callback timer function
     * @param aType Timer type. Default value is stroke timer
     * @param aAutoFalg Flag tells whether the timer will be active again
     */
    CLayoutTimer(MTimerHandler* aTimerHandler,TTimeType aMode,TBool bAutoRepeat);

    /**
     * second phase constructor
     *
     * @since S60 v4.0
     */
    void ConstructL();

    //from base class CActive

    /**
     * From CActive
     * will be called when stroke timer ends
     *
     * @since S60 v4.0
     */
    void RunL();

    /**
     * From CActive
     * will be called if RunL leaves
     *
     * @since S60 v4.0
     */
    TInt RunError(TInt aError);

    /**
     * From CActive
     * will be called when stroke timer has been cancelled
     *
     * @since S60 v4.0
     */
    void DoCancel();

private:

    /**
     * timer object
     */
    RTimer iTimer;
    
    /**
     * Timer type
     */
    TTimeType iType;
    
    /**
     * flag for auto repat event
     */
    TBool iAutoRepeat;
    
    /**
     * timer interval
     */
    TTimeIntervalMicroSeconds32 iInterval;

    /**
     * Timer event handler
     * Not own
     */
    MTimerHandler* iTimerHandler;
    };
#endif //C_LAYOUTTIMER_H
