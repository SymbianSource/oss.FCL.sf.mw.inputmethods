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
* Description:  CTruiShortcutsView class of TrainingUI
*
*/



#ifndef C_TRUISHORTCUTSVIEW_H
#define C_TRUISHORTCUTSVIEW_H

#include <aknview.h>
#include <akntoolbarobserver.h>

class CTruiShortcutsContainer;
class CTruiTextSettingPageContainer;

// UID of view
const TUid KTruiShortcutsViewId = {2};

/**
 *  CTruiShortcutsView view class.
 *
 *  @code
 *    CTruiShortcutsView* view = CTruiShortcutsView::NewLC(); 
 *    AddViewL( view );
 *    CleanupStack::Pop();
 *  @endcode
 *
 */
class CTruiShortcutsView : public CAknView, public MAknToolbarObserver
    {
public:

    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiShortcutsView's instance
     */    
    static CTruiShortcutsView* NewL();

    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiShortcutsView's instance
     */
    static CTruiShortcutsView* NewLC();    

    /**
    * Destructor.
    */    
    virtual ~CTruiShortcutsView();

// from base class CAknView
    /**
    * From CAknView
    * Returns views id.
    *
    * @return id for this view.
    */
    TUid Id() const;    
    
    /**
     * From CAknView 
     * Command handling function. 
     *
     * @param aCommand ID of the command to respond to. 
     */    
    void HandleCommandL( TInt aCommand );

private:

    CTruiShortcutsView();
    
    void ConstructL();
    
    /**
    * From CAknView
    * Called by the framework when view is activated.
    *
    * @param aPrevViewId This is not used now.
    * @param aCustomMessage This is not used now.
    * @param aCustomMessage This is not used now.
    */
    void DoActivateL( const TVwsViewId& aPrevViewId,
                      TUid aCustomMessageId,
                      const TDesC8& aCustomMessage );   
    
    /**
    * From CAknView
    * Called by the framework when view is deactivated.
    *
    */
    void DoDeactivate();

// from MEikMenuObserver    
    /**
     * Dynamically initialises a menu pane.
     *
     * @param aResourceId The resource Id.
     * @param aMenuPane The in-memory representation of the menu pane.     
     */    
    void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );
    
    /**
     * Dynamically initialises a menu bar.
     *
     * @param aResourceId The resource Id.
     * @param aMenuPane The in-memory representation of the menu bar.     
     */ 
    void DynInitMenuBarL( TInt aResourceId, CEikMenuBar *aMenuBar );
    
// from MAknToolbarObserver
    /**     
     * Handles toolbar events for a certain toolbar item.
     *
     * @param aCommand The command ID of some toolbar item.
     */
    void OfferToolbarEventL( TInt aCommand );
    /** 
     * From CAknView. This function processes user commands.  
     * @param aCommand ID of the command to respond to. 
     */        
    void ProcessCommandL( TInt aCommand );    
    
private: // data

    /**
     * Pointer to the component control
     * own.  
     */
    CTruiShortcutsContainer* iContainer; 
    };
    
#endif // C_TRUISHORTCUTSVIEW_H

