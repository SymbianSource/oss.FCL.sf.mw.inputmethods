/*
* Copyright (c) 2002-2004 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:            Provides the
*                TAknFepUiInputStateJapanesePredictiveBase methods.
*
*/












// INCLUDE FILES
#include "AknFepUiInputStateJapanesePredictiveBase.h"
#include "AknFepUIManagerStateInterface.h"  // MAknFepUIManagerStateInterface
#include "AknFepManagerUIInterface.h"       // MAknFepManagerUIInterface
#include "AknFepUICtrlContainerJapanese.h"  // MAknFepUICtrlContainerJapanese
#include "AknFepUICtrlCandidatePaneInterface.h"
                                        // MAknFepUICtrlCandidatePaneInterface
#include "AknFepGlobalEnums.h"
#include "AknFepPanic.h"                                       
#include <PtiDefs.h>                    // keys
#include <PtiEngine.h>
#include <PtiCompositionDataIF.h>       // MPtiEngineCompositionDataInterface
#include <aknfep.rsg>


// CONSTANTS

/**
 *  TAknFepUiInputStateJapanesePredictiveBase class.
 * 
 */
//============================ MEMBER FUNCTIONS ==============================

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapanesePredictiveBase::
//                      TAknFepUiInputStateJapanesePredictiveBase
// C++ default Constructor
// ---------------------------------------------------------------------------
//
TAknFepUiInputStateJapanesePredictiveBase::
    TAknFepUiInputStateJapanesePredictiveBase(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerJapanese* aUIContainer)
    :TAknFepUiInputStateJapaneseBase(aOwner, aUIContainer)
    {
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapanesePredictiveBase::CloseUI
// Closing UI
// ---------------------------------------------------------------------------
//
void TAknFepUiInputStateJapanesePredictiveBase::CloseUI()
    {
    MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
    MAknFepUICtrlCandidatePaneInterface* predictivePane =
        uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                   EJapanesePredictiveCandidatePane);
    // Closeing candidate pop-up list
    predictivePane->HideWindow();
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapanesePredictiveBase::HandleNumericLongKeyL
// Handling Numeric long keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapanesePredictiveBase::HandleNumericLongKeyL(TInt /*aKey*/)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    // Not handling long keypress events if exist transitory input string
    TPtrC text = data->ConvertingString();
    if (text.Length() > 1)
        {
        return ETrue;
        }
    // Long keypress events are handled by HandleNumericLongKeyL of
    // CompletionState
    iOwner->ChangeState(EMultiCompletion);
    return EFalse;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapanesePredictiveBase::HandleNaviKeyL
// Handling Navigate keypress event
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapanesePredictiveBase::
  HandleNaviKeyL(TInt aKey, TKeyPressLength /*aLength*/)
    {
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
    MAknFepUICtrlCandidatePaneInterface* predictivePane =
        uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                   EJapanesePredictiveCandidatePane);
    TBool ret = EFalse;
	if (predictivePane->CurrentItemIndexOfCandidate() > -1)
		{
	    TAknFepUICtrlScroll direction = EScrollNone;

	    // move hightlight of candidate in cadidate pop-up list
	    switch(aKey)
	        {
	        case EKeyLeftArrow:
	        case EStdKeyLeftArrow:      /* 0x0e */
	            {
	            direction = EScrollLeft;
	            }
	            break;
	        case EKeyRightArrow:
	        case EStdKeyRightArrow:     /* 0x0f */
	            {
	            direction = EScrollRight;
	            }
	            break;
	        case EKeyDownArrow:
	        case EStdKeyDownArrow:      /* 0x11 */
	            {
	            direction = EScrollDown;
	            }
	            break;
	        case EKeyUpArrow:
	        case EStdKeyUpArrow:        /* 0x10 */
	            {
	            direction = EScrollUp;
	            }
	            break;
	        default:
	            break;
	        }
	    
	    predictivePane->MoveItem(direction);
	    TInt index = predictivePane->CurrentItemIndexOfCandidate();
	    ptiEngine->HandleCommandL(EPtiCommandUserActionSetCurrentIndexOfCandidates ,&index);
	    ret = ETrue;
		}
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapanesePredictiveBase::UpdateInlineEditingCharacterL
// Update inline editing string
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapanesePredictiveBase::UpdateInlineEditingCharacterL()
    {
    TBool ret = EFalse;
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    // Check ComposirionData
    __ASSERT_DEBUG(data->CompletedString().Length() <= 0,
        RDebug::Print(_L("In TAknFepUiInputStateJapanesePredictiveBase::UpdateInlineEditingCharacterL,\nINVALID DATA: CompletedString. CompletedString was not OperaOperated!")));
    
    TPtrC text = data->ConvertingString();

    if (text.Length() > 0)
        {
        // Update of inline editing characters
        fepMan->UpdateInlineEditL(text, text.Length());
        ret = ETrue;
        }
    else
        {
        // Check ConvertingString length
        __ASSERT_DEBUG(text.Length() == 0,
            RDebug::Print(
               _L("In TAknFepUiInputStateJapanesePredictiveBase::UpdateInlineEditingCharacterL,\nINVALID DATA: ConvertingString. Roll back to initial State!")));
        
        MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
        MAknFepUICtrlCandidatePaneInterface* predictivePane =
            uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                       EJapanesePredictiveCandidatePane);
        ptiEngine->ClearCurrentWord();
        fepMan->CancelInlineEdit();
        predictivePane->HideWindow();
        iOwner->ChangeState(EInitial);
        
        }
    
    return ret;
    }

// ---------------------------------------------------------------------------
// TAknFepUiInputStateJapanesePredictiveBase::UpdateCandidateListL
// 
// ---------------------------------------------------------------------------
//
TBool TAknFepUiInputStateJapanesePredictiveBase::UpdateCandidateListL(TInt aIndex /*= 0*/)
    {
    MAknFepManagerUIInterface* fepMan = iOwner->FepMan();
    CPtiEngine* ptiEngine = iOwner->PtiEngine();
    CDesCArrayFlat* candidateArray = iOwner->CandidateArray();
    MAknFepUICtrlContainerJapanese* uicontainer = UIContainer();
    MAknFepUICtrlCandidatePaneInterface* predictivePane =
        uicontainer->CandidatePane(MAknFepUICtrlContainerJapanese::
                                   EJapanesePredictiveCandidatePane);
    const MPtiEngineCompositionDataInterface* data = ptiEngine->CompositionData();
    // Get coordinate of input area
    TPoint  leftSideOfBaseLine;
    TInt    height;
    TInt    ascent;
    TInt    documentOffset = data->CursorPosition() > 0 ? data->CursorPosition() : 0;
    
    fepMan->GetScreenCoordinatesL(leftSideOfBaseLine,
                                  height,
                                  ascent,
                                  documentOffset);
    // show candidate pop-up list
    predictivePane->SetWindowPosition(leftSideOfBaseLine, height, ascent);            
    predictivePane->ShowCandidateListL(candidateArray, aIndex, iCbaResourceID);
    return ETrue;
    }
// End of file
