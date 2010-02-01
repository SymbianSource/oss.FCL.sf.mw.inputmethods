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
* Description:  peninput vkb laf manager
*
*/


#ifndef C_CPENINPUTGENERICHWRLAFDATAMGR_H
#define C_CPENINPUTGENERICHWRLAFDATAMGR_H

// System includes
#include <e32base.h>
#include <w32std.h>
#include <AknFepGlobalEnums.h>
// User includes

// Forward decalaration

// laf type definition
enum TPeninputHwrLayoutType
	{
	ELayoutNone 	= -1,
	ELayoutLatin,
	ELayoutNonLatin 
	};

struct TPeninputHwrButtonLayoutData
	{
	TRect iRect;
	TRect iInnerRect;
	};
	
struct TPeninputHwrEntryLayoutData
	{
	TRect iRect;
	//const CFont* iPromptTextFont;
	const CFont* iTextFont;
	};

struct TPeninputHwrRangeBarLayoutData
	{
	TRect iRect;
	TPeninputHwrButtonLayoutData 	iAlphaButton;
	TPeninputHwrButtonLayoutData 	iNumberButton;	
	TPeninputHwrButtonLayoutData 	iSymbolButton;

	};

class CPeninputHwrLayoutDataInfo : public CBase
	{
public:
	static CPeninputHwrLayoutDataInfo* NewLC();
	TRect iHwrWndRect;
	TRect iWriteAreaRect;
	TPeninputHwrButtonLayoutData 	iCloseButton;
	TPeninputHwrButtonLayoutData 	iOptionButton;
	TPeninputHwrEntryLayoutData  	iICF;
	TPeninputHwrButtonLayoutData	iMoveButton;	
	TPeninputHwrRangeBarLayoutData	iRangbar;
	TPeninputHwrButtonLayoutData	iLanguageButton;
	TPeninputHwrButtonLayoutData	iSwitchButton;
	};
	
/**
 *  Peninput HWR data management class
 *  This class manager the data of HWR
 *  
 *  @lib peninputgenerichwr.lib
 *  @since S60 v3.2
 */
class CPeninputGenericHwrLafMgr : public CBase 
    {   
public:

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputGenericVkbLafMgr object
     */
    static CPeninputGenericHwrLafMgr* NewL(TPeninputHwrLayoutType aLayoutType);

    /**
     * Two-phased constructor
     *
     * @since S60 v3.2
     * @return The pointer to CPeninputGenericHwrLafMgr object
     */
    static CPeninputGenericHwrLafMgr* NewLC(TPeninputHwrLayoutType aLayoutType);

    /**
     * Destructor
     *
     * @since S60 v3.2
     * @return None
     */
    virtual ~CPeninputGenericHwrLafMgr();

  	/**
     * ConstructL
     *
     * @since S60 v3.2
     * @return None
     */
	void ConstructL();

	/**
     * Set current layout type;
     * @param aLayoutType  Hwr layout type( latin or non lation )
     * @since S60 v3.2
     * @return None
     */
	inline void SetLayoutType(TPeninputHwrLayoutType aLayoutType);	
	inline TPeninputHwrLayoutType LayoutType() const;
	
	/**
     * Get button laf data
     *
     * @since S60 v3.2
     * @param aButtonID 	button id
     * @param aOutterRect	button rect
     * @param aInnerRect	button forground rect
     * @return None
     */	
	void GetButtonRect( TPeninputCommonCtrlID aButtonID, 
						TRect& aOutterRect,
						TRect& aInnerRect );
	
	inline const TRect& EntirePaneRect() const;						
	inline const TRect& RangbarRect() const;
	inline const TRect& TextAreaRect() const;
	inline const TRect& WriteAreaRect() const;						

protected:
	
	CPeninputGenericHwrLafMgr(TPeninputHwrLayoutType aLayoutType);
	
	void ReadLafInfoL();
	
	void GetHwrLayoutDataInfo( const TRect& aParentWndRect, 
							   TInt aLayoutType, 
							   CPeninputHwrLayoutDataInfo& aDataInfo );
	
	inline TBool IsValid() const;
private:
	TPeninputHwrLayoutType iCurLayoutType;
	TBool iIsValid;
	RPointerArray<CPeninputHwrLayoutDataInfo> iLayoutData;
    };

#endif //C_CPENINPUTGENERICHWRLAFDATAMGR_H