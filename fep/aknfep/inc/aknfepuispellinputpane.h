/*
* Copyright (c)  Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the CAknFepUiSpellInputPane definition.
*
*/












#ifndef AKNFEP_SPELLINPUTPANE_H
#define AKNFEP_SPELLINPUTPANE_H

#include <coecntrl.h> // for CCoeControl

// FORWARD DECLARATIONS
class CEikEdwin;
class MEikEdwinObserver;

/**
*  CDicInputPane  input pane class.
*/
class CAknFepUiSpellInputPane : public CCoeControl
{
public: // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aParent Parent control for container.
    * @param aRect Frame rectangle for container.
    * @return A Pointer to new input pane.
    */
    static CAknFepUiSpellInputPane* NewL( CCoeControl* aParent, 
                                 const TInt aEditorFlag, 
                                 const TInt aEditorCase, 
                                 const TInt aEditorSCTResID );

    /**
    * Destructor.
    */
    virtual ~CAknFepUiSpellInputPane();

public: // New functions

    /**
    * Return input win.
    * @return A pointer to the input editor object.
    */
    CEikEdwin* InputWin() const;

    /**
    * Set input win observer
    * @param aObserver input win observer
    */
    void SetInputWinObserver( MEikEdwinObserver* aObserver );

    /**
    * Set the content of input win
    * @param aDes the text to be set
    */
    void SetInputWinTextL( const TDesC* aDes );

    /**
    * Selects the entire document of input win
    */
    void SelectAllL();

	/**
	*  SizeChanged   resize the controls
	**/
	void SizeChanged();
	
public: // Functions from base classes   
 
    /**
    * From CoeControl, return the container's input capabilities.
    * @return The input capabilities of the container
    */
    TCoeInputCapabilities InputCapabilities() const;

    /**
    * From CoeControl, Handles key events.
    * @param aKeyEvent The key event.
    * @param aType The type of key event: EEventKey, 
    *  EEventKeyUp or EEventKeyDown.
    * @return The value to indicates whether or not 
    * the key event was used by this control
    */
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

private:

    /**
    * C++ default constructor.
    */
    CAknFepUiSpellInputPane();

    /**
    * EPOC default constructor.
    * @param aParent Parent control for container.
    * @param aRect Frame rectangle for container.
    */
    void ConstructL( CCoeControl* aParent, const TInt aEditorFlag, 
        const TInt aEditorCase, const TInt aEditorSCTResID );

private: // Functions from base classes

    /**
    * From CoeControl, CountComponentControls.
    */
    TInt CountComponentControls() const;

    /**
    * From CoeControl, ComponentControl.
    */
    CCoeControl* ComponentControl( TInt aIndex ) const;

    /**
    * From CoeControl, Draw.
    */
    void Draw( const TRect& aRect ) const; 

    /**
    * From CoeControl, FocusChanged.
    */
    void FocusChanged( TDrawNow aDrawNow );

private: // data

    CEikEdwin*   iInputWin;
};

#endif      // AKNFEP_SPELLINPUTPANE_H

// End of File
