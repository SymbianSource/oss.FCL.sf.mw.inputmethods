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
* Description:  CTruiInfoMessageView class of TrainingUI
*
*/


#ifndef C_TRUIINFOMESSAGEVIEW_H
#define C_TRUIINFOMESSAGEVIEW_H

#include <aknview.h>

class CTruiInfoMessageContainer;

// UID of view
const TUid KTruiInfoMessageViewId = {5};

/**
 *  CTruiInfoMessageView view class.
 *
 *  @code
 *    CTruiInfoMessageView* view = CTruiInfoMessageView::NewLC(); 
 *    AddViewL( view );
 *    CleanupStack::Pop();
 *  @endcode
 *
 */
class CTruiInfoMessageView : public CAknView
    {
public:
    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiInfoMessageView's instance
     */    
    static CTruiInfoMessageView* NewL();
    
    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiInfoMessageView's instance
     */
    static CTruiInfoMessageView* NewLC();    

    /**
    * Destructor.
    */    
    virtual ~CTruiInfoMessageView();

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

    CTruiInfoMessageView();
    
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
    
private: // data
    
    CTruiInfoMessageContainer* iContainer;    
    };

#endif // C_TRUIINFOMESSAGEVIEW_H

