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
* Description:  header file of control group and button group 
*
*/

#ifndef C_CBUBBLECTRL_H
#define C_CBUBBLECTRL_H

#include <peninputlayoutbasecontrol.h>
#include <AknsConstants.h>
#include <AknsItemID.h>
#include <AknLayout2Def.h>

class CBubbleCtrl: public CFepUiBaseCtrl
    {
public: 
    //constructor and destructor
    /**
     * Destructor.        
     *
     * @since S60 V4.0        
     */                  
    IMPORT_C virtual ~CBubbleCtrl();

    /**
     * Factory function.
     *
     * @since S60 V4.0        
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CButtonBase class        
     */
    IMPORT_C static CBubbleCtrl* NewL(const TRect& aRect,
                             CFepUiLayout* aUiLayout,
                             TInt aCtrlId);
    
    IMPORT_C virtual void Draw();

    IMPORT_C virtual void Hide(TBool aShowFlag);    
        
    IMPORT_C void SetBitmapParam(CFbsBitmap* aBmpId,
                        CFbsBitmap* aMaskBmpId,
                        TAknsItemID aBgSkinId);
    IMPORT_C void SetBitmapParam(CFbsBitmap* aBmpId,
                        CFbsBitmap* aMaskBmpId,
                        TAknsItemID aFirstBmpId,
                        TAknsItemID aMiddleBmpId,
                        TAknsItemID aLastBmpId);	                        
    /**
     * Set text line layout
     *
     * @since S60 V4.0
     * @param aTextFormat Text line layout to be set
     * @return None
     */
    IMPORT_C void SetTextFormat(TAknTextLineLayout aTextFormat);
    
    /**
     * Set text to be shown on button
     *
     * @since S60 V4.0
     * @param aText Text to be shown on button
     * @return None
     */
    IMPORT_C void SetTextL(const TDesC& aText);

    IMPORT_C void Popup(const TRect& aRect);
    
    IMPORT_C void Close();

    IMPORT_C TBool IsShowing();

    IMPORT_C void SetFrameDiff(TInt aLeftDiff,TInt aTopDiff,TInt aRightDiff,TInt aBottomDiff);
    
    void Freeze();
    
    void UnFreeze( TBool aUpdate = ETrue );
    inline void SetTextColorIndex( TInt aTxtClrIndex );
    inline TInt TextColorIndex() const;
protected:
    IMPORT_C CBubbleCtrl(const TRect& aRect,
                CFepUiLayout* aUiLayout,
                TInt aCtrlId);
                    
    IMPORT_C void ConstructL(TInt aBmpId, TInt aMaskId);
       
private:
    CFbsBitmap* iForgroundBmp;
    CFbsBitmap* iForgroundBmpMask;
    
    TAknsItemID iBgSkinId;
    TAknsItemID iFirstBmpId;
    TAknsItemID iMiddleBmpId;
    TAknsItemID iLastBmpId;
    
    
    HBufC* iText;
    
    TAknTextLineLayout iTextFormat;     
    
    TBool iShowing;   
    
    TInt iLeftDiff; 
    TInt iTopDiff; 
    TInt iRightDiff; 
    TInt iBottomDiff; 
    TBool iFreeze;
    TRect iInvalidRect;
    
    TBool iNeedRedraw; // redraw flag after text changed.
    TInt iTextColorIndex;    
    };
    
inline void CBubbleCtrl::SetTextColorIndex( TInt aTxtClrIndex )
	{
	iTextColorIndex = aTxtClrIndex;	
	}
	
inline TInt CBubbleCtrl::TextColorIndex() const
	{
	return iTextColorIndex;
	}

class CTooltipBubbleCtrl: public CBubbleCtrl
    {
public: 
    //constructor and destructor
    /**
     * Destructor.        
     *
     */                  
    virtual ~CTooltipBubbleCtrl();

    /**
     * Factory function.
     *
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @return An instance of CButtonBase class        
     */
    static CTooltipBubbleCtrl* NewL(const TRect& aRect,
                             CFepUiLayout* aUiLayout,
                             TInt aCtrlId);
    
// From CFepUiBaseCtrl	
    
	/**
	 * Handle pointer up event
	 *
	 * @param aPoint The point position relative the layout
	 * @return The control which handles the event.     
	 */                  
	CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);

protected:
	CTooltipBubbleCtrl( const TRect& aRect,
                           CFepUiLayout* aUiLayout,
                           TInt aCtrlId );		       
private:
    };

#endif //C_CBUBBLECTRL_H