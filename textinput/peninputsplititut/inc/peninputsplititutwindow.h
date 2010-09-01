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
* Description:  drop-down list control
*
*/


#ifndef C_SPLITITUTWINDOW_H
#define C_SPLITITUTWINDOW_H

#include <peninputlayout.h>
#include <peninputpluginutils.h>
#include <peninputinputcontextfield.h>
#include <peninputlayoutmultilineicf.h>
#include <peninputlayoutvkb.h>

#include "peninputsplititutwindowmanager.h"
#include "peninputsplititutpropertysubscriber.h"
#include "peninputsplititutlayoutcontext.h"

class CSplitItutDataMgr;
class CFepCtrlDropdownList;
class CAknFepCtrlCommonBgCtrl;
class CPeninputSyncBitmapRotator;

const TInt KUnavailableID = -1;

/**
 *  class CSplitItutWindow.
 *
 *  Split itu-t window class.
 *
 *  @lib peninputsplititut.lib
 *  @since S60 v5.0
 */
class CSplitItutWindow : public CBase
    {
private:

    enum TButtonType
        {
        ERawKeyButton,
        ECommonButton,
        ERepeatButtonEx
        };
    
    enum TIndicatorAlign
    	{
    	EIndiAlignLeft,
    	EIndiAlignCenter,
    	EIndiAlignRight	
    	};
public:
    /** 
     * Symbian constructor.
     *
     * @since S60 5.0
     * 
     * @param aWindowMgr
     * @param aLayoutOwner
     * @param aDataMgr
     * @return Pointer to created CSplitItutWindow object
     */
    static CSplitItutWindow* NewL(CSplitItutWindowManager* aWindowMgr, 
                                    CSplitItutUiLayout* aLayoutOwner,
                                    CSplitItutDataMgr* aDataMgr);

    /** 
     * tandard c++ destructor.
     *
     * @since S60 5.0
     * @return None
     */
    ~CSplitItutWindow();

    
    /**
     * Get Icf pointer
     *
     * @since S60 v5.0
     * @return CFepLayoutMultiLineIcf*
     */
    inline CFepLayoutMultiLineIcf* Icf();
    /** 
     * get pen input type
     *
     * @since S60 5.0
     * @return peninput type
     */
    TInt PenInputType(); 

    /** 
     * set property
     *
     * @since S60 5.0
     * @param aPropertyName
     * @param aPropertyValue
     * @return peninput type
     */
    void SetPropertyL( MItutPropertySubscriber::TItutProperty aPropertyName, 
                       const TDesC& aPropertyValue);

    /** 
     * handle size change
     *
     * @since S60 5.0
     * @return TInt
     */
    TInt SizeChanged();

    /** 
     * get control
     *
     * @since S60 5.0
     * @param aCtrlId
     * @return return control pointer
     */
    CFepUiBaseCtrl* Control(const TInt aCtrlId);

    /** 
     * create chinese spec controls
     *
     * @since S60 5.0
     * @return None
     */
    void CreateChineseSpecificCtrlsIfNeededL();
    
    /** 
     * get itut keypad
     *
     * @since S60 5.0
     * @return return itut keypad
     */
    inline CVirtualKeyboard* ItutKeypad();
    
    /** 
     * construct itut keypad
     *
     * @since S60 5.0
     * @param aResId
     * @return None
     */
    void ConstructItutKeypadFromResourceL(TInt aResId);  
    
    /** 
     * construct icf from resource
     *
     * @since S60 5.0
     * @return None
     */
    void ConstructIcfFromResourceL();
    
    /** 
     * handle skin change
     *
     * @since S60 5.0
     * @return None
     */
    void OnSkinChange(); 
    
    /** 
     * apply variant laf data
     *
     * @since S60 5.0
     * @param aResolutionChange
     * @return None
     */
    void ApplyVariantLafDataL(TBool aResolutionChange = EFalse);
         
    /** 
     * apply spell variant laf data
     *
     * @since S60 5.0
     * @return None
     */         
    void ApplyVariantLafDataForSpellL();     
                        
    /** 
     * construct spell control
     *
     * @since S60 5.0
     * @return None
     */
    void ConstructSpellCtrlFromResourceL();
    
    /** 
     * set hard key
     *
     * @since S60 5.0
     * @param aData
     * @return None
     */
    void SetHardKeyOneSymbol(TUint8* aData);
    
    /** 
     * set hard key
     *
     * @since S60 5.0
     * @param aData
     * @return None
     */
    void SetHardKeyOneSymbolL(TUint8* aData);
    
    /** 
     * handle button res
     *
     * @since S60 5.0
     * @param aControlId
     * @return None
     */
    void HandleButtonResOnLangDirChange( TInt aControlId );
    
    /** 
     * handle button res
     *
     * @since S60 5.0
     * @param aControlId
     * @return None
     */
    void HandleButtonResOnLangDirChangeL( TInt aControlId );

    /** 
     * Set prompt text
     *
     * @since S60 5.0
     * @param aData text
     * @return None
     */
    void SetPromptTextL( TUint8* aData );
    
    /** 
     * Set bubble
     *
     * @since S60 5.0
     * @return None
     */    
    void SetIndiBubble();
    
    /** 
     * Update bubble prompt text
     *
     * @since S60 5.0
     * @param aData text
     * @return None
     */
    void UpdateIndiBubbleL( TUint8* aData );
    
    /** 
     * Set bubble image
     *
     * @since S60 5.0
     * @param aImgID1 image ID1
     * @param aMaskID1 mask image id1
     * @param aImgID2 image ID2
     * @param aMaskID2 mask image id2
     * @return None
     */
    void SetIndiBubbleImageL( const TInt aImgID1,
						      const TInt aMaskID1,
                              const TInt aImgID2,
                              const TInt aMaskID2 );
private:
    /**
     * C++ constructor
     *
     * @since S60 5.0
     * @param aWindowMgr
     * @param aLayoutOwner
     * @param aDataMgr
     * @return none
     */
    CSplitItutWindow(CSplitItutWindowManager* aWindowMgr, 
                       CSplitItutUiLayout* aLayoutOwner,
                       CSplitItutDataMgr* aDataMgr);
                       
    /**
     * Symbian second-phase constructor
     *
     * @since S60 5.0
     * @return none
     */
    void ConstructL(); 

    /**
     * create itut keypad
     *
     * @since S60 5.0
     * @return none
     */
    void CreateItutKeypadL();

    /**
     * create layout icons
     *
     * @since S60 5.0
     * @param aMajorSkinId
     * @param aMinorSkinId
     * @param aBmpFileName
     * @param aBmpId
     * @param aMaskBmpId
     * @param aBmp
     * @param aMaskBmp
     * @param aSize
     * @return none
     */
    void CreateLayoutIconL(TInt aMajorSkinId,
                           TInt aMinorSkinId,
                           const TDesC& aBmpFileName,
                           TInt aBmpId,
                           TInt aMaskBmpId,
                           CFbsBitmap*& aBmp,
                           CFbsBitmap*& aMaskBmp,
                           TSize aSize);
                           

    /**
     * create virtual keys
     *
     * @since S60 5.0
     * @param aReader
     * @param aKeyRect
     * @return return virtual key
     */
    CVirtualKey* CreateKeyL(TResourceReader& aReader,
                            TRect aKeyRect);

    /**
     * create virtual keys
     *
     * @since S60 5.0
     * @param aReader
     * @param aKeyRect
     * @return None
     */
    void ChangeVirtualKeyInfoL(TResourceReader& aReader,
                              CVirtualKey* aKey);

    /**
     * create all buttons
     *
     * @since S60 5.0
     * @return None
     */
    void CreateAllButtonL();
    
    /**
     * create back ground control
     *
     * @since S60 5.0
     * @param aReader
     * @param aKeyRect
     * @return None
     */
    void CreateBackGroundControlL();

    /**
     * create dropdown list
     *
     * @since S60 5.0
     * @param aReader
     * @param aKeyRect
     * @return None
     */
    void CreateDropdownListL();

    /**
     * create virtual keys
     *
     * @since S60 5.0
     * @param aCtrl
     * @param aRectIdx
     * @return None
     */
    void SetCtrlRect(CFepUiBaseCtrl* aCtrl, TInt aRectIdx, const TBool aOffset = EFalse);
    
    /**
     * resize candidate list
     *
     * @since S60 5.0
     * @param aList
     * @param aLTPosIdx
     * @param aExpandable
     * @return None
     */
    void ResizeCandidateList( CFepCtrlDropdownList* aList, 
                              TInt aLTPosIdx, 
                              TBool aExpandable);
    
    /**
     * Calculate frame rects
     *
     * @since S60 5.0
     * @param aRect
     * @param aOuterRect
     * @param aInnerRect
     * @return None
     */
	void CalculateFrameRects(const TRect aRect,
							 TRect& aOuterRect, 
						     TRect& aInnerRect);
						     
    /**
     * create rawkey buttons
     *
     * @since S60 5.0
     * @param aControlId
     * @param aCommand
     * @return raw key button
     */
    CAknFepCtrlCommonButton* CreateRawKeyButtonL( const TInt aControlId,
                                                  const TInt aCommand);

    /**
     * get common button control
     *
     * @since S60 5.0
     * @param aControlId
     * @return common button control
     */
    CAknFepCtrlCommonButton* CommonButtonControl(const TInt aControlId);

    /**
     * create repeat buttons
     *
     * @since S60 5.0
     * @param aControlId
     * @param aCommand
     * @return repeat button
     */
    CAknFepCtrlCommonButton* CreateRepeatButtonL( const TInt aControlId,
                                                  const TInt aCommand);
                                        
    /**
     * create common buttons
     *
     * @since S60 5.0
     * @param aControlId
     * @return common button
     */
    CAknFepCtrlCommonButton* CreateCommonButtonL(const TInt aControlId);
                                        
    /**
     * create buttons
     *
     * @since S60 5.0
     * @param aType
     * @param aControlId
     * @param aRectId
     * @param aInnerRectId
     * @param aResourceId
     * @param aCommand
     * @return button
     */
    CFepUiBaseCtrl* CreateButtonL(TButtonType aType,
                                  const TInt aControlId,
                                  const TInt aRectId,
                                  const TInt aInnerRectId,
                                  const TInt aResourceId,
                                  const TInt aCommand = KUnavailableID);
                                  
    /**
     * Create ICF for spell mode
     *
     * @since S60 5.0
     * @return None
     */
    void CreateICFL();
     
    /**
     * Check control size change
     *
     * @since S60 5.0
     * @param aRect
     * @param aControlId
     * @param aInnerRect
     * @param aIsReloadImages
     * @return TBool
     */
    TBool ControlSizeChanged(const TInt aControlId, 
                            const TInt aRect, 
                            const TInt aInnerRect, 
                            TBool aIsReloadImages);
                                        
    /**
     * add control
     *
     * @since S60 5.0
     * @param aControl
     * @return None
     */
    void AddControlL(CFepUiBaseCtrl* aControl);
    
    /**
     * is chinese mode
     *
     * @since S60 5.0
     * @return TBool
     */
    TBool IsChineseMode();
    
    /**
     * construct key image
     *
     * @since S60 5.0
     * @param aKeyImageResID
     * @return None
     */
    void ConstructKeyImageFromResourceL( TInt aKeyImageResID );
  
    /**
     * construct key image
     *
     * @since S60 5.0
     * @param aKey
     * @param aMapData
     * @return None
     */
    void SetUnicodesForHardKey1(CVirtualKey* aKey, const TDesC& aMapData);
    
    /**
     * construct key image
     *
     * @since S60 5.0
     * @param aKeyImageResID
     * @return None
     */
    void SetUnicodesForHardKey1L(CVirtualKey* aKey, const TDesC& aMapData);
    
    /**
     * Indicator rect
     *
     * @since S60 5.0
     * @param aBoundRect
     * @param aRealRect1
     * @param aRealRect2
     * @param aAlign
     * @return None
     */ 
    void CalIndicatorRect( const TRect& aBoundRect,
                           TRect& aRealRect1,
                           TRect& aRealRect2,
                           TIndicatorAlign aAlign );
private:
    /**
     * ITUT keypad.
     * own
     */
    CVirtualKeyboard* iStandardItutKp;

    /**
     * spell candidate list.
     * own
     */
    CFepCtrlDropdownList* iSpellCandsList;
    
    /**
     * candidate list.
     * own
     */
    CFepCtrlDropdownList* iCandsList;
    
    /**
     * Punc candidate list.
     * own
     */
    CFepCtrlDropdownList* iPuncCandsList;

    /**
     * data manager.
     * not own
     */
    CSplitItutDataMgr* iDataMgr;

    /**
     * init chinese control flag.
     */
    TBool iChnCtrlInited;   
 
    /**
     * layout owner.
     * not own
     */
    CSplitItutUiLayout* iLayoutOwner;
    
    /**
     * window manager
     * not own
     */
    CSplitItutWindowManager* iWindowMgr;
    
    /**
     * Background control
     * Own
     */
    CAknFepCtrlCommonBgCtrl* iBackgroundCtrl;
    
    /**
     * Spell candidate list
     * Own
     */
    CFepInputContextField* iSpell;
    
    /**
     * bitmap rotator
     * Own
     */
	CPeninputSyncBitmapRotator* iBmpRotator;
	
    /**
     * symbol data for chinese
     */
    TFepSymbolOfHardwareOne iSymbolData;
    
    TSize iIndicatorSize;
    TSize iIndicatorTextSize;
    TBool iImDimensionSet;
    CFepLayoutMultiLineIcf* iICF;
    };
   
// ---------------------------------------------------------------------------
// CSplitItutWindow::ItutKeypad
// ---------------------------------------------------------------------------
//
inline CVirtualKeyboard* CSplitItutWindow::ItutKeypad()
    {
    return iStandardItutKp;    
    }
  
// ---------------------------------------------------------------------------
// CSplitItutWindow::Icf
// ---------------------------------------------------------------------------
//
inline CFepLayoutMultiLineIcf* CSplitItutWindow::Icf()
    {
    return iICF;   
    }
#endif //C_SPLITITUTWINDOW_H
