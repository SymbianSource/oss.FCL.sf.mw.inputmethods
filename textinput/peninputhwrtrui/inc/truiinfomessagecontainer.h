/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  CTruiInfoMessageContainer class of TrainingUI
*
*/


#ifndef C_TRUIINFOMESSAGECONTAINER_H
#define C_TRUIINFOMESSAGECONTAINER_H

#include <AknLayout2Def.h>
#include "truicontainerbase.h"

class CAknsBasicBackgroundControlContext;
class CEikButtonGroupContainer;
class CEikLabel;
class CTruiRadioButton;
class CEikEdwin;

/**
 *  CTruiInfoMessageContainer container control
 *  Used as the component to shortcut main view
 *
 *  @code
 *      iContainer = CTruiInfoMessageContainer::NewL( ClientRect() );
 *      iContainer->SetMopParent( this );
 *      AppUi()->AddToStackL( *this, iContainer );
 *  @endcode
 *
 */
 
class CTruiInfoMessageContainer : public CTruiContainerBase
    {
public:    
    /**
     * Two-phased constructor.
     * @param aRect The rectangle that defines the control's extent. 
     *              The rectangle's origin is relative to the origin
     *              of its associated window.
     * @return Pointer to the component control
     */
    static CTruiInfoMessageContainer* NewL( const TRect& aRect );
    
    /**
     * Two-phased constructor.
     * @param aRect The rectangle that defines the control's extent. 
     *              The rectangle's origin is relative to the origin
     *              of its associated window.
     * @return Pointer to the component control
     */
    static CTruiInfoMessageContainer* NewLC( const TRect& aRect );
    
    /**
     * Destructor.
     */
    virtual ~CTruiInfoMessageContainer();
    
    /**
     * Do something before exit
     */
    void PrepareToExitL();
    
private:

    /**
     * Constructor.
     */
    CTruiInfoMessageContainer();

    /**
     * Two-phased constructor. Second phase
     * @param aRect The rectangle that defines the control's extent. 
     *              The rectangle's origin is relative to the origin
     *              of its associated window.
     */
    void ConstructL( const TRect& aRect );
    
    /**
     * Create controls to be displayed in this container.
     */
    void InitializeControlsL();

    /**
     * Create label and display it in the container.
     *    
     * @param aParent The parent of the control.
     * @return The pointer to the label.
     */
    CEikLabel* CreateLabelL( const CCoeControl* aParent );
    


// from base class CCoeControl    
     /**
     * Draw the screen.
     * @param aRect the rectangle of this view that needs updating
     */
    void Draw( const TRect& aRect ) const;

    /**
     * From CoeControl.
     * Responds to changes to the size and position of the contents of this control.
     */
    void SizeChanged();
    
    /**
     * From CoeControl.
     * Handles a change to the control's resources.
     *
     * @param aType A message UID value.
     */
    void HandleResourceChange( TInt aType );

private: // data

    /**
     * Pointer to background
     * Own
     */
    CAknsBasicBackgroundControlContext*	iBackGround;

    /**
     * Pointer to the label text
     * Own
     */
    HBufC* iLabelText;
    
    /**
     * Pointer to the radio button
     * Own
     */
    CTruiRadioButton* iRadioButtonContainer;
    
    /**
     * Pointer to the edwin text
     * Own
     */
    CEikEdwin* iEdwin;    
    
    /**
     * MultilineLayout for edwin
     * 
     */
    TAknMultiLineTextLayout iMultilineLayout;
    
    /**
     * TRect of instruct pane for edwin
     * 
     */
    TRect iInstructPaneRect;
    };

#endif // C_TRUIINFOMESSAGECONTAINER_H

