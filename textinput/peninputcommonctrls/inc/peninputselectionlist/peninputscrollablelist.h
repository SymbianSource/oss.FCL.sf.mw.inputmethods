/*
* Copyright (c) 2008-2008 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of button and dragbar
*
*/

#ifndef C_FEPLAYOUTSCROLLABLELIST_H
#define C_FEPLAYOUTSCROLLABLELIST_H

#include <peninputlayoutchoicelist.h>
#include <AknLayout2Def.h>
#include <AknsConstants.h>
#include <e32cmn.h>

#define FORMATPAGEINFO(des, ktxt, parm...) {des.Format(ktxt, parm);};

class CAknFepCtrlCommonButton;

class CFepLayoutScrollableList : public CFepLayoutChoiceList, public MEventObserver
    {
public:
    IMPORT_C static CFepLayoutScrollableList* NewL(CFepUiLayout* aUiLayout,
                                                   TInt aControlId,
                                                   TSize aItemSize,
                                                   TInt aHorizontalMargin,
                                                   TInt aVerticalMargin,
                                                   TSize aNaviSize,
                                                   TSize aNaviInnerSize);

    IMPORT_C static CFepLayoutScrollableList* NewLC(CFepUiLayout* aUiLayout,
                                                    TInt aControlId,
                                                    TSize aItemSize,
                                                    TInt aHorizontalMargin,
                                                    TInt aVerticalMargin,
                                                    TSize aNaviSize,
                                                    TSize aNaviInnerSize);

    IMPORT_C virtual ~CFepLayoutScrollableList();

    IMPORT_C void ConstructFromResourceL();

    IMPORT_C void SizeChanged(TSize aItemSize,
                              TInt aHorizontalMargin,
                              TInt aVerticalMargin,
                              TSize aNaviSize,
                              TSize aNaviInnerSize);

    IMPORT_C void SetTextProperty(const CFont* aFont,
                                  TRgb aFontColor);

    IMPORT_C void SetImgFrId(TAknsItemID aBgFrId, 
                           TAknsItemID aNaviFrId,
                           TAknsItemID aNaviActiveFrId,
                           TAknsItemID aNaviDimFrId,
                           TAknsItemID aItemFrId,
                           TAknsItemID aItemActiveFrId);

    inline void SetTextLineLayout(TAknTextLineLayout aTextLine);

    inline void SetPageInfoTextLineLayout(TAknTextLineLayout aTextLine);
    inline void SetPageInfoFormatL(const TDesC& aFormat);

    // if current page include special item, the count also include
    // special item
    inline void SetOnePageItemCount(TInt aItemCount);

    inline void SetSpecialItemOn(TBool aOn);

    // get whole control width except item width
    inline TInt CtrlWidthExceptItem();

    // get whole control height
    inline TInt CtrlHeight();

    IMPORT_C void Draw();
    
    IMPORT_C void ReCalcLayout();
    
    IMPORT_C void UpdateNaviButtonStates();
	
	IMPORT_C void SetListSeperationRect( const TRect& aRect );
	
	inline void SetPageInfoResID( TInt aResID );

    /**
     * Hide or shoe control. A hiden control will not handle the event.
     *
     * @since S60 V4.0
     * @param aFlag ETrue if want to hide the control.
     */                                               
    IMPORT_C virtual void Hide(TBool aFlag);
    
protected:

    IMPORT_C CFepLayoutScrollableList(CFepUiLayout* aUiLayout,
                             TInt aControlId,
                             TSize aItemSize,
                             TInt aHorizontalMargin,                             
                             TInt aVerticalMargin,
                             TSize aNaviSize,
                             TSize aNaviInnerSize);

    IMPORT_C void ConstructL(); 
    
    IMPORT_C TInt HitTest(const TPoint& aPoint);

    IMPORT_C virtual void DrawItem(TInt aItemIndex, TBool aItemActive = EFalse);

    IMPORT_C virtual void CalculateItemWidth();
    
    IMPORT_C virtual void DrawPageInfo();


    void ScrollPageUp();
    
    void ScrollPageDown();

    IMPORT_C void Move(const TPoint& aOffset);
                            
private:
    void CalcPageInfo();
protected:
    //from MEventObserver
    IMPORT_C void HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl,
                            const TDesC& aEventData);
                            
    IMPORT_C void UpdatePageInfoL( TInt aCurrentPage, TInt aTotalPage );                        
	
#ifdef RD_TACTILE_FEEDBACK	
	// Update tactile feedback area when scroll page up and down.
	void UpdateFeedbackArea();
#endif // RD_TACTILE_FEEDBACK
	
protected:
    TPoint iTopLeftPoint;
    TSize iItemSize;
    TSize iNaviSize;
    TSize iNaviInnerSize;
    TInt iHorizontalMargin;
    TInt iVerticalMargin;
    TInt iBaselineOffset;
    const CFont* iFont;
    TRgb iFontColor;
    TAknTextLineLayout iTextLine;
    TBool iTextLineSet;    
    TAknTextLineLayout iPageInfoTextLine;
    TBool iPageInfoLineSet;

    RPointerArray<CFbsBitmap> iBmpList;

    TAknsItemID iBgFrId;
    // used for special item
    TAknsItemID iItemFrId;
    TAknsItemID iItemActiveFrId;

    TBool iSpecialItemOn;
    TBool iNaviBtnShown;

    HBufC* iPageInfoFormat;
    HBufC* iPageInfo;
    TInt iCurrentPage;
    TInt iTotalPages;

    //TPoint iTlPos;
    TInt iOnePageItemCnt;
    TRect iContentRect;
    TRect iScrollRect;
    TRect iPageUpRect;
    TRect iPageDownRect;
    TRect iPageInfoRect;
    RArray<TRect> iItemRects;
    CAknFepCtrlCommonButton* iPageUp;
    CAknFepCtrlCommonButton* iPageDown;
    
    // List speration 
    TRect iSeperationRect;
    CFbsBitmap* iSeperationBmp;	//own
    CFbsBitmap* iSeperationMaskBmp; // own
    
    // Res ID of page info
    TInt iPageInfoResID;
    
    TRect iLastPageItemRect;
    TInt iInitItemWidth;
    };

inline void CFepLayoutScrollableList::SetTextLineLayout(TAknTextLineLayout aTextLine)
    {
    iTextLine = aTextLine;
    iTextLineSet = ETrue;
    }
inline void CFepLayoutScrollableList::SetPageInfoTextLineLayout(TAknTextLineLayout aTextLine)
    {
    iPageInfoTextLine = aTextLine;
    iPageInfoLineSet = ETrue;
    }

inline void CFepLayoutScrollableList::SetPageInfoFormatL(const TDesC& aFormat)
    {
    delete iPageInfoFormat;
    iPageInfoFormat = NULL;
    iPageInfoFormat = aFormat.AllocL();

    // on the assumption that page info do not beyond 100
    delete iPageInfo;
    iPageInfo = NULL;
    iPageInfo = HBufC::NewL(aFormat.Length() + 6);
    }

inline void CFepLayoutScrollableList::SetOnePageItemCount(TInt aItemCount)
    {
    iOnePageItemCnt = aItemCount;
    }

inline void CFepLayoutScrollableList::SetSpecialItemOn(TBool aOn)
    {
    iSpecialItemOn = aOn;
    }

inline TInt CFepLayoutScrollableList::CtrlWidthExceptItem()
    {
    return 4*iHorizontalMargin + iNaviSize.iWidth;
    }

inline TInt CFepLayoutScrollableList::CtrlHeight()
    {
    return (iOnePageItemCnt + 1)*iVerticalMargin + iOnePageItemCnt*iItemSize.iHeight;
    }

inline void CFepLayoutScrollableList::SetPageInfoResID( TInt aResID )	
	{
	iPageInfoResID = aResID;	
	}
#endif //C_FEPLAYOUTSCROLLABLELIST_H
