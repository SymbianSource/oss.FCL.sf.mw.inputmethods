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
#include <peninputlayoutmultilineicf.h>
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
//#include <peninputcmdparam.h>
#include <aknFepVietnameseInputTables.h>

// User includes
#include "peninputdataconverter.h"
#include "peninputrangebarinfo.h"
#include "peninputlayoutwindowext.h"
#include "peninputclientlayoutinfo.h"
#include "peninputdataprovider.h"
#include "peninputlayoutcontext.h"
#include "peninputvkbctrlext.h"
#include "peninputnumerickeymappingmgr.h"
#include "peninputgenericvkb.hrh"

// Constants
const TInt KIntLengthForByte = 8;
const TInt KPeninputLayoutWindowUnitWidth = 12;
const TInt KInvalidIndex = -1;

//const TInt32 KInvalidResId = -1;
//const TInt KInvalidImg = -1 ;
const TUint32 KDefaultTextColor = 0x000000;
const TUint32 KDefaultShadowTextColor = 0xffffff;
//const TInt KNotSupportSkin = -1;

const TInt KIntSizeToInt16 = 2;
const TInt KMaxNumericString = 32;

// ======== MEMBER FUNCTIONS ========

// ------------------------------------------------------------------------
// CPeninputLayoutWindowExt::CPeninputLayoutWindowExt
// (other items were commented in a header)
// ------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutWindowExt::CPeninputLayoutWindowExt( 
    CFepUiLayout* aUiLayout, MPeninputLayoutContext* aLayoutContext )
    : CAknFepCtrlBaseWindowExt( aUiLayout, EPeninutWindowCtrlIdBaseWindow ), 
     iConfigInfo( NULL ), iLayoutContext( aLayoutContext ),
    iLastUsedTotalColumns( KPeninputLayoutWindowUnitWidth ),
    iFirstTimeConstruct( ETrue ),
    iSwitchFontSet(EFalse),
    iUnitSizeChange(EFalse)
    { 
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::ConstructL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::ConstructL()
    {
    BaseConstructL();  

    if( iLayoutContext->LayoutType() == EPluginInputModeVkb ||
        iLayoutContext->LayoutType() == EPluginInputModeFSQ )
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
        
    //TResourceReader reader;
    
    //coeEnv->CreateResourceReaderLC( reader, GetWindowResId() );
    SetResourceId( GetWindowResId() );
    ConstructFromResourceL();
    
    if (!iKeyMappingMgr)
        {
        // if key mapping mgr is not constructed in ConstructFromResourceL, need to 
        // construct it here
        TResourceReader resReader;
        CCoeEnv::Static()->CreateResourceReaderLC(resReader, GetNumKeymappingResId());
        iKeyMappingMgr = CPeninputNumericKepMappingMgr::NewL(resReader);
        CleanupStack::PopAndDestroy(1);
        }

    CreateAllControlsL();
    SetControlsFont();
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::~CPeninputLayoutWindowExt
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutWindowExt::~CPeninputLayoutWindowExt()
    {
    delete iConfigInfo;
    delete iKeyMappingMgr;
    delete iVkbLayout;    
    // Delete resource file from CoeEnv
    CCoeEnv::Static()->DeleteResourceFile( iWinResId );
    
    CCoeEnv::Static()->DeleteResourceFile( iConfigResId );
    
    delete iToneSet;   
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::SizeChanged
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::SizeChanged( TBool /*aLandscapeStyle*/ )
    {
    iLangOrSizeChanged = ETrue;    
    ReadLafForShadow();
	ResetLastColRow();
    
    if ( iConfigInfo )
        {        
        ChangeClientSize();        
        }
        
    //ReadLafForShadow( rect );
    
    TInt style = EPeninputPositionChangeBrJustify;
    TBuf<KIntSizeToInt16> bufStyle;
    bufStyle = ( TUint16* )&style;
    HandleControlEvent(EPeninputLayoutEventMovePosition, NULL, bufStyle);           
        
    iLangOrSizeChanged = EFalse;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::ReadLafForShadow
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindowExt::ReadLafForShadow()
    {
    TAknWindowLineLayout shadowPane;
    TAknWindowLineLayout shadowTl;
    TAknWindowLineLayout shadowBr;
    TAknLayoutRect shadowRect;
    TAknLayoutRect shadowTlRect;
    TAknLayoutRect shadowBrRect;

    shadowPane = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane(1).LayoutLine();
    shadowRect.LayoutRect(TRect(), shadowPane);

    shadowTl = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane_g1().LayoutLine();
    shadowTlRect.LayoutRect(shadowRect.Rect(), shadowTl);

    shadowBr = AknLayoutScalable_Avkon::bg_popup_fep_shadow_pane_g8().LayoutLine();
    shadowBrRect.LayoutRect(shadowRect.Rect(), shadowBr);
    
    iShadowTlWidth = shadowTlRect.Rect().Size().iWidth;
    iShadowTlHeight = shadowTlRect.Rect().Size().iHeight;
    iShadowBrWidth = shadowBrRect.Rect().Size().iWidth;
    iShadowBrHeight = shadowBrRect.Rect().Size().iHeight;
    }
    
// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::SizeChanged
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::SizeChangedForBaseWindow( TInt /*aTotalColumns*/ )
    {
    }
// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::HandleControlEvent
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::HandleControlEvent( TInt aEventType,
    CFepUiBaseCtrl* aCtrl, const TDesC& aEventData )
    {
    // Call this function in base class
    CAknFepCtrlBaseWindowExt::HandleControlEvent( aEventType, 
        aCtrl, aEventData );
    
    TInt* data = ( TInt* ) aEventData.Ptr(); 
    
    switch ( aEventType )
        {
        case EEventButtonUp:
        	{
        	// check current range
		    TInt curRange = CPeninputDataConverter::AnyToInt
		        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );		    
        	if( aCtrl->ControlId() == EPeninutWindowCtrlIdShiftBtn)
	        	{
	        	  if ( ( curRange == ERangeEnglish )  || ( curRange == ERangeAccent ) 
        			   || ( ConfigInfo()->CaseSensitive() && ( curRange == ERangeNative ) ) )
	        	  	{
	        		CAknFepCtrlMultiModeButton* button = 
					static_cast<CAknFepCtrlMultiModeButton*>( aCtrl );
		        	TInt realCase = button->GetRealCaseByMode( button->CurrentMode() );	     	       		
					HandleMergedBtnClicked( realCase );
	        		}
		       	else
		       		{
		       		HandleShiftAndCapslockBtnClicked();
		       		TInt shiftStatus = CPeninputDataConverter::AnyToInt
				        ( iLayoutContext->RequestData( EPeninputDataTypeShiftDown ) );
				    ChangeMergedButtonStatus( shiftStatus, 0 );
		       		}				
	        	}

        	}
        	break;
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
            HandleShiftAndCapslockBtnClicked();
       		TInt shiftStatus = CPeninputDataConverter::AnyToInt
		        ( iLayoutContext->RequestData( EPeninputDataTypeShiftDown ) );
		    ChangeMergedButtonStatus( shiftStatus, 0 );
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

            iLayoutContext->SetData( EPeninputDataTypePermittedRange, data );
            
            // Set range of range bar component
            CFepUiBaseCtrl* bar = 
                Control( EPeninutWindowCtrlIdRangeBar );
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
                Control( EPeninutWindowCtrlIdRangeBar );
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
            if(aCtrl->ControlId() == EPeninutWindowCtrlIdInputContextField ||  
               aCtrl->ControlId() == EPeninputWindowCtrlIdMultiLineICF)
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
// CPeninputLayoutWindowExt::SetEditorTextL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::SetEditorTextL( 
    const TFepInputContextFieldData& aData )
    {
    if (Control(EPeninutWindowCtrlIdInputContextField))
        {
        static_cast<CFepInputContextField*>( Control(EPeninutWindowCtrlIdInputContextField))->SetTextL( aData );
        return;
        }
    
    if (Control(EPeninputWindowCtrlIdMultiLineICF))
        {
        CFepLayoutMultiLineIcf* multiIcf = 
            static_cast<CFepLayoutMultiLineIcf*>( Control(EPeninputWindowCtrlIdMultiLineICF));
            
        multiIcf->SetTextL( aData );
      
        if ( iLayoutContext->LayoutType() == EPluginInputModeVkb || 
             iLayoutContext->LayoutType() == EPluginInputModeFSQ )
            {
            TInt lang = CPeninputDataConverter::AnyToInt
                    ( iLayoutContext->RequestData( EPeninputDataTypeInputLanguage ) );
            TInt curRange = CPeninputDataConverter::AnyToInt
        		    ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
        		        
            if ( lang == ELangVietnamese && curRange == ERangeEnglish )  
                {
                TBuf<1> preData;
                multiIcf->ExtractText( preData, aData.iCurSel.LowerPos()-1, 1 );
                iVowelChar = EFalse;
                
                if ( KNullDesC() != preData )
                    {
                    for (TUint i = 0; i < sizeof(VietVowelList) / sizeof(TText); ++i)
                        {
                        TBuf<1> buf;
                        buf.Append( VietVowelList[i] );
                        if (preData == buf)
                            {
                            iVowelChar = ETrue;
                            break;
                            }
                        }
                    }
                CPeninputVkbCtrlExt* vkbCtrl = static_cast<CPeninputVkbCtrlExt*>
                    ( Control( EPeninutWindowCtrlIdVkbCtrl ) );
                vkbCtrl->DimKeySet( iToneSet, !iVowelChar );
                }
            }
        }
    }  

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::SetEditorTextL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::SetTextIsSecret( TBool aData )
    {
    if ( Control(EPeninutWindowCtrlIdInputContextField))
        {
        static_cast<CFepInputContextField*>
        (Control(EPeninutWindowCtrlIdInputContextField))->SetTextIsSecret( aData );
        }
    else if (Control(EPeninputWindowCtrlIdMultiLineICF))
        {
        static_cast<CFepLayoutMultiLineIcf*>
        (Control(EPeninputWindowCtrlIdMultiLineICF))->SetTextIsSecret( aData );
        }
    }  
    
void CPeninputLayoutWindowExt::SetNumberGrouping( TBool aData )
    {
    if (Control(EPeninputWindowCtrlIdMultiLineICF))
        {
        static_cast<CFepLayoutMultiLineIcf*>
        (Control(EPeninputWindowCtrlIdMultiLineICF))->SetNumberGrouping( aData );
        }
    }
    

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::DoClose
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C TBool CPeninputLayoutWindowExt::DoClose()
    {
    // Send EVkbEventWindowClose to layout
    this->ReportEvent( EPeninputLayoutEventClose, KNullDesC );
    
    // Return true which will permit the window close
    return EFalse;
    }

    
// ---------------------------------------------------------------------------
// Get layout config method. It does a lasy initialization for a instance of
// CPeninputLayoutWindowExt::ConfigInfo
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C CPeninputLayoutConfig* CPeninputLayoutWindowExt::ConfigInfo() const
    {
    return iConfigInfo;
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::CheckResourceExist
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
TBool CPeninputLayoutWindowExt::CheckResourceExist( const TDesC& aFileName )
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
// CPeninputLayoutWindowExt::ChangeInputLanguageL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::ChangeInputLanguageL( TInt aLangID )
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
   
        if ( iLayoutContext->LayoutType() == EPluginInputModeVkb || 
            iLayoutContext->LayoutType() == EPluginInputModeFSQ )
            {       
            CFepUiBaseCtrl* ctrl = 
                Control( EPeninutWindowCtrlIdVkbCtrl );
            if ( ctrl )
                {
                ( static_cast<CPeninputVkbCtrlExt*>( ctrl ) )->Reset();
                }
            } 

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
        if ( iLayoutContext->LayoutType() == EPluginInputModeVkb || 
            iLayoutContext->LayoutType() == EPluginInputModeFSQ )
            {
            iVkbLayout->Reset(); 
            }
        iConfigInfo = CPeninputLayoutConfig::NewL( *iVkbLayout, reader ); 
        
        // Pop and destroy reader
        CleanupStack::PopAndDestroy( 1 ); 
        	       
        // Re-construct rangebar
        ChangeRangeBarL();

	    //Reset range and layoutid
	    TInt range = -1;
	    iLayoutContext->SetData( EPeninputDataTypeCurrentRange, &range );
	    iLayoutContext->SetData( EPeninputDataTypeClientLayout, &range );
	    ResetLastColRow();
	    iLangOrSizeChanged = ETrue;
	    
	    CFepUiBaseCtrl* icfCtrl = Control( EPeninputWindowCtrlIdMultiLineICF );
        if ( icfCtrl )
            {
            static_cast<CFepLayoutMultiLineIcf*>(icfCtrl)->SetLanguageId( aLangID );
            }
		
	    delete iToneSet;
	    iToneSet = NULL;
	        
	    if ( aLangID == ELangVietnamese )
            {
            iToneSet = HBufC::NewL( KNumberOfToneMarks );
        
            for (TInt index = 0; index < KNumberOfToneMarks; index ++)
                {
                iToneSet->Des().Append( VietToneMatrix[0][index] );
                }
            }
	    }
    }
    
// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::ChangeRangeBarL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindowExt::ChangeRangeBarL()
    {
    // Read range bar from config into and reconstruct range button
    if ( iConfigInfo )
        {
        TInt resid = iConfigInfo->RangebarResId();
        CFepUiBaseCtrl* bar = 
            Control( EPeninutWindowCtrlIdRangeBar );
            
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
// CPeninputLayoutWindowExt::ChangeRange
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::ChangeRange( TInt aRange, TInt aVkbLayoutId, 
                                                     TBool aNotify )
    {
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
    TInt curVkbLayout = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeVkbLayout ) );
    TInt curLanguage =  CPeninputDataConverter::AnyToInt
            ( iLayoutContext->RequestData( EPeninputDataTypeInputLanguage ) );

    CFepUiBaseCtrl* bar = 
        Control( EPeninutWindowCtrlIdRangeBar );
    CAknFepCtrlRangeBar* rangebar = 
        static_cast<CAknFepCtrlRangeBar*>( bar );
        
    if ( rangebar )
        {
        rangebar->ActiveRange( aRange );
        }
        
    if (curRange != aRange || curVkbLayout != aVkbLayoutId || aRange == ERangeAccent)
        {
        // Update data:range
        if (curRange != aRange)
            iLayoutContext->SetData( EPeninputDataTypeCurrentRange, &aRange );
            
        // Change client layout
        CPeninputRangeBarInfo* rangeBarInfo = iConfigInfo->RangeBarInfo();
        CPeninputRangeInfo* rangeInfo = rangeBarInfo->FindRange(aRange);
        
        __ASSERT_DEBUG(rangeInfo, User::Panic(_L("Commonlayout-Invalid Range"), aRange));

        ChangeClientLayout(rangeInfo->ClientLayoutId());
        
        // Notify fep the new range
        if (aNotify)
            {
            TBuf<KIntLengthForByte> buf;
            CPeninputDataConverter::IntToDesc( aRange, buf );
            iLayoutContext->Sendkey( ESignalRange, buf );
            
	        // When change range mannually, make the entire window visible
	        TInt style = EPeninputPositionChangeBrJustify;
	        TBuf<KIntSizeToInt16> bufStyle;
	        bufStyle = ( TUint16* )&style;
	        HandleControlEvent(EPeninputLayoutEventMovePosition, NULL, bufStyle);           
            }

        // Change vkb layout
        if ( iLayoutContext->LayoutType() == EPluginInputModeVkb ||
            iLayoutContext->LayoutType() == EPluginInputModeFSQ )
            {
            if ( ( aRange == ERangeEnglish ) || ( aRange == ERangeAccent ) 
                || (( aRange == ERangeNativeNumber ) && (iLayoutContext->LayoutType() != EPluginInputModeFSQ ))
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
                //ChangeButtonStatus( shfit, EPeninutWindowCtrlIdShiftBtn );
                iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &capslock );
                //ChangeButtonStatus( capslock, EPeninutWindowCtrlIdCapslockBtn );                
                ChangeMergedButtonStatus( shfit, capslock );
                
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
            // Deal these four language in special way
            else if((( aRange == ERangeNativeNumber ) || ( aRange == ERangeNumber ))
            		&& ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
            		&& (( curLanguage == ELangArabic )
            		|| ( curLanguage == ELangFarsi) 
            		|| ( curLanguage == ELangUrdu )
            		|| ( curLanguage == ELangThai )))
            	{
                 if ( !aVkbLayoutId )
                     {
                     TInt shiftFlag = ( aRange == ERangeNativeNumber )? 0:1;
                     aVkbLayoutId = ( aRange == ERangeNativeNumber )? 
                		     EPeninputVkbLayoutNativeNumberShift : EPeninputVkbLayoutLatinNumber;
                		   
                     TInt capslockFlag = 0;                   
                   
                     iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shiftFlag );
                     iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &capslockFlag );
                   
                     ChangeMergedButtonStatus( shiftFlag, capslockFlag );  
                     }
                ChangeVkbLayout( aVkbLayoutId );
                }
            else
                {
                TInt flag = 0;
                
                iLayoutContext->SetData( EPeninputDataTypeShiftDown, &flag );
                //ChangeButtonStatus( 0, EPeninutWindowCtrlIdShiftBtn );
                iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &flag );
                //ChangeButtonStatus( 0, EPeninutWindowCtrlIdCapslockBtn );
                ChangeMergedButtonStatus( 0, 0 );
     
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
            
        }
        
    if ( ( iLayoutContext->LayoutType() == EPluginInputModeVkb ||
        iLayoutContext->LayoutType() == EPluginInputModeFSQ )
          && ( curRange == ERangeNumber || curRange == ERangeNativeNumber )
          && ( curRange == aRange ) )
        {
        // Dimmed some keys if needed  
        DimKeys();                 
        }                        
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::ChangeClientLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::ChangeClientLayout( 
    TInt aClientLayoutId )
    {
    TInt curClientId = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeClientLayout ) );

    if ( curClientId != aClientLayoutId )
        {
        CPeninputClientLayoutInfo* clientLayout = 
            ConfigInfo()->FindClientLayoutInfo( aClientLayoutId );
            
        __ASSERT_DEBUG( clientLayout, User::Panic(_L("Commonlayout-Invalid ClientLayout"), aClientLayoutId));

        SetLafLayOut( clientLayout->Rows(), clientLayout->Columns() );
        
        //SizeChangedForBaseWindow( clientLayout->Columns() );

        // Re-Orginize all controls according to the configuration
        ReorganizeControls( aClientLayoutId, ETrue );
      
        // Update data:client layout id
        iLayoutContext->SetData( EPeninputDataTypeClientLayout, 
                                 &aClientLayoutId );
        }
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::ChangeVkbLayout
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::ChangeVkbLayout( TInt aVkbLayoutId )
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
        iLangOrSizeChanged = EFalse;
        // Update data : vkb layout id
        iLayoutContext->SetData( EPeninputDataTypeVkbLayout, &aVkbLayoutId );

        iVkbLayout->ChangeVkbLayout( aVkbLayoutId );        
        SetVkbLayoutSize();
        
        TInt curClientId = CPeninputDataConverter::AnyToInt 
           ( iLayoutContext->RequestData( EPeninputDataTypeClientLayout ) );
        CPeninputControlInfo* vkbInfo = iConfigInfo->FindClientLayoutInfo 
            ( curClientId )->FindControlInfo( EPeninutWindowCtrlIdVkbCtrl );
    
        TRect rect;
        CPeninputVkbCtrlExt* vkbCtrl = static_cast<CPeninputVkbCtrlExt*>
            ( Control(EPeninutWindowCtrlIdVkbCtrl));
      
          
        CPeninputVkbLayoutInfo* vkblayout = 
            ConfigInfo()->FindVkbLayoutInfo( aVkbLayoutId );  
      
        if ( vkbCtrl && vkblayout )
            {
            DimKeys(); 
            
            // Make the true draw
            UpdateArea( vkbCtrl->Rect(),  EFalse ); 
            }     
        }        
    }

// --------------------------------------------------------------------------
// CPeninputLayoutWindowExt::HandleRangeEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::HandleRangeEvent( TInt aRange )
    {                                 
    TInt curRange = CPeninputDataConverter::AnyToInt 
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
  
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
    
    if (curRange != aRange && responseStyle == EPeninputRangeResponseSwitchRange)
        {
        TInt index = KInvalidIndex;

        if ( aRange == ERangeAccent )
            {
            index = 0;
            }
        iLayoutContext->SetData( EPeninputDataTypeCurrentAccent, &index );

        ChangeRange( aRange );     
        }
    else if ( responseStyle == EPeninputRangeResponsePopupChoiceItem )
        {
        PopupChoiceList();
        }
    }

// --------------------------------------------------------------------------
// CPeninputLayoutWindowExt::HandleRangeLoopEvent
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::HandleRangeLoopEvent( TInt aLoop )
    {
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
                
    CFepUiBaseCtrl* ctrl = Control( EPeninutWindowCtrlIdRangeBar );
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
// CPeninputLayoutWindowExt::CaseByShiftCapslockStatus
// (other items were commented in a header)
// --------------------------------------------------------------------------
//
EXPORT_C TInt CPeninputLayoutWindowExt::CaseByShiftCapslockStatus()
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
EXPORT_C void CPeninputLayoutWindowExt::HandleMergedBtnClicked( TInt aCase )
	{
	switch( aCase )
		{
		case ECaseLower:
		    {
			HandleCapslockBtnClicked();
		    }
		    break;
		case ECaseUpper:
			{
			HandleShiftBtnClicked();
			}
			break;
		case ECaseText:
//		case ECaseInverseText:
			{
			// Current case is Text, equals shift button
			//HandleCapslockBtnClicked();			
			HandleShiftAndCapslockBtnClicked();
			}
			break;	
		default:
			break;
		}
		
	}

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::HandleShiftBtnClicked
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::HandleShiftBtnClicked()
    {
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
    
    TInt shiftStatus = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeShiftDown ) );
    TInt capslockStatus = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCapslockDown ) );
         
    // Update data
    if ( iLayoutContext->IsCapsLockPermitted() )
    	{
    	if ( curRange == ERangeEnglish || curRange == ERangeAccent 
	        || ( ConfigInfo()->CaseSensitive() && curRange == ERangeNative ) )
		    {
	        capslockStatus = !capslockStatus;
	        ChangeMergedButtonStatus( shiftStatus, capslockStatus );
	        iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &capslockStatus );   
		    }
    	}
    else
    	{
    	if ( iLayoutContext->IsShiftPermitted() )
    		{
    		shiftStatus = !shiftStatus;
	        ChangeMergedButtonStatus( shiftStatus, capslockStatus );
	        iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shiftStatus );
    		}
    	}	
  
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
    if ( ( curRange == ERangeEnglish )  || ( curRange == ERangeAccent ) 
        || ( ConfigInfo()->CaseSensitive() && ( curRange == ERangeNative ) ) )
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
// CPeninputLayoutWindowExt::HandleCapslockBtnClicked
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::HandleCapslockBtnClicked()
    {
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );

    TInt capslockStatus = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCapslockDown ) );
    TInt shiftStatus = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeShiftDown ) );
        
    if ( iLayoutContext->IsShiftPermitted() )
    	{
    	if ( curRange == ERangeEnglish || curRange == ERangeAccent || 
            ( ConfigInfo()->CaseSensitive() && curRange == ERangeNative ) )
	        {
	        shiftStatus = !shiftStatus;
	        ChangeMergedButtonStatus( shiftStatus, capslockStatus );
	        iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shiftStatus );   
	        }
    	}
    else
    	{
    	if ( iLayoutContext->IsCapsLockPermitted() )
    		{
    		capslockStatus = !capslockStatus;
	        ChangeMergedButtonStatus( shiftStatus, capslockStatus );
	        iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &capslockStatus );
    		}
    	}
            
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
    if ( ( curRange == ERangeEnglish )  || ( curRange == ERangeAccent ) 
        || ( ConfigInfo()->CaseSensitive() && ( curRange == ERangeNative ) ) )
        {
        // Notify fep the new case
        TBuf<KIntLengthForByte> buf;
        TInt fepcase = CPeninputDataConverter::FepCaseByCaseId( caseid );
        iLayoutContext->SetData( EPeninputDataTypeCase, &fepcase );
        CPeninputDataConverter::IntToDesc( fepcase, buf );
        iLayoutContext->Sendkey( ESignalCaseMode, buf );
        }
    }
    
EXPORT_C void CPeninputLayoutWindowExt::HandleShiftAndCapslockBtnClicked()
    {
    TInt curRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
    
    TInt shiftStatus = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeShiftDown ) );
    TInt capslockStatus = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeCapslockDown ) );
    TInt languageStatus = CPeninputDataConverter::AnyToInt
            ( iLayoutContext->RequestData( EPeninputDataTypeInputLanguage ));
    TInt permittedRange = CPeninputDataConverter::AnyToInt
                ( iLayoutContext->RequestData( EPeninputDataTypePermittedRange ));
     
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
        
    if ( iLayoutContext->IsCapsLockPermitted() )
    	{
    	if ( curRange == ERangeEnglish || curRange == ERangeAccent 
	        || ( ConfigInfo()->CaseSensitive() && curRange == ERangeNative ) )
		    {
	        capslockStatus = !capslockStatus;
	        ChangeMergedButtonStatus( shiftStatus, capslockStatus );
	        iLayoutContext->SetData( EPeninputDataTypeCapslockDown, &capslockStatus );   
		    }
    	}
    else
    	{
    	if ( iLayoutContext->IsShiftPermitted() )
    		{
    		shiftStatus = !shiftStatus;
	        ChangeMergedButtonStatus( shiftStatus, capslockStatus );
	        iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shiftStatus );
    		}
    	}
	
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
        
    TBool bSupportNative = ( languageStatus == ELangArabic )
        		        || ( languageStatus == ELangFarsi) 
        		        || ( languageStatus == ELangUrdu )
        		        || ( languageStatus == ELangThai );
    
    // Click shift will change number between "native number" and "latin number"
    if (( iLayoutContext->LayoutType() == EPluginInputModeFSQ ) 
     && ( bSupportNative )
     && (( ERangeNativeNumber == curRange ) || ( ERangeNumber == curRange )))
    	{
        if (( ERangeNumber == curRange )
         && ( permittedRange & ERangeNativeNumber ))
       	    {
            vkbLayout = EPeninputVkbLayoutNativeNumberShift;
            }
    
        if (( ERangeNativeNumber == curRange )
         && ( permittedRange & ERangeNumber ))
       	    {
            vkbLayout = EPeninputVkbLayoutLatinNumber;
            }
    	}
    else
    	{
        ChangeVkbLayout( vkbLayout );
    	}
    
    // Synchronize case if needed    
    if ( ( curRange == ERangeEnglish )  || ( curRange == ERangeAccent ) 
        || ( ConfigInfo()->CaseSensitive() && ( curRange == ERangeNative ) ) )
        {
        // Notify fep the new case
        TBuf<KIntLengthForByte> buf;
        TInt fepcase = CPeninputDataConverter::FepCaseByCaseId( caseid );
        iLayoutContext->SetData( EPeninputDataTypeCase, &fepcase );
        CPeninputDataConverter::IntToDesc( fepcase, buf );
        iLayoutContext->Sendkey( ESignalCaseMode, buf );
        }
    else if ((( curRange == ERangeNumber ) || ( curRange == ERangeNativeNumber ))
    	   && ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
    	   && ( bSupportNative ))
    	{
        // If support native number, change range to native number
        if( curRange == ERangeNumber && ( permittedRange & ERangeNativeNumber ))
        	{
            ChangeRange( ERangeNativeNumber, vkbLayout );
        	}
        // If not support native number, set shift state back
        else if (( curRange == ERangeNumber ) && !( permittedRange & ERangeNativeNumber ))
        	{
            TInt shiftButton = 1;
            TInt capslockButton = 0;
            ChangeMergedButtonStatus( shiftButton, capslockButton );
            iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shiftButton );
        	}
        // If support latin number, change range to latin number
        else if (( curRange == ERangeNativeNumber ) && ( permittedRange & ERangeNumber ) )
        	{
            ChangeRange( ERangeNumber, vkbLayout );
        	}
        // If not support latin number, set shift state back
        else if (( curRange == ERangeNativeNumber ) && !( permittedRange & ERangeNumber ))
            {
            TInt shiftButton = 0;
            TInt capslockButton = 0;
            ChangeMergedButtonStatus( shiftButton, capslockButton );
            iLayoutContext->SetData( EPeninputDataTypeShiftDown, &shiftButton );
            }
    	}
    else
    	{
        // Do nothing
    	}
    
    
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::DimKeys
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
void CPeninputLayoutWindowExt::DimKeys()
    {
    TRAP_IGNORE(DimKeysL());
    }
void CPeninputLayoutWindowExt::DimKeysL()
    {
    CPeninputVkbCtrlExt* vkbCtrl = static_cast<CPeninputVkbCtrlExt*>
        ( Control( EPeninutWindowCtrlIdVkbCtrl ) );
    vkbCtrl->CancelDims();
    TInt currentRange = CPeninputDataConverter::AnyToInt 
        ( iLayoutContext->RequestData( EPeninputDataTypeCurrentRange ) );
    TInt permitRange = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypePermittedRange ) ); 
    TInt keyMapping = CPeninputDataConverter::AnyToInt 
        ( iLayoutContext->RequestData( EPeninputDataTypeNumericKeymap ) );
    TInt inputLang = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeInputLanguage ) );
         
    if ( currentRange == ERangeNumber && 
            (permitRange == ERangeNumber || 
                    permitRange == ERangeNativeNumber || 
                    permitRange == (ERangeNumber | ERangeNativeNumber)) )
        {
        HBufC* rs = NULL;
        if ( keyMapping == EKeymapFromResource )
            {
            HBufC* customRes = 
                           (HBufC*) iLayoutContext->RequestData(EAknFepDataTypeUseDefinedResource);
            _LIT(KTenNumbers,"0123456789");
            rs = HBufC::NewL(KTenNumbers().Length() + customRes->Length());
            rs->Des().Copy(*customRes);
            rs->Des().Append(KTenNumbers);
            }
        else
            {
            TRAP_IGNORE( rs = iKeyMappingMgr->KeyMappingStringL( keyMapping ); );
            }
        
        if( rs )
            {
            vkbCtrl->DimKeys( rs );
            delete rs;
            }
        }
    else if ( currentRange == ERangeNativeNumber && 
            (permitRange == ERangeNumber || 
                    permitRange == ERangeNativeNumber || 
                    permitRange == (ERangeNumber | ERangeNativeNumber)) )
        {
        HBufC* rs = NULL;
        TRAP_IGNORE( rs = GetKeyMappingStringL( keyMapping, inputLang ) );
        if( rs )
            {
            vkbCtrl->DimKeys( rs );
            delete rs;
            }
        }
    
    else if ( (currentRange == ERangeNative || currentRange == ERangeEnglish) && CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeInputLanguage ) ) == ELangVietnamese )
        {
        vkbCtrl->DimKeySet( iToneSet, !iVowelChar );
        }
        
    vkbCtrl->Draw();
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::AddControlL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::AddNotOwnedControl( 
    CFepUiBaseCtrl* aControl )
    {
    //do nothing if we already has the control.
    if ( KErrNotFound != ControlList().Find( aControl ) )
        {
        return;
        }
    
    if ( aControl )
        {
        ( const_cast<RPointerArray<CFepUiBaseCtrl>&>
            ( ControlList() ) ).Append( aControl );
        }
    } 
// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::ChangeButtonStatus
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::ChangeButtonStatus( const TInt aIsDown, 
    const TInt aControlId )
    {
    CAknFepCtrlEventButton* button = 
        static_cast<CAknFepCtrlEventButton*>
        (ControlById( aControlId ) );

    if ( button )
        {
        button->SetHighlight( aIsDown == 0 ? EFalse : ETrue );
        } 
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindow::ChangeButtonStatus
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPeninputLayoutWindowExt::ChangeMergedButtonStatus( 
											const TInt aIsShiftCase, 
    								 		const TInt aIsCapslockCase )
	{
	CAknFepCtrlMultiModeButton* button = 
		static_cast<CAknFepCtrlMultiModeButton*>
        (ControlById( EPeninutWindowCtrlIdShiftBtn ) );
    
    if( !button )
        {
        return;
        }
    switch ( aIsCapslockCase * 2 + aIsShiftCase )
        {
        case 1: // text case        
            {
            TRAP_IGNORE( button->SetCurrentModeL( EBtnBmpActive, 1 ) );			
	    	button->SetHighlight( ETrue );
            }
            break; 
        case 2: // upper case
            {
            TRAP_IGNORE( button->SetCurrentModeL( EBtnBmpActive, 2 ) );	
            button->SetHighlight( ETrue );
            }
        	break;
        default:
            {
            TRAP_IGNORE( button->SetCurrentModeL( EBtnBmpActive, 0 ) );	
        	button->SetHighlight( EFalse );
            }
            break;
	    }
	}    								 	
// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::ChangeLayoutPosition
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//               
EXPORT_C void CPeninputLayoutWindowExt::ChangeLayoutPosition( TInt aJustifyStyle )
    {        
    if ( iLayoutContext->LayoutType() == EPluginInputModeFSQ )
        {
        TopLeftJustify();
        return;
        }
    switch ( aJustifyStyle )
        {
        case EPeninputPositionChangeBrJustify:
            {
            BottomRightJustify();
            }         
            break;
        case EPeninputPositionChangeDataQuery:
            {
            //BottomRightJustify();
            DataQueryJustify();
            }
            break;
        default:
            {
            TopLeftJustify();
            }
            break;
        }
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::SetWindowRect
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//      
EXPORT_C void CPeninputLayoutWindowExt::SetWindowRect( const TRect& aRect )
    {
    //we should know the layout size now
    if(iLayoutContext->LayoutType() == EPluginInputModeFSQ)
        {
        SetRect( aRect );
        UiLayout()->SetRect(aRect);
        }
    else
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
    }
    

// -----------------------------------------------------------------------------
// CPeninputLayoutWindowExt::SetSwitchBtnFont
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
EXPORT_C void CPeninputLayoutWindowExt::SetSwitchBtnFont(CAknFepCtrlEventButton& aSwitchBtn)
    {
    // read switch font laf info only when need
    if (CPeninputDataConverter::AnyToInt(
        iLayoutContext->RequestData(EAkninputDataTypeSizeChanging)) ||(!iSwitchFontSet))
    	{
        TAknTextLineLayout langSwitch;
        TAknTextLineLayout langSwitchShadow;

        switch( iLayoutContext->LayoutType() )
            {
            case EPluginInputModeVkb:
            case EPluginInputModeFSQ:
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
// CPeninputLayoutWindowExt::TopLeftJustify
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
EXPORT_C void CPeninputLayoutWindowExt::SetSwitchBtnTextColor(CAknFepCtrlEventButton& aSwitchBtn)
    {
    TRgb textColor;
    TInt error = AknsUtils::GetCachedColor(UiLayout()->SkinInstance(),
                                           textColor,
                                           KAknsIIDQsnTextColors,
                                           EAknsCIQsnTextColorsCG59);

    if (error != KErrNone)
        {
    	textColor = TRgb(KDefaultTextColor);
        }
        
    aSwitchBtn.SetFontColor( textColor );   
    
    TRgb shadowTextColor;    
    error = AknsUtils::GetCachedColor(UiLayout()->SkinInstance(),
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
// CPeninputLayoutWindowExt::TopLeftJustify
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
void CPeninputLayoutWindowExt::TopLeftJustify()
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
        
    UiLayout()->LayoutOwner()->SetPosition( tl );
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::BottomRightJustify
// (other items were commented in a header).
// ---------------------------------------------------------------------------
//    
void CPeninputLayoutWindowExt::BottomRightJustify()
    {
    // Read position from datamgr 
    TSize layoutSize( UiLayout()->Rect().Size() );

    TPoint oldbr;
    TPoint br;
    TPoint tl;

    TSize screenSize = *( ( TSize* ) iLayoutContext->RequestData
                            ( EPeninputDataTypeScreenSize ) );
    
    TInt windowPosition = CPeninputDataConverter::AnyToInt
        ( iLayoutContext->RequestData( EPeninputDataTypeWindowPosition ) );

    oldbr.iX = windowPosition & 0x0000FFFF;
    oldbr.iY = ( windowPosition & 0xFFFF0000 ) >> 16;
    
    br = oldbr;
        
    // Check the top left point is in screen
    if (br.iX > screenSize.iWidth)
        {
    	br.iX = screenSize.iWidth;
        }
    if( br.iY > screenSize.iHeight )
        {
    	br.iY = screenSize.iHeight;
        }

    tl.iX = br.iX - layoutSize.iWidth;
    tl.iY = br.iY - layoutSize.iHeight;
    
    if ( tl.iX < 0 )
        {
        tl.iX = 0;
        br.iX = layoutSize.iWidth;
        }

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
        
    UiLayout()->LayoutOwner()->SetPosition( tl );
    }

// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::DataQueryJustify
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//   
void CPeninputLayoutWindowExt::DataQueryJustify()
    {
    TRect* dataQueryRect = ( TRect* )( iLayoutContext->RequestData
        ( EPeninputDataTypeDataQueryDlgRect) );
    
    TRect rect = TRect( UiLayout()->Position(), UiLayout()->Rect().Size() );
    
	CCoeEnv* coeEnv = CCoeEnv::Static();
    TPixelsTwipsAndRotation ptSize;     
    coeEnv->ScreenDevice()->GetDefaultScreenSizeAndRotation(ptSize);    

	// Landscape mode: move window to the br of the screen
    if (ptSize.iPixelSize.iWidth > ptSize.iPixelSize.iHeight)
    	{
    	rect.SetRect(ptSize.iPixelSize.iWidth - rect.Width(), 
    			     ptSize.iPixelSize.iHeight/2 - rect.Height()/2,
    			     ptSize.iPixelSize.iWidth, 
    			     ptSize.iPixelSize.iHeight/2 + rect.Height()/2);
    	}
	// Portrait mode: dodge the dataquery   	
    else if ( rect.Intersects( *dataQueryRect ) )
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
        }

    //Write the new position into CenRep
    TInt x = 0;
    TInt y = 0;
    x = 0x0000FFFF & rect.iBr.iX;
    y = 0xFFFF0000 & ( rect.iBr.iY << 16 );
    TInt pos = x | y;
    iLayoutContext->SetData( EPeninputDataTypeWindowPosition, &pos );          

    //Set the layout position
    UiLayout()->LayoutOwner()->SetPosition( rect.iTl );
    }

TBool CPeninputLayoutWindowExt::CreateCustomControlL( TInt16 /*aControlId*/, TInt32 /*aImageId*/ )
	{
	return EFalse;	
	}
    
// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::Draw
// (other items were commented in a header)
// ---------------------------------------------------------------------------
// 
EXPORT_C void CPeninputLayoutWindowExt::Draw()
    {    
    // Draw shadow
    CAknFepCtrlBaseWindowExt::Draw();
    UpdateArea(UiLayout()->Rect(), EFalse);   
    }
       
// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::GetKeyMappingStringL
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
HBufC* CPeninputLayoutWindowExt::GetKeyMappingStringL(TInt aKeyMapping,
    const TInt aLang )
    {
    HBufC* rs = NULL;
    if ( aKeyMapping == EKeymapFromResource )
        {
        HBufC* customRes = (HBufC*) iLayoutContext->RequestData(EAknFepDataTypeUseDefinedResource);
        _LIT(KTenNumbers,"0123456789");
        TBuf<KMaxNumericString> numbers;
        if( aLang == ELangArabic )
            {
            numbers.Format( _L("%c%c%c%c%c%c%c%c%c%c"),
                0x0660, 0x0661, 0x0662,
                0x0663, 0x0664, 0x0665,
                0x0666, 0x0667, 0x0668, 0x0669 );
            }
        else if( aLang == ELangUrdu || aLang == ELangFarsi )
            {
            numbers.Format( _L("%c%c%c%c%c%c%c%c%c%c"),
                0x06F0, 0x06F1, 0x06F2,
                0x06F3, 0x06F4, 0x06F5,
                0x06F6, 0x06F7, 0x06F8, 0x06F9 );
            }
        else if( aLang == ELangHindi )
            {
            numbers.Format( _L("%c%c%c%c%c%c%c%c%c%c"),
                0x0966, 0x0967, 0x0968,
                0x0969, 0x096a, 0x096b,
                0x096c, 0x096d, 0x096e, 0x096f );
            }
        else if( aLang == ELangThai )
            {
            numbers.Format( _L("%c%c%c%c%c%c%c%c%c%c"),
                0x0E50, 0x0E51, 0x0E52,
                0x0E53, 0x0E54, 0x0E55,
                0x0E56, 0x0E57, 0x0E58, 0x0E59 );        
            }
        else
            {
            numbers = KTenNumbers;
            }
            
        rs = HBufC::NewL(numbers.Length() + customRes->Length());
        rs->Des().Copy(*customRes);
        rs->Des().Append(numbers);
        }
    else
        {
        rs = iKeyMappingMgr->KeyMappingStringL( aKeyMapping, aLang );
        }
        
    return rs;     
    }
       
// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::SetEnableSwitchToHwr
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
EXPORT_C void CPeninputLayoutWindowExt::SetEnableSwitchToHwr(TBool aEnable)
    {
    CAknFepCtrlEventButton* switchToHwrBtn = static_cast<CAknFepCtrlEventButton*>
        ( Control( EPeninutWindowCtrlIdSwitchToHwrBtn ) );

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
// CPeninputLayoutWindowExt::SetEnableSettingBtn
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
EXPORT_C void CPeninputLayoutWindowExt::SetEnableSettingBtn(TBool aEnable)
    {
    CAknFepCtrlEventButton* optionBtn = static_cast<CAknFepCtrlEventButton*>
        (Control( EPeninutWindowCtrlIdOptionBtn ) );
            
    if ( aEnable )    
        {
        optionBtn->SetDimmed(EFalse);
        }
    else
        {
        optionBtn->SetDimmed(ETrue);
        optionBtn->SetActive(EFalse);
        } 
    }
        
// ---------------------------------------------------------------------------
// CPeninputLayoutWindowExt::ReconfigUnitSize
// (other items were commented in a header)
// ---------------------------------------------------------------------------
//     
EXPORT_C TBool CPeninputLayoutWindowExt::ReconfigUnitSize(TInt /*aClientLayoutId*/)
    {
    return EFalse;
    }
    
EXPORT_C void CPeninputLayoutWindowExt::SetLafLayOut(TInt /*aRows*/, TInt /*aColumns*/)
    {
    
    }

EXPORT_C void CPeninputLayoutWindowExt::SetVkbLayoutSize()
    {
    
    }
EXPORT_C void CPeninputLayoutWindowExt::OnInputLanguageChange()
    {
    if ( iLayoutContext->LayoutType() == EPluginInputModeVkb )
        {       
        CFepUiBaseCtrl* ctrl = 
            Control( EPeninutWindowCtrlIdVkbCtrl );
        if ( ctrl )
            {
            ( static_cast<CPeninputVkbCtrlExt*>( ctrl ) )->Reset();
            }
        } 
    }
    
void CPeninputLayoutWindowExt::AddCustomControlGroupL(CFepUiBaseCtrl* aCtrl)
	{
	AddControlL(aCtrl);
	}

void CPeninputLayoutWindowExt::ResetLastColRow()
	{
    iLastUsedTotalColumns = 0;
    iLastUsedTotalRows = 0;
	}


EXPORT_C void CPeninputLayoutWindowExt::SetTextAlignmentL( TInt aAlignment )	
	{
	CFepLayoutMultiLineIcf* icf = 
	    static_cast<CFepLayoutMultiLineIcf*>( Control(EPeninputWindowCtrlIdMultiLineICF));
	    
	if ( icf && ConfigInfo()->Language() != ELangNone )
		{
		icf->SetTextAlignmentL( aAlignment, ConfigInfo()->Language() );
		}
	}
	
//End Of File
