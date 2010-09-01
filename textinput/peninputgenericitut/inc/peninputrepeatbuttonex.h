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
* Description:  Interface of normal button and multidisplay button
*
*/


#ifndef C_CAKNFEPCTRLREPEATBUTTONEX_H
#define C_CAKNFEPCTRLREPEATBUTTONEX_H

// system includes
#include <AknsConstants.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputlayoutbutton.h>
#include <peninputpluginutils.h>
#include <peninputcommonbutton.h>

/**
 *  CAknFepCtrlRepeatButtonEx
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlRepeatButtonEx: public CAknFepCtrlCommonButton, public MAknFepTimerHandler
{
public:
    
    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout The layout
     * @param aControlId The control id
     * @param aEvent An event to be repeatedly sent
     * @param aUnicode A Unicode value to be repeatedly sent
     * @return Pointer to created CAknFepCtrlRepeatButtonEx object
     */
    static CAknFepCtrlRepeatButtonEx* NewL(CFepUiLayout* aUiLayout, 
                                                  TInt aControlId,
                                                  TAknsItemID aNormalID,
    											  TAknsItemID aPressedID,
    											  TAknsItemID aInactiveID, 
                                                  TInt aEvent = 0xFFFF,
                                                  TInt aUnicode = 0);

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout The layout
     * @param aControlId The control id
     * @param aEvent An event to be repeatedly sent
     * @param aUnicode A Unicode value to be repeatedly sent
     * @return Pointer to created CAknFepCtrlRepeatButtonEx object
     */
    static CAknFepCtrlRepeatButtonEx* NewLC(CFepUiLayout* aUiLayout, 
                                                   TInt aControlId, 
                                                   TAknsItemID aNormalID,
    											   TAknsItemID aPressedID,
    											   TAknsItemID aInactiveID,
                                                   TInt aEvent = 0xFFFF,
                                                   TInt aUnicode = 0);
    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepCtrlRepeatButtonEx();

    /**
     * Set the repeat interval.
     *
     * @since S60 v3.2
     * @param aTime Interval in ms.
     * @return None
     */
    void SetRepeatInterval(const TTimeIntervalMicroSeconds32& aTime);

    /**
     * Cancel key sending
     *
     * @since S60 v3.2
     * @return None
     */
    void CancelRepeat();
    
    /**
     * Unicode
     *
     * @since S60 v3.2
     * @return None
     */    
   	TInt Unicode();
    
    /**
     * Event 
     *
     * @since S60 v3.2
     * @return None
     */    
	TInt Event();
	
// from base class MAknFepHwrTimerHandler
    
    /**
     * from MAknFepHwrTimerHandler
     * HandleTimerOut. Handling time out event
     *
     * @since S60 v3.2
     * @param aTimer The timer which sending the request
     * @return None
     */                
    void HandleTimerOut(const CAknFepTimer* aTimer);

protected:
    
    /**
     * C++ constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout Ui Layout who contains this control
     * @param aCtrlId Control Id
     * @param aEvent An event to be repeatedly sent
     * @param aUnicode A Unicode value to be repeatedly sent     
     * @return None 
     */
    CAknFepCtrlRepeatButtonEx(CFepUiLayout* aUiLayout, 
                                     TInt aCtrlId,
                                     TAknsItemID aNormalID,
    								 TAknsItemID aPressedID,
    								 TAknsItemID aInactiveID,
                                     TInt aEvent,
                                     TInt aUnicode);

// from base class CButtonBase

    /**
     * From CButtonBase
     * Handle button down event. start long press timer        
     *
     * @since S60 v3.2
     * @param aPt Point position
     * @return The control who handles the event
     */                
    CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPt);
    
    /**
     * From CButtonBase
     * Handle button up event. cancel all timers
     *
     * @since S60 v3.2
     * @param aPt Point position
     * @return The control who handles the event
     */                
    CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPt);
    
    /**
     * From CButtonBase
     * Handle pointer levae event. 
     *
     * @since S60 v3.2
     * @param aPoint current pointer position
     * @return Nothing
     */
    void HandlePointerLeave(const TPoint& aPoint);

    /**
     * Symbian 2nd phase constructor.
     */
    void ConstructL();  
    
private:

    /**
     * interval of long press
     */
    TTimeIntervalMicroSeconds32 iLongPressInterval;

    /**
     * interval of repeat 
     */
    TTimeIntervalMicroSeconds32 iRepeatInterval;

    /**
     * long press timer (Own)
     */
    CAknFepTimer* iLongPressTimer;

    /**
     * repeat timer (Own)
     */
    CAknFepTimer* iRepeatTimer;
    
    /**
     * Event Id
     */
    TInt iEvent;
    
    /**
     * Unicode value
     */    
    TInt iUnicode;
    
};

#endif // C_CAKNFEPCTRLREPEATBUTTONEX_H

// End Of File
