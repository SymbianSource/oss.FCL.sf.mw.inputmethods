// Copyright (c) 2006-2009 Nokia Corporation and/or its subsidiary(-ies).
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
//

/**
 @file
 @test
 @internalComponent - Internal Symbian test code
*/

#include <e32std.h>
#include <e32base.h>
#include <gdi.h>
#include <w32std.h>
#include <coemain.h>
#include <coeaui.h>
#include <coecntrl.h>
#include <coeinput.h>
#include <e32math.h>
#include <eikenv.h>

#include "KEYBOARDLOGGER.H"


//Timer dealy after which event is requested.\n
const TInt KClearerDelay(1000000);

enum
	{
	EGapLeftOfEachLine	=10,
	EGapAboveTopLine	=1,
	EGapBetweenEachLine	=3,
	EGapBelowBottomLine	=3
	};
	
//String literal to represent the name of the typeface used.\n
_LIT(KLitFontTypefaceName, "publicDomainUnicode");


#if defined(_DEBUG)

_LIT(KLitKEYBOARDLOGGER, "KEYBOARDLOGGER");

enum TPanic
	{
	EPanicBadHeight=1
	};

LOCAL_C void Panic(TPanic aPanic)
	{
	User::Panic(KLitKEYBOARDLOGGER, aPanic);
	}

#endif


/*************************************************************
 **  
 **  TTstOverflowIgnorer
 **  
 *************************************************************/

void TTstOverflowIgnorer::Overflow(TDes16&)
	{
	}


/*************************************************************
 **  
 **  CTstControl
 **  
 *************************************************************/

CTstControl* CTstControl::NewL()
	{
	CTstControl* const control=new(ELeave) CTstControl;
	CleanupStack::PushL(control);
	control->ConstructL();
	CleanupStack::Pop(control);
	return control;
	}

CTstControl::~CTstControl()
	{
	iCoeEnv->ReleaseScreenFont(iFont);
	iArrayOfKeyDownEvents.Close();
	iArrayOfKeyEvents.Close();
	iArrayOfKeyUpEvents.Close();
	delete iClearer;
	}

CTstControl::CTstControl()
	:iFont(NULL),
	 iArrayOfKeyDownEvents(2),
	 iArrayOfKeyEvents(2),
	 iArrayOfKeyUpEvents(2),
	 iClearer(NULL)
	{
	}

/**
  Create a control window \n Enable to receive pointer drag and move events.\n
  Ensures that all subsequent pointer events are delivered \n 
  Gets the control's associated drawable window and set its shadow height\n
  Create a font for the device.\n Set the screen size and font height \n
  Set controls extent with size and position and set the control readt to be drawn \n
*/
void CTstControl::ConstructL()
	{
	CreateWindowL();
	EnableDragEvents();
	ClaimPointerGrab();
	DrawableWindow()->SetShadowHeight(3);
	iFont=iCoeEnv->CreateScreenFontL(TFontSpec(KLitFontTypefaceName, 200));
	iClearer=CClearer::NewL(*this);
	const TSize screenSize(iCoeEnv->ScreenDevice()->SizeInPixels());
	const TInt fontHeightInPixels=iFont->HeightInPixels();
	SetExtent(TPoint(10, 10), TSize(screenSize.iWidth-(10*2), 1+EGapAboveTopLine+(2*(fontHeightInPixels+EGapBetweenEachLine))+fontHeightInPixels+EGapBelowBottomLine+1));
	ActivateL();
	}

/**
  @return - Event handling flag.(EKeyWasConsumed)\n
  The control handles the key events\n
  Draw the control\n
  When a key event occurs, the control framework calls this function for each control on the control stack,\n
  until one of them can process the key event.\n 
*/
TKeyResponse CTstControl::OfferKeyEventL(const TKeyEvent& aKeyEvent, TEventCode aEventCode)
	{
	iClearer->Cancel();
	iClearer->After(TTimeIntervalMicroSeconds32(KClearerDelay));
	
	switch (aEventCode)
		{
		case EEventKeyDown:
			User::LeaveIfError(iArrayOfKeyDownEvents.Append(aKeyEvent));
			break;
		case EEventKey:
			if (aKeyEvent.iCode==EKeyEscape)
				{
		        CBaActiveScheduler::Exit();
				return EKeyWasConsumed;
				}
			User::LeaveIfError(iArrayOfKeyEvents.Append(aKeyEvent));
			break;
		case EEventKeyUp:
			User::LeaveIfError(iArrayOfKeyUpEvents.Append(aKeyEvent));
			break;
		default:
			__DEBUGGER();
			break;
			}
	
	DrawNow();
	return EKeyWasConsumed;
	}

/**
  Auxiliary Function for Test Case ID TestKeyBoardLoggerUi-RunTestStepL.\n
  Return the capability that indicated support for all types of text.\n
*/
TCoeInputCapabilities CTstControl::InputCapabilities() const
	{
	return TCoeInputCapabilities(TCoeInputCapabilities::EAllText);
	}

/**
  The screen appearance of control is changed by overriding the method Draw().\n
  By default, this draws a border of the appropriate type around the control.
*/ 
void CTstControl::Draw(const TRect&) const
	{
	CWindowGc& graphicsContext=SystemGc();
	graphicsContext.SetPenStyle(CGraphicsContext::ESolidPen);
	graphicsContext.SetPenColor(KRgbBlack);
	graphicsContext.SetBrushStyle(CGraphicsContext::ENullBrush);
	TRect rectangle(Rect());
	graphicsContext.DrawRect(rectangle);
	rectangle.Shrink(1, 1);
	graphicsContext.SetBrushStyle(CGraphicsContext::ESolidBrush);
	TRect temp(rectangle);
	temp.iBr.iY=temp.iTl.iY;
	const TInt fontHeightInPixels=iFont->HeightInPixels();
	const TInt fontAscentInPixels=iFont->AscentInPixels();
	graphicsContext.SetBrushColor(KRgbGray);
	graphicsContext.UseFont(iFont);
	TBuf<200> textToDisplay;
	TTstOverflowIgnorer overflowIgnorer;
	TInt i;
	TInt n;
	n=iArrayOfKeyDownEvents.Count();
	textToDisplay.Format(_L("Key-down events (%d): "), n);
	for (i=0; i<n; ++i)
		{
		textToDisplay.AppendFormat(_L("0x%x "), &overflowIgnorer, iArrayOfKeyDownEvents[i].iScanCode);
		}
	temp.iTl.iY=temp.iBr.iY;
	temp.iBr.iY+=EGapAboveTopLine+fontHeightInPixels+EGapBetweenEachLine;
	graphicsContext.DrawText(textToDisplay, temp, EGapAboveTopLine+fontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
	n=iArrayOfKeyEvents.Count();
	textToDisplay.Format(_L("Key events (%d): "), n);
	for (i=0; i<n; ++i)
		{
		textToDisplay.AppendFormat(_L("[0x%x,%c] "), &overflowIgnorer, iArrayOfKeyEvents[i].iScanCode, iArrayOfKeyEvents[i].iCode);
		}
	temp.iTl.iY=temp.iBr.iY;
	temp.iBr.iY+=fontHeightInPixels+EGapBetweenEachLine;
	graphicsContext.DrawText(textToDisplay, temp, fontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
	n=iArrayOfKeyUpEvents.Count();
	textToDisplay.Format(_L("Key-up events (%d): "), n);
	for (i=0; i<n; ++i)
		{
		textToDisplay.AppendFormat(_L("0x%x "), &overflowIgnorer, iArrayOfKeyUpEvents[i].iScanCode);
		}
	temp.iTl.iY=temp.iBr.iY;
	temp.iBr.iY=rectangle.iBr.iY;
	__ASSERT_DEBUG(temp.iBr.iY==temp.iTl.iY+fontHeightInPixels+EGapBelowBottomLine, Panic(EPanicBadHeight));
	graphicsContext.DrawText(textToDisplay, temp, fontAscentInPixels, CGraphicsContext::ELeft, EGapLeftOfEachLine);
	graphicsContext.DiscardFont();
	}


// CTstControl::CClearer

CTstControl::CClearer* CTstControl::CClearer::NewL(CTstControl& aControl)
	{
	CClearer* clearer=new(ELeave) CClearer(aControl);
	CleanupStack::PushL(clearer);
	clearer->ConstructL();
	CleanupStack::Pop(clearer);
	return clearer;
	}

CTstControl::CClearer::CClearer(CTstControl& aControl)
	:CTimer(EActivePriorityDefault),
	 iControl(aControl)
	{
	CActiveScheduler::Add(this);
	}

/**
  The function is called by the active scheduler when a request completion event occurs. \n
  Reset the events to default and call the DrawNow()\n 
*/
void CTstControl::CClearer::RunL()
	{
	iControl.iArrayOfKeyDownEvents.Reset();
	iControl.iArrayOfKeyEvents.Reset();
	iControl.iArrayOfKeyUpEvents.Reset();
	iControl.DrawNow();
	}


/*************************************************************
 **  
 **  CTestKeyBoardLoggerUi 
 **  
 *************************************************************/

CTestKeyBoardLoggerUi::~CTestKeyBoardLoggerUi()
	{
	if(iControl)
		{
		RemoveFromStack(iControl);
		}
	delete iControl;
	}

/**
  Call ConstructL() of CTestAppUi \n
  Initialize the CTstControl object  \nCall ConstructL of CTstControl \n
  Install the FEP\n  
*/
void CTestKeyBoardLoggerUi::ConstructL()
	{
	CTestAppUi::ConstructL();
	iControl= new (ELeave) CTstControl;
	AddToStackL(iControl,ECoeStackPriorityDialog);
	iControl->ConstructL();
	
	const TUid KUidTFep1 = { 0x102024D0 };
	iCoeEnv->InstallFepL(KUidTFep1);

	AutoTestManager().StartAutoTest();
	}



/**
   @SYMTestCaseID 		UIF-FEPTEST-0002
   @SYMPREQ				0000
   @SYMTestCaseDesc  	Launch the application and offer events.
   @SYMTestPriority 	High 
   @SYMTestStatus 		Implemented
   @SYMTestActions 		Launch an application and load the FEP.
   						Create character codes that represent the Key events, Key up and down events.
   						Offer these Key events to the application.
   						The FEP intercepts these events and gets displayed on the application view.
   @SYMTestExpectedResults 	Failure in FEP will cause crashing of the application
 */
void CTestKeyBoardLoggerUi::RunTestStepL(TInt aNumStep)
	{
	TWsEvent theEvent;
	TKeyEvent *theKey = theEvent.Key();

	switch(aNumStep)
		{
		case 1:
			SetTestStepID(_L("UIF-FEPTEST-0002"));
		case 2: case 4: case 5:
			{
			theKey->iScanCode = 0;
			theKey->iModifiers= 0;
			theKey->iRepeats=0;
			theKey->iCode = 'A' + aNumStep - 1;

			SendEventToWindowGroups(theEvent);
			INFO_PRINTF2(_L("Simulate Key Event with code %d"), theKey->iCode);
			}
			break;
			
		case 3: case 6:
			{
		
			theKey->iScanCode = 0;
			theKey->iModifiers= 0;
			theKey->iRepeats=0;
			theKey->iCode = EKeyEnter;

			INFO_PRINTF1(_L("Simulate Key Enter Event"));
			SendEventToWindowGroups(theEvent);
			}
			break;
			
		case 7: case 8:
			{
		
			theKey->iScanCode = EKeyTab;
			theKey->iModifiers= 0;
			theKey->iRepeats=0;
			theKey->iCode = EKeyTab;

			INFO_PRINTF1(_L("Move to next editor"));
			SendEventToWindowGroups(theEvent);
			}
			break;
		
		case 9:
			iCoeEnv->InstallFepL(KNullUid);
			AutoTestManager().FinishAllTestCases(CAutoTestManager::EPass);
			RecordTestResultL();
			CloseTMSGraphicsStep();
			break;
		}
	}

/**
  Handle the window events.\n
  Create a window server session and connect the client\n
  Set the event type and call the SendEventToWindowGroup().\n
  Close the session on completion \n
*/
void CTestKeyBoardLoggerUi::SendEventToWindowGroups(TWsEvent&	aEvent)
	{
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


/*************************************************************
 **  
 **  CTestKeyboardLoggerStep
 **  
 *************************************************************/

CTestKeyboardLoggerStep::CTestKeyboardLoggerStep()
	{
	SetTestStepName(KKeyboardLoggerStep);
	}

CTestKeyboardLoggerStep::~CTestKeyboardLoggerStep()
	{
	}

void CTestKeyboardLoggerStep::ConstructAppL(CEikonEnv* aCoe)
    { // runs inside a TRAP harness
	aCoe->ConstructL();

	CTestAppUi* appUi= new (ELeave) CTestKeyBoardLoggerUi(this);
    aCoe->SetAppUi(appUi);
    appUi->ConstructL();
	}

TVerdict CTestKeyboardLoggerStep::doTestStepL()
	{
	PreallocateHALBuffer();

	INFO_PRINTF1(_L("TestKeyboardLoggerStep started..."));

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

	INFO_PRINTF1(_L("...TestKeyboardLoggerStep finished!"));

	return TestStepResult();
	}
