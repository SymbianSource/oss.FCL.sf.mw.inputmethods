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
* Description:  boxbase hwr window
*
*/


#ifndef C_CAKNFEPHWRBXWND_H
#define C_CAKNFEPHWRBXWND_H

// INCLUDES
#include <peninputbasewindow.h>
#include "peninputhwrpropertysubscriber.h"

// FORWARD DECLARATIONS
class CFepInputContextField;
class CAknFepCtrlButton;
class CAknFepHwrBxCtrlBar;
class CAknFepHwrBoxGroup;
class CAknFepHwrBxDataStore;
class CFepCtrlDropdownList;
class TResourceReader;
class CPeninputLayoutInputmodelChoice;
class CAknFepCtrlRepeatButton;

/**
 *  boxbased hwr window
 *
 *  @lib peninputhwrboxcn.lib
 *  @since S60 v3.2
 */
class CAknFepHwrBxWnd : public CAknFepCtrlBaseWindow, public MAknFepHwrPropertySubscriber
    {
            
public:

    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout The layout
     * @param aControlId The control id
     * @param aLandscapeStyle Current screen is landscape or not
     * @return Pointer to created CAknFepHwrBxWnd object
     */
    static CAknFepHwrBxWnd* NewL(CFepUiLayout* aFepUiLayout, 
                                 TInt aControlId,
                                 TBool aLandscapeStyle);
                
    /**
     * Symbian constructor.
     *
     * @since S60 v3.2
     * @param aUiLayout The layout
     * @param aControlId The control id
     * @param aLandscapeStyle Current screen is landscape or not
     * @return Pointer to created CAknFepHwrBxWnd object
     */
    static CAknFepHwrBxWnd* NewLC(CFepUiLayout* aFepUiLayout, 
                                  TInt aControlId,
                                  TBool aLandscapeStyle);
        
    /**
     * destructor.
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CAknFepHwrBxWnd();
    
    /** 
     * ConstructFromResourceL load from resource.
     *
     * @since S60 v3.2
     * @param aReader A resource reader
     * @return None
     */
    void ConstructFromResourceL(TResourceReader& aReader); 

    /** 
     * ConstructFromResourceL load from resource.
     * Before calling this function, resource id should be set
     *
     * @since S60 v3.2
     * @return None
     */
    void ConstructFromResourceL(); 

    /**
     * Set the input context field text
     *
     * @since S60 v3.2
     * @param aTotalTextLen The total text length in editor
     * @param aStartPos The position of aText in whole editor
     * @param aCursorSel Current cursor selection
     * @param aText The text to be set 
     * @return None
     */          
    void SetEditorTextL(const TFepInputContextFieldData& aData);

    /**
     * Highlight the dropdown list cell
     *
     * @since S60 v3.2
     * @param aIndex The cell index
     * @return None
     */          
    void HighlightCell(TInt aIndex);
    
    /**
     * Cancel wrting
     *
     * @since S60 v3.2
     * @return None
     */          
    void CancelWriting();
    
    /**
     * Cancel repeat
     *
     * @since S60 v3.2
     * @return None
     */          
    void CancelRepeat();
    
    /**
     * Cancel repeat
     *
     * @since S60 v3.2
     * @param aFlag The dim flag
     * @return None
     */          
    void DimInputContextField(TBool aFlag);
    
    /**
     * Pup up pen input mode switcher
     *
     * @since S60 v3.2
     * @return None
     */          
    void PopUpPenInputModeSwitcher();

// from base class CAknFepCtrlBaseWindow

    /**
     * From CAknFepCtrlBaseWindow.
     * Resize according to LAF
     *
     * @since S60 v3.2
     * @param aLandscapeStyle Current screen is landscape or not
     * @return None
     */          
    void SizeChanged(TBool aLandscapeStyle);
    
    /**
     * From CAknFepCtrlBaseWindow.
     * call back function when closing window
     *
     * @since S60 v3.2
     * @return ETrue for allowing close window; otherwise EFalse
     */          
    TBool DoClose();

// from base class MAknFepHwrPropertySubscriber

    /**
     * From MAknFepHwrPropertySubscriber.
     * Handle the property change
     *
     * @since S60 v3.2
     * @param aPropertyName The property name 
     * @param aPropertyValue The new value
     * @return None
     */
    void SetPropertyL(const MAknFepHwrPropertySubscriber::TAknFepHwrProperty aPropertyName, 
                     const TDesC& aPropertyValue);
    
    /**
     * Draw window and shadow
     *
     * @since S60 v3.2
     * @return None
     */          
    void Draw();
    
    /**
     * Get drop down list
     *
     * @since S60 v3.2
     * @return Pointer to drop down list
     */          
    CFepCtrlDropdownList* DropdownList();
    
    TPoint StrokeEndMark();

    /**
     * Get top left shadow size
     *
     * @since S60 v3.2
     * @return Top left corner shadow size
     */          
    TSize ShadowTlSize();
    
    /**
     * Get bottom right shadow size
     *
     * @since S60 v3.2
     * @return Bottom right corner shadow size
     */          
    TSize ShadowBrSize();
    
    /**
     * Set if enable option button and language switch button
     *
     * @since S60 v3.2
     * @param aEnable The enable flag
     * @return None
     */ 
    void SetEnableSettingBtn(TBool aEnable);
    
    /**
     * Reset writting window
     *
     * @since S60 v3.2
     * @return None
     */ 
    void ResetWrittingWnd();
    
    void SetTextAlignmentL( TInt aAlignment );
    
protected:

    /**
     * C++ default constructor
     *
     * @since S60 v3.2
     * @param aFepUiLayout A Ui Layout environment (CFepUiLayout)
     * @param aControlId Control id
     * @return None
     */          
    CAknFepHwrBxWnd(CFepUiLayout* aFepUiLayout, TInt aControlId);
    
private:

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v3.2
     * @param aLandscapeStyle Current screen is landscape or not
     * @return None
     */          
    void ConstructL(TBool aLandscapeStyle);

    /**
     * Add input context field to window
     *
     * @since S60 v3.2
     * @return None
     */
    void AddContextFieldL();

    /**
     * Add option button to window
     *
     * @since S60 v3.2
     * @return None
     */
    void AddOptionButtonL();
    
    /**
     * Add control bar to window
     *
     * @since S60 v3.2
     * @return None
     */
    void AddControlBarL();
    
    /**
     * Add box group to window
     *
     * @since S60 v3.2
     * @return None
     */
    void AddBoxGroupL();
    
    /**
     * Add dropdown list to window
     *
     * @since S60 v3.2
     * @param aLandscapeStyle Current screen is landscape or not
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
     * Add backspace button to window
     *
     * @since S60 v3.2
     * @return None
     */
    void AddBackspaceBtnL();
    
    /**
     * read background info of base window
     *
     * @since S60 v3.2
     * @return None
     */   
    void ReadBackgroundInfoL( TInt aResId );
    
private:// data

    /**
     * The input context field (Own)
     */
    CFepInputContextField* iContextField;
    
    /**
     * The touch input option button (Own)
     */
    CAknFepCtrlCommonButton* iOptionButton;

    /**
     * Instance of back space button (own)
     */
    CAknFepCtrlRepeatButton* iBackspace;       
    
    /**
     * The control bar (Own)
     */
    CAknFepHwrBxCtrlBar* iControlBar;
    
    /**
     * The drop down list control (Own)
     */    
    CFepCtrlDropdownList* iDropdownList;
     
    /**
     * The hwr box group (Own)
     */
    CAknFepHwrBoxGroup* iBoxGroup;

    /**
     * The data store (Not own)
     */
    CAknFepHwrBxDataStore* iDataStore;
    
    /**
     * The drop down list font (own)
     */
    // removed in 2006.02.10, font get from LAF now
    //CFont* iDropdownListFont;
    
    /**
     * second range color
     */
    TRgb iSecondRangeColor;
 
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

    /**
     * First time construct flag
     */
    TBool iFirstTimeConstruct;
    
    /**
     * Indicate whether unit size and shadow tl br size
     * have been set
     */
    TBool iUnitShadowSizeSet;
    
    /**
     * Indicate whether sub controls size and position
     * have been set
     */
    TBool iControlPosSet;
    
    CPeninputLayoutInputmodelChoice* iInputModeSwitch;    
    };

#endif // C_CAKNFEPHWRBXWND_H

// End Of File
