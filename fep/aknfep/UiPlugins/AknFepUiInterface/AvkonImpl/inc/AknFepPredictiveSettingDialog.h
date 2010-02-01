/*
* Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies). 
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
*
*/










/*
 * ============================================================================
 *  Name     : CAknFepPredictiveSettingList from AknFepPredictiveSettingList 
 * ============================================================================
 */

#ifndef __AKN_FEP_PREDICTIVESETTING_DIALOG_H
#define __AKN_FEP_PREDICTIVESETTING_DIALOG_H

#include <AknDialog.h>
#include "AknFepPredictiveSettingList.h"
#include "AknFepPredictiveSettingData.h"

class CRepository;
class CAknTitlePane;
class CAknNavigationControlContainer;
class CAknNavigationDecorator;

enum TInputSettings
{
	EAutoWordCompletion,
	ETypingCorrectionLevel,
	ENumberCandidate,
	EPrimaryCandidate
};

class CAknFepPredictiveSettingDialog : public CAknDialog
{
	public:
	/**
	 	* Function: 	NewL
		*
	 	* Discussion:	Factory function used to create a new instance of
	 	* 				CAknFepPredictiveSettingDialog
		*
		* Params:		aMenuResId - menu resource id.
		* Params:		aConfirmationQueryResId - Confirmation Query resource id.
		* Params:       aTitlePaneResId - Titlepane resource id.
		* Returns:		A newly created instance of CAknFepPredictiveSettingDialog
		*/
		static CAknFepPredictiveSettingDialog* NewL(TInt aMenuResId, TInt aConfirmationQueryResId, TInt aTitlePaneResId);

		/**
	 	* Function: 	NewLC
		*
	 	* Discussion:	Factory function used to create a new instance of
	 	* 				CAknFepPredictiveSettingDialog and leave a pointer to it on the
	 	*				cleanup	stack.
		*
		* Params:		aMenuResId - menu resource id.
		* Params:		aConfirmationQueryResId - Confirmation Query resource id.
		* Params:       aTitlePaneResId - Titlepane resource id.
	 	* Returns:		A newly created instance of CAknFepPredictiveSettingDialog
		*/
		static CAknFepPredictiveSettingDialog* NewLC(TInt aMenuResId, TInt aConfirmationQueryResId, TInt aTitlePaneResId);

		/**
		* Function:		~CAknFepPredictiveSettingDialog
		*
		* Discussion: 	Virtual destructor for CAknFepPredictiveSettingDialog. Ensures that all
		*				allocated memory is returned to the heap before deletion
		* 				of object.
		*
		*/
		virtual ~CAknFepPredictiveSettingDialog();
		
		virtual SEikControlInfo CreateCustomControlL(TInt aControlType);
		virtual CAknFepPredictiveSettingDialog::TFormControlTypes ConvertCustomControlTypeToBaseControlType(TInt aControlType) const;

		virtual TBool OkToExitL(TInt aButtonId);
		virtual void ProcessCommandL(TInt aCommandId);
		virtual void HandleResourceChange(TInt aType);
		
		void EditItemL(TBool aCalledFromMenu);
		
		void HandlePointerEventL(const TPointerEvent& aPointerEvent);
		
		// From MEikMenuObserver
		virtual void DynInitMenuPaneL( TInt aResourceId, 
		                               CEikMenuPane* aMenuPane );
	    
	private:
		void ConstructL( TInt aMenuTitleResourceId );
		CAknFepPredictiveSettingDialog(TInt aConfirmationQueryResId, TInt aTitlePaneResId);
		void ReadSettingsDataL();
	    void SaveSettingsDataL() const;
	    void SaveCurrentSettingsDataL() const;
	    void LaunchPredictiveSettingOffQueryIfNecessaryL(TInt prevSettings);
	    TInt CurrentSettings();
	    void PrepareStatusPaneL();
	    void SetTitlePaneL();
	    void RestoreStatusPaneL();
	    TInt  iConfirmationQueryResId;
	    TInt  iPredictiveTextFlag;
	    TBool iPredictiveSettingOffQueryNotShown;
	    
	    CAknTitlePane* iTitlePane;
	    CAknNavigationControlContainer* iNaviPane;
	    CAknNavigationDecorator*        iNaviDecoratedTabGroup;
	    HBufC* iLastTitle;
	    HBufC* iStatusTitle;
	    TInt iTitlePaneResId;
	    
		static TInt EventCheckCallback(TAny* aObj);
		void AttemptToExitL();

#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__		
	    TInt 						iOriginalKeyboardLayout;
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
	    
	    // iSettingItemInEditingState is introduced to avoid the crash when we are editing 
	    // the settings item and the flip staus changes.
	    TBool 						iSettingItemInEditingState; 
	    CIdle* 						iItemCloseEventCheck;
	
	protected:
	CAknFepPredictiveSettingList*	iSettingsList;
	CAknFepPredictiveSettingData*	iSettingsData;
	CRepository*					iAknFepRepository;
};
#endif //__AKN_FEP_PREDICTIVESETTING_DIALOG_H