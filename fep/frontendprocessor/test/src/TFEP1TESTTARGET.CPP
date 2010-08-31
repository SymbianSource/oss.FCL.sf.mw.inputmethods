// Copyright (c) 2006-2010 Nokia Corporation and/or its subsidiary(-ies).
// All rights reserved.
// This component and the accompanying materials are made available
// under the terms of "Eclipse Public License v1.0"
// which accompanies this distribution, and is available
// at the URL "http://www.eclipse.org/legal/epl-v10.html".
//
// Initial Contributors:
// Nokia Corporation - initial contribution.
//
// Contributors:
//
// Description:
// Launch the test dialog application with edit boxes and load the FEP.
// Create the character codes of the key events.
// Send them to the dialog box for the FEP to intercept and to display on the editboxes of the target dialog box.
// NOTE: This test is mostly automated as the result of the dialog boxes is checked. However, due to time constraints
// it has not been investigated a way to automate parts of it, particularly key combinations of 'shift+A', 'shift+B' or 'shift+S',
// which still rely on a live creature checking that the output on the screen is correct (that creature may want to put a delay
// when sending messages to be able to see that message, otherwise it may be too fast for it/him/her).
//
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code
*/


#include <e32math.h>
#include <ecom/ecom.h>
#include <fepbase.h>

#include <tfep1testtarget.rsg>

#include "TFEP1TESTTARGET.h"
#include "TFEP1TESTTARGET.hrh"



const TUid KUidTFep1 = { 0x102024D0 };
const TUid KUidTFep4 = { 0x1028716D };

// String literal to represent the language-specific resource file path.
_LIT(KFep1TestTargetResourceFilePath, "z:\\system\\test\\fepbasetest\\tfep1testtarget.r01");

// global variables
TBuf<16> gTextNormalEdwin;
TBuf<16> gTextNormalNumEdwin;
TBuf<16> gTextKatakanaEdwin;
TBuf<32> gTextPhoneticalEdwin;
TBuf<32> gTextCJKEdwin;

// expected results (according to FEP1 settings and the actions performed in the CFep1TestTargetUi::RunTestStepL method below)
_LIT(KExpectedTextNormalEdwin, "ABCGHIJKLMQRST@");
_LIT(KExpectedTextNormalNumEdwin, "1230056");
_LIT(KExpectedTextKatakanaEdwin, "EFG");
_LIT(KExpectedTextPhoneticalEdwin, "JulietteKiloLimaMNO");
_LIT(KExpectedTextCJKEdwin, "!\x0411\x0E02\x2E81\x303A\x4E19\xAD00\xD840\xDC00\xD87E\xDC00 !\"#");


/*************************************************************
 **
 **  TTstResourceFileId - TCleanupItem derived class
 **
 *************************************************************/
/*
	Encapsulates the clean up operations. The class unloads the resource file from the list maintained by CCoeEnv
*/
TTstResourceFileId::TTstResourceFileId(CCoeEnv& aConeEnvironment, TInt aResourceFileId)
	:TCleanupItem(UnloadResourceFile, this),
	 iConeEnvironment(aConeEnvironment),
	 iResourceFileId(aResourceFileId)
	{
	}

void TTstResourceFileId::UnloadResourceFile(TAny* aThis)
	{
	TTstResourceFileId& resourceFileId=*static_cast<TTstResourceFileId*>(aThis);
	resourceFileId.iConeEnvironment.DeleteResourceFile(resourceFileId.iResourceFileId);
	}



/*************************************************************
 **
 **  CTstTestTargetDialog - CEikDialog derived class
 **
 *************************************************************/

/**
  This fucntion is derived to perform the pre-layout dialog initialisation.\n
  This is called by the dialog framework before the dialog is sized and laid out. \n
  It is overloaded to initialise the control values that influence sizing and layout.\n
*/
void CTstTestTargetDialog::PreLayoutDynInitL()
	{
	CEikEdwin& halfWidthKatakanaEdwin=*static_cast<CEikEdwin*>(Control(EControlIdHalfWidthKatakanaEdwin));
	TCoeInputCapabilities inputCapabilities=halfWidthKatakanaEdwin.InputCapabilities();
	inputCapabilities.SetCapabilities(TCoeInputCapabilities::EJapaneseKatakanaHalfWidth|TCoeInputCapabilities::ENavigation);
	halfWidthKatakanaEdwin.SetInputCapabilitiesL(inputCapabilities);
	CEikEdwin& phoneticAlphabetEdwin=*static_cast<CEikEdwin*>(Control(EControlIdPhoneticAlphabetEdwin));
	inputCapabilities=phoneticAlphabetEdwin.InputCapabilities();
	phoneticAlphabetEdwin.SetInputCapabilitiesL(TCoeInputCapabilities(TCoeInputCapabilities::EWesternAlphabetic|TCoeInputCapabilities::ENavigation, inputCapabilities.FepAwareTextEditor(), inputCapabilities.CaptionRetrieverForFep(), TUid::Uid(KTfep1Uid), this));
	CEikEdwin& cjkEdwin=*static_cast<CEikEdwin*>(Control(EControlIdCJKEdwin));
	inputCapabilities=cjkEdwin.InputCapabilities();
	inputCapabilities.SetCapabilities(TCoeInputCapabilities::EAllText|TCoeInputCapabilities::ENavigation);
	cjkEdwin.SetInputCapabilitiesL(inputCapabilities);
	}


TBool CTstTestTargetDialog::OkToExitL(TInt /*aKeyCode*/)
	{
    RDebug::Print(_L("**************************************************************"));
    RDebug::Print(_L("****                OK or CANCEL key pressed                  "));
    RDebug::Print(_L("**************************************************************\n"));

	// Get fields text values...
	CEikEdwin* edwinNormal = static_cast<CEikEdwin*>(Control(EControlIdNormalEdwin));
	edwinNormal->GetText(gTextNormalEdwin);

	CEikEdwin* edwinNormalNum = static_cast<CEikEdwin*>(Control(EControlIdNormalNumEdwin));
	edwinNormalNum->GetText(gTextNormalNumEdwin);

	CEikEdwin* edwinKatana = static_cast<CEikEdwin*>(Control(EControlIdHalfWidthKatakanaEdwin));
	edwinKatana->GetText(gTextKatakanaEdwin);

	CEikEdwin* edwinPhonetical = static_cast<CEikEdwin*>(Control(EControlIdPhoneticAlphabetEdwin));
	edwinPhonetical->GetText(gTextPhoneticalEdwin);

	CEikEdwin* edwinCJK = static_cast<CEikEdwin*>(Control(EControlIdCJKEdwin));
	edwinCJK->GetText(gTextCJKEdwin);

	//It's always ok to return OK in our case...
	return ETrue;
	}


/**
   Flag TRUE for support of Phonetic alphabets
*/
TBool CTstTestTargetDialog::SupportsPhoneticAlphabet() const
	{
	return ETrue;
	}




/*************************************************************
 **
 **  CFep1TestTargetUi
 **
 *************************************************************/

CFep1TestTargetUi::CFep1TestTargetUi(CTmsTestStep* aStep) :
  	CTestAppUi(aStep, KNullDesC)
	{
  	}

CFep1TestTargetUi::~CFep1TestTargetUi()
	{
	}

/**
  Install the FEP.\n
  Set the windows server event as User defined.\n
  Connect the client session to widows server.\n
  Create the Client side handle \n
  Send the user events and close the session on completion\n
*/
void CFep1TestTargetUi::ConstructL()
	{
    RDebug::Print(_L("**************************************************************"));
    RDebug::Print(_L("****           CFep1TestTargetUi::ConstructL                  "));
    RDebug::Print(_L("**************************************************************\n"));

	CTestAppUi::ConstructL();

	iCoeEnv->InstallFepL(KNullUid);
	iCoeEnv->InstallFepL(KUidTFep1);

	TWsEvent theEvent;
	theEvent.SetType(EEventUser);

	RWsSession	ws;
	TInt theRes = ws.Connect();

	if(theRes == KErrNone)
		{
		RWindowGroup& winGroup = iCoeEnv->RootWin();
		theEvent.SetType(EEventUser + 1);
		theEvent.SetTimeNow();
		TInt theId = winGroup.Identifier();
		ws.SendEventToWindowGroup(theId, theEvent);
		}
	ws.Close();
	}





void CFep1TestTargetUi::RunTestStepL(TInt aStep)
	{
	switch(aStep)
		{

//----------------------------------------------------------------------------------------------

/**
   @SYMTestCaseID 		UIF-FEPTEST-0004
   @SYMPREQ				0000
   @SYMTestCaseDesc  	Offer key events to the dialog having the TFEP1 activated.
   @SYMTestPriority 	High
   @SYMTestStatus 		Implemented
   @SYMTestActions 		Simulate key events sent to the dialog box, and do some actions with the fep,
   						like commit, cancel and go back to add a 'missing' letter.
   @SYMTestExpectedResults 	When the dialog is dismissed, the text box reads as is expected.
 */

//----------------------------------------------------------------------------------------------
		case 1:
			SetTestStepID(_L("UIF-FEPTEST-0004"));
		case 2: case 3:
			{
			TUint code = 'A' + aStep - 1;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 4:
			{
			INFO_PRINTF1(_L("...simulate Key Event Enter ---> Submits 'ABC'"));
			SendKey(EKeyEnter);
			}
			break;

		case 5: case 6: case 7:
			{
			TUint code = 'D' + aStep - 5;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 8:
			{
			INFO_PRINTF1(_L("...simulate Key Event Escape ---> Cancels 'DEF'"));
			SendKey(EKeyEscape);
			}
			break;

		case 9: case 10: case 11:
			{
			TUint code = 'H' + aStep - 9;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 12: case 13: case 14: case 15: case 16:
			{
			INFO_PRINTF1(_L("...simulate Key Cursor Left --->  Go left 5 positions (should not go further than the previous 'H' however"));
			SendKey(EKeyLeftArrow);
			}
			break;

		case 17:
			{
			TUint code = 'G' + aStep - 17;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 18:
			{
			INFO_PRINTF1(_L("...simulate Key Event Enter ---> Submits 'GHIJ'"));
			SendKey(EKeyEnter);
			}
			break;

		case 19: case 20: case 21: case 22: case 23:
			{
			INFO_PRINTF1(_L("...simulate Key Cursor Right --->  Go right few positions in case we were within inline editing fep mode"));
			SendKey(EKeyRightArrow);
			if(aStep == 23)
				{
				RecordTestResultL();
				}
			}
			break;


//----------------------------------------------------------------------------------------------

/**
   @SYMTestCaseID 		UIF-FEPTEST-0005
   @SYMPREQ				0000
   @SYMTestCaseDesc  	Repeat the same as the other but changing mode (inline <--> non inline)
   @SYMTestPriority 	High
   @SYMTestStatus 		Implemented
   @SYMTestActions 		Simulate key events sent to the dialog box, and do some actions with the fep,
   						like commit, cancel and go back to add a 'missing' letter.
   @SYMTestExpectedResults 	When the dialog is dismissed, the text box reads as is expected.
 */

//----------------------------------------------------------------------------------------------

		case 51:
			{
			SetTestStepID(_L("UIF-FEPTEST-0005"));
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + I' ---> Changes fep mode!"));
			SendKey('I', EModifierRightShift);
			}
			break;

		case 52: case 53: case 54:
			{
			TUint code = 'K' + aStep - 52;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 55:
			{
			INFO_PRINTF1(_L("...simulate Key Event Enter ---> Submits 'KLM'"));			// Submit "KLM" ----> We should have "ABCGHIJKLM"
			SendKey(EKeyEnter);
			}
			break;

		case 56: case 57: case 58:
			{
			TUint code = 'N' + aStep - 56;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 59:
			{
			INFO_PRINTF1(_L("...simulate Key Event Escape ---> Cancels 'NOP'"));
			SendKey(EKeyEscape);
			}
			break;

		case 60: case 61: case 62:
			{
			TUint code = 'R' + aStep - 60;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 63: case 64: case 65: case 66: case 67:
			{
			INFO_PRINTF1(_L("...simulate Key Cursor Left --->  Go left 5 positions (should not go further than the previous 'R' however"));
			SendKey(EKeyLeftArrow);
			}
			break;

		case 68:
			{
			TUint code = 'Q' + aStep - 68;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 69:
			{
			INFO_PRINTF1(_L("...simulate Key Event Enter ---> Submits 'QRST'"));	// Submit "QRST" ----> We should have "ABCGHIJKLMQRST"
			SendKey(EKeyEnter);
			}
			break;

		case 70: case 71: case 72: case 73: case 74:
			{
			INFO_PRINTF1(_L("...simulate Key Cursor Right --->  Go right few positions in case we were within inline editing fep mode"));
			SendKey(EKeyRightArrow);
			if(aStep == 74)
					{
					RecordTestResultL();
					}
			}
			break;


//----------------------------------------------------------------------------------------------

/**
   @SYMTestCaseID 		UIF-FEPTEST-0006
   @SYMPREQ				0000
   @SYMTestCaseDesc  	Test changing focus
   @SYMTestPriority 	High
   @SYMTestStatus 		Implemented
   @SYMTestActions 		Simulate key events to change focus of the Fep window and move the windows around.
   						In addition, simulate the key event that sends the Fep window to the current dialog cursor.
   @SYMTestExpectedResults 	When the dialog is dismissed, the text box reads as is expected.
 */

//----------------------------------------------------------------------------------------------

		case 101:
			{
			SetTestStepID(_L("UIF-FEPTEST-0006"));
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + N' ---> Moves the Fep window to dialog cursor!"));
			SendKey('N', EModifierRightShift);
			}
			break;

		case 102:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + F' ---> Changes focus!"));
			SendKey('F', EModifierRightShift);
			}
			break;

		case 103: case 104: case 105: case 106: case 107: case 108: case 109: case 110: case 111: case 112: case 113: case 114: case 115:
			{
			INFO_PRINTF1(_L("...send several cursor right events to move the window"));
			for (TInt ii=10; ii>0; --ii)
				SendKey(EKeyRightArrow);
			}
			break;

		case 116:
			{
			INFO_PRINTF1(_L("...simulate Key Event Enter ---> Finishes moving the Fep window!"));
			SendKey(EKeyEnter);
			RecordTestResultL();
			}
			break;


//----------------------------------------------------------------------------------------------

/**
   @SYMTestCaseID 		UIF-FEPTEST-0007
   @SYMPREQ				0000
   @SYMTestCaseDesc  	Test TFEP1 in HEXadecimal mode
   @SYMTestPriority 	High
   @SYMTestStatus 		Implemented
   @SYMTestActions 		Change Fep to HEX mode by means of the corresponding key event.
   						Simulate that the combination of 0x0040 produces the symbol '@'
   @SYMTestExpectedResults 	When the dialog is dismissed, the text box reads as is expected.
 */

//----------------------------------------------------------------------------------------------

		case 151:
			{
			SetTestStepID(_L("UIF-FEPTEST-0007"));
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + M' ---> Changes to hex fep!"));
			SendKey('M', EModifierRightShift);
			}
			break;

		case 153:
			{
			TUint code = '0';
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 154:
			{
			TUint code = '0';
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 155:
			{
			TUint code = '4';
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 156:
			{
			TUint code = '0';
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 157:
			{
			INFO_PRINTF1(_L("...simulate Key Event Enter ---> Submits '0040' ==> At symbol (@)"));
			SendKey(EKeyEnter);
			}
			break;

		case 158:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + M' ---> Changes back to normal fep!"));
			SendKey('M', EModifierRightShift);

		    RDebug::Print(_L("**************************************************************"));
		    RDebug::Print(_L("****           UIF-FEPTEST-0007 Finished                      "));
		    RDebug::Print(_L("**************************************************************\n"));

			RecordTestResultL();
			}
			break;


//----------------------------------------------------------------------------------------------

/**
   @SYMTestCaseID 		UIF-FEPTEST-0008
   @SYMPREQ				0000
   @SYMTestCaseDesc  	Text selection within a dialog box editor.
   @SYMTestPriority 	High
   @SYMTestStatus 		Implemented
   @SYMTestActions 		To test this functionality we simulate a key event to go to the next dialog
   						field (cursor down). Once in there, some numerical key events are sent to the
   						application.
   						The selection of uncommited text and replacing it for new characters is also tested.
   						Simulate the selection of some of the characters in the box, and by means
   						of Fep capabilities, show what's been selected, what's after the selection, and
   						what's the text before the selection.
   @SYMTestExpectedResults 	Fep shows the selection by means of User::InfoPrint() on the screen (partly manual)
   							When the dialog is dismissed, the text box reads as is expected.
 */

//----------------------------------------------------------------------------------------------

		case 201:
			{
			SetTestStepID(_L("UIF-FEPTEST-0008"));
			INFO_PRINTF1(_L("Send cursor down events to move the next dialog"));
			SendKey(EKeyDownArrow);
			}
			break;

		case 202: case 203: case 204:
			{
			TUint code = 'x' + aStep - 202;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 205:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + W' ---> Change cursor type in case of inline editing!"));
			SendKey('W', EModifierRightShift);
			}
			break;

		case 206: case 207: case 208:
			{
			INFO_PRINTF1(_L("...simulate Key Cursor 'shift + Left' --->  Selects non-submitted text"));
			SendKey(EKeyLeftArrow, EModifierLeftShift|EModifierShift);
			}
			break;

		case 209: case 210: case 211:
			{
			TUint code = '1' + aStep - 209;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);	//should replace uncommitted 'xyz' to '123'
			SendKey(code);
			}
			break;

		case 212:
			{
			INFO_PRINTF1(_L("...simulate Key Event Enter ---> Submits '123'"));
			SendKey(EKeyEnter);
			}
			break;

		case 213:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + I' ---> Changes fep mode!"));
			SendKey('I', EModifierRightShift);
			}
			break;

		case 214: case 215: case 216:
			{
			TUint code = 'x' + aStep - 214;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 217:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + W' ---> Change cursor type in case of inline editing!"));
			SendKey('W', EModifierRightShift);
			}
			break;

		case 218: case 219: case 220:
			{
			INFO_PRINTF1(_L("...simulate Key Cursor 'shift + Right' --->  Selects non-submitted text"));
			SendKey(EKeyLeftArrow, EModifierLeftShift|EModifierShift);
			}
			break;

		case 221: case 222: case 223:
			{
			TUint code = '4' + aStep - 221;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);	//should replace uncommitted 'xyz' to '456'
			SendKey(code);
			}
			break;

		case 224:
			{
			INFO_PRINTF1(_L("...simulate Key Event Enter ---> Submits '456'"));
			SendKey(EKeyEnter);
			}
			break;

		case 225: case 226: case 227:
			{
			INFO_PRINTF1(_L("...simulate Key Cursor Left --->  Go left 3 positions (we should be between digits 3 and 4!"));
			SendKey(EKeyLeftArrow);
			}
			break;

		case 228:
			{
			INFO_PRINTF1(_L("...simulate Key Cursor 'shift + Right' --->  Selects 4!"));
			SendKey(EKeyRightArrow, EModifierLeftShift|EModifierShift);
			}
			break;

		case 229:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + A' ---> Tells about what's AFTER the selected 4!"));	// Should be 56
			SendKey('A', EModifierRightShift);
			}
			break;

		case 230:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + S' ---> Tells about what's SELECTED!"));	// Should be 4
			SendKey('S', EModifierRightShift);
			}
			break;

		case 231:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + B' ---> Tells about what's BEFORE the selected 4!"));	// Should be 123
			SendKey('B', EModifierRightShift);
			}
			break;

		case 232: case 233:
			{
			TUint code = '0';
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 234:
			{
			INFO_PRINTF1(_L("...simulate Key Event Enter ---> Submits '00'"));						// We should have 1230056
			SendKey(EKeyEnter);
			RecordTestResultL();
			}
			break;



//----------------------------------------------------------------------------------------------

/**
   @SYMTestCaseID 		UIF-FEPTEST-0009
   @SYMPREQ				0000
   @SYMTestCaseDesc  	Fep state within a dialog box editor.
   @SYMTestPriority 	High
   @SYMTestStatus 		Implemented
   @SYMTestActions 		To test this functionality we simulate a key event to go to change the state
   						variable (in TFEP1 case it's just a counter), repeating the same event few times.
   						he next dialog.
   						Move to the next text editor box and go back to the previous one. State is maintained
   @SYMTestExpectedResults 	The state is maintained even though we move across to other text editor boxes (partly manual)
 */

//----------------------------------------------------------------------------------------------
		case 251:
			SetTestStepID(_L("UIF-FEPTEST-0009"));

		case 252: case 253: case 254: case 255:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + T' ---> Changes State variable!"));
			SendKey('T', EModifierRightShift);
			}
			break;

		case 256:
			{
			INFO_PRINTF1(_L("Send cursor up events to move the next dialog"));
			SendKey(EKeyUpArrow);
			}
			break;

		case 257:
			{
			INFO_PRINTF1(_L("Send cursor down events to move the next dialog"));
			SendKey(EKeyDownArrow);
			}
			break;

		case 258: case 259: case 260: case 261: case 262:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + T' ---> Changes State variable!"));
			SendKey('T', EModifierRightShift);
			if(aStep == 262)
				{
                RDebug::Print(_L("**************************************************************"));
                RDebug::Print(_L("****           UIF-FEPTEST-0009 Finished                      "));
                RDebug::Print(_L("**************************************************************\n"));

				RecordTestResultL();
				}
			}
			break;


//----------------------------------------------------------------------------------------------

/**
   @SYMTestCaseID 		UIF-FEPTEST-0010
   @SYMPREQ				0000
   @SYMTestCaseDesc  	Kata-kata dialog box editor.
   @SYMTestPriority 	High
   @SYMTestStatus 		Implemented
   @SYMTestActions 		To test this functionality we simulate a key event to go to next dialog box
   						editor (Katakana dialog box). Simulate some characters and check that
   						they appear on the box accordingly.
   @SYMTestExpectedResults 	When the dialog is dismissed, the text box reads as is expected.
 */

//----------------------------------------------------------------------------------------------

		case 351:
			{
			SetTestStepID(_L("UIF-FEPTEST-0010"));
			INFO_PRINTF1(_L("Send cursor down events to move the next dialog"));
			SendKey(EKeyDownArrow);
			}
			break;

		case 352: case 353: case 354:
			{
			SetTestStepID(_L("UIF-FEPTEST-0010"));
			TUint code = 'E' + aStep - 352;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			if(aStep == 354)
					{
					RecordTestResultL();
					}
			}
			break;


//----------------------------------------------------------------------------------------------

/**
   @SYMTestCaseID 		UIF-FEPTEST-0011
   @SYMPREQ				0000
   @SYMTestCaseDesc  	Phoneticall dialog box editor.
   @SYMTestPriority 	High
   @SYMTestStatus 		Implemented
   @SYMTestActions 		To test this functionality we simulate a key event to go to next dialog box
   						editor (Phoneticall dialog box). Simulate some characters and check that
   						they appear on the box accordingly, i.e, converting single characters to those
   						in the phoneticall alphabet (A-->Alpha, B-->Bravo...)
   						In addition, uninstall the Fep and write more characters, so they are not
   						phonetically 'translated' by the Fep.
   @SYMTestExpectedResults 	When the dialog is dismissed, the text box reads as is expected.
 */

//----------------------------------------------------------------------------------------------

		case 451:
			{
			SetTestStepID(_L("UIF-FEPTEST-0011"));
			INFO_PRINTF1(_L("Send cursor down events to move the next dialog"));
			SendKey(EKeyDownArrow);
			}
			break;

		case 452: case 453: case 454:
			{
			TUint code = 'J' + aStep - 452;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 455:
			{
			INFO_PRINTF1(_L("...simulate Key Event Enter ---> Submits 'JulietteKiloLima'"));
			SendKey(EKeyEnter);
			}
			break;

		case 456:
			{
			INFO_PRINTF1(_L("Uninstall the FEP"));
			iCoeEnv->InstallFepL(KNullUid);
			User::After(TTimeIntervalMicroSeconds32(2000000));
			}
			break;

		case 457: case 458: case 459:
			{
			TUint code = 'M' + aStep - 457;
			INFO_PRINTF2(_L("...simulate Key Event '%d'"), code);
			SendKey(code);
			}
			break;

		case 460:
			{
			//INFO_PRINTF1(_L("...simulate Key Event Enter ---> Submits 'MNO' (not Fep installed --> No Alphabetical!"));
			//SendKey(EKeyEnter);

		    RDebug::Print(_L("**************************************************************"));
		    RDebug::Print(_L("****           UIF-FEPTEST-0011 Finished                      "));
		    RDebug::Print(_L("**************************************************************\n"));

			RecordTestResultL();
			}
			break;

//----------------------------------------------------------------------------------------------

/**
   @SYMTestCaseID 		TI18N-FEPBASE-CIT-4001
   @SYMPREQ				2471
   @SYMTestCaseDesc  	Check FEPBASE Unicode 5.0 support.
   @SYMTestPriority 	High
   @SYMTestStatus 		Implemented
   @SYMTestActions 		1. Pass BMP code points to WORD.exe
                        2. Pass non-BMP code points to WORD.exe

   @SYMTestExpectedResults 	When the dialog is dismissed, the text box reads as is expected.
 */

//----------------------------------------------------------------------------------------------

		case 461:
			{
			SetTestStepID(_L("TI18N-FEPBASE-CIT-4001"));
			INFO_PRINTF1(_L("Send cursor down events to move the next dialog"));
			SendKey(EKeyDownArrow);
			// FEP 1 is already tested in above test cases.
			// Now install test FEP 4 to test UTF-16 support.
			INFO_PRINTF1(_L("Install the FEP 4"));
			iCoeEnv->InstallFepL(KUidTFep4);
			User::After(TTimeIntervalMicroSeconds32(2000000));
			}
			break;
		case 462:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + M' ---> Changes to hex fep!"));
			SendKey('M', EModifierRightShift);
			TUint codes[] = {'0', '0', '0', '2', '1'};	// FEP 4 supports UTF-16, and need 5 digits for one Unicode value
			for (int i = 0; i < sizeof(codes)/sizeof(TUint); i++)
				{
				INFO_PRINTF2(_L("...simulate Key Event '%d'"), codes[i]);
				SendKey(codes[i]);
				}
			INFO_PRINTF1(_L("...simulate Key Event Enter"));
			SendKey(EKeyEnter);
			}
			break;
		case 463:
			{
			TUint codes[] = {'0', '0', '4', '1', '1'};	// FEP 4 supports UTF-16, and need 5 digit for one Unicode value
			for (int i = 0; i < sizeof(codes)/sizeof(TUint); i++)
				{
				INFO_PRINTF2(_L("...simulate Key Event '%d'"), codes[i]);
				SendKey(codes[i]);
				}
			INFO_PRINTF1(_L("...simulate Key Event Enter"));
			SendKey(EKeyEnter);
			}
			break;
		case 464:
			{
			TUint codes[] = {'0', '0', 'E', '0', '2'};	// FEP 4 supports UTF-16, and need 5 digit for one Unicode value
			for (int i = 0; i < sizeof(codes)/sizeof(TUint); i++)
				{
				INFO_PRINTF2(_L("...simulate Key Event '%d'"), codes[i]);
				SendKey(codes[i]);
				}
			INFO_PRINTF1(_L("...simulate Key Event Enter"));
			SendKey(EKeyEnter);
			}
			break;
		case 465:
			{
			TUint codes[] = {'0', '2', 'E', '8', '1'};	// FEP 4 supports UTF-16, and need 5 digit for one Unicode value
			for (int i = 0; i < sizeof(codes)/sizeof(TUint); i++)
				{
				INFO_PRINTF2(_L("...simulate Key Event '%d'"), codes[i]);
				SendKey(codes[i]);
				}
			INFO_PRINTF1(_L("...simulate Key Event Enter"));
			SendKey(EKeyEnter);
			}
			break;
		case 466:
			{
			TUint codes[] = {'0', '3', '0', '3', 'A'};	// FEP 4 supports UTF-16, and need 5 digit for one Unicode value
			for (int i = 0; i < sizeof(codes)/sizeof(TUint); i++)
				{
				INFO_PRINTF2(_L("...simulate Key Event '%d'"), codes[i]);
				SendKey(codes[i]);
				}
			INFO_PRINTF1(_L("...simulate Key Event Enter"));
			SendKey(EKeyEnter);
			}
			break;
		case 467:
			{
			TUint codes[] = {'0', '4', 'E', '1', '9'};	// FEP 4 supports UTF-16, and need 5 digit for one Unicode value
			for (int i = 0; i < sizeof(codes)/sizeof(TUint); i++)
				{
				INFO_PRINTF2(_L("...simulate Key Event '%d'"), codes[i]);
				SendKey(codes[i]);
				}
			INFO_PRINTF1(_L("...simulate Key Event Enter"));
			SendKey(EKeyEnter);
			}
			break;
		case 468:
			{
			TUint codes[] = {'0', 'A', 'D', '0', '0'};	// FEP 4 supports UTF-16, and need 5 digit for one Unicode value
			for (int i = 0; i < sizeof(codes)/sizeof(TUint); i++)
				{
				INFO_PRINTF2(_L("...simulate Key Event '%d'"), codes[i]);
				SendKey(codes[i]);
				}
			INFO_PRINTF1(_L("...simulate Key Event Enter"));
			SendKey(EKeyEnter);
			}
			break;
		case 469:
			{
			TUint codes[] = {'2', '0', '0', '0', '0'};
			for (int i = 0; i < sizeof(codes)/sizeof(TUint); i++)
				{
				INFO_PRINTF2(_L("...simulate Key Event '%d'"), codes[i]);
				SendKey(codes[i]);
				}
			INFO_PRINTF1(_L("...simulate Key Event Enter"));
			SendKey(EKeyEnter);
			}
			break;
		case 470:
			{
			TUint codes[] =  {'2', 'F', '8', '0', '0'};
			for (int i = 0; i < sizeof(codes)/sizeof(TUint); i++)
				{
				INFO_PRINTF2(_L("...simulate Key Event '%d'"), codes[i]);
				SendKey(codes[i]);
				}
			INFO_PRINTF1(_L("...simulate Key Event Enter"));
			SendKey(EKeyEnter);
			}
			break;
		case 471:
			{
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + X' ---> test SimulateKeyEventsL!"));
			SendKey('X', EModifierRightShift);
			User::After(TTimeIntervalMicroSeconds32(2000000));
			INFO_PRINTF1(_L("...simulate Key Event 'Shift + M' ---> Changes to Pinyin!"));
			SendKey('M', EModifierRightShift);
			User::After(TTimeIntervalMicroSeconds32(2000000));
			}
			break;
		case 472:
			{
			INFO_PRINTF1(_L("Uninstall the FEP 4"));
			iCoeEnv->InstallFepL(KNullUid);
			User::After(TTimeIntervalMicroSeconds32(2000000));
			SendKey(EKeyEnter);

			RDebug::Print(_L("**************************************************************"));
			RDebug::Print(_L("****           TI18N-FEPBASE-CIT-4001 Finished                      "));
			RDebug::Print(_L("**************************************************************\n"));

			RecordTestResultL();
			}
			break;

//----------------------------------------------------------------------------------------------

/**
   @SYMTestCaseID 		UIF-FEPTEST-0012
   @SYMPREQ				0000
   @SYMTestCaseDesc  	Dismiss dialog.
   @SYMTestPriority 	High
   @SYMTestStatus 		Implemented
   @SYMTestActions 		To test this functionality we simulate a key event to dismiss the dialog.
   						When the dialog is closed, CTstTestTargetDialog::OkToExitL() is called and
   						general variables are set with the values from the dialog text editors.
   @SYMTestExpectedResults 	All global variables read as expected.
 */

//----------------------------------------------------------------------------------------------

		case 501:
			{
			SetTestStepID(_L("UIF-FEPTEST-0012"));
			INFO_PRINTF1(_L("Ssimulate Key Enter Event to dismiss the dialog"));
			AutoTestManager().Cancel();
			SendKey(EKeyEnter);
			}
			break;

		case 502:
			{
			INFO_PRINTF1(_L("Wait for 2 seconds..."));
			User::After(TTimeIntervalMicroSeconds32(2000000));
			}
			break;

		case 503:
			{
	         RDebug::Print(_L("**************************************************************"));
	         RDebug::Print(_L("****           Checking dialog values...                      "));
	         RDebug::Print(_L("**************************************************************\n"));

			INFO_PRINTF1(_L("Checking dialog values..."));
			INFO_PRINTF3(_L("gTextNormalEdwin was '%S' (expected '%S')"), &gTextNormalEdwin, &KExpectedTextNormalEdwin);
			INFO_PRINTF3(_L("gTextNormalNumEdwin was '%S' (expected '%S')"), &gTextNormalNumEdwin, &KExpectedTextNormalNumEdwin);
			INFO_PRINTF3(_L("gTextKatakanaEdwin was '%S' (expected '%S')"), &gTextKatakanaEdwin, &KExpectedTextKatakanaEdwin);
			INFO_PRINTF3(_L("gTextPhoneticalEdwin was '%S' (expected '%S')"), &gTextPhoneticalEdwin, &KExpectedTextPhoneticalEdwin);
			INFO_PRINTF3(_L("gTextCJKEdwin was '%S' (expected '%S')"), &gTextCJKEdwin, &KExpectedTextCJKEdwin);

			TEST(gTextNormalEdwin == KExpectedTextNormalEdwin);
			TEST(gTextNormalNumEdwin == KExpectedTextNormalNumEdwin);
			TEST(gTextKatakanaEdwin == KExpectedTextKatakanaEdwin);
			TEST(gTextPhoneticalEdwin == KExpectedTextPhoneticalEdwin);
			TEST(gTextCJKEdwin == KExpectedTextCJKEdwin);
			RecordTestResultL();
			CloseTMSGraphicsStep();
			}
			break;

		case 504:
			{
			AutoTestManager().FinishAllTestCases(CAutoTestManager::EPass);
			}
			break;
		}

	}



void CFep1TestTargetUi::SendKey(TUint aCode, TUint aModifiers, TInt aRepeats, TInt aScanCode)
	{
	TWsEvent theEvent;
	TKeyEvent *theKey = theEvent.Key();
	theKey->iScanCode = aScanCode;
	theKey->iModifiers = aModifiers;
	theKey->iRepeats = aRepeats;
	theKey->iCode = aCode;
	SendEventToWindowGroups(theEvent);
	}


/**
  Handle the window events.\n
  Create a window server session and connect the client\n
  Create a client side handle for the server.\n
  Set the window event type and call the SendEventToWindowGroup().\n
  Close the session on completion \n
*/
void CFep1TestTargetUi::SendEventToWindowGroups(TWsEvent&	aEvent)
	{
User::After(100000);

	RWsSession	ws;
	TInt theRes = ws.Connect();
	TEST(theRes == KErrNone);

	RWindowGroup& winGroup = iCoeEnv->RootWin();
	TInt theId = winGroup.Identifier();

	aEvent.SetType(EEventKeyDown);
	aEvent.SetTimeNow();
	ws.SendEventToWindowGroup(theId, aEvent);

	aEvent.SetType(EEventKey);
	aEvent.SetTimeNow();
	ws.SendEventToWindowGroup(theId, aEvent);

	aEvent.SetType(EEventKeyUp);
	aEvent.SetTimeNow();
	ws.SendEventToWindowGroup(theId, aEvent);

	ws.Close();
	}

/**
  Handle the user defined events.\n
  Search for the resource file that support the language.\n
  Adds the specified resource file to the list maintained by CCoeEnv. \n
*/
void CFep1TestTargetUi::HandleApplicationSpecificEventL(TInt aType,const TWsEvent& aEvent)
	{
	if(aEvent.Type() == (EEventUser + 1))
		{
		CCoeEnv& coneEnvironment=*CCoeEnv::Static();
		TFileName* resourceFileName=new(ELeave) TFileName(KFep1TestTargetResourceFilePath);

		TBuf8<128> buf;
		buf.Copy(*resourceFileName);

		CleanupStack::PushL(resourceFileName);
		BaflUtils::NearestLanguageFile(coneEnvironment.FsSession(), *resourceFileName);
		TTstResourceFileId resourceFileId(coneEnvironment, coneEnvironment.AddResourceFileL(*resourceFileName));
		CleanupStack::PopAndDestroy(resourceFileName);
		CleanupStack::PushL(resourceFileId);
		CTstTestTargetDialog* dlg = new(ELeave) CTstTestTargetDialog;

		AutoTestManager().StartAutoTest();
		dlg->ExecuteLD(R_TFT_DIALOG);
		CleanupStack::PopAndDestroy(&resourceFileId);
		}
	else
		{
		CTestAppUi::HandleApplicationSpecificEventL(aType, aEvent);
		}
	}


/*************************************************************
 **
 **  CTestFep1TargetStep
 **
 *************************************************************/

CTestFep1TargetStep::CTestFep1TargetStep()
	{
	SetTestStepName(KFep1TestTargetStep);
	}

CTestFep1TargetStep::~CTestFep1TargetStep()
	{
	}


/**
  Call ConstructL() of CEikonEnv and initialize a pointer to CFep1TestTargetUi \n
  Set the application's user interface object \n
  Call ConstructL() of CFep1TestTargetUi
*/
void CTestFep1TargetStep::ConstructAppL(CEikonEnv* aCoe)
    { // runs inside a TRAP harness
	aCoe->ConstructL();
	CTestAppUi* appUi= new (ELeave) CFep1TestTargetUi(this);
    aCoe->SetAppUi(appUi);
    appUi->ConstructL();
	}


/**
   Entry function for the test steps.\n
   Provide an active environment for creating controls and utility functions \n
   Launches the application using ExecuteD().\n
 */
TVerdict CTestFep1TargetStep::doTestStepL()
	{
	PreallocateHALBuffer();

	INFO_PRINTF1(_L("TestFep1Target started..."));

	__UHEAP_MARK;

	CEikonEnv* coe=new(ELeave) CEikonEnv;
	TRAPD(err,ConstructAppL(coe));

	if (!err)
		coe->ExecuteD();
	else
		{
		delete coe;
		SetTestStepResult(EFail);
		}

	__UHEAP_MARKEND;

	INFO_PRINTF1(_L("...TestFep1Target finished!"));

	return TestStepResult();
	}


