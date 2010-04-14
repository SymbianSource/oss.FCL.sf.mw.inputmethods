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
* Description:  drop-down list control
*
*/


#include <AknFepGlobalEnums.h>
#include <peninputgeneralitutnew.rsg>
#include <peninputscrollablelist.h>
#include <peninputlayout.h>
#include <e32keys.h>
#include <s32mem.h>
#include <peninputlayoutmultilineicf.h>
#include <StringLoader.h>

#include "peninputitutwesternuistatematchselection.h"
#include "peninputgenericitutuimgrbase.h"
#include "peninputgenericitutdatamgr.h"
#include "peninputgenericitutlayoutcontext.h"
#include "peninputgenericitutconverter.h"
#include "peninputgenericitutwindowmanager.h"

CWesternItutUiStateMatchSelection* CWesternItutUiStateMatchSelection::NewL(
    CGenericItutUiMgrBase* aOwner)
    {
    CWesternItutUiStateMatchSelection* self = 
        new (ELeave) CWesternItutUiStateMatchSelection(aOwner);
    CleanupStack::PushL(self);
    self->ConstructL(); 
    CleanupStack::Pop(self);
    return self;
    }

CWesternItutUiStateMatchSelection::~CWesternItutUiStateMatchSelection()
    {
    iCandlist.ResetAndDestroy();
    iCandlist.Close();
    delete iSpellText;
    }

CWesternItutUiStateMatchSelection::CWesternItutUiStateMatchSelection(
    CGenericItutUiMgrBase* aOwner):CGenericItutUiStateBase(aOwner)
    {
    iSelList = static_cast<CFepLayoutScrollableList*>(
        iOwner->LayoutContext()->Control(ECtrlIdMatchSelection));
        
    iIcf = static_cast<CFepLayoutMultiLineIcf*>(
        iOwner->LayoutContext()->Control(ECtrlIdICF));
    }

void CWesternItutUiStateMatchSelection::ConstructL()
    {
    CGenericItutUiStateBase::ConstructL();
    iSpellText = StringLoader::LoadL(R_PENINPUT_FINGER_SPELL);
    }

void CWesternItutUiStateMatchSelection::OnEntryL()
    {
    TInt keypadResId = KInvalidResId;
    switch (iOwner->DataMgr()->InputMode())
        {
        case ENumber:
            keypadResId = R_FINGER_INPUT_KEYPAD_NUMBER;
            break;
        case ELatin:
            keypadResId = iOwner->DataMgr()->KeypadResourceId();
            break;
        default:
            break;                
        }

    // update resource id of keypad, candidate drop list
    iOwner->DataMgr()->NotifyChangeControlLayout(
        MItutPropertySubscriber::EItutPropertyKeypadResourceId, 
        keypadResId);

    // Show ICF, Backspace after position changed to avoid flick
    iOwner->LayoutContext()->Control(ECtrlIdICF)->Hide( EFalse );
    iOwner->LayoutContext()->Control(ECtrlIdBackspace)->Hide( EFalse );
    if (iOwner->DataMgr()->IsChinese())
        {
        iOwner->LayoutContext()->ShowArrowBtn(EBtnArrowLeft | EBtnArrowRight
                 | EBtnArrowUp| EBtnArrowDown);   
        }
    else
        {
        iOwner->LayoutContext()->ShowArrowBtn(EBtnArrowLeft | EBtnArrowRight);
        }
   }

void CWesternItutUiStateMatchSelection::OnExit()
    {
    // Hide ICF, Backspace, Arrow contrls when exit to avoid flick
    iOwner->LayoutContext()->ShowArrowBtn(0);
    iOwner->LayoutContext()->Control(ECtrlIdICF)->Hide( ETrue );
    iOwner->LayoutContext()->Control(ECtrlIdBackspace)->Hide( ETrue );
    iSelList->CloseWindow();
    iIcf->SetOverLapState(EFalse);
    }

CGenericItutUiMgrBase::TUiState CWesternItutUiStateMatchSelection::StateType()
    {
    return CGenericItutUiMgrBase::EStateMatchSelection;
    }
   
void CWesternItutUiStateMatchSelection::ReportItemSelected(TInt aCmdId, TInt aIdx, TBool aCommit)
    {
    TBuf<8> buf;
    buf.Append(reinterpret_cast<TText*>(&aIdx), sizeof(TInt)/sizeof(TText));
    buf.Append(reinterpret_cast<TText*>(&aCommit), sizeof(TBool)/sizeof(TText));

    iOwner->LayoutContext()->UiLayout()->SignalOwner(aCmdId, buf);
    }
    
void CWesternItutUiStateMatchSelection::ReportItemSelected(TInt aCmdId, TInt aIdx)
    {
    TBuf<4> buf;
    buf.Append(reinterpret_cast<TText*>(&aIdx), sizeof(TInt)/sizeof(TText));
 
    iOwner->LayoutContext()->UiLayout()->SignalOwner(aCmdId, buf);
    }

TBool CWesternItutUiStateMatchSelection::HandleCtrlEventL(
    TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData)
    {
    switch (aEventType)
        {
        case EItutCmdPredictItemSelected:
            {
            TInt itemidx = *(TInt*)aEventData.Ptr();

            // if press spell, then enter spell mode, otherwise select an item
            // note that spell do not belong to iCandlist
            if ( itemidx == iCandlist.Count())
            	{

				iOwner->UiManager()->StartPreventSCTTimer();

       
				iOwner->LayoutContext()->UiLayout()->SignalOwner(ESignalEnterSpellMode); 
            	}
            else
            	{
				// according to updated ui spec, if select an item, do not commit it
				//ReportItemSelected(ESignalSelectMatchSelectionText, itemidx, ETrue);
				ReportItemSelected(ESignalSelectMatchSelectionText, itemidx, ETrue);
            	}
            
            return ETrue;
            }
        case EEventChoiceSelected:
            {
            CFepLayoutChoiceList::SEvent* event = 
                    (CFepLayoutChoiceList::SEvent*)aEventData.Ptr();

            if ( aCtrl->ControlId() == ECtrlIdMatchSelection && event->iIndex == -1)
                {
                iOwner->LayoutContext()->EnterMatchSelectionState(EFalse);
                return ETrue;
                }
            }
            break;
        default:
            break;
        }

    return EFalse;
    }

TInt CWesternItutUiStateMatchSelection::HandleCommandL(TInt aCmd, TUint8* aData)
    {
    switch (aCmd)
        {
        case ECmdPenInputFingerMatchList:
            {
            RDesReadStream readStream;
            TPtr8 countPtr(aData, 2*sizeof(TInt), 2*sizeof(TInt));
            readStream.Open(countPtr);
            CleanupClosePushL(readStream);

            TInt candcount = readStream.ReadInt32L();
            TInt totalsize = readStream.ReadInt32L();
            CleanupStack::PopAndDestroy(&readStream);

            TPtr8 ptr(aData + 2*sizeof(TInt), totalsize - 2*sizeof(TInt), 
                      totalsize - 2*sizeof(TInt));
            readStream.Open(ptr);
            CleanupClosePushL(readStream);

            iCandlist.ResetAndDestroy();

            iSelList->ClearItemsL();
            CFepLayoutChoiceList::SItem item;

            for (TInt i = 0; i < candcount; i++)
                {
                TInt candSize = readStream.ReadInt32L();

                HBufC* candBuf = HBufC::NewLC(candSize/2);
                TPtr candBufPtr = candBuf->Des();

                readStream.ReadL(candBufPtr, candSize/2);
                iCandlist.AppendL(candBuf);
                CleanupStack::Pop(candBuf);

                item.iCommand = EItutCmdPredictItemSelected;
                item.iText.Copy(candBufPtr);
                iSelList->AddItemL(item); 
                
                }

            if (iCandlist.Count() != 0)
                {
                item.iCommand = EItutCmdEnterSpellMode;
                item.iText.Copy(*iSpellText);
                iSelList->AddItemL(item);
                SetMatchSelListLTPosAndDisplay();
                }

    		CleanupStack::PopAndDestroy(&readStream);
            return KErrNone;
            }
        default:
            break;    
        }

    return KErrNotSupported;    
    }

void CWesternItutUiStateMatchSelection::SetMatchSelListLTPosAndDisplay()
    {
    TPoint posbelow, posright;
    posbelow = posright = iIcf->Rect().iTl;
    iIcf->InlineTextPos(posbelow, posright);
    TPoint tlPos = iOwner->DataMgr()->IsLandScape() ? posright : posbelow;
    iSelList->SetCurrentFocusedIndex(-1);
    iSelList->Display( TRect(tlPos, tlPos), CFepLayoutPopupWnd::EDispBottomRight);
    }

// End Of File
