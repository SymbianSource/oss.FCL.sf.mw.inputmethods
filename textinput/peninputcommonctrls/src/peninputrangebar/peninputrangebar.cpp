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
* Description:  Implementation of range bar
*
*/


// system includes
//#include <peninputpluginutils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknsUtils.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <barsread.h>

// user includes
#include <peninputrangebar.h>
#include <peninputbutton.h>
#include <peninputmultimodebutton.h>
#include <peninputcommonctrls.hrh>
#include <peninputpluginutils.h>
#include <peninputcommonbutton.h>

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::CAknFepCtrlRangeBar
// C++ constructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlRangeBar::CAknFepCtrlRangeBar(CFepUiLayout* aUiLayout, TInt aControlId, 
                                                  TButtonGroupStyle aStyle)
: CControlGroup(aUiLayout, aControlId, EFalse), iFirstTimeConstruct(ETrue)
    {
    iStyle = aStyle;
    //iActionStyle = ERangeBarActionStyleHide;
    iActionStyle = ERangeBarUsingIndicatorStyle;
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::~CAknFepCtrlRangeBar
// C++ destructor
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlRangeBar::~CAknFepCtrlRangeBar()
    {
    iIndicators.ResetAndDestroy();
    iIndicators.Close();
    iCellRects.Close();
    iCellInnerRects.Close();
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::NewL
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlRangeBar* CAknFepCtrlRangeBar::NewL(CFepUiLayout* aUiLayout, TInt aControlId,
                                                        TButtonGroupStyle aStyle)
    {
    CAknFepCtrlRangeBar* self = new (ELeave) CAknFepCtrlRangeBar (aUiLayout, aControlId, aStyle);
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::NewLC
// Two-phased constructor.
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepCtrlRangeBar* CAknFepCtrlRangeBar::NewLC(CFepUiLayout* aUiLayout,
                                                         TInt aControlId,
                                                         TButtonGroupStyle aStyle)
    {
    CAknFepCtrlRangeBar* self = new (ELeave) CAknFepCtrlRangeBar (aUiLayout, aControlId, aStyle);
    CleanupStack::PushL(self);
    self->ConstructL();
    return self;
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::ConstructL
// Construct the instance
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::ConstructL()
    {
    // Construct the base control first
    CControlGroup::ConstructL();
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::HandlePointerDownEventL
// Implement the pointer down handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
CFepUiBaseCtrl* CAknFepCtrlRangeBar::HandlePointerDownEventL(const TPoint& aPoint)
    {
    CFepUiBaseCtrl* ctrl = CControlGroup::HandlePointerDownEventL( aPoint );
    return ctrl;
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::SizeChanged
// Implement the SizeChanged handler
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::SizeChanged(const TRect aRect)
    {
    SetCellRect(aRect);

    if( iCellChanged )
		{
		SetRect(aRect);

		iActionStyle == ERangeBarUsingIndicatorStyle ? 
        UpdateIndicatorRectForUsingIndicatorStyle(EFalse) :
		UpdateIndicatorRect(EFalse);
		}
    else
		{
		iIsReloadImages = ETrue;
        SetWholeRect();
        //Draw();
        SetIndicatorRect();					
		}
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::ConstructFromResourceL
// Construct the range bar from the resource
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::ConstructFromResourceL(TResourceReader& aReader)
    {
    //destroy all previous range buttons

    for (TInt ii = 0; ii < iIndicators.Count(); ++ii)
        {
        CAknFepCtrlCommonButton* button = iIndicators[ ii ]->iIndicator;
        if (button)
            {
            RemoveControl(button);
            }
        }

    iIndicators.ResetAndDestroy();

    const TInt countRanges = aReader.ReadInt16();
    
    if (countRanges <= 0)
        {
        User::Leave(KErrArgument);
        }

    // Create range indicator buttons
    for (TInt ii = 0; ii < countRanges; ++ii)
        {
        AddRangeL(aReader);        
        }
        
    const TInt defaultRangeIndex = aReader.ReadInt16();
    
    if (defaultRangeIndex >= 0 && defaultRangeIndex < iIndicators.Count())
        {
        SetRange(iIndicators[defaultRangeIndex]->iInputRange);    
        }
    
    iFirstTimeConstruct = EFalse;    
    }
    
// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::SetIndicatorRect()
// Re-calculate the position and layout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CAknFepCtrlRangeBar::SetIndicatorRect()
	{
    TRect rect(iIndicatorRect);
    TInt count = iIndicators.Count();
    TInt lastPos = -1;
	
    for (TInt ii = 0; ii < count; ++ii)
        {  
        if ( ii != 0 && 
		     (iActionStyle != ERangeBarUsingIndicatorStyle || 
             lastPos != iIndicators[ii]->iPosIdx ) )
            {
            if (iStyle == EBtnGroupVertical)
                {
                rect.Move(0, iIndicatorRect.Height());
                }
            else 
                {
                rect.Move(iIndicatorRect.Width(), 0);
                } 
            }
		lastPos = iIndicators[ii]->iPosIdx;
		
		TRect innerRect;
		AknPenImageUtils::CalculateGraphicRect( rect, innerRect );
                                                                 
        if (iIndicators[ii]->iCtrlType == EButtonTypeMultiMode)
            {
            static_cast<CAknFepCtrlMultiModeButton*>(iIndicators[ii]->iIndicator)->
            			SizeChanged(rect, innerRect, iIsReloadImages);
            }
        else
            {             
            static_cast<CAknFepCtrlCommonButton*>(iIndicators[ii]->iIndicator)->
            			SizeChanged(rect, innerRect, iIsReloadImages);
            }
        }
	}
    
// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::Draw()
// Re-calculate the position and layout
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::Draw()
    {
	if(!AbleToDraw())
        return;
    
    CControlGroup::Draw();
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::RefreshUI
// Refresh the component
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::RefreshUI()
    {
    // this function not called anymore
    SetIndicatorRect();
    
    // Flush the gc memory to the LCD device
    UpdateArea(iEraseRect, EFalse);               
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::HandleControlEvent
// responding on button up event
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::HandleControlEvent(TInt aEventType, CFepUiBaseCtrl* aCtrl, 
                                                      const TDesC& /*aEventData*/)
    {
    if (aEventType == EEventButtonUp)
        {
        // get aCtrl's index
        TBool ctrlExist = EFalse;
        TInt ctrlIndex = 0;
        
        for (TInt ii = 0; ii < iIndicators.Count(); ii++)
            {
            if ((iIndicators[ii]->iIndicator == aCtrl) 
            && (!(iIndicators[ii]->iIndicator->IsDimmed())))
                {
                ctrlExist = ETrue;
                ctrlIndex = ii;
                break;
                }
            }        
        
        if (!ctrlExist)
            {
            return;
            }
        
        TBool needSendEvent = EFalse;
        
        // aCtrl's index is ctrlIndex
        if ( (ActiveControl() != aCtrl) 
            || (iIndicators[ctrlIndex]->iCtrlType == EButtonTypePopupMode) )
            {
            needSendEvent = ETrue;
            }
        else // press active button again
            {
            // only meaningful when it is multimode button
            if (iIndicators[ctrlIndex]->iCtrlType == EButtonTypeMultiMode)
                {
                CAknFepCtrlMultiModeButton* button = static_cast<CAknFepCtrlMultiModeButton*>(iIndicators[ctrlIndex]->iIndicator);
                
                // change range: user press mixed range multimode button, and range is changed
                if (iIndicators[ctrlIndex]->iRangeType == ERangeTypeMixed)
                    {
                    if ((button->NextMode() != KErrNotFound) && 
                        (button->RangeOfGivenMode(button->CurrentMode()) != 
                         button->RangeOfGivenMode(button->NextMode())))
                        {
                        // change range
                        ReportEventToObserver(iEventIdForRange, 
                                              button->RangeOfGivenMode(button->NextMode()));
                        }
                    else if (button->RangeOfGivenMode(button->CurrentMode()) == 
                             button->RangeOfGivenMode(button->NextMode()))
                        {
                        // mixed range multimode button: change case  
                        ReportEventToObserver(iEventIdForCase, button->NextMode());
                        }
                    }
                else
                    {
                    // single range multimode button: change case
                    ReportEventToObserver(iEventIdForCase, button->NextMode());
                    }
                }
            else
                {
                if ( iIndicators[ctrlIndex]->iInputRange != iInputRange )
                    {
                    needSendEvent = ETrue;
                    }
                }
            }
            
        if ( needSendEvent )  
            {
            // press another button instead of active button, it will send range event
            if ((iIndicators[ctrlIndex]->iCtrlType == EButtonTypeMultiMode) && 
                (iIndicators[ctrlIndex]->iRangeType == ERangeTypeMixed))
                {
                CAknFepCtrlMultiModeButton* button = 
                                                static_cast<CAknFepCtrlMultiModeButton*>(aCtrl);
                        
                if ((button) && 
                    (button->IsGivenRangeSupported(
                                                button->RangeOfGivenMode(button->CurrentMode()))))
                    {
                    ReportEventToObserver(iEventIdForRange, 
                                          button->RangeOfGivenMode(button->CurrentMode()));
                    }
                }
            else
                {
                // for single range multimode button and normal button
                ReportEventToObserver(iEventIdForRange, iIndicators[ctrlIndex]->iInputRange);
                }                    
            }
        
        // make sure the normal button press effect for :
        // using indicator style, and EIndicatorActionStyleNormal
        if (iActionStyle == ERangeBarUsingIndicatorStyle && 
            iIndicators[ctrlIndex]->iActionStyle == EIndicatorActionStyleNormal &&
            iIndicators[ctrlIndex]->iIndicator->IsActive())
        	{
        	iIndicators[ctrlIndex]->iIndicator->SetHighlight(EFalse);	
        	}
        	
	    if ( iActionStyle == ERangeBarUsingIndicatorStyle && 
	    	 iIndicators[ctrlIndex]->iActionStyle != EIndicatorActionStyleHighLight )
	    	{
	    	for (TInt ii = 0; ii < iIndicators.Count(); ii++)
		        {
		        if(iIndicators[ ii ]->iActionStyle == EIndicatorActionStyleHighLight
		           && ii != ctrlIndex)
		        	{
		        	iIndicators[ ii ]->iIndicator->SetHighlight( 
		        	  iIndicators[ ii ]->iIndicator->IsActive() );
		        	}
		    	}
	    	}
        }
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::SetRange
// set current input range
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CAknFepCtrlRangeBar::SetRange(const TInt aRange)
    {
    // if set range in a mixed range multimode button, 
    // should use detailed range, but not mixed range
    TBool ret = EFalse;
    
    for (TInt ii = 0; ii < iIndicators.Count(); ii++)
        {
        CAknFepCtrlCommonButton* button = iIndicators[ ii ]->iIndicator;
        
        if ((button) && (!(button->IsDimmed())))
            {
            if ((iIndicators[ii]->iInputRange == aRange) ||
               ((iIndicators[ii]->iCtrlType == EButtonTypeMultiMode) && 
                (iIndicators[ii]->iRangeType == ERangeTypeMixed) &&             
                (static_cast<CAknFepCtrlMultiModeButton*>(button)->IsGivenRangeSupported(aRange))))
                {
                // 1st condition: normal button or single range multimode button
                // 2nd condition: mixed range multimode button
                if (!(iIndicators[ii]->iInputRange == aRange))
                    {
                    // mixed range multimode button
                    static_cast<CAknFepCtrlMultiModeButton*>(button)->SetRange(aRange);
                    }

                iInputRange = aRange;

                SetActiveCtrl(button);

                ret = ETrue; 
                }
            else
            	{
            	button->SetActive(EFalse);
            	}
            }
        }
    
    if ( iActionStyle == ERangeBarActionStyleHighLight )
    	{
    	for (TInt ii = 0; ii < iIndicators.Count(); ii++)
	        {
	        iIndicators[ ii ]->iIndicator->SetHighlight( iIndicators[ ii ]->iIndicator->IsActive() );
	    	}
    	}
    else
    	{
    	for (TInt ii = 0; ii < iIndicators.Count(); ii++)
	        {
	        if(iIndicators[ ii ]->iActionStyle == EIndicatorActionStyleHighLight)
	        	{
				iIndicators[ ii ]->iIndicator->SetHighlight( iIndicators[ ii ]->iIndicator->IsActive() );
	        	}
	    	}
    	}
    return ret;    
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::SetCaseL
// set mode of given range
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::SetCaseL(const TInt aRange, const TInt aModeIndex)
    {
    // aRange is detailed range, it only meaningful for multimodebutton
    for (TInt ii = 0; ii < iIndicators.Count(); ii++)
        {
        CAknFepCtrlMultiModeButton* button = static_cast<CAknFepCtrlMultiModeButton*>
                                             (iIndicators[ii]->iIndicator);            
        if ((iIndicators[ii]->iCtrlType == EButtonTypeMultiMode) && 
            ((iIndicators[ii]->iInputRange == aRange) ||
             ((iIndicators[ii]->iRangeType == ERangeTypeMixed) && 
              (button->IsGivenRangeSupported(aRange)))))
            {            
            if (iIndicators[ii]->iRangeType == ERangeTypeMixed)
                {
                button->SetRange(aRange);
                }
                
            if (button->IsActive())
                {
                button->SetCurrentModeL(EBtnBmpActive, aModeIndex);
                }
            else
                {
                button->SetCurrentModeL(EBtnBmpNonActive, aModeIndex);
                }
            
            break;    
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::SetPermittedRanges
// set permitted ranges
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::SetPermittedRanges(const TInt aPermitted)
    {
    // aPermitted only include detailed range, not mixed    
    for (TInt ii = 0; ii < iIndicators.Count(); ii++)
        {
        TBool needDimButton = EFalse;
        CAknFepCtrlCommonButton* button= iIndicators[ii]->iIndicator;
 
        if ((iIndicators[ii]->iCtrlType == EButtonTypeMultiMode) && 
            (iIndicators[ii]->iRangeType == ERangeTypeMixed))
            {
            // mixed multi button
            RArray<TInt> rangeList;
            TRAP_IGNORE(static_cast<CAknFepCtrlMultiModeButton*>
                        (button)->GetRangeListL(rangeList));

            needDimButton = ETrue;
            TInt disableIdx = -1;
            
            for (TInt ii = 0; ii < rangeList.Count(); ii++)
                {
                if (aPermitted & rangeList[ii])
                    {
                    static_cast<CAknFepCtrlMultiModeButton*>(button)->SetPermittedRange(
                                                                   rangeList[ii], ETrue);
                    needDimButton = EFalse;
                    }
                else
                    {
                    static_cast<CAknFepCtrlMultiModeButton*>(button)->SetPermittedRange(
                                                                   rangeList[ii], EFalse);
                    disableIdx = ii;
                    }
                }
                
            // disable mode can only after permitted modes are all setted.
			    if ( disableIdx >= 0 )
                {
                static_cast<CAknFepCtrlMultiModeButton*>(button)->DisableOneModes(
                                                             rangeList[disableIdx]);
                }
				
            rangeList.Reset();
            rangeList.Close(); 
            }
        else if (!(aPermitted & iIndicators[ii]->iInputRange))
            {
            // disable a normal or single range multi button
            needDimButton = ETrue;
                    
            if (iIndicators[ii]->iCtrlType == EButtonTypeMultiMode)
                {
                TRAP_IGNORE(static_cast<CAknFepCtrlMultiModeButton*>(button)->SetCurrentModeL(
                                                                        EBtnBmpNonActive , 0));
                }
            }
        
        if (needDimButton)
            {
            button->SetDimmed(ETrue);
            button->SetActive(EFalse);
            }
        else
            {
            button->SetDimmed(EFalse);
            }
        }
    
	if ( iActionStyle == ERangeBarUsingIndicatorStyle )
	    {
		for(TInt n = 0; n < iIndicators.Count(); n++)
			{
			for( TInt m = n+1; m < iIndicators.Count(); m++ )
				{
				if( iIndicators[n]->iPosIdx == iIndicators[m]->iPosIdx )
					{
					TBool nDimmed = iIndicators[n]->iIndicator->IsDimmed();
					TBool mDimmed = iIndicators[m]->iIndicator->IsDimmed();
					
					if ( nDimmed != mDimmed )
						{
						iIndicators[n]->iIndicator->Hide(nDimmed);
						iIndicators[m]->iIndicator->Hide(mDimmed);
						}
					}
				}
			}
	    }
    //RefreshUI();
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::SetPermittedRange
// set permitted ranges
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::SetPermittedRange(const TInt aRangeId, TBool aPermit)
    {
    // this function has take consideration of mixed range multimodebutton
    // aRangeId is detailed range here
    TBool needDimButton = EFalse;
    TBool findButton = EFalse;
    TInt buttonIdx = 0;

    for (TInt ii = 0; ii < iIndicators.Count(); ii++)
        {
        CAknFepCtrlCommonButton* button = iIndicators[ii]->iIndicator;
        
        if ((iIndicators[ii]->iCtrlType == EButtonTypeMultiMode) && 
            (iIndicators[ii]->iRangeType == ERangeTypeMixed))
            {
            // mixed multi button
            if (static_cast<CAknFepCtrlMultiModeButton*>(button)->IsGivenRangeSupported(aRangeId))
                {
                findButton = ETrue; 
                buttonIdx = ii;

                static_cast<CAknFepCtrlMultiModeButton*>(button)->SetPermittedRange(aRangeId, 
                                                                                    aPermit);

                RArray<TInt> rangeList;

                TRAP_IGNORE(static_cast<CAknFepCtrlMultiModeButton*>(button)->GetRangeListL(
                                                                                 rangeList));

                if ( (!aPermit) && (rangeList.Count() == 1) )
                    {
                    needDimButton = ETrue;
                    }

                rangeList.Reset();
                rangeList.Close();
                
                break;
                }
            }
        else
            {
            // normal or single range multi button
            if (iIndicators[ii]->iInputRange == aRangeId)
                {
                findButton = ETrue;
                buttonIdx = ii;

                if (!aPermit)
                    {
                    needDimButton = ETrue;

                    if (iIndicators[ii]->iCtrlType == EButtonTypeMultiMode)
                        {
                        TRAP_IGNORE(
                        static_cast<CAknFepCtrlMultiModeButton*>(button)->SetCurrentModeL(
                                                                    EBtnBmpNonActive , 0));
                        }
                    }
                break;
                }
            }    
        }

    if (findButton)
        {
        CAknFepCtrlCommonButton* button = iIndicators[buttonIdx]->iIndicator;

        if (!needDimButton)
            {
            button->SetDimmed(EFalse);
            }
        else
            {
            button->SetDimmed(ETrue);
            button->SetActive(EFalse);
            }    
        }
    
    //RefreshUI();    
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::SetPermittedCase
// set permitted ranges
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::SetPermittedCase(const TInt aRangeId,
                                                    const TInt aRealCaseId,
                                                    TBool aPermit)
    {
    // aRangeId is detailed range here
    for (TInt ii = 0; ii < iIndicators.Count(); ii++)
        {
        if (((iIndicators[ii]->iCtrlType == EButtonTypeMultiMode) && 
             (iIndicators[ii]->iInputRange == aRangeId)) ||
            ((iIndicators[ii]->iCtrlType == EButtonTypeMultiMode) && 
             (iIndicators[ii]->iRangeType == ERangeTypeMixed) && 
             (static_cast<CAknFepCtrlMultiModeButton*>(iIndicators[ii]->iIndicator)->IsGivenRangeSupported(aRangeId))))
            { 
            // single range multimode button OR mixed range multimode button
        	static_cast<CAknFepCtrlMultiModeButton*>(iIndicators[ii]->iIndicator)->SetPermittedModes(aRangeId, aRealCaseId, aPermit);
        	break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::HideRange
// hide given range
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::HideRange(const TInt aRange)
    {
    // mixed range multimode button is not considerred here
    for (TInt ii = 0; ii < iIndicators.Count(); ++ii)
        {
        if ( ( iIndicators[ ii ] ) && 
             ( iIndicators[ ii ]->iInputRange == aRange ) &&
             ( !iIndicators[ ii ]->iIsHide))
            {                    
            iIndicators[ii]->iIndicator->Hide(ETrue);            
            
            iIndicators[ ii ]->iIsHide = ETrue;  

			if( iCellChanged )
				{
				UpdateIndicatorRect(EFalse);
				}
			else
				{
		        SetWholeRect();
		        iIsReloadImages = EFalse;
		        //Draw();
		        SetIndicatorRect();					
				}
            
            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::HideRangeUsingIndicatorStyle
// hide given range
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::HideRangeUsingIndicatorStyle(const TInt aRange)
    {
    // mixed range multimode button is not considerred here
	TInt indiIdx = IndicatorIdxByRange(aRange);

	if (indiIdx == KErrNotFound)
	    return;

    HideRangeForHidingIndicatorStyle(indiIdx, ETrue);
    
    UpdateIndicatorRectForUsingIndicatorStyle(EFalse);
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::ShowRange
// show given range
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::ShowRange(const TInt aRange)
    {
    // mixed range multimode button is not considerred here    
    for (TInt ii = 0; ii < iIndicators.Count(); ++ii)
        {
        if ( ( iIndicators[ ii ] ) && 
             ( iIndicators[ ii ]->iInputRange == aRange ) &&
             ( iIndicators[ ii ]->iIsHide))
            {
            iIndicators[ii]->iIndicator->Hide(EFalse);
            iIndicators[ ii ]->iIsHide = EFalse;                

			if( iCellChanged )
				{
				UpdateIndicatorRect(ETrue);
				}
			else
				{
			    SetWholeRect();
			    iIsReloadImages = EFalse;			

		        //Draw();
		        SetIndicatorRect();					
				}

            break;
            }
        }
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::ShowRangeUsingIndicatorStyle
// show given range
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::ShowRangeUsingIndicatorStyle(const TInt aRange)
    {
    // mixed range multimode button is not considerred here
	TInt indiIdx = IndicatorIdxByRange(aRange);

	if (indiIdx == KErrNotFound)
	    return;

    HideRangeForHidingIndicatorStyle(indiIdx, EFalse);

    UpdateIndicatorRectForUsingIndicatorStyle(EFalse);
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::AddRangeL
// add range
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::AddRangeL(TResourceReader& aReader)
    {
    const TInt16 rangeId = aReader.ReadInt16();
    const TInt16 rangeType = aReader.ReadInt16();
    const TInt16 ctrlId = aReader.ReadInt16();
    const TInt16 ctrlType = aReader.ReadInt16(); 
    const TInt32 resId = aReader.ReadInt32();
    const TInt actionStyle = aReader.ReadInt16();
    TBool hide = aReader.ReadInt16();
    TInt posidx = aReader.ReadInt16();

    CAknFepCtrlCommonButton* button = NULL;

    if (ctrlType != EButtonTypeMultiMode)
        {
        // normal button        
       	button = CAknFepCtrlCommonButton::NewLC(UiLayout(), ctrlId,
        									  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
        button->SetResourceId(resId);
        button->ConstructFromResourceL();
        }
    else
        { 
        // Multiple display type button
        button = CAknFepCtrlMultiModeButton::NewLC(UiLayout(), ctrlId,
        									  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive);

        button->SetResourceId(resId);
        button->ConstructFromResourceL();
        }     

    button->AddEventObserver( this );
        
    AddControlL( button );
    CleanupStack::Pop(button); // button
        
    TAknFepRangeIndicator* indicator = new (ELeave) TAknFepRangeIndicator;
    CleanupStack::PushL( indicator );

    indicator->iCtrlType = ctrlType;
    indicator->iInputRange = rangeId;
    indicator->iRangeType = rangeType;
    indicator->iCtrlId = ctrlId;
    //indicator->iIsHide = EFalse;
    indicator->iIsHide = hide;
    if (hide)
        button->Hide(ETrue);
    indicator->iIndicator = button;
    indicator->iActionStyle = actionStyle;
    indicator->iPosIdx = posidx;

    User::LeaveIfError( iIndicators.Append(indicator) );
    CleanupStack::Pop(indicator); // indicator        
    }

// ---------------------------------------------------------------------------
// CAknFepCtrlRangeBar::GetCase
// get mode of given range
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CAknFepCtrlRangeBar::GetCase(TInt aRange) const
    {
    // get current real case of aRange    
    // aRange is detailed range here, for mixed range multimode button, it contained in one range of it
    for (TInt ii = 0; ii < iIndicators.Count(); ++ii)
        {
        if (iIndicators[ii]->iCtrlType == EButtonTypeMultiMode)
            {
            CAknFepCtrlMultiModeButton* button = static_cast<CAknFepCtrlMultiModeButton*>
                                                 (iIndicators[ii]->iIndicator);

            if ( ( (iIndicators[ii]->iRangeType == ERangeTypeMixed) && 
                  (button->IsGivenRangeSupported(aRange)) && 
                  ((button->RangeOfGivenMode(button->CurrentMode()) == aRange)) ) 
                  || (iIndicators[ii]->iInputRange == aRange)) 
                {
                return button->GetRealCaseByMode(button->CurrentMode());
                }
            }
        }

    return KErrNotFound;
    }

// -----------------------------------------------------------------------------
// Implementation of CAknFepCtrlRangeBar::SetFont
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::SetFont(const CFont* aFont)
    {
    iFont = aFont;

    for (TInt ii = 0; ii < iIndicators.Count(); ii++)
        {
        if (iIndicators[ii]->iCtrlType == EButtonTypeMultiMode)
            {
            static_cast<CAknFepCtrlMultiModeButton*>(iIndicators[ii]->iIndicator)->SetFont(iFont);
            
            break; 
            }
        }
    }

// -----------------------------------------------------------------------------
// Implementation of CAknFepCtrlRangeBar::Move
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::Move(const TPoint& aOffset)
    {
    CControlGroup::Move(aOffset);
    
    iIndicatorRect.Move(aOffset);
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlRangeBar::SetEventIdForRange
// set event id when range is clicked
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CAknFepCtrlRangeBar::SetEventIdForRange(TInt aEventId)
    {
    iEventIdForRange = aEventId;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlRangeBar::SetEventIdForCase
// set event id when case is clicked
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CAknFepCtrlRangeBar::SetEventIdForCase(TInt aEventId)
    {
    iEventIdForCase = aEventId;
    }
    

// -----------------------------------------------------------------------------
// CAknFepCtrlRangeBar::ShowAllRanges
// Display all ranges whether it is IsHide
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
EXPORT_C void CAknFepCtrlRangeBar::ShowAllRanges()
    {
    for (TInt ii = 0; ii < iIndicators.Count(); ++ii)
        {
        iIndicators[ii]->iIsHide = EFalse;
        iIndicators[ii]->iIndicator->Hide(EFalse);
        }    

		if( iCellChanged )
			{
			iActionStyle == ERangeBarUsingIndicatorStyle ? 
	        UpdateIndicatorRectForUsingIndicatorStyle(EFalse) :
			UpdateIndicatorRect(EFalse);
			}
		else
			{
	        SetWholeRect();
	        iIsReloadImages = EFalse;
	        //Draw();
	        SetIndicatorRect();					
			}
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlRangeBar::SetWholeRect
// Set rect of whole range bar
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//      
void CAknFepCtrlRangeBar::SetWholeRect()
	{
    if (iStyle == EBtnGroupVertical)
       	{
        SetRect(TRect(iIndicatorRect.iTl, 
                TPoint(iIndicatorRect.iTl.iX + iIndicatorRect.Width(),
                iIndicatorRect.iTl.iY + 
                               iIndicatorRect.Height() * (NumOfControls() - NumOfHidedRange()))));	
       	}
    else
        {
        SetRect(TRect(iIndicatorRect.iTl,
                TPoint(iIndicatorRect.iTl.iX + 
                       iIndicatorRect.Width() * (NumOfControls() - NumOfHidedRange()),
                iIndicatorRect.iTl.iY + iIndicatorRect.Height())));	
        }	
	}

// -----------------------------------------------------------------------------
// CAknFepCtrlRangeBar::NumOfHidedRange
// Get number of hided input ranges
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TInt CAknFepCtrlRangeBar::NumOfHidedRange()
    {
    TInt numOfHidedRange = 0;
    
    for (TInt ii = 0; ii < iIndicators.Count(); ++ii)
        {
        if (iIndicators[ ii ]->iIsHide)
            {
            numOfHidedRange++;
            }
        }
        
    return numOfHidedRange;    
    }


// -----------------------------------------------------------------------------
// CAknFepCtrlRangeBar::SetActionStyle
// set action style
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::SetActionStyle(TRangeBarActionStyle aActionStyle)
    {
	iActionStyle = aActionStyle;
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlRangeBar::IndicatorIdxByRange
// get indicator index by range
// -----------------------------------------------------------------------------
//
TInt CAknFepCtrlRangeBar::IndicatorIdxByRange(TInt aRange)
    {
    for (TInt ii = 0; ii < iIndicators.Count(); ++ii)
        {
        if ( IsMixedRange(ii) )
            {
            CAknFepCtrlMultiModeButton* button = static_cast<CAknFepCtrlMultiModeButton*>
                                                 (iIndicators[ii]->iIndicator);
            if ( button->RangeOfGivenMode(button->NextMode()) == aRange ||
                button->RangeOfGivenMode(button->CurrentMode()) == aRange )
                {
                return ii;
                }
            }
        else if (iIndicators[ii]->iInputRange == aRange)
            {
            return ii;
            }
        }
    
    return KErrNotFound;    
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlRangeBar::ActiveRange
// active range
// -----------------------------------------------------------------------------
//
EXPORT_C void CAknFepCtrlRangeBar::ActiveRange(TInt aRange)
    {
    TInt indiIdx = IndicatorIdxByRange(aRange);
    
    if (indiIdx == KErrNotFound)
        return;

    if (iActionStyle == ERangeBarActionStyleHide ||
        (iActionStyle == ERangeBarUsingIndicatorStyle && 
         iIndicators[indiIdx]->iActionStyle == EIndicatorActionStyleHide))
        {
        TBool needChange = EFalse;

        if (!iIndicators[indiIdx]->iIndicator->IsDimmed() && iIndicators[indiIdx]->iIndicator)
            {
            needChange = ETrue;
            iInputRange = iIndicators[indiIdx]->iInputRange;
            SetActiveCtrl(iIndicators[indiIdx]->iIndicator);
            }

        if (needChange)
            {
            if (iActionStyle == ERangeBarActionStyleHide)
                {
	            for (TInt ii = 0; ii < iIndicators.Count(); ++ii)
	                {
	                if (ii == indiIdx)
	                    {
	                    iIndicators[ii]->iIndicator->Hide(ETrue);
	                    iIndicators[ii]->iIsHide = ETrue;
	                    }
	                else if (iIndicators[ii]->iIsHide)
	                    {
	                    iIndicators[ii]->iIsHide = EFalse;
	                    iIndicators[ii]->iIndicator->Hide(EFalse);
	                    }
	                }
				if (iCellChanged )
					{
					UpdateIndicatorRect( EFalse );	
					}
				else
					{
		            SetWholeRect();
		            iIsReloadImages = EFalse;
		            //Draw();
		            SetIndicatorRect();					
					}
                }
            else
                {
            	HideRangeForHidingIndicatorStyle(indiIdx, ETrue);
            	UpdateIndicatorRectForUsingIndicatorStyle(EFalse);
                }
            }
        }
    else 
        {
        // whole range bar's action style is highlight, or it depends on each indicator's
        // action style, which is highlight or just visible
        SetRange(aRange);
        }

	for (TInt ii = 0; ii < iIndicators.Count(); ++ii)
	    {
		TBool highlight = 
		    iIndicators[ii]->iIndicator->IsActive() && !iIndicators[ii]->iIndicator->Hiden();
		iIndicators[ii]->iIndicator->SetHighlight(highlight);
	    }
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlRangeBar::IsMixedRange
// Judge if button in given index has mixed range
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
TBool CAknFepCtrlRangeBar::IsMixedRange(const TInt aIndex)
    {
    if ((aIndex >= 0) && 
        (aIndex <= iIndicators.Count()-1) &&
        (iIndicators[aIndex]->iCtrlType == EButtonTypeMultiMode) && 
        (iIndicators[aIndex]->iRangeType == ERangeTypeMixed))
        {
       	return ETrue;
        }
    
    return EFalse;    
    }

// -----------------------------------------------------------------------------
// CAknFepCtrlRangeBar::ReportEventToObserver
// Report range or case event to observer
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
void CAknFepCtrlRangeBar::ReportEventToObserver(const TInt aEventId, const TInt aEventData)
    {
    TPtrC ptr;
    TInt sendData = aEventData;
    ptr.Set(reinterpret_cast<TText*>(&sendData),sizeof(aEventData));
    
    ReportEvent(aEventId, ptr);
    }

EXPORT_C void CAknFepCtrlRangeBar::SetDispalyCellRectsL( const RArray<TRect>& aRects,
						  					    const RArray<TRect>& aInnerRects )
	{
	iCellChanged = ETrue;
	iCellRects.Close();
	iCellInnerRects.Close();
	for( TInt i = 0; i < aRects.Count(); i++ )
		{
		iCellRects.AppendL( aRects[i] );
		}
		
	for( TInt i = 0; i < aInnerRects.Count(); i++ )
		{
		iCellInnerRects.AppendL( aInnerRects[i] );
		}		

    iActionStyle == ERangeBarUsingIndicatorStyle ? 
    UpdateIndicatorRectForUsingIndicatorStyle(ETrue) :
    UpdateIndicatorRect(ETrue);
	}

void CAknFepCtrlRangeBar::UpdateIndicatorRect( TBool aUpdateImage)
	{
    TInt index = 0;
    for (TInt ii = 0; ii < iIndicators.Count() && iCellRects.Count() > 0; ++ii)
	    {
	    TPoint offset( iIndicatorRect.iTl.iX, iIndicatorRect.iTl.iY );
	    if (!iIndicators[ii]->iIsHide )
	        {
	        CAknFepCtrlCommonButton* button = iIndicators[ii]->iIndicator;
	        if( button )
	            {
	            TInt n = index++;
	            TRect btnRect( iCellRects[n] );
	            TRect btnInnerRect( iCellInnerRects[n] );
	            //btnRect.Move( offset );
	            if (iIndicators[ii]->iCtrlType == EButtonTypeMultiMode)
	                {
	                static_cast<CAknFepCtrlMultiModeButton*>(iIndicators[ii]->iIndicator)->
	                            SizeChanged(btnRect, btnInnerRect, aUpdateImage);
	                }
	            else
	                {             
	                static_cast<CAknFepCtrlCommonButton*>(iIndicators[ii]->iIndicator)->
	                            SizeChanged(btnRect, btnInnerRect, aUpdateImage);
	                }
	            }
	        if( index == iCellRects.Count() )
	        	break;
	        }
	    }
	}

void CAknFepCtrlRangeBar::UpdateIndicatorRectForUsingIndicatorStyle(TBool aUpdateImage)
    {
    for (TInt ii = 0; ii < iIndicators.Count(); ii++)
	    {
		if (/*!iIndicators[ii]->iIsHide && */
		    iIndicators[ii]->iIndicator && 
		    iIndicators[ii]->iPosIdx >= 0 && 
		    iIndicators[ii]->iPosIdx < iCellRects.Count())
		    {			    
            iIndicators[ii]->iIndicator->SizeChanged(iCellRects[iIndicators[ii]->iPosIdx],
                                                     iCellInnerRects[iIndicators[ii]->iPosIdx],
                                                     aUpdateImage);
		    }
	    }
    }

void CAknFepCtrlRangeBar::HideRangeForHidingIndicatorStyle(TInt aIdx, 
                                                           TBool aHide)
    {
    RArray<TInt> permitRangeInxArray;
    TInt maxPos = 0;

    // Get all the permitted range index array
    for (TInt ii = 0; ii < iIndicators.Count(); ii++)
        {
        maxPos = iIndicators[ii]->iPosIdx > maxPos ? iIndicators[ii]->iPosIdx : maxPos;

        if( !(iIndicators[ii]->iIndicator->IsDimmed() ) && iIndicators[ii]->iPosIdx >=0 )
            {
            permitRangeInxArray.Append(ii);
            }
        }
    
    TBool showAllPermittedRange = ETrue;
     
    // compare the position for every permitted range
    if( permitRangeInxArray.Count() == maxPos + 1 )
        {
        for(TInt n = 0; n < permitRangeInxArray.Count(); n++)
	        {
		    for( TInt m = n+1; m < permitRangeInxArray.Count(); m++ )
			    {
			    if( iIndicators[permitRangeInxArray[n]]->iPosIdx == 
			        iIndicators[permitRangeInxArray[m]]->iPosIdx )
			        {
			        showAllPermittedRange = EFalse;
				    break;
			        }
			    }
	        }
        }
    else
    	{
    	showAllPermittedRange = EFalse;
    	}
    	
    permitRangeInxArray.Reset();
    permitRangeInxArray.Close();     

    if( !showAllPermittedRange ) // hide style
        {
	    if (iIndicators[aIdx]->iIsHide != aHide && 
	        iIndicators[aIdx]->iIndicator)
	        {
			TBool canShowNext = ETrue;
			
	        for (TInt ii = 0; ii < iIndicators.Count(); ii++)
	            {
	            if (ii != aIdx && iIndicators[ii]->iPosIdx == iIndicators[aIdx]->iPosIdx)
	                {
	                if ( iIndicators[ii]->iIndicator->IsDimmed() )
	                    {
	                    canShowNext = EFalse;
	                    }
				    else
					    {
						iIndicators[ii]->iIndicator->Hide(!aHide);
	                    iIndicators[ii]->iIsHide = !aHide;
						}

	                if (aHide)
	                    break;
	                }
	            }
	            
	        iIndicators[aIdx]->iIndicator->Hide( canShowNext && aHide);
	        iIndicators[aIdx]->iIsHide = canShowNext && aHide;
	        }
        }
    else
        {
        // Set the range for special state
        for (TInt ii = 0; ii < iIndicators.Count(); ii++)
            {
            if( iIndicators[ii]->iIndicator->IsDimmed() )
                {
                iIndicators[ii]->iIndicator->Hide(ETrue);
                iIndicators[ii]->iIsHide = ETrue;
                }
            else
                {
                iIndicators[ii]->iIndicator->Hide(EFalse);
                iIndicators[ii]->iIsHide = EFalse;
                }
            }

        }
    }
// End Of File
