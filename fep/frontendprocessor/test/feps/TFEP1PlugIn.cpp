// Copyright (c) 2005-2009 Nokia Corporation and/or its subsidiary(-ies).
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
 @internalComponent
*/

#include <bautils.h>
#include <ecom/implementationproxy.h>
#include <ecom/ecom.h>

#include <tfep1_ui.rsg>

#include "TFEP1.H"
#include "TFEP1PlugIn.H"
#include "TESTFEPRESFILEPATH.H"



// CTstFepPlugIn

const TInt KTstFepPlugInImplementationValue = 0x102024D0;

const TImplementationProxy ImplementationTable[] = 
	{
	IMPLEMENTATION_PROXY_ENTRY(KTstFepPlugInImplementationValue, CTstFepPlugIn::NewL )
	};

EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)
	{
	aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);
	return ImplementationTable;
	}


CTstFepPlugIn* CTstFepPlugIn::NewL()
	{ // static
	return new(ELeave) CTstFepPlugIn;
	}

CCoeFep* CTstFepPlugIn::NewFepL(CCoeEnv& aConeEnvironment, const CCoeFepParameters& aFepParameters)
	{
	CTstFep* const fep=new(ELeave) CTstFep(aConeEnvironment);
	CleanupStack::PushL(fep);
	fep->ConstructL(aFepParameters);
	CleanupStack::Pop(fep);
	return fep;
	}

void CTstFepPlugIn::SynchronouslyExecuteSettingsDialogL(CCoeEnv& aConeEnvironment)
	{
	_LIT(KLitResourceFileName,"TFEP1PlugIn.rsc");
	
	TFileName* resourceFileName=new(ELeave) TFileName;
	CleanupStack::PushL(resourceFileName);
	Dll::FileName(*resourceFileName); // Get the drive letter

	TParse* parse=new(ELeave) TParse;
	CleanupStack::PushL(parse);
	User::LeaveIfError(parse->SetNoWild(KLitResourceFileName, &KTestFepResFilePath, resourceFileName));
	resourceFileName->Copy(parse->FullName());
	CleanupStack::PopAndDestroy(parse);
	
	BaflUtils::NearestLanguageFile(aConeEnvironment.FsSession(), *resourceFileName);
	TTstResourceFileId resourceFileId(aConeEnvironment, aConeEnvironment.AddResourceFileL(*resourceFileName)); // object must not be an anonymous temporary passed into CleanupStack::PushL, as its lifetime would be too short
	CleanupStack::PopAndDestroy(resourceFileName);
	CleanupStack::PushL(resourceFileId);
	(new(ELeave) CTstSettingsDialog)->ExecuteLD(R_TFP_SETTINGS_DIALOG);
	CleanupStack::PopAndDestroy(&resourceFileId);
	}
