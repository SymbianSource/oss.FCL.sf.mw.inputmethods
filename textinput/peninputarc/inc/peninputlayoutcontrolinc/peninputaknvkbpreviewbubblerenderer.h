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

#ifndef C_PENINPUTVKBPREVIEWBUBBLERENDERER_H
#define C_PENINPUTVKBPREVIEWBUBBLERENDERER_H

// User includes
#include "peninputvkbpreviewbubblerenderer.h"

// Forward declarations
class CPeninputFloatingBubble;

// CLASS DECLARATION

/**
 * CPeninputAknVkbPreviewBubbleRenderer
 * CCoeControl-based preview bubble extension  
 *  
 */
class CPeninputAknVkbPreviewBubbleRenderer : public CBase, 
                                             public MPeninputVkbPreviewBubbleRenderer
{
public:
    /**
     * Symbian constructor
     *
     * @since S60 v5.0
     * @return The pointer to created object
     */
    IMPORT_C static CPeninputAknVkbPreviewBubbleRenderer* NewL();
    
    /**
     * C++ destructor
     *
     * @since S60 v5.0
     * @return None
     */    
    virtual ~CPeninputAknVkbPreviewBubbleRenderer();
    
protected: //from base class MPeninputVkbPreviewBubbleRenderer
    /**
     * Attach this extension to a virtual keyboard object.
     * Increase reference count
     * 
     * @since S60 S60 v5.0
     * @return None.
     */    
    virtual void Attach();
    
    /**
     * Detach this extension from a virtual keyboard object.
     * decrease reference count
     * 
     * @since S60 S60 v5.0
     * @return None.
     */        
    virtual void Detach();
    
    /**
     * Enable preview bubble. 
     * 
     * @since S60 S60 v5.0
     * @param aFlag ETrue to enable preview bubble.
     * @return None.
     */     
    virtual void ShowBubble( TBool aFlag );
    
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
                             const TPoint& aLayoutPos, const TSize& aSize );
    
    /**
     * Clear preview bubble.
     * 
     * @since S60 S60 v5.0
     * @return None.
     */     
    virtual void ClearBubble();

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
                                       TAknsItemID aBgSkinId );
    
    /**
     * Set text format
     * 
     * @since S60 S60 v5.0
     * @param aTextFormat Text format.
     * @return None.
     */      
    virtual void SetTextFormat( TAknTextLineLayout aTextFormat );
    
    
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
    virtual void SetFrameDiff( TInt aLeftDiff, TInt aTopDiff, 
                               TInt aRightDiff,TInt aBottomDiff );

private:
    /**
     * C++ constructor
     *
     * @since S60 v5.0
     * @return None
     */    
    CPeninputAknVkbPreviewBubbleRenderer();
    
    /**
     * Symbian constructor
     * 
     * @since S60 v5.0
     * @return None
     */    
    void ConstructL();

private:
    /**
     * reference count
     */
	TInt iRefCount;
	
	/**
	 * Bubble control
	 * Own
	 */
	CPeninputFloatingBubble* iBubbleCtrlEx;
	
};

#endif //C_PENINPUTVKBPREVIEWBUBBLERENDERER_H
