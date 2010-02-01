/*
* Copyright (c) 2005-2006 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of candidate window 
*
*/

#ifndef C_CCANDIDATEWND_H
#define C_CCANDIDATEWND_H

#include <peninputlayoutbasecontrol.h>

/**
 *  CCandidateWnd
 *
 *  Candidate window.
 *
 *  @lib feplayoutcontrol.lib
 *  @since S60 V4.0
 */
class CCandidateWnd : public CFepUiBaseCtrl, public MCandidateBuffer, 
                      public MTimerHandler
    {
public:

    /**
     * Destructor.        
     *
     * @since S60 V4.0        
     */           
    IMPORT_C ~CCandidateWnd();

    /**
     * Factory function.        
     *
     * @since S60 V4.0        
     * @param aRect The rectangle area for this control
     * @param aCandidateRectNum Number of candidates in one page
     * @param aUiLayout Ui layout who contains this control
     * @param aControlId control Id
     * @param aHideTimer Auto hide timer delay
     * @return An instance of CCandiadateWnd class        
     */                       
    IMPORT_C static CCandidateWnd* NewL(const TRect& aRect, TInt aCandidateRectNum,
                        CFepUiLayout* aUiLayout, TInt aCtrlId,
                        TTimeIntervalMicroSeconds32 aHideTimer = 0 );
    /**
     * Get Selected candidates.
     *
     * @since S60 V4.0
     * @return Current selected candidate
     */                           
    IMPORT_C const TDesC& GetSelCandidate();
    
    /**
     * Set autohide timer delay.
     *
     * @since S60 V4.0
     * @param aDelay The timer delay. If delay interval is 0, then disable auto hide
     */                                   
    IMPORT_C void SetAutoHideDelay(TTimeIntervalMicroSeconds32 aDelay);
    
    /**
     * Set number of candiates in one line
     * The cell rect and text rect is then caculted. text rect = cell rect
     *
     * @since S60 V4.0
     * @param aNumInLine The number of candiates in one page line
     */                                           
    IMPORT_C void SetCandidateNumL(TInt aNumInLine);       
   
    /**
     * Set number of candiates in one line and cell rect and text rect size.
     *
     * @since S60 V4.0
     * @param aNumInLine The number of candiates in one page line
     * @param aCellRectSize The cell rect size for one candidate
     * @param aTextRect The text rect for candidate drawing. This is relative 
     *                  to the cell rect.
     */                                                      
    IMPORT_C void SetCandidateNumL(TInt aNumInLine,const TSize& aCellRectSize,
    												const TRect& aTextRect);
    /**
     * Set the font for candidate
     *
     * @since S60 V4.0
     * @param aFontSpec the font spec to be used.
     */                                                   
    IMPORT_C void SetFont(const TFontSpec& aFontSpec);
    
    /**
     * Set the font for candidate
     *
     * @since S60 V4.0
     * @param aFont the font to be used.Ownership not transferred.
     */                                                   
    IMPORT_C void SetFont(const CFont* aFont);    

    /**
     * Set the baseline for text drawing
     *
     * @since S60 V4.0
     * @param aBaseline The baseline used for candidate drawing
     */                                         
    inline void SetBaseline(TInt aBaseline);    
    
    /**
     * get the baseline for text drawing
     *
     * @since S60 V4.0
     * @return The baseline
     */                                         
    inline TInt Baseline();        
    
    //from base class CFepUiBaseCtrl
    
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */                       
    IMPORT_C void Draw();

    /**
     * From CFepUiBaseCtrl
     * Update control and it's components rect
     *
     * @since S60 V4.0
     * @param aOffset The offset to be moved
     */                
    IMPORT_C void Move(const TPoint& aOffset);    
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer levae event. 
     * This will be called only when it's got pointer down event before and 
     * now pointer leaves without pointer up event
     *
     * @since S60 V4.0
     * @param aPoint current pointer position
     */
    virtual IMPORT_C void HandlePointerLeave(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Call back function when layout starts dragging
     *
     * @since S60 V4.0
     */
    virtual IMPORT_C void OnLayoutDraggingStart();

    /**
     * From CFepUiBaseCtrl    
     * Call back function when layout ends dragging
     *
     * @since S60 V4.0
     */
    virtual IMPORT_C void OnLayoutDraggingEnd();
       
    /**
     * OnActivate. Called when the layout is going to be shown.
     *
     * @since S60 v4.0
     */        
    virtual IMPORT_C void OnActivate();
    
    /**
     * OnDeActivate. Called when the layout is going to be hidden
     *
     * @since S60 v4.0
     */        
    virtual IMPORT_C void OnDeActivate();
       
    //from base class MTimerHandler
    /**
     * From MTimerHandler
     * HandleTimerOut. Handling time out event
     *
     * @since S60 V4.0
     * @param aTimeType The timer type
     */                   
    IMPORT_C void HandleTimerOut(TInt aTimeType);        

    //from base class MCandidateBuffer        
    /**
     * From MCandidateBuffer
     * Set candidate buffer 
     *
     * @since S60 V4.0
     * @param aCandidateData Candidate data
     */        
    IMPORT_C void SetCandidateL(const RPointerArray<HBufC>& aCandidateData);
    
protected:
    /**
     * Symbian second phrase constructor
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
     * Constructor
     *
     * @since S60 V4.0
     * @param aRect The rectangle area for this control
     * @param aCandidateRectNum Number of candidates in one page
     * @param aUiLayout Ui layout who contains this control.Ownership not transferred
     * @param aControlId control Id
     * @param aHideTimer Auto hide timer delay
     */       
    IMPORT_C CCandidateWnd(const TRect& aRect,TInt aCandidateRectNum,
                            CFepUiLayout* aUiLayout, TInt aControlId,
                               TTimeIntervalMicroSeconds32 aHideTimer);

    /**
     * 
     * Draw indication candidate is selected
     *
     * @since S60 V4.0
     * @param aSelection The selection index
     * @param aPressedFlag The flag indicates whether the pointer is pressed.
     *        Default value is ETrue, show pressed effect
     */    
    void DrawIndication(TInt aSelection, TBool aPressedFlag = ETrue);
    
private:
    /**
     * Number of candiate rect in one page line
     */
    TInt iCandidateRectNum;
    
    /**
     * Selection index. From 0 to iCandiateRectNum - 1
     */
    TInt iSelection;
    
    /** 
     * Auto hide flag. When true, the candidate window will hide if no candidate in list 
     */
    TBool iAutoHide;
       
    /**
     * Hide timer delay
     */
    TTimeIntervalMicroSeconds32  iDelay;
    
    /**
     * Font information 
     */
    TFontSpec iFontSpec;

    /**
     * Candidate rects for each candidate. Valid rect for pen event
     */
    RArray<TRect> iCandidateRectList;
    
    /**
     * Candidate text drawing rects for each candidate.
     */
    RArray<TRect> iCandidateTextRectList;    
    
    /**
     * Candidate data
     */
    RPointerArray<HBufC> iCandidateList;
    
    /**
     * Baseline for candidate drawing
     */
    TInt iBaseline;
    
    /**
     * Hide timer
     * Ownd
     */
    CLayoutTimer* iHideTimer;
     
     /** 
      * Font object used to draw candiates
      * Ownd
      */
     CFont* iFont;

    /**
     *  Flag ownership flag. ETrue if CFont object is owned by this control.
     */
    TBool iFontOwnership;

    /**
     * Indicate whether the pointer has leaved a candidate cell
     */    
    TBool iPointerLeftCandidate;
    
    /**
     * Reserved item1
     */
     TInt iReserved1;
     
    /**
     * Reserved item2
     */
     TInt iReserved2;    
    };
#include "peninputlayoutcandidatewnd.inl"    
#endif //C_CCANDIDATEWND_H
//end of CCandidateWindow
