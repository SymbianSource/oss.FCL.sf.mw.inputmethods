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
* Description:  this class is used to test functions in aknfep uiinterface api
*
*/


#ifndef C_TESTCAKNFEPUIINTERFACEIMPLEMENT_H
#define C_TESTCAKNFEPUIINTERFACEIMPLEMENT_H

#include <aknfepuiinterface.h>

/**
 * CTestCAknFepUIInterfaceImplement for testing CAknFepUIInterface
 */

class CTestCAknFepUIInterfaceImplement : public CAknFepUIInterface
    {
public:
    
    /**
     *  Constructor
     */
    CTestCAknFepUIInterfaceImplement() {}
    
    /**
     *  Constructor
     */
    void NewParentL(){ iParent = CAknFepUIInterface::NewL(); }
    
    /**
     *  Deconstructor
     */
    ~CTestCAknFepUIInterfaceImplement() { if ( iParent ) delete iParent; }
    
public:
    
    /**
     * inherit from CAknFepUIInterface
     */
    void SetFepMenuObserver( MAknFepUiInterfaceMenuObserver* /*aObserver */) ;
    
    /**
     * inherit from CAknFepUIInterface
     */
    CAknFepUiInterfaceMenuBar* EditorStateMenuBar( 
        CAknEdwinState* /* aEditorState*/ );
    
    /**
     * inherit from CAknFepUIInterface
     */
    CAknFepUiInterfaceMenuBar* NewMenuBarL( 
        MAknFepUiInterfaceMenuObserver* /*aObserver*/, TInt /*aHotKeyResourceId=0*/, 
        TInt /*aMenuTitleResourceId=0*/ );
    
    /**
     * inherit from CAknFepUIInterface
     */
    CAknFepUiInterfaceMenuBar* ObjectProviderMenuBar( 
        MObjectProvider* /*aObjectProvider*/ );
    
    /**
     * inherit from CAknFepUIInterface
     */
    CAknFepUiInterfaceStylusPopUpMenu* NewStylusPopUpMenuL(
        MAknFepUiInterfaceMenuObserver* /*aObserver*/, const TPoint& /* aPoint*/ );

    /**
     * inherit from CAknFepUIInterface
     */
    CAknFepIndicator* NewIndicatorL();

    /**
     * inherit from CAknFepUIInterface
     */
    void LaunchConfirmationNoteL( TInt /*aResourceId*/ ); 
    
    /**
     * inherit from CAknFepUIInterface
     */
    void LaunchHelpTextQueryL( TInt /*aResourceId*/ );
    
    /**
     * inherit from CAknFepUIInterface
     */
    TInt LaunchCangJieOptionDlgL( 
        TInt /*aResourceId*/, TInt& /*aLevel*/, MDesC16Array* /*aItems*/ );
    
    /**
     * inherit from CAknFepUIInterface
     */
    TInt LaunchListPopupL( TInt /*aResourceId*/, TInt& /*aIndex*/,
                                             TInt /*aInitialIndex*/, 
                                             MDesC16Array* /*aItems*/, 
                                             CArrayPtr<CGulIcon>* /*aIcons*/ );

    /**
     * inherit from CAknFepUIInterface
     */
    void UpdateSoftkeysL( TInt /*aResourceId*/, CAknEdwinState* /*aEditorState*/, 
                                            MEikCommandObserver* /*aObserver*/, 
                                            TInt /*aInputMode*/ ) ;
    
    /**
     * inherit from CAknFepUIInterface
     */
    TBool SoftkeysExist();

    /**
     * inherit from CAknFepUIInterface
     */
    TInt SoftkeyCommandId( TInt /*aPosition*/ );
    
    /**
     * inherit from CAknFepUIInterface
     */
    void DeleteSoftkeys();
    
    /**
     * inherit from CAknFepUIInterface
     */
    void DrawSoftkeysNow() ;

    /**
     * inherit from CAknFepUIInterface
     */
    void CreateSoftkeys( TInt /*aResourceId*/, MEikCommandObserver* /*aObserver*/ ) ;

    /**
     * inherit from CAknFepUIInterface
     */
    TBool UpdateSoftkeyLabelL( 
        TInt /*aPosition*/, TInt /*aCommandId*/, TInt /*aTextResId*/ ) ;
    
    /**
     * inherit from CAknFepUIInterface
     */
    TInt CharMapDialogL(TInt aCharCase, 
                        TDes& aSpecialChars, 
                        TInt aCharSetResourceId, 
                        TBool aLockNumericKeys, 
                        TBool aShowPictographsFirst, 
                        TBool aShowNoPictographs, 
                        TBool aShowAnotherTable, 
                        TInt aAfterSctChars, 
                        TInt aResId, 
                        TBool aDisableRecentItemRow = EFalse); 
    /**
     * inherit from CAknFepUIInterface
     */
    TInt TextQueryDialogL( TDes& /*aDataText*/, 
                           CAknEdwinState& /*aEditorState*/, 
                           TInt /*aEditorFlag*/, 
                           TInt /*aResId*/ );
    
    /**
     * inherit from CAknFepUIInterface
     */
    CPlainText* PlainText( CAknEdwinState* /*aEditorState*/ ) ;
    
    /**
     * inherit from CAknFepUIInterface
     */
    void UpdateEditorContext( MAknEditingStateIndicator& /*aIndicator*/,
                              MAknFepManagerInterface& /*aManager*/ );
    
    /**
     * inherit from CAknFepUIInterface
     */
    void TouchPaneSetFepPenSupportInterface( 
        MAknFepPenSupportInterface* /*aFepPenSupportInterface*/ );
    
    /**
     * inherit from CAknFepUIInterface
     */
    void TouchPaneAllowInputMethodActivation( TBool /*aValue*/ );
    
    /**
     * inherit from CAknFepUIInterface
     */
    void TouchPaneRefreshL();
    
    /**
     * inherit from CAknFepUIInterface
     */
    void TouchPaneSetInputMethodIconActivated( TBool /*aActivated*/ );

    /**
     * inherit from CAknFepUIInterface
     */
    TBool IsDisplayDataQuery( TBool /*aIsFepAwareTextEditor*/ ) ;
    
    /**
     * inherit from CAknFepUIInterface
     */
    MAknFepUICtrlContainerChinese* NewAknFepUICtrlContainerChineseL() ;
    
    /**
     * inherit from CAknFepUIInterface
     */
    MAknFepUICtrlContainerJapanese* NewAknFepUICtrlContainerJapaneseL(
            CAknFepUIManagerJapanese* /*aUiMng*/ );

    /**
     * inherit from CAknFepUIInterface
     */
    void JapaneseKutenQueryDialogL( 
        TInt /*aResId*/, TInt& /*aCharCode*/, TInt /*aTone*/ ) ;

#ifdef RD_INTELLIGENT_TEXT_INPUT
    /**
     * inherit from CAknFepUIInterface
     */
    MAknFepUiWordPopupContent* CreateWordPopupContentL();

    /**
     * inherit from CAknFepUIInterface
     */
    MAknFepCandidatePopup* NewCandidatePopupL(MAknFepCandidatePopupCallback& aOwner);

    /**
     * inherit from CAknFepUIInterface
     */
    void LaunchPredictiveSettingDialogL( TInt /*aDialogResId*/, 
                                         TInt /*aMenuResId*/,
                                         TInt /*aConfirmationQueryResId*/,
                                         TInt /*aTitlePaneResId*/ );

    /**
     * inherit from CAknFepUIInterface
     */
    void LaunchUserDictEditDialogL() ;
 
#ifdef __DUAL_LANGUAGE_SUPPORT__
    /**
     * inherit from CAknFepUIInterface
     */
    void LaunchWritingLanguageSettingDialogL( TInt /*aMenuResId*/ );
#endif //__DUAL_LANGUAGE_SUPPORT__
    
#endif //RD_INTELLIGENT_TEXT_INPUT
    void DeleteDialogs();    
private:
    /*
     * Test CAknFepUIInterface's function
     * */
    CAknFepUIInterface* iParent;
    };

#endif /*C_TESTCAKNFEPUIINTERFACEIMPLEMENT_H*/

// End of File



