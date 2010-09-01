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
* Description:  Control bar
*
*/


#ifndef C_CAKNFEPHWRBXCTRLBAR_H
#define C_CAKNFEPHWRBXCTRLBAR_H

// system includes
#include <peninputlayoutctrlgroup.h>
#include <peninputlayoutminterface.h>

// user includes
#include "peninputhwrpropertysubscriber.h"

// forward declarations
class CFepUiLayout;
class TResourceReader;
class CAknFepCtrlRangeBar;
//class CAknFepCtrlMultiModeButton;
//do not use multimode button anymore, use eventbutton instead
class CAknFepCtrlEventButton;
class CAknFepCtrlButton;

// class declaration

/**
 *  This is the class definition for control group of different
 *  input ranges
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxCtrlBar : public CControlGroup, 
                            public MAknFepHwrPropertySubscriber
    {
public:

    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of the Control Bar
     * @return The pointer point to CAknFepHwrBxCtrlBar type object
     */
    static CAknFepHwrBxCtrlBar* NewL(CFepUiLayout* aUiLayout , 
                                    TInt aControlId);
    
    /**
     * Two-phased constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of the Control Bar
     * @return The pointer point to CAknFepHwrBxCtrlBar type object
     */
    static CAknFepHwrBxCtrlBar* NewLC(CFepUiLayout* aUiLayout , 
                                     TInt aControlId);

    /**
     * Destructor.
     *
     * @since S60 v3.2
     */
    virtual ~CAknFepHwrBxCtrlBar();

    /** 
     * ConstructFromResourceL load from resource.
     *
     * @since S60 v3.2
     * @param aReader A resource reader
     * @return None
     */
    void ConstructFromResourceL(TResourceReader& aReader); 

    /**
     * Handles the SizeChanged event
     * @param aRect The new size
     * @param aGridRect 
     *
     * @since S60 v3.2
     * @return None
     */
    void SizeChangedL(const TRect& aRect, const TRect& aGridRect);

//from MAknFepHwrPropertySubscriber

    /**
     * From MAknFepHwrPropertySubscriber.
     * Set property
     *
     * @since S60 v3.2
     * @param aPropertyName The property name
     * @param aPropertyName The property name
     * @return None
     */
    void SetPropertyL(const TAknFepHwrProperty aPropertyName, const TDesC& aPropertyValue);
    
    TPoint StrokeEndMark();

protected:
        
    /**
     * C++ default constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout An instance of CFepUiLayout
     * @param aControlId The control id of the Control Bar
     */
    CAknFepHwrBxCtrlBar(CFepUiLayout* aUiLayout,TInt aControlId);

    /**
     * By default Symbian 2nd phase constructor is private.
     *
     * @since S60 v3.2
     * @param aUtils An instance of the resource environment
     * @return None
     */
    void ConstructL();
    
            
    /**
     * read from resource function
     *
     * @since Series 60 3.2
     * @return None
     */     
    void ConstructFromResourceL();

private:

    /**
     * Set active input range
     *
     * @since S60 v3.2
     * @param aRange A specified input range
     * @return ETrue if given input range set successfully, EFalse otherwise
     */
    TBool SetRange(const TInt aRange);
        
    /**
     * Set permitted input ranges
     *
     * @since S60 v3.2
     * @param aPermitted Used to decide which input ranges permitted
     * @return None
     */
    void SetPermittedRanges(const TInt aPermitted);

    /**
     * Set case of given range
     *
     * @since S60 v3.2
     * @param aCase One mode in given range
     * @return None
     */
    void SetCaseL(const TInt aCase);
    void SetPermittedCasesL(const TInt aCase);

private: // data
    /**
     * Instance of range bar (own)
     */
    CAknFepCtrlRangeBar* iRangeBar;             


    /**
     * Instance of virtual keyboard button(own)
     */
    CAknFepCtrlCommonButton* iVkb;             

    };

#endif // C_CAKNFEPHWRBXCTRLBAR_H
            
// End Of File
