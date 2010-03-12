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
* Description:  header file of Floating bubble
*
*/


#ifndef C_PENINPUTFLOATINGBUBBLE_H
#define C_PENINPUTFLOATINGBUBBLE_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <AknLayout2Def.h>

#include "peninputfloatctrl.h"

// CLASS DECLARATION

/**
 *  CPeninputFloatingBubble
 * 
 */
NONSHARABLE_CLASS(CPeninputFloatingBubble) : public CPeninputFloatingCtrl
    {
public: // Constructors
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created object
     */
    static CPeninputFloatingBubble* NewL();
    /**
     * Symbian constructor.
     *
     * @since S60 v5.0
     * @return Pointer to created object
     */
    static CPeninputFloatingBubble* NewLC();
    
    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     * @return None
     */
    ~CPeninputFloatingBubble();
    
public: //from base class CCoeControl
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
     * Set background image
     * 
     * @since S60 v5.0
     * @param aBkgndImage skin item id of background image
     * @return None
     */
    void SetBackgroundImage( TAknsItemID aBkgndImage );
 
    /**
     * Set text 
     * 
     * @since S60 v5.0
     * @param aText new text to be shown in this control
     * @return None
     */    
    void SetText( const TDesC& aText );
    
    /**
     * 
     * @since S60 v5.0
     * @param aTextFormat text layout
     * @return None
     */    
    void SetTextFormat( TAknTextLineLayout aTextFormat );
    
    /**
     * 
     * @since S60 v5.0
     * @param aIconImage icon to be shown in this control
     * @param aIconMask mask of icon 
     * @return None
     */    
    void SetIcon( CFbsBitmap* aIconImage, CFbsBitmap* aIconMask );
    
private:
    /**
     * C++ default constructor.
     * 
     * @since S60 v5.0
     * @return None
     */  
    CPeninputFloatingBubble();
    
    /**
     * Symbian constructor
     * 
     * @since S60 v5.0
     * @return None
     */    
    void ConstructL();
    
private:
    /**
     * the text to be shown
     * Own
     */
    HBufC* iText;
    
    /**
     * text format
     */
    TAknTextLineLayout iTextFormat;
    
    /**
     * the icon to be shown
     * not own
     */
    CFbsBitmap* iIconImage;
    
    /**
     * mask of icon
     * not own
     */
    CFbsBitmap* iIconMask;
    
    /**
     * skin item id of background
     */
    TAknsItemID iBkgndSkinId;
    };

#endif // C_PENINPUTFLOATINGBUBBLE_H
