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
* Description:  Implement of pen input plugin utils:resource,repository,LafEnv
*
*/

#include <centralrepository.h>
#include <apacmdln.h>
#include <coemain.h>
#include <barsread.h>
#include <e32std.h>
#include <bitmtrans/bitmtranspanic.h> //for panic enum

#include "peninputpluginutils.h"
#include "AknsUtils.h"

// constant
const TInt KInvalidResId = -1;
const TInt KInvalidBmp = -1 ;
const TInt KInvalidColorGroup = -1;

const TInt KTransparency[256] =
    { 
    0, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 7, 7, 7,
    8, 8, 8, 8, 9, 9, 9, 10, 10, 10, 11, 11, 11, 11, 12, 12, 12, 13, 13, 13,
    14, 14, 14, 14, 15, 15, 15, 16, 16, 16, 17, 17, 17, 17, 18, 18, 18, 19,
    19, 19, 20, 20, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 23, 23, 24, 24,
    24, 25, 25, 25, 26, 26, 26, 26, 27, 27, 27, 28, 28, 28, 29, 29, 29, 29,
    30, 30, 30, 31, 31, 31, 32, 32, 32, 32, 33, 33, 33, 34, 34, 34, 35, 35,
    35, 35, 36, 36, 36, 37, 37, 37, 38, 38, 38, 38, 39, 39, 39, 40, 40, 40,
    41, 41, 41, 41, 42, 42, 42, 43, 43, 43, 44, 44, 44, 44, 45, 45, 45, 46,
    46, 46, 47, 47, 47, 47, 48, 48, 48, 49, 49, 49, 50, 50, 50, 50, 51, 51,
    51, 52, 52, 52, 53, 53, 53, 53, 54, 54, 54, 55, 55, 55, 56, 56, 56, 56,
    57, 57, 57, 58, 58, 58, 59, 59, 59, 59, 60, 60, 60, 61, 61, 61, 62, 62,
    62, 62, 63, 63, 63, 64, 64, 64, 65, 65, 65, 65, 66, 66, 66, 67, 67, 67,
    68, 68, 68, 68, 69, 69, 69, 70, 70, 70, 71, 71, 71, 71, 72, 72, 72, 73,
    73, 73, 74, 74, 74, 74, 75, 75, 75, 76, 76, 76, 77, 77 
    };
// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// C++ constructor.
// ---------------------------------------------------------------------------
//
CAknFepTimer::CAknFepTimer(MAknFepTimerHandler* aTimerHandler)
    :CActive(EPriorityStandard), iTimerHandler(aTimerHandler)
    {
    }

// ---------------------------------------------------------------------------
// Symbian Constructor
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepTimer* CAknFepTimer::NewL(MAknFepTimerHandler* aTimerHandler)
    {
    CAknFepTimer *self = new (ELeave) CAknFepTimer(aTimerHandler);
    
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

// ---------------------------------------------------------------------------
// Destructor
// ---------------------------------------------------------------------------
//
EXPORT_C CAknFepTimer::~CAknFepTimer()
    {
    Cancel();
    iTimer.Close();
    }

// ---------------------------------------------------------------------------
// Set timer delay
// ---------------------------------------------------------------------------
//
EXPORT_C void CAknFepTimer::SetTimer(const TTimeIntervalMicroSeconds32& aDelay)
    {
    Cancel();
    iTimer.After(iStatus,aDelay);
    SetActive();
    }

// ---------------------------------------------------------------------------
// Symbian second-phase constructor
// ---------------------------------------------------------------------------
//
void CAknFepTimer::ConstructL()
    {
    User::LeaveIfError(iTimer.CreateLocal());
    
    CActiveScheduler::Add(this);
    }

// ---------------------------------------------------------------------------
// Will be called when timer ends
// ---------------------------------------------------------------------------
//
void CAknFepTimer::RunL()
    {
    iTimerHandler->HandleTimerOut(this);
    }

// ---------------------------------------------------------------------------
// Will be called if RunL leaves
// ---------------------------------------------------------------------------
//
TInt CAknFepTimer::RunError(TInt /*aError*/)
    {
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// Will be called when timer has been cancelled
// ---------------------------------------------------------------------------
//
void CAknFepTimer::DoCancel()
    {
    iTimer.Cancel();
    }


EXPORT_C CAknFepRepositoryWatcher* CAknFepRepositoryWatcher::NewL(
    const TUid aUid,
    const TUint32 aKey,
    CCenRepNotifyHandler::TCenRepKeyType aKeyType,
    TCallBack aCallBack,
    CRepository* aRepository)
    {
    CAknFepRepositoryWatcher* self = new(ELeave) CAknFepRepositoryWatcher(aUid, aKey, 
                                                                          aCallBack, aRepository);

    CleanupStack::PushL(self);
    self->ConstructL(aKeyType);
    CleanupStack::Pop(self);

    return self;
    }

EXPORT_C CAknFepRepositoryWatcher* CAknFepRepositoryWatcher::NewL(
    const TUid aUid,
    TCallBack aCallBack,
    CRepository* aRepository)
    {
    CAknFepRepositoryWatcher* self = new(ELeave) CAknFepRepositoryWatcher(aUid, 
                                               NCentralRepositoryConstants::KInvalidNotificationId,
                                               aCallBack, aRepository);

    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);

    return self;
    }

EXPORT_C CAknFepRepositoryWatcher::~CAknFepRepositoryWatcher()
    {
    iNotifyHandler->StopListening();
    delete iNotifyHandler;
    }

CAknFepRepositoryWatcher::CAknFepRepositoryWatcher(
    const TUid aUid,
    const TUint32 aKey,
    TCallBack aCallBack,
    CRepository* aRepository)
    :
    iUid(aUid), iKey(aKey), iCallBack(aCallBack), iRepository(aRepository)
    {
    }

void CAknFepRepositoryWatcher::ConstructL(CCenRepNotifyHandler::TCenRepKeyType aKeyType)
    {
    iNotifyHandler = CCenRepNotifyHandler::NewL(*this, *iRepository, aKeyType, iKey);
    iNotifyHandler->StartListeningL();
    }

void CAknFepRepositoryWatcher::ConstructL()
    {
    iNotifyHandler = CCenRepNotifyHandler::NewL(*this, *iRepository);
    iNotifyHandler->StartListeningL();
    }

EXPORT_C TUint32 CAknFepRepositoryWatcher::ChangedKey()
    {
    return iChangedKey;
    }
       
EXPORT_C void CAknFepRepositoryWatcher::HandleNotifyInt(TUint32 aKey, TInt /*aNewValue*/)
    {
    iChangedKey = aKey;
    iCallBack.CallBack();
    iChangedKey = NCentralRepositoryConstants::KInvalidNotificationId;
    }

EXPORT_C void CAknFepRepositoryWatcher::HandleNotifyError(TUint32 /*aKey*/, TInt /*aError*/, 
                                                          CCenRepNotifyHandler* /*aHandler*/)
    {
    }

EXPORT_C void CAknFepRepositoryWatcher::HandleNotifyGeneric(TUint32 aId)
    {
    iChangedKey = aId;
    iCallBack.CallBack();
    iChangedKey = NCentralRepositoryConstants::KInvalidNotificationId;  
    }

// ---------------------------------------------------------------------------
// Get ISO code accroding to language ID
// ---------------------------------------------------------------------------
//
EXPORT_C TBool AknPenInputUtils::GetISOLanguageCode(const TLanguage aLanguage, 
                                                    TDes& aISOCode)
    {
    struct TMapArray {TInt iLanguageCode; TPtrC iLanguageSb;};
    const TInt KPrc= 0x7B80;
    const TInt KTw = 0x81fa;
    const TInt KHk = 0x6e2f;

    const  TMapArray KISOCode[] = 
        {
        {ELangEnglish,_L("en")},{ELangFrench,_L("fr")},
        {ELangGerman,_L("de")}, {ELangSpanish,_L("es")},
        {ELangItalian,_L("it")},{ELangSwedish,_L("sv")},
        {ELangDanish,_L("da")}, {ELangNorwegian, _L("no")}, 
        {ELangFinnish,_L("fi")},{ELangAmerican,_L("")},
        {ELangSwissFrench,_L("")},{ELangSwissGerman,_L("")},
        {ELangPortuguese,_L("pt")},{ELangTurkish,_L("tr")},
        {ELangIcelandic,_L("is")}, {ELangRussian, _L("ru")},
        {ELangHungarian,_L("hu")},{ELangDutch,_L("nl")},
        {ELangBelgianFlemish,_L("nl")},{ELangAustralian,_L("as")},
        {ELangBelgianFrench,_L("")},{ELangAustrian,_L("")},
        {ELangNewZealand,_L("")},{ELangInternationalFrench, _L("")}, 
        {ELangCzech,_L("cs")},{ELangSlovak,_L("sk")},
        {ELangPolish,_L("pl")}, {ELangSlovenian,_L("sl")}, 
        {ELangTaiwanChinese,_L("")},{ELangHongKongChinese,_L("")},
        {ELangPrcChinese,_L("zh")}, {ELangJapanese, _L("ja")},
        {ELangThai,_L("th")},{ELangAfrikaans,_L("af")},
        {ELangAlbanian,_L("sq")},{ELangAmharic,_L("am")},
        {ELangArabic,_L("ar")},{ELangArmenian,_L("hy")},
        {ELangTagalog,_L("tl")},{ELangBelarussian, _L("be")},
        {ELangBengali,_L("bn")}, {ELangBulgarian,_L("bg")},
        {ELangBurmese,_L("my")},{ELangCatalan,_L("ca")},
        {ELangCroatian,_L("hr")},{ELangCanadianEnglish,_L("")},
        {ELangInternationalEnglish,_L("")},{ELangSouthAfricanEnglish, _L("")}, 
        {ELangEstonian,_L("et")},{ELangFarsi,_L("fa")},
        {ELangCanadianFrench,_L("cf")},{ELangScotsGaelic,_L("")},
        {ELangGeorgian,_L("ka")},{ELangGreek,_L("el")},
        {ELangCyprusGreek,_L("")}, {ELangGujarati, _L("gu")},        
        {ELangKorean,_L("ko")},{ELangLao,_L("lo")},
        {ELangLatvian,_L("lv")},{ELangLithuanian,_L("lt")},
        {ELangMacedonian,_L("mk")},{ELangMalay,_L("ms")},
        {ELangMalayalam,_L("ml")}, {ELangMarathi, _L("mr")},
        {ELangMoldavian,_L("mo")},{ELangMongolian,_L("mn")},
        {ELangNorwegianNynorsk,_L("nn")},{ELangBrazilianPortuguese,_L("pt")},
        {ELangPunjabi,_L("pa")},{ELangRomanian,_L("ro")},
        {ELangSerbian,_L("sr")}, {ELangSinhalese, _L("si")},
        {ELangSomali,_L("so")},{ELangInternationalSpanish,_L("fr")},
        {ELangLatinAmericanSpanish,_L("es")},{ELangSwahili,_L("sw")},
        {ELangFinlandSwedish,_L("")},{ELangTamil,_L("ta")}, 
        {ELangTelugu, _L("te")},{ELangTibetan,_L("bo")},
        {ELangTigrinya,_L("ti")},{ELangCyprusTurkish,_L("")},
        {ELangTurkmen,_L("tk")},{ELangUkrainian,_L("uk")},
        {ELangUrdu,_L("ur")},{ELangVietnamese, _L("vi")},
        {ELangWelsh,_L("cy")},{ELangZulu,_L("zu")},
        {ELangHebrew,_L("he")}, {ELangIndonesian,_L("in")},
        {ELangHindi,_L("hi")}       
        };

    switch(aLanguage)
        {
        case ELangPrcChinese:
            aISOCode.Zero();
            aISOCode.Append(KPrc);
            return ETrue;
        case ELangTaiwanChinese:
            aISOCode.Zero();
            aISOCode.Append(KTw);
            return ETrue;
        case ELangHongKongChinese:
            aISOCode.Zero();
            aISOCode.Append(KHk);
            return ETrue;
        default:
             break;        
        }
    for (TInt temp = 0; temp < (sizeof(KISOCode)/sizeof(TMapArray)); temp++)
        {
        if (KISOCode[temp].iLanguageCode == aLanguage && 
            KISOCode[temp].iLanguageSb.Length() > 0)
            {
        aISOCode.Copy(KISOCode[temp].iLanguageSb);
        return ETrue;
            }
        }
            
    return EFalse;
    }

EXPORT_C void AknPenInputUtils::StartAppL( const TDesC& aAppName )
    {   
    TFindServer findApp( aAppName );
    TFullName name;
    if (findApp.Next( name ) == KErrNone)
        {
        // if server is already running return immediately
        return;
        } 
        
    RProcess process;
    TInt err = process.Create( aAppName, KNullDesC );
    User::LeaveIfError(err);
 
    CApaCommandLine* commandLine = CApaCommandLine::NewLC();
    commandLine->SetDocumentNameL( KNullDesC );
    commandLine->SetExecutableNameL( aAppName );
    commandLine->SetProcessEnvironmentL( process );
    CleanupStack::PopAndDestroy( commandLine );
 
    process.Resume();
    process.Close();
    }
    
// ---------------------------------------------------------------------------
// Combine two image into one
// ---------------------------------------------------------------------------
//
EXPORT_C CFbsBitmap* AknPenImageUtils::CombineTwoImagesL(const CFbsBitmap* aBmp1,
                                                         const CFbsBitmap* aBmp2,
                                                         TDisplayMode aMode,
                                                         TBool aHorizontal)
    {
    CFbsBitmap* offscreenbmp = new (ELeave) CFbsBitmap();
    CleanupStack::PushL(offscreenbmp);

    TInt bigger;
    TSize newsize;
    TSize bmp1size = aBmp1->SizeInPixels();
    TSize bmp2size = aBmp2->SizeInPixels();

    if (aHorizontal)
        {
        bigger = bmp1size.iHeight > bmp2size.iHeight ? bmp1size.iHeight : bmp2size.iHeight;
        newsize = TSize(bmp1size.iWidth + bmp2size.iWidth,
                        bigger);
        }
    else
        {
        bigger = bmp1size.iWidth > bmp2size.iWidth ? bmp1size.iWidth : bmp2size.iWidth;
        newsize = TSize(bigger,
                        bmp1size.iHeight + bmp2size.iHeight);
        }    

    offscreenbmp->Create(newsize, aMode);

    CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL(offscreenbmp);
    CleanupStack::PushL(bitmapDevice);

    CFbsBitGc* bitmapContext = NULL;
    bitmapDevice->CreateContext(bitmapContext);
    CleanupStack::PushL(bitmapContext);

    // copy image1 to context, and thus to offscreenbmp
    bitmapContext->BitBlt(TPoint(0,0), aBmp1);

    // copy image2 to context, thus to offscreenbmp
    if (aHorizontal)
        {
        bitmapContext->BitBlt(TPoint(bmp1size.iWidth, 0), aBmp2);
        }
    else
        {
        bitmapContext->BitBlt(TPoint(0, bmp1size.iHeight), aBmp2);
        }

    CleanupStack::PopAndDestroy(2, bitmapDevice);
    CleanupStack::Pop(offscreenbmp);
    return offscreenbmp;
    }

// ---------------------------------------------------------------------------
// draw 3 pieces skined frame
// ---------------------------------------------------------------------------
//
EXPORT_C void AknPenInputDrawUtils::Draw3PiecesFrame(MAknsSkinInstance *aInstance,
                                           CFbsBitGc& aGc,
                                           const TRect& aLeftOrTopRect,
                                           const TRect& aCenterRect,
                                           const TRect& aRightOrBottomRect,
                                           const TAknsItemID& aSideLeftOrTopID,
                                           const TAknsItemID& aCenterID,
                                           const TAknsItemID& aSideRightOrBottomID)
   	{
	if( !aInstance )
		return;
	AknsDrawUtils::DrawCachedImage( aInstance, aGc, aLeftOrTopRect, aSideLeftOrTopID);
	AknsDrawUtils::DrawCachedImage( aInstance, aGc, aCenterRect, aCenterID);
	AknsDrawUtils::DrawCachedImage( aInstance, aGc, aRightOrBottomRect, aSideRightOrBottomID);	
   	}
   	
// ---------------------------------------------------------------------------
// draw 3 pieces skined frame
// ---------------------------------------------------------------------------
//
EXPORT_C void AknPenInputDrawUtils::Draw3PiecesFrame(MAknsSkinInstance *aInstance,
                                           CFbsBitGc& aGc,
                                           const TRect& aOuterRect,
                                           const TRect& aInnerRect,
                                           const TAknsItemID& aSideLeftOrTopID,
                                           const TAknsItemID& aCenterID,
                                           const TAknsItemID& aSideRightOrBottomID)
	{
	TRect aSideRect1, aSideRect2;
	// horizon
	if( aOuterRect.iTl.iY == aInnerRect.iTl.iY && aOuterRect.iBr.iY == aInnerRect.iBr.iY )
		{
		aSideRect1 = TRect( aOuterRect.iTl, TPoint( aInnerRect.iTl.iX, aInnerRect.iBr.iY ) );
		aSideRect2 = TRect( TPoint( aInnerRect.iBr.iX, aInnerRect.iTl.iY), aOuterRect.iBr );	
		}
	else if( aOuterRect.iTl.iX == aInnerRect.iTl.iX &&  aOuterRect.iBr.iX == aInnerRect.iBr.iX )
		{
		aSideRect1 = TRect( aOuterRect.iTl, TPoint( aInnerRect.iBr.iX, aInnerRect.iTl.iY ) );
		aSideRect2 = TRect( TPoint( aInnerRect.iTl.iX, aInnerRect.iBr.iY), aOuterRect.iBr );			
		}
	Draw3PiecesFrame( aInstance,
					  aGc,
					  aSideRect1,
					  aInnerRect,
					  aSideRect2,
					  aSideLeftOrTopID,
					  aCenterID,
					  aSideRightOrBottomID );
	}
	
// ---------------------------------------------------------------------------
// calculate the graphic rect for common button
// ---------------------------------------------------------------------------
//
EXPORT_C void AknPenImageUtils::CalculateGraphicRect(const TRect& aBoundRect, TRect& aGraphicRect)
	{
	// judge whether the width or height is larger
	TPoint ltPoint;
	TInt unitLength = 0;
	if( aBoundRect.Width() > aBoundRect.Height() )
		{
		unitLength = aBoundRect.Height();
		ltPoint.iX = aBoundRect.iTl.iX + (aBoundRect.Width() - aBoundRect.Height()) / 2;
		ltPoint.iY = aBoundRect.iTl.iY;
		aGraphicRect.SetRect( ltPoint, TSize( unitLength, unitLength ) );
		}
	else
		{
		unitLength = aBoundRect.Width();
		ltPoint.iX = aBoundRect.iTl.iX;
		ltPoint.iY = aBoundRect.iTl.iY + (aBoundRect.Height() - aBoundRect.Width()) / 2;
		aGraphicRect.SetRect( ltPoint, TSize( unitLength, unitLength ) );			
		}
	aGraphicRect.Shrink(5, 5);
	}

// ---------------------------------------------------------------------------
// calculate the graphic rect for common button
// ---------------------------------------------------------------------------
//
EXPORT_C void AknPenImageUtils::CalculateLongBtnGraphicRect(const TRect& aBoundRect, 
															TRect& aGraphicRect)
	{
	// Shrink 1/5 in width and height
	TInt shrLength = aBoundRect.Width() / 5;
	TInt shrHeight = aBoundRect.Height() / 5;
	aGraphicRect = aBoundRect;
	aGraphicRect.Shrink( shrLength, shrHeight );	
	}

// ---------------------------------------------------------------------------
// DrawColorIcon
// ---------------------------------------------------------------------------
//	
EXPORT_C void AknPenInputDrawUtils::DrawColorIcon( CPenInputColorIcon *aColorIcon,   										 
								 				   CFbsBitGc& aGc,
								 				   const TRect& aRect )
	{
	if( NULL == aColorIcon )
		return;
	
	CFbsBitmap* bmpMask = aColorIcon->IsDimmed() ? aColorIcon->DimmedBitmapMask() :
												   aColorIcon->BitmapMask();
	TRect srcRect( TPoint( 0, 0 ), aColorIcon->Bitmap()->SizeInPixels() );
	if( bmpMask )
		{
		aGc.BitBltMasked( aRect.iTl, 
						  aColorIcon->Bitmap(), 
						  srcRect,
						  bmpMask,
						  EFalse);
		}
	}

// ---------------------------------------------------------------------------
// Draw3PiecesColorIcon
// ---------------------------------------------------------------------------
//	
EXPORT_C void AknPenInputDrawUtils::
				Draw3PiecesColorIcon( CPenInput3PiecesColorIcons *aColorIcons,   										 
						 		      CFbsBitGc& aGc,
						 		      const TRect& aOuterRect,
						 		      const TRect& aInnerRect )
	{
	if( NULL == aColorIcons)
		return;
	
	TRect aSideRect1, aSideRect2;
	// horizon
	if( aOuterRect.iTl.iY == aInnerRect.iTl.iY && aOuterRect.iBr.iY == aInnerRect.iBr.iY )
		{
		aSideRect1 = TRect( aOuterRect.iTl, TPoint( aInnerRect.iTl.iX, aInnerRect.iBr.iY ) );
		aSideRect2 = TRect( TPoint( aInnerRect.iBr.iX, aInnerRect.iTl.iY), aOuterRect.iBr );	
		}
	else if( aOuterRect.iTl.iX == aInnerRect.iTl.iX &&  aOuterRect.iBr.iX == aInnerRect.iBr.iX )
		{
		aSideRect1 = TRect( aOuterRect.iTl, TPoint( aInnerRect.iBr.iX, aInnerRect.iTl.iY ) );
		aSideRect2 = TRect( TPoint( aInnerRect.iTl.iX, aInnerRect.iBr.iY), aOuterRect.iBr );			
		}
	
	if( aColorIcons->FirstIcon() )
		{
		if( aColorIcons->FirstIcon()->Bitmap()->SizeInPixels() != aSideRect1.Size() )
			TRAP_IGNORE( aColorIcons->FirstIcon()->ResizeL( aSideRect1.Size() ) );
		
		AknPenInputDrawUtils::DrawColorIcon( aColorIcons->FirstIcon(),
											 aGc,
											 aSideRect1 );			
		}
	if( aColorIcons->MiddleIcon() )
		{
		if( aColorIcons->MiddleIcon()->Bitmap()->SizeInPixels() != aInnerRect.Size() )
			TRAP_IGNORE( aColorIcons->MiddleIcon()->ResizeL( aInnerRect.Size() ) );

		AknPenInputDrawUtils::DrawColorIcon( aColorIcons->MiddleIcon(),
											 aGc,
											 aInnerRect );			
		}
	if( aColorIcons->LastIcon() )
		{
		if( aColorIcons->LastIcon()->Bitmap()->SizeInPixels() != aSideRect2.Size() )
			TRAP_IGNORE( aColorIcons->LastIcon()->ResizeL( aSideRect2.Size() ) );
		
		AknPenInputDrawUtils::DrawColorIcon( aColorIcons->LastIcon(),
											 aGc,
											 aSideRect2 );			
		}										 	
	}
	
EXPORT_C CPenInputColorIcon* CPenInputColorIcon::NewL( TInt aResID )
	{
	CPenInputColorIcon *self = CPenInputColorIcon::NewLC( aResID );
	CleanupStack::Pop(self);
	return self;
	}
	
EXPORT_C CPenInputColorIcon* CPenInputColorIcon::NewLC( TInt aResID )
	{
	CPenInputColorIcon *self = new (ELeave) CPenInputColorIcon( aResID );
    
    CleanupStack::PushL(self);
    self->ConstructL();
    
    return self;	
	}
	
EXPORT_C void CPenInputColorIcon::ConstructFromResourceL( TInt aResID )
	{
	if (aResID == KInvalidResId)
		{
	    User::Leave( KErrArgument );
		}
	DestroyRes();
		
	TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, aResID);
    
    // Read the file name of the bmps
    TPtrC bmpFileName = reader.ReadTPtrC();    
    TInt32 imgMajorSkinId = reader.ReadInt32();
    TInt colorGroup = reader.ReadInt16();
    TAknsItemID id;

	// Get the image ids and mask ids from resource
    TInt bmpId = reader.ReadInt16(); 
    TInt bmpMskId = reader.ReadInt16();
    
    // Read skin item id
    const TInt skinitemid = reader.ReadInt16();
    id.Set(TInt(imgMajorSkinId), skinitemid);
	
    if ( bmpId == KInvalidBmp || 
    	 bmpMskId == KInvalidBmp ||
    	 colorGroup == KInvalidColorGroup )
    	{
    	User::Leave( KErrGeneral );
    	}
    	
    AknsUtils::CreateColorIconL( AknsUtils::SkinInstance(),
				                 id,
				                 KAknsIIDQsnIconColors,
				                 colorGroup,
				                 iBmp,
				                 iBmpMask,
				                 bmpFileName,
				                 bmpId,
				                 bmpMskId,
				                 TRgb() );   	

    CleanupStack::PopAndDestroy(); // reader
	}
		
EXPORT_C void CPenInputColorIcon::ResizeL( const TSize& aSize )
	{
	ASSERT( iBmp && iBmpMask );
	AknIconUtils::SetSize(iBmp, aSize, EAspectRatioNotPreserved);
    AknIconUtils::SetSize(iBmpMask, aSize, EAspectRatioNotPreserved);
	//CreateDimmedMaskL( iDimmedBmp, iBmpMask );	
	}
	
CPenInputColorIcon::~CPenInputColorIcon()
	{
	DestroyRes();
	}
	
void CPenInputColorIcon::CreateDimmedMaskL( CFbsBitmap*& aDimmedMask,
                            				const CFbsBitmap* aMask )
	{
	if (aMask && aMask->DisplayMode() == EGray256)
        {
        delete aDimmedMask;
        aDimmedMask = NULL;

        aDimmedMask = new (ELeave) CFbsBitmap;

        User::LeaveIfError(aDimmedMask->Create(aMask->SizeInPixels(), EGray256)); 
        CleanupStack::PushL(aDimmedMask);

        CFbsBitmapDevice* bitmapDevice = CFbsBitmapDevice::NewL(aDimmedMask);
        CleanupStack::PushL(bitmapDevice);

        CFbsBitGc* bitGc(NULL);
        User::LeaveIfError(bitmapDevice->CreateContext(bitGc));
        CleanupStack::PushL(bitGc);

        bitGc->SetPenStyle(CGraphicsContext::ESolidPen);
        bitGc->BitBlt(TPoint(0, 0), aMask);

        aDimmedMask->LockHeap();
        TInt w = aMask->SizeInPixels().iWidth; 
        TInt h = aMask->SizeInPixels().iHeight;
        TInt dataStride = aMask->DataStride() - w; 
        unsigned char* address = (unsigned char *)aDimmedMask->DataAddress();  

        for ( TInt i = 0; i < h; ++i )
            {
            for ( TInt j = 0; j < w; ++j )
                {
                *address = KTransparency[*address];
                ++address;
                }
            address += dataStride;         
            }

        aDimmedMask->UnlockHeap();

        CleanupStack::PopAndDestroy(2); // bitmapDevice, bitGc
        CleanupStack::Pop(1); // aDimmedMask
        }
	}

void CPenInputColorIcon::DestroyRes()
	{
	delete iBmp;
	delete iBmpMask;
	delete iDimmedBmp;
	iBmp = NULL;
	iBmpMask = NULL;
	iDimmedBmp = NULL;
	}
	
EXPORT_C CPenInput3PiecesColorIcons* 
		 	CPenInput3PiecesColorIcons::NewL( TInt aFirstIconResID,
										   	  TInt aMiddleIconResID,
										   	  TInt aLastIconResID )
	{
	CPenInput3PiecesColorIcons *self = 
    CPenInput3PiecesColorIcons::NewLC( aFirstIconResID,
                                       aMiddleIconResID,
                                       aLastIconResID );
	CleanupStack::Pop(self);
	return self;
	}
	
EXPORT_C CPenInput3PiecesColorIcons* 
			CPenInput3PiecesColorIcons::NewLC( TInt aFirstIconResID,
										   	   TInt aMiddleIconResID,
										   	   TInt aLastIconResID )
	{
	CPenInput3PiecesColorIcons *self = new (ELeave) CPenInput3PiecesColorIcons();
    
    CleanupStack::PushL(self);
    self->ConstructFromResourceL( aFirstIconResID,
    							  aMiddleIconResID,
    							  aLastIconResID);
    
    return self;	
	}	
	
CPenInput3PiecesColorIcons::~CPenInput3PiecesColorIcons()
	{
	delete iFirstIcon;
	delete iMiddleIcon;
	delete iLastIcon;
	}
	
void CPenInput3PiecesColorIcons::ConstructFromResourceL( TInt aFirstIconResID,										 					
								 						 TInt aMiddleIconResID,
								 						 TInt aLastIconResID )
	{
	iFirstIcon = CPenInputColorIcon::NewL( aFirstIconResID );
	iMiddleIcon = CPenInputColorIcon::NewL( aMiddleIconResID );
	iLastIcon = CPenInputColorIcon::NewL( aLastIconResID );		
	}
	
EXPORT_C void CPenInput3PiecesColorIcons::ReConstructL()
	{
	iFirstIcon->ReConstructL();
	iMiddleIcon->ReConstructL();
	iLastIcon->ReConstructL();	
	}

	
EXPORT_C CPeninputSyncBitmapRotator* CPeninputSyncBitmapRotator::NewL()
    {
    CPeninputSyncBitmapRotator* self = new(ELeave) CPeninputSyncBitmapRotator();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }

/**
Constructor for this class. Adds itself to <code>CActiveScheduler</code>.
The priority of this active object is CActive::EPriorityIdle
*/

CPeninputSyncBitmapRotator::CPeninputSyncBitmapRotator()
    :iScanlineDes(NULL,0)
    {
    }


/**
Performs second phase of contruction
*
*/

void CPeninputSyncBitmapRotator::ConstructL()
    {
    }

/**
*
* Default destructor for this class.
*
*/

CPeninputSyncBitmapRotator::~CPeninputSyncBitmapRotator()
    {
    Cleanup();

    // should have been deleted by cleanup
    ASSERT(iScanlineBuffer==NULL);
    ASSERT(iTempBitmap==NULL);
    ASSERT(iDevice==NULL);
    ASSERT(iGc==NULL);
    }

/**
This function performs deallocation of memory allocated by the class

*/

void CPeninputSyncBitmapRotator::Cleanup()
    {
    delete [] iScanlineBuffer; iScanlineBuffer = NULL;
    delete iTempBitmap; iTempBitmap = NULL;
    delete iGc; iGc = NULL;
    delete iDevice; iDevice = NULL;
    delete iScanlineBitmap; iScanlineBitmap = NULL;
#if defined(ROTATION_PROFILING)
    TUint fcTaken=User::FastCounter() - iStartedAtFc;
    RDebug::Print(_L("BmpRotator: FC time taken %d"),fcTaken);
#endif //   
    }

/**
The function Rotate schedules a rotate/mirror operation on a bitmap supplied in the 
aSrcBitmap whose output overwrites aSrcBitmap.
Preconditions:
aRequestStatus is not a NULL pointer
aSrcBitmap     is a fully constructed bitmap of unknown size including zero dimension
aAngle         is a member of the enumeration TSyncRotationAngle

@param "CFbsBitmap& aBitmap" 
       is a reference to a CFbsBitmap. This bitmap should have been created
       and is also an output
@param "TSyncRotationAngle aAngle"
       aAngle is a member of the enumeration TSyncRotationAngle and specifies the rotation mirror operation   

@panic  This function panics with TBitmapTransformsMain::ENoSourceBitmap when the aSrcBitmap has not been constructed
ie its handle is 0

Sucess Guarantee
aSrcBitmap      contains the rotated bitmap
aRequestStatus  points to the value KErrorNone

*/

EXPORT_C TInt CPeninputSyncBitmapRotator::Rotate(CFbsBitmap& aBitmap, TSyncRotationAngle aAngle)
    {
    //[ panic if the src has not been created]
    __ASSERT_ALWAYS( (aBitmap.Handle() != 0), User::Panic(_L("PENINPUTUTILITY"), ENoSourceBitmap ) );

    //[ assert the angle is ok ]
    __ASSERT_ALWAYS( ( aAngle >= CBitmapRotator::ERotation90DegreesClockwise ) &&
                     ( aAngle <= CBitmapRotator::EMirrorVerticalAxis ), 
                     User::Panic(_L("PENINPUTUTILITY"),EBadArgumentRotate) );

    ASSERT(iTempBitmap==NULL);
    iTempBitmap = new CFbsBitmap;
    if (!iTempBitmap)
        {
        return KErrNoMemory;
        }
    return Rotate(aBitmap, *iTempBitmap, aAngle);
    }

    /**
The Rotate function schedules a rotate/mirror operation on a bitmap supplied in the srcBitmap and 
produces the output in the tgtBitmap.
The CPeninputSyncBitmapRotator is an active object and as such provides asynchronous operations

Preconditions:
aRequestStatus is not a NULL pointer
aSrcBitmap     is a fully constructed bitmap of unknown size including zero dimension and 
of type EColor16M
aTgtBitmap     is a fully constructed bitmap of unknown size including zero dimension
and is of type EColor16M
aAngle         is a member of the enumeration TSyncRotationAngle 

Sucess Guarantee:
aTgtBitmap      contains the rotated bitmap
aRequestStatus  points to the value KErrNone

Minimal Guarantee:
The bitmap supplied in aSrcBitmap is unaltered


@param "CFbsBitmap& aSrcBitmap"
       This bitmap should have been created and be of type EColor16M
@param "CFbsBitmap& aTgtBitmap"
       This bitmap should have been created and be of type EColor16M
@param "TSyncRotationAngle aAngle" 
       is a member of the enumeration TSyncRotationAngle and specifies the rotation mirror operation


@panic This function panics with TBitmapTransformsMain::ENoSourceBitmap when the aSrcBitmap has not been constructed
       i.e. its handle is zero


*/

EXPORT_C TInt CPeninputSyncBitmapRotator::Rotate(CFbsBitmap& aSrcBitmap, CFbsBitmap& aTgtBitmap, TSyncRotationAngle aAngle)
    {
    //[ panic if the src has not been created]
    __ASSERT_ALWAYS( (aSrcBitmap.Handle() != 0), User::Panic(_L("PENINPUTUTILITY"), ENoSourceBitmap ) );

    //[ assert the angle is ok ]
    __ASSERT_ALWAYS( ( aAngle >= CBitmapRotator::ERotation90DegreesClockwise ) && 
                     ( aAngle <= CBitmapRotator::EMirrorVerticalAxis ), 
                     User::Panic(_L("AKNFEP"),EBadArgumentRotate) );

    //[ we do not need to ensure a tgt has been created]
#if defined(ROTATION_PROFILING)
    iStartedAtFc = User::FastCounter();
#endif // ROTATION_PROFILING    
    // Initialize member variables
    iSrcBitmap = &aSrcBitmap;
    iTgtBitmap = &aTgtBitmap;
    iAngle = aAngle;
    iCurOffset = 0;
    iBitmapSize = iSrcBitmap->SizeInPixels();
    iDisplayMode = iSrcBitmap->DisplayMode();

    TSize newSize;
    if ((aAngle == CBitmapRotator::ERotation90DegreesClockwise) || (aAngle == CBitmapRotator::ERotation270DegreesClockwise))
        {
        newSize.SetSize(iBitmapSize.iHeight, iBitmapSize.iWidth); // Swap width and height
        }
    else
        {
        newSize.SetSize(iBitmapSize.iWidth, iBitmapSize.iHeight);
        }
    TInt err = iTgtBitmap->Create(newSize, iDisplayMode);
    
    TBool bmpDeviceNeeded=EFalse;
    if (iSrcBitmap->DisplayMode()==EColor16M || iSrcBitmap->DisplayMode()==EColor16MU ||
        iSrcBitmap->DisplayMode()==EColor16MA ||
        iSrcBitmap->DisplayMode()==EColor256 || iSrcBitmap->DisplayMode()==EGray256 || 
        iSrcBitmap->DisplayMode()==EColor64K )
        {
        iScanlineDisplayMode = iSrcBitmap->DisplayMode();
        }
    else
        {
        // we can't cope with that color mode, then use the highest one for intermediate buffer
        bmpDeviceNeeded     = ETrue;
        iScanlineDisplayMode= EColor16MA;
        }
    iPixelSizeInBytes = TDisplayModeUtils::NumDisplayModeBitsPerPixel(iScanlineDisplayMode) / 8;
    
    TInt scanlineLength=iSrcBitmap->ScanLineLength(newSize.iWidth, iScanlineDisplayMode);
    scanlineLength=Align4(scanlineLength);
    if (err == KErrNone)
        {
        ASSERT(iScanlineBuffer==NULL);
        iScanlineBuffer = new TUint32 [ scanlineLength ];
        if(!iScanlineBuffer)
            {
            err = KErrNoMemory;
            }
        }
    if (err != KErrNone)
        {		
        return err;
        }
    iRows = newSize.iHeight;

    TPtr8 scanlineDes(reinterpret_cast<TText8*>(iScanlineBuffer),scanlineLength,scanlineLength); // Use a temporary to avoid compiler warnings
    iScanlineDes.Set(scanlineDes);
    
    ASSERT(iDevice==NULL);
    if (bmpDeviceNeeded)
        {
        iScanlineBitmap = new CFbsBitmap();
        err= (iScanlineBitmap?    iScanlineBitmap->Create(TSize(iTgtBitmap->SizeInPixels().iWidth, 1), iScanlineDisplayMode)
                                : KErrNoMemory);
        if (err==KErrNone)
            {
            TRAP(err, iDevice = CFbsBitmapDevice::NewL(iTgtBitmap));
            }
        if (err == KErrNone)
            {
            err = iDevice->CreateContext(iGc);
            }
        }    
    DoRotate();    
    
    return err;
    }

/**
This function is called by the Active Scheduler
to perform the rotate operation

*/
void CPeninputSyncBitmapRotator::DoRotate()
    {
    while (iCurOffset < iRows)
        {
        switch (iAngle)
            {
            // Rotation of 90 degrees
            case CBitmapRotator::ERotation90DegreesClockwise:
                {
                iSrcBitmap->GetVerticalScanLine(iScanlineDes, iCurOffset, iScanlineDisplayMode);
                FlipScanLine(iScanlineDes, iTgtBitmap->SizeInPixels().iWidth);
                PutScanline(iCurOffset);
                break;
                }
            
            // Rotation of 180 degrees
            case CBitmapRotator::ERotation180DegreesClockwise:
                {
                iSrcBitmap->GetScanLine(iScanlineDes,TPoint(0, iCurOffset), iBitmapSize.iWidth, iScanlineDisplayMode);
                FlipScanLine(iScanlineDes, iBitmapSize.iWidth);
                PutScanline(iBitmapSize.iHeight - 1 - iCurOffset);
                break;
                }

            // Rotation of 270 degrees
            case CBitmapRotator::ERotation270DegreesClockwise:
                {
                iSrcBitmap->GetVerticalScanLine(iScanlineDes, iCurOffset, iScanlineDisplayMode);
                PutScanline(iBitmapSize.iWidth - 1 - iCurOffset);
                break;
                }

            // Flip about the vertical Axis
            case CBitmapRotator::EMirrorVerticalAxis:    
                {
                iSrcBitmap->GetScanLine(iScanlineDes,TPoint(0,iCurOffset),iBitmapSize.iWidth,iScanlineDisplayMode);
                FlipScanLine(iScanlineDes, iBitmapSize.iWidth);
                PutScanline(iCurOffset);
                break;
                }

            // Flip about the horizontal axis
            case  CBitmapRotator::EMirrorHorizontalAxis:
                {
                iSrcBitmap->GetScanLine(iScanlineDes,TPoint(0,iCurOffset),iBitmapSize.iWidth,iScanlineDisplayMode);
                PutScanline(iBitmapSize.iHeight-1-iCurOffset);
                break;
                }

            default:
                {
                ASSERT( EFalse );
                }
            }

        iCurOffset++;
        }

    if (iCurOffset == iRows)
        {
        if (iTempBitmap)
            {
            iSrcBitmap->Duplicate(iTgtBitmap->Handle());
            }
        Cleanup();                    
        }
    }

/**
  template function that can be used for swapping of memory locations 
  of particular pointer type
  @param ptr1 - pointer to value1
  @param ptr2 - pointer to value2
*/
template <class T>
inline void SwapPixels(T* ptr1, T* ptr2)
    {
    T temp  = *ptr1;
    *ptr1   = *ptr2;
    *ptr2   = temp;
    }
    
/**
  template function that can be used for mirroring of linear
  memory location of a particular scalar type
  @param aScanLinePtr location address
  @param aWidth width of location in elements, not bytes
*/  
template <class T>
inline void FlipLine(TUint8* aScanLinePtr, TInt aWidth)
    {
    T* startPixelPtr    = reinterpret_cast<T*>(aScanLinePtr);
    T* endPixelPtr      = startPixelPtr + aWidth - 1;
    if (aWidth&1)
        {
        SwapPixels(startPixelPtr++, endPixelPtr--);
        }
    while (startPixelPtr < endPixelPtr)
        {
        SwapPixels(startPixelPtr++, endPixelPtr--);
        SwapPixels(startPixelPtr++, endPixelPtr--);
        }   
    }
    
/**
This function flips a scan line buffer of width aWidth
PreConditions:
aWidth >= 0 && aWidth is the length of the buffer
aDes is a reference to a buffer of rgb pixels
Postcondition:
The contents of the buffer have flipped about the buffers centre

@param TDes8 aDes
       reference to a buffer of rgb pixels of lenth aWidth
@param Tint aWidth
       is the width of the buffer

*/
void CPeninputSyncBitmapRotator::FlipScanLine(TDes8& aDes, TInt aWidth)
    {
    //[ assert consistency between descriptor length and width ]
    ASSERT( (aDes.Length() == (iPixelSizeInBytes * aWidth) ));
    
    TUint8* const scanLinePtr=const_cast<TUint8*>(aDes.Ptr());
    
    switch (iPixelSizeInBytes)
        {
        case 4:
            {
            FlipLine<TUint32>(scanLinePtr, aWidth);
            }
            break;
        case 3:
            {
            TUint8* startPixelPtr = scanLinePtr;
            TUint8* endPixelPtr = startPixelPtr + (aWidth - 1)*iPixelSizeInBytes;
    while (startPixelPtr < endPixelPtr)
        {
        TUint8 temp0 = startPixelPtr[0];
        TUint8 temp1 = startPixelPtr[1];
        TUint8 temp2 = startPixelPtr[2];
        startPixelPtr[0] = endPixelPtr[0];
        startPixelPtr[1] = endPixelPtr[1];
        startPixelPtr[2] = endPixelPtr[2];
        endPixelPtr[0] = temp0;
        endPixelPtr[1] = temp1;
        endPixelPtr[2] = temp2;
                startPixelPtr += iPixelSizeInBytes;
                endPixelPtr -= iPixelSizeInBytes;               
                }
            }
            break;
        case 2:
            {
            FlipLine<TUint16>(scanLinePtr, aWidth);
            }
            break;
        case 1:
            {
            FlipLine<TUint8>(scanLinePtr, aWidth);
            }
            break;

        default:
            ASSERT(EFalse);
        }
    }

void CPeninputSyncBitmapRotator::PutScanline(TInt aYPos)
    {
    if (iScanlineBitmap)
        {
        iScanlineBitmap->SetScanLine(iScanlineDes, 0);
        iGc->BitBlt(TPoint(0, aYPos), iScanlineBitmap); 
        }
    else
        {
        iTgtBitmap->SetScanLine(iScanlineDes, aYPos);
        }
    }


EXPORT_C TBool AknPenInputTrailColorUtils::CheckColorInColorTable(TInt color)
    {
        for (TInt i = 0; i < ColorCount(); i++)
            {
            if (ColorAt(i).Value() == color)
                {
                return ETrue;
                }
            }
     return EFalse;
    }

EXPORT_C TInt AknPenInputTrailColorUtils::ColorCount()
    {
	const TRgb KRgbArray[] = 
	    {
	    KRgbBlack,
	    KRgbDarkGray,
	    KRgbDarkRed,
	    KRgbDarkGreen,
	    KRgbDarkYellow,
	    KRgbDarkBlue,
	    KRgbDarkMagenta,
	    KRgbDarkCyan,
	    KRgbRed,
	    KRgbGreen,
	    KRgbYellow,
	    KRgbBlue,
	    KRgbMagenta,
	    KRgbCyan,
	    KRgbGray,
	    KRgbWhite    
	    };
    
    return sizeof(KRgbArray)/sizeof(TRgb);
    }

EXPORT_C TRgb AknPenInputTrailColorUtils::ColorAt(TInt aIndex)
    {
	const TRgb KRgbArray[] = 
	    {
	    KRgbBlack,
	    KRgbDarkGray,
	    KRgbDarkRed,
	    KRgbDarkGreen,
	    KRgbDarkYellow,
	    KRgbDarkBlue,
	    KRgbDarkMagenta,
	    KRgbDarkCyan,
	    KRgbRed,
	    KRgbGreen,
	    KRgbYellow,
	    KRgbBlue,
	    KRgbMagenta,
	    KRgbCyan,
	    KRgbGray,
	    KRgbWhite    
	    };    
	    
    if((aIndex >=0) && (aIndex < ColorCount()))
        return KRgbArray[aIndex];
    else
        return KRgbArray[0];
    }
EXPORT_C TInt AknPenInputTrailColorUtils::GetTrailColorByTheme()
    {
    TRgb color(0x000000);
    AknsUtils::GetCachedColor( AknsUtils::SkinInstance(),
                color, KAknsIIDQsnOtherColors, EAknsCIQsnOtherColorsCG20 );
    return color.Value();

    }

// End Of File
