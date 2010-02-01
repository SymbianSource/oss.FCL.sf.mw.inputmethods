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
* Description:  common layout framework
*
*/


// System includes
#include <aknfeppeninputenums.h>    // Command from fep or IME to plugin
#include <AknFepGlobalEnums.h>      // Define EPluginInputModeHwr
#include <peninputcmd.h>            // Use global signal
#include <peninputlayoutchoicelist.h>
#include <peninputlayoutbutton.h>
#include <peninputdragbutton.h>
#include <coemain.h>

// User includes
#include "peninputuistatebase.h"
#include "peninputdataconverter.h"
#include "peninputcommonlayout.h"
#include "peninputlayoutwindow.h"
#include "peninputdatamgr.h"
#include "peninputuistatemgr.h"
#include "peninputrangebarinfo.h"	

// Constants
const TInt KPeninputCommonLayoutBufLen = 8;
const TInt KIntSizeToInt16 = 2;

// ======== MEMBER FUNCTIONS ========

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::CPeninputCommonLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputCommonLayout::CPeninputCommonLayout( MLayoutOwner* aLayoutOwner )
    : CFepUiLayout( aLayoutOwner )
    {
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputCommonLayout::ConstructL( const TAny* aInitData )
    {
    BaseConstructL();

    TPixelsTwipsAndRotation ptSize;
    
    CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation( ptSize );
            
    SetScreenSize(ptSize.iPixelSize);    
    
    // This method must be called first, because it will initialize 
    // language from CenRep
    CreateDataMgrL(aInitData);

    TBool sizeChanging = ETrue;
    if ( iDataMgr )
        {
        // Store screen mode, and get new window position
        iDataMgr->SetData( EPeninputDataTypeScreenMode, ( TAny* ) &ptSize );
        
        // Store justify style
        iDataMgr->SetData(EAkninputDataTypeSizeChanging, &sizeChanging);
        }

    iUiStateMgr = CPeninputUiStateMgr::NewL( this );

    CreateLayoutWindowL();

    if ( iLayoutWindow )
        {
        iLayoutWindow->Hide(ETrue);
        AddControlL( iLayoutWindow );
        
        // Config layout self using the whole screen
        //SetRect(TRect(TPoint(0, 0), ptSize.iPixelSize ));

        // Call the function in window to read config info
        //ChangeInputLanguage( *( (TInt*)iDataMgr->RequestData(EPeninputDataTypeInputLanguage)));

        iLayoutWindow->SizeChanged( ETrue );
        EnableLayoutMoving( static_cast<CDragBar*>( iLayoutWindow->MoveButton() ) );
        }
    
    if (iDataMgr)
    	{
    	sizeChanging = EFalse;
    	iDataMgr->SetData(EAkninputDataTypeSizeChanging, &sizeChanging);
    	}
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::~CPeninputCommonLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputCommonLayout::~CPeninputCommonLayout()
    {       
    delete iUiStateMgr;
    delete iDataMgr;
    
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::UiStateMgr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C MPeninputUiStateMgr* CPeninputCommonLayout::UiStateMgr() const
    {
    return iUiStateMgr;
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::LayoutWindow
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutWindow* CPeninputCommonLayout::LayoutWindow() const
    {
    return NULL;
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::HandleCommand
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPeninputCommonLayout::HandleCommand( TInt aCmd, 
                                                    TUint8* aData )
    {
    // Handle command from fep or IME to this layout
    TUint* data = ( TUint* ) aData;
    TBuf<KPeninputCommonLayoutBufLen> buf;
    TBuf<KIntSizeToInt16> bufStyle;
    TInt style;
    TInt ret = KErrNone;
    
    switch ( aCmd )
        {
        case ECmdPenInputPermittedRange:
            CPeninputDataConverter::IntToDesc( *data, buf );
            SendEventToWindow( EPeninputLayoutEventSetPermittedRange, NULL, 
                               buf );        
            break;
        case ECmdPenInputPermittedCase:
            {
            CPeninputDataConverter::IntToDesc( *data, buf );
            SendEventToWindow( EPeninputLayoutEventSetPermittedCase, NULL, 
                               buf );             
            }
            break;
        case ECmdPenInputEditorNumericKeyMap:
            {
            iDataMgr->SetData( EPeninputDataTypeNumericKeymap, data );
            }
            break;
        case ECmdPenInputCase:
            {
            CPeninputDataConverter::IntToDesc( *data, buf );
            SendEventToWindow( EPeninputLayoutEventSetCase, NULL, 
                               buf );
            }
            break;
        case ECmdPenInputLanguage:
            {
            ChangeInputLanguage(*data);
            }            
            break;
        case ECmdPenInputRange:
            {
            ChangeRange( TranslateRange(*data) );
            }
            break;
        case ECmdPenInputEnableSettingBtn:
            {
            SetEnableSettingBtn(*data);
            }
            break;
        case ECmdPenInputIsSecretText:
            {
            iLayoutWindow->SetTextIsSecret( *aData );
            }
            break;
        case ECmdPenInputIsNumberGrouping:
            {
            iLayoutWindow->SetNumberGrouping( *aData );
            }
            break;    
        case ECmdPenInputWindowOpen:
            {
            if ( *data == EPenInputOpenManually )
                {
                style = EPeninputPositionChangeBrJustify;
                bufStyle = ( TUint16* )&style;
                SendEventToWindow(EPeninputLayoutEventMovePosition, NULL, bufStyle );  
                }
            iLayoutWindow->Hide(EFalse);
            LayoutOwner()->Hide( EFalse );
            }
            break;
        case ECmdPenInputWindowClose:
            {
            iLayoutWindow->Hide(ETrue);            
            LayoutOwner()->Hide( ETrue );

            SendEventToWindow( EPeninputLayoutEventCloseWindow, NULL );
            }
            break;
        case ECmdPenInputSetWindowPos:
            {
            // Only VKB need to move position automatically
            if ( LayoutType() == EPluginInputModeVkb )
                {
                // Data is a TRect
                iDataMgr->SetData( EPeninputDataTypeDataQueryDlgRect, 
                                   (TRect*) aData );
                                   
                // Store justify style
                style = EPeninputPositionChangeDataQuery;
                bufStyle = ( TUint16* )&style;
                SendEventToWindow(EPeninputLayoutEventMovePosition, NULL, bufStyle);
                }
            }
            break;
        case ECmdPenInputEditorCustomNumericKeyMap:
            {
            iDataMgr->SetData(EAknFepDataTypeUseDefinedResource, aData);
            }
            break;    
        case ECmdPenInputSetTextAlignment:
        	{
		    TRAP_IGNORE(iLayoutWindow->SetTextAlignmentL( *data ));
        	}
        	break;
        default:
            {
            ret = CFepUiLayout::HandleCommand( aCmd, aData );
            }
            break;
        }

    return ret;
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::SizeChanged
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPeninputCommonLayout::SizeChanged( const TAny* /*aData*/ )
    {
    TBool sizeChanging = ETrue;
    iDataMgr->SetData(EAkninputDataTypeSizeChanging, &sizeChanging);

    // Handle size changed event
    TPixelsTwipsAndRotation ptSize;  
       
    CCoeEnv::Static()->ScreenDevice()->GetDefaultScreenSizeAndRotation( ptSize );
    //SetScreenSize(ptSize.iPixelSize);

    // Store screen mode, and get new window position
    iDataMgr->SetData( EPeninputDataTypeScreenMode, ( TAny* )&ptSize );

     // The following need to implement
    iLayoutWindow->SizeChanged( ETrue );

    sizeChanging = EFalse;
    iDataMgr->SetData(EAkninputDataTypeSizeChanging, &sizeChanging);

    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::HandleControlEvent
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputCommonLayout::HandleControlEvent(
    TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    // Handle event from control
    TInt* data = ( TInt* ) aEventData.Ptr();

    // Buffer for data conversion
    TBuf<KPeninputCommonLayoutBufLen> buf;

    switch ( aEventType )
        {
        case EPeninputLayoutEventClose:
            {
            SignalOwner( ESignalLayoutClosed );
            }
            break;
        case EPeninputLayoutEventOption:
            {
            SignalOwner( ESignalLaunchOptionMenu );
            }
            break;
        case EPeninputLayoutEventSwitchLanguage:
            {
            SignalOwner( ESignalLaunchLanguageMenu );
            }
            break;
        // When drag end, store the position data
        case EEventDraggingEnd:
            {
            TInt x = 0;
            TInt y = 0;
            x = 0x0000FFFF & ( Position().iX + Rect().Width() );
            y = 0xFFFF0000 & ( ( Position().iY + Rect().Height() ) << 16 );
            TInt position = x | y;
            if ( position )
                {
                iDataMgr->SetData( EPeninputDataTypeWindowPosition, &position );
                }
            }
            break;
        default:
            {
            if ( iUiStateMgr->CurrentUiState() )
                {
                if ( !iUiStateMgr->CurrentUiState()->HandleControlEvent
                    ( aEventType, aEventData ) )
                    {
                    SendEventToWindow( aEventType, aCtrl, aEventData );
                    }                    
                }
            }
            break;       
        }
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::OnAppEditorTextComing
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TInt CPeninputCommonLayout::OnAppEditorTextComing(
    const TFepInputContextFieldData& aData )
    {
    // Handle edit text coming
    if ( iLayoutWindow )
        {
        TRAPD( err, iLayoutWindow->SetEditorTextL( aData ) );
        return err;
        }
    
    return KErrNone;
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::HandleRawKeyEventL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPeninputCommonLayout::HandleRawKeyEventL(
    const TRawEvent& aKeyEvent )
    { 
    return iUiStateMgr->CurrentUiState()->HandleKeyEventL( aKeyEvent );
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::SendEventToWindow
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputCommonLayout::SendEventToWindow( 
    TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    iLayoutWindow->HandleControlEvent( aEventType, aCtrl, aEventData );
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::SendEventToLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputCommonLayout::SendEventToLayout( 
    TInt aEventType, CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    HandleControlEvent( aEventType, aCtrl, aEventData );
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::Sendkey
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputCommonLayout::Sendkey( TInt aEventType, 
                                              const TDesC& aEventData )
    {
    SignalOwner( aEventType, aEventData ); 
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::IsShiftPermitted
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//           
EXPORT_C TBool CPeninputCommonLayout::IsShiftPermitted()
    {
    return iDataMgr->IsShiftPermitted();
    }
    
// ---------------------------------------------------------------------------
// CPeninputCommonLayout::IsCapsLockPermitted
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//  
EXPORT_C TBool CPeninputCommonLayout::IsCapsLockPermitted()
    {
    return iDataMgr->IsCapsLockPermitted();
    }
    
// ---------------------------------------------------------------------------
// CPeninputCommonLayout::RequestData
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TAny* CPeninputCommonLayout::RequestData( 
    TPeninputDataType aDataType )
    {
    return iDataMgr->RequestData( aDataType );
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::SetData
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputCommonLayout::SetData( TPeninputDataType aDataType, 
                                              TAny* aData )
    {
    iDataMgr->SetData( aDataType, aData );
    }
                
// ---------------------------------------------------------------------------
// CPeninputCommonLayout::SetData
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPeninputCommonLayout::IsValidDestination( const TRect& aRect,
                                                          CDragBar* aDragBar,
                                                          TBool& aVInfo,
                                                          TBool& aHInfo,
                                                          TBool aFlag )
    {
    // Validate move button destination 
    if ( !aDragBar )
        {
        return EFalse;
        }
        
    TRect rect = aRect;
    
    if ( aFlag )
        {
        // Layout is moving
        aHInfo = aVInfo = EFalse;
        TRect dragbarRect( aRect.iTl, aDragBar->Rect().Size() );
        
        if ( dragbarRect.iTl.iX >= 0 
             && dragbarRect.iBr.iX < ScreenSize().iWidth )
            {
            aHInfo = ETrue;
            }
            
        if ( dragbarRect.iTl.iY >= 0 
             && dragbarRect.iBr.iY < ScreenSize().iHeight )
            {
            aVInfo = ETrue;
            }
        
        return ( aHInfo && aVInfo );
        }
    else
        {
        if ( aDragBar )
            {
            rect.SetSize( aDragBar->Rect().Size() );
            }
            
        TBool tlInvalid = !Rect().Contains( rect.iTl );
        TBool brInvalid = !Rect().Contains( rect.iBr );

        if ( tlInvalid && !brInvalid )
            {
            if ( rect.iTl.iX >= Rect().iTl.iX 
                 && rect.iTl.iX <= Rect().iBr.iX )
                {
                aHInfo = ETrue;
                }
            else if ( rect.iTl.iY >= Rect().iTl.iY 
                      && rect.iTl.iY <= Rect().iBr.iY )
                {
                aVInfo = ETrue;
                }
            }
        else if ( !tlInvalid && brInvalid )
            {
            if ( rect.iBr.iX >= Rect().iTl.iX 
                 && rect.iBr.iX <= Rect().iBr.iX )
                {
                aHInfo = ETrue;
                }
            else if ( rect.iBr.iY >= Rect().iTl.iY 
                      && rect.iBr.iY <= Rect().iBr.iY )
                {
                aVInfo = ETrue;
                }
            }

        return !( tlInvalid || brInvalid );
        }
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::TranslateRange
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TInt CPeninputCommonLayout::TranslateRange(TInt aRange)
    {
    TInt newRange = aRange;
    	     
    if ( aRange == ERangeNative)
        {
        if ( iLayoutWindow
             && iLayoutWindow->ConfigInfo()
             && iLayoutWindow->ConfigInfo()->RangeBarInfo()->FindRange( ERangeNative ) )
            {
            newRange = ERangeNative;    
            }
        else
            {
            newRange = ERangeEnglish;    
            }
        }
    else if ( aRange == ERangeEnglish)
        {
        if ( iLayoutWindow
             && iLayoutWindow->ConfigInfo()
             && iLayoutWindow->ConfigInfo()->RangeBarInfo()->FindRange( ERangeEnglish ) )
            {
            newRange = ERangeEnglish;    
            }
        else
            {
            newRange = ERangeNative;    
            }
        }
                
    return newRange;    
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::Init
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TRect CPeninputCommonLayout::Init()
    {
    TRect r = CFepUiLayout::Init();
    //wxy -- No need to set position here
    //LayoutOwner()->SetPosition( Rect().iTl );
    //r = TRect( Position(), r.Size() );

    return r;
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::SaveKey
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputCommonLayout::SaveKey( TInt aKey, TInt aValue )
    {
    iDataMgr->SaveKey( aKey, aValue );
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::ChangeInputLanguage
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputCommonLayout::ChangeInputLanguage( TInt aLangID )
    {
    // Call ChangeInputLanguage function in iLayoutWindow
    TRAP_IGNORE( iLayoutWindow->ChangeInputLanguageL( aLangID ) );
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::ChangeLanguage
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputCommonLayout::ChangeRange( TInt aRange )
    {
    iLayoutWindow->ChangeRange( aRange, 0 , EFalse);
    }

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::SetEnableSwitchToHwr
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputCommonLayout::SetEnableSwitchToHwr( TBool aEnable )
    {
    iLayoutWindow->SetEnableSwitchToHwr( aEnable );
    }   

// ---------------------------------------------------------------------------
// CPeninputCommonLayout::SetEnableSettingBtn
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputCommonLayout::SetEnableSettingBtn( TBool aEnable )
    {
    iLayoutWindow->SetEnableSettingBtn( aEnable );
    }  

EXPORT_C TInt CPeninputCommonLayout::PenInputType()
    {
    return LayoutType();
    }

EXPORT_C void CPeninputCommonLayout::SetPositionFromOutside(const TPoint& aNewPos)
    {
    LayoutOwner()->SetPosition(aNewPos);
    TInt x = 0x0000FFFF & aNewPos.iX;
    TInt y = 0xFFFF0000 & ( aNewPos.iY << 16 );
    TInt position = x | y;
    
    SetData( EPeninputDataTypeWindowPosition, (TAny*)&position );
    }

