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
* Description:  CTruiShortcutEditView class of TrainingUI
*
*/



#ifndef C_TRUISHORTCUTEDITVIEW_H
#define C_TRUISHORTCUTEDITVIEW_H

#include <aknview.h>
#include <akntoolbarobserver.h>

class CTruiShortcutEditContainer;

// UID of view
const TUid KTruiShortcutEditViewId = {3};

/**
 *  CTruiShortcutEditView view class.
 *
 *  @code
 *    CTruiShortcutEditView* view = CTruiShortcutEditView::NewLC(); 
 *    AddViewL( view );
 *    CleanupStack::Pop();
 *  @endcode
 *
 */
class CTruiShortcutEditView : public CAknView, public MAknToolbarObserver
    {
public:

    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiShortcutEditView's instance
     */        
    static CTruiShortcutEditView* NewL();

    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiShortcutEditView's instance
     */
    static CTruiShortcutEditView* NewLC();    

    /**
    * Destructor.
    */    
    virtual ~CTruiShortcutEditView();
    
    inline const TVwsViewId& PreviousViewId() const
        {
        return iPreViewId;
        };

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
   
private:

   /**
    * Constructor.
    */     
    CTruiShortcutEditView();
    
   /**
    * Perform the second phase construction of a CTruiShortcutEditView object.
    */     
    void ConstructL();

// From CAknView
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
    
// MAknToolbarObserver
    /**
     * Handles toolbar events for a certain toolbar item.
     * @param aCommand The command ID of some toolbar item.
     */
    void OfferToolbarEventL( TInt aCommand );

private: // data

    /**
     * Pointer to the component control
     * own.  
     */
    CTruiShortcutEditContainer* iContainer;    
    
    /**
     * The previous view id     
     */
    TVwsViewId iPreViewId;
    };
    
#endif // C_TRUISHORTCUTEDITVIEW_H

