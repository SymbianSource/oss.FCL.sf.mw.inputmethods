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
* Description:  CTruiMainView class of TrainingUI
*
*/


#ifndef C_TRUIMAINVIEW_H
#define C_TRUIMAINVIEW_H

#include <aknview.h>
#include <akntoolbarobserver.h>

class CTruiMainViewContainer;

// UID of view
const TUid KTruiMainViewId = {1};

/**
 *  CTruiMainView view class.
 *
 *  @code
 *    CTruiMainView* view = CTruiMainView::NewLC(); 
 *    AddViewL( view );
 *    CleanupStack::Pop();
 *  @endcode
 *
 */
class CTruiMainView : public CAknView, public MAknToolbarObserver
    {
public:

    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiMainView's instance
     */    
    static CTruiMainView* NewL();

    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiMainView's instance
     */
    static CTruiMainView* NewLC();    

    /**
    * Destructor.
    */    
    virtual ~CTruiMainView();

// from base class CAknView

   /**
    * Returns views id.
    *
    * @return id for this view.
    */
    TUid Id() const;
    
   /**     
     * Command handling function. 
     *
     * @param aCommand ID of the command to respond to. 
     */    
    void HandleCommandL( TInt aCommand );

// from MEikMenuObserver    
    /**
     * Dynamically initialises a menu pane.
     *
     * @param aResourceId The resource Id.
     * @param aMenuPane The in-memory representation of the menu pane.     
     */    
    void DynInitMenuPaneL( TInt aResourceId, CEikMenuPane* aMenuPane );

private:

   /**
    * Constructor.
    */    
    CTruiMainView();
    
   /**
    * Perform the second phase construction of a CTruiMainView object.
    */    
    void ConstructL();   
                                
    /**
    * Dynamically initialises the character range menu pane.
    *
    * @param aScriptId Language script Id.    
    * @param aMenuPane The in-memory representation of the menu pane.
    */
    void InitCharRangeSubMenuL( TInt aScriptId, CEikMenuPane* aMenuPane);
                                
    /**
    * Get reource ID of menu items for initializing menu pane.
    *
    * @param aScriptId Language script Id.    
    * @return resource Id of menu items.
    */
    TInt MenuItemsReourceId( TInt aScriptId );
    
    /**
    * Check menu item dimmed by menu item id.
    *
    * @param aMenuPane Pointer to menu pane. 
    * @param aMenuItemId Menu item id of menu.    
    * @return ETrue: menu item is dimmed
    */
    TBool IsMenuItemDimmed( CEikMenuPane* aMenuPane, TInt aMenuItemId );
    
// from base class CAknView
   /**
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
    * Called by the framework when view is deactivated.
    *
    */
    void DoDeactivate();
    
    /**
     * From MAknToolbarObserver.
     * Handles toolbar events for a certain toolbar item.
     *
     * @param aCommand The command ID of some toolbar item.
     */
    void OfferToolbarEventL( TInt aCommand );    

private: // data

    /**
     * Pointer to the component control
     * own.  
     */
    CTruiMainViewContainer* iContainer;     
    
    /**
     * Indicates whether all delete is dimmed
     */
    TBool iDimAllDelete;
    };
    
#endif // C_TRUIMAINVIEW_H

