/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
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
*
*/
#ifndef __PEN_UI_WINDOW_CONTROL__
#define __PEN_UI_WINDOW_CONTROL__

#include <coecntrl.h>
#include <AknIconUtils.h>
#include <AknsUtils.h>
#include <AknsDrawUtils.h> 

#ifndef FIX_FOR_NGA
#define FIX_FOR_NGA
#endif

class RAknUiServer;
class CPenUiWndCtrl;
class CPenUiHwnWnd;

struct TCommonBgCtrlSubItem
    {
    TRect iRect;    
    TAknsItemID iFrameID;
    TAknsItemID iCenterID;
    TBool iIsShow;      
    };
class CPenUiWndCtrl;

class CCursorWindow : public CCoeControl
    {
public: // Functions from base classes.

    CCursorWindow(CPenUiWndCtrl* aParent);
    //void ConstructL(CCoeControl* aParent);
    /**
     * From CCoeControl    
     * Function where control's drawing is done.
     *
     * @param aRect Control's area.
     */
    void Draw( const TRect &aRect ) const;
    
    void SetCursorVisible(TBool aFlag);

private:
    TBool iCursorVisible;
    CPenUiWndCtrl* iParent;
    };
	
class CPenUiPopWnd : public CCoeControl
    {
public: // Constructors and destructor
    static CPenUiPopWnd* NewL(RWindowGroup& aWndGroup,CFbsBitmap * aBitmap,
                                    CPenUiWndCtrl* aCtrl);
    /**
     * Destructor.
     */
    ~CPenUiPopWnd();
    
    /** 
     * show popup window
     * @param aRectInScreen the rect of popup in screen coord-sys
     * @param aRectInLayout the rect of popup in layout(extended) coord-sys
     * @return none
     */
    void PopUp(const TRect& aRectInScreen, const TRect aRectInLayout );
    
    void PopUp(TInt aPriority);
    void Invalidate(const TRect& aRect);
    void Close();
public: // Functions from base classes.

    /**
     * From CCoeControl    
     * Function where control's drawing is done.
     *
     * @param aRect Control's area.
     */
    void Draw( const TRect &aRect ) const;
private:
    /**
     * C++ default constructor.
     * @param aAppUi AppUi of Eikon server.
     */
    CPenUiPopWnd(RWindowGroup& aWndGroup,CFbsBitmap * aBitmap,
                                                      CPenUiWndCtrl* aCtrl);
    
    /**
     * Two-phased constructor.
     */
    void ConstructL();
    
    
private: // Data
    RWindowGroup& iWndGroup;
    CFbsBitmap * iBitmap;
    //RWindowTreeNode& iBackgroundWnd;    
    RAknUiServer* iAknUiSrv;    
    CPenUiWndCtrl* iBackgroundCtrl;
    
    /**
     * the position and size of image in extended layout coord-sys
     */
    TRect iRectInLayout;
    };

class CAknIncallBubble;
class CPenUiWndCtrl : public CCoeControl
    {
public:

    enum EditorType
        {
        ENoremalEditor,
        EGlobleNotes,
        EGlobleNotesWithEditor  
        };
    
public: // Constructors and destructor
    /**
     * C++ default constructor.
     * @param aAppUi AppUi of Eikon server.
     */
    CPenUiWndCtrl(RWindowGroup& aWndGroup,CFbsBitmap * aBitmap);
    
    /**
     * Two-phased constructor.
     */
    void ConstructL();
    
    /**
     * Destructor.
     */
    ~CPenUiWndCtrl();

public: // New functions
    /**
     * Called from akncapserver appui to run fastswap.
     */

    /**
     * Show pen UI with priority.
     */
    void ShowPenUiL(TBool aDimmed = EFalse);
    
    /**
     * Show popup window
     * @param aRectInScreen the rect of popup, in screen coord-sys
     * @param aRectInLayout the rect of popup, in layout(extended) coord-sys
     * @param aLayoutTrimRect the rect of old layout, in layout(extended) coord-sys
     * @return none
     */
    void ShowPopup( const TRect& aRectInScreen, const TRect& aRectInLayout, 
                    const TRect& aLayoutTrimRect ); 

    /**
     * Close the pop up window.
     */
    void ClosePopup();
        
    /**
     * Called from akncapserver appui to close fastswap.
     */
    void ClosePenUi(TBool aResChanging = EFalse);    
    
    void Invalidate(const TRect& aRect,TBool aFullUpdate);
    void OnDeactivate();
    void OnActivate(EditorType aType);


    void Fade();
    
    
    TBool IsDimmed();    

    TInt WndPriority();
    
    void DimPenUiForMenu(TInt aPriority);
    void DimInternalPenUiForMenu(TInt aPriority);
    void DimPenUiForMenu();
    TInt GetWndPriority();
    void SetResourceChange(TBool aResourceChange);
    void UpdateCursor(TBool aOnFlag,const CFbsBitmap* aCursorBmp,const TRect& aPos);
    void SetPopupArea(const TRect& aRect, TBool aFlag);
    void UpdateICFArea(const CFbsBitmap* aBmp,const TPoint& aPos);
    void UpdateChangedArea(const CFbsBitmap* aBmp,const TRect& aRect,TBool aFlag);
    void UpdateBubble(const CFbsBitmap* aBmp,const CFbsBitmap* aMaskBmp,const TRect& aPos,TBool aFlag);
    void Clean();
    
    void HandleNGASpecificSignal(TInt aEventType, const TDesC& aEventData);
    void LiftUpPriority();
public: // Functions from base classes.

    /**
     * From CCoeControl    
     * Function where control's drawing is done.
     *
     * @param aRect Control's area.
     */
    void Draw( const TRect &aRect ) const;
    
public:
    /**
     * Auto refresh timer callback, refresh pen ui. (for NGA)
     */
    void RefreshUI();

protected:
    CCoeControl* ComponentControl(TInt) const;
    TInt CountComponentControls() const;
	    
private:

    /**
     * when pen ui is activated, e.g. after screen saver, restore system fading status 
     * and set non-fading for pen ui
     */
    void RestoreSystemFadeStatus();
    
    /**
     * set non-fading feature for pen ui window when pen ui is open to be shown
     */
    void ChangeSysteFadeStatus();


    /**
     * From CCoeControl.
     * Key event handling
     *
     * @return TKeyResponse  Indicates whether or not the key event was used by this control.
     * @param aKeyEvent      Key event.
     * @param aType          Type of key event.
     */
    TKeyResponse OfferKeyEventL( const TKeyEvent& aKeyEvent, TEventCode aType );
    
    
    /**
     * Get the AppUid of WinsowGroupName
     * 
     * @return TUid          Indicate the Id of WindowGroupNameApp
     * @param  aWS           Indicate RWsSession.
     * @param  aWGId         Id of focus window group.
     */
    TUid WindowGroupNameAppUidL( RWsSession& aWS,TInt aWGId  );
    
    /**
     * Restart auto refresh timer. (for NGA)
     */
    void RestartRefreshTimer();
    
    /**
     * stop auto refresh timer. (for NGA)
     */
    void StopRefreshTimer();
    
    void DrawBkground(CWindowGc& aGc,const TRect& aRect) const;
    void DrawFrame( CWindowGc& aGc,const TRect& aRect,TAknsItemID aFrameID,TAknsItemID aCenterID ) const;
    
    void DrawCursor(CWindowGc& aGc) const;
    void DrawBubbleAsBackground(CFbsBitGc* aGc, CFbsBitmap* aBmp, const TRect& aRect);

private: // Data
    RWindowGroup& iWndGroup;
    CFbsBitmap * iBitmap;

    // whether system fading status has been changed?
    TBool iChangeSystemFadeStatus;

    RAknUiServer* iAknUiSrv;
    
    TInt iFocusWndGrpPriority;
    TInt iFocusWndGrpPos;
    TInt iFocusWgHandle;
    
    TInt iPriority;
    TInt iPreEditorPriority;
    CAknIncallBubble* iIncallBubble;
    CPenUiPopWnd* iPopupWnd;
    
    TBool iShowPopup;
    TRect iInvalidateRect;
    TBool iResourceChange;
    //TBool iUiLayoutChange;
    TBool iInGlobalEditorState;
    TRect iLayoutClipRect;

    CFbsBitmap * iCursorBmp;
    CFbsBitmap* iICFBmp;
    CFbsBitmap* iChangedBmp;
    RPointerArray<CFbsBitmap> iBubblesArea;
    RPointerArray<CFbsBitmap> iBubblesMaskArea;
    RArray<TRect> iBubblesPos;
    
    TPoint iCursorPos;
    TPoint iIcfPos;
    TPoint iChangedPos;
    TRect iIcfRect;
    TRect iCursorRect;
    CIdle *iIdle;
    TRect iRedrawRect;
    TInt iTouchCount;
    
    TBool iCursorVisible;
    
    RRegion iPopRegion;

    TBool iBackground;
    TAknsItemID iFrameID;
    TAknsItemID iCenterID;
    
    /**
     * Auto refresh timer
     * own
     */
    CPeriodic* iAutoRefreshTimer;

    /**
     * The sub items of the background 
     */
    RArray<TCommonBgCtrlSubItem> *iSubItems; //not own
    
    TBool iNotUpdating;
    CCursorWindow* iCursorWnd;
    
friend class CCursorWindow;
    };
class CInternalBkCtrl : public CCoeControl
    {
public: // Constructors and destructor
    /**
     * C++ default constructor.
     * @param aAppUi AppUi of Eikon server.
     */
    CInternalBkCtrl(RWindowGroup& aWndGroup);
    
    /**
     * Two-phased constructor.
     */
    void ConstructL();
    
    /**
     * Destructor.
     */
    

public: // New functions
    
//    void Invalidate(const TRect& aRect,TBool aFullUpdate);
public: // Functions from base classes.

    /**
     * From CCoeControl    
     * Function where control's drawing is done.
     *
     * @param aRect Control's area.
     */
  //  void Draw( const TRect &aRect ) const;

//    void SizeChanged();   
//    void CreateListBox(); 
private: // Data
    RWindowGroup& iWndGroup;
    CFbsBitmap * iBitmap;
    };    
#endif    
