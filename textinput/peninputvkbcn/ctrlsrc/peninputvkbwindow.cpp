/*
* Copyright (c) 2002-2005 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  vkb main window area implementation
*
*/


// system includes
#include <peninputcmdparam.h>            // use global signal
#include <peninputvkbcn.rsg>
#include <peninputlayoutmultilineicf.h>
#include <peninputeventbutton.h>
#include <peninputdragbutton.h>
#include <peninputrepeatbutton.h>
#include <AknLayoutDef.h>
#include <peninputcommonchoicelist.h>
#include <peninputlayout.h>
#include <AknIconUtils.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <barsread.h>
#include <AknsUtils.h>
#include <AknFepGlobalEnums.h>
#include <peninputlayoutinputmodechoice.h>
#include <peninputcmd.h>            // use global signal
#include <aknfeppeninputenums.h>    //command from fep or IME to plugin

#include <aknlayoutscalable_apps.cdl.h>
#include <peninputlayoutbubblectrl.h>
#include <peninputscrollablelist.h>

#include <s32mem.h>
#include <peninputvkbctrllayout.h>
#include <peninputvkbctrlext.h>

// user includes
#include "peninputvkb.hrh"
#include "peninputvkbwindow.h"
#include "peninputvkbevent.h"
#include "peninputvkbclientarea.h"
#include "peninputvkbdataprovider.h"

#include "peninputvkbcnlafdatamgr.h"
#include "peninputvkblayout.h"

// constant definition
const TInt KInvalidImg = -1 ;

const TInt KTooltipLeftMargin = 10;
const TInt KTooltipTopMargin = 0;
const TInt KTooltipRightMargin = 10;
const TInt KTooltipBottomMargin = 0;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::NewL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbWindow* CAknFepVkbWindow::NewL(CFepUiLayout* aUiLayout, 
                                         TInt aControlId,
                                         MAknFepVkbDataProvider* aDataProvider,
                                         CAknFepVkbUiStateMgr* aUiStateMgr)
    {
    CAknFepVkbWindow* self = 
        CAknFepVkbWindow::NewLC(aUiLayout, aControlId, aDataProvider, aUiStateMgr);
    CleanupStack::Pop(self);
    return self;
    }
        
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::NewLC
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbWindow* CAknFepVkbWindow::NewLC(CFepUiLayout* aUiLayout, 
                                          TInt aControlId,
                                          MAknFepVkbDataProvider* aDataProvider,
                                          CAknFepVkbUiStateMgr* aUiStateMgr)
    {
    CAknFepVkbWindow* self = 
        new (ELeave) CAknFepVkbWindow(aUiLayout, aControlId, aDataProvider, aUiStateMgr);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::~CAknFepVkbWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbWindow::~CAknFepVkbWindow()
    {
    delete iIdle;
    delete iLafMgr;
    iLafMgr = NULL;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::CAknFepVkbWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbWindow::CAknFepVkbWindow(CFepUiLayout* aUiLayout, 
                                   TInt aControlId, 
                                   MAknFepVkbDataProvider* aDataProvider,
                                   CAknFepVkbUiStateMgr* aUiStateMgr )
    :CAknFepCtrlBaseWindowExt(aUiLayout, aControlId), 
    iLafMgr(NULL),
    iDataProvider(aDataProvider), 
    iUiStateMgr(aUiStateMgr),
    iFirstTimeConstruct(ETrue), 
    iPopupInited(EFalse),
    iPopupSet(EFalse)    
    {
    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::ConstructL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::ConstructL()
    {
    iLafMgr = CPeninputCnGenericVkbLafMgr::NewL();
    iLafMgr->SetInputModeL((TPluginInputMode)(UiLayout()->PenInputType()));
    
    BaseConstructL();
    
    // Add input context field into the headpane
//    AddContextFieldL();
    
    // Read bitmaps of close and move button
    TInt resID = GetWindowResId();
    SetResourceId( resID );
    
    // Create client area
    iClientArea = CAknFepVkbClientArea::NewL(UiLayout(), 
                                             EAknFepVkbCtrlIdClientArea,
                                             iDataProvider,
                                             iLafMgr);    
                               
    // Then add it into the clientpane
    AddControlL(iClientArea);       
    iClientArea->SetTopParent(this);

    
    //Add popup window
    AddPopupWindowL();

    
    ConstructFromResourceL();
    
    // For addition of ITI features, add a candidate list in vkb window   
    CreateCandidateListOnFSQL();      
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::ConstructFromResourceL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::ConstructFromResourceL()
    {
    CAknFepCtrlBaseWindowExt::ConstructFromResourceL();
    iInputContextField = 
        static_cast<CFepLayoutMultiLineIcf*>
        (Control(EPeninputWindowCtrlIdMultiLineICF));
        
    iBubbleSize = AknLayoutScalable_Apps::popup_char_count_window().LayoutLine();
    iBubbleTextLayout = AknLayoutScalable_Apps::popup_char_count_window_t1(0).LayoutLine();
    
    iInputContextField->MsgBubbleCtrl()->SetTextFormat(iBubbleTextLayout);
    iInputContextField->MsgBubbleCtrl()->SetTextColorIndex( EAknsCIQsnTextColorsCG67 );
    //Change the ID when ID into release  
    iInputContextField->MsgBubbleCtrl()->SetBitmapParam(NULL,
									  NULL,
									  KAknsIIDQsnFrInputPreviewSideL,
									  KAknsIIDQsnFrInputPreviewMiddle,
									  KAknsIIDQsnFrInputPreviewSideR);  

    if(UiLayout()->PenInputType() == EPluginInputModeFSQ)
        {
        iInputContextField->SetMsgBubbleCtrlSize(TSize(iBubbleSize.iW,iBubbleSize.iH));
        }  
    }    

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::ChangeImLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::ChangeImLayout(TInt aRange, TBool aChangeVkb)
    {
    TInt imLayout = iDataProvider->RequestData(EAknFepDataTypeIMLayout);

    if ( iArrowLeftButton )
        {
        iArrowLeftButton->Hide( EAknFepVkbImCnZhuyin == imLayout );    
        }
    if ( iArrowRightButton )
        {
        iArrowRightButton->Hide( EAknFepVkbImCnZhuyin == imLayout );    
        }
    
    iClientArea->ChangeImLayout(aRange, aChangeVkb);
    SizeChangedForTitle();
    Draw();
    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::ChangeVkbLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::ChangeVkbLayout(TInt aVkbLayoutId)
    {
    iClientArea->ChangeVkbLayout(aVkbLayoutId);
    }
        
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::LoopVkbLayout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::LoopVkbLayout(TVkbShiftType aType, TBool aNotify )
    {
    iClientArea->LoopVkbLayout(aType, aNotify);
    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::Draw
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::Draw()
	{
	if ( Hiden() )
		{
		return;
		}
		
    CAknFepCtrlBaseWindowExt::Draw();
    UpdateArea(Rect(), EFalse);
	}
  
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::SetRect
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::SetRect(const TRect& aRect)
    {
	TRect rect = aRect;
	

    if(UiLayout()->PenInputType() == EPluginInputModeFSQ)
        {
        rect.Move( TPoint(- rect.iTl ));
        CFepUiBaseCtrl::SetRect( rect );
        UiLayout()->SetRect(rect);
        }
    else
                                    
    	{
        TSize shadowTl = *((TSize*)iDataProvider->RequestData(EAknFepDataTypeShadowTlSize));
        TSize shadowBr = *((TSize*)iDataProvider->RequestData(EAknFepDataTypeShadowBrSize));

	    TRect shadowRect = TRect( TPoint(), 
	                              TSize( aRect.Width() + shadowTl.iWidth + shadowBr.iWidth,
	                                     aRect.Height() + shadowTl.iHeight + shadowBr.iHeight)
	                            );
	    rect.Move( TPoint(shadowTl.iWidth, shadowTl.iHeight) - rect.iTl );
        
	CFepUiBaseCtrl::SetRect( rect );

	    RootControl()->SetRect( rect );
    UiLayout()->SetShadowRect(shadowRect);
	    }
     /*   
    CFepUiBaseCtrl::SetRect(aRect);
    ResetShadowSizeIfNeeded();*/
    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::SetPopupProperty
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::SetPopupProperty()
	{
	TRect mainPaneRect = *((TRect*)iDataProvider->RequestData(EAknFepDataTypeMainPaneRect));

    TAknWindowLineLayout choiceWnd = 
                                AknLayoutScalable_Avkon::popup_choice_list_window(1).LayoutLine();
    
    TAknLayoutRect choiceRect;
    choiceRect.LayoutRect(mainPaneRect, choiceWnd);

    TRect itemRect = TRect(0,
                           0,
                           choiceRect.Rect().Width(),
                           AknLayoutScalable_Avkon::list_single_choice_list_pane(0).LayoutLine().iH);

    TAknTextLineLayout choiceTextLayout = 
                         AknLayoutScalable_Avkon::list_single_choice_list_pane_t1(1).LayoutLine();

    const CFont* font = AknLayoutUtils::FontFromId(choiceTextLayout.iFont, NULL);
    iPopupWnd->SetFont(font);
        
    iPopupWnd->SetItemRect(itemRect, 
                           itemRect);

    TInt leftMargin = choiceTextLayout.il;
   
    iPopupWnd->SetMarginAndBaseline(leftMargin,font->HeightInPixels());

    iPopupWnd->SetFontColor(AKN_LAF_COLOR_STATIC(choiceTextLayout.iC));

    if (iPopupWndBgImg)
    	{
    	AknIconUtils::SetSize(iPopupWndBgImg, TSize(itemRect.Width(),itemRect.Height()*3), 
                            EAspectRatioNotPreserved);
    	}
    
    if (iPopupWndFocusImg)
    	{
    	AknIconUtils::SetSize(iPopupWndFocusImg, TSize(itemRect.Width(),itemRect.Height()*3), 
                            EAspectRatioNotPreserved);
    	}
    
    iPopupSet = ETrue;	
    iPopupItemSize = itemRect.Size();
	}

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::SizeChanged
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::SizeChanged(TBool /*aLandscapeStyle*/)
    {
    SizeChangedForTitle();
    iClientArea->SizeChanged();

    Draw();
        }

void CAknFepVkbWindow::SizeChangedForTitle()
    	{
	TRect innerRect;
	
    if (( iLafMgr == NULL ) || 
        ( iLafMgr && !iLafMgr->IsValid() ))
        {
        return;
        }
        
    if (iDataProvider->SizeChanging() || (!iDataProvider->UnitShadowSizeSet()))
    	{
    	iDataProvider->ReadUnitShadowSize();
    	}

	TSize shadowTl = *((TSize*)iDataProvider->RequestData(EAknFepDataTypeShadowTlSize));
    TInt x = shadowTl.iWidth;
    TInt y = shadowTl.iHeight;
    
    if ( UiLayout()->PenInputType() == EPluginInputModeFSQ )
        {
        x = 0;
        y = 0;
        }

    
            
    TInt btnWidth = iDataProvider->RequestData(EAknFepDataTypeUnitWidth);
    TInt btnHeight = iDataProvider->RequestData(EAknFepDataTypeUnitHeight);    
    
    
    // Close btn
    TRect closeBtnRect;
    closeBtnRect.iTl = TPoint(x, y);
    closeBtnRect.SetSize(TSize(btnWidth, btnHeight));
    AknPenImageUtils::CalculateGraphicRect( closeBtnRect, innerRect );
    static_cast<CAknFepCtrlCommonButton*>
        ( Control( EPeninutWindowCtrlIdCloseBtn ) )->SizeChanged( closeBtnRect, innerRect, ETrue );


    // ICF
    TInt columNum = iDataProvider->RequestData(EAknFepDataTypeIcfColumnNums); 
    TInt titlebarHeight = iDataProvider->RequestData(EAknFepDataTypeTitleBarHeight); 
    	
    TRect icfRect;
    icfRect.iTl = TPoint(closeBtnRect.iBr.iX, closeBtnRect.iTl.iY);
    icfRect.SetSize(TSize(btnWidth * columNum + iClientArea->iCellAidGap, titlebarHeight));   

    // Set ICF Margin 
   	TPeninputCnICFConfig icfCfg;
   	iLafMgr->GetICFConfig( icfCfg );
    const CFont* icfFont = iLafMgr->ICFTextFont();
	iInputContextField->SetTextMargin( icfCfg.iLeftMargin,
									   icfCfg.iRightMargin,
									   icfCfg.iTopMargin,
									   0 );
    iInputContextField->SetLineSpace( icfCfg.iLineSpaceMargin );
    TRAP_IGNORE(iInputContextField->SizeChangedL( icfRect, 
	                                  icfCfg.iTextLineHeight,
	                                  icfFont->FontMaxHeight(),
	                                  icfFont ));    

    // Option Button
    TRect optionBtnRect;
    optionBtnRect.iTl = TPoint(closeBtnRect.iTl.iX, closeBtnRect.iBr.iY);
    optionBtnRect.SetSize(TSize(btnWidth, btnHeight));
    AknPenImageUtils::CalculateGraphicRect( optionBtnRect, innerRect );
    static_cast<CAknFepCtrlEventButton*>
        (Control(EPeninutWindowCtrlIdOptionBtn))->SizeChanged(optionBtnRect, innerRect, ETrue);
    
    TPoint backSpaceRefPoint;
    
    // Move Button
    TInt imLayout = iDataProvider->RequestData(EAknFepDataTypeIMLayout);
    
    if (UiLayout()->PenInputType() == EPluginInputModeVkb)
        {
        TRect moveBtnRect;
        if(EAknFepVkbImCnZhuyin == imLayout)
            {
            moveBtnRect.iTl = TPoint(icfRect.iBr.iX + btnWidth, icfRect.iTl.iY);
            moveBtnRect.SetSize(TSize(btnWidth, btnHeight));
            }
        else
            {
            moveBtnRect.iTl = TPoint(icfRect.iBr.iX, icfRect.iTl.iY);
            moveBtnRect.SetSize(TSize(btnWidth * 2, btnHeight));            
            }
        AknPenImageUtils::CalculateGraphicRect( moveBtnRect, innerRect );
        static_cast<CAknFepCtrlDragButton*>
            ( Control( EPeninutWindowCtrlIdMoveBtn ) )->SizeChanged(moveBtnRect, innerRect, ETrue);
            
        backSpaceRefPoint = TPoint(moveBtnRect.iTl.iX, moveBtnRect.iBr.iY);
        }
    else if (UiLayout()->PenInputType() == EPluginInputModeFSQ)
        {
        if ( EAknFepVkbImCnZhuyin == imLayout )
            {
            /*TRect emptyRect= TRect();
            AknPenImageUtils::CalculateGraphicRect( emptyRect, innerRect );
            static_cast<CAknFepCtrlRepeatButton*>
                ( Control( EPeninupWindowCtrlIdArrowLeftBtn ) )->SizeChanged(emptyRect, innerRect, ETrue);
            static_cast<CAknFepCtrlRepeatButton*>
                ( Control( EPeninupWindowCtrlIdArrowRightBtn ) )->SizeChanged(emptyRect, innerRect, ETrue);*/
           /* TRect arrowRect;
            TRect innerRect;
            
            arrowRect.iTl = TPoint(closeBtnRect.iBr.iX, closeBtnRect.iBr.iY);
            arrowRect.SetSize(TSize(btnWidth + btnWidth*2, btnHeight));
            AknPenImageUtils::CalculateGraphicRect( arrowRect, innerRect );
            static_cast<CAknFepCtrlRepeatButton*>
                ( Control( EPeninupWindowCtrlIdArrowUpBtn ) )->SizeChanged(arrowRect, innerRect, ETrue);
                
            arrowRect.iTl = TPoint(arrowRect.iBr.iX, arrowRect.iTl.iY);
            arrowRect.SetSize(TSize(btnWidth + btnWidth*2, btnHeight));
            AknPenImageUtils::CalculateGraphicRect( arrowRect, innerRect );
            static_cast<CAknFepCtrlRepeatButton*>
                ( Control( EPeninupWindowCtrlIdArrowDownBtn ) )->SizeChanged(arrowRect, innerRect, ETrue);
                
            arrowRect.iTl = TPoint(arrowRect.iBr.iX, arrowRect.iTl.iY);
            arrowRect.SetSize(TSize(btnWidth + btnWidth*2, btnHeight));
            AknPenImageUtils::CalculateGraphicRect( arrowRect, innerRect );
            static_cast<CAknFepCtrlRepeatButton*>
                ( Control( EPeninupWindowCtrlIdArrowLeftBtn ) )->SizeChanged(arrowRect, innerRect, ETrue);
                
            arrowRect.iTl = TPoint(arrowRect.iBr.iX, arrowRect.iTl.iY);
            arrowRect.SetSize(TSize(btnWidth + btnWidth*2, btnHeight));
            AknPenImageUtils::CalculateGraphicRect( arrowRect, innerRect );
            static_cast<CAknFepCtrlRepeatButton*>
                ( Control( EPeninupWindowCtrlIdArrowRightBtn ) )->SizeChanged(arrowRect, innerRect, ETrue);*/
            }
        else
            {
            TRect leftBtnRect;
            leftBtnRect.iTl = TPoint(icfRect.iBr.iX, icfRect.iTl.iY);
            
            leftBtnRect.SetSize(TSize(btnWidth, btnHeight));        
			AknPenImageUtils::CalculateGraphicRect( leftBtnRect, innerRect );
            iArrowLeftButton->SizeChanged(leftBtnRect, innerRect, ETrue);

            TRect rightBtnRect;
            rightBtnRect.iTl = TPoint(leftBtnRect.iBr.iX, leftBtnRect.iTl.iY);
            rightBtnRect.SetSize(TSize(btnWidth, btnHeight));        
			AknPenImageUtils::CalculateGraphicRect( rightBtnRect, innerRect );
            iArrowRightButton->SizeChanged(rightBtnRect, innerRect, ETrue);

            backSpaceRefPoint = TPoint(leftBtnRect.iTl.iX, leftBtnRect.iBr.iY);
            }
        }

    // BackSpace button
    TRect backSpaceBtnRect;
    if(EAknFepVkbImCnZhuyin == imLayout)
        {
        if(UiLayout()->PenInputType() == EPluginInputModeFSQ)
            {
            backSpaceBtnRect.iTl = TPoint(icfRect.iBr.iX, icfRect.iTl.iY);
            backSpaceBtnRect.SetSize(TSize(btnWidth * 2, btnHeight));
            }
        else
            {
            backSpaceBtnRect.iTl = TPoint(icfRect.iBr.iX, icfRect.iTl.iY);
            backSpaceBtnRect.SetSize(TSize(btnWidth, btnHeight));
            }
        }
    else
        {
        backSpaceBtnRect.iTl = backSpaceRefPoint;
        backSpaceBtnRect.SetSize(TSize(btnWidth * 2, btnHeight));
        }
    AknPenImageUtils::CalculateGraphicRect( backSpaceBtnRect, innerRect );
    static_cast<CAknFepCtrlRepeatButton*>
        ( Control( EPeninutWindowCtrlIdBackspaceBtn ) )->SizeChanged(backSpaceBtnRect, 
                                                                     innerRect, ETrue);

    // Tooltip and candidate list
	SetITILayoutOnFSQ();

    if (iDataProvider->SizeChanging() || (!iPopupSet))
    	{
    	SetPopupProperty();
    	}
    
        }


    
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::HandleControlEvent
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::HandleControlEvent(TInt aEventType,
                                          CFepUiBaseCtrl* aCtrl, 
                                          const TDesC& aEventData)
    {
    //Call this function in base class
    CAknFepCtrlBaseWindowExt::HandleControlEvent(aEventType,aCtrl,aEventData);
    
    TUint16* data = (TUint16*) aEventData.Ptr();
    
    TBool dealFlag = EFalse;
    
    switch (aEventType)
        {
        case EEventControlFocusGained:
            //Get composition
            {
            CFepLayoutEditAreaBase* compField = ClientArea()->CompositionField();
            
            //if the event is sent by pen down in edit area
            if ( ( aCtrl ) && ( aCtrl->ControlId() == EPeninputWindowCtrlIdMultiLineICF ) )
                {
                //Notify compField lost focus
                if ( compField )
                    {
                    compField->SetFocus(EFalse);
                    }                
                }
            else if ( ( aCtrl ) && ( aCtrl->ControlId() == EAknFepVkbCtrlIdCompositionField ) )
                {
                //Notify iInputContextField lost focus
                if ( iInputContextField ) 
                    {
                    iInputContextField->SetFocus(EFalse);
                    }
                }
            
            //if the event is sent from by internal
            if ( *data == EInputContextFieldGetFocus )
                {
                if ( compField )
                    {
                    compField->SetFocus(EFalse);
                    }  
                if ( iInputContextField ) 
                    {
                    iInputContextField->SetFocus(ETrue);
                    }
                }                                    
            else if ( *data == ECompotisionFieldGetFocus )
                {
                if ( iInputContextField ) 
                    {
                    iInputContextField->SetFocus(EFalse);
                    }
                if ( compField )
                    {
                    compField->SetFocus(ETrue);
                    }                                
                }                
                
            dealFlag = ETrue;
            }
            break;
        case EVkbEventUpArrow:
            {
            TRAP_IGNORE(iInputContextField->HandleArrowBtnEventL(
                                                        CFepLayoutMultiLineIcf::EArrowUp));
            }
            break;
        case EVkbEventDownArrow:
            {
            TRAP_IGNORE(iInputContextField->HandleArrowBtnEventL(
                                                        CFepLayoutMultiLineIcf::EArrowDown));
            }
            break;                     
        default:
            break;
        }
    
    if ( !dealFlag )
        {
        iClientArea->HandleControlEvent(aEventType,aCtrl,aEventData);
        }
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::DoClose
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CAknFepVkbWindow::DoClose()
    {
    //Send EVkbEventWindowClose to layout
    this->ReportEvent(EVkbEventWindowClose,KNullDesC);
    
    //Return true which will permit the window close
    return EFalse;
    }
            
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::ClientArea
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CAknFepVkbClientArea* CAknFepVkbWindow::ClientArea()
    {
    return iClientArea;
    }       

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::InputContextField
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepLayoutMultiLineIcf* CAknFepVkbWindow::InputContextField()
    {
    return iInputContextField;
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::CompositionFieldStringLength
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbWindow::CompositionFieldStringLength()
    {
    return iClientArea->CompositionFieldStringLength();
    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::FocusedControlID
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbWindow::FocusedControlID()
    {
    TInt ctrlID = iInputContextField->ControlId();
    CFepLayoutEditAreaBase* compField = ClientArea()->CompositionField();
    
    if ( compField && compField->IsFocused() )
        {
        return compField->ControlId();
        }
        
    return ctrlID;
    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::SetEditorTextL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::SetEditorTextL(const TFepInputContextFieldData& aData)
    {
    if(iInputContextField)
        {
        iInputContextField->SetTextL(aData);
        }
    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::SetEditorTextL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::SetTextIsSecret( TBool aIsSecret )
    {
    if(iInputContextField)
        {
        iInputContextField->SetTextIsSecret( aIsSecret );
        }
    }
            
    
    
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::BackgroundConstructL
// Do background construct.
// ---------------------------------------------------------------------------
//
TInt CAknFepVkbWindow::BackgroundTaskL(TAny* aPtr)
    {
    CAknFepVkbWindow* self = static_cast<CAknFepVkbWindow*>(aPtr);
    self->DoIdleConstructL();
    return EFalse;
    }
    
void CAknFepVkbWindow::DoIdleConstructL()
    {
    if (iPopupInited)
        {
   	    return;
        }
    
    //set font   
    /*
    TAknWindowLineLayout appWnd = AknLayoutScalable_Avkon::application_window(0).LayoutLine();
    
    TAknWindowLineLayout mainPane = AknLayoutScalable_Avkon::main_pane(4).LayoutLine();
        
    TAknLayoutRect mainPaneRect;
    mainPaneRect.LayoutRect(appWnd.Rect(), mainPane);
    
    TAknWindowLineLayout choiceWnd = AknLayoutScalable_Avkon::popup_choice_list_window(1).LayoutLine();
    
    TAknLayoutRect choiceRect;
    choiceRect.LayoutRect(mainPaneRect.Rect(), choiceWnd);
    

    TRect itemRect = TRect(0,
                           0,
                           choiceRect.Rect().Width(),
                           AknLayoutScalable_Avkon::list_single_choice_list_pane(0).LayoutLine().iH);

    TAknTextLineLayout choiceTextLayout = AknLayoutScalable_Avkon::list_single_choice_list_pane_t1(1).LayoutLine();

    const CFont* font = AknLayoutUtils::FontFromId(choiceTextLayout.iFont, NULL);
    iPopupWnd->SetFont(font);
    iPopupWnd->SetItemRect(itemRect, 
                           itemRect);

    TInt leftMargin = choiceTextLayout.il;
   
    iPopupWnd->SetMarginAndBaseline(leftMargin,font->HeightInPixels());

    iPopupWnd->SetFontColor(AKN_LAF_COLOR_STATIC(choiceTextLayout.iC));
    */
    if (iDataProvider->SizeChanging() || (!iPopupSet))
    	{
        SetPopupProperty();
    	}

    // Read resource
    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, R_AKN_FEP_VKB_POPUP_WINDOW); 
    
    GetPopupWndInfoFromRes(reader);
      
    CleanupStack::PopAndDestroy(); // reader

    //add item
    
    CFepLayoutChoiceList::SItem item;

    item.iCommand = EAknFepVkbVkbAccented1;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_VKB_ACCENT1 );
    TPtrC p1 = reader.ReadTPtrC();
    item.iText.Copy( p1 );
    iPopupWnd->AddItemL( item );
    CleanupStack::PopAndDestroy( 1 );
    item.iCommand = EAknFepVkbVkbAccented2;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_VKB_ACCENT2 );
    TPtrC p2 = reader.ReadTPtrC();
    item.iText.Copy( p2 );
    iPopupWnd->AddItemL( item );
    CleanupStack::PopAndDestroy( 1 );
    item.iCommand = EAknFepVkbVkbAccented3;
    CCoeEnv::Static()->CreateResourceReaderLC( reader, R_PENINPUT_VKB_ACCENT3 );
    TPtrC p3 = reader.ReadTPtrC();
    item.iText.Copy( p3 );
    iPopupWnd->AddItemL( item );
    CleanupStack::PopAndDestroy( 1 );  
    iPopupInited = ETrue;
    
    iPopupWnd->SetListColumnNumWithLafL( iPopupWnd->ItemsCount() );
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::AddPopupWindowL
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::AddPopupWindowL()
    {
    iPopupWnd = CPeninputCommonChoiceList::NewL(UiLayout(),
                                           EAknFepVkbPopupWindow,
                                			KAknsIIDQsnFrList,
                                			KAknsIIDQsnFrPopupSub );
    
    iPopupWnd->AddEventObserver(UiLayout());
    AddControlL(iPopupWnd);

    
    iIdle = CIdle::NewL(CActive::EPriorityIdle);
    iIdle ->Start(TCallBack(BackgroundTaskL,this));
    }

void  CAknFepVkbWindow::SetSwitchlistLanguage( TInt /*aLang*/ )
    {
    }
    
void  CAknFepVkbWindow::SetSwitchlistSecretFlag(TBool aSecret)
    {
    static_cast<CPeninputLayoutInputmodelChoice*>
        (Control( EPeninutWindowCtrlIdSwitcherPopupWindow))->SetSecretEditor(aSecret);
    }
// ---------------------------------------------------------------------------
// CPeninputGenericVkbWindow::DimArrowKeys
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::DimArrowKeys( TBool aDimArrow )
    {
    //disable or enable arrow keys of FSQ in secret editor. 
    if(UiLayout()->PenInputType() == EPluginInputModeFSQ )
        {
        iArrowLeftButton->SetDimmed( aDimArrow );
        iArrowRightButton->SetDimmed( aDimArrow );
        iClientArea->DimArrowKeys( aDimArrow );
        }	    	
    }
    
// --------------------------------------------------------------------------
// CAknFepVkbWindow::GetPopupWndInfoFromRes
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
void CAknFepVkbWindow::GetPopupWndInfoFromRes(TResourceReader aReader)
    {
    TPtrC bmpFileName = aReader.ReadTPtrC();
    TInt32 imgMajorSkinId = aReader.ReadInt32();

    TAknsItemID id;
    
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();

    TInt popWinBmpId = aReader.ReadInt16();
    TInt popWinBmpMaskId = aReader.ReadInt16();
    TInt popWinBmpSkinId = aReader.ReadInt16();
    
    id.Set( TInt( imgMajorSkinId ), popWinBmpSkinId );
    
    if ( popWinBmpId != KInvalidImg )
    	{
        TRAP_IGNORE(AknsUtils::CreateIconL( skininstance,
                                id,
                                iPopupWndBgImg,
                                bmpFileName,
                                popWinBmpId ));
            
        AknIconUtils::SetSize(iPopupWndBgImg, iPopupItemSize, EAspectRatioNotPreserved); 
        TRAP_IGNORE(iPopupWnd->SetBackgroundBitmapL( iPopupWndBgImg ));
    	}
    	
    TInt focusBmpId = aReader.ReadInt16();
    TInt focusBmpMaskId = aReader.ReadInt16();
    TInt focusBmpSkinId = aReader.ReadInt16();

    id.Set( TInt( imgMajorSkinId ), focusBmpSkinId );
    
    if ( focusBmpId != KInvalidImg )
    	{
    	TRAP_IGNORE(AknsUtils::CreateIconL( skininstance,
                                id,
                                iPopupWndFocusImg,
                                bmpFileName,
                                focusBmpId ));
                                
        AknIconUtils::SetSize(iPopupWndFocusImg, iPopupItemSize, EAspectRatioNotPreserved);  

        iPopupWnd->SetFocusBmp( iPopupWndFocusImg );
    	}
    }
      
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::PopupAccentWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::PopupAccentWindow()
    {
    CFepUiBaseCtrl* accentBtn = Control(EAknFepVkbCtrlIdAccentedButton);
    if ( accentBtn )
        {
        if (!iPopupInited)
            {
            TRAP_IGNORE(DoIdleConstructL());
            }
        
        TRect rect = accentBtn->Rect();   
        iPopupWnd->Display(rect);
        iAccentPopup = ETrue;                 
        }         
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::PopupAccentWindow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::PopupSwitchWindow()
    {
    CFepUiBaseCtrl* accentBtn = Control(EAknFepVkbCtrlIdSwitchToHwrBtn);
    if ( accentBtn )
        {
        if (!iPopupInited)
            {
            TRAP_IGNORE(DoIdleConstructL());
            }
        
        iAccentPopup = ETrue;                 
        
       	TRAP_IGNORE( static_cast<CPeninputLayoutInputmodelChoice*>
		    (Control(EPeninutWindowCtrlIdSwitcherPopupWindow))->PopUpSwitchListL(accentBtn->Rect()) );
        }         
    }    
// CAknFepVkbWindow::ResetAccentButton
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepVkbWindow::ResetAccentButton()
    {
    CFepUiBaseCtrl* accentBtn = Control(EAknFepVkbCtrlIdAccentedButton);
    if ( accentBtn )
        {
        accentBtn->SetActive(EFalse);
        
        TRect innerRect;
        CAknFepCtrlCommonButton* accentBtnCtrl = static_cast<CAknFepCtrlCommonButton*> (accentBtn);
        AknPenImageUtils::CalculateGraphicRect( accentBtn->Rect(), innerRect );
        accentBtnCtrl->SizeChanged(accentBtn->Rect(), innerRect, EFalse);                    
        iAccentPopup = EFalse;

        iClientArea->ActiveCurrentRange();
        }          
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::NeedCreateWord
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TBool CAknFepVkbWindow::NeedCreateWord()
	{
	return iClientArea->NeedCreateWord();
	}

void CAknFepVkbWindow::CloseAccentWindow()
    {
    if(iAccentPopup)
        {
        iPopupWnd->CloseWindow();
        ResetAccentButton();
        }
    }
    
// ---------------------------------------------------------------------------
// CAknFepVkbWindow::GetCreatedWordSpell
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
TPtrC CAknFepVkbWindow::GetCreatedWordSpell()
    {
    return iClientArea->GetCreatedWordSpell();
    }

// ---------------------------------------------------------------------------
// CAknFepVkbWindow::SetEnableSettingBtn
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
void CAknFepVkbWindow::SetEnableSettingBtn(TBool aEnable)
    {
    CAknFepCtrlEventButton* optionBtn = static_cast<CAknFepCtrlEventButton*>
        (Control( EPeninutWindowCtrlIdOptionBtn ) );    
    if ( aEnable )    
        {
        optionBtn->SetDimmed(EFalse);
        }
    else
        {
        optionBtn->SetDimmed(ETrue);
        optionBtn->SetActive(EFalse);
        } 
    } 
// --------------------------------------------------------------------------
// CAknFepVkbWindow::GetWindowResId
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TInt CAknFepVkbWindow::GetWindowResId()
    {
    if ( UiLayout()->PenInputType() == EPluginInputModeFSQ )
        {
        return R_AKN_FEP_VKB_FSQ_WINDOW_INFO;
        }
    return R_AKN_FEP_VKB_WINDOW_INFO;        
    }

// --------------------------------------------------------------------------
// CAknFepVkbWindow::CreateCustomControlL
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
TBool CAknFepVkbWindow::CreateCustomControlL( TInt16 aControlId, TInt32 aImageId )
	{
	TBool create = ETrue;
    switch ( aControlId )
        {
        case EPeninutWindowCtrlIdMoveBtn:
            {
            if ( !iMoveButton )
                {
                iMoveButton = CAknFepCtrlDragButton::NewL(UiLayout(), EPeninutWindowCtrlIdMoveBtn);
                iMoveButton->SetImageFrameId( KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive ); 
                AddControlL(iMoveButton);
                iMoveButton->AddPositionObserver(this);
                iMoveButton->AddEventObserver( UiLayout() );
                iMoveButton->SetResourceId( aImageId );
                UiLayout()->EnableLayoutMoving( static_cast<CDragBar*>( iMoveButton ) );
                }
            iMoveButton->ConstructFromResourceL();
            }
            break;
        case EPeninupWindowCtrlIdArrowLeftBtn:
            {
            if ( UiLayout()->PenInputType() != EPluginInputModeFSQ )
                {
                break;
                }

            if (!iArrowLeftButton)
                {
	            iArrowLeftButton = CAknFepCtrlRepeatButton::NewL( 
	                UiLayout(), aControlId, EVkbEventLeftArrow, EKeyLeftArrow, 
	            								  KAknsIIDQsnFrFunctionButtonNormal,
	        									  KAknsIIDQsnFrFunctionButtonPressed,
	        									  KAknsIIDQsnFrFunctionButtonInactive );
                 AddControlL(iArrowLeftButton);
                iArrowLeftButton->AddEventObserver(UiLayout());
                iArrowLeftButton->SetResourceId( aImageId );
                }

            iArrowLeftButton->ConstructFromResourceL();
            }
            break;
        case EPeninupWindowCtrlIdArrowRightBtn:
            {
            if ( UiLayout()->PenInputType() != EPluginInputModeFSQ )
                {
                break;
                }

            if (!iArrowRightButton)
                {
	            iArrowRightButton = CAknFepCtrlRepeatButton::NewL( 
	                UiLayout(), aControlId, EVkbEventRightArrow, EKeyRightArrow, 
	            								  KAknsIIDQsnFrFunctionButtonNormal,
	        									  KAknsIIDQsnFrFunctionButtonPressed,
	        									  KAknsIIDQsnFrFunctionButtonInactive );                
                AddControlL(iArrowRightButton);
                iArrowRightButton->AddEventObserver(UiLayout());
                iArrowRightButton->SetResourceId( aImageId );
                }
            iArrowRightButton->ConstructFromResourceL();
            }
            break;
        default:
            {
            create = EFalse;
            }
        }
        return create;
    }
 
void CAknFepVkbWindow::AddCustomControlGroupL(CFepUiBaseCtrl* aCtrl)
	{
	AddControlL(aCtrl);
	}
	
void CAknFepVkbWindow::SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType)
    {
    TRawEvent event; 
    event.Set(aType,aScanCode);
    
    TPtrC eventPtr(reinterpret_cast<const TUint16*>(&event),sizeof(event)/sizeof(TUint16));

    UiLayout()->SignalOwner(ESignalSimulateEvent,eventPtr);
    }	
	
void CAknFepVkbWindow::SetPromptTextL(TUint8* aData)
    {
    RDesReadStream readStream;

    TPtr8 countPtr(aData, 2*sizeof(TInt), 2*sizeof(TInt));
	  readStream.Open(countPtr);
	  CleanupClosePushL(readStream);
    const TInt dataCount = readStream.ReadInt32L();
	  const TInt textCount = readStream.ReadInt32L();
    CleanupStack::PopAndDestroy(&readStream);
    
    TPtr8 ptr( aData+2*sizeof(TInt), dataCount+textCount, dataCount+textCount );            
	  readStream.Open(ptr);
	  CleanupClosePushL(readStream);
	
	  HBufC8* dataBuf = HBufC8::NewLC(dataCount);
	  TPtr8 dataBufPtr = dataBuf->Des();
	  readStream.ReadL(dataBufPtr, dataCount);

    TFepPromptText* pIcfData = 
        reinterpret_cast<TFepPromptText*>(const_cast<TUint8*>(dataBufPtr.Ptr()));

    HBufC* textBuf;
    if (textCount > 0)
        {
        textBuf = HBufC::NewLC(textCount/2);
    	  TPtr textBufPtr = textBuf->Des();
    	  readStream.ReadL(textBufPtr, textCount/2);
        static_cast<CFepLayoutMultiLineIcf*>
            (Control(EPeninputWindowCtrlIdMultiLineICF))->SetPromptTextL
                (textBuf->Des(), pIcfData->iCleanContent);
        CleanupStack::PopAndDestroy(textBuf);
        }
    else
        {
        static_cast<CFepLayoutMultiLineIcf*>(Control(EPeninputWindowCtrlIdMultiLineICF))->
            SetPromptTextL( KNullDesC, pIcfData->iCleanContent );
        }

    CleanupStack::PopAndDestroy(dataBuf);
	CleanupStack::PopAndDestroy(&readStream);
    }    

void CAknFepVkbWindow::ChangeMergedButtonStatus( 
											const TInt aIsShiftCase, 
    								 		const TInt aIsCapslockCase )
    {
    iClientArea->ChangeMergedButtonStatus(aIsShiftCase, aIsCapslockCase);
    }
    
void CAknFepVkbWindow::ShowBubble(TInt aShow)
    {
    if (aShow > 0)
        {
        iClientArea->ShowBubble(ETrue);    
        }
    else
        {
        iClientArea->ShowBubble(EFalse);     
        }
    }
    
void CAknFepVkbWindow::SetTextAlignmentL( TInt aAlignment )	
	{
	TInt currentLanguage = iDataProvider->RequestData(EAknFepDataTypeInputLanguage);
	
	if ( iInputContextField && currentLanguage != ELangNone )
		{
		iInputContextField->SetTextAlignmentL( aAlignment, currentLanguage );
		}
	}  
	
void CAknFepVkbWindow::UpdateICFTextL()
    {
    if( iInputContextField )
        {
        iInputContextField->UpdateSecretTextL();
        }
    }

// --------------------------------------------------------------------------
// Show tooltip of ICF
// --------------------------------------------------------------------------
//
void CAknFepVkbWindow::ShowTooltipOnFSQL( const TDesC& aText )
    {
    if ( UiLayout()->PenInputType() != EPluginInputModeFSQ )
        {
        return;
        }    
        
    // Calculate tooltip width
    const TPeninputCnTooltipBoxLayoutData& tooltipBoxLAF 
                                      = iLafMgr->TooltipBoxLayoutData();
    TRect tooltipRect = tooltipBoxLAF.iRect;
    // Calculate inner rect
    TRect innerRect = tooltipRect;
    innerRect.iTl += TPoint( KTooltipLeftMargin, KTooltipTopMargin );
    innerRect.iBr -= TPoint( KTooltipRightMargin, KTooltipBottomMargin );          
    const CFont* font = tooltipBoxLAF.iFont;
    // Set inner width in terms of text width
    innerRect.SetWidth( font->TextWidthInPixels( aText ) );    
    tooltipRect.iTl 
           = innerRect.iTl - TPoint( KTooltipLeftMargin, KTooltipTopMargin );
    tooltipRect.iBr 
           = innerRect.iBr + TPoint( KTooltipRightMargin, KTooltipBottomMargin );
    // Set tooltip rect in terms of inner rect
    iInputContextField->ShowTooltipL( aText, tooltipRect );    
    }
	
// --------------------------------------------------------------------------
// Hide tooltip of ICF
// --------------------------------------------------------------------------
//
void CAknFepVkbWindow::HideTooltipOnFSQ()
    {
    if ( UiLayout()->PenInputType() != EPluginInputModeFSQ )
        {
        return;
        }
    iInputContextField->HideTooltip();
    }

// --------------------------------------------------------------------------
// Hide candidate list
// --------------------------------------------------------------------------
//
void CAknFepVkbWindow::HideCandidateListOnFSQ()
    {
    if ( UiLayout()->PenInputType() != EPluginInputModeFSQ )
        {
        return;
        }
    iCandidateList->Hide( ETrue );
    }
	
// --------------------------------------------------------------------------
// Show candidate list
// --------------------------------------------------------------------------
//
void CAknFepVkbWindow::ShowCandidateListOnFSQL( const CDesCArray* aItemArray,
                                                   TInt aActiveIndex )
    {
    if ( UiLayout()->PenInputType() != EPluginInputModeFSQ )
        {
        return;
        }                
    iCandidateList->ClearItemsL();
    if ( aItemArray )
        {        
        for ( TInt i = 0; i < aItemArray->Count(); i++ )
            {
            CFepLayoutChoiceList::SItem item;
            item.iCommand = 0;
            if ( (*aItemArray)[i].Length() <= item.iText.MaxLength() )
                {
                item.iText.Copy( (*aItemArray)[i] );
                }
            else
                {
                // Given longer than maxlength, display the part of less than max
                item.iText.Copy( (*aItemArray)[i].Left( item.iText.MaxLength() ) );
                }
            iCandidateList->AddItemL( item );
            }
        
			iCandidateList->SetCurrentFocusedIndex( aActiveIndex );
        }
    
    CFepLayoutMultiLineIcf* icf = static_cast<CFepLayoutMultiLineIcf*>
         (Control(EPeninputWindowCtrlIdMultiLineICF)); 

    TPoint posBelow( 0, 0 );
    TPoint posRight( 0, 0 );
    posBelow = posRight = icf->Rect().iTl;
    icf->InlineTextPos( posBelow, posRight );
    TPoint tlPos = posRight;
    iCandidateList->Display( TRect( tlPos, tlPos ), 
                             CFepLayoutPopupWnd::EDispBottomRight );
    }

// --------------------------------------------------------------------------
// Handle candidate list selected events.
// --------------------------------------------------------------------------
//
void CAknFepVkbWindow::HandleCandidateListSelectedOnFSQL
                         ( CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    if ( UiLayout()->PenInputType() != EPluginInputModeFSQ )
        {
        return;
        }

    CFepLayoutChoiceList::SEvent* event = 
            (CFepLayoutChoiceList::SEvent*)aEventData.Ptr();
    if ( event->iIndex != KErrNotFound )
        {
        TPtrC ptr;
        ptr.Set( reinterpret_cast<TUint16*>(&( event->iIndex ) ), 
                 sizeof( TInt ) );            
        UiLayout()->SignalOwner( ESignalSelectCandidate, ptr );
        }
    }

// --------------------------------------------------------------------------
// Create a candidate list control.
// --------------------------------------------------------------------------
//    
void CAknFepVkbWindow::CreateCandidateListOnFSQL()
    {
    if ( UiLayout()->PenInputType() != EPluginInputModeFSQ )
        {
        return;
        }
    
    iCandidateList = CFepLayoutScrollableList::NewL( 
                            UiLayout(), 
                            EPentinputHwrCandidateList,
                            TSize( 0, 0 ),
                            0,
                            0,
                            TSize( 0, 0 ),
                            TSize( 0, 0 ) );
         
    iCandidateList->SetImgFrId(KAknsIIDQsnFrPopupSub, 
                                KAknsIIDQsnFrFunctionButtonNormal,
                                KAknsIIDQsnFrFunctionButtonPressed,
                                KAknsIIDQsnFrFunctionButtonInactive,
                                KAknsIIDQsnFrList,
                                KAknsIIDQsnFrPopupSub);
                                
    iCandidateList->SetResourceId(R_PENINPUT_FSQ_CANDIDATE_LIST);
    iCandidateList->ConstructFromResourceL();
    iCandidateList->SetPageInfoResID( R_PENINPUT_FSQ_CANDIDATE_LIST_PAGE_NUM );
    iCandidateList->AddEventObserver( UiLayout() );
    AddControlL(iCandidateList);    
    }

// --------------------------------------------------------------------------
// Retrieve text color from skin for candidiate list.
// --------------------------------------------------------------------------
//
const TRgb CAknFepVkbWindow::CandidateListTextColorOnFSQ()
    {
    TRgb matchlistcolor = KRgbBlack;
    if ( UiLayout()->PenInputType() == EPluginInputModeFSQ )
        {
        MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
        AknsUtils::GetCachedColor( skininstance, 
                                    matchlistcolor, 
                                    KAknsIIDQsnTextColors,
                                    EAknsCIQsnTextColorsCG43 );
        }
    
    return matchlistcolor;
    }

// --------------------------------------------------------------------------
// Retrieve text color from skin for autocompletion part.
// --------------------------------------------------------------------------
//
const TRgb CAknFepVkbWindow::AutoCompletionPartColorOnFSQ()
    {
    TRgb matchlistcolor = KRgbBlack;
    if ( UiLayout()->PenInputType() == EPluginInputModeFSQ )
        {
        MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
        AknsUtils::GetCachedColor( skininstance, 
                                    matchlistcolor, 
                                    KAknsIIDQsnTextColors,
                                    EAknsCIQsnTextColorsCG64 );
        }
    
    return matchlistcolor;
    }
// --------------------------------------------------------------------------
// Set property for tooltip and candidate list.
// --------------------------------------------------------------------------
//
void CAknFepVkbWindow::SetITILayoutOnFSQ()
	{
    if ( UiLayout()->PenInputType() != EPluginInputModeFSQ 
    		|| !iLafMgr || !iLafMgr->IsValid() )
        {
        return;
        }
    
    // Addition for ITI features, read LAF data for candidate list
    const TPeninputCnCandidateListLayoutData& candidateListLAF =    
                                        iLafMgr->CandidateListLayoutData();
    iCandidateList->SizeChanged( candidateListLAF.iSelectionItemSize, 
                                 candidateListLAF.iSelectionHorMargin,
                                 candidateListLAF.iSelectionVerMargin,
                                 candidateListLAF.iSelectionNaviSize,
                                 candidateListLAF.iSelectionNaviInnerSize );    
    iCandidateList->SetPageInfoTextLineLayout
                                ( candidateListLAF.iSelectionPageTextLine );
    
    
    const CFont* itemFont = candidateListLAF.iFont;    
    TRgb textColor = CandidateListTextColorOnFSQ();
    if ( itemFont )
        {
        // Set font defined in LAF
        iCandidateList->SetTextProperty( itemFont, textColor );
        }
    else
        {
        // If font in LAF is NULL, set ICF text font to candidate list
        iCandidateList->SetTextProperty( iLafMgr->ICFTextFont(), textColor );
        }
    
    if ( iInputContextField )
        {
        // read LAF data for tooltip box
        const TPeninputCnTooltipBoxLayoutData& tooltipBoxLAF 
                                        = iLafMgr->TooltipBoxLayoutData();
        iInputContextField->TooltipBubbleCtrl()->SetRect( 
                                        tooltipBoxLAF.iRect );
        iInputContextField->TooltipBubbleCtrl()->SetTextFormat( 
                                        tooltipBoxLAF.iTooltipTextLayout );
        iInputContextField->TooltipBubbleCtrl()->SetTextColorIndex( 
                                        EAknsCIQsnTextColorsCG55 );
        iInputContextField->TooltipBubbleCtrl()->SetBitmapParam(NULL,
                                        NULL,
                                        KAknsIIDQsnFrInputPreviewSideL,
                                        KAknsIIDQsnFrInputPreviewMiddle,
                                        KAknsIIDQsnFrInputPreviewSideR);        
        iInputContextField->TooltipBubbleCtrl()->SetFrameDiff( KTooltipLeftMargin, 
        		                                               KTooltipTopMargin,
        		                                               KTooltipRightMargin,
        		                                               KTooltipBottomMargin );        
        // read color from skin for autocompletion part
        iInputContextField->SetAutoCompleteTextColor( 
                                        AutoCompletionPartColorOnFSQ() );        
        }      
    }
// End Of File
