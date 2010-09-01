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
* Description:  header file of choice list window
*
*/

#ifndef C_FEPLAYOUTINPUTMODELCHOICE_H
#define C_FEPLAYOUTINPUTMODELCHOICE_H

// INCLUDES
#include <w32std.h>
#include <e32base.h>
#include <peninputcommonchoicelist.h>
#include <AknFepGlobalEnums.h>

/** class forward decalaration */
class CFepUiLayout;
class CRepository;

//CONSTANT
const TInt KTouchInputModeCount = 4;

/**
 *  CPeninputLayoutInputmodelChoice class 
 *
 *  This class for touch input mode switch 
 *
 *  @lib peninputmodeswitchutils
 *  @since S60 v5.0
 */
class CPeninputLayoutInputmodelChoice : public CPeninputCommonChoiceList
    {

public: 

    /**
     * Symbian Constructor
     *
     * @since S60 v5.0
     * @param aUiLayout Pointer to uilayout
     * @param aControlId Choice list id
     * @param aInputMode Pen input mode
     * @return Pointer to created CPeninputLayoutInputmodelChoice object
     */
    IMPORT_C static CPeninputLayoutInputmodelChoice* NewL(CFepUiLayout* aUiLayout, TInt aControlId,
                                                          TPluginInputMode aInputMode);

    /**
     * Destructor
     *
     * @since S60 v5.0
     * @return None
     */
    IMPORT_C virtual ~CPeninputLayoutInputmodelChoice();
    
   
    /**
     * Pop up touch input switch list
     *
     * @since S60 v5.0
     * @param aPoint Reference to pop up windows left and top point 
     * @param aListUnderBtn list is under the button
     * @return none
     */
    IMPORT_C void PopUpSwitchListL( const TRect& aRtControl );
    
    /**
     * Set current editor is Secret
     *
     * @since S60 V4.0
     * @param aLanguage Input language
     * @return none
     */                                              
    IMPORT_C void SetSecretEditor(TBool aSecure);
     
     /**
     * Get switch list rect
     *
     * @since S60 v5.0
     * @return list rect
     */
     IMPORT_C TRect SwitchListRect(); 

private:

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v5.0
     * @return None
     */
    void CustomConstructL();
   
   /**
     * Initialize internal control
     *
     * @since S60 v5.0
     * @return none
     */
    void DoInitializeControlL();    
    
    /**
     * C++ constructor.
     *
     * @since S60 v5.0
     * @param aUiLayout Pointer to uilayout
     * @param aInputMode Pen input mode   
     * @return None
     */
    CPeninputLayoutInputmodelChoice(CFepUiLayout* aUiLayout, TInt aControlId, 
                                    TPluginInputMode aInputMode);
    
    /**
     * Insert choice item to input mode switch list
     *
     * @since S60 v5.0
     * @param aReader Reference to resource reader   
     * @param aRsFile Reference to file resource
     * @return None
     */
    void CreateChoiceItemL( TResourceReader &aReader, RResourceFile& aRsFile );
 
     /**
     * Create choice item icon
     *
     * @since S60 v5.0
     * @param aReader Reference to resource reader   
     * @param aBmpFileName Reference to file name
     * @param aMajorSkinId Major skin id
     * @return ETrue for creating icon successfully; otherwise EFalse
     */
    TBool CreateItemIconL(TResourceReader &aReader, CFbsBitmap** aImg, TPtrC& aBmpFileName, 
        TInt32 aMajorSkinId);
        
     /**
     * Get main pane rect
     *
     * @since S60 v5.0
     * @return Main pane rect
     */
    TRect MainPaneRect();
     
     /**
     * nsert list item
     *
     * @since S60 v5.0
     * @param aIndex Item array index
     * @param aInputMode Pen input mode
     * @return none
     */
     void InsertListItemL( const CFepLayoutChoiceList::SItem& item,TInt aInputMode );

        /**
     * Application size Changed
     *
     * @since S60 v5.0
     * @return none
     */
    void SizeChanged(); 
    
        
    /**
     * Get hide flag
     *
     * @since S60 V4.0
     * @return ETrue if control is hiden, otherwise EFalse
     */                                              
    TBool Hiden();
    
    /**
     * Reset choice list item
     *
     * @since S60 v5.0
     * @return none
     */
     void ResetListItemL(); 
private: // data

    CFepUiLayout* iUiLayout;   
    /**
     * The background bitmap of popup window
     */      
    CFbsBitmap* iPopupWndBgImg;
    
    /**
     * The focus bitmap of popup window (not own)
     */  
    CFbsBitmap* iPopupWndFocusImg;
    
    /**
     * Popup window item size
     */
    TSize iPopupItemSize;  
    
    /**
     * This control is construced
     */
    TBool iConstructed; 

    /**
     * This control is construced
     */
    TPluginInputMode iCurPluginInputMode;
    
    /**
     * Input mode item array
     */
    //CFepLayoutChoiceList::SItem iInputModeItem[KTouchInputModeCount];

    RArray<CFepLayoutChoiceList::SItem> iInputModeItems;
    /**
     * Editor secure flag
     */
    TBool iSecretEditor;

    /**
     * Editor secure flag
     */
    TRect iItemRect;

    
    /**
     *  Text input language
     */
    TLanguage iInputLanguage;
    
    /**
     *  fep repository key
     */
     CRepository*  iAknFepRepository;

    };

//#include "peninputlayoutchoicelist.inl"
#endif //C_FEPLAYOUTINPUTMODELCHOICE_H
//End Of File
