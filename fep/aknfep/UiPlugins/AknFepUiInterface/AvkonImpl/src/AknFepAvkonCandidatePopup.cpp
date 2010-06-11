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
* Description:             Avkon implementation of non-focusing popup menu to show word candidates.
*
*/













#include <eikenv.h>
#include <eikappui.h>
#include <badesca.h>
#include <coemain.h>
#include <aknPopupHeadingPane.h>
#include <aknsfld.h>
#include <AknsFrameBackgroundControlContext.h>
#include <aknlists.h>
#include <aknmessagequerycontrol.h> 
#include <skinlayout.cdl.h>
#include <AknLayout2ScalableDef.h>
#include <aknlayoutscalable_avkon.cdl.h>
#include <layoutmetadata.cdl.h>
#include <aknlayoutscalable_apps.cdl.h>
#include <AknFepInternalCRKeys.h>
#include <AvkonInternalCRKeys.h>

#include <aknfep.rsg>
#include <AknFepGlobalEnums.h>
#include <avkon.hrh>
#include "AknFepAvkonCandidatePopup.h"
#include "AknPriv.hrh"
const TInt KChrKeyTimeout = 250000;
const TInt KChrKeyRepeatDelay = 1000000;

const TInt KScreenWidthQHDLandscape = 640;
const TInt KScreenHeightQHDLandscape = 360;
const TInt KOffsetWidthForCandidatePopup = 32;

/**
* Creates the pop-up list
*
*/
CAknFepAvkonCandidatePopup* CAknFepAvkonCandidatePopup::NewL( MAknFepCandidatePopupCallback& aOwner )
    {
    CAknFepAvkonCandidatePopup* self = new(ELeave)CAknFepAvkonCandidatePopup( aOwner );        
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;
    }


/**
* C++ constructor
*
* @param    aOwner  The owner of the popup list. Provides the candidates and receives notification
*                   when candidate selected.
*
*/
CAknFepAvkonCandidatePopup::CAknFepAvkonCandidatePopup( MAknFepCandidatePopupCallback& aOwner )
    : CAknPopupList()
    , iOwner(aOwner)
    , iKeyboard(EPtiKeyboardNone)
    {
        
    }
    
    
/**
* 2nd phase construction
*
*/
void CAknFepAvkonCandidatePopup::ConstructL()
    {
    // Construct the inner listbox and the base class
   	iList = new( ELeave ) CAknSinglePopupMenuStyleListBox;
   	TInt primaryCandidate = 0;
    
    CRepository* aknFepRepository = NULL;
    aknFepRepository = CRepository::NewL(KCRUidAknFep);
    if(aknFepRepository)
    	{
       	aknFepRepository->Get(KAknFepPrimaryCandidateFlag, primaryCandidate);
        delete aknFepRepository;
        }
    
    TInt keyboardLayout = 0;
    RProperty::Get(KCRUidAvkon, KAknKeyBoardLayout, keyboardLayout);
    TPtiKeyboardType layout = (TPtiKeyboardType)keyboardLayout;
    
	// Not use embedded CBA, so new the CBA seperately
    TInt cbaResource = 0;
    if(primaryCandidate && layout != EPtiKeyboard12Key  )
    	{
    	CAknPopupList::ConstructL( iList,R_AKNFEP_SOFTKEYS_OK_CANCEL_SELECT,
    						   AknPopupLayouts::EMenuWindow );
		cbaResource = R_AKNFEP_SOFTKEYS_OK_CANCEL_SELECT;
    	}
    else
    	{
    	CAknPopupList::ConstructL( iList, R_AKNFEP_SOFTKEYS_SPELL_CANCEL_SELECT,
    						   AknPopupLayouts::EMenuWindow );  
	    cbaResource = R_AKNFEP_SOFTKEYS_SPELL_CANCEL_SELECT;
    	}
	// To add new CBA, here set its flag as the base class, 
	// but just not set Embedded flag.
	TUint flags = CEikButtonGroupContainer::EAddToStack;
    if ( AknLayoutUtils::PenEnabled() )
        {
        flags |= CEikButtonGroupContainer::EDelayActivation;
        }
	// Destroy the CBA constructed in base class, and new its own CBA
    delete iPopoutCba;
    iPopoutCba =  CEikButtonGroupContainer::NewL(
            CEikButtonGroupContainer::ECba,
            CEikButtonGroupContainer::EHorizontal,
            this, cbaResource, *this, flags );	

	iList->ConstructL( this, CEikListBox::ELeftDownInViewRect );
    iList->CreateScrollBarFrameL( ETrue );
    iList->ScrollBarFrame()->SetScrollBarVisibilityL( CEikScrollBarFrame::EOff,      //< horizontal
                                                      CEikScrollBarFrame::EAuto );   //< vertical
    iList->SetNonFocusing();


    // Construct the item array
    iCandidates = new (ELeave) CDesCArrayFlat( KPredictiveListCandidateMax ); 
    CTextListBoxModel* model = iList->Model();
    model->SetItemTextArray( iCandidates );
    model->SetOwnershipType( ELbmDoesNotOwnItemArray );
    iChrKeypressMonitor = CPeriodic::NewL(CActive::EPriorityStandard);
    SetNonFocusing();
    MakeVisible(EFalse); 
    }
    

MAknFepCandidatePopup::~MAknFepCandidatePopup()
{
	
}

/**
* Destructor
*
*/
CAknFepAvkonCandidatePopup::~CAknFepAvkonCandidatePopup()
    {
    delete iList;
    delete iCandidates;
    if( iChrKeypressMonitor )
        {
        iChrKeypressMonitor->Cancel();
        delete iChrKeypressMonitor;
        iChrKeypressMonitor = NULL;
        }
    }
 

TPtrC CAknFepAvkonCandidatePopup::ActiveWord() const
    {
    if ( iCandidates->Count()>*iSelectedIdx && *iSelectedIdx>0 )
        {
        return (*iCandidates)[*iSelectedIdx];    
        }
    else
        {
        return TPtrC( KNullDesC );
        }
    }
    
    
TPtrC CAknFepAvkonCandidatePopup::ExactWord() const
    {
    if ( iCandidates->Count() > 0 )
        {
        return (*iCandidates)[0];        
        }
    else
        {
        return TPtrC( KNullDesC );
        }
    }
 
 
/**
* Makes the pop-up selection list visible.
*
* @param        aInlineEditorRect   Tells the place of the inline editor. If possible, the popup is opened
*                                   so that it does not hide the inline editor.
*
* @param        aSelectedIdx        Input/output argument for the selected index. If legal index is given in,
*                                   it will be used as default selection. Otherwise the default selection will
*                                   be the currently active word. On succesful exit this variable contains
*                                   the index selected by the user.
*
* @param        aLastKeyEvent       The last key event received by the popup. On return this is the event
*                                   which closed the popup.
*               
* @return       The command id used to close the window. EAknFepSoftkeySpell, EAknSoftkeyCancel, or EAknSoftkeySelect
*/
TInt CAknFepAvkonCandidatePopup::ExecutePopupL( const TRect& aInlineEditorRect, TInt& aSelectedIdx,
												TKeyEvent& aLastKeyEvent, TBool aRightToLeftLanguage,
												TInt aKeyboard )
    {
    iSelectedIdx = &aSelectedIdx;
    iLastKeyEvent = &aLastKeyEvent;
    iKeyboard = (TPtiKeyboardType)aKeyboard;
    //This is the inline text rectangle, this is needed in the event of a layout change
    iInlineTextRect = aInlineEditorRect;
    
    TInt requestedSelection = *iSelectedIdx;
    iOwner.GetCandidatesL( *iCandidates, *iSelectedIdx );
    if ( requestedSelection >= 0 && requestedSelection < iCandidates->Count() )
        {
        *iSelectedIdx = requestedSelection;
        }
    
    iListBox->SetCurrentItemIndex( *iSelectedIdx );
    
    iRightToLeftCandidate = aRightToLeftLanguage;
    
    const TSize screenSize = iAvkonAppUi->ApplicationRect().Size(); //TSize(AKN_LAYOUT_WINDOW_screen.iW,AKN_LAYOUT_WINDOW_screen.iH);
    iPopoutCba->SetBoundingRect(TRect(screenSize));

    SetupWindowLayout(iWindowType);
    SetupWindowLocation( aInlineEditorRect );

    iListBox->SetListBoxObserver(this);        

    iEikonEnv->RemoveFromStack(this);
    iEikonEnv->EikAppUi()->AddToStackL(this,ECoeStackPriorityDialog/*ECoeStackPriorityFep*/);


    ActivateL();
    // this is required here to make code like
    // iList->SetCurrentItemIndex( last item of the list );
    // iPopupList->ExecuteLD();
    // to work as it used to. Without this current item of the
    // list would be topmost item, and there would be unused empty
    // space below that.
    iListBox->UpdateScrollBarsL();

    // The main rule is to make the first index (the exact match) visible. However, the active index should never
    // be hidden.
    iListBox->ScrollToMakeItemVisible( 0 );
    if ( iListBox->BottomItemIndex() < *iSelectedIdx )
        {
        iListBox->ScrollToMakeItemVisible( *iSelectedIdx );
        }
        
    // Ensure that the popup is on top. Without this the popup window is left in the background at least
    // in the dialogs of the Phonebook application.
    //Window().SetOrdinalPosition(0);
    //iPopoutCba->SetContainerWindowL( Window() ); this didn't help
        
    // Make the popup visible
    iPopoutCba->MakeVisible(ETrue);
    iListBox->MakeVisible(ETrue);    
    MakeVisible(ETrue);
    SetFocus(ETrue);
    FadeBehindPopup(EFalse);
    TInt returnValue;
    iReturn = &returnValue;
    iLastCommandId = EAknSoftkeySelect;
    iWait.Start();
    return returnValue;
    }


/**
* Called when the popup is closed. Unlike the base class, this class does not commit suicide on this situation.
*
* @return        ETrue if the popup was accepted. EFalse if the popup was cancelled
*/
void CAknFepAvkonCandidatePopup::AttemptExitL(TBool aAccept)
    {
    *iSelectedIdx = iList->CurrentItemIndex();

    if (iCoeEnv && iEikonEnv)
        {
        iEikonEnv->RemoveFromStack(this);
        }
      
    SetFocus(EFalse);
    MakeVisible(EFalse);
    ListBox()->MakeVisible(EFalse);    
    iPopoutCba->MakeVisible(EFalse);

    if (iReturn) //Always not null unless ExecutePopupL leaves
        {
        if (!aAccept)
            {
            *iReturn = EAknSoftkeyCancel;
            }
        else
            {
            *iReturn = iLastCommandId;                        
            }
        }
	if(iWait.IsStarted())
    iWait.AsyncStop();
    }


void CAknFepAvkonCandidatePopup::HandleResourceChange(TInt aType)
    {
    CAknPopupList::HandleResourceChange(aType);
    //When there is a dynamic layout change, the candidate list position needs to be aligned with
    //the new position of the inline text.
    if(aType == KEikDynamicLayoutVariantSwitch)
        {
        // Move back candidate popup control priority.
        iEikonEnv->RemoveFromStack(this);        
        TRAP_IGNORE( iEikonEnv->EikAppUi()->AddToStackL(this, ECoeStackPriorityDialog )) ;       
        // Get candidate update position based on layout.
        TRAP_IGNORE(iOwner.GetUpdateCandidatePositionL(iInlineTextRect));

	// Ensure that the popup is on top.
	Window().SetOrdinalPosition(0); 
 
        SetupWindowLocation( iInlineTextRect);
        DrawNow();
        }
    else if(aType == KAknMessageFocusLost)
    	FadeBehindPopup(ETrue);
 
    }


void CAknFepAvkonCandidatePopup::UnFocus()
    {
    //Remove the candidate list from the control stack so that it does not receive any key event.
    if (iCoeEnv && iEikonEnv)
        {
        iEikonEnv->RemoveFromStack(this);
        }
    //Un-Focus the candidate list
    SetFocus(EFalse);
    }
void CAknFepAvkonCandidatePopup::ShowAtNewPosition(TRect aRect)
    {
    if (iCoeEnv && iEikonEnv)
        {
        TRAP_IGNORE( iEikonEnv->EikAppUi()->AddToStackL(this,ECoeStackPriorityDialog/*ECoeStackPriorityFep*/ ));
        }
    SetFocus(ETrue);
    iInlineTextRect = aRect;
    SetupWindowLocation( iInlineTextRect);
    DrawNow();
    }

void CAknFepAvkonCandidatePopup::SetFocusAddStackReducePriorityL()
    {
    if (iCoeEnv && iEikonEnv)
        {
        iEikonEnv->EikAppUi()->AddToStackL(this, ECoeStackPriorityDefault-1/*ECoeStackPriorityDialog-1*/ );
        }
    SetFocus(ETrue);
    }

TInt CAknFepAvkonCandidatePopup::HandleChrKeyMonitorCallback(TAny* aParam)
    {
    // the timer will be cancelled only when the key up event is received
    // the down arrow key event is simulated now
    TKeyEvent keyEvent;
    keyEvent.iCode = EKeyDownArrow;	
//    TEventCode type = EEventKey;
    TRAP_IGNORE( ((CAknFepAvkonCandidatePopup*)aParam)->ListBox()->OfferKeyEventL(keyEvent, EEventKey) );
    // Prevent the screen saver
    User::ResetInactivityTime();
    return 1;
    }

/**
* Modified from CAknPopupList::OfferKeyEventL().
*/
TKeyResponse CAknFepAvkonCandidatePopup::OfferKeyEventL(const TKeyEvent& aKeyEvent, 
														TEventCode aType)
    {
    *iLastKeyEvent = aKeyEvent;
    
    // Selection key is substituted with the space key. Thus, selecting a candidate with the selection
    // key auto-appends space character.
    if ( aKeyEvent.iScanCode == EStdKeyDevice3 )
        {
        //iLastKeyEvent->iScanCode = EStdKeySpace;
        iLastKeyEvent->iCode = EKeyOK;
        }
    // Asterisk is substituted with arrow down key to get same functionality in ITU-T keypads
    // Emulator sends key event with scancode EStdKeyNkpAsterisk and hardware an event
    // with code '*' so we check for both
    else if ( ( (aKeyEvent.iScanCode == EStdKeyNkpAsterisk || aKeyEvent.iCode == '*' ) 
            && iKeyboard == EPtiKeyboard12Key ) )
        {
        iLastKeyEvent->iCode = EKeyDownArrow;
        }
    else if( aKeyEvent.iScanCode == EStdKeyLeftFunc
            && iKeyboard == EPtiKeyboardHalfQwerty  ) 
    	{
    	if( aType == EEventKeyDown)
    		{
    		iLastKeyEvent->iCode = EKeyDownArrow;	
    		// start the timer
    		if(iChrKeypressMonitor->IsActive())
    		    {
    		    iChrKeypressMonitor->Cancel();
    		    }
            iChrKeypressMonitor->Start( KChrKeyRepeatDelay, KChrKeyTimeout, 
                TCallBack(HandleChrKeyMonitorCallback, this));

    		aType = EEventKey;
    		}    	
    	else
    		{
    		iChrKeypressMonitor->Cancel();
    		return EKeyWasNotConsumed;
    		}
    	}
    // this must be first check, since window will be faded when fast
    // swap window is visible
    if (aType==EEventKey && aKeyEvent.iCode == EKeyEscape)
        {
        AttemptExitL(EFalse);
        return EKeyWasConsumed;
        }

    if ( Window().IsFaded() )
        {
        // this happens, when popuplist has a findbox, and user
        // presses shift to launch fep menu. Fep menu has priority menu
        // in control stack, but we have dialog priority. As result,
        // keyevents will get here first. If we return
        // EKeyWasNotConsumed, fep menu will catch those events
        // next.
        return EKeyWasNotConsumed;
        }

    TBool needRefresh = EFalse;
    TKeyResponse res = AknFind::HandleFindOfferKeyEventL(aKeyEvent, aType, this,
    													 ListBox(), FindBox(), EFalse, needRefresh);

    if (needRefresh && FindBox())
        {
        DrawNow();
        }

    if ( res == EKeyWasConsumed )
        {
        return res;
        }
 
    if (aType==EEventKey)
        {
        // Handle arrow keys based on iScancode
        TBool keyEventHandled = EFalse;
        switch (iLastKeyEvent->iScanCode)
            {
            case EStdKeyUpArrow: //fall through
            case EStdKeyDownArrow:
            			keyEventHandled = ETrue;
                 return iListBox->OfferKeyEventL(*iLastKeyEvent, aType);
                 
            case EStdKeyLeftArrow:
                    if(iRightToLeftCandidate)
                        // These keys confirm the selection and are then handled by AknFepManager
                        AttemptExitL(ETrue);
                    else
                        // These keys cancel the selection and are then handled by AknFepManager
                        AttemptExitL(EFalse);
                    keyEventHandled = ETrue;
                    return EKeyWasConsumed;
                
            case EStdKeyRightArrow:  
                    if(iRightToLeftCandidate)
                            // These keys cancel the selection and are then handled by AknFepManager
                            AttemptExitL(EFalse);
                        else
                            // These keys confirm the selection and are then handled by AknFepManager
                            AttemptExitL(ETrue);
               			keyEventHandled = ETrue;
                    return EKeyWasConsumed;

            }
        if (!keyEventHandled)
        	{
        // Handle through iCode
	        switch (iLastKeyEvent->iCode)
	            {
	
	            case EKeyUpArrow: //fall through
	            case EKeyDownArrow:
	                 return iListBox->OfferKeyEventL(*iLastKeyEvent, aType);
	                 
	            case EKeyLeftArrow:
	                    if(iRightToLeftCandidate)
	                        // These keys confirm the selection and are then handled by AknFepManager
	                        AttemptExitL(ETrue);
	                    else
	                        // These keys cancel the selection and are then handled by AknFepManager
	                        AttemptExitL(EFalse);
	                    return EKeyWasConsumed;
	                
	            case EKeyRightArrow:  
	                    if(iRightToLeftCandidate)
	                            // These keys cancel the selection and are then handled by AknFepManager
	                            AttemptExitL(EFalse);
	                        else
	                            // These keys confirm the selection and are then handled by AknFepManager
	                            AttemptExitL(ETrue);
	                    
	                        return EKeyWasConsumed;
	
	            
	            case EKeyEnter:
	            case EKeyOK:
	            case EKeySpace:
	            case EKeyTab:
	            default:
	                // These keys confirm the selection and are then handled by AknFepManager
	                AttemptExitL(ETrue);
	                return EKeyWasConsumed;                
							case	EKeyApplication:
							case	EKeyPhoneEnd:
							// Flip open close event.
				      case EKeyFlipOpen:
				      case EKeyFlipClose:            
									AttemptExitL(EFalse);
				            		return EKeyWasNotConsumed;
	   
	            }
          }              
        }
        
     // For Layout switching
     // If user switch the layout, means QWERTY to ITU-T and vice versa
     // Keyboard layout going to cahnge, So, not need to open the candidate
     // list as it is, becz its may predict differnt list of word.
        if (aKeyEvent.iScanCode == EStdKeyApplicationE ||
            aKeyEvent.iScanCode ==  EStdKeyApplication10 ||
        	aKeyEvent.iScanCode ==   EStdKeyDeviceF ||
    		aKeyEvent.iScanCode ==   EStdKeyDeviceA ||
        	aKeyEvent.iScanCode ==   EStdKeyApplication12 ||
        	aKeyEvent.iScanCode ==   EStdKeyApplication15 ||
        	aKeyEvent.iScanCode ==   EStdKeyApplication16 ||
        	aKeyEvent.iScanCode ==   EStdKeyDeviceB)
	        {
	        AttemptExitL(EFalse);
	        return EKeyWasConsumed;
	        }
 
    return EKeyWasNotConsumed;
    }


/**
* Process commands from CBA buttons.
* @param  aCommandId  The command to handle.
*/
void CAknFepAvkonCandidatePopup::ProcessCommandL(TInt aCommandId)
    {
    iLastCommandId = aCommandId;
    if (aCommandId==EAknFepSoftkeySpell 
    || aCommandId == EAknSoftkeyOk)
       {
        AttemptExitL(ETrue);
        }
    else
        {
        CAknPopupList::ProcessCommandL(aCommandId);
        }
    }


/**
* Calculates the position for the popup window and places it there
* @param    aInlineEditorRect   The placement of the inline editor is used as reference.
*                               The inline editor is not hidden if possible.
*/
void CAknFepAvkonCandidatePopup::SetupWindowLocation( const TRect& aInlineEditorRect )
    {
    TRect clientRect;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, clientRect);
    
    TRect popupRect = Rect();
    
    // 1. Is it possible to place the popup under the inline editor
    if ( aInlineEditorRect.iBr.iY + popupRect.Height() < clientRect.iBr.iY )
        {
        SetPosition( TPoint(aInlineEditorRect.iTl.iX, aInlineEditorRect.iBr.iY) );        
        }
        
    // 2. Is it possible to place the popup on the right side of the inline editor
    else if ( aInlineEditorRect.iBr.iX + popupRect.Width() < clientRect.iBr.iX )
        {
        SetPosition( TPoint(aInlineEditorRect.iBr.iX, aInlineEditorRect.iTl.iY) );
        }
        
    // 3. Is it possible to place the popup above the inline editor
    else if ( aInlineEditorRect.iTl.iY > popupRect.Height() )
        {
        SetPosition( aInlineEditorRect.iTl - TPoint(0, popupRect.Height()) );
        }
        
    // 4. Is it possible to place the popup on the left side of the inline editor
    else if ( aInlineEditorRect.iTl.iX > popupRect.Width() )
        {
        SetPosition( aInlineEditorRect.iTl - TPoint(popupRect.Width(), 0) );
        }

    // 5. If everything else fails, place the popup in the center of the screen
    else
        {
        TInt xMargins = clientRect.Width() - popupRect.Width();
        TInt yMargins = clientRect.Height() - popupRect.Height();
        SetPosition( TPoint(xMargins/2, yMargins/2) );
        }
        
        
        
    // Shift popup to left if necessary
    if ( Position().iX + popupRect.Width() > clientRect.iBr.iX )
        {
        SetPosition( TPoint(clientRect.iBr.iX-popupRect.Width(), Position().iY) );
        }

    // Shift popup upwards if necessary
    if ( Position().iY + popupRect.Height() > clientRect.iBr.iY )
        {
        SetPosition( TPoint(Position().iX, clientRect.iBr.iY-popupRect.Height()) );
        }
    }


/**
* Sets up the layout of the popup window.
* Modified from CAknPopupList::SetupWindowLayout()
*/
void CAknFepAvkonCandidatePopup::SetupWindowLayout(AknPopupLayouts::TAknPopupLayouts aType)
    {
    // A linked list for HandleSizeChanged().
    TAknPopupLayoutsNode list = { 0, EListNode, ListBox() };
    TAknPopupLayoutsNode heading = { &list, EHeadingNode, Heading() };
    TAknPopupLayoutsNode windowOwning = { &heading, EWindowOwningNode, this };
    TAknPopupLayoutsNode findPane = { &windowOwning, EFindBoxNode, FindBox() };
    TAknPopupLayoutsNode *listBegin = &findPane;
    
    HandleSizeChanged( Layout(), aType, listBegin );

    // create skin context for popuplist (list itemdrawer uses normal list skinning)
    TRect windowRect = Rect();

    TAknLayoutRect topLeft;
    topLeft.LayoutRect(windowRect, 
    				   SkinLayout::Popup_windows_skin_placing__frame_general__Line_2());

    TAknLayoutRect bottomRight;
    bottomRight.LayoutRect(windowRect,
    					   SkinLayout::Popup_windows_skin_placing__frame_general__Line_5());

    TRect outerRect = TRect(topLeft.Rect().iTl, bottomRight.Rect().iBr);
    TRect innerRect = TRect(topLeft.Rect().iBr, bottomRight.Rect().iTl);
    
    // we can safely use FormattedCellData only if normal popup layouts are in use
    switch(iWindowType)
        {
        case AknPopupLayouts::EMenuUnknownColumnWindow:
        case AknPopupLayouts::EMenuUnknownFormattedCellWindow:
            break;
        default:  
            {
            CFormattedCellListBoxData *boxData =
            	((CEikFormattedCellListBox*)ListBox())->ItemDrawer()->FormattedCellData();

            boxData->SetSkinPopupFrame(&KAknsIIDQsnFrPopup,&KAknsIIDQsnFrPopupCenter);
            boxData->SetSkinPopupFramePosition(outerRect,innerRect);
            }
            break;
        }

    //CListItemDrawer::SetItemCellSize needs to be called to set the selection bar width
    TSize cellSize = iList->ItemDrawer()->ItemCellSize();
    cellSize.iWidth = iLayout.iListRect.Rect().Width();
    iList->ItemDrawer()->SetItemCellSize( cellSize );
    }
    

/**
* Calculates the width of the popup window based on the candidates available on the list. 
* The with is always set to as small as possible without truncation. 
* The client application area is used as reference to ensure that the popup is never wider
* than the application area.
*/
TInt CAknFepAvkonCandidatePopup::CalculateWindowWidth( const TRect& aClientRect )
    {
    // Fetch the font
    const CFont* font = AknLayoutUtils::FontFromId(AKN_LAYOUT_TEXT_List_pane_texts__menu_single__Line_1(0).FontId());
	
	TInt maxTextWidth = 0;
	
	for (TInt i=0 ; i<iCandidates->Count() ; ++i)
	    {
	    TInt curTextWidth = font->TextWidthInPixels( (*iCandidates)[i] );
        if ( curTextWidth > maxTextWidth )
            {
            maxTextWidth = curTextWidth;
            }
   	    }
   	
   	TInt popupWidth = maxTextWidth + ( 2 * font->MaxCharWidthInPixels() ); 
   	
   	// Modified as per Avkon team's suggestions
   	// From Avkon:-
    // Layout data of listscroll_menu_pane are changed for CR 417-35260.
    // The change is just for QHD landscape model.
    // The CR makes listscroll_menu_pane's ir or il bigger than normal,
    // so that width of list item is smaller than needs. Then, first cell 
    // of list item can not be drawn on proper position.
    // Adjustment of layout is a solution for this problem. This is not a perfect idea, but
    // creating a new layout for popuplist is too complex to do that. Adjustment is a must.

   	if ( Layout_Meta_Data::IsLandscapeOrientation() )
        {
         const TSize screenSize = iAvkonAppUi->ApplicationRect().Size();
         
         if( KScreenWidthQHDLandscape == screenSize.iWidth && 
             KScreenHeightQHDLandscape == screenSize.iHeight )
             {
             popupWidth += KOffsetWidthForCandidatePopup;
             }
        }

   	if ( popupWidth > aClientRect.Width() )
   	    {
   	    popupWidth = aClientRect.Width();
   	    }
   	    
   	return popupWidth;
    }

    
/**
* Utility function copied from aknpopuplayout.cpp
*/
static CCoeControl *FindControl(TAknPopupLayoutsNode *aNode, TInt aId)
    {
    while(aNode)
        {
        if (aId == aNode->iId)
            {
            return aNode->iControl;
            }
        aNode = aNode -> iNext;
        }
    return NULL;
    }

/**
* Utility function copied from aknpopuplayout.cpp
*/
static TInt GetMaxListHeight()
    {
    TAknLayoutRect temp, layout;

    TRect mainPane;
    TRect statusPane;
    TRect controlPane;
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EMainPane, mainPane );
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EStatusPane, statusPane );
    AknLayoutUtils::LayoutMetricsRect( AknLayoutUtils::EControlPane, controlPane );
    
    // in landscape orientation popuplist can't go on top of statuspane
    if ( !Layout_Meta_Data::IsLandscapeOrientation() )
        {
        mainPane.iTl.iY -= statusPane.Height();
        }
    
    
    // treat mainpane+statuspane area as popup window
    // too bad we can't use this, because e.g. QVGA landscape has border-size 7
    // in avkon layout and border-size 9 in skin drawing...
	/*temp.LayoutRect( mainPane, AknLayoutScalable_Avkon::listscroll_menu_pane(0));
	layout.LayoutRect( temp.Rect(), AknLayoutScalable_Avkon::list_menu_pane(0));
    return layout.Rect().Height();*/
    	
    // shadow
    TInt varietyIndex = Layout_Meta_Data::IsLandscapeOrientation();
    
    TAknLayoutRect insideArea;
    insideArea.LayoutRect(
        mainPane,
        AknLayoutScalable_Avkon::bg_popup_window_pane_g1(varietyIndex) );
    
    return insideArea.Rect().Height();
    }
    
/**
* Modified from AknPopupLayouts::HandleSizeChanged()
*/
void CAknFepAvkonCandidatePopup::HandleSizeChanged( TAknPopupWindowLayoutDef &aDef,
                                                AknPopupLayouts::TAknPopupLayouts /*aLayout_1*/,
                                                TAknPopupLayoutsNode *aNode)
    {
    CAknPopupHeadingPane *heading = (CAknPopupHeadingPane*)FindControl(aNode, EHeadingNode);
    CEikListBox *listBox = (CEikListBox*)FindControl(aNode, EListNode);
    CCoeControl *windowOwningControl = FindControl(aNode, EWindowOwningNode);
    //CAknMessageQueryControl *msgQueryCtrl = (CAknMessageQueryControl*)FindControl(aNode, EMessageBoxNode);
    
    //TInt layout = aLayout_1;
    TInt numofitems = listBox->Model()->NumberOfItems();

    TRAP_IGNORE( listBox->View()->ItemDrawer()->SetSkinEnabledL(ETrue) );
    
    TInt maxListHeight = GetMaxListHeight();

    // position popup window's bottom correctly
    TRect clientRect;
    AknLayoutUtils::LayoutMetricsRect(AknLayoutUtils::EMainPane, clientRect);
    // set windowrect to minimum size - this will be adjusted later    
    TAknLayoutRect windowRect;
    windowRect.LayoutRect( clientRect, AknLayoutScalable_Avkon::popup_menu_window(8));
   
    aDef.iWindowRect = windowRect.Rect();
    aDef.iWindowRect.SetWidth( CalculateWindowWidth(clientRect) );
    TRAP_IGNORE( listBox->View()->ItemDrawer()->SetSkinEnabledL(ETrue) );
    
    TInt minItems = 1;
    
    TAknWindowLineLayout listLayout;
    TAknLayoutScalableParameterLimits listLimits;
    
    listLayout = AknLayoutScalable_Avkon::list_single_pane_cp2(0);
    listLimits = AknLayoutScalable_Avkon::list_single_pane_cp2_ParamLimits();

	TInt maxLayoutItems = listLimits.LastRow() + 1; // last row is a zero based index, we need num items which is 1 based
        
    TAknLayoutRect listItemRect;
	listItemRect.LayoutRect( aDef.iWindowRect, listLayout);
    TInt listItemHeight = listItemRect.Rect().Height();
    TInt maxItems = maxListHeight / listItemHeight;
    // minItems == 1 only if the popuplist is dynamically changeable
    if ( (numofitems > 1) && (minItems == 1) )
        {
        minItems = numofitems;
        }
    if (minItems > maxItems)
        {
        minItems = maxItems;
        }
    // maxItems might be greater than max items from layout -> use layout's maximum
    if (minItems > maxLayoutItems)
        {
        minItems = maxLayoutItems;
        }

    TRect window_rect = AknPopupLayouts::MenuRect(aDef);

    TAknLayoutRect temp, layout;
    temp.LayoutRect( window_rect, AknLayoutScalable_Avkon::listscroll_menu_pane(0));
	layout.LayoutRect( temp.Rect(), AknLayoutScalable_Avkon::list_menu_pane(0));
	
	
    TRect tempListRect = layout.Rect(); // this is list's rect for the whole window

    // We really don't want parent relative list layout here because findbox will be overwritten.
    // Just calculate list height and use that.
    TRect nullRect(0,0,0,0);
    listLayout.iH = (TInt16)(minItems * listItemHeight);
    listLayout.ib = ELayoutEmpty;

    aDef.iListRect.LayoutRect(tempListRect,
        listLayout);

    // we have to scale iWindowRect to list rect - layout is not (yet) correct
    TInt usedHeight = aDef.iListRect.Rect().Height();

    // popupwindow's inside area
    TInt varietyIndex = Layout_Meta_Data::IsLandscapeOrientation();
    
    TAknLayoutRect insideArea;
    insideArea.LayoutRect(
        window_rect,
        AknLayoutScalable_Avkon::bg_popup_window_pane_g1(varietyIndex) );

    if (layout.Rect().Height() < usedHeight)
        {
        aDef.iWindowRect.iTl.iY -= (usedHeight - layout.Rect().Height());
        }
    

    // now we finally know the window rect
    AknPopupLayouts::MenuPopupWindowGraphics(aDef);
    
    TAknWindowLineLayout line = AknLayoutScalable_Avkon::listscroll_menu_pane(0).LayoutLine();
        
    layout.LayoutRect(AknPopupLayouts::MenuRect(aDef), line);
    TRect scrollBarClientRect(layout.Rect());

    varietyIndex = 0;
    AknLayoutUtils::LayoutVerticalScrollBar(
        listBox->ScrollBarFrame(),
        scrollBarClientRect, 
        AknLayoutScalable_Avkon::scroll_pane_cp25(varietyIndex).LayoutLine() ) ;

    windowOwningControl->SetRect(AknPopupLayouts::WindowRect(aDef));
    AknPopupLayouts::HandleSizeAndPositionOfComponents(aDef, listBox, heading);

    window_rect = AknPopupLayouts::WindowRect(aDef);
    MAknsControlContext *cc = AknsDrawUtils::ControlContext( listBox );
    TBool defaultContext = EFalse;
    if (!cc)
        {
        cc = listBox->View()->ItemDrawer()->SkinBackgroundControlContext();
        defaultContext = ETrue;
        }
    if (cc)
        {
        CAknsBasicBackgroundControlContext *bcc = (CAknsBasicBackgroundControlContext*)cc;
        TAknLayoutRect popupBgRect;
         popupBgRect.LayoutRect(window_rect,
                               SkinLayout::Popup_windows_skin_placing__background_slice__Line_1(window_rect));
    
         bcc->SetBitmap(KAknsIIDQsnFrPopupCenter);
        if (defaultContext) bcc->SetRect(popupBgRect.Rect());
        bcc->SetParentPos(windowOwningControl->PositionRelativeToScreen());
        if (defaultContext)
            bcc->SetParentPos(TPoint(0,0));
        }        
    }

// end of file
