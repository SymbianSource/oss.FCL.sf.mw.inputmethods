/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of virtual keyboard control
*
*/

#ifndef M_PENINPUTVKBPREVIEWBUBBLERENDERER_H
#define M_PENINPUTVKBPREVIEWBUBBLERENDERER_H

// System includes
#include <e32def.h>
#include <fbs.h>

// User includes
#include "peninputlayoutvkb.h"

// CLASS DECLARATION

/**
 * MPeninputVkbPreviewBubbleRenderer
 * Interface for preview bubble extension.
 *  
 */
class MPeninputVkbPreviewBubbleRenderer
{
public:
    /**
     * Attach this extension to a virtual keyboard object.
     * 
     * @since S60 S60 v5.0
     * @return None.
     */
    virtual void Attach() = 0;
    
    /**
     * Detach this extension from a virtual keyboard object.
     * 
     * @since S60 S60 v5.0
     * @return None.
     */    
    virtual void Detach() = 0;
    
    /**
     * Enable preview bubble 
     * 
     * @since S60 S60 v5.0
     * @param aFlag ETrue to enable preview bubble.
     * @return None.
     */    
    virtual void ShowBubble( TBool aFlag ) = 0;
    
    /**
     * Draw preview bubble.
     * 
     * @since S60 S60 v5.0
     * @param aKey Virtual key.
     * @param aVkbPos Position of virtual keyboard.
     * @param aLayoutPos Position of layout, in screen coord-sys.
     * @param aSize Size of bubble.
     * @return None.
     */    
    virtual void DrawBubble( CVirtualKey* aKey, const TPoint& aVkbPos, 
                             const TPoint& aLayoutPos, const TSize& aSize  ) = 0;
    
    /**
     * Clear preview bubble.
     * 
     * @since S60 S60 v5.0
     * @return None.
     */    
    virtual void ClearBubble() = 0;

    /**
     * Set icon and background of bubble.
     * 
     * @since S60 S60 v5.0
     * @param aBmpId Bitmap of icon.
     * @param aMaskBmpId Bitmap of mask.
     * @param aBgSkinId Skin id of background.
     * @return None.
     */
    virtual void SetBubbleBitmapParam( CFbsBitmap* aBmpId, CFbsBitmap* aMaskBmpId, 
                                       TAknsItemID aBgSkinId )=0;
    
    /**
     * Set text format
     * 
     * @since S60 S60 v5.0
     * @param aTextFormat Text format.
     * @return None.
     */    
    virtual void SetTextFormat( TAknTextLineLayout aTextFormat ) = 0;
    
    /**
     * Set margin of bubble
     * 
     * @since S60 S60 v5.0
     * @param aLeftDiff Left margin.
     * @param aTopDiff Top margin.
     * @param aRightDiff Right margin.
     * @param aBottomDiff Bottom margin.
     * @return None.
     */
    virtual void SetFrameDiff( TInt aLeftDiff, TInt aTopDiff, TInt aRightDiff,
                               TInt aBottomDiff )=0;
};

#endif //M_PENINPUTVKBPREVIEWBUBBLERENDERER_H
