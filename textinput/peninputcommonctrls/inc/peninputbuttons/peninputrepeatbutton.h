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


#ifndef C_CAKNFEPCTRLREPEATBUTTON_H
#define C_CAKNFEPCTRLREPEATBUTTON_H

// system includes
#include <peninputlayoutbasecontrol.h>
#include <peninputlayoutbutton.h>
#include <peninputpluginutils.h>
#include <peninputeventbutton.h>

// forward declarations
class TResourceReader;

/**
 *  CAknFepCtrlRepeatButton
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlRepeatButton: public CAknFepCtrlEventButton, public MAknFepTimerHandler
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
     * @return Pointer to created CAknFepCtrlRepeatButton object
     */
    IMPORT_C static CAknFepCtrlRepeatButton* NewL(CFepUiLayout* aUiLayout, 
                                                  TInt aControlId, 
                                                  TInt aEvent = 0xFFFF,
                                                  TInt aUnicode = 0,
											      TAknsItemID aNormalID = KAknsIIDQsnFrFunctionButtonNormal,
			    								  TAknsItemID aPressedID = KAknsIIDQsnFrFunctionButtonPressed,
			    								  TAknsItemID aInactiveID = KAknsIIDQsnFrFunctionButtonInactive);

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout The layout
     * @param aControlId The control id
     * @param aEvent An event to be repeatedly sent
     * @param aUnicode A Unicode value to be repeatedly sent
     * @return Pointer to created CAknFepCtrlRepeatButton object
     */
    IMPORT_C static CAknFepCtrlRepeatButton* NewLC(CFepUiLayout* aUiLayout, 
                                                   TInt aControlId, 
                                                   TInt aEvent = 0xFFFF,
                                                   TInt aUnicode = 0,
											      TAknsItemID aNormalID = KAknsIIDQsnFrFunctionButtonNormal,
			    								  TAknsItemID aPressedID = KAknsIIDQsnFrFunctionButtonPressed,
			    								  TAknsItemID aInactiveID = KAknsIIDQsnFrFunctionButtonInactive);
    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CAknFepCtrlRepeatButton();

    /**
     * Set the repeat interval.
     *
     * @since S60 v3.2
     * @param aTime Interval in ms.
     * @return None
     */
    IMPORT_C void SetRepeatInterval(const TTimeIntervalMicroSeconds32& aTime);

    /**
     * Cancel key sending
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void CancelRepeat();

// from base class MAknFepHwrTimerHandler
    
    /**
     * from MAknFepHwrTimerHandler
     * HandleTimerOut. Handling time out event
     *
     * @since S60 v3.2
     * @param aTimer The timer which sending the request
     * @return None
     */                
    IMPORT_C void HandleTimerOut(const CAknFepTimer* aTimer);

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
    IMPORT_C CAknFepCtrlRepeatButton(CFepUiLayout* aUiLayout, 
                                     TInt aCtrlId,
                                     TInt aEvent,
                                     TInt aUnicode,
											      TAknsItemID aNormalID = KAknsIIDQsnFrFunctionButtonNormal,
			    								  TAknsItemID aPressedID = KAknsIIDQsnFrFunctionButtonPressed,
			    								  TAknsItemID aInactiveID = KAknsIIDQsnFrFunctionButtonInactive);

// from base class CButtonBase

    /**
     * From CButtonBase
     * Handle button down event. start long press timer        
     *
     * @since S60 v3.2
     * @param aPt Point position
     * @return The control who handles the event
     */                
    IMPORT_C CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPt);
    
    /**
     * From CButtonBase
     * Handle button up event. cancel all timers
     *
     * @since S60 v3.2
     * @param aPt Point position
     * @return The control who handles the event
     */                
    IMPORT_C CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPt);
    
    /**
     * From CButtonBase
     * Handle pointer levae event. 
     *
     * @since S60 v3.2
     * @param aPoint current pointer position
     * @return Nothing
     */
    IMPORT_C void HandlePointerLeave(const TPoint& aPoint);

    IMPORT_C void CancelPointerDownL();
    
    /**
     * Symbian 2nd phase constructor.
     */
    IMPORT_C void ConstructL();  
    
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
    
};

#endif // C_CAKNFEPCTRLREPEATBUTTON_H

// End Of File
