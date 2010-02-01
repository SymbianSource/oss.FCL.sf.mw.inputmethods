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
* Description:  CTruiShortcutEditTextView class of TrainingUI
*
*/


#ifndef C_TRUISHORTCUTEDITTEXTVIEW_H
#define C_TRUISHORTCUTEDITTEXTVIEW_H

#include <aknview.h>

class CTruiTextSettingPageContainer;

// UID of view
const TUid KTruiShortcutEditTextViewId = {4};

/**
 *  CTruiShortcutEditTextView view class.
 *
 *  @code
 *    CTruiShortcutEditTextView* view = CTruiShortcutEditTextView::NewLC(); 
 *    AddViewL( view );
 *    CleanupStack::Pop();
 *  @endcode
 *
 */
class CTruiShortcutEditTextView : public CAknView
    {        
public:

    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiShortcutEditTextView's instance
     */    
    static CTruiShortcutEditTextView* NewL();
    
    /** 
     * Two phase construction.
     * 
     * @return Pointer to CTruiShortcutEditTextView's instance
     */
    static CTruiShortcutEditTextView* NewLC();    

    /**
    * Destructor.
    */    
    virtual ~CTruiShortcutEditTextView();

    inline const TVwsViewId& StartViewId()
        {
        return iStartView;
        };

// from base class CAknView

    /**
    * From CAknView
    * Returns views id.
    *
    * @return id for this view.
    */
    TUid Id() const;
  
private:

    CTruiShortcutEditTextView();
    
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
    
    /**
     * Pointer to the component control
     * own.  
     */
    CTruiTextSettingPageContainer* iContainer;
    
    TVwsViewId iStartView;
    };

#endif
