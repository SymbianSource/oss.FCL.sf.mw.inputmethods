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
* Description:  CAknFepCtrlBaseWindowExt
*
*/


#ifndef C_CAKNFEPCTRLBASEWINDOWEXT_H
#define C_CAKNFEPCTRLBASEWINDOWEXT_H

//  INCLUDES

#include <peninputlayout.h>
#include <peninputlayoutctrlgroup.h>
#include "peninputcommonbutton.h"
#include "peninputeventbutton.h"
#include "peninputrepeatbutton.h"
#include "peninputinputcontextfield.h"


// FORWARD DECLARATIONS
class CFepUiLayout;
class CAknFepCtrlButton;
class CAknFepCtrlRepeatButton;
class CAknFepCtrlDragButton;
class CPeninputLayoutInputmodelChoice;
class CFepLayoutMultiLineIcf;
/**
 *  Class definiton for base window
 *
 *  @lib fepcommonctrls.lib
 *  @since S60 v3.2
 */
class CAknFepCtrlBaseWindowExt : public CControlGroup, public MEventObserver
    {    
public:
    
    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CAknFepCtrlBaseWindowExt();
    
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
    IMPORT_C CAknFepCtrlBaseWindowExt( CFepUiLayout* aFepUiLayout, 
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
    
    IMPORT_C void ConstructFromResourceL();

    IMPORT_C virtual void CreateBaseWindowControlsL( TInt16 aControlId, TInt32 aImageId ); 
    
    virtual TBool CreateCustomControlL( TInt16 aControlId, TInt32 aImageId );
    
    virtual void AddCustomControlGroupL(CFepUiBaseCtrl* aCtrl);
        
private:

    void ReadIcfInfoL(TResourceReader aResReader,
                     CFepInputContextField* icf);
    void ReadMultiLineIcfInforL( TResourceReader aResReader,
                                CFepLayoutMultiLineIcf* icf  );
    
    void ReadShadowInfoL( TInt aResId );

    inline CAknFepCtrlCommonButton* CloseButton() const;
    
    inline void SetCloseButton(CAknFepCtrlCommonButton* aCloseBtn);
    
    void ReadBackgroundInfoL( TInt aResId );
private:// data
  
    /**
     * The close button (Own)
     */
    CAknFepCtrlCommonButton* iCloseButton;

    TBool iFirstTimeConstruct;
        
    CCoeEnv* iCoeEnv;

    };

#include "peninputbasewindowext.inl"
    
#endif // C_CAKNFEPCTRLBASEWINDOWEXT_H

// End Of File
