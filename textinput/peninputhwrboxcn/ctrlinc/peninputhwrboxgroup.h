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
* Description:  The class of handwriting box container.
*
*/


#ifndef C_AKNFEPHWRBOXGROUP_H
#define C_AKNFEPHWRBOXGROUP_H

#include <e32base.h>
#include <peninputlayoutbasecontrol.h>
#include <peninputlayoutctrlgroup.h>
#include <peninputlayouthwrwnd.h>
#include <peninputlayoutminterface.h>
#include "peninputhwrpropertysubscriber.h"

/* FORWARD DECLARATIONS */
class TResourceReader;

/**
 *  HWR(Hand Writing Recognition) box container class.
 *
 *  This class holds one or more handwriting boxes, processes event
 *  sent from hwr boxes and managers boxes UI.
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2 
 */
class CAknFepHwrBoxGroup: public CControlGroup, 
                          public MEventObserver,
                          public MAknFepHwrPropertySubscriber
    {
public:


    /**
     * Two-phased constructor. 
     * CAknFepHwrBoxGroup creates aBoxCount HWR boxes.
     * After creating a new CAknFepHwrBoxGroup and SetTransparency
     * must be called.

     * @param aUiLayout Ui layout instance from CFepUiLayout
     * @param aBoxGroupId The box group control id
     * @param aBoxCount The count of contained HWR box
     * @return The instance of CAknFepHwrBoxGroup
     */
    static CAknFepHwrBoxGroup* NewL(CFepUiLayout* aUiLayout,
                                    TInt aBoxGroupId,
                                    TInt aBoxCount);
                                    
    /**
     * Two-phased constructor. 
     * CAknFepHwrBoxGroup creates aBoxCount HWR boxes.
     * After creating a new CAknFepHwrBoxGroup and SetTransparency
     * must be called.

     * @param aUiLayout Ui layout instance from CFepUiLayout
     * @param aBoxGroupId The box group control id
     * @param aBoxCount The count of contained HWR box
     * @return The instance of CAknFepHwrBoxGroup
     */
    static CAknFepHwrBoxGroup* NewLC(CFepUiLayout* aUiLayout,
                                     TInt aBoxGroupId,
                                     TInt aBoxCount);
    /**
     * Destructor.
     */
    virtual ~CAknFepHwrBoxGroup();

    /** 
     * ConstructFromResourceL load HWR box settings from resource.
     * Reserved interface currently does nothing.
     *
     * @since S60 v3.2
     * @param aReader A resource reader
     * @return None
     */
    void ConstructFromResourceL(TResourceReader& aReader); 
    
    /** 
     * ConstructFromResourceL load HWR box settings from resource.
     * Before calling this function, the resource id should be set
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructFromResourceL();

    /**
     * Return the HWR box count in the container
     *
     * @since S60 v3.2
     * @return Number of HWR box in the container
     */
    TInt BoxCount();

    /**
     * Clear current handwriting trail if any. There is only one 
     * box is writing at a time. CancelWriting only clear current
     * writing box's trail.
     *
     * @since S60 v3.2
     * @return None 
     */
    void CancelWriting();
    
    /**
     * Return current active box pen stroke list array.
     *
     * @since S60 v3.2
     * @return Reference of stroke list array 
     */
    const RArray<TPoint>& StrokeList();
    
    /**
     * Hand size change event.
     * this function is called by parent control which knows the
     * box container position.
     *
     * @since S60 v3.2
     * @param aRect The new position rectangle
     * @return None
     */
    void SizeChanged(const TRect& aRect);

    /**
     * Hand size change event.
     * this function loads the new position from LAF.
     *
     * @since S60 v3.2
     * @return None
     */
    void SizeChanged();
    
    /**
     * Iterate to set each HWR box backgound color.
     *
     * @since S60 v3.2
     * @param aCol New pen color
     * @return None
     */
    void SetBoxBackColor(const TRgb aCol);
    
    /**
     * Iterate to set each HWR box frame color.
     *
     * @since S60 v3.2
     * @param aCol New frame color
     * @return None
     */
    void SetBoxFrameColor(const TRgb aCol);
    
    /**
     * Iterate to set each HWR box stroke end mark.
     *
     * @since S60 v3.2
     * @param aEndMark The stroke end mark
     * @return None
     */
    void SetStrokeEndMark(const TPoint& aEndMark);

    /**
     * Set transparent level of HWR box.
     * 
     * @since S60 v3.2
     * @param aActiveTrans, The active box(current writing) tranparency
     * @param aInactiveTrans, The inactive box transparency
     * @return None
     */
     void SetTransparency(const TUint8 aActiveTrans, const TUint8 aInactiveTrans);
    
    /**
     * Get transparent level of HWR box for active box transparency.
     * 
     * @since S60 v3.2
     * @return The active box transparency.
     */
    inline TUint8 ActiveTransparency() const;
    
    /**
     * Get transparent level of HWR box for inactive box transparency.
     * 
     * @since S60 v3.2
     * @return The inactive box transparency.
     */
    inline TUint8 InactiveTransparency() const;
    
    /**
     * Set property,  From base class MAknFepHwrPropertySubscriber
     *
     * @since S60 v3.2
     * @param aPropertyName The properyt name
     * @param aPropertyValue The properyt value
     * @return None
     */
    void SetPropertyL(
            const MAknFepHwrPropertySubscriber::TAknFepHwrProperty aPropertyName, 
            const TDesC& aPropertyValue);
            
    TPoint StrokeEndMark();
    
protected: 

    /**
     * From base class MEventObserver
     *
     * Handle event 
     * @since S60 v3.2
     * @param aEventType The event type
     * @param aCtrlId The control who sends the event
     * @param aEventData The event data
     * @return None
     */
    virtual void HandleControlEvent(TInt aEventType, 
                             CFepUiBaseCtrl* aCtrl, 
                             const TDesC& aEventData);
private:

    /**
     * C++ default constructor.
     *
     * @param aUiLayout Ui layout instance from CFepUiLayout
     * @param aBoxGroupId The box group control id
     */
    CAknFepHwrBoxGroup(CFepUiLayout* aUiLayout, TInt aBoxGroupId);
    
    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @param aCandidateWnd Candidate window observer
     * @param aBoxCount the count of contained HWR box
     * @return None
    */
    void ConstructL(TInt aBoxCount);

    /**
     * Iterate to set each HWR box writting speed.
     *
     * @since S60 v3.2
     * @param aCharDelay The submit character timer in micro seconds
     * @return None
     */
    void SetWritingSpeed(const TTimeIntervalMicroSeconds32& aCharDelay);

    /**
     * Iterate to set each HWR box trail drawing pen size.
     *
     * @since S60 v3.2
     * @param aSize New pen size
     * @return None
     */
    void SetBoxPenSize(const TSize aSize);

    /**
     * Iterate to set each HWR box trail drawing pen color.
     *
     * @since S60 v3.2
     * @param aCol New pen color
     * @return None
     */
    void SetBoxPenColor(const TRgb aCol);
    
    /**
     * Handle EEventHwrStrokeStarted event. The message is reported each time 
     * pen down in the HWR box area.
     * @since S60 v3.2
     * @param aBoxFrom The box which generates the message
     * @return None
     */
    void OnStrokeStarted(CTransparentHwrWnd* aBoxFrom);
                         
    /**
     * Handle EEventHwrStrokeFinished event. The message is reported when
     * StrokeTimer times out.
     * @since S60 v3.2
     * @param aBoxFrom The box which generates the message
     * @return None
     */
    void OnStrokeFinished(CTransparentHwrWnd* aBoxFrom);

    /**
     * Handle EEventHwrCharacterTimerOut event. The message is reported when
     * CharacterTimer times out.
     * @since S60 v3.2
     * @param aBoxFrom The box which generates the message
     * @return None
     */
    void OnStrokeCharacterTimerOut(CTransparentHwrWnd* aBoxFrom);

    /**
     * Handle EEventHwrStrokeCanceled event. The message is reported when
     * manually cancel writing.
     * @since S60 v3.2
     * @param aBoxFrom The box which generates the message
     * @return None
     */
    void OnStrokeCanceled(CTransparentHwrWnd* aBoxFrom);

    /**
     * Explicit refresh HWR boxes UI.
     *
     * @return None
     */
    void RefreshUI(CTransparentHwrWnd* aBox = NULL);

    /**
     * Submit first candidate in candidate buffer immediatelly 
     * and clear handwriting trail.
     *
     * @since S60 v3.2
     * @return None
     */
    void SubmitWriting();

private: // data

    /**
     * The background transparent alpha value(percentage).
     * The value is between 0-255, 0 mean totally invisible, 100 means opaque
     * iActiveMask Current writing box background transparency
     * iInactiveMask None writing box background transparency
     */
    TUint8 iActiveTrans;

    TUint8 iInactiveTrans;

    /**
     * Current wrting box pointer. Not own.
     * NULL means no box in writing state.
     */
    CTransparentHwrWnd* iActiveBox;
    
    /**
     * First time construct flag
     */
    TBool iFirstTimeConstruct;
    };

#include "peninputhwrboxgroup.inl"

#endif // C_AKNFEP_HWRBOXGROUP_H

//End Of File
