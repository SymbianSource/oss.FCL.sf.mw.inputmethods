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
* Description:  Implementation for base control
*
*/


#include "peninputlayouttimer.h"
#include "peninputlayoutcandidatewnd.h"
#include "peninputlayout.h"

const TInt KDefaultBorder = 1 ;

// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CCandidateWnd::CCandidateWnd
// C++ default constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CCandidateWnd::CCandidateWnd(const TRect& aRect,TInt aCandidateRectNum,
                              CFepUiLayout* aUiLayout,TInt aCtrlId,
                              TTimeIntervalMicroSeconds32 aHideTimer)
                              :CFepUiBaseCtrl(aRect,aUiLayout,aCtrlId),
                              iSelection(-1),
                              iDelay(aHideTimer),
                              iBaseline(-1),
                              iFontOwnership(EFalse)
    {
    SetControlType(ECtrlCandidateList);

    if(iDelay.Int()!=0)
        iAutoHide=ETrue;
    else
        iAutoHide=EFalse;
    
    if(aCandidateRectNum <=0)
        iCandidateRectNum = 1;
    else
        iCandidateRectNum = aCandidateRectNum;
    
    iFontSpec.iHeight = 0;
    //candidate window is opaque
    SetMaskBkCol(KRgbBlack);
    }

// ---------------------------------------------------------------------------
// CCandidateWnd::~CCandidateWnd
// Destructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CCandidateWnd::~CCandidateWnd()
    {
    //free the candidate buffer
    for(TInt i = 0; i < iCandidateList.Count(); i++)
        {
        HBufC* buf = iCandidateList[i];
        delete buf;
        }
    iCandidateList.Close();
    iCandidateRectList.Close();    
    iCandidateTextRectList.Close();
    delete iHideTimer;

    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        iFont = NULL;    
        }    
    }

// ---------------------------------------------------------------------------
// CCandidateWnd::NewL
// factory function
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CCandidateWnd* CCandidateWnd::NewL(const TRect& aRect,TInt aCandidateRectNum,
                          CFepUiLayout* aUiLayout,TInt aCtrlId,
                          TTimeIntervalMicroSeconds32 aHideTime)                            
    {
    CCandidateWnd* wnd=new (ELeave)CCandidateWnd(aRect,aCandidateRectNum,
                                                aUiLayout,aCtrlId,aHideTime);
    CleanupStack::PushL(wnd);
    wnd->ConstructL ();
    CleanupStack::Pop(wnd);
    return wnd;
    }

// ---------------------------------------------------------------------------
// CCandidateWnd::GetCandidateL
// Set candidate data
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C const TDesC& CCandidateWnd::GetSelCandidate()
    {
    if(iSelection >= 0 && iSelection < iCandidateList.Count()) 
        {
        return *(iCandidateList[iSelection]);
        }
    return KNullDesC();
    }

// ---------------------------------------------------------------------------
// CCandidateWnd::SetAutoHideDelay
// Set auto hide timer delay
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CCandidateWnd::SetAutoHideDelay(TTimeIntervalMicroSeconds32 aDelay)
    {
    iDelay = aDelay;
    if(iDelay.Int()!=0)
        {               
        iAutoHide=ETrue;
        iHideTimer->SetTimer(iDelay);
        }
    else
        {
        iAutoHide=EFalse;
        iHideTimer->Cancel();
        }
        
    }

// ---------------------------------------------------------------------------
// CCandidateWnd::SetCandidateNumL
// Set the number of candidates in one line
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CCandidateWnd::SetCandidateNumL(TInt aNumInLine)
    {
    if(aNumInLine <=0)
        iCandidateRectNum = 1;
    else
        iCandidateRectNum = aNumInLine;
    
    iCandidateRectList.Reset();
    iCandidateTextRectList.Reset();
    TRect rect(Rect());
    
    TInt w = (rect.Width() - KDefaultBorder * (1+iCandidateRectNum) ) 
                                            / iCandidateRectNum;
    for(TInt i=0;i<iCandidateRectNum;i++)
        {
        rect.iBr.iX=rect.iTl.iX+w;
        iCandidateRectList.AppendL(rect);
        iCandidateTextRectList.AppendL(rect);
        rect.iTl.iX=rect.iBr.iX+1; 
        }
    }


// ---------------------------------------------------------------------------
// CCandidateWnd::SetCandidateNumL
// Set the number of candidates in one line
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CCandidateWnd::SetCandidateNumL(TInt aNumInLine,
							const TSize& aCellRectSize, const TRect& aTextRect)
    {
    if(aNumInLine <= 0)
        iCandidateRectNum = 1;
    else
        iCandidateRectNum = aNumInLine;
    
    iCandidateRectList.Reset();
	iCandidateTextRectList.Reset();    
	TRect rect(Rect());
	TRect textRect; //for text drawing
	
	TPoint offset = aTextRect.iTl;
    for(TInt i = 0;i < iCandidateRectNum;i++)
        {
        rect.iBr.iX = rect.iTl.iX + aCellRectSize.iWidth;
        iCandidateRectList.AppendL(rect);
        textRect.iTl = rect.iTl + offset;
        textRect.iBr.iX = textRect.iTl.iX + aTextRect.Width();
        textRect.iBr.iY = textRect.iTl.iY + aTextRect.Height();
        iCandidateTextRectList.AppendL(textRect);

        rect.iTl.iX = rect.iBr.iX + 1; 
        }    
    }

// ---------------------------------------------------------------------------
// CCandidateWnd::SetFont
// Set candidate window font
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CCandidateWnd::SetFont(const TFontSpec& aFontSpec)
    {
    
    iFontSpec = aFontSpec;
    if(iFontOwnership && iFont) //release the owned font
        {
        BitmapDevice()->ReleaseFont(iFont);
        iFont = NULL;    
        }    
    if(BitmapDevice())
        {
        if (KErrNone != BitmapDevice()->GetNearestFontInPixels(iFont,iFontSpec))
            iFont = NULL;            
        }      
    iFontOwnership = ETrue;
    }        

// ---------------------------------------------------------------------------
// CCandidateWnd::SetFont
// Set candidate window font
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//        
EXPORT_C void CCandidateWnd::SetFont(const CFont* aFont)
    {
    if(iFontOwnership && iFont)
        {
        BitmapDevice()->ReleaseFont(iFont);
        iFont = NULL;    
        }    
    iFont = const_cast<CFont*>(aFont);
    if(iFont)
        iFontSpec = iFont->FontSpecInTwips();
    iFontOwnership = EFalse;
    }        

// ---------------------------------------------------------------------------
// CCandidateWnd::Draw
// Draw control
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CCandidateWnd::Draw()
    {
    //do nothing if not ready or hiden or no candidate while in auto hide mode
    if(!AbleToDraw() || (iAutoHide && iCandidateList.Count() <= 0))
        return;
        
    DrawOpaqueMaskBackground();
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());
    TRect wndRect = Rect();

    // ----- draw on bitmaps -----
    DrawBackground();
    //draw two lines: the up left and up top line    
    gc->DrawLine(wndRect.iTl,TPoint(wndRect.iTl.iX,wndRect.iBr.iY));
    gc->DrawLine(wndRect.iTl,TPoint(wndRect.iBr.iX,wndRect.iTl.iY));    

    
        
    //draw candidate text        
    if(iFont)
        BitGc()->UseFont(iFont);
    
    TBuf<1> buf;

    BitGc()->SetBrushStyle( CGraphicsContext::ENullBrush );
    
//    __ASSERT_DEBUG(iCandidateRectList.Count(),EUiLayoutCandiateNumIsNotSet);
    
    TRect rect=iCandidateRectList[0];
    BitGc()->DrawLine(TPoint(rect.iTl.iX,rect.iTl.iY),
                                    TPoint(rect.iTl.iX,rect.iBr.iY));

    TInt baseLine = iBaseline > 0 ? iBaseline : 
                           iCandidateTextRectList[0].Height()*KDEFAULTBASELINERATIO;
                                         
    for(TInt i = 0 ; i < iCandidateRectNum ;i++) 
        {
        //draw bottom line and right line
        TPoint bottom_from(iCandidateRectList[i].iTl.iX,
                                            iCandidateRectList[i].iBr.iY);
        TPoint bottom_to(iCandidateRectList[i].iBr.iX,
                                            iCandidateRectList[i].iBr.iY);
        BitGc()->DrawLine(bottom_from,bottom_to);
                                    
        TPoint right_from(iCandidateRectList[i].iBr.iX,
                                            iCandidateRectList[i].iTl.iY);
        TPoint right_to(iCandidateRectList[i].iBr.iX,
                                            iCandidateRectList[i].iBr.iY);
        BitGc()->DrawLine(right_from,right_to);

        if(i < iCandidateList.Count() && iFont)
            {
            HBufC* data=iCandidateList[i];    

            BitGc()->DrawText(*data,iCandidateTextRectList[i],baseLine,
                                                CGraphicsContext::ECenter );
            }             
        }            
    if(iFont)
        BitGc()->DiscardFont();        
    }

    
// ---------------------------------------------------------------------------
// CCandidateWnd::Move
// Move the candidate window
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//            
EXPORT_C void CCandidateWnd::Move(const TPoint& aOffset)
    {
    CFepUiBaseCtrl::Move(aOffset);
    //Recal candidate rect after moving

    for(TInt i = 0 ; i < iCandidateTextRectList.Count();i++)
        {
        iCandidateTextRectList[i].Move(aOffset);
        iCandidateRectList[i].Move(aOffset);
        }

    }
    
// ---------------------------------------------------------------------------
// CCandidateWnd::DrawIndication
// Draw selection effect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CCandidateWnd::DrawIndication(TInt aSelection,TBool aPressedFlag)
    {
    if(aSelection < 0)
        return;
    CFbsBitGc* gc = static_cast<CFbsBitGc*>(BitGc());        
    
    TRect rect = iCandidateTextRectList[aSelection];        


    gc->Activate( BitmapDevice() );
    const TUint8 blackMap = 128;
    const TUint8 whiteMap = 240;
    if(aPressedFlag) //need press effect
        {        
        gc->SetFadingParameters(blackMap, whiteMap);
        gc->SetFaded(ETrue);
        }
    else
        gc->SetFaded(EFalse);
    
    TPoint bmpPos = rect.iTl;
    gc->SetPenColor( PenColor() );    
    if(BackgroundBmp())
        gc->BitBlt(bmpPos, BackgroundBmp(), rect);
    else
        {
        gc->SetBrushColor( BkColor());
        gc->SetBrushStyle( CGraphicsContext::ESolidBrush );        
        gc->SetPenColor(BkColor());
        gc->SetPenStyle(CGraphicsContext::ESolidPen);
        gc->DrawRect(rect);
        }

    gc->UseFont(iFont);
    
    HBufC* data=iCandidateList[aSelection];    
    gc->SetBrushStyle( CGraphicsContext::ENullBrush );
    
    TInt baseLine = iBaseline > 0 ? iBaseline : 
                           iCandidateTextRectList[0].Height()*KDEFAULTBASELINERATIO;
    gc->SetPenColor( PenColor());
    gc->SetPenStyle(CGraphicsContext::ESolidPen);
    gc->DrawText(*data,rect,baseLine,CGraphicsContext::ECenter );

    gc->DiscardFont();    

    UpdateArea(rect, EFalse);
    gc->SetFaded(EFalse);    
    }
    
// ---------------------------------------------------------------------------
// CCandidateWnd::HandlePointerDownEventL
// handle pointer down event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CCandidateWnd::HandlePointerDownEventL( const TPoint& aPt)
    {    
    //call base to set state
    CFepUiBaseCtrl::HandlePointerDownEventL(aPt);    
    for(TInt i=0;i < iCandidateRectList.Count() && i<iCandidateList.Count();i++)
        {        
        if(iCandidateRectList[i].Contains(aPt))
            {
            if(iHideTimer)
                iHideTimer->Cancel();

            //selected
            iSelection = i;           
            DrawIndication(i);
            iPointerLeft = EFalse;
            iPointerLeftCandidate = EFalse;
            return this;
            }
        }
    
    iSelection = -1; //no selection
    return this;
    }
// ---------------------------------------------------------------------------
// CCandidateWnd::HandlePointerMoveEventL
// handle pointer move event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CCandidateWnd::HandlePointerMoveEventL( const TPoint& aPoint)
    {    
    for(TInt i=0;i < iCandidateRectList.Count() && i<iCandidateList.Count();i++)
        {        
        if(iCandidateRectList[i].Contains(aPoint))
            {
            //selected
            if(i != iSelection)
                {
                if(!iPointerLeftCandidate)
                    {                    
                    DrawIndication(iSelection,EFalse);
                    iPointerLeftCandidate = ETrue; //pointer move to another candidate
                    }
                }
            else
                {
                if(iPointerLeftCandidate)
                    {                    
                    DrawIndication(iSelection,ETrue);
                    iPointerLeftCandidate = EFalse;
                    }
                }            
            return this;
            }
        }    
    return this;    
    }
// ---------------------------------------------------------------------------
// CCandidateWnd::HanldePointerUpEventL
// handle pointer up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C CFepUiBaseCtrl* CCandidateWnd::HandlePointerUpEventL( const TPoint& aPoint)
    {      
    TBool bContinue = ETrue;       
    for(TInt i=0;i < iCandidateRectList.Count() && i < iCandidateList.Count()
                 && bContinue;i++)
        {        
        if(iCandidateRectList[i].Contains(aPoint))
            {
            if(iHideTimer)
                iHideTimer->Cancel();

            //selected
            if(iSelection != i)
                {
                //cancel the selection if pen down and up in different place
                DrawIndication(iSelection,EFalse);
                iSelection = -1; 
                iPointerLeftCandidate = EFalse;
                iPointerLeft = EFalse;
                bContinue = EFalse;
                
                }
             else
                {
                if(iCandidateList.Count()>0)
                    {
                    HBufC* data=iCandidateList[i];
                    if(data->Length() > 0)
                        {
                        //report to observer
                        ReportEvent(EEventCandidateSelected,*data);
                        }
                    }
                DrawIndication(iSelection,EFalse);

                bContinue = EFalse;
                }
            }
        }
    //reset the timer
    if(iHideTimer)
        iHideTimer->SetTimer(iDelay);
        
    return this;
    }

// ---------------------------------------------------------------------------
// CCandidateWnd::SetCandidateL
// Set candidate data
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CCandidateWnd::SetCandidateL(const RPointerArray<HBufC>& aCandidateData)
    {
    iCandidateList.ResetAndDestroy();

    if(aCandidateData.Count())
        {
        for(TInt i = 0; i < aCandidateData.Count(); i++)
            {
            HBufC* data = aCandidateData[i]->AllocL();
            CleanupStack::PushL(data);
            iCandidateList.AppendL(data);
            CleanupStack::Pop(data);
            }
        Hide(EFalse);
        Draw();
        //report event, so as hwr window may redraw the stroke in case of that
        // hwr window is overlapped with candidate window
        RootControl()->HandleControlEvent(EEventRegionUpdated,this,KNullDesC);       
        
        iSelection = 0;    
        }
    else
        iSelection = -1;    
    
    UpdateArea(Rect(),EFalse);

    if(iAutoHide && iCandidateList.Count()>0)
        iHideTimer->SetTimer(iDelay);
    }


// ---------------------------------------------------------------------------
// CCandidateWnd::HandleTimerOut
// handle hide timer event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
EXPORT_C void CCandidateWnd::HandleTimerOut(TInt /*aTimeType*/)
    {
    Hide(ETrue);
    }
    
    
// ---------------------------------------------------------------------------
// CCandidateWnd::ConstructL
// Second phrase constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CCandidateWnd::ConstructL()
    {
    BaseConstructL();
    if(BitmapDevice() && iFontSpec.iHeight)
        {        
        if (KErrNone != BitmapDevice()->GetNearestFontInPixels(iFont,iFontSpec))
            iFont = NULL;
        }
    
    SetCandidateNumL(iCandidateRectNum);
    if(iAutoHide)
        {
        iHideTimer = CLayoutTimer::NewL(this,CLayoutTimer::ECandidateClearTimer);
        }               
    }

// ---------------------------------------------------------------------------
// CCandidateWnd::HandlePointerLeave
// Handle pointer leave event
// ---------------------------------------------------------------------------
//           
EXPORT_C void CCandidateWnd::HandlePointerLeave(const TPoint& aPt)
    {
    DrawIndication(iSelection,EFalse);
    iPointerLeftCandidate = ETrue; //pointer left the cell.
    CFepUiBaseCtrl::HandlePointerLeave(aPt);
    }

// ---------------------------------------------------------------------------
// CCandidateWnd::OnLayoutDraggingStart
// Response to layout dragging start event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CCandidateWnd::OnLayoutDraggingStart()
    {
    if(iHideTimer)
        iHideTimer->Cancel();
    }
    
// ---------------------------------------------------------------------------
// CCandidateWnd::OnLayoutDraggingEnd
// Response to layout dragging end event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CCandidateWnd::OnLayoutDraggingEnd()
    {
    }    

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::OnDeActivate
// Response to layout de activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CCandidateWnd::OnDeActivate()
    {    
    CFepUiBaseCtrl::OnDeActivate();
    if(iAutoHide)
        iHideTimer->Cancel();
    }

// ---------------------------------------------------------------------------
// CFepUiBaseCtrl::OnActivate
// Response to layout activation event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//     
EXPORT_C void CCandidateWnd::OnActivate()
    {
    //do nothing    
    CFepUiBaseCtrl::OnActivate();
    if(iAutoHide)
        iHideTimer->SetTimer(iDelay);    
    }
    
//end of file

