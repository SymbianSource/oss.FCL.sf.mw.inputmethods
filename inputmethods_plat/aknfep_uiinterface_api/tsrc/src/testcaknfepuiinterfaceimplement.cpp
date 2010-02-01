/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  test functions in aknfep uiinterface api
*
*/


#include "testcaknfepuiinterfaceimplement.h"

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::SetFepMenuObserver( 
    MAknFepUiInterfaceMenuObserver* /*aObserver*/ )
    {
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
CAknFepUiInterfaceMenuBar* CTestCAknFepUIInterfaceImplement::EditorStateMenuBar( 
    CAknEdwinState* /*aEditorState*/ )
    {
        return NULL;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
CAknFepUiInterfaceMenuBar* CTestCAknFepUIInterfaceImplement::NewMenuBarL( 
    MAknFepUiInterfaceMenuObserver* /*aObserver*/, TInt /*aHotKeyResourceId*/ , 
    TInt /*aMenuTitleResourceId*/  )
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
CAknFepUiInterfaceMenuBar* CTestCAknFepUIInterfaceImplement::ObjectProviderMenuBar( 
    MObjectProvider* /*aObjectProvider*/ )
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
CAknFepUiInterfaceStylusPopUpMenu* CTestCAknFepUIInterfaceImplement::NewStylusPopUpMenuL( 
    MAknFepUiInterfaceMenuObserver* /*aObserver*/, const TPoint& /*aPoint*/ )
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
CAknFepIndicator* CTestCAknFepUIInterfaceImplement::NewIndicatorL()
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::LaunchConfirmationNoteL( TInt /*aResourceId*/ )
    {
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::LaunchHelpTextQueryL( TInt /*aResourceId*/ )
    {
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
TInt CTestCAknFepUIInterfaceImplement::LaunchCangJieOptionDlgL( 
    TInt /*aResourceId*/, TInt& /*aLevel*/, MDesC16Array* /*aItems*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
TInt CTestCAknFepUIInterfaceImplement::LaunchListPopupL( 
    TInt /*aResourceId*/, TInt& /*aIndex*/, TInt /*aInitialIndex*/, 
    MDesC16Array* /*aItems*/, CArrayPtr<CGulIcon>* /*aIcons*/ )
    {
    return KErrNone;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::UpdateSoftkeysL( 
    TInt /*aResourceId*/, CAknEdwinState* /*aEditorState*/, 
    MEikCommandObserver* /*aObserver*/, TInt /*aInputMode*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
TBool CTestCAknFepUIInterfaceImplement::SoftkeysExist()
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
TInt CTestCAknFepUIInterfaceImplement::SoftkeyCommandId( TInt /*aPosition*/ )
    {       
    return KErrNone;     
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::DeleteSoftkeys() 
    {
    
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::DrawSoftkeysNow() 
    {
    
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::CreateSoftkeys( 
    TInt /*aResourceId*/, MEikCommandObserver* /*aObserver*/ ) 
    {
    
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
TBool CTestCAknFepUIInterfaceImplement::UpdateSoftkeyLabelL( 
    TInt /*aPosition*/, TInt /*aCommandId*/, TInt /*aTextResId*/ ) 
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
TInt CTestCAknFepUIInterfaceImplement::CharMapDialogL(TInt /*aCharCase*/, 
                                        TDes& /*aSpecialChars*/, 
                                        TInt /*aCharSetResourceId*/, 
                                        TBool /*aLockNumericKeys*/, 
                                        TBool /*aShowPictographsFirst*/, 
                                        TBool /*aShowNoPictographs*/, 
                                        TBool /*aShowAnotherTable*/, 
                                        TInt /*aAfterSctChars*/, 
                                        TInt /*aResId*/, 
                                        TBool /*aDisableRecentItemRow*/)
    {
    return KErrNone; 
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
TInt CTestCAknFepUIInterfaceImplement::TextQueryDialogL( TDes& /*aDataText*/, 
                                                         CAknEdwinState& /*aEditorState*/, 
                                                         TInt /*aEditorFlag*/, 
                                                         TInt /*aResId*/ ) 
    {
    return KErrNone;  
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
CPlainText* CTestCAknFepUIInterfaceImplement::PlainText(
    CAknEdwinState* /*aEditorState*/ ) 
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::UpdateEditorContext( 
    MAknEditingStateIndicator& /*aIndicator*/, MAknFepManagerInterface& /*aManager*/ ) 
    {
    
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::TouchPaneSetFepPenSupportInterface(
    MAknFepPenSupportInterface* /*aFepPenSupportInterface*/ )
    {
    
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::TouchPaneAllowInputMethodActivation( 
    TBool /*aValue*/ ) 
    {
    
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::TouchPaneRefreshL() 
    {
    
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::TouchPaneSetInputMethodIconActivated( 
    TBool /*aActivated*/ )
    {
   
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
TBool CTestCAknFepUIInterfaceImplement::IsDisplayDataQuery( 
    TBool /*aIsFepAwareTextEditor*/ ) 
    {
    return ETrue;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
MAknFepUICtrlContainerChinese* CTestCAknFepUIInterfaceImplement
                                         ::NewAknFepUICtrlContainerChineseL() 
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
MAknFepUICtrlContainerJapanese* CTestCAknFepUIInterfaceImplement
    ::NewAknFepUICtrlContainerJapaneseL( CAknFepUIManagerJapanese* /*aUiMng*/ ) 
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::JapaneseKutenQueryDialogL( 
    TInt /*aResId*/, TInt& /*aCharCode*/, TInt /*aTone*/ ) 
    {
    }

#ifdef RD_INTELLIGENT_TEXT_INPUT
// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
MAknFepUiWordPopupContent* CTestCAknFepUIInterfaceImplement
                                                   ::CreateWordPopupContentL() 
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
MAknFepCandidatePopup* CTestCAknFepUIInterfaceImplement::NewCandidatePopupL(
        MAknFepCandidatePopupCallback& /*aOwner*/)
    {
    return NULL;
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::LaunchPredictiveSettingDialogL( 
    TInt /*aDialogResId*/, TInt /*aMenuResId*/, TInt /*aConfirmationQueryResId*/, 
    TInt /*aTitlePaneResId*/ ) 
    {
    }

// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::LaunchUserDictEditDialogL()
    {
    
    }

#ifdef __DUAL_LANGUAGE_SUPPORT__
// -----------------------------------------------------------------------------
// CTestCAknFepUIInterfaceImplement::SetFepMenuObserver
// -----------------------------------------------------------------------------
//
void CTestCAknFepUIInterfaceImplement::LaunchWritingLanguageSettingDialogL(
    TInt /*aMenuResId*/ ) 
    {
    
    }
#endif //__DUAL_LANGUAGE_SUPPORT__
    
#endif //RD_INTELLIGENT_TEXT_INPUT
void CTestCAknFepUIInterfaceImplement::DeleteDialogs()
    {
    }
//  [End of File]


