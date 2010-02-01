/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies). 
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
*       Interface through which the FEP can use UI components
*
*/















#ifndef AKNFEPUIINTERFACE_H
#define AKNFEPUIINTERFACE_H

#include <e32base.h>

class MDesC16Array;
class CGulIcon;
class CAknEdwinState;
class MEikCommandObserver;
class MAknFepPenSupportInterface;
class CPlainText;
class MAknEditingStateIndicator;
class MAknFepManagerInterface;
class CAknFepIndicator;
class MObjectProvider;
class MAknFepUICtrlContainerChinese;
class MAknFepUICtrlContainerJapanese;
class CAknFepUIManagerJapanese;
class CAknFepUiInterfaceMenuPane;
class CAknFepUiInterfaceMenuBar;
class MAknFepUiInterfaceMenuObserver;
class CAknFepUiInterfaceStylusPopUpMenu;
#ifdef RD_INTELLIGENT_TEXT_INPUT
// Predictive QWERTY (XT9) changes ---->
class MAknFepUiWordPopupContent;
class MAknFepCandidatePopupCallback;
class MAknFepCandidatePopup;
// Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT

class CAknFepUIInterface : public CBase
	{
public:
	enum
		{
		ELeftSoftkeyIndex                           =0,
        ERightSoftkeyIndex                          =2,
        EMiddleSoftkeyIndex                         =3,        
		};

public:
	IMPORT_C static CAknFepUIInterface* NewL();
	IMPORT_C ~CAknFepUIInterface();
	
public:	// menu abstraction
	virtual void SetFepMenuObserver(MAknFepUiInterfaceMenuObserver* aObserver) = 0;
	virtual CAknFepUiInterfaceMenuBar* EditorStateMenuBar(CAknEdwinState* aEditorState) = 0;
	virtual CAknFepUiInterfaceMenuBar* NewMenuBarL(MAknFepUiInterfaceMenuObserver* aObserver, TInt aHotKeyResourceId=0, TInt aMenuTitleResourceId=0) = 0;
	virtual CAknFepUiInterfaceMenuBar* ObjectProviderMenuBar(MObjectProvider* aObjectProvider) = 0;

public:	// stylus pop-up menu abstraction
    virtual CAknFepUiInterfaceStylusPopUpMenu* NewStylusPopUpMenuL(MAknFepUiInterfaceMenuObserver* aObserver, const TPoint& aPoint) = 0;
	
public: // indicator abstraction
	virtual CAknFepIndicator* NewIndicatorL() = 0;
	
public:	// use case abstraction
	virtual void LaunchConfirmationNoteL(TInt aResourceId) = 0;
	virtual void LaunchHelpTextQueryL(TInt aResourceId) = 0;
	virtual TInt LaunchCangJieOptionDlgL(TInt aResourceId, TInt& aLevel, MDesC16Array* aItems) = 0;
	virtual TInt LaunchListPopupL(TInt aResourceId, TInt& aIndex, TInt aInitialIndex, MDesC16Array* aItems, CArrayPtr<CGulIcon>* aIcons) = 0;
	virtual void UpdateSoftkeysL(TInt aResourceId, CAknEdwinState* aEditorState, MEikCommandObserver* aObserver, TInt aInputMode) = 0;
	virtual TBool SoftkeysExist() = 0;
	virtual TInt SoftkeyCommandId(TInt aPosition) = 0;
	virtual void DeleteSoftkeys() = 0;
	virtual void DrawSoftkeysNow() = 0;
	virtual void CreateSoftkeys(TInt aResourceId, MEikCommandObserver* aObserver) = 0;
	virtual TBool UpdateSoftkeyLabelL(TInt aPosition, TInt aCommandId, TInt aTextResId) = 0;
	virtual TInt CharMapDialogL(TInt aCharCase, TDes& aSpecialChars, TInt aCharSetResourceId, TBool aLockNumericKeys, TBool aShowPictographsFirst, TBool aShowNoPictographs, TBool aShowAnotherTable, TInt aAfterSctChars, TInt aResId, TBool aDisableRecentItemRow = EFalse) = 0;
	virtual TInt TextQueryDialogL(TDes& aDataText, CAknEdwinState& aEditorState, TInt aEditorFlag, TInt aResId) = 0;
	virtual CPlainText* PlainText(CAknEdwinState* aEditorState) = 0;
	virtual void UpdateEditorContext(MAknEditingStateIndicator& aIndicator, MAknFepManagerInterface& aManager) = 0;

    virtual void TouchPaneSetFepPenSupportInterface(MAknFepPenSupportInterface* aFepPenSupportInterface) = 0;
    virtual void TouchPaneAllowInputMethodActivation(TBool aValue) = 0;
    virtual void TouchPaneRefreshL() = 0;
    virtual void TouchPaneSetInputMethodIconActivated(TBool aActivated) = 0;

	virtual TBool IsDisplayDataQuery(TBool aIsFepAwareTextEditor) = 0;

public:	// APAC abstraction
	virtual MAknFepUICtrlContainerChinese* NewAknFepUICtrlContainerChineseL() = 0;
	virtual MAknFepUICtrlContainerJapanese* NewAknFepUICtrlContainerJapaneseL(CAknFepUIManagerJapanese* aUiMng) = 0;
	virtual void JapaneseKutenQueryDialogL(TInt aResId, TInt& aCharCode, TInt aTone = 0) = 0;

#ifdef RD_INTELLIGENT_TEXT_INPUT
// Predictive QWERTY (XT9) changes ---->
public:	// candidates words pop-up abstraction
	virtual MAknFepUiWordPopupContent* CreateWordPopupContentL() = 0;
	virtual MAknFepCandidatePopup* NewCandidatePopupL(MAknFepCandidatePopupCallback& aOwner) = 0;
	virtual void LaunchPredictiveSettingDialogL(TInt aDialogResId, 
																							TInt aMenuResId, 
																							TInt aConfirmationQueryResId,
																							TInt aTitlePaneResId) = 0;
#ifdef __USER_DICTIONARY_EDITING__	
	virtual void LaunchUserDictEditDialogL() = 0;
#endif //__USER_DICTIONARY_EDITING__

#ifdef FF_DUAL_LANGUAGE_SUPPORT
	virtual void LaunchWritingLanguageSettingDialogL(TInt aDialogResId, TInt aMenuResId, TInt aTitlePaneResId) = 0;
#endif //FF_DUAL_LANGUAGE_SUPPORT
// Predictive QWERTY (XT9) changes <----
#endif //RD_INTELLIGENT_TEXT_INPUT
	virtual void DeleteDialogs() = 0;
private:
	TUid iEcomDtorUid;		// used when implementation is loaded by ECom
	};

#endif
