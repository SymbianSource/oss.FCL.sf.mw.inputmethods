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


#include <peninputsplititutnew.rsg>
#include <peninputsplititutconfig_chinese.rsg>
#include <skinlayout.cdl.h>
#include <AknsDrawUtils.h>
#include <AknsUtils.h>
#include <AknUtils.h> 
#include <peninputrawkeybutton.h>
#include <peninputrepeatbutton.h>
#include <peninputcommonbutton.h>
#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
//#include <peninputdropdownlist.h>
#include <s32mem.h>
#include <peninputlabel.h>
#include <peninputlayoutinputmodechoice.h>
#include <peninputlayoutbubblectrl.h>
#include <peninputcommonlayoutglobalenum.h>

#include "peninputsplititutwindowmanager.h"
#include "peninputsplititutwindow.h"
#include "peninputsplititutdatamgr.h"
#include "peninputsplititutuimgrbase.h"
#include "peninputsplititutuistatebase.h"
#include "peninputsplititutconverter.h"


// ============================ MEMBER FUNCTIONS =============================

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::NewL
// ---------------------------------------------------------------------------
//
CSplitItutWindowManager* CSplitItutWindowManager::NewL(CSplitItutUiLayout* aLayoutOwner,
                                                       CSplitItutDataMgr* aDataMgr)
    {
    CSplitItutWindowManager* layout = new (ELeave) 
                                                CSplitItutWindowManager(aLayoutOwner,aDataMgr);
    CleanupStack::PushL(layout);
    layout->ConstructL();
    CleanupStack::Pop(layout);
    return layout;
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::CSplitItutWindowManager
// ---------------------------------------------------------------------------
//
CSplitItutWindowManager::CSplitItutWindowManager(CSplitItutUiLayout* aLayoutOwner,
                                                     CSplitItutDataMgr* aDataMgr)
                                          :iLayoutOwner(aLayoutOwner),
                                           iDataMgr(aDataMgr),
                                           iLastRawKeyDown(EStdKeyNull)
                                           
    {
    iInEditWordQueryDlg = EFalse;
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::CSplitItutWindowManager
// ---------------------------------------------------------------------------
//
CSplitItutWindowManager::~CSplitItutWindowManager()
    {
    delete iWindow;
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::CSplitItutWindowManager
// ---------------------------------------------------------------------------
//
TInt CSplitItutWindowManager::OnAppEditorTextComing(const TFepInputContextFieldData& aData)
    {
    TRAPD(err, iWindow->Icf()->SetTextL(aData));
    return err;
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::SetPropertyL
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::SetPropertyL(MItutPropertySubscriber::TItutProperty aPropertyName, 
                                        const TDesC& aPropertyValue)
    {
    iWindow->SetPropertyL(aPropertyName, aPropertyValue);
    
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::ConstructL
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::ConstructL()
    {
    iWindow = CSplitItutWindow::NewL(this, iLayoutOwner,iDataMgr);
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::SimulateRawEvent
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::SimulateRawEvent(TInt aScanCode, TRawEvent::TType aType)
    {
    iLayoutOwner->SimulateRawEvent(aScanCode, aType);
    }
                
// ---------------------------------------------------------------------------
// CSplitItutWindowManager::HandleCtrlEventL
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::HandleCtrlEventL(TInt aEventType, 
                                                 CFepUiBaseCtrl* aCtrl, 
                                                 const TDesC& aEventData)
    {
    switch(aEventType)
        {
        case EEventRawKeyDownEvent:
            {
            const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());
            SimulateRawEvent(key->iScanCode,TRawEvent::EKeyDown);
            SetLastRawKeyDown(key->iScanCode, ETrue, aCtrl);
            }
            break;
        case EEventRawKeyUpEvent:
            {
            const TKeyEvent *key = reinterpret_cast<const TKeyEvent*>(aEventData.Ptr());
            SimulateRawEvent(key->iScanCode,TRawEvent::EKeyUp);
            SetLastRawKeyDown(key->iScanCode, EFalse, aCtrl);
            }
            break;       
        case EEventButtonUp:
            {
/*            else if (aCtrl->ControlId() == ECtrlIdArrowUp)
                {
                TRAP_IGNORE(iWindow->Icf()->HandleArrowBtnEventL(
                                                          CFepLayoutMultiLineIcf::EArrowUp));
                }
            else if (aCtrl->ControlId() == ECtrlIdArrowDown)
                {
                TRAP_IGNORE(iWindow->Icf()->HandleArrowBtnEventL(
                                                          CFepLayoutMultiLineIcf::EArrowDown));
                }*/
            if (aCtrl->ControlId() == ECtrlIdClose)
                {
                iLayoutOwner->SignalOwner(ESignalLayoutClosed);
                }
            else if (aCtrl->ControlId() == ECtrlIdOptions)
                {
                iLayoutOwner->SignalOwner(ESignalLaunchOptionsMenu);
                }               
            }
            break;
            
 /*       case EItutCmdArrowUp:
            {
            if (aCtrl->ControlId() == ECtrlIdArrowUp)
                {
                TRAP_IGNORE(iWindow->Icf()->HandleArrowBtnEventL(
                                                          CFepLayoutMultiLineIcf::EArrowUp));
                }
            }
            break;
        case EItutCmdArrowDown:
            {
            if (aCtrl->ControlId() == ECtrlIdArrowDown)
                {
                TRAP_IGNORE(iWindow->Icf()->HandleArrowBtnEventL(
                                                          CFepLayoutMultiLineIcf::EArrowDown));
                }
            }
            break;*/
        case EEventCapturePointer:
            {
            if (iRawKeyDown)
                {
                SimulateRawEvent(iLastRawKeyDown, TRawEvent::EKeyUp);
                iRawKeyCtrl->CancelPointerDownL();
                SetLastRawKeyDown(iLastRawKeyDown, EFalse, NULL);
                }
            }
            break;      
        default:
            break;
        };
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::HandleCtrlEventL
// ---------------------------------------------------------------------------
//
TBool CSplitItutWindowManager::HandleCommandL(TInt aCmd, TUint8* aData)
    {
    TBool handle = EFalse;
    TInt* data = (TInt*) aData;
    switch (aCmd)
        {
        case ECmdPenInputDimArrowKeys:
            {
            TBool IsDimArrowKeys = *aData;
            DimArrowKeys( IsDimArrowKeys );
            }
            break;
        case ECmdPenInputEnableSettingBtn:
            {
            CAknFepCtrlCommonButton * optionsbtn;
            TBool enable = *data;            
            optionsbtn = static_cast<CAknFepCtrlCommonButton *>(iWindow->Control(ECtrlIdOptions));
            if(optionsbtn && !enable)
                {
                optionsbtn->SetDimmed(ETrue);
                handle = ETrue;
                iDimOperationBtn = ETrue;
                }
            else if(optionsbtn && enable)
                {
                optionsbtn->SetDimmed(EFalse);
                handle = ETrue;
                iDimOperationBtn = EFalse;
                }
            break;
            }
        case ECmdPenInputSymbolOfHardKeyOne:
            {
            iWindow->SetHardKeyOneSymbol(aData);
            handle = ETrue;
            }
            break;

        case ECmdPenInputInEditWordQueryDlg:
            {
            iInEditWordQueryDlg = *data;
            }
            break;
        case ECmdPenInputSetPromptText:
        	{
        	if( iDataMgr->IsSpellMode())
        		{
        	    SetPromptTextL(aData);
        	    handle = ETrue;
        		}
        	}
        	break;
        case ECmdPenInputFingerMatchIndicator:
        	{
        	iWindow->UpdateIndiBubbleL( aData );
        	handle = ETrue;
        	}
        	break;
        default:
            break;
            
        };
    return handle;
    }


// ---------------------------------------------------------------------------
// CSplitItutWindowManager::SizeChanged
// ---------------------------------------------------------------------------
//
TInt CSplitItutWindowManager::SizeChanged()
    {
    return iWindow->SizeChanged();
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::SubmitText
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::SubmitText(const TDesC& aEventData)
    {
    iLayoutOwner->SignalOwner(ESignalKeyEvent, aEventData);
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::OnSkinChange
// ---------------------------------------------------------------------------
//
TInt CSplitItutWindowManager::OnSkinChange()
    {
    TRAPD(err,
          iWindow->ConstructSpellCtrlFromResourceL(); 
          iWindow->ConstructItutKeypadFromResourceL(iWindow->ItutKeypad()->ResourceId()));
   
    iWindow->OnSkinChange();
    return err;
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::OnSkinChange
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::ShowArrowBtn(TInt aShowFlag)
    {
    iWindow->Control(ECtrlIdArrowLeft)->Hide(!(EBtnArrowLeft & aShowFlag));
    iWindow->Control(ECtrlIdArrowRight)->Hide(!(EBtnArrowRight & aShowFlag));
//    iWindow->Control(ECtrlIdArrowUp)->Hide(!(EBtnArrowUp & aShowFlag));
//    iWindow->Control(ECtrlIdArrowDown)->Hide(!(EBtnArrowDown & aShowFlag));
    }
    
// ---------------------------------------------------------------------------
// CSplitItutWindowManager::UiLayout
// ---------------------------------------------------------------------------
//
CFepUiLayout* CSplitItutWindowManager::UiLayout()
    {
    return iLayoutOwner;
    }
    
// ---------------------------------------------------------------------------
// CSplitItutWindowManager::HandleAppInfoChangeL
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::HandleAppInfoChangeL(const TDesC& /*aInfo*/)
    {
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::ApplyVariantLafDataL
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::ApplyVariantLafDataL(TBool aResolutionChange)
    {
    iWindow->ApplyVariantLafDataL(aResolutionChange);
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::ApplyVariantLafDataForSpellL
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::ApplyVariantLafDataForSpellL()
    {
    iWindow->ApplyVariantLafDataForSpellL();
    }


// ---------------------------------------------------------------------------
// CSplitItutWindowManager::CreateChineseSpecificCtrlsIfNeededL
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::CreateChineseSpecificCtrlsIfNeededL()
    {
    iWindow->CreateChineseSpecificCtrlsIfNeededL();   
    }
    
    
// ---------------------------------------------------------------------------
// CSplitItutWindowManager::SetLastRawKeyDown
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::SetLastRawKeyDown(TInt aScanCode,  TBool aKeyDown, 
                                                  CFepUiBaseCtrl* aCtrl)
    {
    !aKeyDown ? iLastRawKeyDown = EStdKeyNull : iLastRawKeyDown = aScanCode;
    iRawKeyDown = aKeyDown;
    iRawKeyCtrl = aCtrl;
    } 
        
// ---------------------------------------------------------------------------
// CSplitItutWindowManager::Control
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CSplitItutWindowManager::Control(TInt aCtrlId)  
    {
    return iWindow->Control(aCtrlId);   
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::OnActivate
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::OnActivate()
    {
    if (iDimOperationBtn)
        {
        static_cast<CAknFepCtrlCommonButton*>(Control(ECtrlIdOptions))->SetDimmed(ETrue);    
        }
    else
        {
        static_cast<CAknFepCtrlCommonButton*>(Control(ECtrlIdOptions))->SetDimmed(EFalse);      
        }
    }
    
// ---------------------------------------------------------------------------
// CSplitItutWindowManager::DimArrowKeys
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::DimArrowKeys( TBool aDimArrow )
    {
    if (iDataMgr->IsChinese())
        {
        CAknFepCtrlRawKeyButton * leftbtn;
        CAknFepCtrlRawKeyButton * rightbtn;
//        CAknFepCtrlRawKeyButton * upbtn;
//        CAknFepCtrlRawKeyButton * downbtn;
        leftbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowLeft));
        rightbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowRight));
//        upbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowUp));
//        downbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowDown));

        leftbtn->SetDimmed(aDimArrow);   
        rightbtn->SetDimmed(aDimArrow);   
//        upbtn->SetDimmed(aDimArrow);   
//        downbtn->SetDimmed(aDimArrow);   

        }
    else
        {
        CAknFepCtrlRawKeyButton * leftbtn;
        CAknFepCtrlRawKeyButton * rightbtn;
        leftbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowLeft));
        rightbtn = static_cast<CAknFepCtrlRawKeyButton *>(iWindow->Control(ECtrlIdArrowRight));

        leftbtn->SetDimmed(aDimArrow);   
        rightbtn->SetDimmed(aDimArrow);   
        }                
    }
    
// ---------------------------------------------------------------------------
// CSplitItutWindowManager::ShowBubble
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::ShowBubble(TInt /*aShow*/)
    {
    /*
    CFepCtrlDropdownList* spell = 
        static_cast<CFepCtrlDropdownList*>(iWindow->Control(ECtrlIdSpellCandsList));
    CFepCtrlDropdownList* standard = 
        static_cast<CFepCtrlDropdownList*>(iWindow->Control(ECtrlIdStdCandsList));
    CFepCtrlDropdownList* punctuation = 
        static_cast<CFepCtrlDropdownList*>(iWindow->Control(ECtrlIdPuncCandsList));
    
    if (aShow > 0)
        {
    	TRect outrect,innerrect;
    	outrect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(EPreviewBubbleRect));  
    	innerrect = TItutDataConverter::AnyToRect(iDataMgr->RequestData(EPreviewBubbleInnerRect));
        TAknTextLineLayout bubbleTextLayout = 
            TItutDataConverter::AnyToTextLine(iDataMgr->RequestData(EPreviewBubbleTextlayout));
        CFont* bubbleFont = TItutDataConverter::AnyToFont(iDataMgr->RequestData(EPreviewBubbleFont));
        
        spell->ShowBubble(ETrue);
        standard->ShowBubble(ETrue);
        punctuation->ShowBubble(ETrue);

        spell->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview); 	
     	
    	spell->SetTextFormat(bubbleTextLayout);
    	spell->SetBubbleTextFont(bubbleFont);
    	spell->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
    	spell->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
    	               innerrect.iTl.iY - outrect.iTl.iY,
    	               outrect.iBr.iX - innerrect.iBr.iX,
    	               outrect.iBr.iY - innerrect.iBr.iY);
        
        standard->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview); 	
     	
    	standard->SetTextFormat(bubbleTextLayout);
    	standard->SetBubbleTextFont(bubbleFont);
    	standard->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
    	standard->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
    	               innerrect.iTl.iY - outrect.iTl.iY,
    	               outrect.iBr.iX - innerrect.iBr.iX,
    	               outrect.iBr.iY - innerrect.iBr.iY);
    
        punctuation->SetBubbleBitmapParam(NULL,NULL,KAknsIIDQsnFrInputCharPreview); 	
     	
    	punctuation->SetTextFormat(bubbleTextLayout);
    	punctuation->SetBubbleTextFont(bubbleFont);
    	punctuation->SetBubbleSize(TSize(outrect.Width(),outrect.Height()));  // Read laf
    	punctuation->SetFrameDiff(innerrect.iTl.iX - outrect.iTl.iX,
    	               innerrect.iTl.iY - outrect.iTl.iY,
    	               outrect.iBr.iX - innerrect.iBr.iX,
    	               outrect.iBr.iY - innerrect.iBr.iY);
        }
    else
        {
        spell->ShowBubble(EFalse);
        standard->ShowBubble(EFalse);
        punctuation->ShowBubble(EFalse);
        }
    */
    }

// ---------------------------------------------------------------------------
// CSplitItutWindowManager::ShowBubble
// ---------------------------------------------------------------------------
//
void CSplitItutWindowManager::SetPromptTextL(TUint8* aData)
    {
    iWindow->SetPromptTextL(aData);
    }
         
// End Of File
