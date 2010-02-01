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
* Description:  header file of virtual keyboard control
*
*/

#ifndef C_CVIRTUALKEYBOARD_H
#define C_CVIRTUALKEYBOARD_H

#include <AknsConstants.h>
#include <AknLayout2Def.h>
#include <AknsItemID.h>

#include "peninputlayoutbutton.h"
#include "peninputlayoutctrlgroup.h"
class CVirtualKey;
class CVirtualKeyCtrl;
class CBubbleCtrl;
class CPenInputColorIcon;
   
#define INVALID_COLOR_GROUP -1
   
typedef struct TVirtualKeyEventData_Tag
    {
    TKeyEvent       iKeyEvent; //key event
    TPtrC           iKeyData;  //data associated with the key
    CVirtualKey*    iKey;	   // virtual key
    } TVirtualKeyEventData;
    
enum TVirtualKeyBmpType
    {
    EKeyBmpNormal,
    EKeyBmpNormalMask,
    EKeyBmpHighlight,
    EKeyBmpHighlightMask,
    EKeyBmpDim,
    EkeyBmpDimMask,
    EKeyBmpLastType = EkeyBmpDimMask
    };
    
enum TVirtualKeyTextPos
    {
    EPosLeft,
    EPosRight1,
    EPosRight2,
    EPosRight3,
    EPosLast = EPosRight3
    };
    
enum TVirtualKeyIrregularKeyType
    {
    EKeyLeftDown,
    EKeyMiddleUp,
    EKeyMiddleDown,
    EKeyRightDown,
    EKeyRightUp
    };

enum TVirtualKeyIrregularKeyState
    {
    EKeyStateNormal,
    EKeyStateNormalMask,
    EKeyStatePressed,
    EKeyStatePressedMask,
    EKeyStateDimmed,
    EKeyStateDimmedMask
    };

struct TVirtualKeyDrawInfo
    {
    TAknsItemID iVKDimLeftImgID;
    TAknsItemID iVKDimMiddleImgID;
    TAknsItemID iVKDimRightImgID;

    TAknsItemID iVKLeftImgID;
    TAknsItemID iVKMiddleImgID;
    TAknsItemID iVKRightImgID;
    
    TAknsItemID iVKPressedLeftImgID;
    TAknsItemID iVKPressedMiddleImgID;
    TAknsItemID iVKPressedRightImgID;
    
    TRgb iVKHighlightColor;
    
    TBool iSupportSkin;
    
    TVirtualKeyDrawInfo(TAknsItemID aVKDimLeftImgID,
                          TAknsItemID aVKDimMiddleImgID,
                          TAknsItemID aVKDimRightImgID,
                          
                          TAknsItemID aVKLeftImgID,
                          TAknsItemID aVKMiddleImgID,
                          TAknsItemID aVKRightImgID,
                      
                          TAknsItemID aVKPressedLeftImgID,
                          TAknsItemID aVKPressedMiddleImgID,
                          TAknsItemID aVKPressedRightImgID,

                          TRgb aVKHighlightColor,
                          TBool aSupportSkin = EFalse)
        {
        
        iVKDimLeftImgID = aVKDimLeftImgID;
        iVKDimMiddleImgID = aVKDimMiddleImgID;
        iVKDimRightImgID = aVKDimRightImgID;
            
        iVKLeftImgID = aVKLeftImgID;
        iVKMiddleImgID = aVKMiddleImgID;
        iVKRightImgID = aVKRightImgID;

        iVKPressedLeftImgID = aVKPressedLeftImgID;
        iVKPressedMiddleImgID = aVKPressedMiddleImgID;
        iVKPressedRightImgID = aVKPressedRightImgID;      
        
        iVKHighlightColor = aVKHighlightColor;
        
        iSupportSkin = aSupportSkin;
            
        };
        
    TVirtualKeyDrawInfo()
        {
        iSupportSkin = EFalse;
        };
    };    

//class CVirtualKeyboard
/**
 *  CVirtualKeyboard
 *
 *  Keyboard control, it consist of virtual keys
 *
 *  @lib feplayoutcontrol.lib  
 *  @since S60 V4.0
 */
class CVirtualKeyboard : public CControlGroup
    {
protected:    
    enum TVirtualKeyEvent 
    	{
    	EVirtualKeyDown,
    	EVirtualKeyUp
    	};
public: 
    /**
     * class factory function
     *
     * @since S60 V4.0     
     * @param aRect The keyboard position relative to layout
     * @param aUiLayout The layout contains the keyboard. Owner ship not transferred.
     * @param aControlId Id for this keyboard control
     * @param aFontSpec The font used for key drawing.
     */
    IMPORT_C static CVirtualKeyboard* NewL(const TRect& aRect, 
                                           CFepUiLayout* aUiLayout,
                                           TInt aControlId,
                                           const TFontSpec& aFontSpec,
                                           TBool aIrregular = EFalse);
                                                        
    /**
     * class factory function
     *
     * @since S60 V4.0     
     * @param aRect The keyboard position relative to layout
     * @param aUiLayout The layout contains the keyboard. Owner ship not transferred.
     * @param aControlId Id for this keyboard control
     * @param aFont The font used for key drawing.
     */
    IMPORT_C static CVirtualKeyboard* NewL(const TRect& aRect, 
                            CFepUiLayout* aUiLayout,TInt aControlId,
                                           CFont* aFont,
                                           TBool aIrregular = EFalse);

    /**
     * Destructor
     *
     * @since S60 V4.0     
     */
    IMPORT_C virtual ~CVirtualKeyboard();

    /**
     * Add a key to keyboard
     *
     * @since S60 V4.0     
     * @param aKey The key to be added
     */    
    IMPORT_C virtual void AddKeyL(CVirtualKey* aKey);

    /**
     * Add a raw key to keyboard. Raw key works like a hardware key which
     * will generates( by simulating) raw key event when it's pressed.
     *
     * @since S60 V4.0     
     * @param aKey The key to be added
     */    
    IMPORT_C virtual void AddRawKeyL(CVirtualKey* aKey);
    
    /**
     * Create and add a key to keyboard
     *
     * @since S60 V4.0     
     * @param aCode The key code for the adding key
     * @param aScanCode The scancode for the adding key
     * @param aRect The key position. Relative to key board
     * @param aTextRect The rect for key drawing
     * @param aBaseline The baseline for char drawing
     * @param aHighlightBmpIdx The bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.
     * @param aHighlightMaskIdx The mask bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.
     */    
    IMPORT_C void CreateKeyL(TUint aCode, TUint aScanCode, const TRect& aRect,
                                const TRect& aTextRect, TInt aBaseline,
                                TInt aHighlightBmpIdx = -1,
                                TInt aHighlightMaskBmpId = -1);

    /**
     * Create and add a key to keyboard
     *
     * @since S60 V4.0     
     * @param aCode The key code for the adding key
     * @param aScanCode The scancode for the adding key
     * @param aRect The key position. Relative to key board
     * @param aTextRect The rect for key drawing
     * @param aBaseline The baseline for char drawing
     * @param aHighlightBmpIdx The bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.
     * @param aHighlightMaskIdx The mask bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.      
     */    
    IMPORT_C void CreateKeyL(const TDesC& aCode, TUint aScanCode, 
                             const TRect& aRect,const TRect& aTextRect, 
                             TInt aBaseline, TInt aHighlightBmpIdx = -1,
                             TInt aHighlightMaskBmpId = -1);

    /**
     * Create and add a raw key to keyboard
     *
     * @since S60 V4.0     
     * @param aCode The key code for the adding key
     * @param aScanCode The scancode for the adding key
     * @param aRect The key position. Relative to key board
     * @param aTextRect The rect for key drawing
     * @param aBaseline The baseline for char drawing
     * @param aHighlightBmpIdx The bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.
     * @param aHighlightMaskIdx The mask bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.
     */    
    IMPORT_C void CreateRawKeyL(TUint aCode, TUint aScanCode, const TRect& aRect,
                                const TRect& aTextRect, TInt aBaseline,
                                TInt aHighlightBmpIdx = -1,
                                TInt aHighlightMaskBmpId = -1);

    /**
     * Create and add a raw key to keyboard
     *
     * @since S60 V4.0     
     * @param aCode The key code for the adding key
     * @param aScanCode The scancode for the adding key
     * @param aRect The key position. Relative to key board
     * @param aTextRect The rect for key drawing
     * @param aBaseline The baseline for char drawing
     * @param aHighlightBmpIdx The bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.
     * @param aHighlightMaskIdx The mask bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.      
     */    
    IMPORT_C void CreateRawKeyL(const TDesC& aCode, TUint aScanCode, 
                             const TRect& aRect,const TRect& aTextRect, 
                             TInt aBaseline, TInt aHighlightBmpIdx = -1,
                             TInt aHighlightMaskBmpId = -1);
           
    IMPORT_C void CreateRawKeyL(RPointerArray<HBufC>& aUnicodesList,
                                TInt aScanCode,
                                const TRect& aRect);

    /**
     * Remove a key from keyboard. Key is not destroyed.
     *
     * @since S60 V4.0     
     * @param aKey The key to be removed
     */                                 
    IMPORT_C void RemoveKey(CVirtualKey* aKey);                    

    /**
     * Remove a key from keyboard. Key is not destroyed.
     *
     * @since S60 V4.0     
     * @param aIndex The key index. Index is not checked.
     */                                 
    inline void RemoveKey(TInt aIndex);                    


    /* Set the font for candidate
     *
     * @since S60 V4.0
     * @param aFontSpec the font spec to be used.
     */                                                   
    IMPORT_C void SetFont(const TFontSpec& aFontSpec);
    
    /**
     * Set font information
     *
     * @since S60 V4.0          
     * @param aFont The font info for key drawing.Ownership not transferred
     */
    IMPORT_C void SetFont(const CFont* aFont);    
    
    /**
     * Set key font color information.
     *     
     * @since S60 V4.0          
     * @param aFontColor The font color info for key drawing.
     */
    IMPORT_C void SetFontColor( const TRgb aFontColor );
    
    /**
     * Dim key
     *
     * @since S60 V4.0          
     * @param aKey The key to be dimmed
     */    
    IMPORT_C void DimKey(CVirtualKey* aKey,TBool aFlag);
    
    IMPORT_C void EnableKeyFeedback( CVirtualKey* aKey, TBool aEnable );
    
    /**
     * Set key dim bitmap
     *
     * @since S60 V4.0          
     * @param aBitmap The dim bitmap
     */
    inline void SetDimKeyBmp(CFbsBitmap* aBitmap);

    /**
     * Set key dim mask bitmap
     *
     * @since S60 V4.0          
     * @param aBitmap The dim mask bitmap
     */
    inline void SetDimKeyMaskBmp(CFbsBitmap* aBitmap);
    
    /**
     * Add key higilight bitmap
     * @since S60 V4.0
     * @param aBmp The key hight light bitmap. Owner ship transferred.
     */
    IMPORT_C void AddKeyHighLightBitmapL(CFbsBitmap* aBmp);
    
    /**
     * Remove highlight bitmap by index. The bitmap itself is not destroyed.
     * @since S60 V4.0
     * @param aIndex. 
     */
    IMPORT_C void RemoveKeyHighLightBitmap(TInt aIndex);    

    /**
     * Add key highlight mask bitmap
     * @since S60 V4.0
     * @param aBmp The key hight light mask bitmap. Owner ship transferred.
     */
    IMPORT_C void AddKeyHighLightMaskBitmapL(CFbsBitmap* aBmp);    
    
    /**
     * Remove highlight bitmap by index. The bitmap itself is not destroyed.
     * @since S60 V4.0
     * @param aIndex. 
     */
    IMPORT_C void RemoveKeyHighLightMaskBitmap(TInt aIndex);    
    
    /**
     * Set fading parameter. Key pressing effect is done by fading the key.
     * 
     * @since S60 V4.0
     * @param aBlackMap Black map fading parameter. 
     *      See CBitmapContext::SetFadingParameters for more information
     * @param aWhiteMap White map fading parameter
     */                                                                 
    inline void SetFadingParameters(TUint8 aBlackMap, TUint8 aWhiteMap);    

    /**
     * Return key array
     *
     * @since S60 V4.0
     * @return The key array
     */    
    inline const RPointerArray<CVirtualKey>& KeyArray();
    
    /**
     * Return highlight bmp list
     *
     * @since S60 V4.0
     * @return highlight bmp list
     */    
    inline const RPointerArray<CFbsBitmap>& KeyHighlightBmpList();
    
    /**
     * Return highlight mask bmp list
     *
     * @since S60 V4.0
     * @return highlight mask bmp list
     */        
    inline const RPointerArray<CFbsBitmap>& KeyHighlightMaskBmpList();
    
    inline void SetKeySkinId(TVirtualKeyBmpType aBmpType, TAknsItemID aSkinId);
    inline TAknsItemID KeySkinId(TVirtualKeyBmpType aBmpType);
    inline void SetTextLineLayout(TAknTextLineLayout aTextLine, TVirtualKeyTextPos aPos);
    inline TAknTextLineLayout TextLineLayout(TVirtualKeyTextPos aPos);
    inline void SetTextLineLayout(TAknTextLineLayout aTextLine);
    inline TAknTextLineLayout TextLineLayout();
    inline TBool TextLineLayoutSet(TVirtualKeyTextPos aPos);
    inline void SetDrawOpaqueBackground(TBool aNeedDraw);
    /**
     * Set keyboard text color group
     *
     * @since S60 V4.0
     * @param color group index
     */        
    inline void SetKeyTextColorGroup(TInt aColorGroup);
    inline TInt KeyTextColorGroup() const;
    //from base class CFepUiBaseCtrl    
   
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V4.0
     */                       
    IMPORT_C virtual void Draw();     

    IMPORT_C void SetRect(const TRect& aRect);    
    
    /**
     * Add key higilight bitmap
     * @since S60 V4.0
     * @param aBmp The key hight light bitmap. Owner ship transferred.
     */
    IMPORT_C void AddIrregularKeyBitmapL(TVirtualKeyIrregularKeyType aKeyType,
                                         TVirtualKeyIrregularKeyState aKeyState,
                                         CFbsBitmap* aBmp);
    
    /**
     * Remove highlight bitmap by index. The bitmap itself is not destroyed.
     * @since S60 V4.0
     * @param aIndex. 
     */
    IMPORT_C void RemoveIrregularKeyBitmapL(TVirtualKeyIrregularKeyType aKeyType,
                                            TVirtualKeyIrregularKeyState aKeyState);
                                            
    /**
     * Remove highlight bitmap by index. The bitmap itself is not destroyed.
     * @since S60 V4.0
     * @param aIndex. 
     */
    IMPORT_C CFbsBitmap* IrregularKeyBitmap(TVirtualKeyIrregularKeyType aKeyType,
                                            TVirtualKeyIrregularKeyState aKeyState);
                                            
                                            
                                            
    IMPORT_C TBool IsIrregularKeyBoard();
                                                                                          
    IMPORT_C void SetIrregularKeyBoard(TBool aIrregular);
     
    /**
     * Add nonirregular key bitmap
     * @since S60 V4.0
     * @param aBmp The key bitmap. Owner ship transferred.
     */
    IMPORT_C void SetNonIrregularKeyBitmapL(TVirtualKeyBmpType aBmpType, CFbsBitmap* aBmp);
    
    /**
     * Get non irregularkey bitmap by index. The bitmap itself is not destroyed.
     * @since S60 V4.0
     * @param aIndex. 
     */
    IMPORT_C CFbsBitmap* NonIrregularKeyBitmap(TVirtualKeyBmpType aBmpType);
    
    /**
     * Hide or show the bubble.
     * @param aFlag. 
     * @since S60 V4.0
     */
    IMPORT_C void ShowBubble(TBool aFlag);
    
    /**
     * Set bubble bitmap effect.
     * @since S60 V4.0
     * @param aBmpId. 
     * @param aMaskBmpId. 
     * @param aBgSkinId. 
     */
    IMPORT_C void SetBubbleBitmapParam(CFbsBitmap* aBmpId,
                                       CFbsBitmap* aMaskBmpId,
                                       TAknsItemID aBgSkinId);

    /**
     * Test that keep pointer down effect or not when the pointer is down and moving
     * @since S60 V4.0
     */    
    IMPORT_C TBool GowithPointerDown();    

    /**
     * Set keeping pointer down effect when the pointer is down and moving
     * @since S60 V4.0
     * @param aFlag. 
     */    
    IMPORT_C void SetGowithPointerDown(TBool aFlag);
        
    /**
     * Set the size of bubble
     *
     * @since S60 V4.0          
     * @param aSize The size of bubble
     */ 
    IMPORT_C void SetBubbleSize(const TSize& aSize);

    /**
     * Set the margin between the bottom of bubble and top of the key
     *
     * @since S60 V4.0          
     * @param aSize The size of bubble
     */ 
    IMPORT_C void SetBubbleVerticalMargin(TInt aMargin);

    /**
     * Set text format
     *
     * @since S60 V4.0          
     * @param aTextFormat The format of text on bubble 
     */ 
    IMPORT_C void SetTextFormat(TAknTextLineLayout aTextFormat);
    /**
     * Set the size of bubble frame inner rect
     *
     * @since S60 V4.0          
     * @param aSize The size of bubble frame inner rect
     */ 
    IMPORT_C void SetFrameDiff(TInt aLeftDiff,TInt aTopDiff,TInt aRightDiff,TInt aBottomDiff);    

    IMPORT_C void SetVirtualKeyImgID(TVirtualKeyDrawInfo aVirtualKeyDrawInfo);
	
    /**
     * Set shift icon showing on the key
     *
     * @since S60 V4.0          
     * @param aIcon shift icon
     */ 	
	IMPORT_C void SetShiftIcon( CPenInputColorIcon* aIcon );
	inline CPenInputColorIcon* ShiftIcon();
    
protected:                          

    /**
     * 2nd phase constructor
     *
     * @since S60 V4.0     
     */          
    IMPORT_C void ConstructL();
    
    /**
     * Default constructor
     *
     * @since S60 V4.0     
     * @param aRect The keyboard position relative to layout
     * @param aUiLayout The layout contains the keyboard. Owner ship not transferred.
     * @param aControlId Id for this keyboard control
     * @param aFontSpect The font used for key drawing
     */          
     IMPORT_C CVirtualKeyboard(const TRect& aRect,CFepUiLayout* aUiLayout,TInt aControlId,
                                const TFontSpec& aFontSpec, TBool aIrregular = EFalse);    


    /**
     * 
     * find the key in a position
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The first key which contains the point
     */
    //const CVirtualKey* FindKeyByPosition(const TPoint& aPt);
    
    CVirtualKeyCtrl* FindKeyCtrlByPosition(const TPoint& aPt);
                                
    /**
     * From CFepUiBaseCtrl
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                                        
    //IMPORT_C virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                  
    //IMPORT_C virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer move event
     *
     * @param aPoint The point
     * @since S60 V4.0 position relative the layout
     * @return The control which handles the event.     
     */                  
    IMPORT_C virtual CFepUiBaseCtrl* HandlePointerMoveEventL(const TPoint& aPt);    

    /**
     * From CFepUiBaseCtrl
     * Handle pointer levae event. 
     * This will be called only when it's got pointer down event before and 
     * now pointer leaves without pointer up event
     *
     * @since S60 V4.0
     * @param aPoint current pointer position
     */    
    //IMPORT_C void HandlePointerLeave(const TPoint& aPoint);           
private:
    /**
     * 
     * Add a key control to keyboard
     *
     * @since S60 V4.0
     * @param aKeyCtrl The key to be added
     */            
    void AddKeyControlL(CVirtualKeyCtrl* aKeyCtrl);
    
    /**
     * 
     * Remove a key control from keyboard. The control is removed and should be used 
     * later
     *
     * @since S60 V4.0
     * @param aKeyCtrl The key to be removed.
     */            
    void RemoveKeyControl(CVirtualKeyCtrl* aKeyCtrl);
    
    /**
     * 
     * Draw a key
     *
     * @since S60 V4.0
     * @param aKey The key to be drawn
     */        
    void DrawKey(const CVirtualKey* aKey);
    
    /**
     * 
     * Set a key event and report to observer
     *
     * @since S60 V4.0
     * @param aType The general virtuak key event type, i.e, up or down
     */        
    //IMPORT_C virtual void ReportKeyEvent(TVirtualKeyEvent aType);
    
    //void ReportKeyEvent(TVirtualKeyEvent aType, CVirtualKey* aKey);

    /**
     * 
     * Draw indication when key is pressed
     *
     * @since S60 V4.0
     * @param aKey The key to be drawn
     * @param aPressedFlag The flag indicates whether the key is pressed.
     *        Default value is ETrue, show pressed key
     */    
    //void DrawIndication(const CVirtualKey* aKey, TBool aPressedFlag = ETrue);
    
    /**
     * 
     * Change the feedback area
     *
     * @since S60 V4.0
     * @param aId The unique id for this area
     * @param aNewRect The new feedback area
     * @param aOldRect The old feedback area
     */        
    //void ChangeFeedbackArea(TInt aId,const TRect& aNewRect,const TRect& aOldRect);    
    
    /**
     * 
     * De-register the feedback area
     *
     * @since S60 V4.0
     * @param aId The unique id for this area
     * @param aRect The feedback area
     */      
    //void DeRegisterFeedbackArea(TInt aId,const TRect& aRect);
    
    CVirtualKeyCtrl* FindCtrlForKey(const CVirtualKey* aKey);
    
    
    CVirtualKeyCtrl* CreateKeyCtrlL(CVirtualKey* aKeyInfo, TBool aRawKeyFlag);
    
protected:
    /**
     * Current pressed key.
     * Not own
     */               
    CVirtualKey* iPressedKey;
private:
    
    void ConstructIrregularBmps();
    
    void ConstructOneIrregularBmp(RPointerArray<CFbsBitmap>& iBmpList); 
                                  
    void DestroyIrregularBmps();                                     
                                     
    void DrawBubble(CVirtualKey* aKey);
    
    void ClearBubble(TBool aSetZeroSize);
    
private:
    /**
     * Virtuak Key array
     */
    RPointerArray<CVirtualKey> iKeyArray;    
    
    /**
     * Font information for key drawing
     */     
    TFontSpec iFontSpec;

    /**
     * Fading parameter. Used for pressed key effect
     */          
    TUint8 iFadingParamBlack;
    
    /**
     * Fading parameter. Used for pressed key effect
     */              
    TUint8 iFadingParamWhite;

    /**
     * High light bitmap list for different size of keys
     */                  
    RPointerArray<CFbsBitmap> iKeyHighLightBmpList;
    
    /**
     * bitmap list for different size of keys
     */                  
    RPointerArray<CFbsBitmap> iNonIrregularKeyBmpList;
	
    /**
     * High light mask bitmap list for different size of keys
     */                  
    RPointerArray<CFbsBitmap> iKeyHighLightMaskBmpList;

    RPointerArray<CFbsBitmap> iKeyLeftDownBmpList;

    RPointerArray<CFbsBitmap> iKeyMiddleUpBmpList;

    RPointerArray<CFbsBitmap> iKeyMiddleDownBmpList;

    RPointerArray<CFbsBitmap> iKeyRightDownBmpList;

    RPointerArray<CFbsBitmap> iKeyRightUpBmpList;
     
    /**
     * Font object for key drawing
     * Ownership depends on flag iFontOwnership.
     */          
    CFont* iFont;    

    /**
     * Dim key bitmap.
     * own
     */                   
    CFbsBitmap* iDimKeyBmp;
    
    /**
     * Dim key mask bitmap.
     * own
     */    
    CFbsBitmap* iDimKeyMaskBmp;  

    /**
     *  Flag ownership flag. ETrue if CFont object is owned by this control.
     */
    TBool iFontOwnership;
            
    /**
     * Item text display color.
     */
    TRgb  iFontColor;
    
    /**
     * Reserve item
     */    
    TInt iReserver1;
    
    TAknsItemID iKeyNormalSkinId;
    TAknsItemID iKeyHighlightSkinId;
    TAknsItemID iKeyDimSkinId;
    RArray<TAknTextLineLayout> iTextLineLayout;
    RArray<TBool> iTextLineSet;
    TBool iNeedDrawBackground;

    /**
     * Unused key pool. When key is removed from key board, it's added to the pool.
     * This is a patch for current vkb UI implementation, as they will remove keys 
     * when a key is handling pointer event.
     */            
    RPointerArray<CVirtualKeyCtrl> iUnUsedKeyCtrlPool;
    
    /**
     * Unused raw key pool. When key is removed from key board, it's added to the pool.
     * This is a patch for current vkb UI implementation, as they will remove keys 
     * when a key is handling pointer event.
     */                
    RPointerArray<CVirtualKeyCtrl> iUnUsedRawKeyCtrlPool;
    
    TBool iIrregularKey;

	CBubbleCtrl* iBubbleCtrl;
	
	TInt iBubbleVerticalMargin;
	
	TBool iGowithPointerDown;
	
	TSize iBubbleSize;

	TAknTextLineLayout iVKBTextLineLayout;
	/**
	*  Flag to control whether to show bubble
	*/
	TBool iIsShowBubble;
	
	/**
     * Keypad text color group
     */    
    TInt iKeyTextColorGroup;
	
	/**
     * The shift icon showing on the key
     */      
    CPenInputColorIcon* iShiftIcon;
friend class CVirtualKeyCtrl;
friend class CVirtualRawKeyCtrl;
//friend class CVirtualKey;    
    };

//class CVirtualKey
/**
 * CVirtualKey
 *
 * Key on virtual key board. A virtual key is similar to the key in hard keyboard.
 * It will report virtual key up/down event when pressed up/down with a TkeyEvent data
 *
 * @lib feplayoutcontrol.lib  
 * @since S60 V4.0
 */
class CVirtualKey : public CBase
    {

public:
    /**
     * Factory function
     * This is used for those languange whose character needs more unicodes.
     *
     * @since S60 V4.0     
     * @param aCode The code this key stands for. 
     * @param aScanCode The scan code for this key
     * @param aRect The position of the key relative to keyboard
     * @param aCharRect The rect when key is drawn
     * @param aBaseLine The baseline for key drawing
     * @param aHighlightBmpIdx The bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.
     * @param aHighlightMaskIdx The mask bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.                        
     */
    IMPORT_C static CVirtualKey* NewL(TUint aCode, TInt aScanCode,
                                 const TRect& aRect, const TRect& aCharRect, 
                                 TInt aBaseLine, TInt aHighlightBmpIdx = -1,
                                 TInt aHighlightMaskBmpId = -1);

    /**
     * Factory function
     * This is used for those languange whose character needs more unicodes.
     *
     * @since S60 V4.0     
     * @param aCode The code this key stands for. 
     * @param aScanCode The scan code for this key
     * @param aRect The position of the key relative to keyboard
     * @param aCharRect The rect when key is drawn
     * @param aBaseLine The baseline for key drawing
     * @param aHighlightBmpIdx The bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.
     * @param aHighlightMaskIdx The mask bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.                        
     */
    IMPORT_C static CVirtualKey* NewL(const TDesC& aCode, TInt aScanCode,
                               const TRect& aRect, const TRect& aCharRect, 
                               TInt aBaseLine, TInt aHighlightBmpIdx = -1,
                               TInt aHighlightMaskBmpId = -1);

    IMPORT_C static CVirtualKey* NewL(RPointerArray<HBufC>& aUnicodesList,
                                      TInt aScanCode,
                                      const TRect& aRect);

    /**
     * Destructor
     *
     * @since S60 V4.0     
     */
    IMPORT_C virtual ~CVirtualKey();
    
    IMPORT_C void SetUnicodesL(RPointerArray<HBufC>& aUnicodesList);


    /**
     * Return key position
     *
     * @since S60 V4.0
     * @return The key position relative to key board
     */    
    inline const TRect& Rect() const; 
    /**
     * Set key position
     *
     * @since S60 V4.0
     * @param The key position
     */    
    IMPORT_C void SetRect(const TRect& aRect); 
    
    /**
     * Get key code
     *
     * @since S60 V4.0
     * @return The key code
     */
    inline TUint KeyCode() const;
    
    /**
     * Get key code data. Support for multi language
     *
     * @since S60 V4.0
     * @return The key codes
     */
    inline const TDesC& KeyUnicodes() const;
    const TDesC& KeyUnicodes(TVirtualKeyTextPos aPos) const;
    
    /**
     * Get key scancode
     *
     * @since S60 V4.0
     * @return The key scancode
     */
    inline TInt ScanCode() const;    
    
    /**
     * Get key display code
     *
     * @since S60 V4.0
     * @return The key scancode
     */
    inline TUint DisplayCode() const;

    /**
     * Set key display code
     *
     * @since S60 V4.0
     * @param aCode The display code
     */
    inline void SetDisplayCode(TUint aCode);
    /**
     * Get key highlight image index
     *
     * @since S60 V4.0
     * @return The index for key highligh image
     */
    inline TInt HighlightBmpIndex();    
    
    /**
     * Get key highlight mask image index
     *
     * @since S60 V4.0
     * @return The index for key highligh mask image
     */
    inline TInt HighlightMaskBmpIndex();       
     
    /**
     * Dim a key
     *
     * @since S60 V4.0
     * @param aFlag The dim status
     */
    inline void SetDimmed(TBool aFlag);    
    
    /**
     * Get the key dim status
     *
     * @since S60 V4.0
     * @return The dim status
     */
     inline TBool Dimmed() const;        
    
    /**
     * set key to be a latch key
     *
     * @since S60 V4.0
     * @param aFlag The key latch style flag. ETrue if set to latch key.
     */
     inline void SetLatchKey(TBool aFlag);
    
    /**
     * test whether this key a latch key
     *
     * @since S60 V4.0
     * @return ETrue if the key is a latch key. Otherwise EFalse
     */    
     inline TBool IsLatchKey();
    
    /**
     * set key latch state
     *
     * @since S60 V4.0
     * @param The latch key state.
     */    
     IMPORT_C void SetLatched(TBool aFlag);
     
     IMPORT_C TVirtualKeyIrregularKeyType IrregualrKeyType();
     
     IMPORT_C void SetIrregualrKeyType(TVirtualKeyIrregularKeyType aKeyType);
    /**
     * get key latch state
     *
     * @since S60 V4.0
     * @return The key latch state. ETrue if latched
     */    
     inline TBool Latched();

     inline void SetInnerRect(const TRect& aRect);
     inline TRect InnerRect();
     TBool UnicodesListEmpty(TInt& aIdx);

    /**
     * Set keyboard to key
     *
     * @since S60 V4.0
     * @param The keyboard which contains the key.
     */    
//     inline void SetKeyboard(CVirtualKeyboard* aKeyboard);
    
     IMPORT_C void SetDisplayUnicodeL( const TDesC& aDspUnicode );
     inline TDesC* DisplayUnicode() const;
     
     IMPORT_C void HideKeyCtrl( TBool aHide ); 
     inline void SetKeyData( const TDesC& aKeyData );
     inline void SetKeyScancode( TInt aScancode );
protected:            
    /**
     * 2nd phase constructor
     *
     * @since S60 V4.0     
     */          
    void ConstructL(const TDesC& aCodeData);
    
    void ConstructL(const RPointerArray<HBufC>& aUnicodesList);

private:
    /**
     * Default constructor
     *
     * @since S60 V4.0     
     * @param aCode The code this key stands for
     * @param aScanCode The scan code for this key
     * @param aRect The position of the key relative to keyboard
     * @param aCharRect The rect when key is drawn
     * @param aBaseLine The baseline for key drawing
     * @param aHighlightBmpIdx The bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.
     * @param aHighlightMaskIdx The mask bitmap index for highlighging key
                        background. Bitmap is stored in virtual key board.                        
     */
    CVirtualKey(TUint aCode, TInt aScanCode, const TRect& aRect, TInt aHighlightBmpIdx,
                 TInt aHighlightMaskBmpId);

    CVirtualKey(TInt aScanCode,
                const TRect& aRect);
private:
    /**
     * Create a key control
     *
     * @since S60 V5.0     
     * @param aKeyInfo The virtual key information
     * @param aRawKeyFlag Flag indicates whether a raw key is needed
     * @return The created key control
     */
    CVirtualKeyCtrl* CreateKeyCtrlL(CVirtualKey* aKeyInfo,TBool aRawKeyFlag);    
private:
    /**
     * Key event attribute for this key        
     */
    TUint       iCode;
    TUint       iScanCode;
    /**
     * The position relative to the keyboard. Not relative to the layout.
     * This is the valid region when pointer event effected to this key.
     */
    TRect   iRect;
    
    TRect   innerRect;

    /**
     * The hight light bitmap index for this key. Bitmap is owned by keyboard
     * High light bitmap is used when key does not have background bitmap itself,
     * pressed/highlight effect is down by show the highlight bitmap then draws text
     * on top of the bitmap. See DrawIndication.
     * 
     * The assumption is that when this index is >= 0 , then fading effect
     * is not used. Key pressing effect is then implemented by bitmap.
     */    
    TInt iHighlightBmpIndex;
    
    /**
     * The hight light mask bitmap index for this key
     */    
    TInt iHighlightMaskBmpIndex;    
    
    /*
     * Dim status
     */
    TBool iDimmed;    
     
    /*
    * Latch key flag    
    */ 
    TBool iIsLatchKey;
    
    /*
    * Latch key state
    */ 
    TBool iLatched;    
    
    /**
     * unicode data for the key. By default it only contains the key code.
     * But for some languages, a character may need several unicodes. 
     */    
    HBufC* iCodeData;   
    RPointerArray<HBufC> iUnicodesList;    
    /**
     * Unicode data for the key display. Some special character need additional
     * unicode to display. It's different than its unicode.
     */
    HBufC* iDisplayCodeData;

    /**
     * Keyboard which contains the key					     
     */    
    //CVirtualKeyboard* iKeyboard;    
    /**
     * Key control created for this key information by key board
     */        
    CVirtualKeyCtrl* iKeyCtrl;    
    
    TVirtualKeyIrregularKeyType iIrregularKeyType;
    /**
     * Reserve item
     */    
    TInt iReserver1;
    
    /**
     * Reserve item
     */    
    TInt iReserver2;
    
friend class CVirtualKeyCtrl;
friend class CVirtualKeyboard;   
    };
//end of CVirtualKey
    
#include <peninputlayoutvkb.inl>
#endif //C_CVIRTUALKEYBOARD_H