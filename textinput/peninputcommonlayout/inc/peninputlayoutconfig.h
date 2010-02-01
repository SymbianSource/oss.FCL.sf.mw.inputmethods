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
* Description:  common layout config
*
*/


#ifndef C_CPENINPUTLAYOUTCONFIG_H
#define C_CPENINPUTLAYOUTCONFIG_H


// System includes
#include <barsread.h>
#include <peninputvkbctrllayout.h>

// Forward decalaration
class CPeninputClientLayoutInfo;
class CPeninputVkbLayoutInfo;
class CPeninputRangeBarInfo;
class CPeninputShiftCapsInfo;
class CPeninputCtrlPool;
/**
 *  Peninput commonlayout layout config
 *
 *  @lib peninputcommonlayout.lib
 *  @since S60 v3.2
 */
class CPeninputLayoutConfig : public CBase
    {

public:

    /**
     * Symbian constructor
     *
     * @since S60 v3.2
     * @param aResUtils Resource Utils
     * @param aReader Resource reader
     * @return Pointer to created CPeninputLayoutConfig object
     */
    IMPORT_C static CPeninputLayoutConfig* NewL( CAknFepCtrlVkbLayout& aVkblayout,
                                                 TResourceReader& aReader );

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    IMPORT_C virtual ~CPeninputLayoutConfig();

    /**
     * Get language id
     *
     * @since S60 v3.2
     * @return Language id
     */
    inline TInt Language();

    /**
     * Get language name
     *
     * @since S60 v3.2
     * @return Language name
     */
    inline HBufC* LanguageString();

    /**
     * Get case sensitive flag
     *
     * @since S60 v3.2
     * @return Case sensitive flag
     */
    inline TBool CaseSensitive();

    /**
     * Get guide line
     *
     * @since S60 v3.2
     * @return The guide line style
     */
    inline TInt GuideLine();
    
    /**
     * The position percent of top guide line
     *
     * @since S60 v3.2
     * @return The position percent of top guide line
     */
    inline TInt PositionTopLine();
    
    /**
     * The position percent of bottom guide line
     *
     * @since S60 v3.2
     * @return The position percent of bottom guide line
     */
    inline TInt PositionBottomLine();
            
    /**
     * Get client layout info list
     *
     * @since S60 v3.2
     * @return The reference of pointer array containing client layout list
     */
    inline RPointerArray<CPeninputClientLayoutInfo>& ClientLayoutInfoList();

    /**
     * Get vkb layout info list
     *
     * @since S60 v3.2
     * @return The reference of pointer array containing vkb layout list
     */
    inline RPointerArray<CPeninputVkbLayoutInfo>& VkbLayoutInfoList();
    
    /**
     * Get shift&caps info
     *
     * @since S60 v3.2
     * @return The reference of pointer array containing shift&caps info
     */
    inline RPointerArray<CPeninputShiftCapsInfo>& ShiftCapsInfoList();
    
    /**
     * Get client layout info
     *
     * @since S60 v3.2
     * @param aLayoutId Client layout id  
     * @return The pointer to CPeninputClientLayoutInfo object
     */
    IMPORT_C CPeninputClientLayoutInfo* FindClientLayoutInfo( 
        TInt aLayoutId );
    
    /**
     * Get vkb layout info
     *
     * @since S60 v3.2
     * @param aLayoutId Vkb layout id
     * @return The pointer to CPeninputVkbLayoutInfo object
     */
    IMPORT_C CPeninputVkbLayoutInfo* FindVkbLayoutInfo( TInt aLayoutId );
    
    /**
     * Get shift&caps info
     *
     * @since S60 v3.2
     * @param aRangeId Range id  
     * @return The pointer to CPeninputShiftCapsInfo object
     */
    IMPORT_C CPeninputShiftCapsInfo* FindShiftCapsInfo( TInt aRangeId );
    
    /**
     * Get vkb layout id
     *
     * @since S60 v3.2
     * @param aRangeId Range id  
     * @param aShiftCapsSingleId Shift&caps single id
     * @return The vkb layout id
     */
    IMPORT_C TInt ShiftCapsSingleVkbLayoutId( TInt aRangeId,
                                              TInt aShiftCapsSingleId );

    /**
     * Get vkb layout id
     *
     * @since S60 v3.2
     * @param aRangeId Range id  
     * @param aCase The case
     * @return The vkb layout id
     */
    IMPORT_C TInt ShiftCapsSingleVkbLayoutByCase( TInt aRangeId,
                                                  TInt aCase );
    /**
     * Get range bar info
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputRangeBarInfo object
     */
    inline CPeninputRangeBarInfo* RangeBarInfo();

    /**
     * Get range bar resource id
     *
     * @since S60 v3.2
     * @return The resource id
     */    
    inline TInt RangebarResId();
    
protected:

    /**
     * Constructor
     *
     * @since S60 v3.2
     * @return None
     */
    CPeninputLayoutConfig();

    /**
     * Second phase constructor
     *
     * @since S60 v3.2
     * @param aResUtils Resource Utils
     * @param aReader Resource reader
     * @return None
     */
    void ConstructL( CAknFepCtrlVkbLayout& aVkblayout, TResourceReader& aReader  );  

private: // Data

    /**
     * Language id
     */ 
    TInt iLanguage;

    /**
     * Language string
     * Own
     */
    HBufC* iLanguageString;

    /**
     * Case sensitive flag
     */ 
    TBool iCaseSensitive;
    
    /**
     * Guide line style
     */ 
    TInt iGuideLine;

    /**
     * The position percent of top guide line
     */ 
    TInt iPositionTopLine;

    /**
     * The position percent of bottom guide line
     */ 
    TInt iPositionBottomLine;

    /**
     * Client layout info list
     */ 
    RPointerArray<CPeninputClientLayoutInfo> iClientLayoutInfoList;

    /**
     * Vkb layout info list
     */ 
    RPointerArray<CPeninputVkbLayoutInfo> iVkbLayoutInfoList;

    /**
     * Range bar info
     * Own
     */
    CPeninputRangeBarInfo* iRangeBarInfo;

    /**
     * Shift&caps info list
     */ 
    RPointerArray<CPeninputShiftCapsInfo> iShiftCapsInfoList;

    /**
     * Resource Id for range bar
     */ 
    TInt iRangeBarResId;
    };

#include "peninputlayoutconfig.inl"

#endif // C_CPENINPUTLAYOUTCONFIG_H
