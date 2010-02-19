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
* Description:  peninput VKB data manager
*
*/

#ifndef PENINPUTVKBKRWINDOW_H
#define PENINPUTVKBKRWINDOW_H

#include <e32std.h>
#include <e32base.h>
#include <AknLayoutDef.h>
#include <peninputlayoutwindowext.h>

class CAknFepCtrlLabel;
class CAknFepCtrlRangeBar;
class CPeninputGenericVkbLafMgr;
class CPeninputCommonChoiceList;
class MPeninputUiStateMgr;

class CPeninputVkbKrWindow : public CPeninputLayoutWindowExt
    {
public:

    static CPeninputVkbKrWindow* NewLC(
        CFepUiLayout* aUiLayout, 
        MPeninputLayoutContext* aLayoutContext,
        MPeninputUiStateMgr* aUiStateMgr);

    static CPeninputVkbKrWindow* NewL(
        CFepUiLayout* aUiLayout, 
        MPeninputLayoutContext* aLayoutContext,
        MPeninputUiStateMgr* aUiStateMgr);

    virtual ~CPeninputVkbKrWindow();

private:

    CPeninputVkbKrWindow(
        CFepUiLayout* aUiLayout, 
        MPeninputLayoutContext* aLayoutContext,
        MPeninputUiStateMgr* aUiStateMgr);

    void ConstructL();

private:

    void HandleControlEvent(
        TInt aEventType,
        CFepUiBaseCtrl* aCtrl, 
        const TDesC& aEventData );

    /**
     * Get id of window config resource
     *
     * @since S60 v3.2
     * @return The id of window config resource
     */
    virtual TInt GetWindowConfigResId();

    /**
     * Get id of window resource
     *
     * @since S60 v3.2
     * @return The id of window resource
     */ 
    virtual TInt GetWindowResId();
    
    /**
     * Get numeric keymapping resource id
     *
     * @since S60 v3.2
     * @return The id of numeric keymapping
     */ 
    virtual TInt GetNumKeymappingResId();

    /**
     * Get file name of window config resource
     *
     * @since S60 v3.2
     * @param aLangID The id of language
     * @return The file name of window config resource
     */ 
    virtual const TDesC& 
        GetWindowConfigResFileName( TInt aLangID );

    /**
     * Get file name of window resource
     *
     * @since S60 v3.2
     * @return The file name of window resource
     */ 
    virtual const TDesC& GetWindowResFileName();
    
    /**
     * Change unit size
     *
     * @since S60 v3.2
     * @return The rect of whole window
     */
    virtual const TRect ChangeUnitSize();
  
    /**
     * Change size of client area
     *
     * @since S60 v3.2
     * @param aLeftTopPoint The left top point of client area
     * @return None
     */
    virtual void ChangeClientSize(); 
    
    /**
     * Construct all controls specified in resource(implementation will be 
     * postponed in subclass)
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void CreateAllControlsL();
    
    /**
     * Set control fonts. Fonts info comes from resource
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void SetControlsFont();
  
    /**
     * Pop up choice list
     *
     * @since S60 v3.2
     * @return None
     */
    virtual void PopupChoiceList();
  
    /**
     * Re-organize all controls in the assigned client area layout
     *
     * @since S60 v3.2
     * @param aClientLayoutId The id of client area layout
     * @return None
     */
    virtual void ReorganizeControls( TInt aClientLayoutId, TBool aNeedReset );

    /**
     * Do when case changed
     *
     * @since S60 v3.2
     * @param aNewCase The new case
     * @return None
     */
    virtual void DoCaseChange( TInt aNewCase );

    
    virtual TBool CreateCustomControlL( TInt16 aControlId, TInt32 aImageId );  
    
    virtual void  AddCustomControlGroupL(CFepUiBaseCtrl* aCtrl);  

private:

    void SetVkbLayoutSize();
    
    void ConstructFromResourceL();
    
    void SetRangeBarSizeL(TInt x, TInt y);

    CAknFepCtrlEventButton* AddButtonL( 
        const TInt aControlId, 
        const TInt aEventId, 
        const TInt aResId, 
        const TInt aUnicode = 0, 
        const TBool aIsRepeat = EFalse );
    
    void ReorgnizeTitleBar();
    
    void ReorgnizeTableUpDown();
    
    void ResetAccentButton();
    
    void UpdateICFTextL();
    
    static TInt BackgroundTaskL(TAny* aPtr);
    
    void PopupWindowSizeChange();
    
    void DoIdleConstructL();
    
    void GetPopupWndInfoFromResL( 
        TResourceReader aRes,
        const TRect& aRect );
    
    void PopupSwitchWindowL();
    
    TBool IsSymbolRange();
    
    void FakeClientLayoutUpdateL(const TInt& aVkbLayoutId=0);
    
public:
    
    void SetPromptTextL(TUint8* aData);
    
    void SetSwitchlistSecretFlag(TBool aSecret);
    
    void DimArrowKeys( TBool aDimArrow );
    
    void ShowBubble(TInt aShow);
    
    void PreProcessRange(const TInt& aRange);
    
    void UpdateLafData();

private:

    MPeninputUiStateMgr* iUiStateMgr;
    
    TAknWindowLineLayout iBubbleSize;
    
    TAknTextLineLayout iBubbleTextLayout;
    
    CAknFepCtrlRangeBar* iRangeBar;
    
    CPeninputVkbCtrlExt* iVkbCtrl;

    CPeninputGenericVkbLafMgr* iLafMgr;
    
    CAknFepCtrlDragButton* iMoveButton;
    
    CAknFepCtrlRepeatButton* iArrowLeftButton;
    
    CAknFepCtrlRepeatButton* iArrowRightButton;
    
    CAknFepCtrlRepeatButton* iTableUpButton;
    
    CAknFepCtrlRepeatButton* iTableDownButton;
    
    CAknFepCtrlEventButton* iTablePageLabel;
    
    TBool iFirstConstruct;
    
    CIdle *iIdle;
    
    TBool iPopupSet;
    
    TBool iPopupInited;
    
    TRect iAppWndRect;
    
    TRect iPopupItemRect;
    
    CPeninputCommonChoiceList* iPopupWnd;
    
    CFbsBitmap* iPopupWndBgImg;
    
    CFbsBitmap* iPopupWndFocusImg;
    
    };

#endif // PENINPUTVKBKRWINDOW_H
