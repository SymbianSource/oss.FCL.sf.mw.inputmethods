/*
* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
* Description:  Implement of class CTruiMainViewContainer
*
*/


#include <eiklabel.h>
#include <barsread.h>
#include <aknnavide.h>
#include <akntabgrp.h>
#include <eikspane.h>
#include <aknViewAppUi.h>
#include <eikedwin.h>
#include <aknbutton.h>
#include <aknchoicelist.h>
#include <aknSctDialog.h>
#include <trui.rsg>
#include <StringLoader.h>
#include <aknnotewrappers.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <AknsBasicBackgroundControlContext.h>
#include <AknsDrawUtils.h>
#include <layoutmetadata.cdl.h>
#include <akntoolbar.h>
#include <AknPreviewPopUpController.h>
#include <AknsConstants.h>
#include <trui_icons.mbg>
#include <featmgr.h>

#include "truicontainerbase.h"
#include "truimainviewcontainer.h"
#include "truihwrbox.h"
#include "truiengine.h"
#include "truiappui.h"
#include "truiapplication.h"
#include "truihelp.h"
#include "trui.hrh"
#include "truimainview.h"

// Max length of text in indicator
const TInt KIndicatorTextLen = 10;

// Max length of text of silimar message
const TInt KSimilarMsgLength = 160;

// Delay time of showing tooltip
const TInt KTooltipShowDelay = 0;

// Delay time of hiding tooltip
const TInt KTooltipHideDelay = 3000;

// Size for buffer to fill maximum characters from SCT.
const TInt KSctBufferSize = 32;

const TInt KHwrTooltipY = 20;

const TInt KVarityNaviPaneContents = 0;

//the special gesture characters
_LIT(KDisplayBackspace, "\x2190");
_LIT(KDisplaySpace, "\xF800");
_LIT(KDisplayEnter, "\xF801");

// ======== MEMBER FUNCTIONS ========

CTruiMainViewContainer::CTruiMainViewContainer()
    {  
    }
    
void CTruiMainViewContainer::ConstructL( const TRect& aRect )
    {    
    // Initialize control array
    InitComponentArrayL();   
        
    // Set parent control or owning window
    CreateWindowL();
   
    iBackGround = CAknsBasicBackgroundControlContext::NewL
                                                      ( KAknsIIDQsnBgAreaMain, 
                                                        Rect(), 
                                                        EFalse );

    // Initialize controls
    InitializeControlsL();
 
    SetRect( aRect );    

    // Set first character of the character set as the default trained character
    const HBufC* charSet = iEngine->CharacterSet();
    ASSERT( charSet );
    SetCurrentCharL( charSet->Mid( 0, 1 ) );

    TInt currentRangeId = iEngine->GetCurrentCharacterRange();    
    const RArray<TInt>& rangeIdArray = 
                        iEngine->GetCurrentSupportedRangesId();
    TInt indexOfMenuItems = rangeIdArray.Find( currentRangeId );
    if ( indexOfMenuItems != KErrNotFound )
        {
        // Set choiclist' selected item to index.
        iChoicelist->SetSelectedIndex( indexOfMenuItems ); 
        }
    else
        {
        iChoicelist->SetSelectedIndex( 0 ); 
        }

    // Set hwrbox's rect to engine and set guiding line to hwrbox
    iHwrBox->SetDisplayBottomGuideLine( ETrue );
    if ( iEngine->CurrentLanguageScript() == EMainViewSubmenuHebrew )
        {
        iHwrBox->SetDisplayTopGuideLine( ETrue );
        }

    ActivateL();  

    }
    
CTruiMainViewContainer* CTruiMainViewContainer::NewL( const TRect& aRect )
    {
    CTruiMainViewContainer* self = CTruiMainViewContainer::NewLC( aRect );
    CleanupStack::Pop( self );
    return self;
    }
    
CTruiMainViewContainer* CTruiMainViewContainer::NewLC( const TRect& aRect )
    {
    CTruiMainViewContainer* self = new (ELeave) CTruiMainViewContainer();
    CleanupStack::PushL( self );
    self->ConstructL( aRect );
    return self;
    }
    
CTruiMainViewContainer::~CTruiMainViewContainer()
    {
    if ( iBackGround )
        {
        delete iBackGround;
        }
    delete iWrappedLabelText;
    delete iLabeltext;
    iLineWidthArray.Close();
    }
  
// ---------------------------------------------------------------------------
// Create controls to be displayed in this container.
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::InitializeControlsL()
    {    
    CAknToolbar* toolbar = iAppUi->View( KTruiMainViewId )->Toolbar();
    // Get Clear button from toolbar    
    if ( toolbar )
        {
        iClearBtn = static_cast<CAknButton*>
                    ( toolbar->ControlOrNull( EMainViewButtonIdDelete ) );
        iPreviewBtn = static_cast<CAknButton*>
                    ( toolbar->ControlOrNull( EMainViewButtonIdPreview ) );           
        }
            
    // Create Label for text above writting box
    iLabel = CreateLabelL( R_MAINVIEW_LABEL_TIP, this, NULL );
    Components().AppendLC( iLabel );
    CleanupStack::Pop( iLabel );
    
    // Create left button
    iLeftBtn = CreateButtonL( R_MAINVIEW_BUTTON_LEFT, this, this );
    Components().AppendLC( iLeftBtn );
    CleanupStack::Pop( iLeftBtn );
    
    // Create indicator
    iIindicator = CreateLabelL( R_MAINVIEW_LABEL_INDICATOR, this, this );
    Components().AppendLC( iIindicator );
    CleanupStack::Pop( iIindicator );
        
    // Create right button
    iRightBtn = CreateButtonL( R_MAINVIEW_BUTTON_RIGHT, this, this );
    Components().AppendLC( iRightBtn );
    CleanupStack::Pop( iRightBtn );
    
    // Create choicelist
    TInt buttonResource = ButtonResourceId
                         ( iEngine->GetCurrentCharacterRange() );
    CAknButton* rangeButton = CreateButtonL( buttonResource,
                                             this, NULL );
    iCurChoiceListBtn = rangeButton;
    CDesCArray* charRangeArray = iEngine->GetCharacterRangeName();
    CDesCArray* ownedCharRangeArray = new ( ELeave ) CDesCArrayFlat
                                          ( charRangeArray->MdcaCount() );
                                          
    CleanupStack::PushL( ownedCharRangeArray );
    for ( TInt i=0; i<charRangeArray->MdcaCount(); i++ )
        {
        ownedCharRangeArray->AppendL( charRangeArray->MdcaPoint( i ) );
        }
    iChoicelist = CreateChoiceListL( this, this, 
                                     ownedCharRangeArray,rangeButton );
    CleanupStack::Pop( ownedCharRangeArray );
    ownedCharRangeArray = NULL;// Ownership has been transfered to choicelist                                                                                                     
    Components().AppendLC( iChoicelist );
    CleanupStack::Pop( iChoicelist );    
    
    iHwrBox = CreateHwrBoxL( this, this );
    iHwrBox->SetHwrBoxFlag( CTruiHwrBox::EFrameHwrBox );
    Components().AppendLC( iHwrBox );
    CleanupStack::Pop( iHwrBox );    
    }

// ---------------------------------------------------------------------------
// Create label and display it in the container.
// ---------------------------------------------------------------------------
//
CEikLabel* CTruiMainViewContainer::CreateLabelL( const TInt aResourceId,
                                                 const CCoeControl* aParent,
                                             MCoeControlObserver* aObserver )
    {
    CEikLabel* aknLabel = new (ELeave) CEikLabel();
    CleanupStack::PushL( aknLabel );
    if ( aParent )
        {
        aknLabel->SetContainerWindowL( *aParent );
        }
    if ( aObserver )
        {
        aknLabel->SetObserver( aObserver );    
        }
    if ( aResourceId != 0 )
        {
        TResourceReader reader;
        iEikonEnv->CreateResourceReaderLC( reader, aResourceId );
        aknLabel->ConstructFromResourceL( reader );
        CleanupStack::PopAndDestroy(); // reader
        }    
    aknLabel->ActivateL();
    CleanupStack::Pop( aknLabel ); 
    return aknLabel;
    }

// ---------------------------------------------------------------------------
// Create choicelist and display it in the container.
// ---------------------------------------------------------------------------
// 
 CAknChoiceList* CTruiMainViewContainer::CreateChoiceListL
                                             ( CCoeControl* aParent,
                                             MCoeControlObserver* aObserver,
                                             CDesCArray* aTextArray,
                                             CAknButton* aButton )
    {
    CAknChoiceList* choiceList = CAknChoiceList::NewLC( aParent, 
                                                        aTextArray,
                     CAknChoiceList::EAknChoiceListWithoutCurrentSelection,                     
                                                        aButton );    
    choiceList->SetObserver( aObserver );   
    CleanupStack::Pop( choiceList );
    return choiceList;
    }

// ---------------------------------------------------------------------------
// Create HWRBox and display it in the container.
// ---------------------------------------------------------------------------
// 
CTruiHwrBox* CTruiMainViewContainer::CreateHwrBoxL( CCoeControl* aParent,
                                            MTruiHwrBoxObserver* aObserver )
    {   
    return CTruiHwrBox::NewL( aParent, aObserver, iBackGround );
    }
    
// ---------------------------------------------------------------------------
// Responds to changes to the size and position of the contents of this control.
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::SizeChanged()
    {        
    TInt varity = Layout_Meta_Data::IsLandscapeOrientation() ? KLandscapeVarity 
                                                             : KPortraitVarity;
    DoSizeChanged( varity );
    
    if ( iBackGround )
        {
        iBackGround->SetRect( Rect() );
        }
    }       
   
// ---------------------------------------------------------------------------
// From class CCoeControl.
// Handles key event.
// ---------------------------------------------------------------------------
//      
TKeyResponse CTruiMainViewContainer::OfferKeyEventL
                                     ( const TKeyEvent& aKeyEvent, 
                                       TEventCode aType )
    {    
    if ( aType == EEventKey )
        {        
        switch ( aKeyEvent.iCode )
            {
            case EKeyLeftArrow:
                {
                return EKeyWasConsumed;
                }
            case EKeyRightArrow:
                {       
                CAknNavigationDecorator* naviDecorator = 
                                                  iAppUi->GetNaviDecorator();
                CAknTabGroup* tabGroup = static_cast<CAknTabGroup*>
                                       ( naviDecorator->DecoratedControl() );
                return tabGroup->OfferKeyEventL( aKeyEvent, aType );
                }            
            default:
                break;
            }
        }    
    return EKeyWasNotConsumed;    
    }
    
// ---------------------------------------------------------------------------
// From class CCoeControl.
// The region of the control to be redrawn.
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::Draw( const TRect& /*aRect*/ ) const
    {
    // Get the standard graphics context
    CWindowGc& gc = SystemGc();
    // Gets the control's extent
    TRect rect( Rect() );
    // Clears the screen
    gc.Clear( rect );

    MAknsSkinInstance* skin = AknsUtils::SkinInstance();
    if ( AknsDrawUtils::HasBitmapBackground( skin, iBackGround ) ) 
        {
        AknsDrawUtils::Background( skin, iBackGround, gc, rect );
        }
    }
    
// ---------------------------------------------------------------------------
// From class CCoeControl.
// Handles a change to the control's resources.
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::HandleResourceChange( TInt aType )
    {
    CTruiContainerBase::HandleResourceChange( aType );
    if ( aType == KEikDynamicLayoutVariantSwitch && iHwrBox )
        {
        if ( iHwrBox->BoxState() == CTruiHwrBox::ENewModelDraw )
            {
            TRAP_IGNORE( iHwrBox->SaveModelByManuallyL() );
            }        
        }
    
    if ( aType == KAknsMessageSkinChange )
        {
        TRgb labelColor = KRgbBlack;
        TInt error = GetCachedLabelTextColor( labelColor );
        if ( error == KErrNone )
            {
            TRAPD( errLable, iLabel->OverrideColorL( EColorLabelText, 
                                                     labelColor ) );
            if ( errLable != KErrNone )
                {
                return;
                }
            TRAP_IGNORE( iIindicator->OverrideColorL( EColorLabelText, 
                                                      labelColor ) );            
            }
        TRgb iconColor = KRgbBlack;            
        // Change left icon
        iLeftBtn->State()->SetIcon( ConstructIconFromResourceL
                                    ( R_MAINVIEW_BUTTON_LEFT ) );
        // Change right icon
        iRightBtn->State()->SetIcon( ConstructIconFromResourceL
                                    ( R_MAINVIEW_BUTTON_RIGHT ) );
        
        // Change choicelist button
        TInt rangeId = iEngine->GetCurrentCharacterRange();
        TInt buttonResoureId  = ButtonResourceId( rangeId );
        iCurChoiceListBtn->State()->SetIcon( ConstructIconFromResourceL
                                             ( buttonResoureId ) );            
        }
    }

// ---------------------------------------------------------------------------
// Handles an event from an observed control.
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::HandleControlEventL(CCoeControl* aControl,
                                                 TCoeEvent aEventType)
    {
    // Handle left button event
    if ( aControl == iLeftBtn && aEventType == EEventStateChanged )
        {        
        //Get character set
        const HBufC* charSet = iEngine->CharacterSet();
        ASSERT( charSet );
        //Locate current position
        TInt charPos = charSet->Find( iCurrentChar );
        if ( charPos != KErrNotFound )
            {
            //Move left
            TInt length = charSet->Length();
            charPos = ( charPos - 1 + length ) % length;
            SetCurrentCharL( charSet->Mid( charPos, 1 ) );
            }
        }
    // Handle indicator button event
    else if ( aControl == iIindicator && aEventType == EEventRequestFocus )
        {	
        PreviewCharacterModel();
        }
    // Handle right button event
    else if ( aControl == iRightBtn && aEventType == EEventStateChanged )
        {
        //Get character set
        const HBufC* charSet = iEngine->CharacterSet();
        ASSERT( charSet );
        //Locate current position
        TInt charPos = charSet->Find( iCurrentChar );
        if ( charPos != KErrNotFound )
            {
            //Move left
            TInt length = charSet->Length();
            charPos = ( charPos + 1 + length ) % length;
            SetCurrentCharL( charSet->Mid( charPos, 1 ) );
            }
        }
    // Handle choicelist event
    else if ( aControl == iChoicelist && aEventType == EEventStateChanged )
        {
        // Handle character range changing event
        // Get the selected index            
        TInt index = iChoicelist->SelectedIndex();
            
        // Set current character range by index in Engine
        iEngine->SetCharacterRangeByIndexL( index );
            
        // Get the current character range Id
        TInt rangeId = iEngine->GetCurrentCharacterRange();
            
        // Handle character range changed
        HandleCharRangeChangedL( index, rangeId );
        }    
    }
         
// ---------------------------------------------------------------------------
// From class MTruiHWRBoxObserver.
// Responds to changes to the size and position of the contents of control.
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::SaveNewModelL()
    {    
    TBuf<KSimilarMsgLength> similarMsg;
    TRAPD( err, iEngine->SetCharacterModelL( iCurrentChar,
                                             iHwrBox->Model(),
                                             similarMsg ) );
    switch ( err )
        {
        case KErrNone:
            {
            //Dim clear button and preview button
            if ( iClearBtn )
                {
                iClearBtn->SetDimmed( EFalse );
                iClearBtn->DrawNow();
                }
            if ( iPreviewBtn )
                {
                iPreviewBtn->SetDimmed( EFalse );
                iPreviewBtn->DrawNow();                
                }
            TPoint hwrPos = iHwrBox->PositionRelativeToScreen();
            HBufC* tooltipText = 
                        iEikonEnv->AllocReadResourceLC( R_TRUI_TOOLTIP_SAVED );
            iHwrBox->ShowTooltipL( *tooltipText, 
                          hwrPos + TPoint( iHwrBox->Size().iWidth, KHwrTooltipY ),
                          KTooltipShowDelay, KTooltipHideDelay );
            CleanupStack::PopAndDestroy( tooltipText );                        
            break;
            }
        case KErrAlreadyExists:
            {
            //Clear invalid model
            ClearCurrentModel();
            
            // Transfer some special gesture chars into displayable chars.
            AdjustDisplayChars( similarMsg );
            
            //Popup err message
            HBufC* errMsg = iEikonEnv->AllocReadResourceLC( R_TRUI_NOTE_SIMILAR_MODEL );
            HBufC* tooltipText = HBufC::NewLC( errMsg->Length() + similarMsg.Length()
                                               + KExtraSize );
            TPtr tooltipTextPtr = tooltipText->Des();
            tooltipTextPtr.Format( *errMsg, &similarMsg );
            PopupInformationNoteL( *tooltipText );
            CleanupStack::PopAndDestroy( tooltipText );
            CleanupStack::PopAndDestroy( errMsg );
            break;
            }
        default:
            {
            //Clear invalid model
            ClearCurrentModel();
            //Popup err message
            HBufC* tooltipText = iEikonEnv->AllocReadResourceLC( R_TRUI_NOTE_SAVE_FAILED );
            PopupInformationNoteL( *tooltipText );
            CleanupStack::PopAndDestroy( tooltipText );
            break;
            }
        }
    };
    
// ---------------------------------------------------------------------------
// From class MTruiHWRBoxObserver.
// Handle the message event from the HWRBox
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::HandleEventL( TMessageType aMessage )
    {
    if ( aMessage == EModelExist )
        {
        HBufC* tooltipText = 
                      iEikonEnv->AllocReadResourceLC( R_TRUI_NOTE_CLEAR_BOX );
        PopupInformationNoteL( *tooltipText );
        CleanupStack::PopAndDestroy( tooltipText );        
        }
    }
    
// ---------------------------------------------------------------------------
// Handles language script changed.
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::HandleLanguageScriptChangedL( TInt aScriptId )
    {    
    CTruiEngine* engine = iAppUi->HwrEngine();
    const RArray<TInt>& languageScriptId = engine->SupportedLanguageScript();
    TInt index = languageScriptId.Find( aScriptId );
    if ( index != KErrNotFound )
        {
        // Set the language script 
        engine->SetLanguageScriptByIndexL( aScriptId );        
    
        // Copy language script names array        
        CDesCArray* rangeNames = engine->GetCharacterRangeName();        
        CDesCArray* itemsInChoiceList = new (ELeave) CDesCArrayFlat
                                                     ( rangeNames->Count() );
        CleanupStack::PushL( itemsInChoiceList );
        for ( TInt i = 0; i < rangeNames->Count(); i++ )
            {
            itemsInChoiceList->AppendL( ( *rangeNames )[i] );
            }
            
        // Set the items in choicelist
        iChoicelist->SetItems( itemsInChoiceList ); // Transfered ownership
        CleanupStack::Pop( itemsInChoiceList );
        
        // Set hwrbox's rect to engine and set guiding line to hwrbox
        TBool isDisplayTopGuideLine = ( iEngine->CurrentLanguageScript() 
                                 == EMainViewSubmenuHebrew ) ? ETrue : EFalse;
        
        iHwrBox->SetDisplayTopGuideLine( isDisplayTopGuideLine );

        // Get the ative character range's ID.
        TInt rangeId = engine->GetCurrentCharacterRange();        
        // Handle char range changed with index 0
        HandleCharRangeChangedL( 0, rangeId );
        }
    }

// ---------------------------------------------------------------------------
// Handles character range changed.
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::HandleCharRangeChangedL( TInt aIndex, 
                                                      TInt aRangeId )
    {
    // Get the button's resourceID by char range's ID
    TInt buttonResoureId  = ButtonResourceId( aRangeId );
    // Create new button with the reourceID
    // and add it to the AknChoiceList
    if ( buttonResoureId )
        {       
        CAknButton* rangeButton = CreateButtonL( buttonResoureId, 
                                               this, NULL );
        iCurChoiceListBtn = rangeButton;
        CleanupStack::PushL( rangeButton );
        iChoicelist->SetButtonL( rangeButton );
        CleanupStack::Pop( rangeButton );
        iChoicelist->DrawNow();
        }   
    // Set choiclist' selected item to index.
    iChoicelist->SetSelectedIndex( aIndex );
    
    // Refresh the active char with the first char in the character set
    const HBufC* charSet = iEngine->CharacterSet();
    ASSERT( charSet );
    SetCurrentCharL( charSet->Mid( 0, 1 ) );     
    }
    
// ---------------------------------------------------------------------------
// Handles choosing an active char from SCT.
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::HandleChooseCharEventL()
    {
    TInt rangeId = 
        iAppUi->HwrEngine()->GetCurrentCharacterRange();                        
    
    // Get character set id by character range id
    TInt characterSetId = SctCharacterSetId( rangeId );
        
    // Popup SCT dialog
    TBuf<KSctBufferSize> selectedStr;
    TInt buttonId = PopupSctDialogL( EAknSCTLowerCase, 
                                     selectedStr, 
                                     characterSetId );
    if ( buttonId != 0 && selectedStr.Length() > 0 )
        {
        SetCurrentCharL( selectedStr.Mid( 0, 1 ) );
        }   
    }
            
// ---------------------------------------------------------------------------
// Check if clear button has been dimmed.
// ---------------------------------------------------------------------------
//
TBool CTruiMainViewContainer::IsButtonDimmed()
    {
    return iClearBtn ? iClearBtn->IsDimmed() : EFalse;
    }

// ---------------------------------------------------------------------------
// Check if all delete button has been dimmed.
// ---------------------------------------------------------------------------
//
TBool CTruiMainViewContainer::IsAllDelete()
    {
    const HBufC* charSet = iEngine->CharacterSet();
    for ( TInt i = 0; i < charSet->Length(); i++ )
        {
        if ( iEngine->CheckCharacterModel( charSet->Mid( i, 1 ) ) )
            {
            return EFalse;
            }
        }  
    return ETrue;
    }

// ---------------------------------------------------------------------------
// Return button's resourceId by Character Range Id.
// Use this resoueceId to create the correct button.
// ---------------------------------------------------------------------------
//
TInt CTruiMainViewContainer::ButtonResourceId( TInt aRangeId ) const
    {
    TInt resoureId;
    switch ( aRangeId )
        {
        case ELatinLowerLetters:
            {
            resoureId = R_MAINVIEW_BUTTON_CHARRANGE_LATIN_LOWER;
            }
            break;
        case ELatinUpperLetters:
            {
            resoureId = R_MAINVIEW_BUTTON_CHARRANGE_LATIN_UPPER;
            }        
            break;
        case ELatinAccent:
            {
            resoureId = R_MAINVIEW_BUTTON_CHARRANGE_LATIN_ACCENT;
            }        
            break;
        case ELatinDigital:
            {
            resoureId = R_MAINVIEW_BUTTON_CHARRANGE_NUMERIC;
            }
            break;
        case ELatinSymbol:
            {
            resoureId = R_MAINVIEW_BUTTON_CHARRANGE_SYMBOL;            
            }
            break;
        case ECyrillicLowerLetters:
            { //
            resoureId = R_MAINVIEW_BUTTON_CHARRANGE_CYRILLIC_LOWER;            
            }
            break;
        case ECyrillicUpperLetters:
            {
            resoureId = R_MAINVIEW_BUTTON_CHARRANGE_CYRILLIC_UPPER;            
            }            
            break;
        case ECyrillicAccent:
            {
            resoureId = R_MAINVIEW_BUTTON_CHARRANGE_CYRILLIC_ACCENT;
            }        
            break;
        case EGreekLowerLetters:
            {
            resoureId = R_MAINVIEW_BUTTON_CHARRANGE_GREEK_LOWER;            
            }            
            break;
        case EGreekUpperLetters:
            {
            resoureId = R_MAINVIEW_BUTTON_CHARRANGE_GREEK_UPPER;            
            }            
            break;
        case EHebrewLetters:
            {
            resoureId = R_MAINVIEW_BUTTON_CHARRANGE_HEBREW_LETTERS;
            }
            break;
        default:
            {
            resoureId = 0;
            }            
        }
    return resoureId;        
    }
    
// ---------------------------------------------------------------------------
// Pop Sct dialog.
// ---------------------------------------------------------------------------
//
TInt CTruiMainViewContainer::PopupSctDialogL( TInt aCharCase, 
                                              TDes& aSpecialChars, 
                                              TInt aCharSetResourceId )
    {    
    CAknCharMapDialog* sct = new (ELeave) CAknCharMapDialog( aCharCase, 
                                           aSpecialChars, 
                                           aCharSetResourceId );    
    TInt keyboardMode = CurrentKeyBoardModeL();     
    if ( keyboardMode )
        {
        // QWERTY: Lock numeric keys.
        sct->LockNumericKeys( ETrue );
        }
    else
        {
        // ITU-T: Unlock numeric keys.
        sct->LockNumericKeys( EFalse );
        }    

    sct->DisableRecentCharsRow();
    TInt retValue = sct->ExecuteLD( R_TRUI_SCT_DIALOG );
    return retValue;
    }

// ---------------------------------------------------------------------------
// Set current trained char
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::SetCurrentCharL( const TPtrC& aChar )
    {
    //If previous char's model is not saved, save it before setting new char.
    if ( iHwrBox->BoxState() == CTruiHwrBox::ENewModelDraw )
        {
        //Call save function from hwrbox,
        //in order to stop periodics in the hwrbox
        iHwrBox->SaveModelByManuallyL();
        }
    // In case of new localization not check-in, temporary    
    if ( iCurrentChar != aChar )
        {
        //Set current char
        iCurrentChar = aChar;
        //Set Top Label's text
 	    HBufC* teachCharacter = NULL;
        teachCharacter = StringLoader::LoadLC
                          ( R_TRUI_MAINVIEW_TEXT_TEACH_CHARACTER, aChar ); 	        
        if ( !iLabeltext )
            {
            delete iLabeltext;
            iLabeltext = NULL;
            iLabeltext = HBufC::NewL( teachCharacter->Length() + KExtraSize );
            }
        TPtr labelOldPtr = iLabeltext->Des();
        if ( labelOldPtr.MaxLength() < teachCharacter->Length() )
            {
            iLabeltext = iLabeltext->ReAlloc( teachCharacter->Length() + KExtraSize );            
            }
        TPtr labelTextPtr = iLabeltext->Des();
        labelTextPtr.Copy( *teachCharacter );
        CleanupStack::PopAndDestroy( teachCharacter );
                
        delete iWrappedLabelText;
        iWrappedLabelText = NULL;
        iWrappedLabelText = HBufC::NewL( iLabeltext->Length() 
                                         + iLineWidthArray.Count() 
                                         + KExtraSize );
        // Be ready to show label
        const CFont* font = AknLayoutUtils::FontFromId( iMultilineLayout.FontId() );
        TRAPD( err, WrapLabelTextL( font, *iLabeltext, iLineWidthArray, iWrappedLabelText ) );
        if ( err == KErrNone )
            {
            iLabel->SetTextL( *iWrappedLabelText );
            }
        else
            {
            iLabel->SetTextL( *iLabeltext );
            }
        AknLayoutUtils::LayoutLabel( iLabel, 
                                     iLabelPaneRect, 
                                     iMultilineLayout );
        TRgb labelColor = KRgbBlack;
        if ( GetCachedLabelTextColor( labelColor ) == KErrNone )
            {
            iLabel->OverrideColorL( EColorLabelText, labelColor );
            }
    
        //Set Indicator's text
        const HBufC* charIndicator = iEikonEnv->AllocReadResourceL
                                      ( R_TRUI_MAINVIEW_ACTIVE_CHAR_INDICATOR );
        TBuf<KIndicatorTextLen> indicatortext;
        indicatortext.Format( *charIndicator, &aChar );
        delete charIndicator;
        charIndicator = NULL;
        iIindicator->SetTextL( indicatortext );
        }

    iHwrBox->ClearExistModel();
    //Load existing model and dim clear button
    if ( iEngine->CheckCharacterModel( iCurrentChar ) )
        {
        //Get model from engine
        TRAPD( err, iEngine->GetCharacterModelL( iCurrentChar, iHwrBox->Model() ) );
        if ( err == KErrNone )
            {
            iHwrBox->ModelIsReady();
            //Activate clear button and preview button
            if ( iClearBtn )
                {
                iClearBtn->SetDimmed( EFalse );
                iClearBtn->DrawNow();
                }
            if ( iPreviewBtn )
                {
                iPreviewBtn->SetDimmed( EFalse );
                iPreviewBtn->DrawNow();
                }
            }
        //Preview    
        iHwrBox->PlayAnimation( EFalse );
        }
    else
        {
        iHwrBox->ModelIsReady( EFalse );
        if ( iClearBtn )
            {
            //Dim clear button and preview button       
            iClearBtn->SetDimmed( ETrue );
            iClearBtn->DrawNow();
            }
        if ( iPreviewBtn )
            {
            iPreviewBtn->SetDimmed( ETrue );
            iPreviewBtn->DrawNow();            
            }
        } 
    }
    
// ---------------------------------------------------------------------------
// Preview character model
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::PreviewCharacterModel()
    {
    if ( iEngine->CheckCharacterModel( iCurrentChar ) )
        {
        iHwrBox->PlayAnimation( EFalse );
        }
    }

// ---------------------------------------------------------------------------
// Clear current model
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::ClearCurrentModel()
    {
    if ( iClearBtn )
        {
        iClearBtn->SetDimmed( ETrue );
        iClearBtn->DrawNow();
        }
    if ( iPreviewBtn )
        {
        iPreviewBtn->SetDimmed( ETrue );
        iPreviewBtn->DrawNow();        
        }
    iHwrBox->ClearExistModel();
    }
    
// ---------------------------------------------------------------------------
// Popup information note
// ---------------------------------------------------------------------------
//    
void CTruiMainViewContainer::PopupInformationNoteL( const TDesC& aText )
    {         
    CAknInformationNote* note = new (ELeave) CAknInformationNote( ETrue );
    note->ExecuteLD( aText );
    }
// ---------------------------------------------------------------------------
// Adjust special behavior characters to displayable characters.
// ---------------------------------------------------------------------------
// 
void CTruiMainViewContainer::AdjustDisplayChars( TDes& aResult )
    {
    if ( aResult.Length() > 0 && aResult.Length() <= 1 )
        {
        switch ( aResult[0] )
            {
            case EKeyEnter:
                {
                aResult.Replace( 0, 1, KDisplayEnter );
                break;
                }
            case EKeySpace:
                {
                aResult.Replace( 0, 1, KDisplaySpace );
                break;
                }
            case EKeyBackspace:
                {
                aResult.Replace( 0, 1, KDisplayBackspace );
                break;
                }
            }
        }
    }

// ---------------------------------------------------------------------------
// Delete model of the shortcut
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::DeleteModelL()
    {
    // Delete shortcut from engine
    if ( iEngine->DeleteCharacterModel( *iIindicator->Text() )
        == KErrNone )
        {
        ClearCurrentModel();

        // Pop tooltip        
        HBufC* tooltipText = 
                     iEikonEnv->AllocReadResourceLC( R_TRUI_TOOLTIP_DELETED );
        TPoint hwrPos = iHwrBox->PositionRelativeToScreen();
        iHwrBox->ShowTooltipL( *tooltipText, 
                      hwrPos + TPoint( iHwrBox->Size().iWidth, KHwrTooltipY ),
                      KTooltipShowDelay, KTooltipHideDelay );        
        CleanupStack::PopAndDestroy( tooltipText );
        }
    }

// ---------------------------------------------------------------------------
// Delete all models of current character
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::DeleteAllModelL()
    {
    // Popup note dialog
    CAknQueryDialog* dlg = CAknQueryDialog::NewL();
    if ( dlg->ExecuteLD( R_TRUI_QUERYDIALOG_DELETE_ALL_MODELS ) )
        {
        // press yes 
        const HBufC* charSet = iEngine->CharacterSet();
        for ( TInt i = 0; i < charSet->Length(); i++ )
            {
            if ( iEngine->CheckCharacterModel( charSet->Mid( i, 1 ) ) )
                {
                if ( *iIindicator->Text() == charSet->Mid( i, 1 ) )
                    {
                    DeleteModelL();
                    }
                else
                    {
                    iEngine->DeleteCharacterModel( charSet->Mid( i, 1 ) );
                    }
                }
            }    
        }    
    }
    
// ---------------------------------------------------------------------------
// Get character set Id, 
// used to choose which characters will be displayed in SCT
// ---------------------------------------------------------------------------
//
TInt CTruiMainViewContainer::SctCharacterSetId( TInt aRangeId )
    {
    TInt characterSetId;
    switch ( aRangeId )
        {
        case EMainViewSubmenuLatinLowerLetter:
            {
            characterSetId = R_TRUI_SCT_CHARSET_LATIN_LOWER;
            break;
            }            
        case EMainViewSubmenuCyrillicLowerLetters:
            {
            characterSetId = SctCyrillicLowerCharacterSetId
                                            ( iEngine->InputLanguage() );
            break;
            }
        case EMainViewSubmenuGreekLowerLetters:
            {
            characterSetId = R_TRUI_SCT_CHARSET_GREEK_LOWER;
            break;
            }        
        case EMainViewSubmenuHebrewLetters:
            {
            characterSetId = R_TRUI_SCT_CHARSET_HEBREW_LETTERS;
            break;
            }        
        case EMainViewSubmenuLatinAccent:
            {
            characterSetId = R_TRUI_SCT_CHARSET_LATIN_ACCENT;
            break;
            }            
        case EMainViewSubmenuLatinUpperLetter:
            {            
            characterSetId = R_TRUI_SCT_CHARSET_LATIN_UPPER;
            break;            
            }
        case EMainViewSubmenuCyrillicUpperLetters:
            {            
            characterSetId = SctCyrillicUpperCharacterSetId
                                           ( iEngine->InputLanguage() );
            break;
            }
        case EMainViewSubmenuCyrillicAccent:
            {
            characterSetId = R_TRUI_SCT_CHARSET_CYRILLIC_ACCENT;
            break;
            }                    
        case EMainViewSubmenuGreekUpperLetters:
            {
            characterSetId = R_TRUI_SCT_CHARSET_GREEK_UPPER;
            break;
            }        
        case EMainViewSubmenuDigital:
            {
            characterSetId = R_TRUI_SCT_CHARSET_LATIN_NUMERIC;
            break;
            }
        case EMainViewSubmenuSymbols:
            {
            if ( FeatureManager::FeatureSupported(KFeatureIdChinese) )
                {
                characterSetId = R_TRUI_SCT_CHARSET_LATIN_SYMBOL_CHINESE;
                }
            else
                {
                characterSetId = R_TRUI_SCT_CHARSET_LATIN_SYMBOL;
                }
            break;
            }
        default:
            {
            characterSetId = 0;
            }
        }
    return characterSetId;        
    }

// ---------------------------------------------------------------------------
// Get cyrillic lower character set Id.
// ---------------------------------------------------------------------------
//
TInt CTruiMainViewContainer::SctCyrillicLowerCharacterSetId
                                                  ( TLanguage aInputLanguage )
    {
    if ( aInputLanguage == ELangBulgarian )
        {
        return R_TRUI_SCT_CHARSET_CYRILLIC_LOWER_BULGARIAN;
        }
    else if ( aInputLanguage == ELangRussian )
        {
        return R_TRUI_SCT_CHARSET_CYRILLIC_LOWER_RUSSIAN;
        }
    else if ( aInputLanguage == ELangUkrainian )
        {
        return R_TRUI_SCT_CHARSET_CYRILLIC_LOWER_UKRAINE;
        }
    else
        {
        return R_TRUI_SCT_CHARSET_CYRILLIC_LOWER;
        }    
    }

// ---------------------------------------------------------------------------
// Get cyrillic upper character set Id.
// ---------------------------------------------------------------------------
//
TInt CTruiMainViewContainer::SctCyrillicUpperCharacterSetId
                                                  ( TLanguage aInputLanguage )
    {
    if ( aInputLanguage == ELangBulgarian )
        {
        return R_TRUI_SCT_CHARSET_CYRILLIC_UPPER_BULGARIAN;
        }
    else if ( aInputLanguage == ELangRussian )
        {
        return R_TRUI_SCT_CHARSET_CYRILLIC_UPPER_RUSSIAN;
        }
    else if ( aInputLanguage == ELangUkrainian )
        {
        return R_TRUI_SCT_CHARSET_CYRILLIC_UPPER_UKRAINE;
        }
    else
        {
        return R_TRUI_SCT_CHARSET_CYRILLIC_UPPER;
        }    
    }

// ---------------------------------------------------------------------------
// Before exit, do some necessary operation
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::PrepareToExitL()
    {
    //If previous char's model is not saved, save it before exit.
    if ( iHwrBox->BoxState() == CTruiHwrBox::ENewModelDraw )
        {
        //Call save function from hwrbox,
        //in order to stop periodics in the hwrbox
        iHwrBox->SaveModelByManuallyL();
        }
    iHwrBox->HideInfoPopupNote();
    }

// ---------------------------------------------------------------------------
// This is called whenever the control gains or loses focus, 
// ---------------------------------------------------------------------------
// 
void CTruiMainViewContainer::FocusChanged( TDrawNow /*aDrawNow*/ )
    {    
    if ( IsFocused() )
        {        
        if ( iHwrBox->BoxState() == CTruiHwrBox::EExistModelStop )
            {
            iHwrBox->ResumePlaying();
            }
        }
    else
        {
        if ( iHwrBox->BoxState() == CTruiHwrBox::EExistModelDraw )
            {
            iHwrBox->PausePlaying();
            }
        }
    iHwrBox->SetFocus( IsFocused() );
    }
        
// ---------------------------------------------------------------------------
// Gets the control's help context. Returns a NULL context by default.
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::GetHelpContext( TCoeHelpContext& aContext ) const
    {    
    aContext.iMajor = KUidtruiApp;
    aContext.iContext = HWRT_HLP_MAIN_VIEW;
    }

// ---------------------------------------------------------------------------
// Handle size changed.
// ---------------------------------------------------------------------------
//
void CTruiMainViewContainer::DoSizeChanged( TInt aVarity )
    {    
    TRect rect = Rect();
    
    TAknWindowComponentLayout main_pane_layout = 
                            AknLayoutScalable_Apps::main_hwr_training_pane();
    TAknLayoutRect main_pane_layout_rect;
    main_pane_layout_rect.LayoutRect( rect, main_pane_layout );
    TRect main_pane_rect = main_pane_layout_rect.Rect();
    
    // Caculate hwr_training_navi_pane
    TAknWindowComponentLayout navi_pane_layout = 
               AknLayoutScalable_Apps::hwr_training_navi_pane( aVarity );
    TAknLayoutRect navi_pane_layout_rect;
    navi_pane_layout_rect.LayoutRect( main_pane_rect, navi_pane_layout );
    TRect navi_pane_rect = navi_pane_layout_rect.Rect();
    // Caculate navi pane contents
    // hwr_training_navi_pane_g1
    TAknWindowComponentLayout navi_pane_g1_layout = 
               AknLayoutScalable_Apps::hwr_training_navi_pane_g1
                                                  ( KVarityNaviPaneContents );
    TAknLayoutRect navi_pane_g1_layout_rect;
    navi_pane_g1_layout_rect.LayoutRect( navi_pane_rect, navi_pane_g1_layout );    
    TRect choicelistRect = navi_pane_g1_layout_rect.Rect();    
    iChoicelist->SetRect( choicelistRect );
    
    // hwr_training_navi_pane_g2
    // hwr_training_navi_pane_g3
    TAknWindowComponentLayout navi_pane_g3_layout = 
               AknLayoutScalable_Apps::hwr_training_navi_pane_g3
                                                  ( KVarityNaviPaneContents );       
    TAknLayoutRect navi_pane_g3_layout_rect;
    navi_pane_g3_layout_rect.LayoutRect( navi_pane_rect, navi_pane_g3_layout );    
    iLeftBtn->SetRect( navi_pane_g3_layout_rect.Rect() );
    // hwr_training_navi_pane_g4
    TAknWindowComponentLayout navi_pane_g4_layout = 
               AknLayoutScalable_Apps::hwr_training_navi_pane_g4
                                                  ( KVarityNaviPaneContents );       
    TAknLayoutRect navi_pane_g4_layout_rect;
    navi_pane_g4_layout_rect.LayoutRect( navi_pane_rect, navi_pane_g4_layout );
    iRightBtn->SetRect( navi_pane_g4_layout_rect.Rect() );
    // hwr_training_navi_pane_g5
    TAknWindowComponentLayout navi_pane_g5_layout = 
               AknLayoutScalable_Apps::hwr_training_navi_pane_g5
                                                  ( KVarityNaviPaneContents );
    TAknLayoutRect navi_pane_g5_layout_rect;
    navi_pane_g5_layout_rect.LayoutRect( navi_pane_rect, navi_pane_g5_layout );
    TRect indicator_rect = navi_pane_g5_layout_rect.Rect();
    iIindicator->SetRect( indicator_rect );
    // hwr_training_navi_pane_t1    
    TAknTextComponentLayout nave_pane_t1_layout = 
               AknLayoutScalable_Apps::hwr_training_navi_pane_t1
                                                  ( KVarityNaviPaneContents );
    TAknLayoutText navi_pane_t1_layout_text;
    navi_pane_t1_layout_text.LayoutText( indicator_rect, nave_pane_t1_layout );
    iIindicator->SetFont( navi_pane_t1_layout_text.Font() );
    iIindicator->SetLabelAlignment( navi_pane_t1_layout_text.Align() );
    
    // hwr_training_write_pane
    TAknWindowComponentLayout write_pane_layout = 
               AknLayoutScalable_Apps::hwr_training_write_pane( aVarity );       
    TAknLayoutRect write_pane_layout_rect;
    write_pane_layout_rect.LayoutRect( main_pane_rect, write_pane_layout );
    iHwrBox->SetRect( write_pane_layout_rect.Rect() );
    
    TPoint hwrPos = iHwrBox->PositionRelativeToScreen();
    iHwrBox->SetTooltipPosition( hwrPos + TPoint( iHwrBox->Size().iWidth, KHwrTooltipY ) );
    
    // hwr_training_instruct_pane
    TAknWindowComponentLayout instruct_pane_layout = 
               AknLayoutScalable_Apps::hwr_training_instruct_pane( aVarity );       
    TAknLayoutRect instruct_pane_layout_rect;
    instruct_pane_layout_rect.LayoutRect( main_pane_rect, instruct_pane_layout );    
    iLabelPaneRect = instruct_pane_layout_rect.Rect();
    
    // count of text rows
    TInt count = AknLayoutScalable_Apps::
                 list_single_hwr_training_instruct_pane_ParamLimits().LastRow() + 1;

    // array contains all text rows in help screen
    RArray<TAknTextComponentLayout> textLayoutArray; 
    iLineWidthArray.Reset();
    for( TInt i = 0; i < count; i++ )
        {
        // pane area for each row
        TAknWindowComponentLayout each_row_layout =  
            AknLayoutScalable_Apps::list_single_hwr_training_instruct_pane( 0, 0, i );
        TAknLayoutRect each_row_layout_rect;
        each_row_layout_rect.LayoutRect( iLabelPaneRect, 
                                         each_row_layout );
        iLineWidthArray.Append( each_row_layout_rect.Rect().Width() );
        // smaller pane for text itself
        TAknTextComponentLayout each_row_text_layout =  
            AknLayoutScalable_Apps::list_single_hwr_training_instruct_pane_t1();
             
        // composition of two latter componets
        TAknTextComponentLayout textCompCompositionWiz = 
            TAknWindowComponentLayout::ComposeText(
                each_row_layout, each_row_text_layout );
 
        TRAPD(error, textLayoutArray.AppendL( textCompCompositionWiz ) );
        // if error in append function, then just exit the loop      
        if ( error != KErrNone )
            {
            
            break;
            }
        }       
        
    iMultilineLayout = TAknTextComponentLayout::Multiline( textLayoutArray );        
    if ( iLabeltext && iWrappedLabelText )
        {
        // Be ready to show label
        TAknLayoutText multilineTextLayout;
        multilineTextLayout.LayoutText( iLabelPaneRect, iMultilineLayout );    
        const CFont* font = AknLayoutUtils::FontFromId( iMultilineLayout.FontId() );
        TRAPD( err, WrapLabelTextL( font, *iLabeltext, iLineWidthArray, iWrappedLabelText ) );
        if ( err == KErrNone )
            {
            TRAP_IGNORE( iLabel->SetTextL( *iWrappedLabelText ) );
            }
        else
            {
            TRAP_IGNORE( iLabel->SetTextL( *iLabeltext ) );
            }        
        AknLayoutUtils::LayoutLabel( iLabel, 
                                     iLabelPaneRect, 
                                     iMultilineLayout );
        }
    textLayoutArray.Close();
    
    TRgb labelColor = KRgbBlack;
    TInt error = GetCachedLabelTextColor( labelColor );
    if ( error == KErrNone )
        {
        TRAP_IGNORE( iLabel->OverrideColorL( EColorLabelText, labelColor ) );
        TRAP_IGNORE( iIindicator->OverrideColorL( EColorLabelText, labelColor ) );
        }
    }
    
void CTruiMainViewContainer::HandlePointerEventL(const TPointerEvent& aPointerEvent)
    {    
    if ( iHwrBox->BoxState() == CTruiHwrBox::ENewModelDraw )
        {
        //Call save function from hwrbox,
        //in order to stop periodics in the hwrbox
        iHwrBox->HandlePointerEventL( aPointerEvent );       
        }
    else
        {
        CCoeControl::HandlePointerEventL( aPointerEvent );
        }    
    }

