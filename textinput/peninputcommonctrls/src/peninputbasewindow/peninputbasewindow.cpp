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
* Description:  CAknFepCtrlBaseWindow
*
*/


//  INCLUDES
#include <peninputbutton.h>
#include <peninputdragbutton.h>
#include <peninputbasewindow.h>
#include <peninputcommonbutton.h>
#include <AknIconUtils.h>
// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlBaseWindow::CAknFepCtrlBaseWindow(CFepUiLayout* aFepUiLayout, 
                                          TInt aControlId)
    :CControlGroup(aFepUiLayout,aControlId)
    {
    }

// ---------------------------------------------------------
// Constructor
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlBaseWindow::BaseConstructL()
    {
    CControlGroup::ConstructL();
    
    iTitleBar = CControlGroup::NewL(UiLayout(), 0, EFalse);
    iClientPane = CControlGroup::NewL(UiLayout(), 0, EFalse);
     
    iMoveButton = CAknFepCtrlDragButton::NewL(UiLayout(), 0);
    
    iMoveButton->SetImageFrameId(KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive);       
    
    iCloseButton = CAknFepCtrlCommonButton::NewL(UiLayout(), 0, 
            								  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive);
    iHeaderPane = CControlGroup::NewL(UiLayout(), 0, EFalse);

    iTitleBar->AddControlL(iMoveButton);
    iTitleBar->AddControlL(iHeaderPane);
    iTitleBar->AddControlL(iCloseButton);
    iCloseButton->AddEventObserver(this);
    iMoveButton->AddPositionObserver(this);
    
    AddControlL(iTitleBar);
    AddControlL(iClientPane);
    }
    
// ---------------------------------------------------------
// Destructor
// ---------------------------------------------------------
//
EXPORT_C CAknFepCtrlBaseWindow::~CAknFepCtrlBaseWindow()
    {
    }

// ---------------------------------------------------------
// Draw window frame and sub controls
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlBaseWindow::Draw()
    {
	if(!AbleToDraw())
        return;

    //Clear();
    DrawOpaqueMaskBackground( Rect() );
    CFbsBitGc* gc= static_cast<CFbsBitGc*> ( BitGc());
/*
    gc->Activate( MaskBitmapDevice() );
    gc->SetBrushStyle( CGraphicsContext::ENullBrush );
    gc->SetBrushColor( MaskBkCol() );
    gc->SetPenSize( PenSize());
    gc->SetPenColor( PenColor() );

    gc->DrawRect( Rect());
    TRect rect = iTitleBar->Rect();
    gc->DrawRect(rect);
*/
    // ----- draw bitmaps -----
    gc->Activate( BitmapDevice() ); 
    gc->SetBrushColor( TRgb(KOpaqueColor) );
    gc->SetPenColor( TRgb(KOpaqueColor) );
    gc->DrawRect( Rect());
    if( BackgroundBmp() && BackgroundBmp()->SizeInPixels() != Rect().Size() )    
    	AknIconUtils::SetSize(BackgroundBmp(), Rect().Size(), EAspectRatioNotPreserved);
    
	DrawBackground();
	CControlGroup::Draw();
    }

// ---------------------------------------------------------
// Close window
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlBaseWindow::Close()
    {
    if (DoClose())
        {
        Hide(ETrue);
        }
    }

// ---------------------------------------------------------
// Enable or disable drag window
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlBaseWindow::SetDragEnable(TBool aEnable)
    {
    if (iMoveButton)
        {
        iMoveButton->EnableDragging(aEnable);
        }
    }

// ---------------------------------------------------------
// Handle close event
// ---------------------------------------------------------
//
EXPORT_C void CAknFepCtrlBaseWindow::HandleControlEvent(TInt aEventType, 
                                             CFepUiBaseCtrl* aCtrl, 
                                             const TDesC& /*aEventData*/)
    {
    if (aEventType == EEventButtonUp)
        {
        if (aCtrl == iCloseButton)
            {
            Close();
            }
        }
    }

EXPORT_C TBool CAknFepCtrlBaseWindow::DoClose()
    {
    return ETrue;
    }
    
EXPORT_C CFepUiBaseCtrl* CAknFepCtrlBaseWindow::Control(TInt aCtrlId)
    {
    CFepUiBaseCtrl* ctrl = NULL;
    RPointerArray<CFepUiBaseCtrl> todo;
    TInt current = 0;
    TBool notFound = ETrue;
    
    todo.Append(this);
    
    while (current < todo.Count() && notFound)
        {
        ctrl = todo[current];
        if(ctrl->ControlId() == aCtrlId)
            {
            notFound = EFalse;     
            }
        else
            {
            if(ctrl->IsKindOfControl(ECtrlControlGroup))
                {
                CControlGroup* temp = static_cast<CControlGroup*>(ctrl);
                for (int i = 0; i < temp->NumOfControls(); i++)
                    {
                    todo.Append(temp->At(i));
                    }
                }
            current++;
            }
        }
        
    todo.Close();
    
    return notFound ? NULL : ctrl;
    }    

// End Of File
