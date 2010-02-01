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
 *  Name     : CGSPredictiveSettingList from AknFepPredictiveSettingList 
 * ============================================================================
 */

#ifndef __GSPREDICTIVESETTING_DIALOG_H
#define __GSPREDICTIVESETTING_DIALOG_H

#include <AknDialog.h>
#include "PredictiveSettingList.h"
#include "PredictiveSettingData.h"

enum TInputSettings
{
    EAutoWordCompletion,
    ETypingCorrectionLevel,
    ENumberCandidate,
    EPrimaryCandidate
};

class CGSPredictiveSettingDialog : public CAknDialog
{
	public:
	/**
	 	* Function: 	NewL
		*
	 	* Discussion:	Factory function used to create a new instance of
	 	* 				CGSPredictiveSettingDialog
		*
		* Params:		aMenuResId - menu resource id.
		* Params:		aConfirmationQueryResId - Confirmation Query resource id.
		* Returns:		A newly created instance of CGSPredictiveSettingDialog
		*/
		static CGSPredictiveSettingDialog* NewL(TInt aMenuResId, TInt aConfirmationQueryResId);

		/**
	 	* Function: 	NewLC
		*
	 	* Discussion:	Factory function used to create a new instance of
	 	* 				CGSPredictiveSettingDialog and leave a pointer to it on the
	 	*				cleanup	stack.
		*
		* Params:		aMenuResId - menu resource id.
		* Params:		aConfirmationQueryResId - Confirmation Query resource id.
	 	* Returns:		A newly created instance of CGSPredictiveSettingDialog
		*/
		static CGSPredictiveSettingDialog* NewLC(TInt aMenuResId, TInt aConfirmationQueryResId);

		/**
		* Function:		~CGSPredictiveSettingDialog
		*
		* Discussion: 	Virtual destructor for CGSPredictiveSettingDialog. Ensures that all
		*				allocated memory is returned to the heap before deletion
		* 				of object.
		*
		*/
		virtual ~CGSPredictiveSettingDialog();
		
		virtual SEikControlInfo CreateCustomControlL(TInt aControlType);
		virtual CGSPredictiveSettingDialog::TFormControlTypes ConvertCustomControlTypeToBaseControlType(TInt aControlType) const;

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
		CGSPredictiveSettingDialog(TInt aConfirmationQueryResId);
		void ReadSettingsDataL();
	    void SaveSettingsDataL() const;
	    void SaveCurrentSettingsDataL() const;
	    void LaunchPredictiveSettingOffQueryIfNecessaryL(TInt aPrevSettings);
	    TInt CurrentSettings();
	    
	    TInt  iConfirmationQueryResId;
	    TInt  iPredictiveTextFlag;
	    TBool iPredictiveSettingOffQueryNotShown;
#ifdef __ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
		
		static TInt EventCheckCallback(TAny* aObj);
		void AttemptToExitL();
		
	    TInt 						iOriginalKeyboardLayout;
	    
	    // iSettingItemInEditingState is introduced to avoid the crash when we are editing 
	    // the settings item and the flip staus changes.
	    TBool 						iSettingItemInEditingState; 
	    
	    CIdle* 						iItemCloseEventCheck;
#endif //__ITI_VIRTUAL_TOUCH_FIRST_GENERATION_SUPPORT__
		
	protected:
	CGSPredictiveSettingList*	iSettingsList;
	CGSPredictiveSettingData*	iSettingsData;
};
#endif //__GSPREDICTIVESETTING_DIALOG_H
