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
* Description:            Provides the CAknFepUiSpellContainer definition.
*
*/












#ifndef AKNFEP_SPELLCONTAINER_H
#define AKNFEP_SPELLCONTAINER_H

// INCLUDES
#include <coecntrl.h>          // for CCoeControl
#include <eiklbo.h>            // for MEikListBoxObserver
#include <badesca.h>           // for CDesCArray


// FORWARD DECLARATIONS
class CAknFepUiSpellInputPane;

// CLASS DECLARATION

/**
*  CAknFepUiSpellContainer  container control class.
*/
class CAknFepUiSpellContainer : public CCoeControl
{
public: // Constructors and destructor

    /**
    * Two-phased constructor.
    * @param aRect Frame rectangle for container.
    * @param aModel Data model of the application.
    * @return A Pointer to new dictionary container.
    */
    static CAknFepUiSpellContainer* NewL(const TInt aEditorFlag, const TInt aEditorCase, 
                                         const TInt aEditorSCTResID);

    /**
    * Destructor.
    */
    virtual ~CAknFepUiSpellContainer();
    
    CAknFepUiSpellInputPane* InputPane();
    
    void SetInputWinObserver( MEikEdwinObserver* aObserver );

private:
    
    /**
    * C++ default constructor.
    */
    CAknFepUiSpellContainer();
    
    /**
    * EPOC default constructor.
    * @param aRect Frame rectangle for container.
    * @param aModel Data model of the application.
    */
    void ConstructL(const TInt aEditorFlag, const TInt aEditorCase, const TInt aEditorSCTResID); 

private: // Functions from base classes

    /**
    * From CoeControl, CountComponentControls.
    */
    TInt CountComponentControls() const;

    /**
    * From CCoeControl, ComponentControl.
    */
    CCoeControl* ComponentControl( TInt aIndex ) const;

    /**
    * From CCoeControl, Draw.
    */
    void Draw( const TRect& aRect ) const;

    /**
    * From CCoeControl, return the container's input capabilities.
    */
    TCoeInputCapabilities InputCapabilities() const;

    /**
    * From CCoeControl, handle key events.
    */
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );

   
    void SetContainerRect();
    
    HBufC* InputText();
     
     
protected:
    
    void SizeChanged();
    
    void FocusChanged(TDrawNow aDrawNow);
    
private: // data
    
    CAknFepUiSpellInputPane* iInputPane;

 };

#endif  // AKNFEP_SPELLCONTAINER_H

// End of File
