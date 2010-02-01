/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:
*
*/

// system includes
#include <barsread.h>
#include <peninputlayout.h>
#include <AknIconUtils.h>
#include <coemain.h>
#include <AknsUtils.h>
// user includes
#include <peninputrawkeybutton.h>
#include <peninputcommonctrls.hrh>

const TInt KInvalidEventId = 0xffff;
// ======== MEMBER FUNCTIONS ========

CAknFepCtrlRawKeyButton* CAknFepCtrlRawKeyButton::NewL(CFepUiLayout* aUiLayout, 
                                                       TInt aControlId, 
                                                       TInt aDownEvent,
                                                       TInt aUpEvent,
                                                       TInt aScanCode)
    { 
    CAknFepCtrlRawKeyButton* self = NewLC(aUiLayout, 
                                          aControlId, 
                                          aDownEvent, 
                                          aUpEvent,
                                          aScanCode);
    CleanupStack::Pop(self);
    
    return self;                        
    }
    
CAknFepCtrlRawKeyButton* CAknFepCtrlRawKeyButton::NewLC(CFepUiLayout* aUiLayout, 
                                                        TInt aControlId, 
                                                        TInt aDownEvent,
                                                        TInt aUpEvent,
                                                        TInt aScanCode)
    { 
    CAknFepCtrlRawKeyButton* self = new (ELeave) CAknFepCtrlRawKeyButton(aUiLayout, 
                                                                         aControlId,
                                                                         aDownEvent,
                                                                         aUpEvent,
                                                                         aScanCode);
    CleanupStack::PushL(self);
    self->BaseConstructL();
    return self;                      
    }

CAknFepCtrlRawKeyButton::CAknFepCtrlRawKeyButton(CFepUiLayout* aUiLayout, 
                                                 TInt aControlId, 
                                                 TInt aDownEvent,
                                                 TInt aUpEvent,
                                                 TInt aScanCode)
    : CAknFepCtrlButton(aUiLayout, aControlId),
      iDownEvent(aDownEvent),
      iUpEvent(aUpEvent),
      iScanCode(aScanCode)
    {        
    }

CAknFepCtrlRawKeyButton::~CAknFepCtrlRawKeyButton()
    {
    }

CFepUiBaseCtrl* CAknFepCtrlRawKeyButton::HandlePointerDownEventL(const TPoint& aPoint)
    {
    if (iDownEvent == KInvalidEventId)
        {
        return return CButtonBase::HandlePointerDownEventL(aPoint);
        }
    
    if(iDimmed)
        {
        return NULL;
        }
    
    CFepUiBaseCtrl::HandlePointerDownEventL(aPoint);
    if(IsActive())
        {
        SetBackgroundBmp(iActiveBmpPressed);
        SetBackgroundMaskBmp(iActiveMaskBmpPressed);
        }        
    else
        {
        SetBackgroundBmp(iNonActiveBkBmpPressed);
        SetBackgroundMaskBmp(iNonActiveBkMaskBmpPressed);
        }        

    Draw();
    UpdateArea(Rect(),EFalse);

	TKeyEvent event= {0,0,0,0};
    TPtrC eventDataPtr;
    event.iScanCode = iScanCode;
    eventDataPtr.Set(reinterpret_cast<TUint16*>(&event),sizeof(event));
        
    ReportEvent(iDownEvent,eventDataPtr);

    return this;
    }

CFepUiBaseCtrl* CAknFepCtrlRawKeyButton::HandlePointerUpEventL(const TPoint& aPoint)
    {
    if (iUpEvent == KInvalidEventId)
        {
        return CButtonBase::HandlePointerUpEventL(aPoint);
        }
    
    if(iDimmed)
        {
        return NULL;
        }
        
    if(IsActive())
        {
        SetBackgroundBmp(Bitmap(EBtnBmpActive,EFalse));
        SetBackgroundMaskBmp(Bitmap(EBtnBmpActive,ETrue));
        }
    else
        {
        SetBackgroundBmp(Bitmap(EBtnBmpNonActive,EFalse));
        SetBackgroundMaskBmp(Bitmap(EBtnBmpNonActive,ETrue));
        }

    SetActive(ETrue);
    Draw();
    UpdateArea(Rect(),EFalse);    
    
	TKeyEvent event= {0,0,0,0};
    TPtrC eventDataPtr;
    event.iScanCode = iScanCode;
    eventDataPtr.Set(reinterpret_cast<TUint16*>(&event),sizeof(event));

    ReportEvent(iUpEvent,eventDataPtr);

    return this;
    }

TInt CAknFepCtrlRawKeyButton::DownEvent() const
    {                           
    return iDownEvent;
    }

TInt CAknFepCtrlRawKeyButton::UpEvent() const
    {                           
    return iUpEvent;
    }

TInt CAknFepCtrlRawKeyButton::ScanCode() const
    {             
    return iScanCode;                                     
    }

void CAknFepCtrlRawKeyButton::SetDownEvent(TInt aEvent)
    {               
    iDownEvent = aEvent;
    }

void CAknFepCtrlRawKeyButton::SetUpEvent(TInt aEvent)
    {               
    iUpEvent = aEvent;
    }

void CAknFepCtrlRawKeyButton::SetScanCode(TInt aScanCode)
    {                                                
    iScanCode = aScanCode;  
    }
    
void CAknFepCtrlRawKeyButton::Draw()
    {
    CAknFepCtrlButton::Draw();
    }
    
   
//  End Of File