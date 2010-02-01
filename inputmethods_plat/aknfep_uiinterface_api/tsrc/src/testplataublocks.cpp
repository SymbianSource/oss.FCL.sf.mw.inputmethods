/*
* Copyright (c) 2002 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:  test functions in aknfep uiinterface api
*
*/



// [INCLUDE FILES]
#include <e32svr.h>
#include <StifParser.h>
#include <StifTestInterface.h>
#include <aknfepuimenus.h>

#include "testcaknfepuiinterfaceimplement.h"
#include "testplatau.h"


// FORWARD DECLARATION 
// CTestInterfaceMenuPaneImplement for testing CAknFepUiInterfaceMenuPane
class CTestInterfaceMenuPaneImplement : public CAknFepUiInterfaceMenuPane
    {
public:
    // Constructor
    CTestInterfaceMenuPaneImplement() {}
    
    //inherit from CAknFepUiInterfaceMenuPane
    void ConstructMenuSctRowFromDialogL( TInt /*aCharCase*/, 
                                         TDes& /*aSpecialChars*/, 
                                         TInt /*aResourceId*/ ) 
        {
        
        }
    void ConstructMenuSctRowL( TDes& /*aSpecialChars*/, TInt /*aResourceId*/ ) {}
    void DeleteMenuItem( TInt /*aCommandId*/ ) {} 
    void InsertMenuItemL( const SItemData& /*aMenuItem*/, TInt /*aPosition*/ ) {}
    
    TBool MenuItemExists( TInt /*aCommandId*/, TInt& /*aPosition*/ )
        {
        return ETrue;
        }
    
    TInt NumberOfItemsInPane() const
        {
        return ETrue;
        }
    
    void SetItemDimmed( TInt /*aCommandId*/, TBool /*aDimmed*/ ) {}
    void SetMenuPane( CEikMenuPane* /*aMenuPane*/ ) {}
    CEikMenuPane* GetMenuPane() 
        {
        return NULL;
        }
    };

// CTestInterfaceMenuBarImplement for testing CAknFepUiInterfaceMenuBar
class CTestInterfaceMenuBarImplement : public CAknFepUiInterfaceMenuBar
	{
public:
	// Constructor
    CTestInterfaceMenuBarImplement() {}
    
    //inherit from CAknFepUiInterfaceMenuBar
    CAknFepUiInterfaceMenuPane* MenuPane() 
        {
        return NULL;
        }
    void SetMenuType( TMenuType /*aMenuType*/ ) {}
    void StopDisplayingMenuBar() {}
    void ReplaceAllMenuPanes( TInt /*aResourceId*/ ) {}
    void TryDisplayMenuBarL() {}
    void SetFocus( TBool /*aFocus*/ ) {}
    void RemoveFromStack() {}
    
    TInt FirstMenuPaneResourceId()
        {
        return NULL;
        }
    
    TBool IsDisplayed() 
        {
        return NULL;
        }
    
    void AddToStackL( TInt /*aPriority*/,TInt /*aStackingFlags*/ ) {}
    void TryDisplayMenuBarWithoutFepMenusL() {}
    void SetMenuTitleResourceId( TInt /*aResourceId*/ ) {}
    void SetEditMenuObserver( 
        MAknFepUiInterfaceMenuObserver* /*aEditMenuObserver*/ ) {}
    void RemoveEditMenuObserver( 
        MAknFepUiInterfaceMenuObserver* /*aEditMenuObserver*/ ) {}
    };

// CTestMenuPaneBarLImplement for testing MAknFepUiInterfaceMenuObserver
class CTestMenuPaneBarLImplement : public CBase, public MAknFepUiInterfaceMenuObserver
    {
public:
    // Constructor
    CTestMenuPaneBarLImplement(): CBase() {}
    
    // inherit from MAknFepUiInterfaceMenuObserver
    void SetEmphasis( CBase* /*aMenuControl*/,TBool /*aEmphasis*/ ) {}
    
    // inherit from MEikCommandObserver
    void ProcessCommandL( TInt /*aCommandId*/ ) {}
    };

// ============================ MEMBER FUNCTIONS ===============================

// -----------------------------------------------------------------------------
// CTestPlatAU::RunMethodL
// Run specified method. Contains also table of test mothods and their names.
// -----------------------------------------------------------------------------
//
TInt CTestPlatAU::RunMethodL( CStifItemParser& aItem ) 
    {

    static TStifFunctionInfo const KFunctions[] =
        {  
        // First string is the function name used in TestScripter script file.
        // Second is the actual implementation member function. 
                
        // for MAknFepUiInterfaceMenuObserver in AknFepUiMenus.h
        ENTRY( "TestAUDynInitMenuPaneL", CTestPlatAU::TestAUDynInitMenuPaneL ),
        ENTRY( "TestAUDynInitMenuBarL", CTestPlatAU::TestAUDynInitMenuBarL ),

        // for CAknFepUIInterface in AknFepUiInterface.h
        ENTRY( "TestCAknUINewL", CTestPlatAU::TestCAknUINewL ),
        
        // [test cases entries]

        };

    const TInt count = sizeof( KFunctions ) / sizeof( TStifFunctionInfo );

    return RunInternalL( KFunctions, count, aItem );

    }

// -----------------------------------------------------------------------------
// CTestPlatAU::TestAUDynInitMenuPaneL
// -----------------------------------------------------------------------------
//
TInt CTestPlatAU::TestAUDynInitMenuPaneL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( KTestPlatAU, "testplatau" );
    _LIT( KTestAUDynInitMenuPaneL, "In TestAUDynInitMenuPaneL" );
    TestModuleIf().Printf( 0, KTestPlatAU, KTestAUDynInitMenuPaneL );
    // Print to log file
    iLog->Log( KTestAUDynInitMenuPaneL );

    //create paramenter
    CAknFepUiInterfaceMenuPane* menuPaneImplementObj    = 
                                new ( ELeave ) CTestInterfaceMenuPaneImplement();
    CleanupStack::PushL( menuPaneImplementObj );
    
    CTestMenuPaneBarLImplement* menuPaneBarImplementObj = 
                                     new ( ELeave ) CTestMenuPaneBarLImplement();
    CleanupStack::PushL( menuPaneBarImplementObj );
    
    //test member function DynInitMenuPaneL
    TInt resourceId = 0;
    menuPaneBarImplementObj->DynInitMenuPaneL( resourceId, menuPaneImplementObj );
    
    CleanupStack::PopAndDestroy( menuPaneBarImplementObj );
    CleanupStack::PopAndDestroy( menuPaneImplementObj );
    
    return KErrNone;

    }

// -----------------------------------------------------------------------------
// CTestPlatAU::TestAUDynInitMenuBarL
// -----------------------------------------------------------------------------
//
TInt CTestPlatAU::TestAUDynInitMenuBarL( CStifItemParser& /*aItem*/ )
    {

    // Print to UI
    _LIT( KTestPlatAU, "testplatau" );
    _LIT( KTestAUDynInitMenuBarL, "In TestAUDynInitMenuBarL" );
    TestModuleIf().Printf( 0, KTestPlatAU, KTestAUDynInitMenuBarL );
    // Print to log file
    iLog->Log( KTestAUDynInitMenuBarL );

    //create paramenter
    CTestInterfaceMenuBarImplement* menuBarImplementObj = 
                                 new ( ELeave ) CTestInterfaceMenuBarImplement();
    CleanupStack::PushL( menuBarImplementObj );
    
    CTestMenuPaneBarLImplement* menuPaneBarImplementObj = 
                                     new ( ELeave ) CTestMenuPaneBarLImplement();
    CleanupStack::PushL( menuPaneBarImplementObj );
    
    //test member function DynInitMenuBarL
    TInt resourceId = 0;
    menuPaneBarImplementObj->DynInitMenuBarL( resourceId, menuBarImplementObj );
    
    CleanupStack::PopAndDestroy( menuPaneBarImplementObj );
    CleanupStack::PopAndDestroy( menuBarImplementObj );
    
    return KErrNone;
    
    }

// -----------------------------------------------------------------------------
// CTestPlatAU::TestCAknUINewL
// -----------------------------------------------------------------------------
//
TInt CTestPlatAU::TestCAknUINewL( CStifItemParser& /*aItem*/ )
    {
    
    // Print to UI
    _LIT( KTestPlatAU, "testplatau" );
    _LIT( KTestCAknUINewL, "In TestCAknUINewL" );
    TestModuleIf().Printf( 0, KTestPlatAU, KTestCAknUINewL );
    // Print to log file
    iLog->Log( KTestCAknUINewL );
    
    CTestCAknFepUIInterfaceImplement* cfuiObj = new ( ELeave )
            CTestCAknFepUIInterfaceImplement;
    CleanupStack::PushL( cfuiObj );
    cfuiObj->NewParentL();
    
    CleanupStack::PopAndDestroy( cfuiObj );
    return KErrNone;  
    }

//  [End of File]


