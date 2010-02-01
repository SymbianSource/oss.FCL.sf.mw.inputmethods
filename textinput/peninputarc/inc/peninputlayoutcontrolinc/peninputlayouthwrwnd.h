/*
* Copyright (c) 2005-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of HWR window base and transparent window control
*
*/

#ifndef C_CHWRWNDBASE_H
#define C_CHWRWNDBASE_H

#include <peninputlayoutbasecontrol.h>
#include <centralrepository.h>

class CPeninputPenTraceDecorator;

/**
 *  CHwrWndBase
 *
 *  Base control for all hwr window.
 *
 *  @lib feplayoutcontrol.lib
 *  @since S60 V4.0
 */

class CHwrWndBase : public CFepUiBaseCtrl, public MTimerHandler
                    //public MEventObserver
    {
public:    

    /**
     * Destructor.        
     *
     * @since S60 V4.0        
     */           
    IMPORT_C virtual ~CHwrWndBase();

    /**
     * Clear all strokes
     *
     * @since S60 V4.0
     */                               
    IMPORT_C virtual void ClearStroke(); 
   
    /**
     * Add guiding line
     *
     * @since S60 V4.0
     * @param aStart The start point of the guiding line
     * @param aEnd The end point of the guilding line
     */                                               
    IMPORT_C void AddGuidingLine(const TPoint& aStart, const TPoint& aEnd);                

    /**
     * A callback function. Called when finishing one stroke (pen up).
     *
     * @since S60 V4.0
     * @param aCharacterTimerFlag Flag indicates whether needs to set character timer        
     *                            Default value is ETrue, set the character timer   
     */                               
    virtual void EndStrokeL(TBool aCharacterTimerFlag);        

    /**
     * A callback function. Called when character timer out.
     * @since S60 V4.0
     */                                       
    virtual void EndStrokeAndClearL();            

    /**
     * Set stroke end mark. A stroke end mark indicates end of a stroke
     *
     * @since S60 V4.0
     * @param aPt The stroke end point        
     */                       
    inline void SetStrokeEndMark(const TPoint& aPt);

    /**
     * Set character timer. A character timer defines the maximum writing period.
     * Only those traces within the writing period, are considered belonging 
     * to the same character
     *
     * @since S60 V4.0
     * @param aCharDelay The character timer in micro seconds.
     */                                               
    inline void SetCharacterDelay(TTimeIntervalMicroSeconds32 aCharDelay);

    /**
     * Get the stroke list.
     *
     * @since S60 V4.0
     * @return The stroke list
     */                                                       
    inline const RArray<TPoint>& StrokeList();
    
    /**
     * Set stroke delay. A stroke delay defines the time when to start doing 
     * the recognition before next stroke starts.
     *
     * @since S60 V4.0
     * @param aStrokeDelay
     * @return The stroke list
     */       
    inline void SetStrokeDelay(TTimeIntervalMicroSeconds32 aStrokeDelay);    
            
    //maskcol       
    /**
     * get stroke end mark
     *
     * @since S60 V4.0        
     * @return The stroke end mark
     */        
    inline const TPoint& StrokeEndMark();
        
    /**
     * Do we have new trace
     *
     * @since S60 V4.0        
     * @return New trace flag
     */    
    inline TBool HasNewTrace();        
        
    /**
     * Get frame color
     *
     * Obsolete. Use BorderColor() instead.
     * @since S60 V4.0        
     * @return current frame color
     */        
    inline const TRgb& FrameCol();

    /**
     * Set frame color
     * Obsolete. Use SetBorderColor instead.
     *
     * @since S60 V4.0        
     * @param The new frame color
     */        
    inline void SetFrameCol(const TRgb& aCol);        

    /**
     * Cancel the writing. It will stops the timer and clear all strokes
     *
     * @since S60 V4.0        
     */
    IMPORT_C virtual void CancelCharWriting();        
     
    //from CFepUiBaseCtrl

    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */                       
    IMPORT_C virtual void Draw();

  
    /**
     * From CFepUiBaseCtrl
     * Cancel pointer down event.
     * This will be called when a control has pointer down but pointer up 
     * event happens in other ways.
     *
     * @since S60 V4.0
     * @param aFocusedFlag ETrue if got focused, otherwise EFalse
     */                                
    virtual void CancelPointerDownL();

    
    //From base class MTimerHandler
    /**
     * From MTimerHandler
     * HandleTimerOut. Handling time out event
     *
     * @since S60 V4.0
     * @param aTimeType The timer type
     */                    
    IMPORT_C void HandleTimerOut(TInt aTimeType);           
    
    /**
     * Update the region where other control made dirty.
     *
     * @since S60 V4.0        
     * @return Current dirty rect
     */        
    IMPORT_C void UpdateRegion(CFepUiBaseCtrl* aCtrl);    
    
    /**
     * From CFepUiBaseCtrl
     * OnDeActivate. Called when the layout is going to be hidden
     *
     * @since S60 v4.0
     */        
    IMPORT_C void OnDeActivate();   

    /**
     * From CFepUiBaseCtrl
     * Call back function when layout starts dragging
     *
     * @since S60 V4.0
     */
    IMPORT_C void OnLayoutDraggingStart();

    /**
     * From CFepUiBaseCtrl    
     * Call back function when layout ends dragging
     *
     * @since S60 V4.0
     */
    IMPORT_C void OnLayoutDraggingEnd();
   
protected:
    /**
     * Constructor
     *
     * @since S60 V4.0
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     */       
    IMPORT_C CHwrWndBase(const TRect& aRect,CFepUiLayout* aUiLayout,TInt aControlId);    

    /**
     * Second phrase constructor.
     *
     * @since S60 V4.0
     */
    IMPORT_C void ConstructL();

    /**
     * Get dirty rect
     *
     * @since S60 V4.0        
     * @return Current dirty rect
     */        
    inline TRect& DirtyRect();   

    /**
     * From CFepUiBaseCtrl    
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                                        
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl    
     * Handle pointer move event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);

private:

    /**
     * From CHwrWndBase
     * Draw point
     *
     * @since S60 V4.0
     * @param aPoint The point to be drawn
     * @param aDrawFlag Flag to tell whether to draw or erase stroke
     * @return The affected rect
     */        
    virtual TRect DrawPoint(const TPoint& aPoint,TBool aDrawFlag) =0;
    
    /**
     * From CHwrWndBase
     * Draw line
     *
     * @since S60 V4.0
     * @param aPt1 The start point
     * @param aPt2 The end point
     * @param aDrawFlag Flag to tell whether to draw or erase stroke
     * @return The affected rect
     */        
    virtual TRect DrawLine(const TPoint& aPt1,const TPoint& aPt2,TBool aDrawFlag ) =0;

    /**
     * Draw or erace current stroke
     * @param aDrawFlag ETrue if drawing, otherwise erase the trace
     * @return The minimum rect which current drawing/erasing affects
     */       
    IMPORT_C virtual TRect DrawStroke(TBool aDrawFlag);    

private:   
    /**
     * stoke end mark
     */
    TPoint iStrokeEndMark;
    
    /**
     * Previous point drawed
     */
    TPoint iPtPrev;
    
    /**
     * Character delay for character timer
     */
    TTimeIntervalMicroSeconds32 iCharDelay;
    
    /**
     * stroke delay for stroke timer.
     */
    TTimeIntervalMicroSeconds32 iStrokeDelay;            

    /**
     * Indicates whether we have new pen trace
     */
    TBool iHasNewTrace;
    
    TBool iSubmitted;

    /**
     * The rect which confines the trace.Due to pen size, the dirty rect would
     * sometimes bigger than iRect even trace can't be draw outside of the window. 
     */
    TRect iDirtyRect;

    /**
     * Frame color
     */
     //TRgb iFrameCol;
     
    /**
     * The trace data -- pointer list
     */
    RArray<TPoint> iListOfPoints;

    /**
     * stroke timer. Recognition stargs after timer out
     * Own.
     */
    CLayoutTimer* iStrokeTimer;
    
    /**
     * character time. Character writing finished after timer out.
     * Own.
     */
    CLayoutTimer* iCharacterTimer;
    
    /**
     * Reserved item1
     */
     TInt iReserved1;
     
    /**
     * Reserved item2
     */
     TInt iReserved2;    
    };
//end  of class CHwrWndBase

/**
 *  CTransparentHwrWnd
 *
 *  Transparent hwr window. This could be used for full-screen or non full screen hwr
 *
 *  @lib feplayoutcontrol.lib  
 *  @since S60 V4.0
 */
class CTransparentHwrWnd : public CHwrWndBase
    {
public:

    /**
     * Factory function.        
     *
     * @since S60 V4.0        
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @param aFullScreenFlag Fullscreen window flag. 
     *          ETrue if it's fullscreen window.
     * @return An instance of CTransparentHwrWnd class        
     */                           
    IMPORT_C static CTransparentHwrWnd* NewL(const TRect& aRect,CFepUiLayout* aUiLayout,
                            TInt aControlId,TBool aFullScreenFlag=EFalse);

    /**
     * Enable or disable whether trace can be outside of the writing window.
     *
     * @since S60 V4.0
     * @param aFlag ETrue if trace could be outside of window. Otherwise EFalse
     */       
    IMPORT_C void EnableTraceOutsideWindow(TBool aFlag);

    /**
     * Destructor.        
     *
     * @since S60 V4.0        
     */                                   
    IMPORT_C virtual ~CTransparentHwrWnd();
    
    /**
     * Set transpanrency grade
     *
     * @since S60 V4.0        
     * @param The transparancy factor. Opaque if 0, full transparency if 255    
     */
    IMPORT_C void SetWndTransparencyFactor(TUint8 aFactor);

    //from base class CFepUiBaseCtrl   
    /**
     * From CFepUiBaseCtrl
     * Draw control.
     *
     * @since S60 V4.0
     */               
    IMPORT_C virtual void Draw();

    /**
     * From CFepUiBaseCtrl
     * Update control's region when other control change its displaying status
     *
     * @since S60 V4.0
     * @param aCtrl The control whose hiding status changed. If NULL, recalulate the region.
     * @param bFlag tells control is removed or not. 
     */        
    IMPORT_C virtual void UpdateValidRegion(CFepUiBaseCtrl* aCtrl,TBool bRemoveFlag);

    /**
     * From CHwrWndBase
     * Clear all strokes
     *
     * @since S60 v5.0
     * @return none.
     */     
    IMPORT_C virtual void ClearStroke();

    /**
     * Install a new pen trace decorator.
     * 
     * @since S60 v5.0
     * @param aDecoratorName Specifies the name of new decorator
     * @param aEnable Specifies whether to enable the new decorator after install
     * @return ETrue if success.
     */
    IMPORT_C TBool InstallPenTraceDecoratorL(const TDesC& aDecoratorName, TBool aEnable = ETrue );

    /**
     * Test whether this window support pen trace decoration.
     * 
     * @since S60 v5.0
     * @return ETrue if support.
     */
    IMPORT_C TBool SupportPenTraceDecoration();

    /**
     * Test whether pen trace decoration is activated.
     * 
     * @since S60 v5.0
     * @return ETrue if support.
     */
    IMPORT_C TBool PenTraceDecorationActivated();

    /**
     * Activate/Deactivate pen trace decoration.
     * 
     * @since S60 v5.0
     * @param aActive Specifies whether to activate or deactivate
     * @param aReleaseBuffer Specifies whether to release buffer when deactivate
     * @return none
     */
    IMPORT_C void ActivatePenTraceDecoration(TBool aActive,TBool aReleaseBuffer=EFalse );
    
protected:
    /**
     * Default constructor.        
     *
     * @since S60 V4.0        
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @param aFullScreenFlag Fullscreen window flag. ETrue if it's fullscreen window.
     * @return An instance of CTransparentHwrWnd class        
     */                               
    IMPORT_C CTransparentHwrWnd(const TRect& aRect,CFepUiLayout* aUiLayout,
                                TInt aControlId,TBool aFullScreenFlag);        

    /**
     * From CFepUiBaseCtrl
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                                        
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
        
    /**
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer move event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);

    /**
     * Get transparency factor
     *
     * @since S60 V4.0
     * @return The transparency factor    
     */ 
    inline TUint8 TransparencyFactor();

private:
    /**
     * Set the clip region for drawing. If the region is set, drawing will only happens
     * inside the hwr window.
     *
     * @since S60 V5.0
     */ 
    inline void SetClipRegion();
    
    /**
     * Cancel clip region for drawing. If clip region is not set, some stroke may be drawn
     * outside of hwr window due to the pen size.
     *
     * @since S60 V5.0
     */     
    inline void CancelClipRegion();

    
    //from class CHwrWndBase
    /**
     * From CHwrWndBase
     * Draw point
     *
     * @since S60 V4.0
     * @param aPoint The point to be drawn
     * @param aDrawFlag Flag to tell whether to draw or erase stroke
     * @return The affected rect
     */        
    virtual TRect DrawPoint(const TPoint& aPoint,TBool aDrawFlag = ETrue);
    
    /**
     * From CHwrWndBase
     * Draw line
     *
     * @since S60 V4.0
     * @param aPt1 The start point
     * @param aPt2 The end point
     * @param aDrawFlag Flag to tell whether to draw or erase stroke
     * @return The affected rect
     */        
    virtual TRect DrawLine(const TPoint& aPt1,const TPoint& aPt2,TBool aDrawFlag);

    //from base class CHwrWndBase
    /**
     * From CHwrWndBase
     * Draw stroke
     *
     * @since S60 V4.0
     * @param aDrawFlag Flag to tell whether to draw or erase stroke
     * @return The affected rect
     */
    virtual IMPORT_C TRect DrawStroke(TBool aDrawFlag);
	
    /**      
     * Draw frame
     *
     * @since S60 V4.0
     * @param aFrameRect frame rect
     * @return None
     */	
	virtual void DrawFrame( const TRect& aFrameRect );
private:
    /**
     * mask pen color
     */
    TRgb iMaskPenColor;

    /**
     * a Flag to show whether we use full screen. 
     * In fullscreen mode, we don't draw backgroud
     */
    TBool iIsFullScreen;

    /** 
     * A flag to indicate whether the trace could be draw outside of the hwr window.
     */
    TBool iTraceOutsideWindowFlag;
    
    /**
     * The transparency factor. 255 stands for full transparent and 0 for opaque
     */
    TUint8 iTransparencyFactor;

    /**
     * Pen trace decorator.
     * Own.
     */
    CPeninputPenTraceDecorator* iPenTraceDecorator;
    };
//end of class CTransparentHwrWnd

/**
 *  This is the class definition for the transparent window with guide line
 *
 *  @lib peninputcommonctrls.lib
 *  @since S60 v3.2
 */
class CTransparentHwrWndExt : public CTransparentHwrWnd
    {
    
public:
    /**
     * Pen input guide line style
     */
    enum TGuideLineStyle
        {
        EGuideLineNone               = 0,
        EGuideLineTop                = 1,
        EGuideLineBottom             = 2,
        EGuideLineBoth               = 3
        };
        
    /**
     * Factory function.        
     *
     * @since S60 V4.0        
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @param aFullScreenFlag Fullscreen window flag. 
     *          ETrue if it's fullscreen window.
     * @param aShowGuideLine Show guide line or not
     * @return An instance of CTransparentHwrWndExt class        
     */                           
    IMPORT_C static CTransparentHwrWndExt* NewL( const TRect& aRect,
                                                 CFepUiLayout* aUiLayout,
                                                 TInt aControlId,
                                                 TBool aFullScreenFlag=EFalse, 
                                                 TBool aShowGuideLine=ETrue );


    /**
     * Destructor.
     *
     * @since S60 V4.0
     */
    IMPORT_C virtual ~CTransparentHwrWndExt();

        
    //from class CHwrWndBase
    /**
     * From MPenUiLayoutBase       
     * OnDeActivate. Called by owner when the layout is going to be hidden
     *
     * @since S60 v4.0
     */ 
    IMPORT_C void OnDeActivate();
    
    /**
     * Hide or show guide line
     *
     * @since S60 v3.2
     * @param aHideFlag The hide/show flag of guide line 
     * @return None
     */    
    IMPORT_C void HideGuideLine( TBool aHideFlag );
    
    /**
     * Set guide line style
     *
     * @since S60 v3.2
     * @param aGuideLineStyle The guide line style
     * @return None
     */
    IMPORT_C void SetGuideLineStyle( TInt aGuideLineStyle );
    
    /**
     * Set top guide line position
     *
     * @since S60 v3.2
     * @param aLeftTop The Left Top position of top line
     * @param aRightBottom The Right Bottom position of top line
     * @return None
     */    
    IMPORT_C void SetTopGuideLinePosition(const TPoint& aLeftTop, const TPoint& aRightBottom);

    /**
     * Set bottom guide line position
     *
     * @since S60 v3.2
     * @param aLeftTop The Left Top position of bottom line
     * @param aRightBottom The Right Bottom position of bottom line
     * @return None
     */    
    IMPORT_C void SetBottomGuideLinePosition(const TPoint& aLeftTop, const TPoint& aRightBottom);

    /**
     * Refresh the curent control aera
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C void RefreshUI();

    /**
     * Set guide line color
     *
     * @since S60 v3.2
     * @param aColor The new color of guide line
     * @return None
     */
    IMPORT_C void SetGuideLineColor( const TRgb& aColor );

    /**
     * Set guide line width
     *
     * @since S60 v3.2
     * @param aWidth The new width of guide line
     * @return None
     */
    IMPORT_C void SetGuideLineWidth( TInt aWidth );
        
    //from base class CFepUiBaseCtrl   
    /**
     * From CFepUiBaseCtrl
     * Draw control.
     *
     * @since S60 V4.0
     */               
    IMPORT_C virtual void Draw();
 
    /**
     * From CFepUiBaseCtrl
     * Call back function when layout starts dragging
     *
     * @since S60 V4.0
     */
    IMPORT_C void OnLayoutDraggingStart();
    
    /**
     * Called when character timer out, or write text from the begining again.
     * @since S60 V4.0
     */                                       
    virtual void EndStrokeAndClearL(); 
    
    /**
     * From CFepUiBaseCtrl
     * Cancel pointer down event.
     * This will be called when a control has pointer down but pointer up 
     * event happens in other ways.
     *
     * @since S60 V4.0
     * @param aFocusedFlag ETrue if got focused, otherwise EFalse
     */                                
    virtual void CancelPointerDownL();
    
    /**
     * Enable/Disable fade
     * @since S60 V4.0
     */
    inline void SetEnableFade( TBool aEnable );
    inline void SetFadingSpeed(TInt aFadingSpeed);
    
    IMPORT_C virtual void CancelCharWriting();
protected:
    /**
     * Default constructor.
     *
     * @since S60 V4.0
     * @param aRect The rectangle area for this control
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @param aFullScreenFlag Fullscreen window flag. ETrue if it's fullscreen window.
     * @param aShowGuideLine Show guide line or not     
     * @return An instance of CTransparentHwrWnd class        
     */                               
    IMPORT_C CTransparentHwrWndExt( const TRect& aRect, 
                                    CFepUiLayout* aUiLayout,
                                    TInt aControlId, 
                                    TBool aFullScreenFlag, 
                                    TBool aShowGuideLine );

    /**
     * Second phrase constructor.
     *
     * @since S60 V4.0
     */
    IMPORT_C void ConstructL();                                    
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                                        
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
        
    /**
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer move event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);
    
private: 

    //from class CHwrWndBase
    /**
     * From CHwrWndBase
     * Draw point
     *
     * @since S60 V4.0
     * @param aPoint The point to be drawn
     * @param aDrawFlag Flag to tell whether to draw or erase stroke
     * @return The affected rect
     */        
    virtual TRect DrawPoint(const TPoint& aPoint,TBool aDrawFlag);
    
    /**
     * From CHwrWndBase
     * Draw line
     *
     * @since S60 V4.0
     * @param aPt1 The start point
     * @param aPt2 The end point
     * @param aDrawFlag Flag to tell whether to draw or erase stroke
     * @return The affected rect
     */        
    virtual TRect DrawLine(const TPoint& aPt1,const TPoint& aPt2,TBool aDrawFlag = ETrue);

    //from base class CHwrWndBase
    /**
     * From CHwrWndBase
     * Draw stroke
     *
     * @since S60 V4.0
     * @param aDrawFlag Flag to tell whether to draw or erase stroke
     * @return The affected rect
     */
    IMPORT_C virtual TRect DrawStroke(TBool aDrawFlag);
    
    /**
     * Update hwr window
     *
     * @since S60 V4.0
     * @param aAny The pointer the hwr window
     * @return The error code
     */
    static TInt UpdateShownArea( TAny* aAny );
    
    /**
     * Update hwr window
     *
     * @since S60 V4.0
     * @return None
     */
    void DoUpdateShownArea();

    void DrawGuidingLine();
private:
    /**
     * Show guide line or hide
     */
    TBool iShowGuideLine;

    /**
     * The guide line style
     */    
    TInt iGuideLineStyle;    

    /**
     * The top left of top guide line
     */
    TPoint iTlOfTopGuideLine;

    /**
     * The bottom right of top guide line
     */
    TPoint iBrOfTopGuideLine;

    /**
     * The top left of top guide line
     */
    TPoint iTlOfBottomGuideLine;

    /**
     * The bottom right of top guide line
     */
    TPoint iBrOfBottomGuideLine;
    
    /**
     * The guide line color
     */    
    TRgb iGuideLineColor;

    /**
     * The guide line withd
     */    
    TInt iGuideLineWidth;
    
    /**
     * Stoke end mark
     */
    TPoint iStrokeEndMark;
    
    /**
     * Fade timer
     * Own.
     */
    CPeriodic* iFadeTimer;
    
    /**
     * The global setttings repository
     * Own
     */
    CRepository* iGSRepository;
    
    /**
     * The begin time of fade
     */
    TInt iFadeBeginTime;
    
    /**
     * The interval time of fade
     */
    TInt iFadeIntervalTime;
    
    /**
     * The shown trace data
     */
    RArray<TPoint> iListOfShownPoints;
    
    /**
     * The begin position of fade
     */ 
    TInt iFadeBeginPos;
	
    /**
     * The end position of fade
     */   
	TInt iFadeEndPos;
	
    /**
     * The max position of fade
     */ 
	TInt iFadeMaxPos;
	
    /**
     * The flag of fade enable
     */ 
	TBool iEnableFade;
	
    /**
     * Reserved item1
     */
     TInt iReserved1;
     
    /**
     * Reserved item2
     */
     TInt iReserved2;	
    };
//end of class CTransparentHwrWndExt
  
#include <peninputlayouthwrwnd.inl>
#endif //C_CHWRWNDBASE_H