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
* Description:  header of char range selector.
*
*/

#ifndef C_PENINPUTFINGERHWRRANGESELECTOR_H
#define C_PENINPUTFINGERHWRRANGESELECTOR_H

// INCLUDES
#include <e32std.h>
#include <e32base.h>
#include <e32cmn.h>

#include <peninputlayoutctrlgroup.h>

// FORWARD DECLARATIONS
class CAknFepCtrlEventButton;
class CAknFepCtrlMultiImageButton;

// CLASS DECLARATION

/**
 *  class CPeninputFingerHwrRangeSelector.
 *
 *  a droplist as range selector. 
 *
 *  @lib peninputfingerhwr.lib
 *  @since S60 v5.0
 */
class CPeninputFingerHwrRangeSelector : public CControlGroup
    {
public:
    /**
     * Symbian constructor
     * 
     * @since S60 v5.0
     * @param aUiLayout The layout
     * @param aId The control id
     * @return pointer of created object.
     */
    static CPeninputFingerHwrRangeSelector* NewL( CFepUiLayout* aUiLayout, TInt aId );

    /**
     * Symbian constructor
     * 
     * @since S60 v5.0
     * @param aUiLayout The layout
     * @param aId The control id
     * @return pointer of created object.
     */    
    static CPeninputFingerHwrRangeSelector* NewLC( CFepUiLayout* aUiLayout, TInt aId );

    /**
     * standard c++ destructor.
     *
     * @since S60 v5.0
     */    
    ~CPeninputFingerHwrRangeSelector();
    
public:
    /**
     * popup the list.
     * 
     * @since S60 v5.0
     * 
     * @param aDirect direction of popup. It can be one of the following:
     *     EPopDirAuto, EPopDirLeft, EPopDirRight, EPopDirTop, EPopDirDown.
     *     default value is EPopDirAuto.
     *     See TPeninputPopListDirection
     * @return None
     */
    void Popup( TInt aDirect = EPopDirAuto);
    
    /**
     * cancel the popup.
     * 
     * @since S60 v5.0
     * @return None
     * 
     */
    void CancelPopup();
    
    /**
     * get visibility of popup.
     * @since S60 v5.0
     * @return ETrue if the popup is visible
     */
    TBool IsPopup();
    
    /**
     * set permitted char range.
     * 
     * @since S60 v5.0 
     * @param aPermittedRanges permitted character ranges. It can be combination of the following: 
     *     ERangeNative, ERangeEnglish, ERangeNumber.
     *     See TAknFepPenInputRange.
     */
    void SetPermittedRanges( const TInt aPermittedRanges );
    
    /**
     * set current char range.
     * 
     * @since S60 v5.0
     * @param aCurRange current character range. It must be one of the following:
     *     EFingerHwrNativeRange, EFingerHwrEnglishRange,EFingerHwrNumberRange.
     *     See TFingerHwrRange. 
     * @param aActive 
     */
    void SetCurRange( const TInt aCurRange, const TBool aActive );
    
    /**
     * set base rect of popup.
     * 
     * @since S60 v5.0
     * @param aRect
     * @return None
     */
    void SetBaseRect(const TRect& aRect);

protected: //virtuals from CControlGroup (CFepUiBaseCtrl)

    /**
     * From CControlGroup
     * Handle pointer down event
     *
     * @since S60 V5.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */   
    CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * From CControlGroup
     * Handle pointer up event
     *
     * @since S60 V5.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */           
    CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
    /**
     * From CControlGroup
     * Handle pointer move event
     *
     * @since S60 54.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */      
    CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPoint);
    
private:

    /**
     * C++ constructor
     * 
     * @since S60 v5.0
     * @param aFepUiLayout A Ui Layout environment (CFepUiLayout)
     * @param aControlId Control id
     * @return none.
     */
    CPeninputFingerHwrRangeSelector(CFepUiLayout* aFepUiLayout, TInt aControlId);
    
    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */    
    void ConstructL();
    
    /**
     * EventButton creation helper.
     * 
     * @since S60 v5.0
     * @param aCtrlId button control id.
     * @param aResId resource id.
     * @param aEvent event id;
     * @param aUnicode a unicode value to be sent
     * @return pointer to created CAknFepCtrlEventButton obj.
     */       
    CAknFepCtrlEventButton* CreateEventBtnL( TInt aCtrlId, TInt32 aResId,
            TInt aEvent = 0xFFFF,TInt aUnicode=0 );
    
    
    /**
     * MultiImageButton creation helper.
     * 
     * @since S60 v5.0
     * @param aCtrlId button control id.
     * @param aResId resource id. 
     * @return pointer to created CAknFepCtrlMultiImageButton obj.
     */      
    CAknFepCtrlMultiImageButton* CreateMultiBtnL( TInt aCtrlId, TInt32 aResId );
    
    /**
     * EventButton layout helper. Move button to specified rect.
     * 
     * @since S60 v5.0
     * @param aButton the button to move
     * @param aRect new rect of button
     * @param aXPadding horz padding of button icon.
     * @param aYPadding vert padding of button icon.
     * @param aReloadImages Specifies whether the images is to be reloaded.
     * @return None
     */     
    void MoveIconButton( CAknFepCtrlEventButton* aButton, const TRect& aRect, 
            TInt aXPadding=0, TInt aYPadding=0, TBool aReloadImages=ETrue );
    
    /**
     * MultiIconButton layout helper. Move button to specified rect.
     * 
     * @since S60 v5.0
     * @param aButton the button to move
     * @param aRect new rect of button
     * @param aXPadding horz padding of button icon.
     * @param aYPadding vert padding of button icon.
     * @param aReloadImages  Specifies whether the images is to be reloaded.
     * @return None
     */      
    void MoveMultiIconButton( CAknFepCtrlMultiImageButton* aButton, 
            const TRect& aRect, TInt aXPadding=0, TInt aYPadding=0, 
            TBool aReloadImages=ETrue );
    
    /**
     * layout buttons.
     * 
     * @since S60 v5.0
     * @param aPermittedRanges permitted char range flags. It can be combination of the following: 
     *     ERangeNative
     *     ERangeEnglish
     *     ERangeNumber.
     *     See TAknFepPenInputRange.
     * @param aCurRange current range id. It can be one of the following:
     *      EFingerHwrNativeRange
     *      EFingerHwrEnglishRange
     *      EFingerHwrNumberRange
     *      EFingerHwrSymbolRange
     * @param aActive Specifies whether the current range is active.
     * @param aShowDropList Specifies whether the popup is visible.
     * @param aDropDirect direction of popup. It can be one of the following:
     *     EPopDirAuto 
     *     EPopDirLeft 
     *     EPopDirRight 
     *     EPopDirTop 
     *     EPopDirDown
     *    See TPeninputPopListDirection.
     * @return none.
     */
    void LayoutRangeButtons( TInt aPermittedRanges, TInt aCurRange, 
            TBool aActive, TBool aShowDropList, TInt aDropDirect );
    
    /**
     * calculates popup direction.
     * 
     * @since S60 v5.0
     * @param aBound rect of display area.
     * @param aTarget position of popup list.
     * @param aItemSize item size.
     * @param aItemCount item count.
     * @param aDirect direction of popup. It can be one of the following:
     *     EPopDirAuto 
     *     EPopDirLeft 
     *     EPopDirRight 
     *     EPopDirTop 
     *     EPopDirDown
     *    See TPeninputPopListDirection.
     * @return a TPoint object as popup direction and offset.
     */
    TPoint CalcDropListDirection( const TRect& aBound, const TPoint& aTarget, 
            const TSize& aItemSize, TInt aItemCount, TInt aDirect );
    
    /**
     * calculate popup rect.
     * 
     * @since S60 v5.0
     * @param aRect base rect of popup
     * @param aWidth new width of popup
     * @param aHeight new height of popup
     * @return none
     */
    void CalcDropListRect( TRect& aRect, const TInt aWidth, const TInt aHeight );
    
private:
    /**
     * chinese range button.
     * not own.
     */
    CAknFepCtrlEventButton* iRangeChBtn;
    
    /**
     * english range button.
     * not own.
     */
    CAknFepCtrlEventButton* iRangeEnBtn;
    
    /**
     * numeric range button.
     * not own.
     */    
    CAknFepCtrlEventButton* iRangeNumBtn;
    
    
    /**
     * chinese range button, selected.
     * not own.
     */    
    CAknFepCtrlMultiImageButton* iRangeChMiBtn;
    
    /**
     * english range button, selected.
     * not own.
     */ 
    CAknFepCtrlMultiImageButton* iRangeEnMiBtn;
    
    /**
     * numeric range button, selected.
     * not own.
     */    
    CAknFepCtrlMultiImageButton* iRangeNumMiBtn;
    
    /**
     * visibility of popup list.
     */
    TBool iPopupVisible;
    
    /**
     * bit mask of permitted char ranges.
     */
    TInt iPermittedRanges;
    
    /**
     * Current char range id.
     */
    TInt iCurRange;
    
    /**
     * whether current range is active.
     */
    TBool iCurRangeActive;
    
    /**
     * base rect of popup.
     */
    TRect iPopupBaseRect;
    
    };

#endif // C_PENINPUTFINGERHWRRANGESELECTOR_H
