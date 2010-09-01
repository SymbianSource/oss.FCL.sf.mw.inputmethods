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


#ifndef C_GENERICITUTWINDOW_H
#define C_GENERICITUTWINDOW_H

#include <peninputlayout.h>
#include <peninputpluginutils.h>
#include <peninputlayoutmultilineicf.h>
#include <peninputinputcontextfield.h>

#include <peninputlayoutvkb.h>

#include "peninputgenericitutwindowmanager.h"
#include "peninputgenericitutpropertysubscriber.h"
#include "peninputgenericitutlayoutcontext.h"

class CGenericItutDataMgr;
class CFepCtrlDropdownList;
class CPeninputLayoutInputmodelChoice;
class CFepLayoutScrollableList;
class CAknFepCtrlCommonBgCtrl;
class CPeninputCommonChoiceList;
class CPeninputSyncBitmapRotator;

const TInt KUnavailableID = -1;

class CGenericItutWindow : public CBase
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
    static CGenericItutWindow* NewL(CGenericItutWindowManager* aWindowMgr, 
                                    CGenericItutUiLayout* aLayoutOwner,
                                    CGenericItutDataMgr* aDataMgr);

    ~CGenericItutWindow();

    TInt OnAppEditorTextComing(const TFepInputContextFieldData& aData);

    TInt PenInputType(); 

    // from MITUTPropertySubscriber
    void SetPropertyL( MItutPropertySubscriber::TItutProperty aPropertyName, 
                       const TDesC& aPropertyValue);

    TInt SizeChanged(TBool aIcfOverlap);

    // from MItutLayoutContext
    CFepUiBaseCtrl* Control(const TInt aCtrlId);
    
    inline TBool IsSecretEdtior();

    // drop down list
    // stroke zhuyin spell ctrl
    void CreateChineseSpecificCtrlsIfNeededL();
    
    void EnterMatchSelectionState(TBool aOn);

	void CalIndicatorRect(const TRect& aBoundRect,
	                          TRect& aRealSize1,
	                          TRect& aRealSize2,
	                          TIndicatorAlign aAlign = EIndiAlignRight);
	void ResetIndicatorL(CFepUiBaseCtrl* aIndicator);
	
	void DisplayInputmodeSwitchL();
	
	inline CFepLayoutMultiLineIcf* Icf();
	inline CPeninputLayoutInputmodelChoice* InputModeSwitch();
	inline CFepLayoutScrollableList* MatchSelection();
    inline CVirtualKeyboard* ItutKeypad();
    void SetPromptTextL(TUint8* aData);
    
    
    void ConstructItutKeypadFromResourceL(TInt aResId);  
    
    void ConstructIcfFromResourceL();
    void OnSkinChange();  
    void ApplyVariantLafDataL(TBool aResolutionChange = EFalse);
    
    void SetIndicatorImageL(CFepUiBaseCtrl* aIndicator,
                            const TInt aImgID1,
                            const TInt aMaskID1,
                            const TInt aImgID2,
                            const TInt aMaskID2); 
                            
    void ConstructSpellCtrlFromResourceL();
    
    void UpdateIndicatorL(TUint8* aData);
    
    inline CPeninputCommonChoiceList* EditorMenu();   
    
    void ShowEditorMenuL(TInt* aData);
    
    void SetHardKeyOneSymbol(TUint8* aData);
    void SetHardKeyOneSymbolL(TUint8* aData);
    
    void HandleButtonResOnLangDirChange( TInt aControlId );
    void HandleButtonResOnLangDirChangeL( TInt aControlId );
    void IndiBubbleWithText();
    void IndiBubbleWithoutText();
    void UpdateIndiBubbleL( TUint8* aData );
    void SetIndiBubbleImageL( const TInt aImgID1,
                        const TInt aMaskID1,
                        const TInt aImgID2,
                        const TInt aMaskID2 );
    void SetIndiWithTextFlag( TBool aFlag );
    void MsgBubbleForChinese();
    void SetSpellDisplayContentL( TUint8* aData );
    void CreateKoreanSpecificCtrlsIfNeededL();
private:
    CGenericItutWindow(CGenericItutWindowManager* aWindowMgr, 
                       CGenericItutUiLayout* aLayoutOwner,
                       CGenericItutDataMgr* aDataMgr);

    void ConstructL(); 

    // called when first time construct itut keypad
    void CreateItutKeypadL();

    void CreateLayoutIconL(TInt aMajorSkinId,
                           TInt aMinorSkinId,
                           const TDesC& aBmpFileName,
                           TInt aBmpId,
                           TInt aMaskBmpId,
                           CFbsBitmap*& aBmp,
                           CFbsBitmap*& aMaskBmp,
                           TSize aSize);
                           
    void CreateMatchSelectionCtrlL();

    // aLeftTopPos: the offset of top most and left most key relative to keypad
    CVirtualKey* CreateKeyL(TResourceReader& aReader,
                            TRect aKeyRect);

    void ChangeVirtualKeyInfoL(TResourceReader& aReader,
                              CVirtualKey* aKey);




    void CreateAllButtonL();
    
    void CreateBackGroundControlL();

    void CreateICFL();

    void CreateDropdownListL();



    void SetCtrlRect(CFepUiBaseCtrl* aCtrl, TInt aRectIdx);
    
    void ResizeCandidateList(CFepCtrlDropdownList* aList, TInt aLTPosIdx, TBool aExpandable);
    
	void CalculateFrameRects(const TRect aRect,
							 TRect& aOuterRect, 
						     TRect& aInnerRect);
						     

    
    TRgb GetMatchListTextColor();
    

    
    
    CAknFepCtrlCommonButton* CreateRawKeyButtonL(const TInt aControlId,
                                        const TInt aCommand);
                                        
    CAknFepCtrlCommonButton* CommonButtonControl(const TInt aControlId);
    
    CAknFepCtrlCommonButton* CreateRepeatButtonL(const TInt aControlId,
                                        const TInt aCommand);
                                        
    CAknFepCtrlCommonButton* CreateCommonButtonL(const TInt aControlId);
                                        
    
    CFepUiBaseCtrl* CreateButtonL(TButtonType aType,
                                  const TInt aControlId,
                                  const TInt aRectId,
                                  const TInt aInnerRectId,
                                  const TInt aResourceId,
                                  const TInt aCommand = KUnavailableID);
                                  
    TBool ControlSizeChanged(const TInt aControlId, 
                            const TInt aRect, 
                            const TInt aInnerRect, 
                            TBool aIsReloadImages);
                                        
    void AddControlL(CFepUiBaseCtrl* aControl);
    
    void AddEditorMenuL();

    TBool IsChineseMode();
    void ConstructKeyImageFromResourceL( TInt aKeyImageResID );
    
    void SetUnicodesForHardKey1(CVirtualKey* aKey, const TDesC& aMapData);
    void SetUnicodesForHardKey1L(CVirtualKey* aKey, const TDesC& aMapData);
private:
    CVirtualKeyboard* iStandardItutKp;
    CFepLayoutMultiLineIcf* iICF;

    CFepCtrlDropdownList* iSpellCandsList;
    CFepCtrlDropdownList* iCandsList;
    CFepCtrlDropdownList* iPuncCandsList;

    CFepLayoutScrollableList* iMatchSelection;

    CGenericItutDataMgr* iDataMgr;

    TBool iPenDownOnNaviPane;

    TBool iChnCtrlInited;
    

    CPeninputLayoutInputmodelChoice* iInputModeSwitch;

    CPeninputCommonChoiceList* iEditorMenu;    
 
    TBool iIsSecret;
    TSize iIndicatorSize;
    TSize iIndicatorTextSize;
    TBool iImDimensionSet;
    
    TInt iLastRawKeyDown;
    TBool iRawKeyDown;
    TBool iLastIsChinese;
    
    TAknWindowLineLayout iBubbleSize;
    TAknTextLineLayout iBubbleTextLayout;
    CGenericItutUiLayout* iLayoutOwner;
    CGenericItutWindowManager* iWindowMgr;
    CAknFepCtrlCommonBgCtrl* iBackgroundCtrl;
    CFepInputContextField* iSpell;
	CPeninputSyncBitmapRotator* iBmpRotator;    
    TFepSymbolOfHardwareOne iSymbolData;
    TBool iIndiWithText;
    };

inline CFepLayoutMultiLineIcf* CGenericItutWindow::Icf()
    {
    return iICF;   
    }
    
inline CPeninputLayoutInputmodelChoice* CGenericItutWindow::InputModeSwitch()
    {
    return iInputModeSwitch;  
    }
    
inline CFepLayoutScrollableList* CGenericItutWindow::MatchSelection() 
    {
    return iMatchSelection;   
    }
    
inline CVirtualKeyboard* CGenericItutWindow::ItutKeypad()
    {
    return iStandardItutKp;    
    }
    
inline CPeninputCommonChoiceList* CGenericItutWindow::EditorMenu()
    {
    return iEditorMenu;    
    }
    
#endif //C_GENERICITUTWINDOW_H
