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
* Description:  common layout window for UI interface of VKB and HWR
*
*/


// System includes
#include <coemain.h>

#include <AknFepGlobalEnums.h>
#include <aknfeppeninputenums.h>
#include <peninputpluginutils.h>
#include <peninputcmdparam.h>            // Use global signal
#include <peninputinputcontextfield.h>
#include <peninputeventbutton.h>
#include <peninputmultimodebutton.h>
#include <peninputdragbutton.h>
#include <peninputlayoutchoicelist.h>
#include <peninputlayout.h>
#include <peninputrangebar.h>
#include <bautils.h>
#include <AknIconUtils.h>
#include <AknLayoutDef.h>
#include <AknUtils.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <AknsUtils.h>
#include <peninputlayoutmultilineicf.h>
#include <peninputrepeatbutton.h>

// User includes
#include "peninputdataconverter.h"
#include "peninputrangebarinfo.h"
#include "peninputlayoutwindow.h"
#include "peninputclientlayoutinfo.h"
#include "peninputdataprovider.h"
#include "peninputlayoutcontext.h"
#include "peninputvkbctrlext.h"
#include "peninputnumerickeymappingmgr.h"

// Constants
const TInt KIntLengthForByte = 8;
const TInt KPeninputLayoutWindowRegionCount = 8;
const TInt KPeninputLayoutWindowUnitWidth = 12;
const TInt KInvalidIndex = -1;

const TInt32 KInvalidResId = -1;
const TInt KInvalidImg = -1 ;
const TUint32 KDefaultTextColor = 0x000000;
const TUint32 KDefaultShadowTextColor = 0xffffff;
const TInt KNotSupportSkin = -1;

const TInt KIntSizeToInt16 = 2;



// ======== MEMBER FUNCTIONS ========

// ------------------------------------------------------------------------
// CPeninputLayoutWindow::CPeninputLayoutWindow
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutWindow::CPeninputLayoutWindow( 
    CFepUiLayout* aUiLayout, MPeninputLayoutContext* aLayoutContext )
    : CAknFepCtrlBaseWindow( aUiLayout, EPeninutWindowCtrlIdBaseWindow ), 
     iConfigInfo( NULL ), iLayoutContext( aLayoutContext ),
    iLastUsedTotalColumns( KPeninputLayoutWindowUnitWidth ),
    iFirstTimeConstruct( ETrue ),
    iSwitchFontSet(EFalse),
    iUnitSizeChange(EFalse)
    { 
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::ConstructL()
    {
    BaseConstructL();  

    if( iLayoutContext->LayoutType() == EPluginInputModeVkb )
        {
        iVkbLayout = CAknFepCtrlVkbLayout::NewL(); 
        }
    else
        {
        iVkbLayout = NULL;
        }
        
    CCoeEnv* coeEnv = CCoeEnv::Static();
    	
    // Set up resources
    TFileName resFileName = GetWindowResFileName();
    BaflUtils::NearestLanguageFile(coeEnv->FsSession(), resFileName );
    iWinResId = coeEnv->AddResourceFileL( resFileName );
        
    TResourceReader reader;
    
    coeEnv->CreateResourceReaderLC( reader, GetWindowResId() );
    SetResourceId( GetWindowResId() );
    ConstructFromResourceL();
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );
    MoveButton()->AddEventObserver( UiLayout() );
    iCtrlPool = CPeninputCtrlPool::NewL();
    
    CreateAllControlsL();
    SetControlsFont();
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::~CPeninputLayoutWindow
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutWindow::~CPeninputLayoutWindow()
    {
    delete iCtrlPool;
    delete iConfigInfo;
    delete iKeyMappingMgr;
    delete iVkbLayout;    
    // Delete resource file from CoeEnv
    CCoeEnv::Static()->DeleteResourceFile( iWinResId );
    
    CCoeEnv::Static()->DeleteResourceFile( iConfigResId );
        
    }
    
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ConstructFromResourceL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::ConstructFromResourceL( 
    TResourceReader& aResReader )
    {
    TInt supportskin = aResReader.ReadInt16();

    if (iFirstTimeConstruct && (supportskin != KNotSupportSkin))
        {
    	UiLayout()->AddControlL(this);
        }
    
    const TInt32 moveBtnResId = aResReader.ReadInt32();
    const TInt32 closeBtnResId = aResReader.ReadInt32();
    const TInt32 icfResId = aResReader.ReadInt32();
    const TInt32 optionBtnResId = aResReader.ReadInt32();
    const TInt32 keymappingResId = aResReader.ReadInt32();
    const TInt32 shadowResId = aResReader.ReadInt32();
    const TInt32 arrowLeftResId = aResReader.ReadInt32();
    const TInt32 arrowRightResId = aResReader.ReadInt32();
    const TInt32 bgImageResId = aResReader.ReadInt32();
        
    TResourceReader resReader;
    
    TBool multiLine = EFalse;
    
    if (iFirstTimeConstruct)
        {       
        // Read bitmaps of close and move button   
        CCoeEnv::Static()->CreateResourceReaderLC( resReader, moveBtnResId );
        MoveButton()->SetResourceId( moveBtnResId );
        MoveButton()->ConstructFromResourceL();
        CleanupStack::PopAndDestroy( 1 );
        
        CCoeEnv::Static()->CreateResourceReaderLC( resReader, closeBtnResId );
        CloseButton()->SetResourceId( closeBtnResId );
        CloseButton()->ConstructFromResourceL();
        CleanupStack::PopAndDestroy( 1 );

        // Read information of key mapping 
        CCoeEnv::Static()->CreateResourceReaderLC( resReader, keymappingResId );
        iKeyMappingMgr = CPeninputNumericKepMappingMgr::NewL( resReader );
        CleanupStack::PopAndDestroy( 1 );          
            
        
        multiLine = IsMultiLineIcf();
        
        // Add input context field into the headpane
        if (multiLine)
        	{
        	AddMultiContextFieldL();	
        	}
        else
        	{
        	AddContextFieldL();	
        	}
        
        
        // Add touch input button into the headpane
        AddTouchInputButtonL( optionBtnResId ); 
        }
    else
    	{
    	multiLine = IsMultiLineIcf();
    	CControlGroup::ConstructFromResourceL();
    	}    
    
    // Read information of shadow 
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, shadowResId );
    ReadShadowInfoL( resReader ); 
    CleanupStack::PopAndDestroy( 1 ); 
     
    if (!multiLine)
    	{    
	    //Read resource of icf
	    CCoeEnv::Static()->CreateResourceReaderLC( resReader, icfResId );  
    	ReadIcfInfo( resReader );     	
	    CleanupStack::PopAndDestroy( 1 );
    	}
    else
    	{
    	ReadMultiLineIcfInforL(icfResId);	
    	}    
	
	// Read background image resource
	ReadBackgroundInfoL( bgImageResId );	    	
    iFirstTimeConstruct = EFalse;       
    }

// -----------------------------------------------------------------------------
// CPeninputLayoutWindow::ConstructFromResourceL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::ConstructFromResourceL()
    {    
    if (iResourceId == KInvalidResId)
    	{
        User::Leave(KErrArgument);
    	}

    TResourceReader reader;
    CCoeEnv::Static()->CreateResourceReaderLC(reader, iResourceId);

    TInt supportskin = reader.ReadInt16();
    
    if (iFirstTimeConstruct && (supportskin != KNotSupportSkin))
        {
    	UiLayout()->AddControlL(this);
        }

    const TInt32 moveBtnResId = reader.ReadInt32();
    const TInt32 closeBtnResId = reader.ReadInt32();
    const TInt32 icfResId = reader.ReadInt32();
    const TInt32 optionBtnResId = reader.ReadInt32();
    const TInt32 keymappingResId = reader.ReadInt32();
    const TInt32 shadowResId = reader.ReadInt32();
    const TInt32 arrowLeftResId = reader.ReadInt32();
    const TInt32 arrowRightResId = reader.ReadInt32();
    const TInt32 bgImageResId = reader.ReadInt32();
    
    
    TResourceReader resReader;
    TBool multiLine = EFalse;
        
    if (iFirstTimeConstruct)
        {       
        // Read bitmaps of close and move button   
        CCoeEnv::Static()->CreateResourceReaderLC( resReader, moveBtnResId );
        MoveButton()->SetResourceId( moveBtnResId );
        MoveButton()->ConstructFromResourceL();
        CleanupStack::PopAndDestroy( 1 );
        
        CCoeEnv::Static()->CreateResourceReaderLC( resReader, closeBtnResId );
        CloseButton()->SetResourceId( closeBtnResId );
        CloseButton()->ConstructFromResourceL();
        CleanupStack::PopAndDestroy( 1 );

        // Read information of key mapping 
        CCoeEnv::Static()->CreateResourceReaderLC( resReader, keymappingResId );
        iKeyMappingMgr = CPeninputNumericKepMappingMgr::NewL( resReader );
        CleanupStack::PopAndDestroy( 1 );          
            
        multiLine = IsMultiLineIcf();
        
        // Add input context field into the headpane
        if (multiLine)
        	{
        	AddMultiContextFieldL();	
        	}
        else
        	{
        	AddContextFieldL();	
        	}
        
        // Add touch input button into the headpane
        AddTouchInputButtonL( optionBtnResId ); 
        }
    else
    	{
    	multiLine = IsMultiLineIcf();
    	CControlGroup::ConstructFromResourceL();
    	}    
    
    // Read information of shadow 
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, shadowResId );
    ReadShadowInfoL( resReader ); 
    CleanupStack::PopAndDestroy( 1 );  
    
    if (!multiLine)
    	{    
	    //Read resource of icf
	    CCoeEnv::Static()->CreateResourceReaderLC( resReader, icfResId );  
    	ReadIcfInfo( resReader );     	
	    CleanupStack::PopAndDestroy( 1 );
    	}
    else
    	{
    	ReadMultiLineIcfInforL(icfResId);	
    	}  
    
    CleanupStack::PopAndDestroy(); // reader

	// Read background image resource
	ReadBackgroundInfoL( bgImageResId );
	
    if( iLayoutContext->LayoutType() == EPluginInputModeVkb )
        {
        iVkbLayout->ConstructFromResourceL();
        }
        
    iFirstTimeConstruct = EFalse;   	        
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::SizeChanged
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::SizeChanged( TBool /*aLandscapeStyle*/ )
    {
    iLangOrSizeChanged = ETrue;    
    ChangeUnitSize();
    
    ReadLafForShadow();
    
    if ( iConfigInfo )
        {
        ChangeClientSize();        
        // Change the size of base window
        SizeChangedForBaseWindow( iLastUsedTotalColumns );
        
        }
    
    TInt style = EPeninputPositionChangeBrJustify;
    TBuf<KIntSizeToInt16> bufStyle;
    bufStyle = ( TUint16* )&style;
    HandleControlEvent(EPeninputLayoutEventMovePosition, NULL, bufStyle);           
        
    iLangOrSizeChanged = EFalse;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ReadLafForShadow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindow::ReadLafForShadow()
    {
    if(!CPeninputDataConverter::AnyToInt
        (iLayoutContext->RequestData(EAkninputDataTypeSizeChanging)))
        {
        return;
        }
    
    TRect anyRect;
    
    TAknWindowLineLayout shadowPane;
    TAknWindowLineLayout shadowTl;
    TAknWindowLineLayout shadowBr;
    TAknLayoutRect shadowRect;
    TAknLayoutRect shadowTlRect;
    TAknLayoutRect shadowBrRect;

    shadowPane = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane(1).LayoutLine();
    shadowRect.LayoutRect(anyRect, shadowPane);

    shadowTl = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane_g1().LayoutLine();
    shadowTlRect.LayoutRect(shadowRect.Rect(), shadowTl);

    shadowBr = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane_g8().LayoutLine();
    shadowBrRect.LayoutRect(shadowRect.Rect(), shadowBr);
    
    iShadowTlWidth = shadowTlRect.Rect().Size().iWidth;
    iShadowTlHeight = shadowTlRect.Rect().Size().iHeight;
    iShadowBrWidth = shadowBrRect.Rect().Size().iWidth;
    iShadowBrHeight = shadowBrRect.Rect().Size().iHeight;
    
    //TRect newWinRect = Rect();
    //newWinRect.iTl = ShadowRect().iTl + TPoint(iShadowTlWidth, iShadowTlHeight);
    //SetRect( newWinRect );
    }
    
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::SizeChanged
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindow::SizeChangedForBaseWindow( TInt aTotalColumns )
    {
    //only responsible for controls that layout window managed
    //if ( ( aTotalColumns != iLastUsedTotalColumns ) || iLangOrSizeChanged  )
        {
        TRect wndRect = Rect();
        TRect rtTitleBar = TitleBar()->Rect();
               
        TInt UnitWidth = *( static_cast<TInt*>( iLayoutContext->RequestData
            ( EPeninputDataTypeUnitWidth ) ) );
            
        TInt UnitHeight = *( static_cast<TInt*>( iLayoutContext->RequestData
            ( EPeninputDataTypeUnitHeight ) ) );
            
        TInt moveBtnColumns = 1;
        TInt closeBtnColumns = 1;
        TInt optionBtnColumns = 1;
        TInt headerPaneColumns;
        TInt icfColumns;
        
        //Call SetRect function of dragbutton
        if( iLayoutContext->LayoutType() == EPluginInputModeVkb)
            {
            moveBtnColumns = 2;
            closeBtnColumns = 2;
            }
        
        TRect rectClose ( rtTitleBar.iTl,  TSize( closeBtnColumns * UnitWidth, UnitHeight ) );
		TRect innerRect;
		AknPenImageUtils::CalculateGraphicRect( rectClose, innerRect );        
        CloseButton()->SizeChanged( rectClose, innerRect, ETrue );

        //Call SetRect function of Head pane
        headerPaneColumns = aTotalColumns - moveBtnColumns - closeBtnColumns;
        TRect rectHeader ( TPoint(rectClose.iBr.iX, rectClose.iTl.iY ), 
                           TSize( headerPaneColumns * UnitWidth, UnitHeight ) );
        HeaderPane()->SetRect( rectHeader );

        //Call SetRect function of input context field
        icfColumns = headerPaneColumns - optionBtnColumns;
        TRect rectIcf( rectHeader.iTl, 
                       TSize( icfColumns * UnitWidth, UnitHeight ) );
        if(ConfigInfo()->Language() == ELangArabic)        
            {
            rectIcf.iTl.iX = rectIcf.iTl.iX + UnitWidth;
            }        
        if(IsMultiLineIcf())
        	{
        	iMultiLineIcf->SetRect( rectIcf );	
        	}
        else
        	{
        	iInputContextField->SetRect( rectIcf );	
        	}
        if(ConfigInfo()->Language() == ELangArabic) 
            {
            TRect rectBackspace(rectHeader.iTl, TSize(UnitWidth, UnitHeight));
            AknPenImageUtils::CalculateGraphicRect( rectBackspace, innerRect );
            iBackspaceButton->SizeChanged(rectBackspace, innerRect, ETrue);
            }
        //set the rect of option button
        TRect rectOption ( TPoint( rectIcf.iBr.iX, rectIcf.iTl.iY ),
                           TSize( optionBtnColumns * UnitWidth, UnitHeight ) );
        AknPenImageUtils::CalculateGraphicRect( rectOption, innerRect ); 
        iTouchInputOptionButton->SizeChanged( rectOption, innerRect, ETrue );

        //Call SetRect function of close button
        TRect rectMove ( TPoint( rectHeader.iBr.iX, rectHeader.iTl.iY),
                         TSize( closeBtnColumns * UnitWidth, UnitHeight ) );
        AknPenImageUtils::CalculateGraphicRect( rectMove, innerRect ); 
        MoveButton()->SizeChanged( rectMove, innerRect, ETrue );

        SetControlsFont();            
        iLastUsedTotalColumns = aTotalColumns;                           
        }
    }
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::HandleControlEvent
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::HandleControlEvent( TInt aEventType,
    CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    // Call this function in base class
    CAknFepCtrlBaseWindow::HandleControlEvent( aEventType, 
        aCtrl, aEventData );
    
    TInt* data = ( TInt* ) aEventData.Ptr(); 
    
    switch ( aEventType )
        {
        case EPeninputLayoutEventSwitchLanguage:
            {
            TRAP_IGNORE( ChangeInputLanguageL( *data ) );                
            }
            break;
        case EPeninputLayoutEventRange: 
            {
            HandleRangeEvent( *data );
            }
            break;
        case EPeninputLayoutEventRangeLoop:
            {
            HandleRangeLoopEvent( *data );
            }
            break;
        case EPeninputLayoutEventShift: 
            {
            HandleShiftBtnClicked();
            }
            break;
        case EPeninputLayoutEventCapslock:
            {
            HandleCapslockBtnClicked();
            } 
            break;
        case EPeninputLayoutEventMovePosition:
            {
            TInt style = *( ( TUint16* ) aEventData.Ptr() );
            ChangeLayoutPosition( style );
            }
            break;
        case EPeninputLayoutEventSetPermittedRange:
            {
            if ( *data & ERangeNative )
                {
                *data = *data | ERangeAccent | ERangeNativeNumber 
                    | ERangeMixedText | ERangeMixedNumber;
                }
            else if(*data & ERangeEnglish)
                {
                *data = *data | ERangeAccent;
                }
            /*    
            if ( *data & ERangeNative )
                {
                *data = *data | ERangeNativeNumber 
                    | ERangeMixedText | ERangeMixedNumber;
                }
            */    
            // Update data:range
            iLayoutContext->SetData( EPeninputDataTypePermittedRange, data );
            
            // Set range of range bar component
            CFepUiBaseCtrl* bar = 
                iCtrlPool->Control( EPeninutWindowCtrlIdRangeBar );
            if ( bar ) 
                {
                CAknFepCtrlRangeBar* rangebar = 
                    static_cast<CAknFepCtrlRangeBar*>( bar );
                rangebar->SetPermittedRanges( *data );             
                }                     
            }
            break;  
        case EPeninputLayoutEventSetPermittedCase:
            {
            // Update data:case
            iLayoutContext->SetData( EPeninputDataTypePermittedCase, data );
            
            // Set range of case
            CFepUiBaseCtrl* bar = 
                iCtrlPool->Control( EPeninutWindowCtrlIdRangeBar );
            if ( bar ) 
                {
                CAknFepCtrlRangeBar* rangebar = 
                    static_cast<CAknFepCtrlRangeBar*>( bar );
            
                rangebar->SetPermittedCase( ERangeEnglish, ECaseUpper, 
                    ( *data & ECaseUpper) != 0 );
                rangebar->SetPermittedCase( ERangeEnglish, ECaseLower, 
                    ( *data & ECaseLower) != 0 );
                rangebar->SetPermittedCase( ERangeEnglish, ECaseText, 
                    ( *data & ECaseText) != 0 );
                }
            }
            break;
        case EPeninputLayoutEventSetCase:
            {
            DoCaseChange( *data );
            }
            break;
        case EEventSizeChanged:
            {
            //if icf editor size changed, we need to inform owner
            if(aCtrl == iInputContextField ||
               aCtrl == iMultiLineIcf )
                {
                iLayoutContext->Sendkey( ESignalLayoutICFLengthChanged, KNullDesC );
                }
            }
            break;
        default:
            break;
        }   
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::SetEditorTextL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::SetEditorTextL( 
    const TFepInputContextFieldData& aData )
    {
    if(IsMultiLineIcf())
    	{
    	if (iMultiLineIcf)
    		{
    	    iMultiLineIcf->SetTextL( aData );			
    		}
    	}
    else
    	{
    	if (iInputContextField)
    		{
    		iInputContextField->SetTextL( aData );
    		}
    		
    	}     
    }  

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::SetEditorTextL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::SetTextIsSecret( TBool aData )
    {
    if(IsMultiLineIcf())
    	{
    	if (iMultiLineIcf)
    		{
    	    iMultiLineIcf->SetTextIsSecret( aData );			
    		}
    	}
    else
    	{
    	if (iInputContextField)
    		{
    		iInputContextField->SetTextIsSecret( aData );
    		}
    	}    
    }  
    
void CPeninputLayoutWindow::SetNumberGrouping( TBool aData )
    {
    if(IsMultiLineIcf())
    	{
    	if (iMultiLineIcf)
    		{
    	    iMultiLineIcf->SetNumberGrouping( aData );			
    		}
    	}   
    } 
    

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::DoClose
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPeninputLayoutWindow::DoClose()
    {
    // Send EVkbEventWindowClose to layout
    this->ReportEvent( EPeninputLayoutEventClose, KNullDesC );
    
    // Return true which will permit the window close
    return EFalse;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::AddContextFieldL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindow::AddContextFieldL()
    {       
    // Get reference CControlGroup of headerPane
    CControlGroup* headerPane = HeaderPane();
    
    User::LeaveIfNull( headerPane );

    // Input context field
    iInputContextField = CFepInputContextField::NewL( TRect(), 
        UiLayout(), EPeninutWindowCtrlIdInputContextField );    
        
    // Transfer to headerPane
    headerPane->AddControlL( iInputContextField );
    iInputContextField->SetFocus(ETrue);
    
    // Add layout as event observer
    iInputContextField->AddEventObserver( UiLayout() );
    }
    
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::AddTouchInputButtonL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindow::AddTouchInputButtonL( TInt aResID )
    {
    // Get reference CControlGroup of headerPane
    CControlGroup* headerPane = HeaderPane();
    
    User::LeaveIfNull( headerPane );
        
    // Create Touch Input Option
    iTouchInputOptionButton = CAknFepCtrlEventButton::NewL( UiLayout(),
        EPeninutWindowCtrlIdOptionBtn, EPeninputLayoutEventOption, 0,
        									  KAknsIIDQsnFrFunctionButtonNormal,
        									  KAknsIIDQsnFrFunctionButtonPressed,
        									  KAknsIIDQsnFrFunctionButtonInactive );
        
    // Read resource
    TResourceReader reader;
    
    CCoeEnv::Static()->CreateResourceReaderLC( reader, aResID );    
    iTouchInputOptionButton->SetResourceId( aResID );    
    iTouchInputOptionButton->ConstructFromResourceL();  
    // Pop and destroy reader
    CleanupStack::PopAndDestroy( 1 );
    
    // Add into the group                                                   
    headerPane->AddControlL( iTouchInputOptionButton );
    iTouchInputOptionButton->AddEventObserver( UiLayout() );
    }

// ---------------------------------------------------------------------------
// Get layout config method. It does a lasy initialization for a instance of
// CPeninputLayoutWindow::ConfigInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutConfig* CPeninputLayoutWindow::ConfigInfo() const
    {
    return iConfigInfo;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::CheckResourceExist
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CPeninputLayoutWindow::CheckResourceExist( const TDesC& aFileName )
    {
    RFile file;
    
    if ( !aFileName.Length() )
        {
        return EFalse;
        }
        
    TInt rs = file.Open( CCoeEnv::Static()->FsSession(), aFileName, EFileRead );
    
    if ( rs == KErrNotFound )
        {
        return EFalse;  
        }
    else
        {
        file.Close();
        
        return ETrue;
        }
    }
  
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ChangeInputLanguageL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::ChangeInputLanguageL( TInt aLangID )
    {
    // Determine whether a new language id is same as existing one, 
    if ( ConfigInfo() )
        {
        // If language not changed, exit this function
        if ( ConfigInfo()->Language() == aLangID )
            {
            return;
            }
        }

    // First check file exist or not
    TBool found = CheckResourceExist( GetWindowConfigResFileName( aLangID ) );
    if ( !found )
        {
        aLangID = ELangEnglish;        
        found = CheckResourceExist( GetWindowConfigResFileName( aLangID ) );
        }
        
    if ( found ) 
        {
        // Store language
        iLayoutContext->SetData( EPeninputDataTypeInputLanguage, &aLangID );
   
        if ( iLayoutContext->LayoutType() == EPluginInputModeVkb )
            {       
            CFepUiBaseCtrl* ctrl = 
                iCtrlPool->Control( EPeninutWindowCtrlIdVkbCtrl );
            if ( ctrl )
                {
                ( static_cast<CPeninputVkbCtrlExt*>( ctrl ) )->Reset();
                }
            } 

         ( const_cast<RPointerArray<CFepUiBaseCtrl>&>
             ( ClientPane()->ControlList() ) ).Reset();

        // Re-construct config info 
        if ( iConfigInfo )
            {
            delete iConfigInfo;
            iConfigInfo = NULL;
            }
            
        if (iConfigResId)
            {
            CCoeEnv::Static()->DeleteResourceFile( iConfigResId );
            }
            
        // construct resource utils
        
        CCoeEnv* coeEnv = CCoeEnv::Static();
        
        // Set up resources
        TFileName resFileName = GetWindowConfigResFileName( aLangID );
        BaflUtils::NearestLanguageFile(coeEnv->FsSession(), resFileName );
        iConfigResId = coeEnv->AddResourceFileL( resFileName );
        
        TResourceReader reader;

        coeEnv->CreateResourceReaderLC( reader, GetWindowConfigResId() );  
        if ( iLayoutContext->LayoutType() == EPluginInputModeVkb )
            {
            iVkbLayout->Reset(); 
            }
        iConfigInfo = CPeninputLayoutConfig::NewL( *iVkbLayout, reader ); 
        
        // Pop and destroy reader
        CleanupStack::PopAndDestroy( 1 ); 
        	       
        // Re-construct rangebar
        ChangeRangeBarL();

        iLangOrSizeChanged = ETrue;
        //ChangeClientSize();
        
        TBuf<10> iISOCode;
        AknPenInputUtils::GetISOLanguageCode
            ( static_cast<TLanguage>(aLangID), iISOCode );
        CFepUiBaseCtrl* langBtn = 
            iCtrlPool->Control( EPeninutWindowCtrlIdInputLangSwitcherBtn );
            
        if ( langBtn )
            {
            ( static_cast<CAknFepCtrlEventButton*>( langBtn ) )->SetText
                ( iISOCode );
            UpdateArea(( static_cast<CAknFepCtrlButton*>( langBtn ) )->Rect(), 
                ETrue );
            }
	    TInt range = -1;
	    iLayoutContext->SetData( EPeninputDataTypeCurrentRange, &range );
	    iLayoutContext->SetData( EPeninputDataTypeClientLayout, &range );
	    
	    iMultiLineIcf->SetLanguageId( aLangID );
        } 
    else
        {
        // Report error, do nothing
        }
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ChangeRangeBarL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindow::ChangeRangeBarL()
    {
    // Read range bar from config into and reconstruct range button
    if ( iConfigInfo )
        {
        TInt resid = iConfigInfo->RangebarResId();
        CFepUiBaseCtrl* bar = 
            iCtrlPool->Control( EPeninutWindowCtrlIdRangeBar );
            
        if ( bar ) 
            {
            CAknFepCtrlRangeBar* rangebar = 
                static_cast<CAknFepCtrlRangeBar*>( bar );
            
            CAknFepCtrlRangeBar::TRangeBarActionStyle style = 
                ( CAknFepCtrlRangeBar::TRangeBarActionStyle )
                iConfigInfo->RangeBarInfo()->RangeStyle();
                
            rangebar->SetActionStyle( style );
                            
            TResourceReader reader;
            
            CCoeEnv::Static()->CreateResourceReaderLC( reader, resid ); 
            
            rangebar->ConstructFromResourceL( reader );
            
            // Pop and destroy reader
            CleanupStack::PopAndDestroy( 1 );
            }
        }
    }
  
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ChangeRange
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::ChangeRange( TInt aRange, TInt aVkbLayoutId, TBool aNotify )
    {
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );    


    CFepUiBaseCtrl* bar = 
        iCtrlPool->Control( EPeninutWindowCtrlIdRangeBar );
    CAknFepCtrlRangeBar* rangebar = 
        static_cast<CAknFepCtrlRangeBar*>( bar );
        
    if ( rangebar )
        {
        rangebar->ActiveRange( aRange );
        }
            
    if ( ( curRange != aRange ) || ( iLangOrSizeChanged ) )
        {
        // Change client layout
        //UiLayout()->LayoutOwner()->Hide(ETrue);
        CPeninputRangeBarInfo* rangeBar = iConfigInfo->RangeBarInfo();
        CPeninputRangeInfo* rangeInfo = rangeBar->FindRange( aRange );
        
        if ( !rangeInfo )
            {
            aRange = ERangeNumber;
            rangeInfo = rangeBar->FindRange( ERangeNumber );
            }

        // Get the whole columns    
        
        iUnitSizeChange = ReconfigUnitSize(rangeInfo->ClientLayoutId());
        /*if (iUnitSizeChange)
      	    {
       	    UiLayout()->LayoutOwner()->Hide(ETrue);
       	    }
       	    */
        ChangeClientLayout( rangeInfo->ClientLayoutId() );
        
        CPeninputClientLayoutInfo* clientLayout = 
            ConfigInfo()->FindClientLayoutInfo( rangeInfo->ClientLayoutId() );
        if ( clientLayout )
            {
            SizeChangedForBaseWindow( clientLayout->Columns() );
            }
        	
        // Update data:range
        iLayoutContext->SetData( EPeninputDataTypeCurrentRange, &aRange );
          
        // Notify fep the new range
        if ( aNotify )
            {
            TBuf<KIntLengthForByte> buf;
            CPeninputDataConverter::IntToDesc( aRange, buf );
            iLayoutContext->Sendkey( ESignalRange, buf );
            }

        // Change vkb layout
        if ( iLayoutContext->LayoutType() == EPluginInputModeVkb )
            {
            if ( ( aRange == ERangeEnglish ) || ( aRange == ERangeAccent ) 
                || ( ConfigInfo()->CaseSensitive() 
                && ( aRange == ERangeNative ) ) )
                {
                // Change shift and capslock button status according to the current case
                TInt curCase = CPeninputDataConverter::AnyToInt
                    ( iLayoutContext->RequestData( EPeninputDataTypeCase ) );
                TInt shfit;  
                TInt capslock;  
                
                CPeninputDataConverter::ShiftCapslockByCase( curCase, 
                    shfit, capslock );      

                iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shfit );
                ChangeButtonStatus( shfit, EPeninutWindowCtrlIdShiftBtn );
                iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &capslock );
                ChangeButtonStatus( capslock, EPeninutWindowCtrlIdCapslockBtn );                
                
                TInt caseid = CaseByShiftCapslockStatus();
                TInt vkblayoutid = 
                    ConfigInfo()->ShiftCapsSingleVkbLayoutId( aRange, caseid );
                                
                if ( aRange == ERangeAccent ) 
                    {
                    TInt curAccentIndex = CPeninputDataConverter::AnyToInt
                        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentAccent ) );
                    vkblayoutid = vkblayoutid + curAccentIndex * 2;
                    }      
                    
                // Change vkb layout
                ChangeVkbLayout( vkblayoutid );
                }
            else
                {
                TInt flag = 0;
                
                iLayoutContext->SetData( EPeninputDataTypeShiftDown, &flag );
                ChangeButtonStatus( 0, EPeninutWindowCtrlIdShiftBtn );
                iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &flag );
                ChangeButtonStatus( 0, EPeninutWindowCtrlIdCapslockBtn );
     
                // Change vkb layout
                if ( aVkbLayoutId )
                    {
                    ChangeVkbLayout( aVkbLayoutId );                   
                    }
                else
                    {
                    ChangeVkbLayout( rangeInfo->VkbLayoutId() );  
                    }                
                }

            }
            
        /*if (iUnitSizeChange)
            {
        	//SizeChanged(ETrue);
        	const TRect rect = ChangeUnitSize();
            ReadLafForShadow( rect );
            if ( iConfigInfo )
                {
                // Change the size of base window
                SizeChangedForBaseWindow( iLastUsedTotalColumns );
                ChangeClientSize();        
                }
        	UiLayout()->LayoutOwner()->Hide(EFalse);
        	iUnitSizeChange = EFalse;
            }*/

        if ( ( iLayoutContext->LayoutType() == EPluginInputModeVkb )
              && ( curRange == ERangeNumber ) && ( curRange == aRange ) )
            {
            // Dimmed some keys if needed  
            DimKeys();                 
            } 
                           
        UiLayout()->Draw();    
        }
        
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ChangeClientLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::ChangeClientLayout( TInt aClientLayoutId )
    {
    TInt curClientId = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeClientLayout ) );

    if ( ( curClientId != aClientLayoutId ) || ( iLangOrSizeChanged ) )
        {
        // Re-Orginize all controls according to the configuration
        ReorganizeControls( aClientLayoutId, ETrue );
      
        // Update data:client layout id
        iLayoutContext->SetData( EPeninputDataTypeClientLayout, 
                                 &aClientLayoutId );
                                 
        // Reset lang changed flag
        if ( ( iLangOrSizeChanged ) 
            && ( iLayoutContext->LayoutType() != EPluginInputModeVkb ) )
            {
            iLangOrSizeChanged = EFalse;
            }

        //iUnitSizeChange = ReconfigUnitSize(aClientLayoutId); 
        }
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ChangeVkbLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::ChangeVkbLayout( TInt aVkbLayoutId )
    {
    TInt latchedFlag = CPeninputDataConverter::AnyToInt(
                       iLayoutContext->RequestData(EAkninputDataTypeLatchedSet));
    // If the DeadKey is latched, cancel it and then change the VKB layout
    if(latchedFlag)
        {
        RPointerArray<CPeninputVkbLayoutInfo> vkbListInfo;
        RPointerArray<CPeninputVkbKeyInfo> keyInfoList;
        
        vkbListInfo = iVkbLayout->VkbLayoutInfoList();
        TInt vkbListNum = vkbListInfo.Count();
        
        CVirtualKey* pKey;
        TBool deadKeyChange = EFalse;
        // Find the latched DeadKey in all the Vkb layout
        for(TInt i = 0; i < vkbListNum; i++)
            {
            // Get key info list in one VKB layout
            keyInfoList = vkbListInfo[i]->KeyInfoList();
            TInt keyListNum = keyInfoList.Count();
            for(TInt j = 0; j < keyListNum; j++)
                {
                pKey = keyInfoList[j]->Key();
                if(pKey->Latched())
                    {
                    // Unlatch the DeadKey
                    pKey->SetLatched(EFalse);
                    
                    // Set the DeadKey state
                    iLayoutContext->SetData(EAkninputDataTypeLatchedSet, &deadKeyChange);
                    deadKeyChange = ETrue;
                    break;
                    } 
                }
            if(deadKeyChange)
                {
                break;
                }
            }
        }
    
    TInt curVkbId = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeVkbLayout ) );
                           
    if ( ( curVkbId != aVkbLayoutId ) || ( iLangOrSizeChanged ) )
        {
        iVkbLayout->ChangeVkbLayout( aVkbLayoutId );
        
        TInt curClientId = CPeninputDataConverter::AnyToInt 
           ( iLayoutContext->RequestData( EPeninputDataTypeClientLayout ) );
        CPeninputControlInfo* vkbInfo = iConfigInfo->FindClientLayoutInfo 
            ( curClientId )->FindControlInfo( EPeninutWindowCtrlIdVkbCtrl );
    
        TRect rect;
        CPeninputVkbCtrlExt* vkbCtrl = static_cast<CPeninputVkbCtrlExt*>
            ( ClientPane()->ControlById( EPeninutWindowCtrlIdVkbCtrl ) );
      
        if ( vkbInfo )
            {
            TInt unitWidth = CPeninputDataConverter::AnyToInt
                ( iLayoutContext->RequestData( EPeninputDataTypeUnitWidth ) );
                              
            TInt unitHeight = CPeninputDataConverter::AnyToInt 
                ( iLayoutContext->RequestData
                ( EPeninputDataTypeUnitHeight ) );
                                  
            rect.iTl = vkbCtrl->Rect().iTl + TPoint( vkbInfo->BeginColumn() 
                * unitWidth, vkbInfo->BeginRow() * unitHeight );
            TInt cols = vkbInfo->EndColumn() - vkbInfo->BeginColumn();
            TInt rows = vkbInfo->EndRow() - vkbInfo->BeginRow();
            rect.SetSize( TSize( cols * unitWidth, rows * unitHeight ) );
            }
          
        CPeninputVkbLayoutInfo* vkblayout = 
            ConfigInfo()->FindVkbLayoutInfo( aVkbLayoutId );  
      
        if ( vkbCtrl && vkblayout )
            {
            vkbCtrl->SetRect( rect );
            DimKeys(); 
            
            // Make the true draw
            UpdateArea( vkbCtrl->Rect(), EFalse ); 
            }     
        
        // Update data : vkb layout id
        iLayoutContext->SetData( EPeninputDataTypeVkbLayout, &aVkbLayoutId );
        
        // Reset lang changed flag
        if ( iLangOrSizeChanged )
            {
            iLangOrSizeChanged = EFalse;
            }        
        }        
      
    }

// --------------------------------------------------------------------------
// CPeninputLayoutWindow::HandleRangeEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::HandleRangeEvent( TInt aRange )
    {                                 
    TInt curRange = CPeninputDataConverter::AnyToInt 
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );

    if (!IsCanChangeRange(aRange))
    	{
        ChangeToPreviousRange(curRange);
        return;    		
    	}
  
    CPeninputRangeBarInfo* rangeBar = iConfigInfo->RangeBarInfo();
    if ( !rangeBar )
        {
        return;
        }

    CPeninputRangeInfo* rangeInfo = rangeBar->FindRange( aRange );
    
    if ( !rangeInfo )
        {
        return;
        }
      
    TInt responseStyle = rangeInfo->ResponseStyle();
    if ( responseStyle == EPeninputRangeResponseNoAction )
        {
        return; 
        }
    
    if ( curRange != aRange )
        {
        if ( responseStyle == EPeninputRangeResponseSwitchRange )
            {    
            TInt index = KInvalidIndex;
            
            if ( aRange == ERangeAccent )
                {
                index = 0;
                }
            iLayoutContext->SetData( EPeninputDataTypeCurrentAccent, &index );
            
            ChangeRange( aRange );     
            
            // When change range mannually, make the entire window visible
            TInt style = EPeninputPositionChangeBrJustify;
            TBuf<KIntSizeToInt16> bufStyle;
            bufStyle = ( TUint16* )&style;
            HandleControlEvent(EPeninputLayoutEventMovePosition, NULL, bufStyle);           
            }
        else if ( responseStyle == EPeninputRangeResponsePopupChoiceItem )
            {
            PopupChoiceList();
            }
        }
    }

// --------------------------------------------------------------------------
// CPeninputLayoutWindow::HandleRangeLoopEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::HandleRangeLoopEvent( TInt aLoop )
    {
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
                
    CFepUiBaseCtrl* ctrl = iCtrlPool->Control( EPeninutWindowCtrlIdRangeBar );
    CAknFepCtrlRangeBar* rangebar = static_cast<CAknFepCtrlRangeBar*>( ctrl );
  
    if ( rangebar )
        {
        TRAP_IGNORE( rangebar->SetCaseL( curRange, aLoop ) );
      
        if ( ( curRange == ERangeEnglish ) || ( ConfigInfo()->CaseSensitive()
            && ( curRange == ERangeNative ) ) )
            {
            TInt curCase = rangebar->GetCase( curRange );
            
            

            // Store case in the data manager, the engine will 
            // get notice at the same time
            iLayoutContext->SetData( EPeninputDataTypeCase, &curCase );
          
            // Notify fep the new case
            TBuf<KIntLengthForByte> buf;
            CPeninputDataConverter::IntToDesc( curCase, buf );
            iLayoutContext->Sendkey( ESignalCaseMode, buf );
            }    
        }
    }

// --------------------------------------------------------------------------
// CPeninputLayoutWindow::CaseByShiftCapslockStatus
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C TInt CPeninputLayoutWindow::CaseByShiftCapslockStatus()
    {
    TInt shiftStatus = CPeninputDataConverter::AnyToInt 
        ( iLayoutContext->RequestData( EPeninputDataTypeShiftDown ) );
    TInt capslockStatus = CPeninputDataConverter::AnyToInt 
        ( iLayoutContext->RequestData( EPeninputDataTypeCapslockDown ) );
        
    return ( capslockStatus * 2 + shiftStatus );
    }
    
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::HandleShiftBtnClicked
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::HandleShiftBtnClicked()
    {
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
    TBool flag = ( curRange == ERangeEnglish )  || ( curRange == ERangeAccent ) 
        || ( ConfigInfo()->CaseSensitive() && ( curRange == ERangeNative ) );
        
    if ( flag && !( iLayoutContext->IsShiftPermitted() ) )
        {
        ChangeButtonStatus( 0, EPeninutWindowCtrlIdShiftBtn );
        return;  
        }                 

    TInt shiftStatus = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeShiftDown ) );
         
    if ( shiftStatus )
        {
        shiftStatus = 0;
        }
    else
        {
        shiftStatus = 1;
        }

    // Update data
    iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shiftStatus );
    ChangeButtonStatus( shiftStatus, EPeninutWindowCtrlIdShiftBtn );
    
    // Update vkb layout
    TInt  caseid = CaseByShiftCapslockStatus();
    
    TInt vkbLayout = iConfigInfo->ShiftCapsSingleVkbLayoutId( curRange, 
                                                              caseid );
                                               
    TInt responseStyle = 
        iConfigInfo->RangeBarInfo()->FindRange( curRange )->ResponseStyle();
    if ( ( curRange == ERangeAccent ) 
        && ( responseStyle == EPeninputRangeResponsePopupChoiceItem ) )
        {
        TInt curAccentIndex = CPeninputDataConverter::AnyToInt
            ( iLayoutContext->RequestData( EPeninputDataTypeCurrentAccent ) );
        vkbLayout = vkbLayout + curAccentIndex * 2;
        }
        
    ChangeVkbLayout( vkbLayout );
    
    // Synchronize case if needed    
    if ( flag )
        {
        // Notify fep the new case
        TBuf<KIntLengthForByte> buf;
        TInt fepcase = CPeninputDataConverter::FepCaseByCaseId( caseid );
        iLayoutContext->SetData( EPeninputDataTypeCase, &fepcase );
        CPeninputDataConverter::IntToDesc( fepcase, buf );
        iLayoutContext->Sendkey( ESignalCaseMode, buf );
        }
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::HandleCapslockBtnClicked
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::HandleCapslockBtnClicked()
    {
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
    TBool flag = ( curRange == ERangeEnglish )  || ( curRange == ERangeAccent ) 
        || ( ConfigInfo()->CaseSensitive() && ( curRange == ERangeNative ) );
        
    if ( flag && !( iLayoutContext->IsCapsLockPermitted() ) )
        {
        ChangeButtonStatus( 0, EPeninutWindowCtrlIdCapslockBtn );
        return;  
        } 
    
    TInt capslockStatus = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCapslockDown ) );
    TInt shiftStatus = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeShiftDown ) );
        
    if ( capslockStatus )  
        {
        capslockStatus = 0;
        }
    else
        {
        capslockStatus = 1;
        }
    
    if( shiftStatus )
    {
    shiftStatus = 0;
    iLayoutContext->SetData( EPeninputDataTypeShiftDown, 
         &shiftStatus );                          
    ChangeButtonStatus( shiftStatus, EPeninutWindowCtrlIdShiftBtn );
    }
    
    // Update data
    iLayoutContext->SetData( EPeninputDataTypeCapslockDown, 
                             &capslockStatus );                          
    ChangeButtonStatus( capslockStatus, EPeninutWindowCtrlIdCapslockBtn );
            
    // Update vkb layout
    TInt  caseid = CaseByShiftCapslockStatus(); 
    TInt vkbLayout = iConfigInfo->ShiftCapsSingleVkbLayoutId( curRange, 
                                                              caseid );
                                             
    TInt responseStyle = 
        iConfigInfo->RangeBarInfo()->FindRange( curRange )->ResponseStyle();
    if ( ( curRange == ERangeAccent ) 
        && ( responseStyle == EPeninputRangeResponsePopupChoiceItem ) )
        {
        TInt curAccentIndex = CPeninputDataConverter::AnyToInt
            ( iLayoutContext->RequestData( EPeninputDataTypeCurrentAccent ) );
        vkbLayout = vkbLayout + curAccentIndex * 2;
        }
            
    ChangeVkbLayout( vkbLayout );
    
    // Synchronize case if needed
    if ( flag )
        {
        // Notify fep the new case
        TBuf<KIntLengthForByte> buf;
        TInt fepcase = CPeninputDataConverter::FepCaseByCaseId( caseid );
        iLayoutContext->SetData( EPeninputDataTypeCase, &fepcase );
        CPeninputDataConverter::IntToDesc( fepcase, buf );
        iLayoutContext->Sendkey( ESignalCaseMode, buf );
        }
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::DimKeys
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindow::DimKeys()
    {
    CPeninputVkbCtrlExt* vkbCtrl = static_cast<CPeninputVkbCtrlExt*>
        ( ClientPane()->ControlById( EPeninutWindowCtrlIdVkbCtrl ) );
    vkbCtrl->CancelDims();
    TInt currentRange = CPeninputDataConverter::AnyToInt 
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
    TInt permitRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypePermittedRange ) ); 
    TInt keyMapping = CPeninputDataConverter::AnyToInt 
        ( iLayoutContext->RequestData( EPeninputDataTypeNumericKeymap ) );
         
    if ( ( currentRange == ERangeNumber )
        && ( permitRange == ERangeNumber ) )
        {
        HBufC* rs = NULL;
        TRAP_IGNORE(rs = GetKeyMappingStringL(keyMapping));
        if( rs )
            {
            vkbCtrl->DimKeys( rs );
            delete rs;
            }
        }
    vkbCtrl->Draw();
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::AddControlL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::AddNotOwnedControl( 
    CFepUiBaseCtrl* aControl )
    {
    //do nothing if we already has the control.
    if ( KErrNotFound != ClientPane()->ControlList().Find( aControl ) )
        {
        return;
        }
    
    if ( aControl )
        {
        ( const_cast<RPointerArray<CFepUiBaseCtrl>&>
            ( ClientPane()->ControlList() ) ).Append( aControl );
        aControl->SetParent( ClientPane() );
        RRegion region( KPeninputLayoutWindowRegionCount );
        region.Copy( ClientPane()->Region() );   

        region.AddRect( aControl->Rect() );
        ClientPane()->SetRegion( region );
        }
    } 
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ChangeButtonStatus
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::ChangeButtonStatus( const TInt aIsDown, 
    const TInt aControlId )
    {
    CAknFepCtrlEventButton* button = 
        static_cast<CAknFepCtrlEventButton*>
        ( ClientPane()->ControlById( aControlId ) );

    if ( button )
        {
        button->SetHighlight( aIsDown == 0 ? EFalse : ETrue );
        } 
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ChangeLayoutPosition
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//               
EXPORT_C void CPeninputLayoutWindow::ChangeLayoutPosition( TInt aJustifyStyle )
    {        
    switch ( aJustifyStyle )
        {
        case EPeninputPositionChangeBrJustify:
            {
            BottomRightJustify();
            }         
            break;
        case EPeninputPositionChangeDataQuery:
            {
            BottomRightJustify();
            DataQueryJustify();
            }
            break;
        /*case EPeninputPositionChangeNone:
            break;*/
        default:
            {
            TopLeftJustify();
            }
            break;
        }
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::SetWindowRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//      
EXPORT_C void CPeninputLayoutWindow::SetWindowRect( const TRect& aRect )
    {
    TRect rect = aRect;


    TRect shadowRect = TRect( TPoint(), 
                              TSize( aRect.Width() + iShadowBrWidth + iShadowTlWidth,
                                     aRect.Height() + iShadowTlHeight + iShadowBrHeight)
                            );
                            
    rect.Move( TPoint(iShadowTlWidth, iShadowTlHeight) - rect.iTl );
    
    SetRect( rect );  
    RootControl()->SetRect( rect );
    UiLayout()->SetShadowRect( shadowRect );  	        
    }


// -----------------------------------------------------------------------------
// CPeninputLayoutWindow::SetSwitchBtnFont
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindow::SetSwitchBtnFont(CAknFepCtrlEventButton& aSwitchBtn)
    {
    // read switch font laf info only when need
    if (CPeninputDataConverter::AnyToInt(
        iLayoutContext->RequestData(EAkninputDataTypeSizeChanging)) || (!iSwitchFontSet))
    	{
        TAknTextLineLayout langSwitch;
        TAknTextLineLayout langSwitchShadow;

        switch( iLayoutContext->LayoutType() )
            {
            case EPluginInputModeVkb:
                {
                langSwitch = 
                    AknLayoutScalable_Avkon::cell_vkb_side_pane_t1().LayoutLine();
                langSwitchShadow = 
                    AknLayoutScalable_Avkon::cell_vkb_side_pane_t1_copy1().LayoutLine();
                }
                break;
            case EPluginInputModeHwr:
                {
                langSwitch = 
                    AknLayoutScalable_Avkon::cell_hwr_side_pane_t1().LayoutLine();
                langSwitchShadow = 
                    AknLayoutScalable_Avkon::cell_hwr_side_pane_t1_copy1().LayoutLine();
                }
                break;
            default:
                {
                return;
                }
            }
        
        iSwitchFont = langSwitch.iFont;
        iSwitchShadowFont = langSwitchShadow.iFont;
    	}
    
    aSwitchBtn.SetFont(AknLayoutUtils::FontFromId(iSwitchFont, NULL));
    aSwitchBtn.SetShadowFont(AknLayoutUtils::FontFromId(iSwitchShadowFont, NULL));
    iSwitchFontSet = ETrue;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::TopLeftJustify
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
EXPORT_C void CPeninputLayoutWindow::SetSwitchBtnTextColor(CAknFepCtrlEventButton& aSwitchBtn)
    {
    TRgb textColor;
    TInt error = AknsUtils::GetCachedColor(AknsUtils::SkinInstance(),
                                           textColor,
                                           KAknsIIDQsnTextColors,
                                           EAknsCIQsnTextColorsCG59);

    if (error != KErrNone)
        {
    	textColor = TRgb(KDefaultTextColor);
        }
        
    aSwitchBtn.SetFontColor( textColor );   
    
    TRgb shadowTextColor;    
    error = AknsUtils::GetCachedColor(AknsUtils::SkinInstance(),
                                      shadowTextColor,
                                      KAknsIIDQsnTextColors,
                                      EAknsCIQsnTextColorsCG62);
                                      
    if (error != KErrNone)
        {
    	shadowTextColor = TRgb(KDefaultShadowTextColor);
        }
    
    aSwitchBtn.SetShadowFontColor( shadowTextColor );
    }
    
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::TopLeftJustify
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
void CPeninputLayoutWindow::TopLeftJustify()
    {
    // Read position from datamgr 
    TSize layoutSize = UiLayout()->Rect().Size();
    
    TPoint tl = UiLayout()->Position();
    
    TPoint oldBr = tl + layoutSize;
    
    if ( tl.iX < 0 )
        {
        tl.iX = 0;
        }
    
    if ( tl.iY < 0 )
        {
        tl.iY = 0;  
        }
        
    TPoint br = tl + layoutSize;          

    if ( br != oldBr )
        {
        //Write the new position into CenRep
        TInt x = 0;
        TInt y = 0;
        x = 0x0000FFFF & br.iX;
        y = 0xFFFF0000 & ( br.iY << 16 );
        TInt pos = x | y;
        iLayoutContext->SetData( EPeninputDataTypeWindowPosition, &pos );
        }
        
    //Set the layout position
    //UiLayout()->SetRect( newRect );
    UiLayout()->LayoutOwner()->SetPosition( tl );
    //UiLayout()->LayoutOwner()->LayoutSizeChanged( newRect.Size() );
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::BottomRightJustify
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CPeninputLayoutWindow::BottomRightJustify()
    {
    // Read position from datamgr 
    TSize layoutSize( UiLayout()->Rect().Size() );
    TPoint oldbr;
    TPoint br;
    TPoint tl;
    TSize screenSize = *( ( TSize* ) iLayoutContext->RequestData
                            ( EPeninputDataTypeScreenSize ) );
    
    TRect screenRect = TRect( TPoint( 0, 0 ), screenSize );

    TInt windowPosition = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeWindowPosition ) );

    oldbr.iX = windowPosition & 0x0000FFFF;
    oldbr.iY = ( windowPosition & 0xFFFF0000 ) >> 16;
    
    br = oldbr;
        
    if ( !screenRect.Contains( br ) )
        {
        if ( screenSize.iWidth < br.iX )
            {
            br.iX = screenSize.iWidth;
            }
        if ( screenSize.iHeight < br.iY )
            {
            br.iY = screenSize.iHeight;
            }
        }
    
    // Check the top left point is in screen
    tl.iX = br.iX - layoutSize.iWidth;
    if ( tl.iX < 0 )
        {
        tl.iX = 0;
        br.iX = layoutSize.iWidth;
        }

    tl.iY = br.iY - layoutSize.iHeight;        
    if ( tl.iY < 0 )
        {
        tl.iY = 0; 
        br.iY = layoutSize.iHeight; 
        }
        
    
    if ( oldbr != br )
        {
        //Write the new position into CenRep
        TInt x = 0;
        TInt y = 0;
        x = 0x0000FFFF & br.iX;
        y = 0xFFFF0000 & ( br.iY << 16 );
        TInt pos = x | y;
        iLayoutContext->SetData( EPeninputDataTypeWindowPosition, &pos );
        }
        
    //Set the layout position
    //UiLayout()->SetRect( newRect );
    //UiLayout()->LayoutOwner()->LayoutSizeChanged( newRect.Size() );
    UiLayout()->LayoutOwner()->SetPosition( tl );
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::DataQueryJustify
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
void CPeninputLayoutWindow::DataQueryJustify()
    {
    TRect* dataQueryRect = ( TRect* )( iLayoutContext->RequestData
        ( EPeninputDataTypeDataQueryDlgRect) );
    
    TRect rect = TRect( UiLayout()->Position(), UiLayout()->Rect().Size() );
    
    //move the VKB to the bottom-right corner 
    TSize scrsz = UiLayout()->ScreenSize();
    
    if( scrsz.iWidth < scrsz.iHeight )
        {
        return;
        }
    rect = TRect( TPoint( scrsz.iWidth - rect.Width(), scrsz.iHeight - rect.Height() ),
                  rect.Size() );
    
    //Write the new position into CenRep
    TInt x = 0;
    TInt y = 0;
    x = 0x0000FFFF & rect.iBr.iX;
    y = 0xFFFF0000 & ( rect.iBr.iY << 16 );
    TInt pos = x | y;
    iLayoutContext->SetData( EPeninputDataTypeWindowPosition, &pos );
    
    /*if ( rect.Intersects( *dataQueryRect ) )
        {
        TInt Y = dataQueryRect->iTl.iY;
        TInt height = rect.iBr.iY - rect.iTl.iY;
        rect.iTl.iY = Y - height;
        rect.iBr.iY = Y;
        
        if( rect.iTl.iY <0 )
            {
            rect.iTl.iY = 0;
            rect.iBr.iY = height;
            }
        //Write the new position into CenRep
        TInt x = 0;
        TInt y = 0;
        x = 0x0000FFFF & rect.iBr.iX;
        y = 0xFFFF0000 & ( rect.iBr.iY << 16 );
        TInt pos = x | y;
        iLayoutContext->SetData( EPeninputDataTypeWindowPosition, &pos );
        }
        */
    //Set the layout position
    UiLayout()->SetRect( rect );
    UiLayout()->LayoutOwner()->SetPosition( rect.iTl );
    UiLayout()->LayoutOwner()->LayoutSizeChanged( rect.Size() );
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ReadShadowInfoL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CPeninputLayoutWindow::ReadShadowInfoL( TResourceReader aResReader )
    {
    TPtrC bmpFileName = aResReader.ReadTPtrC();
    TInt32 imgMajorSkinId = aResReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();

    for ( TInt i = 0; i < KShadowBmpBr + 1; i++ )
    	{
        const TInt16 bmpId = aResReader.ReadInt16();
        const TInt16 bmpMskId = aResReader.ReadInt16();
        skinitemid = aResReader.ReadInt16();
        
        id.Set(TInt(imgMajorSkinId), skinitemid);
    	
        if (bmpId != KInvalidImg)
        	{
            CFbsBitmap* shadowImg = NULL;
            CFbsBitmap* shadowMaskImg = NULL;

            if (bmpMskId != KInvalidImg)
                {
                AknsUtils::CreateIconL(skininstance, 
                                       id,
                                       shadowImg,
                                       shadowMaskImg,
                                       bmpFileName,
                                       bmpId,
                                       bmpMskId);
                }
            else
                {
                AknsUtils::CreateIconL(skininstance,
                                       id,
                                       shadowImg,
                                       bmpFileName,
                                       bmpId);
                }
            
            UiLayout()->SetShadowBmp(shadowImg, shadowMaskImg, TShadowBitmapIndex(i));
        	}
    	}
    }
    
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ReadBackgroundInfoL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
void CPeninputLayoutWindow::ReadBackgroundInfoL( TInt aResId )
    {
    if( aResId == 0 )
    	return;
    
    if( BackgroundBmp() )
		{
		CFbsBitmap* bkBmp = BackgroundBmp();
		delete bkBmp;	
		SetBackgroundBmp( NULL );
		}
    if( BkMaskBmp() )
    	{
    	CFbsBitmap* bkMaskBmp = BkMaskBmp();
    	delete bkMaskBmp;
		SetBackgroundMaskBmp( NULL );
    	}    

    TResourceReader resReader;
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, aResId ); 
     
    TPtrC bmpFileName = resReader.ReadTPtrC();
    TInt32 imgMajorSkinId = resReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();
    const TInt16 bmpId = resReader.ReadInt16();
    const TInt16 bmpMskId = resReader.ReadInt16();
    skinitemid = resReader.ReadInt16();   
    id.Set(TInt(imgMajorSkinId), skinitemid);
	
    if (bmpId != KInvalidImg)
    	{
    	CFbsBitmap* backgroundImg = NULL;
    	CFbsBitmap* backgroundMaskImg = NULL;
        if (bmpMskId != KInvalidImg)
            {
            AknsUtils::CreateIconL(skininstance, 
                                   id,
                                   backgroundImg,
                                   backgroundMaskImg,
                                   bmpFileName,
                                   bmpId,
                                   bmpMskId);
            }
        else
            {
            AknsUtils::CreateIconL(skininstance,
                                   id,
                                   backgroundImg,
                                   bmpFileName,
                                   bmpId);
            }
       
        SetBackgroundBmp( backgroundImg );
        SetBackgroundMaskBmp( backgroundMaskImg );           
    	}
    CleanupStack::PopAndDestroy( 1 );
    }
    
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ReadIcfInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPeninputLayoutWindow::ReadIcfInfo( TResourceReader aResReader )
    {
    TPtrC bmpFileName = aResReader.ReadTPtrC();  
    TInt32 imgMajorSkinId = aResReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();

    const TInt16 icfbgId = aResReader.ReadInt16();
    const TInt16 icfbgmaskId = aResReader.ReadInt16();
    skinitemid = aResReader.ReadInt16();

    id.Set( TInt( imgMajorSkinId ), skinitemid );
    
    if ( icfbgId != KInvalidImg )
    	{
        CFbsBitmap* icfImg = NULL;

        if (icfbgmaskId != KInvalidImg)
            {
            CFbsBitmap* icfmaskImg = NULL;
            
            TRAP_IGNORE( AknsUtils::CreateIconL(skininstance,
                                   id,
                                   icfImg,
                                   icfmaskImg,
                                   bmpFileName,
                                   icfbgId,
                                   icfbgmaskId ) );
            
            AknIconUtils::SetSize(icfmaskImg, TSize(1,1), EAspectRatioNotPreserved);
            TRAP_IGNORE( iInputContextField->SetBackgroundMaskBitmapL(icfmaskImg) );
            }
        else
            {
            TRAP_IGNORE( AknsUtils::CreateIconL(skininstance,
                                   id,
                                   icfImg,
                                   bmpFileName,
                                   icfbgId) );
            }
    	
    	AknIconUtils::SetSize(icfImg, TSize(1,1), EAspectRatioNotPreserved);
    	TRAP_IGNORE( iInputContextField->SetBackgroundBitmapL(icfImg) );
    	}

    // read text color
    TRgb textcolor;
    TInt32 colorMajorSkinId = aResReader.ReadInt32();
    skinitemid = aResReader.ReadInt16();
    
    id.Set(TInt(colorMajorSkinId), skinitemid);

    const TInt16 textcoloridx = aResReader.ReadInt16();
    
    TInt error = AknsUtils::GetCachedColor(skininstance, 
                                           textcolor,
                                           id,
                                           textcoloridx);

    if (error == KErrNone)
        {
        iInputContextField->SetTextColor( textcolor );
        }
    else
        {
        iInputContextField->SetTextColor( TRgb( KDefaultTextColor ) );
        }    
    }
          
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::Draw
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPeninputLayoutWindow::Draw()
    {    
    // Draw shadow
    CAknFepCtrlBaseWindow::Draw();

    //DrawShadow();   
    
    UpdateArea(UiLayout()->Rect(), EFalse);   
    }
       
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::GetKeyMappingStringL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
HBufC* CPeninputLayoutWindow::GetKeyMappingStringL(TInt aKeyMapping)
    {
    HBufC* rs = NULL;
    if ( aKeyMapping == EKeymapFromResource )
        {
        HBufC* customRes = (HBufC*) iLayoutContext->RequestData(EAknFepDataTypeUseDefinedResource);
        _LIT(KTenNumbers,"0123456789");
        rs = HBufC::NewL(KTenNumbers().Length() + customRes->Length());
        rs->Des().Copy(*customRes);
        rs->Des().Append(KTenNumbers);
        }
    else
        {
        rs = iKeyMappingMgr->KeyMappingStringL( aKeyMapping );
        }
        
    return rs;     
    }
       
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::SetEnableSwitchToHwr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
EXPORT_C void CPeninputLayoutWindow::SetEnableSwitchToHwr(TBool aEnable)
    {
    CAknFepCtrlEventButton* switchToHwrBtn = static_cast<CAknFepCtrlEventButton*>
        ( iCtrlPool->Control( EPeninutWindowCtrlIdSwitchToHwrBtn ) );

    if ( aEnable )    
        {
        switchToHwrBtn->SetDimmed(EFalse);
        }
    else
        {
        switchToHwrBtn->SetDimmed(ETrue);
        switchToHwrBtn->SetActive(EFalse);
        } 
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::SetEnableSettingBtn
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
EXPORT_C void CPeninputLayoutWindow::SetEnableSettingBtn(TBool aEnable)
    {
    CAknFepCtrlEventButton* langSetBtn = static_cast<CAknFepCtrlEventButton*>
        ( iCtrlPool->Control( EPeninutWindowCtrlIdInputLangSwitcherBtn ) );
        
    if ( aEnable )    
        {
        langSetBtn->SetDimmed(EFalse);
        iTouchInputOptionButton->SetDimmed(EFalse);
        }
    else
        {
        langSetBtn->SetDimmed(ETrue);
        langSetBtn->SetActive(EFalse);
        iTouchInputOptionButton->SetDimmed(ETrue);
        iTouchInputOptionButton->SetActive(EFalse);
        } 
    }
        
// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ReconfigUnitSize
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
EXPORT_C TBool CPeninputLayoutWindow::ReconfigUnitSize(TInt /*aClientLayoutId*/)
    {
    return EFalse;
    }

void CPeninputLayoutWindow::AddMultiContextFieldL()
	{
    const CFont* icfFont = NULL;
    TAknTextLineLayout textPaneTextLayout;

    textPaneTextLayout = 
    AknLayoutScalable_Avkon::fep_vkb_top_text_pane_t1().LayoutLine();
                
    icfFont = AknLayoutUtils::FontFromId( textPaneTextLayout.iFont, NULL );    
    if ( icfFont )
        {
        iMultiLineIcf = CFepLayoutMultiLineIcf::NewL( TRect(), 
            UiLayout(), EPeninputWindowCtrlIdMultiLineICF, 
            icfFont->HeightInPixels(),
            icfFont->FontMaxHeight(),icfFont );    
        
        HeaderPane()->AddControlL( iMultiLineIcf );

        // Add layout as event observer
        iMultiLineIcf->AddEventObserver( UiLayout() );
        iMultiLineIcf->SetFocus(ETrue);
        }   		
	}

EXPORT_C TBool CPeninputLayoutWindow::IsMultiLineIcf()
	{
	return EFalse;	
	}
	
	
void CPeninputLayoutWindow::ReadMultiLineIcfInforL(TInt aResID)
    {
    if (!aResID)
        {
        return;
        }

    TResourceReader resReader;
    CCoeEnv::Static()->CreateResourceReaderLC( resReader, aResID );
    
    TPtrC bmpFileName = resReader.ReadTPtrC();  
    TInt32 imgMajorSkinId = resReader.ReadInt32();

    TAknsItemID id;
    TInt skinitemid;
    
    MAknsSkinInstance* skininstance = AknsUtils::SkinInstance();

    const TInt16 icfbgId = resReader.ReadInt16();
    const TInt16 icfbgmaskId = resReader.ReadInt16();
    skinitemid = resReader.ReadInt16();

    id.Set( TInt( imgMajorSkinId ), skinitemid );
    
    if ( icfbgId != KInvalidImg )
    	{
        CFbsBitmap* icfImg = NULL;

        if (icfbgmaskId != KInvalidImg)
            {
            CFbsBitmap* icfmaskImg = NULL;
            
            TRAP_IGNORE( AknsUtils::CreateIconL(skininstance,
                                   id,
                                   icfImg,
                                   icfmaskImg,
                                   bmpFileName,
                                   icfbgId,
                                   icfbgmaskId ) );
            
            AknIconUtils::SetSize(icfmaskImg, TSize(1,1), EAspectRatioNotPreserved);
            TRAP_IGNORE( iMultiLineIcf->SetBackgroundMaskBitmapL(icfmaskImg) );
            }
        else
            {
            TRAP_IGNORE( AknsUtils::CreateIconL(skininstance,
                                   id,
                                   icfImg,
                                   bmpFileName,
                                   icfbgId) );
            }
    	
    	AknIconUtils::SetSize(icfImg, TSize(1,1), EAspectRatioNotPreserved);
    	TRAP_IGNORE( iMultiLineIcf->SetBackgroundBitmapL(icfImg) );
    	}

    // read text color
    TRgb textcolor;
    TInt32 colorMajorSkinId = resReader.ReadInt32();
    skinitemid = resReader.ReadInt16();
    
    id.Set(TInt(colorMajorSkinId), skinitemid);

    const TInt16 textcoloridx = resReader.ReadInt16();
    
    TInt error = AknsUtils::GetCachedColor(skininstance, 
                                           textcolor,
                                           id,
                                           textcoloridx);

    if (error == KErrNone)
        {
        iMultiLineIcf->SetTextColorL( textcolor );
        }
    else
        {
        iMultiLineIcf->SetTextColorL( TRgb( KDefaultTextColor ) );
        }    
    CleanupStack::PopAndDestroy( 1 ); // resReader*/
    }

EXPORT_C TBool CPeninputLayoutWindow::IsCanChangeRange(TInt /*aRange*/)
	{
	return ETrue;
	}
    
EXPORT_C void CPeninputLayoutWindow::ChangeToPreviousRange(TInt /*aRange*/)
	{
		
	}

EXPORT_C void CPeninputLayoutWindow::SetTextAlignmentL( TInt aAlignment )	
	{
	if ( iMultiLineIcf && ConfigInfo()->Language() != ELangNone )
		{
		iMultiLineIcf->SetTextAlignmentL( aAlignment, ConfigInfo()->Language() );
		}
	}
//End Of File
