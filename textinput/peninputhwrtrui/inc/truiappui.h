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
* Description:  UI class of TrainingUI
*
*/


#ifndef C_TRUIAPPUI_H
#define C_TRUIAPPUI_H

#include <aknViewAppUi.h>
#include <AknTabObserver.h>

// Forward reference
class CTruiMainView;
class CTruiShortcutsView;
class CTruiShortcutEditView;
class CAknNavigationDecorator;
class CTruiEngine;
class CTruiShortcutEditTextView;
class CTruiInfoMessageView;

/**
 *  Instance of CTruidemoAppUi is the User Interface part of the AVKON
 *  application framework for the TrainingUI application
 *
 *  @code
 *   CEikAppUi* appUi = new (ELeave) CTruiAppUi;
 *  @endcode
 *
 *  @lib avkon.lib eikcore.lib eiksrv.lib
 */
class CTruiAppUi : public CAknViewAppUi, public MAknTabObserver 
    {
public:
    /**
     * Perform the second phase construction of a CTruiAppUi object 
     */
    void ConstructL();

    /** 
     * Perform the first phase of two phase construction.
     */
    CTruiAppUi();

    /**  
     * Destroy the object and release all memory objects
     */
    ~CTruiAppUi();
    
    /**  
     * Get hand writing engine
     *
     * @return Pointer to CTruiEngine's instance
     */
    CTruiEngine* HwrEngine() const;
    
    /**  
     * Get hand Navigation Decorator
     *
     * @return The pointer to navigation pane.
     */
    CAknNavigationDecorator* GetNaviDecorator() const;

    /**  
     * Close the navi pane
     */
    void CloseNaviPaneL();

    /**  
     * Show the navi pane
     */
    void ShowNaviPaneL();
    
    /**  
     * Show the title pane
     *
     * @param aTitle: The text to be displayed in Tilte pane     
     */
    void ShowTitlePaneL( const TDesC& aTitle );
    
// from CAknViewAppUi

    /**
     * Handle user menu selections
     *
     * @param aCommand the enumerated code for the option selected
     */
    void HandleCommandL( TInt aCommand );
    
    // from MAknTabObserver    
    /**
     * Handle active tab changed
     *
     * @param aIndex the index of active tab.
     */
    void TabChangedL( TInt aIndex );      
                
private:

    /**
     * Create navigation pane.
     *
     * @param aResourceId The resource Id.
     * @return The pointer to navigation pane.
     *      
     */    
    CAknNavigationDecorator* CreateNavigationTabL( const TInt aResourceId,
                                                   const TInt aDefaultActive );            

private: // data
    /**
     * iMainView is the view that allows user to train HWR engine to
     * recognize user's own writing style
     * Own
     */
    CTruiMainView* iMainView;
    
    /**
     * iShortcutsView is the view that allows user to manage the shortcuts
     * Own
     */
    CTruiShortcutsView* iShortcutsView;
    
    /**
     * iShortcutEditView is the view that allows user to edit the shortcuts
     * Own
     */
    CTruiShortcutEditView* iShortcutEditView;
    
    /**
     * Pointer to shortcut edit text view 
     * on which users can alter shortcut's text.
     * Own
     */
    CTruiShortcutEditTextView* iShortcutEditTextView;
    
    /**
     * Pointer to info message view for shortcut wizard.     
     * Own
     */
    CTruiInfoMessageView* iInfoMessageView;
    
    /**
     * iNaviDecorator is navigation pane.
     * Allow users to switch views between main view and shortcut main view.
     * Own
     */    
    CAknNavigationDecorator* iNaviDecorator;
    
    CAknNavigationDecorator* iEmptyNaviDecorator;
    
    /**  
     * hand writing engine
     * Own
     */
    CTruiEngine* iEngine;
    };


#endif // C_TRUIAPPUI_H

