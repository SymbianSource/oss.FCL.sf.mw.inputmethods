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
* Description:  vkb layout	pool management
*
*/


#include <peninputvkbcn.rsg>
#include <peninputvkbctrllayout.h>
#include <coemain.h>
 
#include "peninputvkblayoutpool.h"
#include "peninputvkbImlayout.h"

// ========	MEMBER FUNCTIONS ========

// ------------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
//
CAknFepLayoutPool* CAknFepLayoutPool::NewL( CAknFepCtrlVkbLayout& aVkbLayout )
	{
	CAknFepLayoutPool* self	= NewLC( aVkbLayout );
	CleanupStack::Pop();
	
	return self;
	}

// ------------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
//
CAknFepLayoutPool* CAknFepLayoutPool::NewLC( CAknFepCtrlVkbLayout& aVkbLayout )
	{
	CAknFepLayoutPool* self	= new (ELeave) CAknFepLayoutPool();

	CleanupStack::PushL(self);
	self->ConstructL( aVkbLayout );	

	return self;
	}

// ------------------------------------------------------------------------
// Destructor
// ------------------------------------------------------------------------
// 
CAknFepLayoutPool::~CAknFepLayoutPool()
	{
	iImLayoutList.ResetAndDestroy();
	iImLayoutList.Close();
	}

// ------------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
//
CAknFepLayoutPool::CAknFepLayoutPool() 
	{
	}

// ------------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
//	  
void CAknFepLayoutPool::ConstructL( CAknFepCtrlVkbLayout& aVkbLayout )  
	{
	ConstructFromResourceL( aVkbLayout );	 
	}

// ---------------------------------------------------------
// Construct from resource file
// ---------------------------------------------------------
//		
void CAknFepLayoutPool::ConstructFromResourceL( CAknFepCtrlVkbLayout& aVkbLayout )
	{
    CCoeEnv* coeEnv = CCoeEnv::Static();
	TResourceReader	reader;
	
	//Read imlayout	from resource
	coeEnv->CreateResourceReaderLC(reader, R_AKN_FEP_VKB_IM_LAYOUT_LIST);
	
	TInt count = reader.ReadInt16();
	TInt i;
	
	for	(i = 0;	i <	count; i++)
		{
		CAknFepVkbImLayout*	imLayout = CAknFepVkbImLayout::NewL(reader);
		AddImLayout(imLayout);	  
		} 
			   
	CleanupStack::PopAndDestroy(); // reader

	//Read vkblayout from resource
    aVkbLayout.SetResourceId( R_AKN_FEP_VKB_VKB_LAYOUT_LIST );
    aVkbLayout.ConstructFromResourceL();
    iVkbLayoutInfoList = aVkbLayout.VkbLayoutInfoList();	
	}

// ---------------------------------------------------------
// Add a image layout to array
// ---------------------------------------------------------
// 
void CAknFepLayoutPool::AddImLayout(CAknFepVkbImLayout*	aImLayout) 
	{
	iImLayoutList.Append(aImLayout);	
	}

// ---------------------------------------------------------
// Remove one image	layout from	array based	on image layout	id
// ---------------------------------------------------------
//			  
void CAknFepLayoutPool::RemoveImLayout(const TInt aImLayout) 
	{
	const TInt count = iImLayoutList.Count();

	for	(TInt i	= 0; i < count;	i++)
		{
		if (iImLayoutList[i]->LayoutID() ==	aImLayout)
			{
			CAknFepVkbImLayout*	ImLayout = iImLayoutList[i]; 
			delete ImLayout;
			iImLayoutList.Remove(i);

			return;
			}
		}
	}
	
// ---------------------------------------------------------
// Get one image layout	from array based on	image layout id
// ---------------------------------------------------------
// 
CAknFepVkbImLayout*	CAknFepLayoutPool::GetImLayout(const TInt aImLayout) const	
	{
	const TInt count = iImLayoutList.Count();

	for	(TInt i	= 0; i < count;	i++)
		{
		if (iImLayoutList[i]->LayoutID() ==	aImLayout)
			{
			return iImLayoutList[i];
			}
		}

	return NULL;
	}

// ---------------------------------------------------------
// Add one image layout	to array 
// ---------------------------------------------------------
// 
void CAknFepLayoutPool::AddVkbLayout(CPeninputVkbLayoutInfo* aVkbLayout) 
	{
	iVkbLayoutInfoList.Append(aVkbLayout);	  
	}

// ---------------------------------------------------------
// Remove one vkb layout from array	based on vkb layout	id
// ---------------------------------------------------------
//			  
void CAknFepLayoutPool::RemoveVkbLayout(const TInt aVkbLayout) 
	{
	const TInt count = iVkbLayoutInfoList.Count();
	
	for	(TInt i	= 0; i < count;	i++)
		{
		if (iVkbLayoutInfoList[i]->LayoutID() == aVkbLayout)
			{
			CPeninputVkbLayoutInfo* VkbLayout = iVkbLayoutInfoList[i];	
			delete VkbLayout;
			iVkbLayoutInfoList.Remove(i);

			return;
			}
		}
	}

// ---------------------------------------------------------
// Get one vkb layout from array based on vkb layout id
// ---------------------------------------------------------
//
CPeninputVkbLayoutInfo* CAknFepLayoutPool::GetVkbLayout(const TInt	aVkbLayout)	const	
	{
	const TInt count = iVkbLayoutInfoList.Count();
	
	for	(TInt i	= 0; i < count;	i++)
		{
		if (iVkbLayoutInfoList[i]->LayoutID() == aVkbLayout)
			{
			return iVkbLayoutInfoList[i];
			}
		}

	return NULL;
	}

// ---------------------------------------------------------
// Get one vkb layout from array based on vkb layout id
// ---------------------------------------------------------
//
void CAknFepLayoutPool::ResetImLayout()
	{
	iImLayoutList.ResetAndDestroy();		
	}

// ------------------------------------------------------------------------
// Remove vkb layout and delete	them			
// ------------------------------------------------------------------------
// 
void CAknFepLayoutPool::ResetVkbLayout()
	{
	iVkbLayoutInfoList.ResetAndDestroy();	 
	}
	
// ------------------------------------------------------------------------
// Remove vkb and image	layout and delete them			  
// ------------------------------------------------------------------------
// 
void CAknFepLayoutPool::Reset()
	{
	ResetImLayout();
	ResetVkbLayout();
	}
	 
//	End	Of File	   
