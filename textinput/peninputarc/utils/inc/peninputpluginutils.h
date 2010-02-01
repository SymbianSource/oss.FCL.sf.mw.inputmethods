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
* Description:  Interface of project pen input plugin utils
*
*/


#ifndef C_PENINPUT_PLUGIN_COMMON_H
#define C_PENINPUT_PLUGIN_COMMON_H

// INCLUDES
#include <w32std.h>
#include <e32base.h>
#include <cenrepnotifyhandler.h>
#include <AknsDrawUtils.h>
#include <AvkonInternalCRKeys.h>
#include <bitmaptransforms.h> // required for enums

// FORWARD DECLARATION
class CRepository;
class MAknFepTimerHandler;

// CONSTANTS
//const TUid KCRUidAvkon = { 0x101F876E };
//const TUint32 KAknLayoutId = 0x00000001;

/**
 *  CAknFepTimer Fep timer class 
 *
 *  This class inherit from CActive to send time event handling 
 *
 *  @lib peninputpluginutils.lib
 *  @since S60 v4.0
 */
class CAknFepTimer : public CActive
    {

public: 

    /**
     * Symbian Constructor
     *
     * @since S60 v4.0
     * @param aTimerHandler The timer handler
     * @return Pointer to created CAknFepHwrTimer object
     */
    IMPORT_C static CAknFepTimer* NewL(MAknFepTimerHandler* aTimerHandler);

    /**
     * Destructor
     *
     * @since S60 v4.0
     * @return None
     */
    IMPORT_C ~CAknFepTimer();

    /**
     * Set timer delay.
     *
     * @since S60 v4.0
     * @param aDelay The timer delay
     * @return None
     */
    IMPORT_C void SetTimer(const TTimeIntervalMicroSeconds32& aDelay);

private:

    /**
     * C++ constructor.
     *
     * @since S60 v4.0
     * @param aTimerHandler The timer handler
     * @return None
     */
    CAknFepTimer(MAknFepTimerHandler* aTimerHandler);

    /**
     * Symbian second-phase constructor
     *
     * @since S60 v4.0
     * @return None
     */
    void ConstructL();

// From CActive

    /**
     * From CActive.
     * Will be called when timer ends
     *
     * @since S60 v4.0
     * @return None
     */
    void RunL();

    /**
     * From CActive.
     * Will be called if RunL leaves
     *
     * @since S60 v4.0
     * @param aError The leave code reported
     * @return KErrNone if it handles the leave; otherwise return error code
     */
    TInt RunError(TInt aError);

    /**
     * From CActive.
     * Will be called when timer has been cancelled
     *
     * @since S60 v4.0
     * @return None
     */
    void DoCancel();

private:// data

    /**
     * Timer 
     */
    RTimer iTimer;
    
    /**
     * Timer event handler (Not own)
     */
    MAknFepTimerHandler* iTimerHandler;
    
    };

/**
 *  Class MAknFepTimerHandler Fep timer handler
 *  
 *  Interface for sending time event handling
 *
 *  @lib peninputpluginutils.lib
 *  @since S60 v4.0
 */
class MAknFepTimerHandler
    {

public:

    /**
     * Handling time out event
     *
     * @since S60 v4.0
     * @param aTimer The timer which sent the request
     * @return None
     */
    virtual void HandleTimerOut(const CAknFepTimer* aTimer) = 0;
    
    };
    

class CAknFepRepositoryWatcher : public CBase, public MCenRepNotifyHandlerCallback
    {
public:
    IMPORT_C static CAknFepRepositoryWatcher* NewL(
        const TUid aUid,
        const TUint32 aKey,
        CCenRepNotifyHandler::TCenRepKeyType aKeyType,
        TCallBack aCallBack,
        CRepository* aRepository);

    IMPORT_C static CAknFepRepositoryWatcher* NewL(
        const TUid aUid,
        TCallBack aCallBack,
        CRepository* aRepository);

    IMPORT_C ~CAknFepRepositoryWatcher();

    IMPORT_C TUint32 ChangedKey();

public: // from MCenRepNotifyHandlerCallback
    IMPORT_C void HandleNotifyInt    (TUint32 aKey, TInt aNewValue);
    IMPORT_C void HandleNotifyError  (TUint32 aKey, TInt aError, CCenRepNotifyHandler* aHandler);
	IMPORT_C void HandleNotifyGeneric(TUint32 aId);    

private:
    CAknFepRepositoryWatcher(
        const TUid aUid,
        const TUint32 aKey,
        TCallBack aCallBack,
        CRepository* aRepository);

    void ConstructL(CCenRepNotifyHandler::TCenRepKeyType aKeyType);

    void ConstructL();

private:
    TUid                    iUid;
    TUint32                 iKey;
    TUint32                 iChangedKey;
    TCallBack               iCallBack;
    CRepository*            iRepository;
    CCenRepNotifyHandler*   iNotifyHandler;

    };
    
/**
 *  Class AknPenInputUtils 
 *  
 *  This class provide some utility function for peninput
 *
 *  @lib peninputpluginutils.lib
 *  @since S60 v4.0
 */
class AknPenInputUtils
    {

public:

    /**
     * Get ISO code accroding to language ID
     *
     * @since S60 v4.0
     * @param aLanguage Language ID for looking up
     * @param aISOCode Pointer to ISO code according to language ID.
     * @return ETrue if it has iso code; otherwise EFalse
     */
    IMPORT_C static TBool GetISOLanguageCode(const TLanguage aLanguage, TDes& aISOCode);
    
    /**
     * Start application
     *
     * @since S60 v4.0
     * @param aAppname Application name
     * @return void
     */
    IMPORT_C static void StartAppL( const TDesC& aAppName );
    }; 

/**
 *  Class AknPenImageUtils 
 *  
 *  This class provide some utility function for image
 *
 *  @lib peninputpluginutils.lib
 *  @since S60 v4.0
 */
class AknPenImageUtils
    {
public:
    /**
     * Combine two images into 1 image
     *
     * @since S60 v4.0
     * @param aBmp1 First image used to be combined
     * @param aBmp2 Second image used to be combined
     * @param aMode Color depth of new image
     * @param aHorizontal ETrue means aBmp1 concatenate aBmp2 at horizontal 
     * direction to create new image, otherwise means aBmp1 concatenate aBmp2 
     * at vertical direction to create new image
     * @return New image created, its size calculated by size of aBmp1 and aBmp2
     */
    IMPORT_C static CFbsBitmap* CombineTwoImagesL(const CFbsBitmap* aBmp1,
                                                  const CFbsBitmap* aBmp2,
                                                  TDisplayMode aMode,
                                                  TBool aHorizontal = ETrue);
	/**
     * It only used for common button, not so long or so heigh. 
     *
     * @since S60 v5.0
     * @param aBoundRect  	The rect of the button
     * @param aGraphicRect  The calculated rect for graphics 
     * @return none
     */   	
	IMPORT_C static void CalculateGraphicRect(const TRect& aBoundRect, TRect& aGraphicRect);
	
	/**
     * Calculate graphics rect for long button 
     *
     * @since S60 v5.0
     * @param aBoundRect  	The rect of the button
     * @param aGraphicRect  The calculated rect for graphics 
     * @return none
     */   	
	IMPORT_C static void CalculateLongBtnGraphicRect(const TRect& aBoundRect, TRect& aGraphicRect);	                                                    
    };

/**
 *  Class CPenInputColorIcon 
 *  
 *  This class provide some encapulation for color icon
 *  Use AKN_FEP_COLOR_ICON_IMAGE resource struct to config color icon
 *
 *  @lib peninputpluginutils.lib
 *  @since S60 v5.0
 */ 
class CPenInputColorIcon : public CBase
	{
public:
	virtual ~CPenInputColorIcon();
	IMPORT_C static CPenInputColorIcon* NewL( TInt aResID );
	IMPORT_C static CPenInputColorIcon* NewLC( TInt aResID ); 	
	IMPORT_C virtual void ConstructFromResourceL( TInt aResID );

	IMPORT_C virtual void ResizeL( const TSize& aSize );
	inline void ReConstructL();
	inline void SetDimmed( TBool aIsDimmed );
	inline TBool IsDimmed() const;
	inline CFbsBitmap* Bitmap();
	inline CFbsBitmap* BitmapMask();
	inline CFbsBitmap* DimmedBitmapMask();								
protected:
	inline CPenInputColorIcon( TInt aResID );
	inline void ConstructL();
	void CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
                            const CFbsBitmap* aMask );
	void DestroyRes();                            
private:
	CFbsBitmap* iBmp;
	CFbsBitmap* iBmpMask;
	CFbsBitmap* iDimmedBmp;
	TBool 		iIsDimmed;
	TInt 		iResID;
	};
	
class CPenInput3PiecesColorIcons : public CBase
	{
public:
	virtual ~CPenInput3PiecesColorIcons();
	IMPORT_C static CPenInput3PiecesColorIcons* NewL( 
						TInt aFirstIconResID,
						TInt aMiddleIconResID,
						TInt aLastIconResID );
	IMPORT_C static CPenInput3PiecesColorIcons* NewLC( 
						TInt aFirstIconResID,
						TInt aMiddleIconResID,
						TInt aLastIconResID );
	IMPORT_C void SetDimmed( TBool aIsDimmed );								
	IMPORT_C void ReConstructL();							
	inline CPenInputColorIcon* FirstIcon();
	inline CPenInputColorIcon* MiddleIcon();
	inline CPenInputColorIcon* LastIcon();											
protected:
	inline CPenInput3PiecesColorIcons();
	void ConstructFromResourceL( TInt aFirstIconResID,	 					
								 TInt aMiddleIconResID,
								 TInt aLastIconResID );						
private:
	CPenInputColorIcon* iFirstIcon;
	CPenInputColorIcon* iMiddleIcon;
	CPenInputColorIcon* iLastIcon;	
	};

/**
 *  Class AknPenInputDrawUtils 
 *  
 *  This class provide some utility function for draw skin image
 *
 *  @lib peninputpluginutils.lib
 *  @since S60 v5.0
 */
class AknPenInputDrawUtils
    {
public:
    /**
     * This method used to draw 3 pieces skined frame
     *
     * @since S60 v5.0
     * @param aInstance  Pointer to the skin instance. If NULL, the method returns immediately
     * @param aGc  		 Graphics context to be used for drawing 
     * @param aLeftOrTopRect 	 one side rect, maybe left side or top side
     * @param aCenterRect 		 center rect 
     * @param aRightOrBottomRect one side rect, maybe right side or top side
     * @param aSideLeftOrTopID 	 skin ID left or top side
     * @param aCenterID 	 	 skin ID center
     * @param aSideRightOrBottomID 	 skin ID right or bottom side
     * @return none
     */
    IMPORT_C static void Draw3PiecesFrame(MAknsSkinInstance *aInstance,
                                           CFbsBitGc& aGc,
                                           const TRect& aLeftOrTopRect,
                                           const TRect& aCenterRect,
                                           const TRect& aRightOrBottomRect,
                                           const TAknsItemID& aSideLeftOrTopID,
                                           const TAknsItemID& aCenterID,
                                           const TAknsItemID& aSideRightOrBottomID);
   	
    /**
     * This method used to draw 3 pieces skined frame
     *
     * @since S60 v5.0
     * @param aInstance  Pointer to the skin instance. If NULL, the method returns immediately
     * @param aGc  		 Graphics context to be used for drawing 
     * @param aOuterRect 	 OutSide rect
     * @param aInnerRect 	 inside rect 
     * @param aSideLeftOrTopID 	 skin ID left or top side
     * @param aCenterID 	 	 skin ID center
     * @param aSideRightOrBottomID 	 skin ID right or bottom side
     * @return none
     */   	
    IMPORT_C static void Draw3PiecesFrame(MAknsSkinInstance *aInstance,
                                           CFbsBitGc& aGc,
                                           const TRect& aOuterRect,
                                           const TRect& aInnerRect,
                                           const TAknsItemID& aSideLeftOrTopID,
                                           const TAknsItemID& aCenterID,
                                           const TAknsItemID& aSideRightOrBottomID);
	/**
     * This method used to draw color icon
     *
     * @since S60 v5.0
     * @param aColorIcon  Pointer to the color icon. If NULL, the method returns immediately
     * @param aGc  		  Graphics context to be used for drawing 
     * @param aRect 	  OutSide rect
     */                                       
   	IMPORT_C static void DrawColorIcon( CPenInputColorIcon *aColorIcon,   										 
								 		CFbsBitGc& aGc,
								 		const TRect& aRect );
								 		
	/**
     * This method used to draw 3 pieces color icon
     *
     * @since S60 v5.0
     * @param aColorIcon  Pointer to the color icon. If NULL, the method returns immediately
     * @param aGc  		  Graphics context to be used for drawing 
     * @param aRect 	  OutSide rect
     */                                       
   	IMPORT_C static void Draw3PiecesColorIcon( CPenInput3PiecesColorIcons *aColorIcon,   										 
								 		       CFbsBitGc& aGc,
								 		       const TRect& aOuterRect,
								 		       const TRect& aInnerRect );
								 		
	};
 
/**
*
* The class is from the BitmapTransforms Library bitmap rotation.
* @internalTechnology
* @Note: This class can be only used for the small graphics' transforms.Please use 
* CBitmapRotator when you have to handle any big graphics.
*            
*/
typedef CBitmapRotator::TRotationAngle TSyncRotationAngle;
class CPeninputSyncBitmapRotator : public CBase
    {
public:
    IMPORT_C static CPeninputSyncBitmapRotator* NewL();
    ~CPeninputSyncBitmapRotator();

    IMPORT_C TInt Rotate(CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TSyncRotationAngle aAngle);
    IMPORT_C TInt Rotate(CFbsBitmap& aBitmap, TSyncRotationAngle aAngle);

private:

    CPeninputSyncBitmapRotator();
    void ConstructL();
    void Cleanup();
    void DoRotate();
    void FlipScanLine(TDes8& aDes, TInt aWidth);
    void PutScanline(TInt aYPos);

private:
    TInt                iCurOffset;
    TInt                iRows;
    TSyncRotationAngle  iAngle;
    TSize               iBitmapSize;
    TDisplayMode        iDisplayMode;
    CFbsBitmap*         iTempBitmap; // For the Rotate() with only one bitmap supplied
    CFbsBitmap*         iSrcBitmap;
    CFbsBitmap*         iTgtBitmap;
    TUint32*            iScanlineBuffer;
    TDisplayMode        iScanlineDisplayMode;
    TInt                iPixelSizeInBytes;
    TPtr8               iScanlineDes;
    CFbsBitmapDevice*   iDevice;
    CFbsBitGc*          iGc;
    CFbsBitmap*         iScanlineBitmap; // Intermediate scanline bitmap
#if defined(ROTATION_PROFILING) 
    TUint               iStartedAtFc;    // Used for profiling
#endif  
    };

/**
 *  Class CPenInputTrailColor 
 *  
 *  This class provide some encapulation for trail color
 *
 *  @lib peninputpluginutils.lib
 *  @since S60 v5.0
 */ 

class AknPenInputTrailColorUtils
    {
public:
    IMPORT_C static TBool CheckColorInColorTable(TInt aColor);
    //IMPORT_C void SetDefaultColor(TRgb& color);
    IMPORT_C static TInt ColorCount();
    IMPORT_C static TRgb ColorAt(TInt aIndex); 
    IMPORT_C static TInt GetTrailColorByTheme();
    }; 

 
inline CPenInputColorIcon::CPenInputColorIcon( TInt aResID )
	{		
	iResID = aResID;
	}	
inline void CPenInputColorIcon::ConstructL()
	{		
	ConstructFromResourceL( iResID );
	}
inline void CPenInputColorIcon::SetDimmed( TBool aIsDimmed )
	{
	iIsDimmed = aIsDimmed;
	}
inline TBool CPenInputColorIcon::IsDimmed() const
	{
	return iIsDimmed;	
	}	
inline CFbsBitmap* CPenInputColorIcon::Bitmap()
	{
	return iBmp;
	}
inline CFbsBitmap* CPenInputColorIcon::BitmapMask()
	{
	return iBmpMask;
	}
inline CFbsBitmap* CPenInputColorIcon::DimmedBitmapMask()
	{
	return iDimmedBmp;
	}		
	
inline CPenInput3PiecesColorIcons::CPenInput3PiecesColorIcons()
	{		
	}
inline CPenInputColorIcon* CPenInput3PiecesColorIcons::FirstIcon()
	{
	return iFirstIcon;
	}
inline CPenInputColorIcon* CPenInput3PiecesColorIcons::MiddleIcon()
	{
	return iMiddleIcon;
	}	
inline CPenInputColorIcon* CPenInput3PiecesColorIcons::LastIcon()
	{
	return iLastIcon;
	}	
inline void CPenInputColorIcon::ReConstructL()
	{
	ConstructFromResourceL(iResID);	
	}
#endif // C_PENINPUT_PLUGIN_COMMON_H

// End Of File
