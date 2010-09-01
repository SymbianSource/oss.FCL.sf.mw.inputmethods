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
* Description:           
*       Provides the TAknFepInputStateEntryStroke methods.
*
*/












#include <fontids.hrh>
#include <PtiEngine.h>

#include "AknFepUiInputStateEntryStroke.h"

#include "AknFepUIManagerStateInterface.h"

#include "AknFepUiCtrlContainerChinese.h"
#include "AknFepUICtrlInputPane.h"
#include "AknFepUICtrlCandidatePane.h"
#include "AknFepManagerUIInterface.h"       //MAknFepManagerUIInterface
#include "AknFepManager.h"

const TInt KMaxCandidateCountFromCore = 6;

TAknFepInputStateEntryStroke::TAknFepInputStateEntryStroke(
                                MAknFepUIManagerStateInterface* aOwner,
                                MAknFepUICtrlContainerChinese* aUIContainer)
    :TAknFepInputStateEntryZhuyinStrokeBase(aOwner, aUIContainer)
    {
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    uiContainer->SetLayout(MAknFepUICtrlContainerChinese::ELayoutInput);
    uiContainer->CandidatePane()->SelectFirst(); 
    uiContainer->InputPane()->SetOverrideFontId(KScreenFontUidStrokes);
    uiContainer->CandidatePane()->ShowCandidateOrdinals(EFalse);

    CPtiEngine* ptiengine = iOwner->PtiEngine();
    if ( EPtiEngineStroke != ptiengine->InputMode() )
    {
    ptiengine->SetInputMode(EPtiEngineStroke);
    }
    ptiengine->SetCandidatePageLength(MAknFepUICtrlContainerChinese::ELayoutInput);
    }

void TAknFepInputStateEntryStroke::HandleCommandL(TInt /*aCommandId*/)
	{	
    RefreshUI();	
 	}

TBool TAknFepInputStateEntryStroke::HandleKeyL(TInt aKey, TKeyPressLength aLength)
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlCandidatePane* candidatePane = uiContainer->CandidatePane();

    if(aKey == EKeyBackspace)
        {
        if (iOwner->PtiEngine()->DeleteKeyPress().Length())
            {
            RefreshUI();
            SetInputPaneToneMark();
            }
        else
            {
            iOwner->FepMan()->TryCloseUiL();
            if (aLength == ELongKeyPress)
                {
                iOwner->FepMan()->SetLongClearAfterCloseUI(ETrue);
                }
            }
        }
    else if((aLength == EShortKeyPress) && iOwner->IsValidChineseInputKey(aKey))
        {
    	iOwner->FepMan()->SetCcpuFlag(CAknFepManager::ECcpuStateIgnoreStarUp);        
       
        TInt stringBeforeLength(0);
        TInt stringAfterLength(0);
        
        stringBeforeLength = ptiengine->GetPhoneticSpelling(1).Length();
        stringAfterLength = ptiengine->AppendKeyPress((TPtiKey)aKey).Length();
        
        if (stringBeforeLength != stringAfterLength)
            {
            TBool multiplePages = ptiengine->MoreCandidatePages();
            TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
            TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

            uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
            uiContainer->ShowVerticalScrollArrows(multiplePages);
            SetInputPaneToneMark();
            RefreshUI();
            }
        else
            {            
            iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
            }
        }
    else if(aKey == EKeyDownArrow)
        {
        if (iOwner->PtiEngine()->CandidatePage().Length() == 
            KMaxCandidateCountFromCore)
            {
            ptiengine->NextCandidatePage();
            RefreshUI();
            }
        }
    else if(aKey == EKeyUpArrow)
        {
        if (ptiengine->NumberOfCandidates() != 0)
            {
            ptiengine->PreviousCandidatePage();
            RefreshUI();
            }
        }   
    else if(aKey == EKeyOK || aKey == EKeyRightArrow || aKey == EKeyLeftArrow)
        {
        iOwner->ChangeState(ECandidate);
        return EFalse;
        }
        
    else if(aKey == EPtiKeyStar)
    	{
    	return ETrue;
    	}
    	
    else if (!iOwner->IsValidChineseInputKey(aKey))
        {
        iOwner->FepMan()->PlaySound(EAvkonSIDErrorTone);
        }
        
    return ETrue;
    }

void TAknFepInputStateEntryStroke::RefreshUI()
    {
    CPtiEngine* ptiengine = iOwner->PtiEngine();
    MAknFepUICtrlContainerChinese* uiContainer = UIContainer();
    MAknFepUICtrlInputPane* inputPane = uiContainer->InputPane();
 
    TBool multiplePages = ptiengine->MoreCandidatePages();
    TInt bufLength = iOwner->PtiEngine()->CandidatePage().Length();
    TBool showHorizontalScrollArrows = multiplePages || bufLength>1;

    uiContainer->ShowHorizontalScrollArrows(showHorizontalScrollArrows); 
    uiContainer->ShowVerticalScrollArrows(multiplePages);

    // Universal layout 
    TPoint baseLine = TPoint(0,0);
    TInt height = 0;
    TInt ascent = 0;
    
    TRAPD(ret,iOwner->FepMan()->GetScreenCoordinatesL(baseLine,height,ascent));
    if (ret == KErrNone)
        {
        uiContainer->SetContainerPosition(baseLine, height);    
        }
    TPtrC candidates = ptiengine->CandidatePage();
    uiContainer->CandidatePane()->SetCandidateBuffer(candidates);
    uiContainer->Enable(ETrue);

    TPtrC ptr = ptiengine->GetPhoneticSpelling(1);
    inputPane->SetText(ptr);
    if (ptiengine->NumberOfCandidates() == 0)
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(EFalse);    
        }
    else
        {
        uiContainer->CandidatePane()->ShowUpScrollArrows(ETrue);    
        }
    
    uiContainer->CandidatePane()->ShowDownScrollArrows
        (bufLength == KMaxCandidateCountFromCore ? ETrue : EFalse);
    uiContainer->CandidatePane()->ShowLeftScrollArrows(EFalse);
    uiContainer->CandidatePane()->ShowRightScrollArrows(bufLength ? ETrue : EFalse);
    }

// End of file
