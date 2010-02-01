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
* Description:  Interface of vkb ui state
*
*/


#ifndef C_AKNFEPVKBDATAMGR_H
#define C_AKNFEPVKBDATAMGR_H

// System includes
#include <e32base.h>
#include <w32std.h>

// User includes
#include "peninputvkbdataprovider.h"
#include <peninputpluginutils.h>

// The max length for language string 
const TInt KMaxLangStringLength = 30;

// The max length for language string 
const TInt KMaxLengthCompFieldElemCount = 50;
const TInt KMaxLengthPredictiveString = KMaxLengthCompFieldElemCount + 
                                        KMaxLengthCompFieldElemCount;

const TInt KMaxPredictCandCnt = 250;

/** class forward decalaration */
class CPtiEngine;
class MAknFepVkbLayoutContext;
class CRepository;

// class declarations
/**
 *  Data manager class
 *
 *  @lib peninputvkbcn.lib
 *  @since S60 v3.2
 */
class CAknFepVkbDataMgr: public CBase, 
                         public MAknFepVkbDataProvider
    {
public:   
    /**
     * struct SLanguageItem
     * Language item struct
     * 
     */
    //struct SLanguageItem
    //    {
        /*
         * the language id(Symbian)
         */
    //    TInt iLangId;
        
        /*
         * the display text for the language corresponding
         */
    //    TBuf<KMaxLangStringLength> iDisplayText;
    //    };
                  
public:

    /**
     * Create one CAknFepVkbWindow object
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The CFepCtrlDropdownList object.
     */ 
    static CAknFepVkbDataMgr* NewL(MAknFepVkbLayoutContext* aContext, 
                                   CPtiEngine* aPtiEngine = NULL);
    
    /**
     * Create one CAknFepVkbWindow object
     *
     * @since S60 v3.2
     * @param aContext The layout context
     * @return The CFepCtrlDropdownList object.
     */ 
    static CAknFepVkbDataMgr* NewLC(MAknFepVkbLayoutContext* aContext, 
                                    CPtiEngine* aPtiEngine = NULL);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None.
     */ 
    virtual ~CAknFepVkbDataMgr();

    /**
     * Reset data in manager
     *
     * @since S60 v3.2
     * @return None.
     */     
    void Reset();
                    
    /**
     * Get candidates
     *
     * @since S60 v3.2
     * @return candidates list.
     */       
    RPointerArray<HBufC> Candidates();

    /**
     * Notify engine to generate candidates
     *
     * @since S60 v3.2
     * @param aInputString The string sent to engine    
     * @param aIsPredictive The predictive flag, if true, tell engine to predict, 
     * @param aForNextPage Is it for next page candidate
     * @return The count of candidates
     */      
    TInt GetCandidatesL(const TDesC& aInputString, 
                        const TBool aIsPredictive,
                        TBool aForNextPage = EFalse);

    /**
     * Notify engine to generate candidates
     *
     * @since S60 v3.2
     * @param aTriggerStr The string sent to engine    
     * @param aPredictives The predictive 
     * @return None
     */      
    void DoPredictive(const TDesC& aTriggerStr, 
                      RPointerArray<HBufC>& aPredictives);

    /**
     * Clear trigger string
     *
     * @since S60 v3.2
     * @return None
     */
    void ClearTriggerString();

    /**
     * Get the count of candidates
     *
     * @since S60 v3.2
     * @return Candidates count.
     */      
    TInt CandidatesCount();
        
    /**
     * Get case
     *
     * @since S60 v3.2
     * @return Case.
     */      
    TInt Case();

    /**
     * Get peninput language
     *
     * @since S60 v3.2
     * @return Peninput language.
     */      
    TInt InputLanguage();

    /**
     * Get permitted range
     *
     * @since S60 v3.2
     * @return Permitted range.
     */      
    TInt PermittedRange();
    
    /**
     * Get current range
     *
     * @since S60 v3.2
     * @return Current range.
     */      
    TInt CurrentRange();

    /**
     * Get current accent
     *
     * @since S60 v3.2
     * @return The current accent.
     */      
    TInt CurrentAccent();
        
    /**
     * Get editor numeric keymap
     *
     * @since S60 v3.2
     * @return The editor numeric keymap.
     */      
    TInt EditorNumericKeymap();
    
    /**
     * Get input method layout
     *
     * @since S60 v3.2
     * @return The input method layout.
     */      
    TInt IMLayout();

    /**
     * Get shift down flag
     *
     * @since S60 v3.2
     * @return The shift down flag.
     */      
    TBool ShiftDown();
    
    /**
     * Get capslock down flag
     *
     * @since S60 v3.2
     * @return The capslock down flag.
     */      
    TBool CapslockDown();
    
    /**
     * Get length in composition field when button down on backspace button
     *
     * @since S60 v3.2
     * @return The length in composition field when button down on backspace button.
     */      
    TInt LengthWhenButtondownOnBack();

    /**
     * Get window position
     *
     * @since S60 v3.2
     * @return The window position.
     */      
    TInt WindowPosition();
    
    /**
     * Set window position
     *
     * @since S60 v3.2
     * @param aPosition The new value
     * @return None.
     */      
    void SetWindowPosition(TInt aPosition);
    
    /**
     * Set candidate count
     *
     * @since S60 v3.2
     * @param aCandidatesCount The new value
     * @return None.
     */         
    void SetCandidatesCount(TInt aCandidatesCount);
    
    /**
     * Set case
     *
     * @since S60 v3.2
     * @param aCase The new value
     * @return None.
     */         
    void SetCase(TInt aCase);
    void SetPermittedCase(TInt aCase);
    TInt PermittedCase();

    /**
     * Set peninput language
     *
     * @since S60 v3.2
     * @param aInputLanguage The new value
     * @return Display language shown on switch button
     */         
    TInt SetInputLanguage(TInt aInputLanguage = -1);

    /**
     * Set permitted range
     *
     * @since S60 v3.2
     * @param aRange The new value
     * @return None.
     */         
    void SetPermittedRange(TInt aRange);

    /**
     * Set current range
     *
     * @since S60 v3.2
     * @param aPosition The new value
     * @return None.
     */         
    void SetCurrentRange(TInt aRange);
    
    /**
     * Set data query rect
     *
     * @since S60 v3.2
     * @param aRect The rect of data query
     * @return None.
     */         
    void SetDataQueryRect(const TRect& aRect);
    
    /**
     * Set current accent
     *
     * @since S60 v3.2
     * @param aAccent The new value
     * @return None.
     */         
    void SetCurrentAccent(TInt aAccent);    
    
    /**
     * Set editor numbric keymap
     *
     * @since S60 v3.2
     * @param aEditorNumericKeymap The new value
     * @return None.
     */         
    void SetEditorNumericKeymap(TInt aEditorNumericKeymap);

    /**
     * Set input method layout
     *
     * @since S60 v3.2
     * @param aImLayout The new value
     * @return None.
     */         
    void SetImLayout(TInt aImLayout);

    /**
     * Change shift down flag
     *
     * @since S60 v3.2
     * @return New case.
     */         
    TInt ChangeShiftDown();
    
    /**
     * Change capslock down flag
     *
     * @since S60 v3.2
     * @return New case.
     */          
    TInt ChangeCapslockDown();
    
    /**
     * Set shift down flag
     *
     * @since S60 v3.2
     * @param aFlag The new value
     * @return None.
     */         
    void SetShiftDown(TBool aFlag);
        
    /**
     * Set capslock down flag
     *
     * @since S60 v3.2
     * @param aFlag The new value
     * @return None.
     */          
    void SetCapslockDown(TBool aFlag);
    
    /**
     * Set length when button down on backspace button
     *
     * @since S60 v3.2
     * @param aLength The new value
     * @return None.
     */        
    void SetLengthWhenButtondownOnBack(TInt aLength);
    
    /**
     * Shift button permit or not
     *
     * @since S60 v3.2
     * @return ETrue if shift button permit, or EFalse.
     */         
    TBool IsShiftPermitted();
    TBool IsCapsLockPermitted();
    /**
     * Set screen mode
     *
     * @since S60 v3.2
     * @param aScreenMode The new value
     * @return None.
     */         
    void SetScreenMode(const TPixelsTwipsAndRotation& aScreenMode);

    /**
     * Save window position
     *
     * @since S60 v3.2
     * @param aBr The new value
     * @return None.
     */         
    void SaveWindowPos(const TPoint& aBr);

    /**
     * Save range
     *
     * @since S60 v3.2
     * @param aRange The new value
     * @return None.
     */    
    void SaveRange(TInt aRange);

    /**
     * Handle notification from CenRep monitor
     *
     * @since S60 v3.2
     * @param aPtr The data manager self
     * @return None.
     */         
    static TInt HandleFepRepositoryCallBack(TAny* aPtr);
    
    /**
     * Handle notification from CenRep monitor
     *
     * @since S60 v3.2
     * @param aPosition The data manager self
     * @return None.
     */         
    static TInt HandleGSRepositoryCallBack(TAny* aPtr);
    
    /**
     * Write window position
     *
     * @since S60 v3.2
     * @param aPosition The new value
     * @return None.
     */         
    void WriteWindowPostion(TInt aPosition);
    
    /**
     * Write last used range into CenRep
     *
     * @since S60 v3.2
     * @param aLastUsedRange The new value
     * @return None.
     */         
    void WriteLastUsedRange(TInt aLastUsedRange);
    
// From class MAknFepVkbDataProvider

    /**
     * Request data according to the data type
     *
     * @since S60 v3.2
     * @param aDataType The data type need to get
     * @return candidates list.
     */      
    TInt RequestData(TAknFepDataType aDataType);   
     
    /**
     * Set data according to the data type
     *
     * @since S60 v3.2
     * @param aDataType The data type need to set
     * @param aData The new data
     * @return candidates list.
     */      
    void SetData(TAknFepDataType aDataType,TInt aData);
    
    /**
     * Read unit size and shadow from scalable data
     *
     * @since S60 v3.2
     * @return None.
     */      
    void ReadUnitShadowSize();
    
    /**
     * Get info of whether it is size changing
     *
     * @since S60 v3.2
     * @return ETrue if size is changing, otherwise EFalse
     */      
    TBool SizeChanging();
    
    /**
     * Get info of whether unit and shadow size have been read
     *
     * @since S60 v3.2
     * @return Flag to indicate whether unit and shadow size
     * have been read
     */      
    TBool UnitShadowSizeSet();

    /**
     * Judge if next candidate after current page exist
     *
     * @since S60 v3.2
     * @return ETrue if candidate exist, otherwise EFalse
     */      
    TBool NextCandidateExist();
     
    /**
     * Reset unit size by current im layout
     *
     * @since S60 v3.2
     * @return None.
     */      
    void ResetUnitSize();
    
    /**
     * Set flag to indicate whether it is size changing
     *
     * @since S60 v3.2
     * @param aFlag Whether it is size changing
     * @return None.
     */      
    void SetSizeChanging(TBool aFlag);
    
protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @param aContext The layout context     
     * @return None.
     */      
    CAknFepVkbDataMgr(MAknFepVkbLayoutContext* aContext);
        
    /**
     * 2nd constructor function
     *
     * @since S60 v3.2
     * @return None.
     */      
    void ConstructL(CPtiEngine* aPtiEngine);  
    
    /**
     * Get language item according to the id
     *
     * @since S60 v3.2
     * @param aLangId The language id
     * @return The language item.
     */      
    //const SLanguageItem* GetItemByLangId(TInt aLangId);    
    
    /**
     * notification function when the window position is changed in CenRep
     *
     * @since S60 v3.2
     * @return The deal result.
     */      
    TInt PositionChangeNotification();

    /**
     * notification function when the default mode is changed in CenRep
     *
     * @since S60 v3.2
     * @return The deal result.
     */      
    TInt DefaultModeChangeNotification();

    /**
     * notification function when the peninput language is changed in CenRep
     *
     * @since S60 v3.2
     * @return The deal result.
     */      
    TInt PeninputlanguageChangeNotification();
    
    /**
     * Calculate case according to shiftdown and capslock down
     *
     * @since S60 v3.2
     * @return New case.
     */      
    TInt CalculateCase();
    
    /**
     * Judge if given language is Chinese
     *
     * @since S60 v3.2
     * @param aLang Given language code
     * @return ETrue if given language is Chinese, otherwise EFalse
     */      
    TBool LanguageIsChinese(TInt aLang);
    
    /**
     * Set engine language to given language
     *
     * @since S60 v3.2
     * @param aLang Given language code
     * @return None
     */      
    void SetEngineLanguage(TInt aLang);
    
private: // Data

    /**
     * The case
     * Own
     */
    TInt iCase;
    
    /**
     * The last used range
     * Own
     */    
    TInt iLastUsedRange;
    
    /**
     * The peninput language
     * Own
     */    
    TInt iInputLanguage;

    /**
     * The permitted range
     * Own
     */    
    TInt iPermittedRange;
    TInt iPermittedCase;

    /**
     * The candidate count
     * Own
     */    
    TInt iCandidatesCount;
    
    /**
     * The current range
     * Own
     */    
    TInt iCurrentRange; 
       
    /**
     * The current accent
     * Own
     */    
    TInt iCurrentAccent;
    
    /**
     * The unit width
     * Own
     */    
    TInt iUnitWidth;
    
    /**
     * The unit height
     * Own
     */    
    TInt iUnitHeight;
    
    /**
     * The im layout
     * Own
     */    
    TInt iImLayout; 

    /**
     * The default im layout for HK
     * Own
     */    
    TInt iDefaultImLayout4HK;
    
    /**
     * The default im layout for PRC
     * Own
     */    
    TInt iDefaultImLayout4PRC;
    
    /**
     * The default im layout for TW
     * Own
     */    
    TInt iDefaultImLayout4TW;

    //TAknEditorNumericKeymap iEditorNumericKeymap;
    /**
     * The editor numeric key map
     * Own
     */    
    TInt iEditorNumericKeymap;

    /**
     * The flag of shift down
     * Own
     */    
    TBool iShiftDown;
    
    /**
     * The flag of capslock down
     * Own
     */    
    TBool iCapslockDown;
    
    /**
     * The string length in composition field when mouse down on back button
     * Own
     */    
    TInt iLengthWhenButtondownOnBack;
    
    /**
     * The candidate list
     * Own
     */    
    RPointerArray<HBufC> iCandidates;
    
    /**
     * The pti engine
     * Ownership is deternimed by 
     */    
    CPtiEngine* iPtiEngine;
    
    /**
     * The pti engine ownership flag
     */    
    TBool iOwnPtiEngine;
    
    /**
     * The layout context
     * Not wwn
     */    
    MAknFepVkbLayoutContext* iContext;
    
    /**
     * The language string list
     * Own
     */    
    //RPointerArray<SLanguageItem> iLangStringList;
    
    /**
     * The fep repository (Own);
     */
    CRepository* iFepRepository;
    
    /**
     * The repository watcher
     * Own
     */    
    CAknFepRepositoryWatcher* iFepRepositoryWatcher;
    
    /**
     * The gs repository 
     * Own
     */
    CRepository* iGSRepository;

    /**
     * The repository watcher
     * Own
     */    
    CAknFepRepositoryWatcher* iGSRepositoryWatcher;
    
    /**
     * The position setting id
     * Own
     */    
    TInt iPositionSettingId;    
    
    /**
     * The window position
     * Own
     */    
    TInt iWindowPosition;
    
    /**
     * The screen mode
     * Own
     */    
    TPixelsTwipsAndRotation iScreenMode;

    /**
     * The rect of data query dialog
     * Own
     */    
    TRect iDataQueryRect;    

    /**
     * The user defined resource for number
     * Own
     */    
    HBufC* iUserDefinedResource;    
    
    /**
     * The predictive buffer
     * Own
     */    
    TBuf<KMaxLengthPredictiveString> iPredictiveBuf;
    
    /**
     * The array of units size
     * Own
     */    
    RArray<TSize> iUnits;
    
    /**
     * Flag of size changing
     */    
    TBool iSizeChanging;
    
    /**
     * The size of top left shadow corner
     */
    TSize iShadowTl;
    
    /**
     * The size of bottom right shadow corner
     */
    TSize iShadowBr;
    
    /**
     * The rect of main pane
     */
    TRect iMainPaneRect;
    
    /**
     * Indicate whether unit, shadow size have been read
     */
    TBool iUnitShadowSizeRead;
    
    TBool iNeedBrJustify;
    };

#endif // C_AKNFEPVKBDATAMGR_H

// End Of File
 