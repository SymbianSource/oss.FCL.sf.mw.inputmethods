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
* Description:  CAknFepCtrlBaseWindow
*
*/


#ifndef C_CAKNFEPCTRLBASEWINDOW_H
#define C_CAKNFEPCTRLBASEWINDOW_H

//  INCLUDES
#include <peninputlayoutctrlgroup.h>

// FORWARD DECLARATIONS
class CFepUiLayout;
class CAknFepCtrlButton;
class CAknFepCtrlDragButton;
class CAknFepCtrlCommonButton;
/**
 *  Class definiton for base window
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlBaseWindow : public CControlGroup, public MEventObserver
    {    
public:
    
    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CAknFepCtrlBaseWindow();
    
    /**
     * Resize 
     *
     * @since S60 v3.2
     * @param aLandscapeStyle Current screen is landscape or not
     * @return None
     */          
    virtual void SizeChanged(TBool aLandscapeStyle) = 0;
    
    /**
     * Close window
     *
     * @since S60 v3.2
     * @return None
     */          
    IMPORT_C void Close();
    
    /**
     * Enable or disable drag window
     *
     * @since S60 v3.2
     * @param aEnable ETrue to enable otherwise disable drag
     * @return None
     */          
    IMPORT_C void SetDragEnable(TBool aEnable);

    /**
     * Get the control by control id
     *
     * @since S60 v3.2
     * @param aCtrlId The control id
     * @return Pointer to the control found;otherwise NULL
     */          
    IMPORT_C CFepUiBaseCtrl* Control(TInt aCtrlId);
    
    /**
     * Get Header Pane
     *
     * @since S60 v3.2
     * @return Pointer to header pane
     */          
    inline CControlGroup* HeaderPane() const;
    
    /**
     * Get Client Pane
     *
     * @since S60 v3.2
     * @return client pane
     */          
    inline CControlGroup* ClientPane() const;
    
    /**
     * Get the title bar
     *
     * @since S60 v3.2
     * @return Pointer to title bar
     */          
    inline CControlGroup* TitleBar() const;
    
    /**
     * Get Client Pane
     *
     * @since S60 v3.2
     * @return Pointer to move button
     */          
    inline CAknFepCtrlDragButton* MoveButton() const;

    /**
     * Get close button
     *
     * @since S60 v3.2
     * @return Pointer to close button
     */          
    inline CAknFepCtrlCommonButton* CloseButton() const;

// from base class MEventObserver

    /**
     * From MEventObserver.
     * Handle the observer event
     *
     * @since S60 v3.2
     * @param aEventType Event type
     * @param aCtrl Sender control
     * @param aEventData Event data
     */
    IMPORT_C void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                     const TDesC& aEventData);
    
// from base class CControlGroup

    /**
     * From CControlGroup.
     * Draw window
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void Draw();
    
protected:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @param aFepUiLayout A Ui Layout environment (CFepUiLayout)
     * @param aControlId Control id
     */          
    IMPORT_C CAknFepCtrlBaseWindow( CFepUiLayout* aFepUiLayout, 
                         TInt aControlId); 
    
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @return None
     */          
    IMPORT_C void BaseConstructL();

    /**
     * call back function when closing window
     *
     * @since S60 v3.2
     * @return ETrue for allowing close window; otherwise EFalse
     */          
    IMPORT_C virtual TBool DoClose();

private:// data
  
    /**
     * The title bar (Own)
     */
    CControlGroup* iTitleBar;
    
    /**
     * The move drag button (Own)
     */
    CAknFepCtrlDragButton* iMoveButton;

    /**
     * The close button (Own)
     */
    CAknFepCtrlCommonButton* iCloseButton;
    
    /**
     * The header pane (Own)
     */
    CControlGroup* iHeaderPane;

    /**
     * The client pane (Own)
     */
    CControlGroup* iClientPane;
    
    };

#include "peninputbasewindow.inl"
    
#endif // C_CAKNFEPCTRLBASEWINDOW_H

// End Of File
