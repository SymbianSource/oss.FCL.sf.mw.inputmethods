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
* Description:           
*       Provides the FEP's Avkon UI access methods.
*
*/












#include "AknFepUiAvkonImpl.h"
#include "AknFepUiAvkonStylusPopUpMenu.h"
#include <aknnotedialog.h>          //CAknNoteDialog
#include <aknmessagequerydialog.h>  //CAknMessageQueryDialog
#include <aknradiobuttonsettingpage.h>
#include <aknsettingpage.h>
#include <aknlistquerydialog.h>
#include <aknedsts.h>               //CAknEdwinState
#include <barsread.h>
#include <aknfep.rsg>
#include "AknFepGlobalEnums.h"
#include <aknstyluspopupmenu.h>
#include <aknSctDialog.h>           //CAknCharMapDialog
#include <akntouchpane.h>
#include "AknFepQuery.h"
#include <aknedformaccessor.h>
#include "AknFepIndicatorAvkon.h"
#include <AknFepManagerInterface.h> // MAknFepManagerInterface
#include <AknIndicatorContainer.h>  //CAknIndicatorContainer
#include "AknFepUIAvkonCtrlContainerChinese.h"
#include "AknFepUIAvkonCtrlContainerJapanese.h"
#include <AknFepUiInterfacePanic.h>
#include "AknFepUIAvkonCtrlJapaneseKutenQueryDialog.h"
#include <featmgr.h>
#ifdef RD_INTELLIGENT_TEXT_INPUT
// Predictive QWERTY (XT9) changes ---->
#include <aknappui.h>
#include <akntitle.h> //CAknTitlePane
#include <StringLoader.h>
#include "AknFepAvkonCandidatePopup.h"
#include "AknFepUiAvkonCtrlExactWordPopupContent.h"
#include "AknFepPredictiveSettingDialog.h"
#include "AknFepUserDictionaryEditDialog.h"

#ifdef FF_DUAL_LANGUAGE_SUPPORT
#include "AknFepUIAvkonCtrlDualLanguageSettingDialog.h"
#endif //FF_DUAL_LANGUAGE_SUPPORT
// Predictive QWERTY (XT9) changes <----
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__    
#include <AvkonInternalCRKeys.h> 
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
#endif //RD_INTELLIGENT_TEXT_INPUT

GLDEF_C void AknFepUiInterfacePanic(TAknFepUiInterfacePanic aPanic)
    {
    User::Panic(_L("AKNFEPUI"),aPanic);
    }


// This command is is used for setting empty CBA.
const TInt ENullCommandId = 0;


CAknFepUIAvkonImpl* CAknFepUIAvkonImpl::NewL()
	{
	CAknFepUIAvkonImpl* self = new(ELeave) CAknFepUIAvkonImpl();
    CleanupStack::PushL(self);
    self->ConstructL();
    CleanupStack::Pop(self);
    return self;	
	}

void CAknFepUIAvkonImpl::ConstructL()
    {
#ifdef RD_INTELLIGENT_TEXT_INPUT
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__   
    //Attach the P&S property to the keyboardlayout and subscribe to the P&S framework
    // for any keyboard layout change.
    User::LeaveIfError(iKeyboardLayoutStatusProperty.Attach(KCRUidAvkon, KAknKeyBoardLayout));    
    iKeyboardLayoutStatusSubscriber = new (ELeave) CSubscriber(
                                         TCallBack(HandleKeyboardLayoutChangeNotification, this), 
                                         iKeyboardLayoutStatusProperty);    
    iKeyboardLayoutStatusSubscriber->SubscribeL();
#endif
#endif     
    }

CAknFepUIAvkonImpl::CAknFepUIAvkonImpl()
	{
    iDialog = NULL;
    iPredictiveSettingDialog = NULL;
    iListQueryDialog = NULL;
    iTextQueryDialog = NULL;
	}
CAknFepUIAvkonImpl::~CAknFepUIAvkonImpl()
	{
    delete iCba;
    iCba = NULL;
    DeleteDialogs();

#ifdef RD_INTELLIGENT_TEXT_INPUT    
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__       
    if (iKeyboardLayoutStatusSubscriber)
        {
        iKeyboardLayoutStatusSubscriber->StopSubscribe();
        }
    iKeyboardLayoutStatusProperty.Close();
    delete iKeyboardLayoutStatusSubscriber;     
#endif    
#endif   
	}

void CAknFepUIAvkonImpl::DeleteDialogs()
    {
    if(iDialog)
        {
        delete iDialog;
        iDialog = NULL;
        }
    if(iCharMapDialog)
        {
        delete iCharMapDialog;
        iCharMapDialog = NULL;
        }
    if(iListQueryDialog)
        {
        delete iListQueryDialog;
        iListQueryDialog = NULL;
        }
    if(iTextQueryDialog)
        {
        delete iTextQueryDialog;
        iTextQueryDialog = NULL;
        }
    if(iPredictiveSettingDialog)
        {
        TRAP_IGNORE( iPredictiveSettingDialog->ProcessCommandL(EAknCmdExit));
        }
    }

void CAknFepUIAvkonImpl::LaunchConfirmationNoteL(TInt aResourceId)
	{
    CAknNoteDialog* dlg = new (ELeave) CAknNoteDialog();
    dlg->SetTimeout(CAknNoteDialog::EShortTimeout);
    dlg->SetTone(CAknNoteDialog::ENoTone);
    dlg->ExecuteLD(aResourceId);
	}

void CAknFepUIAvkonImpl::LaunchHelpTextQueryL(TInt aResourceId)
	{
    CAknMessageQueryDialog* dlg = new (ELeave) CAknMessageQueryDialog();
    iDialog = dlg;
    dlg->ExecuteLD(aResourceId);
    iDialog = NULL;
	}

TInt CAknFepUIAvkonImpl::LaunchCangJieOptionDlgL(TInt aResourceId, TInt& aLevel, MDesC16Array* aItems)
	{
    CAknRadioButtonSettingPage* dlg = new (ELeave) CAknRadioButtonSettingPage (
        aResourceId,
        aLevel,
        aItems
        );
    TBool ret = dlg->ExecuteLD(CAknSettingPage::EUpdateWhenChanged);
	return ret;
	}

/* TCleanupSlotBase can be placed on the cleanup stack to cleanup
 * a CBase derived object, but with the benefit that the CBase
 * object can be reassigned.
 * Essesntially it is a reusable cleanup stack slot.
 * This is useful for situations where you would like to be able
 * to remove or change an item on the cleanup stack, but do not
 * know enough about the contents of the cleanup stack above it
 * to unstack then restack the items that should remain.
 */
class TCleanupSlotCBase
	{
public:
	TCleanupSlotCBase() 
		: iObj(NULL) 
		{}
	TCleanupSlotCBase(CBase* aObj) 
		: iObj(aObj) 
		{}
	void PushL()
		{ CleanupStack::PushL(TCleanupItem(Cleanup, this)); }
	void Pop()
		{ CleanupStack::Pop(); }
	void PopAndDestroy()
		{ CleanupStack::PopAndDestroy(); }
	void SetObj(CBase* aObj)
		{ iObj = aObj; }
private:
	static void Cleanup(TAny* aThis)
		{ delete static_cast<CBase*>(static_cast<TCleanupSlotCBase*>(aThis)->iObj); }
private:
	CBase* iObj;
	};
	
TInt CAknFepUIAvkonImpl::LaunchListPopupL(TInt aResourceId, TInt& aIndex, TInt aInitialIndex, MDesC16Array* aItems, CArrayPtr<CGulIcon>* aIcons)
	{
	TCleanupSlotCBase iconsCleanup(aIcons);
	iconsCleanup.PushL();
	
    iListQueryDialog = new (ELeave) CAknListQueryDialog(&aIndex);
    iListQueryDialog->PrepareLC( aResourceId );
    
    CAknListQueryControl* listQueryControl = 
        STATIC_CAST( CAknListQueryControl*, iListQueryDialog->Control(EListQueryControl));
    if (aIcons)
    	{
	    listQueryControl->Listbox()->ItemDrawer()->FormattedCellData()
	        ->SetIconArrayL( aIcons );
	   	iconsCleanup.SetObj(NULL);	// ownership transferred to dialog
	    listQueryControl->Listbox()->ItemDrawer()->FormattedCellData()
	        ->SetNotAlwaysDrawnSubCellL( 0, ETrue );// Needed because not using icon in cell.
    	}

    // Activate selected language:
    CTextListBoxModel* model = listQueryControl->Listbox()->Model();
    model->SetItemTextArray( aItems );
    model->SetOwnershipType( ELbmDoesNotOwnItemArray );
    iListQueryDialog->ListBox()->SetCurrentItemIndex( aInitialIndex );

    // Fire up the dialog
    TInt ret = iListQueryDialog->RunLD();
    iconsCleanup.Pop();
    iListQueryDialog = NULL;
    return ret;
	}

void CAknFepUIAvkonImpl::UpdateSoftkeysL(TInt aResourceId, CAknEdwinState* aEditorState, MEikCommandObserver* aObserver, TInt aInputMode)
	{
    CEikButtonGroupContainer* currentEditorCba=aEditorState->CbaState().iCurrentCba;
    if (currentEditorCba /*&& currentEditorCba->IsVisible()*/)
        {
//        currentEditorCba->MakeVisible(ETrue);
        TBool cbaModified = EFalse;

        // remove the old command button(s)
        if (aEditorState->CbaState().iLeftSoftkeyCommandId)
            {
            currentEditorCba->RemoveCommandFromStack(ELeftSoftkeyIndex, currentEditorCba->ButtonGroup()->CommandId(ELeftSoftkeyIndex));
            currentEditorCba->RemoveCommandObserver(ELeftSoftkeyIndex);
            aEditorState->CbaState().iLeftSoftkeyCommandId=0;
            cbaModified = ETrue;
            }
        if (aEditorState->CbaState().iRightSoftkeyCommandId)
            {
            currentEditorCba->RemoveCommandFromStack(ERightSoftkeyIndex, currentEditorCba->ButtonGroup()->CommandId(ERightSoftkeyIndex));
            currentEditorCba->RemoveCommandObserver(ERightSoftkeyIndex);
            aEditorState->CbaState().iRightSoftkeyCommandId=0;
            cbaModified = ETrue;
            }
        if (aEditorState->MiddleSoftkeyCommandId())
            {
            currentEditorCba->RemoveCommandFromStack(EMiddleSoftkeyIndex, currentEditorCba->ButtonGroup()->CommandId(EMiddleSoftkeyIndex));
            currentEditorCba->RemoveCommandObserver(EMiddleSoftkeyIndex);
            aEditorState->SetMiddleSoftkeyCommandId(0);
            cbaModified = ETrue;
            }

        if( aResourceId  == R_AKNFEP_SOFTKEYS_FINGER ||
            aResourceId  == R_AKNFEP_SOFTKEYS_FINGER_OPTION_PREVIOUS  ||
            aResourceId  == R_AKNFEP_SOFTKEYS_FINGER_SPELL_PREVIOUS )
            {
            ShowOwnsSoftkeysL(aResourceId, aObserver);
            return;     // return this function
            }
        // for Japanese
        switch (aResourceId)
            {
            case R_AKNFEP_SOFTKEYS_PREDICT_OTHER_COMMIT:
            case R_AKNFEP_SOFTKEYS_EMPTY_EMPTY_COMMIT:
            case R_AKNFEP_SOFTKEYS_EMPTY_OTHER_COMMIT:
            case R_AKNFEP_SOFTKEYS_PHRASE_CREATION_SELECT_CANCEL_SELECT:
            case R_AKNFEP_SOFTKEYS_DONE_CANCEL_DONE:
                ShowOwnsSoftkeysL(aResourceId, aObserver);
                return;     // return this function
            case R_AKNFEP_SOFTKEYS_CONVERT_OTHER_SELECT:
            case R_AKNFEP_SOFTKEYS_EMPTY_CANCEL_SELECT:
            case R_AVKON_SOFTKEYS_EMPTY:
                ShowEmptySoftkeysL(ETrue, aObserver, aInputMode);
                return;     // return this function
            default:
                // delete iCba object in this function
                ShowEmptySoftkeysL(EFalse, aObserver, aInputMode);
                break;
            }

        // add the new button/command ids
        if (aResourceId)
            {
            TResourceReader reader;
            CCoeEnv::Static()->CreateResourceReaderLC(reader, aResourceId);
            reader.ReadInt32();     // flags
            reader.ReadInt16();     // width
            reader.ReadInt32();     // related buttons
            reader.ReadInt16();     // size of buttons array

            TInt leftSoftkeyShortCommandId=(currentEditorCba->ButtonGroup()->CommandId(ELeftSoftkeyIndex)) & 0x0000ffff;

            for (TInt index=0; index < currentEditorCba->ButtonCount(); index++)
                {
                TInt cbaIndex = index;
                if ( index >=1 )
                    {
                    cbaIndex++;    // skip the scrollbar
                    }

                TUint8 version = (TUint8)reader.ReadUint8();
                TInt commandId;
                if (version == KAknCbaVersion)
                    {
                    TUint32 shortCommandId = reader.ReadUint16();
                    TUint32 longCommandId = reader.ReadUint16();
                    commandId = (longCommandId << 16) | shortCommandId;
                    }
                else
                    {
                    commandId = TInt(TUint(reader.ReadUint16()));
                    }
                TPtrC text=reader.ReadTPtrC();
                reader.ReadTPtrC(); // bmp filename
                reader.ReadInt16(); // bmp id
                reader.ReadInt16(); // bmp mask id

                // check we have a command and also check that if we're changing the
                // left softkey that it is 'Options'
                if ( commandId )
                    {
                    if(!currentEditorCba->UpdatedCommandObserverExists(CEikButtonGroupContainer::TCommandPosition(cbaIndex)))
                        {
                        currentEditorCba->UpdateCommandObserverL(cbaIndex, *aObserver);
                        currentEditorCba->AddCommandToStackL(cbaIndex, commandId, text);
                        
                    if (cbaIndex==ELeftSoftkeyIndex)
                        {
                        aEditorState->CbaState().iLeftSoftkeyCommandId=commandId;
                        }
                    else if (cbaIndex==ERightSoftkeyIndex)
                        {
                        aEditorState->CbaState().iRightSoftkeyCommandId=commandId;
                        }
                    else if (cbaIndex==EMiddleSoftkeyIndex)
                        {
                        aEditorState->SetMiddleSoftkeyCommandId(commandId);
                        }
                        }

                    cbaModified = ETrue;
                    }
                }
            CleanupStack::PopAndDestroy(); // reader
            }

        if (cbaModified)
            {
            currentEditorCba->DrawNow(); // to update the cba
            }
        }
	}

/**
 *  Show cba of aResourceId using owns cba object(iCba)
 *
 *  @since 3.2
 *  @param aResourceId Cba resource id.
 *  @return None
 */
void CAknFepUIAvkonImpl::ShowOwnsSoftkeysL(const TInt aResourceId, MEikCommandObserver* aObserver)
    {
    if (iCba)
        {
        DeleteCBAL();
        }
    // Create Inside CBA if needed
    CreateInsideCBAL( CEikButtonGroupContainer::ECba,
    		          CEikButtonGroupContainer::EHorizontal,
    		          aObserver,
    		          aResourceId );
    
    if ( iCba == NULL )
        {
        iCba = CEikButtonGroupContainer::NewL( CEikButtonGroupContainer::ECba,
                                               CEikButtonGroupContainer::EHorizontal,
                                               aObserver,
                                               aResourceId );
        iCba->SetBoundingRect(CEikonEnv::Static()->EikAppUi()->ApplicationRect());
        iCba->DrawNow();
        }
    }

/**
 * This method is that empty CBA is shown in case aShown is ETrue.
 * If aShown is EFalse, empty CBA is erased.
 */
void CAknFepUIAvkonImpl::ShowEmptySoftkeysL(TBool aShown, MEikCommandObserver* aObserver, TInt aInputMode)
    {
    if (aShown)
        {
        if (!iCba)
            {
            CreateInsideCBAL( CEikButtonGroupContainer::ECba, CEikButtonGroupContainer::EHorizontal, aObserver, R_AVKON_SOFTKEYS_EMPTY );
            if ( iCba == NULL )
            	{
                iCba = CEikButtonGroupContainer::NewL( CEikButtonGroupContainer::ECba, 
                		                               CEikButtonGroupContainer::EHorizontal, 
                		                               aObserver, R_AVKON_SOFTKEYS_EMPTY );
                iCba->SetBoundingRect(CEikonEnv::Static()->EikAppUi()->ApplicationRect());
            	}
            }
        // set each CBA
        TBool update = UpdateSoftkeyLabelL(ELeftSoftkeyIndex, ENullCommandId, R_TEXT_SOFTKEY_EMPTY);
        if (aInputMode == EPinyin || aInputMode == EZhuyin ||
            aInputMode == EStroke || aInputMode == ECangJie)
            {
            update |= UpdateSoftkeyLabelL(ERightSoftkeyIndex, EAknFepSoftkeyCloseWindow, R_AKNFEP_SOFTKEY_CCPU_CANCEL);
            update |= UpdateSoftkeyLabelL( EMiddleSoftkeyIndex, ENullCommandId, R_AVKON_SOFTKEY_SELECT_TEXT ); 
            }
        else
            {
            update |= UpdateSoftkeyLabelL(ERightSoftkeyIndex, ENullCommandId, R_TEXT_SOFTKEY_EMPTY);
            update |= UpdateSoftkeyLabelL(EMiddleSoftkeyIndex, ENullCommandId, R_TEXT_SOFTKEY_EMPTY);
            }

        if (update)
            {
            iCba->DrawNow();
            }
        }
    else
        {
        DeleteCBAL();
        }
    }

TBool CAknFepUIAvkonImpl::SoftkeysExist()
	{
	return iCba != NULL;
	}

TInt CAknFepUIAvkonImpl::SoftkeyCommandId(TInt aPosition)
	{
	return iCba->ButtonGroup()->CommandId(aPosition);
	}

void CAknFepUIAvkonImpl::DeleteSoftkeys()
	{
	TRAP_IGNORE( DeleteCBAL() );
	}

void CAknFepUIAvkonImpl::DrawSoftkeysNow()
	{
	iCba->DrawNow();
	}

void CAknFepUIAvkonImpl::CreateSoftkeys(TInt aResourceId, MEikCommandObserver* aObserver)
	{
	DeleteSoftkeys();
	TRAP_IGNORE(CreateInsideCBAL(CEikButtonGroupContainer::ECba, CEikButtonGroupContainer::EHorizontal, aObserver, aResourceId));
	if ( iCba == NULL)
		{
        TRAP_IGNORE(iCba = CEikButtonGroupContainer::NewL(CEikButtonGroupContainer::ECba, CEikButtonGroupContainer::EHorizontal, aObserver, aResourceId));
        iCba->SetBoundingRect(CEikonEnv::Static()->EikAppUi()->ApplicationRect());
		}
    iCba->MakeVisible(ETrue);
    iCba->ButtonGroup()->AsControl()->DrawableWindow()->SetOrdinalPosition(0,ECoeWinPriorityAlwaysAtFront + 10);
	}

TBool CAknFepUIAvkonImpl::UpdateSoftkeyLabelL(TInt aPosition, TInt aCommandId, TInt aTextResId)
	{
    if (iCba->ButtonGroup()->CommandId(aPosition) != aCommandId)
        {
        HBufC* buf = CEikonEnv::Static()->AllocReadResourceLC(aTextResId);
        if( buf->Des().Length() > 1 && EMiddleSoftkeyIndex != aPosition )
            {
            iCba->SetCommandL(aPosition, aCommandId, buf->Des().Mid(1));  
            CEikCba *eikcba = static_cast<CEikCba*>( iCba->ButtonGroup() );
            eikcba->EnableItemSpecificSoftkey( EFalse );                             
            }
        else
            {           
            iCba->SetCommandL(aPosition, aCommandId, *buf);
            }
        CleanupStack::PopAndDestroy(buf);
        return ETrue;
        }
    return EFalse;
	}

void CAknFepUIAvkonImpl::SetFepMenuObserver(MAknFepUiInterfaceMenuObserver* aObserver)
	{
	iFepMenuObserver = aObserver;
	if (aObserver)
		{	
		CAknEnv::Static()->SetFepMenuObserver(this);
		}
	else
		{	
		CAknEnv::Static()->SetFepMenuObserver(NULL);
		}
	}

CAknFepUiInterfaceMenuBar* CAknFepUIAvkonImpl::EditorStateMenuBar(CAknEdwinState* aEditorState)
	{
	iEditorMenuBar.iMenuBar = aEditorState->MenuBar();
	return &iEditorMenuBar;
	}

void CAknFepUIAvkonImpl::DynInitMenuPaneL(TInt aResourceId,CEikMenuPane* aMenuPane)
	{
	iTempMenuPane.iMenuPane = aMenuPane;
	if (iFepMenuObserver)
		iFepMenuObserver->DynInitMenuPaneL(aResourceId, &iTempMenuPane);
	}

void CAknFepUIAvkonImpl::DynInitMenuBarL(TInt aResourceId,CEikMenuBar* aMenuBar)
	{
	CAknFepUIAvkonMenuBarImpl* impl = &iTempMenuBar;
	if (iEditorMenuBar.iMenuBar == aMenuBar)
		impl = &iEditorMenuBar;
	impl->iMenuBar = aMenuBar;
	if (iFepMenuObserver)
		iFepMenuObserver->DynInitMenuBarL(aResourceId, impl);
	}

void CAknFepUIAvkonImpl::SetEmphasis(CCoeControl* aMenuControl,TBool aEmphasis)
	{
	CBase* impl = NULL;
	if (aMenuControl == iEditorMenuBar.iMenuBar)
		impl = &iEditorMenuBar;
	else if (aMenuControl == iTempMenuBar.iMenuBar)
		impl = &iTempMenuBar;
	else if (aMenuControl == iTempMenuPane.iMenuPane)
		impl = &iTempMenuPane;
	if (iFepMenuObserver)
		iFepMenuObserver->SetEmphasis(impl, aEmphasis);
	}

void CAknFepUIAvkonImpl::ProcessCommandL(TInt aCommandId)
	{
	if (iFepMenuObserver)
		iFepMenuObserver->ProcessCommandL(aCommandId);
	}

CAknFepUiInterfaceMenuBar* CAknFepUIAvkonImpl::NewMenuBarL(MAknFepUiInterfaceMenuObserver* aObserver, TInt aHotKeyResourceId, TInt aMenuTitleResourceId)
	{
	CAknFepUIAvkonMenuBarOwningImpl* menuBar = new(ELeave) CAknFepUIAvkonMenuBarOwningImpl;
	CleanupStack::PushL(menuBar);
	menuBar->ConstructL(aObserver, aHotKeyResourceId, aMenuTitleResourceId);
	CleanupStack::Pop(menuBar);
	return menuBar;
	}

CAknFepUiInterfaceStylusPopUpMenu* CAknFepUIAvkonImpl::NewStylusPopUpMenuL(MAknFepUiInterfaceMenuObserver* aObserver, const TPoint& aPoint)
	{
	CAknFepUIAvkonStylusPopUpMenu* popupMenu = new(ELeave) CAknFepUIAvkonStylusPopUpMenu;
	CleanupStack::PushL(popupMenu);
	popupMenu->ConstructL(aObserver, aPoint);
	CleanupStack::Pop(popupMenu);
	return popupMenu;
	}

TInt CAknFepUIAvkonImpl::CharMapDialogL(TInt aCharCase, TDes& aSpecialChars, TInt aCharSetResourceId, TBool aLockNumericKeys, TBool aShowPictographsFirst, TBool aShowNoPictographs, TBool aShowAnotherTable, TInt aAfterSctChars, TInt aResId, TBool aDisableRecentItemRow )
	{
	TInt result;
	
	iCharMapDialog = new(ELeave) CAknCharMapDialog(aCharCase, aSpecialChars, aCharSetResourceId);

    if (aLockNumericKeys)
        {
        iCharMapDialog->LockNumericKeys(ETrue);
        }

    if ( aShowPictographsFirst )
        {
        // It is supposed that the pictograph table launch is allowed in this case.
        iCharMapDialog->ShowPictographsFirst();
        }
    else if (aShowNoPictographs)
        {
        // Pictograph table is not shown.
        iCharMapDialog->ShowNoPictographs();
        }

    if (aShowAnotherTable)
        {
        iCharMapDialog->ShowAnotherTable(aAfterSctChars);
        }
    if(aDisableRecentItemRow)
        {
        iCharMapDialog->DisableRecentCharsRow();
        }
    

    result = iCharMapDialog->ExecuteLD(aResId);
    iCharMapDialog = NULL;
    return result;
	}

void CAknFepUIAvkonImpl::TouchPaneSetFepPenSupportInterface(MAknFepPenSupportInterface* aFepPenSupportInterface)
	{
	if(iAvkonAppUi->TouchPane())
		iAvkonAppUi->TouchPane()->SetFepPenSupportInterface(aFepPenSupportInterface);
	}

void CAknFepUIAvkonImpl::TouchPaneAllowInputMethodActivation(TBool aValue)
	{
	if(iAvkonAppUi->TouchPane())
		iAvkonAppUi->TouchPane()->AllowInputMethodActivation(aValue);
	}

void CAknFepUIAvkonImpl::TouchPaneRefreshL()
	{
	if(iAvkonAppUi->TouchPane())
		iAvkonAppUi->TouchPane()->RefreshL();
	}

void CAknFepUIAvkonImpl::TouchPaneSetInputMethodIconActivated(TBool aActivated)
	{
	if(iAvkonAppUi->TouchPane())
		iAvkonAppUi->TouchPane()->SetInputMethodIconActivated(aActivated);
	}

TInt CAknFepUIAvkonImpl::TextQueryDialogL(TDes& aDataText, CAknEdwinState& aEditorState, TInt aEditorFlag, TInt aResId)
	{
	TInt result;
    iTextQueryDialog = new (ELeave) CAknFepTextQueryDialog(aDataText, aEditorState, CAknFepTextQueryDialog::ENoTone, aEditorFlag);
    iTextQueryDialog->PrepareLC(aResId);
#ifdef RD_INTELLIGENT_TEXT_INPUT
    //last char space has to be reserved for null terminator
    iTextQueryDialog->SetMaxLength( KMaxUDBLength - 1 );
#endif //RD_INTELLIGENT_TEXT_INPUT
    result = iTextQueryDialog->RunLD();
    iTextQueryDialog = NULL;
    return result;
	}

CPlainText* CAknFepUIAvkonImpl::PlainText(CAknEdwinState* aEditorState)
	{
    CEikEdwin* editor = NULL;
    if (aEditorState)
        {
        CAknEdwinFormAccessor* formAccessor = aEditorState->FormAccessor();
        if (formAccessor)
            {
            editor = (CEikEdwin*)formAccessor->FormClientControl();
            if (editor)
                {
                return editor->Text();                
                }
            }
        }
    return NULL;
	}

void CAknFepUIAvkonImpl::UpdateEditorContext(MAknEditingStateIndicator& aIndicator, MAknFepManagerInterface& aManager)
	{
    CAknIndicatorContainer* indicatorContainer = aIndicator.IndicatorContainer();

    if(indicatorContainer)
        {
        TInt indicatorContext = indicatorContainer->IndicatorContext();
        aManager.SetEditorContext(indicatorContext);
        }
	}

CAknFepIndicator* CAknFepUIAvkonImpl::NewIndicatorL()
	{
    return CAknFepIndicatorAvkon::NewL();
	}

TBool CAknFepUIAvkonImpl::IsDisplayDataQuery(TBool aIsFepAwareTextEditor)
	{
    CEikAppUi* eikAppUi = (CEikAppUi *)CCoeEnv::Static()->AppUi();
    
    if( eikAppUi->IsDisplayingDialog() && eikAppUi->TopFocusedControl() )
        {
        CEikDialog* dlg = eikAppUi->TopFocusedControl()->MopGetObject( dlg );
        
        if ( dlg )
            {
            TInt flag = dlg->DialogFlags();

            if(!(flag & EEikDialogFlagFillScreen) &&
               !(flag & EEikDialogFlagFillAppClientRect) )
                {
                MObjectProvider* objectProvider = NULL;
				MCoeFepAwareTextEditor* editor;
			    MCoeFepAwareTextEditor_Extension1* ext1;
    			CAknEdwinState* state;
                CCoeEnv* coeEnv = CCoeEnv::Static();
                TCoeInputCapabilities inputCapabilities = 
                    static_cast<const CCoeAppUi*>(coeEnv->AppUi())->InputCapabilities();

    			if ( aIsFepAwareTextEditor &&
         			( editor = inputCapabilities.FepAwareTextEditor() ) != NULL &&       
         		    ( ext1 = editor->Extension1() ) != NULL &&
				    ( state = (CAknEdwinState*)ext1->State( KNullUid ) ) != NULL )
        			{
        			objectProvider = state->ObjectProvider();
        			}

    			// If not found, try to get object provider from input capabilities.
    			// It is set there with CEikMfne-derived editors.

    			if ( !objectProvider )
        			{
        			objectProvider = inputCapabilities.ObjectProvider();
        			}

			    if ( objectProvider )
        			{
        			CAknQueryControl* ctrl = objectProvider->MopGetObject( ctrl );
        		
        			if ( ctrl )
        				{
        				return ETrue;
        				}
        			}
        		}
            }
        }
         
    return EFalse;
	}

CAknFepUiInterfaceMenuBar* CAknFepUIAvkonImpl::ObjectProviderMenuBar(MObjectProvider* aObjectProvider)
	{
	if (!aObjectProvider)
		return NULL;
	CEikMenuBar* menu;
	aObjectProvider->MopGetObject(menu);
	if (menu)
		{
		iObjectProviderMenuBar.iMenuBar = menu;
		return &iObjectProviderMenuBar;
		}
	else
		return NULL;
	}

MAknFepUICtrlContainerChinese* CAknFepUIAvkonImpl::NewAknFepUICtrlContainerChineseL()
	{
    if (FeatureManager::FeatureSupported(KFeatureIdChinese))
        {
        return CAknFepUICtrlContainerChinese::NewL();
        }

    User::Leave(KErrNotSupported);
	return NULL;
	}

MAknFepUICtrlContainerJapanese* CAknFepUIAvkonImpl::NewAknFepUICtrlContainerJapaneseL(CAknFepUIManagerJapanese* aUiMng)
	{
    if (FeatureManager::FeatureSupported(KFeatureIdJapanese))
        {
        return CAknFepUICtrlContainerJapanese::NewL(aUiMng);
        }
    return NULL;
	}

void CAknFepUIAvkonImpl::JapaneseKutenQueryDialogL(TInt aResId, TInt& aCharCode, TInt aTone)
	{
    if (FeatureManager::FeatureSupported(KFeatureIdJapanese))
        {
        CAknFepUICtrlJapaneseKutenQueryDialog* query = CAknFepUICtrlJapaneseKutenQueryDialog::NewL(aCharCode, (CAknQueryDialog::TTone)aTone);
        iDialog = query;
        query->ExecuteLD(aResId);
        iDialog = NULL;
        }
	}

#ifdef RD_INTELLIGENT_TEXT_INPUT
// Predictive QWERTY (XT9) changes ---->
MAknFepCandidatePopup* CAknFepUIAvkonImpl::NewCandidatePopupL(MAknFepCandidatePopupCallback& aOwner)
	{
	MAknFepCandidatePopup* candidatePopup = CAknFepAvkonCandidatePopup::NewL(aOwner);
	return candidatePopup;
	}

MAknFepUiWordPopupContent* CAknFepUIAvkonImpl::CreateWordPopupContentL()
	{
	return CAknFepExactWordPopupContent::NewL();	
	}
	
void CAknFepUIAvkonImpl::LaunchPredictiveSettingDialogL(TInt aDialogResId, 
														TInt aMenuResId, 
														TInt aConfirmationQueryResId,
														TInt aTitlePaneResId)
	{
	iPredictiveSettingDialog = CAknFepPredictiveSettingDialog::NewL(aMenuResId, aConfirmationQueryResId, aTitlePaneResId);
	iPredictiveSettingDialog->ExecuteLD(aDialogResId);
	iPredictiveSettingDialog = NULL;
	}

#ifdef __USER_DICTIONARY_EDITING__
void CAknFepUIAvkonImpl::LaunchUserDictEditDialogL()
    {
    CAknFepUserDictionaryEditDialog::RunDlgLD();
    }
#endif //__USER_DICTIONARY_EDITING__

#ifdef FF_DUAL_LANGUAGE_SUPPORT
void CAknFepUIAvkonImpl::LaunchWritingLanguageSettingDialogL(TInt aDialogResId, TInt aMenuResId, TInt aTitlePaneResId)
    {    
    CCoeEnv* coeEnv = CCoeEnv::Static();
	CAknAppUi* appUi = static_cast<CAknAppUi*>(coeEnv->AppUi());
    CAknTitlePane* titlePane = static_cast<CAknTitlePane*>( appUi->StatusPane()->ControlL( TUid::Uid( EEikStatusPaneUidTitle ) ) );
    
    const TDesC* appTitle = titlePane->Text();
	HBufC* oldTitle = appTitle->AllocL();
	CleanupStack::PushL(oldTitle);
	HBufC* title = StringLoader::LoadLC( aTitlePaneResId,coeEnv);
    titlePane->SetTextL( *title );
    CleanupStack::PopAndDestroy(title);
    
    CAknFepDualLanguageSettingDialog* dlg = CAknFepDualLanguageSettingDialog::NewL(aMenuResId);            
    iDialog = dlg;
    dlg->ExecuteLD(aDialogResId);
    iDialog = NULL;
    
    titlePane->SetTextL( *oldTitle );
    CleanupStack::PopAndDestroy(oldTitle);
    }
#endif //FF_DUAL_LANGUAGE_SUPPORT
// Predictive QWERTY (XT9) changes <----	

#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
TInt CAknFepUIAvkonImpl::HandleKeyboardLayoutChangeNotification(TAny* aObj)
    {
    if (aObj)
        {
        static_cast<CAknFepUIAvkonImpl*>(aObj)->HandleKeyboardLayoutChange();
        return KErrNone;
        }
    else
        {
        return KErrArgument;
        }
    }

void CAknFepUIAvkonImpl::HandleKeyboardLayoutChange()
    {
    if(iPredictiveSettingDialog)
        {
        iPredictiveSettingDialog->HandleResourceChange(KEikDynamicLayoutVariantSwitch);
        }
    }

CAknFepUIAvkonImpl::CSubscriber::CSubscriber(TCallBack aCallBack, RProperty& aProperty)
    :
    CActive(EPriorityNormal), iCallBack(aCallBack), iProperty(aProperty)
    {
    CActiveScheduler::Add(this);
    }

CAknFepUIAvkonImpl::CSubscriber::~CSubscriber()
    {
    Cancel();
    }

void CAknFepUIAvkonImpl::CSubscriber::SubscribeL()
    {
    if (!IsActive())
        {
        iProperty.Subscribe(iStatus);
        SetActive();
        }
    }

void CAknFepUIAvkonImpl::CSubscriber::StopSubscribe()
    {
    Cancel();
    }

void CAknFepUIAvkonImpl::CSubscriber::RunL()
    {
    if (iStatus.Int() == KErrNone)
        {
        iCallBack.CallBack();
        SubscribeL();
        }
    }

void CAknFepUIAvkonImpl::CSubscriber::DoCancel()
    {
    iProperty.Cancel();
    }
    
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
#endif //RD_INTELLIGENT_TEXT_INPUT

void CAknFepUIAvkonImpl::DeleteCBAL()
	{
	if ( iCba )
	    {
	    delete iCba;
	    iCba = NULL;
	    }
	
	if ( isCbaEmded )
		{
		CEikAppUi* eikAppUi = (CEikAppUi *)CCoeEnv::Static()->AppUi();
		if( eikAppUi!= NULL && eikAppUi->IsDisplayingDialog() && eikAppUi->TopFocusedControl() )
		    {
		    CEikDialog* dlg = eikAppUi->TopFocusedControl()->MopGetObject( dlg );
		    if ( dlg )
		        {
		        CEikButtonGroupContainer* currentCba = dlg->MopGetObject( currentCba );
	        	//CEikCba* dlgcba = static_cast<CEikCba*>( currentCba->ButtonGroup());
	        	if ( currentCba && iDialogCba == currentCba )
		            {
		            currentCba->RemoveCommandFromStack( ELeftSoftkeyIndex, -1 );
		            currentCba->RemoveCommandFromStack( ERightSoftkeyIndex, -1 );

		            currentCba->DrawNow();
		            currentCba->ActivateL();
		            iDialogCba = NULL;
                    isCbaEmded = EFalse;
		            }
		        }
		    }
	    }
	}

void CAknFepUIAvkonImpl::CreateInsideCBAL( CEikButtonGroupContainer::TUse aUse,
		                                   CEikButtonGroupContainer::TOrientation aOrientation,
                                           MEikCommandObserver* aCommandObserver,
                                           TInt aResourceId )
	{
	CEikAppUi* eikAppUi = (CEikAppUi *)CCoeEnv::Static()->AppUi();
	// If current is dialog create inside CBA
	if ( eikAppUi!= NULL && eikAppUi->IsDisplayingDialog() && eikAppUi->TopFocusedControl())
		{
		CEikDialog* dlg = eikAppUi->TopFocusedControl()->MopGetObject( dlg );
		if ( dlg && CbaEmbeddedInDialog( dlg->DialogFlags()))
		    {
			CEikButtonGroupContainer* currentCba = dlg->MopGetObject( currentCba );
			//CEikCba* dlgcba = static_cast<CEikCba*>( currentCba->ButtonGroup());
			if ( currentCba )
				{
				TUint flags( 0 );
				flags |= CEikButtonGroupContainer::EIsEmbedded | CEikButtonGroupContainer::EAddToStack;
				if ( iCba != NULL )
					{
					delete iCba;
					iCba = NULL;
					}
				iCba = CEikButtonGroupContainer::NewL( aUse, aOrientation, 
						                               aCommandObserver, 
						                               aResourceId, flags );
				
				CEikCba* cba = static_cast<CEikCba*>( iCba->ButtonGroup());
				
				//CEikCba* dlgcba = static_cast<CEikCba*>( currentCba->ButtonGroup());
				if( !isCbaEmded )
					{
					currentCba->AddCommandToStackL( ELeftSoftkeyIndex, -1, _L(""), NULL, NULL );
					currentCba->AddCommandToStackL( ERightSoftkeyIndex, -1, _L(""), NULL, NULL );
					currentCba->ActivateL();
					currentCba->DrawNow();
					iDialogCba = currentCba;
					isCbaEmded = ETrue;
					}
			
				cba->SetButtonGroupFlags( ~( EEikCbaFlagTransparent | EEikCbaFlagOutlineFont ));
				TRect dlgRect( dlg->Rect());
				TRect cbaRect( currentCba->Rect());
				iCba->SetRect( currentCba->Rect());
				iCba->SetPosition( 
						TPoint( dlg->DrawableWindow()->Position().iX,dlg->DrawableWindow()->Position().iY + dlgRect.Height() - cbaRect.Height()));
				iCba->SetBoundingRect( dlg->Rect());
			    }
		    }
		}
	}

TBool CAknFepUIAvkonImpl::CbaEmbeddedInDialog( const TInt& aFlags )
    {
    return AknLayoutUtils::PenEnabled() && 
        !( aFlags & EEikDialogFlagFillAppClientRect ) &&
        !( aFlags & EEikDialogFlagFillScreen ) &&
        !( aFlags & EEikDialogFlagVirtualInput );
    }
//End of File
