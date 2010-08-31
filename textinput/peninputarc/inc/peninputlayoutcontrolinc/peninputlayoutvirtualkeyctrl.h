/*
* Copyright (c) 2007-2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  header file of virtual keyboard key information and control
*
*/


#ifndef C_CVIRTUALKEYBOARD_KEY_CTRL_H
#define C_CVIRTUALKEYBOARD_KEY_CTRL_H
#include "peninputlayoutvkb.h"
#include "peninputlayoutbasecontrol.h"
class CVirtualKeyboard;
class CVirtualKey;

//class CVirtualKeyCtrl
//Internal used by virtual keyboard
NONSHARABLE_CLASS(CVirtualKeyCtrl) : public CFepUiBaseCtrl
    {
protected:
    //internal event type
    enum TVirtualKeyEventType
        {
        EVirtualKeyControlUp,	 //key up event on virtual key
        EVirtualKeyControlDown,  //key down event on virtual key
        };
public:
    /**
     * Factory function.        
     *
     * @since S60 V5.0        
     * @param aKeyboard The keyboard contains the key
     * @param aKeyInfo The virtual key information data
     * @return An instance of CVirtualKeyCtrl class        
     */ 
    static CVirtualKeyCtrl* NewL(CVirtualKeyboard* aKeyboard,CVirtualKey* aKeyInfo);    
       
    /**
     * Destructor
     *
     * @since S60 V5.0
     */       
    ~CVirtualKeyCtrl();
    
    //from base class CFepUiBaseCtrl
    /**
     * From CFepUiBaseCtrl
     * Draw control
     *
     * @since S60 V5.0
     */                               
    virtual void Draw();    

    /**
     * From CFepUiBaseCtrl
     * Handle pointer levae event. 
     * This will be called only when it's got pointer down event before and 
     * now pointer leaves without pointer up event
     *
     * @since S60 V5.0
     * @param aPoint current pointer position
     */
    virtual void HandlePointerLeave(const TPoint& aPoint);
    
    /**
     * From CFepUiBaseCtrl
     * Handle pointer enter event. 
     * This will be called only when it's got pointer down event previously then 
     * pointer leaves but now comes back without pointer up event.
     *
     * @since S60 V5.0
     * @param aPoint current pointer position     
     */
    virtual void HandlePointerEnter(const TPoint& aPoint);            

    /**
     * From CFepUiBaseCtrl
     * Handle pointer down event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
     * @return The control which handles the event.     
     */                                        
    virtual CFepUiBaseCtrl* HandlePointerDownEventL(const TPoint& aPoint);    
   
    /**
     * From CFepUiBaseCtrl
     * Handle pointer up event
     *
     * @since S60 V4.0
     * @param aPoint The point position relative the layout
 	 * @return The control which handles the event
     */                  
    virtual CFepUiBaseCtrl* HandlePointerUpEventL(const TPoint& aPoint);
    
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

    inline CVirtualKeyboard* Keyboard();
    
    inline CVirtualKey* VirtualKey();

    TBool Contains(const TPoint& aPt);
    
    void SetVirtualKeyImgID(TVirtualKeyDrawInfo aVirtualKeyDrawInfo);
    
protected:
    /**
     * 2nd phase constructor
     *
     * @since S60 V5.0
     */    
    void ConstructL();

    /**
     * Report key event to outside
     *
     * @since S60 V5.0
     * @param aEventType The event type for current key event
     */                     
    void ReportKeyEvent(TControlEvent aEventType);    
    
    /**
     * Constructor
     *
     * @since S60 V5.0
     * @param aKeyboard The key board contains the key
     * @param aKeyInfo The virtual key information
     */                         
    CVirtualKeyCtrl(CVirtualKeyboard* aKeyboard,CVirtualKey *aKeyInfo);
       
private:
    /**
     * Draw dimmed key
     *
     * @since S60 V5.0     
     */        
    void DrawDimKey();
    
    /**
     * Draw highlighed key
     *
     * @since S60 V5.0     
     */            
    void DrawHighlightKey();
    
    /**
     * Draw key in normal state (non-highlight/non-dimmed)
     *
     * @since S60 V5.0     
     */            
    void DrawNormalStateKey();            
    
    /**
     * Draw key text
     *
     * @since S60 V5.0     
     */            
    void DrawKeyText();
    
    /**
     * Redraw and update the key
     *
     * @since S60 V5.0     
     */            
    void ReDraw();
    
    /**
     * Draw a key area with the given bitmap
     *
     * @since S60 V5.0     
     * @param aDestRect The rect to be drawn
     * @param aSourceRect The source area in the bitmap
     * @param aBmp The bitmap used
     * @param aMask The mask bitmap
     * @param aInvertFlag The flag controls the mask effect. See DrawBitmapMasked.
     */                
    void DrawBmp(const TRect& aDestRect,const TRect& aSourceRect,
                    const CFbsBitmap* aBmp,CFbsBitmap* aMask,
                    TBool aInvertFlag = EFalse);

    /**
     * Get the control event for given internal event type
     *
     * @since S60 V5.0     
     * @param aEvent Internal event type
     * @return The event type after conversion.
    */                                
    virtual TControlEvent GetKeyEventType(TVirtualKeyEventType aEvent);
    
    TBool MaskBmpContains(const TPoint& aPt);
    CFbsBitmap* MaskBmp();
    
    void DrawIrregularKey(TVirtualKeyIrregularKeyState aKeyState, 
                          TVirtualKeyIrregularKeyState aKeyStateMask);
    
private:
    /** 
     * virtual key information
     * Not own
     */    
    CVirtualKey* iKeyInfo;
    
    /** 
     * virtual keyboard
     * Not own
     */    
    CVirtualKeyboard* iKeyboard;    
    /**
     * scan code. This is used to when searching key by scancode.
     */    
    TInt iScanCode;       
    
    TVirtualKeyDrawInfo iVirtualKeyDrawInfo;     
    
friend class CVirtualKeyboard;    
    };
    
inline CVirtualKeyboard* CVirtualKeyCtrl::Keyboard()
    {
    return iKeyboard;
    }

inline CVirtualKey* CVirtualKeyCtrl::VirtualKey()
	{
	return iKeyInfo;
	}
    
//class CVirtualRawKeyCtrl
/*
CVirtualRawKeyCtrl differes with CVirtualKeyCtrl in that when pressed, it generates
raw key event ( by simulating) which works just like a hardware key
*/
NONSHARABLE_CLASS(CVirtualRawKeyCtrl) : public CVirtualKeyCtrl    
    {
public:    
    /**
     * Factory function.        
     *
     * @since S60 V5.0        
     * @param aKeyboard The keyboard contains the key
     * @param aKeyInfo The virtual key information data
     * @return An instance of CVirtualKeyCtrl class        
     */ 
    static CVirtualRawKeyCtrl* NewL(CVirtualKeyboard* aKeyboard,CVirtualKey* aKeyInfo);
    
    /**
     * Cancel pointer down event.
     * This will be called when a control has pointer down but pointer up 
     * event happens in other ways. If overrided, this base implementation 
     * must be called in order to report the event.
     *
     * @since S60 V4.0
     * @param aFocusedFlag ETrue if got focused, otherwise EFalse
     */                                
    virtual void CancelPointerDownL();

    /**
     * From CFepUiBaseCtrl
     * Handle pointer levae event. 
     * This will be called only when it's got pointer down event before and 
     * now pointer leaves without pointer up event
     *
     * @since S60 V5.0
     * @param aPoint current pointer position
     */
    virtual void HandlePointerLeave(const TPoint& aPoint);
private:
    /**
     * Constructor
     *
     * @since S60 V5.0        
     * @param aKeyboard The keyboard contains the key
     * @param aKeyInfo The virtual key information data
     */ 
    CVirtualRawKeyCtrl(CVirtualKeyboard* aKeyboard,CVirtualKey* aKeyInfo);
    
    //from base CVirtualKeyCtrl
    /**
     * Get the control event for given internal event type
     *
     * @since S60 V5.0     
     * @param aEvent Internal event type
     * @return The event type after conversion.
    */                                        
    virtual TControlEvent GetKeyEventType(TVirtualKeyEventType aEvent);
    };  
    
#endif //C_CVIRTUALKEYBOARD_KEY_CTRL_H

//end of file