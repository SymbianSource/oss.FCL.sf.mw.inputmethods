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
#ifndef PENINPUTHWRARABICWINDOW_H_
#define PENINPUTHWRARABICWINDOW_H_

// System includes
#include <peninputlayoutwindow.h>

// Forward declarations
class CFepUiBaseCtrl;
class CAknFepCtrlRangeBar;
class CAknFepCtrlEventButton;
class CTransparentHwrWndExt;
class CPeninputLayoutInputmodelChoice;
class CFepLayoutMultiLineIcf;
class CFepCtrlDropdownList;
class CPeninputSyncBitmapRotator;

// Constants
const TInt KMaxFileLength = 80;

/**
 *  Hwr layout window class
 *  This class define methods to deal with window layout and 
 *  handle events sent to this window
 *
 *  @lib peninputgenerichwr.lib
 *  @since S60 v5.0
 */
class CPeninputHwrBxAbWnd : public CPeninputLayoutWindow
    {

public:

    /**
     * Two-phased constructor
     *
     * @since S60 v5.0
     * @param aUiLayout The plugin ui layout
     * @param aLayoutContext The hwr layout context
     * @return The pointer to CPeninputHwrBxAbWnd object
     */
    static CPeninputHwrBxAbWnd* NewL( CFepUiLayout* aUiLayout, 
        MPeninputLayoutContext* aLayoutContext );
    
    /**
     * Destructor
     *
     * @since S60 v5.0
     * @return None
     */
    virtual ~CPeninputHwrBxAbWnd();
    
    /**
     * Do when case changed
     *
     * @since S60 v5.0
     * @param aNewCase The new case
     * @return None
     */
    void DoCaseChange( TInt aNewCase );

// From base class CPeninputLayoutWindow

    /**
     * From CPeninputLayoutWindow
     * Handle control event 
     *
     * @since S60 v5.0
     * @param aEventType The event type
     * @param aCtrl The control who sends the event
     * @param aEventData The event data
     * @return None
     */
    void HandleControlEvent( TInt aEventType,
                             CFepUiBaseCtrl* aCtrl, 
                             const TDesC& aEventData );
   
    /**
     * From CPeninputLayoutWindow
     * Get id of window config resource
     *
     * @since S60 v5.0
     * @return The id of window config resource
     */
    TInt GetWindowConfigResId();

    /**
     * From CPeninputLayoutWindow
     * Get id of window resource
     *
     * @since S60 v5.0
     * @return The id of window resource
     */ 
    TInt GetWindowResId();

    /**
     * From CPeninputLayoutWindow
     * Get file name of window config resource
     *
     * @since S60 v5.0
     * @param aLangID The id of language
     * @return The file name of window config resource
     */ 
    const TDesC& GetWindowConfigResFileName( TInt aLangID );

    /**
     * From CPeninputLayoutWindow
     * Get file name of window resource
     *
     * @since S60 v5.0
     * @return The file name of window resource
     */ 
    const TDesC& GetWindowResFileName();

    /**
     * From CPeninputLayoutWindow
     * Re-organize all controls in the assigned client area layout
     *
     * @since S60 v5.0
     * @param aClientLayoutId The id of client area layout
     * @return None
     */
    void ReorganizeControls( TInt aClientLayoutId, TBool aNeedReset=ETrue );
    
    /**
     * From CPeninputLayoutWindow
     * Change unit size
     *
     * @since S60 v5.0
     * @return The window rect
     */
    const TRect ChangeUnitSize();
  
    /**
     * From CPeninputLayoutWindow
     * Change size of client area
     *
     * @since S60 v5.0
     * @param aLeftTopPoint The left top point of client area
     * @return None
     */
    void ChangeClientSize();  
    
    /**
     * From CPeninputLayoutWindow
     * Construct all controls specified in resource
     *
     * @since S60 v5.0
     * @return None
     */
    void CreateAllControlsL();
    
    /**
     * From CPeninputLayoutWindow
     * Set control fonts. Fonts info comes from resource
     *
     * @since S60 v5.0
     * @return None
     */
    void SetControlsFont();
  
    /**
     * From CPeninputLayoutWindow
     * Pop up choice list
     *
     * @since S60 v5.0
     * @return None
     */
    void PopupChoiceList(); 
        
    /** 
     * ConstructFromResourceL load settings from resource.
     * Before calling this function, the resource id should be set
     *
     * @since S60 v5.0
     * @return None
     */
    void ConstructFromResourceL();
    
    void OnLanguageChange();
    
    void Move(const TPoint& aOffset);
    
    /**
     * Handle EEventHwrStrokeStarted event. The message is reported each time
     * pen down in the HWR box area
     *
     * @since S60 v5.0
     * @return None
     */
    void OnStrokeStarted();

    /**
     * Handle EEventHwrStrokeFinished event. The message is reported when
     * StrokeTimer times out
     *
     * @since S60 v5.0
     * @return None
     */
    void OnStrokeFinished();

    /**
     * Handle EEventHwrCharacterTimerOut event. The message is reported when
     * CharacterTimer times out
     *
     * @since S60 v5.0
     * @return None
     */
    void OnStrokeCharacterTimerOut();

    /**
     * Handle EEventHwrStrokeCanceled event. The message is reported when
     * manually cancel writing
     *
     * @since S60 v5.0
     * @return None
     */
    void OnStrokeCanceled();

    CTransparentHwrWndExt* getCTransparentHwrWndExt() {return iHwBox;}	
    void GetCandidate(TInt aIndex, TDes& aCharCode) const;	
    void CancelWriting();
    void ClearAndCloseDropdownList();
    void ResetAndShowDropdownList();   
    void HandleBackspaceKeyEvent();
    TBool IsAddingSpace(HBufC* aRes);
protected:

    /**
     * Constructor
     *
     * @since S60 v5.0
     * @param aUiLayout The plugin ui layout
     * @param aLayoutContext The hwr layout context
     * @return None
     */
    CPeninputHwrBxAbWnd( CFepUiLayout* aUiLayout, 
                               MPeninputLayoutContext* aLayoutContext );

    /**
     * Second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */
    void ConstructL();
    
    virtual TBool IsMultiLineIcf();

    virtual TBool IsCanChangeRange(TInt aRange);
    
    virtual void ChangeToPreviousRange(TInt aRange);    
private:

    /**
     * Add range bar into client area
     *
     * @since S60 v5.0
     * @return None
     */
    void AddRangeBarL();

    /**
     * Add button to client area
     *
     * @since S60 v5.0
     * @param aControlId The control id
     * @param aEventId The control's event id
     * @param aResId The control's resource id
     * @param aUnicode The control's unicode
     * @param aIsRepeat The TBool indicating whether the 
     *        control is repeated or not
     * @return The pointer to CAknFepCtrlEventButton object
     */    
    CAknFepCtrlEventButton* AddButtonL( const TInt aControlId,
                                        const TInt aEventId,
                                        const TInt aResId,
                                        const TInt aUnicode = 0,
                                        const TBool aIsRepeat = EFalse );

    /**
     * Add hwr box to client area
     *
     * @since S60 v5.0
     * @return None
     */
    void AddHwBoxL();

    /**
     * Add dropdown list to  client area
     *
     * @since S60 v5.0
     * @return None
     */
    void AddDropdownListL(TBool aLandscapeStyle);

    /**
     * Read unit size, shadow size of hwr window
     *
     * @since S60 v3.2
     * @return None
     */
    void ReadLafInfo(TBool aLandscapeStyle);
    
    /**
     * Set frame color of hwr write box
     *
     * @since S60 v5.0
     * @return None
     */
    void SetHwBoxFrameBackColor();


    /**
     * Draw guide line
     *
     * @since S60 v5.0
     * @return None
     */
    void DrawGuideLine();
    
    /**
     * Calculate guide line position
     *
     * @since S60 v5.0
     * @return None
     */
    void CalculateGuideLinePos();
    
    /**
     * Popup input mode switch list
     *
     * @since S60 v5.0
     * @return None
     */
    void PopupSwitchWindow();
    
    void OnDeActivate();
    
    void SubmitRecognitionResult( HBufC* aRes );
    void AddBackspaceButtonL();
    void MirrorBackspaceButtonResL();
private:

    /**
     * The handwriting box
     * Not own
     */
    CTransparentHwrWndExt* iHwBox;
    
    /**
     * The range bar
     * Not own
     */
    CAknFepCtrlRangeBar* iRangeBar;
    
    /**
     * The language switch button
     * Not own
     */
    CAknFepCtrlEventButton* iLangSwitchBtn;
    
    /**
     * The resource file name
     */
    TBuf<KMaxFileLength> iResourceFilename;
            
    /**
     * The array to hold stroke points
     */
    RArray<TPoint> iStrokeArray;
    
    /**
     * The array to hold recognize result
     */
    RPointerArray<HBufC> iRecogResult;
    
    HBufC* iLastResult;
    
    /**
     * Normal transparency factor of hwr write box
     */
    TInt iNormalTransFactor;

    /**
     * Writting transparency factor of hwr write box
     */
    TInt iWriteTransFactor;
    
    TBool iDisableCaseChange;
    TInt  iCachedCase;

    /**
     * The unit width
     */
    TInt iUnitWidth;

    /**
     * The unit height
     */
    TInt iUnitHeight;
    
    /**
     * Size of top left shadow corner
     */
    TSize iShadowTl;

    /**
     * Size of bottom right shadow corner
     */
    TSize iShadowBr;
    
    TRect iHwrWndRect;
    
    TPoint iGuideLineTopTl;
    TPoint iGuideLineTopBr;
    TPoint iGuideLineBottomTl;
    TPoint iGuideLineBottomBr;
    TBool iGuideLineSet;
    TBool iCharacterStart;

    CPeninputLayoutInputmodelChoice* iModeSwitchChoice;
    
    /**
     * The drop down list control (Own)
     */    
    CFepCtrlDropdownList* iDropdownList;
    /**
     * Bitmap rotator
     */  
    CPeninputSyncBitmapRotator* iBmpRotator;
    TBool iBackspaceMirrored;
    };

#endif /*PENINPUTHWRARABICWINDOW_H_*/
