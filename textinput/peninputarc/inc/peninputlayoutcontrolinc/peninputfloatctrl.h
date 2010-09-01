/*
* Copyright (c) 2006-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of Floating window
*
*/


#ifndef C_PENINPUTFLOATINGCTRL_H
#define C_PENINPUTFLOATINGCTRL_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <coecntrl.h>

// CLASS DECLARATION

/**
 *  CPeninputFloatingCtrl
 * 
 */
NONSHARABLE_CLASS(CPeninputFloatingCtrl): public CCoeControl
    {
public: // Constructors
    /**
     * C++ default constructor.
     * 
     * @since S60 v5.0
     * @return None
     */   
    CPeninputFloatingCtrl();
    
    /**
     * C++ destructor
     *
     * @since S60 v5.0
     * @return None
     */
    ~CPeninputFloatingCtrl();

public: // from base class CCoeControl
    /**
     * From CCoeControl
     * Draw this control
     * 
     * @since S60 v5.0
     * @param aRect the rectangle of this view that needs updating
     * @return None
     */    
    virtual void Draw( const TRect& aRect ) const;
  
public:
    /**
     * Show this floating control
     * 
     * @since S60 v5.0
     * @param aRect rect of control, in screen coord-sys
     * @return None
     */
    void Show( const TRect& aRect );

    /**
     * Show this floating control
     * 
     * @since S60 v5.0
     * @param aRect rect of control, in screen coord-sys
     * @param aOrdinalPos Ordinal position
     * @param aOrdinalPriority Ordinal priority
     * @return None
     */
    void Show( const TRect& aRect, TInt aOrdinalPos, TInt aOrdinalPriority );
    
    /**
     * Hide this floating control
     * 
     * @since S60 v5.0
     * @return None
     */
    void Hide();
    
    /**
     * Test if transparency is supported by this control
     * 
     * @since S60 v5.0
     * @return ETrue if support
     */
    TBool SupportTransparent() const;
    
protected:
    /**
     * Symbian constructor
     * 
     * @since S60 v5.0
     * @return None
     */
    void BaseConstructL();
    
private:
    TBool iSupportTransparent;
    };

#endif // C_PENINPUTFLOATINGCTRL_H
