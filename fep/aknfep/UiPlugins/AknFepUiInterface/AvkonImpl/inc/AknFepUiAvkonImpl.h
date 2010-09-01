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
*       Avkon implementation of the FEP's UI component access
*
*/











#ifndef AKNFEPUIAVKONIMPL_H
#define AKNFEPUIAVKONIMPL_H

#include "aknfepuiinterface.h"
#include "AknFepUiAvkonMenus.h"
#include <eikmobs.h>
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
#include <e32property.h>
#endif
#include "eikbtgpc.h"

class CEikButtonGroupContainer;
class CAknCharMapDialog;
class CEikDialog;
class CAknFepPredictiveSettingDialog;
class CAknListQueryDialog;
class CAknFepTextQueryDialog;

#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
class CRepository;
#endif

class CAknFepUIAvkonImpl : public CAknFepUIInterface, public MEikMenuObserver
	{
private:
    void ConstructL();
public:
	static CAknFepUIAvkonImpl* NewL();
	~CAknFepUIAvkonImpl();
	CAknFepUIAvkonImpl();
public:	// menu abstraction
	void SetFepMenuObserver(MAknFepUiInterfaceMenuObserver* aObserver);
	CAknFepUiInterfaceMenuBar* EditorStateMenuBar(CAknEdwinState* aEditorState);
	CAknFepUiInterfaceMenuBar* NewMenuBarL(MAknFepUiInterfaceMenuObserver* aObserver, TInt aHotKeyResourceId, TInt aMenuTitleResourceId);
	CAknFepUiInterfaceMenuBar* ObjectProviderMenuBar(MObjectProvider* aObjectProvider);

public:	// stylus pop-up menu abstraction
    CAknFepUiInterfaceStylusPopUpMenu* NewStylusPopUpMenuL(MAknFepUiInterfaceMenuObserver* aObserver, const TPoint& aPoint);
	
public: // indicator abstraction
	virtual CAknFepIndicator* NewIndicatorL();
	
public:	// use case abstraction
	void LaunchConfirmationNoteL(TInt aResourceId);
	void LaunchHelpTextQueryL(TInt aResourceId);
	TInt LaunchCangJieOptionDlgL(TInt aResourceId, TInt& aLevel, MDesC16Array* aItems);
	TInt LaunchListPopupL(TInt aResourceId, TInt& aIndex, TInt aInitialIndex, MDesC16Array* aItems, CArrayPtr<CGulIcon>* aIcons);
	void UpdateSoftkeysL(TInt aResourceId, CAknEdwinState* aEditorState, MEikCommandObserver* aObserver, TInt aInputMode);
	TBool SoftkeysExist();
	TInt SoftkeyCommandId(TInt aPosition);
	void DeleteSoftkeys();
	void DrawSoftkeysNow();
	void CreateSoftkeys(TInt aResourceId, MEikCommandObserver* aObserver);
	TBool UpdateSoftkeyLabelL(TInt aPosition, TInt aCommandId, TInt aTextResId);
	TInt CharMapDialogL(TInt aCharCase, TDes& aSpecialChars, TInt aCharSetResourceId, TBool aLockNumericKeys, TBool aShowPictographsFirst, TBool aShowNoPictographs, TBool aShowAnotherTable, TInt aAfterSctChars, TInt aResId, TBool aDisableRecentItemRow );
	TInt TextQueryDialogL(TDes& aDataText, CAknEdwinState& aEditorState, TInt aEditorFlag, TInt aResId);
	CPlainText* PlainText(CAknEdwinState* aEditorState);
	void UpdateEditorContext(MAknEditingStateIndicator& aIndicator, MAknFepManagerInterface& aManager);

    void TouchPaneSetFepPenSupportInterface(MAknFepPenSupportInterface* aFepPenSupportInterface);
    void TouchPaneAllowInputMethodActivation(TBool aValue);
    void TouchPaneRefreshL();
    void TouchPaneSetInputMethodIconActivated(TBool aActivated);

	TBool IsDisplayDataQuery(TBool aIsFepAwareTextEditor);

public:	// APAC abstraction
	MAknFepUICtrlContainerChinese* NewAknFepUICtrlContainerChineseL();
	MAknFepUICtrlContainerJapanese* NewAknFepUICtrlContainerJapaneseL(CAknFepUIManagerJapanese* aUiMng);
	void JapaneseKutenQueryDialogL(TInt aResId, TInt& aCharCode, TInt aTone);
	
#ifdef RD_INTELLIGENT_TEXT_INPUT
// Predictive QWERTY (XT9) changes ---->
public:	// candidate words pop-up abstraction
	MAknFepCandidatePopup* NewCandidatePopupL(MAknFepCandidatePopupCallback& aOwner);

public:	//bubble implmentation
	MAknFepUiWordPopupContent* CreateWordPopupContentL();
	
public: // for predictive setting dialog
	void LaunchPredictiveSettingDialogL(TInt aDialogResId, 
										TInt aMenuResId, 
										TInt aConfirmationQueryResId,
										TInt aTitlePaneResId );
#ifdef FF_DUAL_LANGUAGE_SUPPORT
	void LaunchWritingLanguageSettingDialogL(TInt aDialogResId, TInt aMenuResId, TInt aTitlePaneResId);
#endif //FF_DUAL_LANGUAGE_SUPPORT

#ifdef __USER_DICTIONARY_EDITING__
	//for editing user dictionary
	void LaunchUserDictEditDialogL();
#endif //__USER_DICTIONARY_EDITING__

// Predictive QWERTY (XT9) changes <----	
#endif //RD_INTELLIGENT_TEXT_INPUT
	void DeleteDialogs();
	
private:	// from MEikMenuObserver
	void DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane);
	void DynInitMenuBarL(TInt aResourceId,CEikMenuBar* aMenuBar);
	void SetEmphasis(CCoeControl* aMenuControl,TBool aEmphasis);

private:	// from MEikCommandObserver
	void ProcessCommandL(TInt aCommandId);

private:
    // Delete CBA
	void DeleteCBAL();

    // Create new inside CBA
	void CreateInsideCBAL( CEikButtonGroupContainer::TUse aUse,
	                       CEikButtonGroupContainer::TOrientation aOrientation,
	                       MEikCommandObserver* aCommandObserver,
	                       TInt aResourceId );
	
	// Embeded or not
	TBool CbaEmbeddedInDialog( const TInt& aFlags );

    /**
     *  Show cba of aResourceId using owns cba object(iCba)
     *
     *  @since 3.2
     *  @param aResourceId softkeys resource id.
     *  @return None
     */
    void ShowOwnsSoftkeysL(const TInt aResourceId, MEikCommandObserver* aObserver);

    /**
    * This method is that empty softkeys is shown in case aShown is ETrue.
    * If aShown is EFalse, empty softkeys is erased.
    * @param aShown    If ETrue, empty softkeys is shown.
    */
	void ShowEmptySoftkeysL(TBool aShown, MEikCommandObserver* aObserver, TInt aInputMode);
	
#ifdef RD_INTELLIGENT_TEXT_INPUT	
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
        class CSubscriber : public CActive
            {
        public:
            CSubscriber(TCallBack aCallBack, RProperty& aProperty);
            ~CSubscriber();

        public: // New functions
            void SubscribeL();
            void StopSubscribe();

        private: // from CActive
            void RunL();
            void DoCancel();

        private:
            TCallBack   iCallBack;
            RProperty&  iProperty;
            };
        /**
         * Call back function to handle a change in keyboard layout.
         */ 
        static TInt HandleKeyboardLayoutChangeNotification(TAny* aObj);
        
        void HandleKeyboardLayoutChange();
        
        CSubscriber*    iKeyboardLayoutStatusSubscriber;
        RProperty       iKeyboardLayoutStatusProperty;    
        
        friend class CSubscriber;
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__        
#endif
        
private:
    CEikButtonGroupContainer* iCba;         // own
    CAknFepUIAvkonMenuPaneImpl iTempMenuPane;
    CAknFepUIAvkonMenuBarImpl iTempMenuBar;
    CAknFepUIAvkonMenuBarImpl iEditorMenuBar;
    CAknFepUIAvkonMenuBarImpl iObjectProviderMenuBar;
    MAknFepUiInterfaceMenuObserver* iFepMenuObserver;
    CAknCharMapDialog* iCharMapDialog;
    CEikDialog* iDialog;
    CAknFepPredictiveSettingDialog* iPredictiveSettingDialog;
    CAknListQueryDialog* iListQueryDialog;
    CAknFepTextQueryDialog* iTextQueryDialog;
    // cba container
    CEikButtonGroupContainer* iDialogCba;
    // cba mode
    TBool isCbaEmded;
	};


#endif
