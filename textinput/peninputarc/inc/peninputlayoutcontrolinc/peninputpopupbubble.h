/*
* Copyright (c) 2005-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description: Header file of CPopupBubbleCtrl
*
*/

#ifndef C_CPOPUPBUBBLECTRL_H
#define C_CPOPUPBUBBLECTRL_H

#include <peninputlayoutbasecontrol.h>
#include <AknsConstants.h>
#include <AknsItemID.h>
#include <AknLayout2Def.h>
#include <coecntrl.h>
#include <aknsskininstance.h>
#include <e32cmn.h>

class CPopupBubbleCtrl;

/**
 *  CPopupBubbleWnd
 *  
 *  CCoeControl based class used for drawing
 * 
 */
NONSHARABLE_CLASS(CPopupBubbleWnd) : public CCoeControl
    {
public:
    /**
     * Destructor.        
     */
    virtual ~CPopupBubbleWnd();

    /**
     * Symbian constructor.        
     *
     * @param aRect control rect
     * @param aParent parent control
     * @return An instance of CPopupBubbleWnd class        
     */     
    static CPopupBubbleWnd* NewL(const TRect& aRect, CPopupBubbleCtrl& aParent);
    
    /**
     * Symbian constructor.       
     *
     * @param aRect control rect
     * @param aParent parent control
     * @return An instance of CPopupBubbleWnd class        
     */
    static CPopupBubbleWnd* NewLC(const TRect& aRect, CPopupBubbleCtrl& aParent);

    /**
     * Draw control.       
     *
     * @param aRect control rect     
     */    
    void Draw(const TRect& aRect) const;
    
    /**
     * Test transparency support.       
     *
     * @return ETrue if transparency is supported, otherwise EFalse.     
     */    
    inline TBool SupportTransparent() const
        {
        return iSupportTransparent;
        };
    
    /**
     * Handle size change.       
     */  
    void SizeChanged();
    
    /**
     * Handle resource change.       
     * @param aType resource change event.
     */ 
    void HandleResourceChange(TInt aType);
    
protected:
    
    /**
    * Symbian OS 2nd phase constructor 
    */
    void ConstructL(const TRect& aRect);
    
    /**
    * C++ constructor
    * @param aParent CPopupBubbleCtrl parent control
    */
    CPopupBubbleWnd(CPopupBubbleCtrl& parent);
 
    /**
    * Size images used for drawing
    * @param aId Skin item Id
    * @param aRect Rect to be used for sizing
    * @param aBitmap Bitmap to be sized 
    * @param aDevice BitmapDevice to be used for sizing
    * @param aBitGc BitGc to be used for sizing
    */    
    void SizeImage(TAknsItemID aId, TRect aRect, CFbsBitmap* aBitmap, CFbsBitmapDevice* aDevice, CFbsBitGc* aBitGc );
    
private:
    /**
     * Draw transparent background.       
     *
     * @param aRect control rect     
     */    
    void DrawTransparentBackground(const TRect& aRect) const;    
    
private:
    
    enum TItems
        {
        ETempItem = 0,
        ELeftItem,
        ERightItem,
        EMiddleItem,
        ELastItem
        };
    
    RPointerArray<CFbsBitmap> iBitmap;
    RPointerArray<CFbsBitmapDevice> iDevice;
    RPointerArray<CFbsBitGc> iBitGc; 
    TRect iRect[ELastItem];
    
    CPopupBubbleCtrl& iParent;
    TBool iSupportTransparent;
    };

/**
 *  CPopupBubbleCtrl
 *  
 *  Bubble with own window
 * 
 */
class CPopupBubbleCtrl: public CFepUiBaseCtrl
    {
public: 
    //constructor and destructor
    /**
     * Destructor.        
     */                  
    IMPORT_C virtual ~CPopupBubbleCtrl();
    /**
     * Symbian constructor.        
     *
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control. Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CPopupBubbleCtrl class        
     */
    IMPORT_C static CPopupBubbleCtrl* NewL(const TRect& aRect,
                             CFepUiLayout* aUiLayout,
                             TInt aCtrlId);
    
    
    /**
     * SetIconOffsetAndSize
     *
     * @param aOffset The offset to be moved
     * @param aOffset The size to be set
     */    
    inline void SetIconOffsetAndSize( 
            const TSize& aOffset, const TSize& aSize );
    
    /**
     * Draw the control
     *
     */      
    IMPORT_C void Draw();
    
    
    /**
     * Move the control
     *
     * @param aOffset The offset to be moved
     */    
    IMPORT_C void Move(const TPoint& aOffset);

    /**
     * Set bitmap parameters
     *
     * @param aBmpId Bitmap to be used. Can be NULL
     * @param aMaskBmpId Mask to be used. Can be NULL
     * @param aFirstBmpId Skin Bitmap id.
     * @param aMiddleBmpId Skin Bitmap id.
     * @param aLastBmpId Skin Bitmap id.
     */    
    IMPORT_C void SetBitmapParamL(
                        CFbsBitmap* aBmpId,
                        CFbsBitmap* aMaskBmpId,
                        TAknsItemID aFirstBmpId,
                        TAknsItemID aMiddleBmpId,
                        TAknsItemID aLastBmpId);                            

    /**
     * Hide the control
     *    
     * @param aShowFlag visibility flag. ETrue if hidden, otherwise EFalse
     */
    IMPORT_C void Hide(TBool aShowFlag); 

    /**
     * Set text format used for text drawing
     *    
     * @param aTextFormat bubble text format
     */    
    IMPORT_C void SetTextFormat(TAknTextLineLayout aTextFormat);

    /**
     * Set bubble text
     *    
     * @param aText descriptor containing bubble text
     */        
    IMPORT_C void SetTextL(const TDesC& aText );

    /**
     * Get bubble text
     *    
     * @param aText descriptor to be filled in with data
     */     
    inline void GetText( TDes& aText );
    
    /**
     * Check if text is set
     *    
     * @return ETrue is text was set. Otherwise EFalse.
     */     
    inline TBool HasText();

    /**
     * Set text color index used for drawing
     *    
     * @param aTxtClrIndex Color index.
     */     
    inline void SetTextColorIndex( TInt aTxtClrIndex );

    /**
     * Get text color index used for drawing
     *    
     * @return Color index.
     */         
    inline TInt TextColorIndex() const;
protected:
    
    /**
     * C++ constructor.        
     *
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control. Ownership not transferred
     * @param aControlId control Id
     */    
    IMPORT_C CPopupBubbleCtrl(const TRect& aRect,
                CFepUiLayout* aUiLayout,
                TInt aCtrlId);
private:
    CFbsBitmap* iForgroundBmp;
    CFbsBitmap* iForgroundBmpMask;
    
    TAknsItemID iFirstBmpId;
    TAknsItemID iMiddleBmpId;
    TAknsItemID iLastBmpId;
      
    HBufC* iText;
    TAknTextLineLayout iTextFormat;
    TInt iTextColorIndex;

    TSize iIconOffset;
    TSize iIconSize;    
    
    friend class CPopupBubbleWnd;
    };
   
inline void CPopupBubbleCtrl::SetTextColorIndex( TInt aTxtClrIndex )
    {
    iTextColorIndex = aTxtClrIndex; 
    }

inline void CPopupBubbleCtrl::SetIconOffsetAndSize( 
        const TSize& aOffset, const TSize& aSize )
    {
    iIconOffset = aOffset;
    iIconSize = aSize;
    }

inline void CPopupBubbleCtrl::GetText( TDes& aText )
    {
    if (iText)
        {
        aText.Copy( *iText );
        }
    }

inline TBool CPopupBubbleCtrl::HasText()
    {
    TBool ret = EFalse;

    if (iText && iText->Length() > 0)
        {
        ret = ETrue;
        }

    return ret;
    }

/******************************************************************************/

#endif //C_CPOPUPBUBBLECTRL_H
