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
* Description:  Implementation of the vkb data manager
*
*/



#include "AknFepVkbPinyinAnalyserDb.h"
// constant define 

// Zhuyin DB data
const TUint16 KZhuyinBase = 0x3105;
const TInt16  KZhuyinElementCount = 37;
const TUint16 KZhuyinSpellsLength[] = 
    {
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x2, 0x2, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x2, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x2, 0x3, 0x3, 0x3, 0x3, 0x2, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x2, 0x3, 0x3, 0x3, 0x2, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x2, 0x3, 0x3, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x2, 0x3, 0x3, 0x3, 0x3, 0x2, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x2, 0x3, 0x3, 0x3, 
    0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x3, 0x2, 0x3, 0x3, 0x3, 0x3, 0x1, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x1, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x1, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x2, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x1, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x3, 0x3, 0x3, 0x3, 0x3, 0x1, 0x2, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x3, 
    0x3, 0x3, 0x3, 0x3, 0x1, 0x2, 0x2, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 
    0x3, 0x3, 0x1, 0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 
    0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 
    0x1, 0x1, 0x1, 0x1, 0x1, 0x2, 0x2, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x1, 0x2, 0x2, 
    0x2, 0x2, 0x2, 0x2, 0x2, 0x2, 0x1, 0x2, 0x2, 
    0x2, 0x2
    };
const TUint16 KZhuyinSpellsLib[] =
    {
    0x3105, 0x311a, 
    0x3105, 0x311b, 
    0x3105, 0x311e, 
    0x3105, 0x311f, 
    0x3105, 0x3120, 
    0x3105, 0x3122, 
    0x3105, 0x3123, 
    0x3105, 0x3124, 
    0x3105, 0x3125, 
    0x3105, 0x3127, 
    0x3105, 0x3127, 0x311d, 
    0x3105, 0x3127, 0x3120, 
    0x3105, 0x3127, 0x3122, 
    0x3105, 0x3127, 0x3123, 
    0x3105, 0x3127, 0x3125, 
    0x3105, 0x3128, 
    0x3106, 0x311a, 
    0x3106, 0x311b, 
    0x3106, 0x311e, 
    0x3106, 0x311f, 
    0x3106, 0x3120, 
    0x3106, 0x3121, 
    0x3106, 0x3122, 
    0x3106, 0x3123, 
    0x3106, 0x3124, 
    0x3106, 0x3125, 
    0x3106, 0x3127, 
    0x3106, 0x3127, 0x311d, 
    0x3106, 0x3127, 0x3120, 
    0x3106, 0x3127, 0x3122, 
    0x3106, 0x3127, 0x3123, 
    0x3106, 0x3127, 0x3125, 
    0x3106, 0x3128, 
    0x3107, 0x311a, 
    0x3107, 0x311b, 
    0x3107, 0x311c, 
    0x3107, 0x311e, 
    0x3107, 0x311f, 
    0x3107, 0x3120, 
    0x3107, 0x3121, 
    0x3107, 0x3122, 
    0x3107, 0x3123, 
    0x3107, 0x3124, 
    0x3107, 0x3125, 
    0x3107, 0x3127, 
    0x3107, 0x3127, 0x311d, 
    0x3107, 0x3127, 0x3120, 
    0x3107, 0x3127, 0x3121, 
    0x3107, 0x3127, 0x3122, 
    0x3107, 0x3127, 0x3123, 
    0x3107, 0x3127, 0x3125, 
    0x3107, 0x3128, 
    0x3108, 0x311a, 
    0x3108, 0x311b, 
    0x3108, 0x311f, 
    0x3108, 0x3121, 
    0x3108, 0x3122, 
    0x3108, 0x3123, 
    0x3108, 0x3124, 
    0x3108, 0x3125, 
    0x3108, 0x3128, 
    0x3109, 0x311a, 
    0x3109, 0x311c, 
    0x3109, 0x311e, 
    0x3109, 0x311f, 
    0x3109, 0x3120, 
    0x3109, 0x3121, 
    0x3109, 0x3122, 
    0x3109, 0x3124, 
    0x3109, 0x3125, 
    0x3109, 0x3127, 
    0x3109, 0x3127, 0x311d, 
    0x3109, 0x3127, 0x3120, 
    0x3109, 0x3127, 0x3121, 
    0x3109, 0x3127, 0x3122, 
    0x3109, 0x3127, 0x3125, 
    0x3109, 0x3128, 
    0x3109, 0x3128, 0x311b, 
    0x3109, 0x3128, 0x311f, 
    0x3109, 0x3128, 0x3122, 
    0x3109, 0x3128, 0x3123, 
    0x3109, 0x3128, 0x3125, 
    0x310a, 0x311a, 
    0x310a, 0x311c, 
    0x310a, 0x311e, 
    0x310a, 0x3120, 
    0x310a, 0x3121, 
    0x310a, 0x3122, 
    0x310a, 0x3124, 
    0x310a, 0x3125, 
    0x310a, 0x3127, 
    0x310a, 0x3127, 0x311d, 
    0x310a, 0x3127, 0x3120, 
    0x310a, 0x3127, 0x3122, 
    0x310a, 0x3127, 0x3125, 
    0x310a, 0x3128, 
    0x310a, 0x3128, 0x311b, 
    0x310a, 0x3128, 0x311f, 
    0x310a, 0x3128, 0x3122, 
    0x310a, 0x3128, 0x3123, 
    0x310a, 0x3128, 0x3125, 
    0x310b, 0x311a, 
    0x310b, 0x311c, 
    0x310b, 0x311e, 
    0x310b, 0x311f, 
    0x310b, 0x3120, 
    0x310b, 0x3121, 
    0x310b, 0x3122, 
    0x310b, 0x3123, 
    0x310b, 0x3124, 
    0x310b, 0x3125, 
    0x310b, 0x3127, 
    0x310b, 0x3127, 0x311d, 
    0x310b, 0x3127, 0x3120, 
    0x310b, 0x3127, 0x3121, 
    0x310b, 0x3127, 0x3122, 
    0x310b, 0x3127, 0x3123, 
    0x310b, 0x3127, 0x3124, 
    0x310b, 0x3127, 0x3125, 
    0x310b, 0x3128, 
    0x310b, 0x3128, 0x311b, 
    0x310b, 0x3128, 0x3122, 
    0x310b, 0x3128, 0x3125, 
    0x310b, 0x3129, 
    0x310b, 0x3129, 0x311d, 
    0x310c, 0x311a, 
    0x310c, 0x311c, 
    0x310c, 0x311e, 
    0x310c, 0x311f, 
    0x310c, 0x3120, 
    0x310c, 0x3121, 
    0x310c, 0x3122, 
    0x310c, 0x3124, 
    0x310c, 0x3125, 
    0x310c, 0x3127, 
    0x310c, 0x3127, 0x311a, 
    0x310c, 0x3127, 0x311d, 
    0x310c, 0x3127, 0x3120, 
    0x310c, 0x3127, 0x3121, 
    0x310c, 0x3127, 0x3122, 
    0x310c, 0x3127, 0x3123, 
    0x310c, 0x3127, 0x3124, 
    0x310c, 0x3127, 0x3125, 
    0x310c, 0x3128, 
    0x310c, 0x3128, 0x311b, 
    0x310c, 0x3128, 0x3122, 
    0x310c, 0x3128, 0x3123, 
    0x310c, 0x3128, 0x3125, 
    0x310c, 0x3129, 
    0x310c, 0x3129, 0x311d, 
    0x310c, 0x3129, 0x3122, 
    0x310d, 0x311a, 
    0x310d, 0x311c, 
    0x310d, 0x311e, 
    0x310d, 0x311f, 
    0x310d, 0x3120, 
    0x310d, 0x3121, 
    0x310d, 0x3122, 
    0x310d, 0x3123, 
    0x310d, 0x3124, 
    0x310d, 0x3125, 
    0x310d, 0x3128, 
    0x310d, 0x3128, 0x311a, 
    0x310d, 0x3128, 0x311b, 
    0x310d, 0x3128, 0x311e, 
    0x310d, 0x3128, 0x311f, 
    0x310d, 0x3128, 0x3122, 
    0x310d, 0x3128, 0x3123, 
    0x310d, 0x3128, 0x3124, 
    0x310d, 0x3128, 0x3125, 
    0x310e, 0x311a, 
    0x310e, 0x311c, 
    0x310e, 0x311e, 
    0x310e, 0x3120, 
    0x310e, 0x3121, 
    0x310e, 0x3122, 
    0x310e, 0x3123, 
    0x310e, 0x3124, 
    0x310e, 0x3125, 
    0x310e, 0x3128, 
    0x310e, 0x3128, 0x311a, 
    0x310e, 0x3128, 0x311b, 
    0x310e, 0x3128, 0x311e, 
    0x310e, 0x3128, 0x311f, 
    0x310e, 0x3128, 0x3122, 
    0x310e, 0x3128, 0x3123, 
    0x310e, 0x3128, 0x3124, 
    0x310e, 0x3128, 0x3125, 
    0x310f, 0x311a, 
    0x310f, 0x311c, 
    0x310f, 0x311e, 
    0x310f, 0x311f, 
    0x310f, 0x3120, 
    0x310f, 0x3121, 
    0x310f, 0x3122, 
    0x310f, 0x3123, 
    0x310f, 0x3124, 
    0x310f, 0x3125, 
    0x310f, 0x3128, 
    0x310f, 0x3128, 0x311a, 
    0x310f, 0x3128, 0x311b, 
    0x310f, 0x3128, 0x311e, 
    0x310f, 0x3128, 0x311f, 
    0x310f, 0x3128, 0x3122, 
    0x310f, 0x3128, 0x3123, 
    0x310f, 0x3128, 0x3124, 
    0x310f, 0x3128, 0x3125, 
    0x3110, 0x3127, 
    0x3110, 0x3127, 0x311a, 
    0x3110, 0x3127, 0x311d, 
    0x3110, 0x3127, 0x3120, 
    0x3110, 0x3127, 0x3121, 
    0x3110, 0x3127, 0x3122, 
    0x3110, 0x3127, 0x3123, 
    0x3110, 0x3127, 0x3124, 
    0x3110, 0x3127, 0x3125, 
    0x3110, 0x3129, 
    0x3110, 0x3129, 0x311d, 
    0x3110, 0x3129, 0x3122, 
    0x3110, 0x3129, 0x3123, 
    0x3110, 0x3129, 0x3125, 
    0x3111, 0x3127, 
    0x3111, 0x3127, 0x311a, 
    0x3111, 0x3127, 0x311d, 
    0x3111, 0x3127, 0x3120, 
    0x3111, 0x3127, 0x3121, 
    0x3111, 0x3127, 0x3122, 
    0x3111, 0x3127, 0x3123, 
    0x3111, 0x3127, 0x3124, 
    0x3111, 0x3127, 0x3125, 
    0x3111, 0x3129, 
    0x3111, 0x3129, 0x311d, 
    0x3111, 0x3129, 0x3122, 
    0x3111, 0x3129, 0x3123, 
    0x3111, 0x3129, 0x3125, 
    0x3112, 0x3127, 
    0x3112, 0x3127, 0x311a, 
    0x3112, 0x3127, 0x311d, 
    0x3112, 0x3127, 0x3120, 
    0x3112, 0x3127, 0x3121, 
    0x3112, 0x3127, 0x3122, 
    0x3112, 0x3127, 0x3123, 
    0x3112, 0x3127, 0x3124, 
    0x3112, 0x3127, 0x3125, 
    0x3112, 0x3129, 
    0x3112, 0x3129, 0x311d, 
    0x3112, 0x3129, 0x3122, 
    0x3112, 0x3129, 0x3123, 
    0x3112, 0x3129, 0x3125, 
    0x3113, 
    0x3113, 0x311a, 
    0x3113, 0x311c, 
    0x3113, 0x311e, 
    0x3113, 0x311f, 
    0x3113, 0x3120, 
    0x3113, 0x3121, 
    0x3113, 0x3122, 
    0x3113, 0x3123, 
    0x3113, 0x3124, 
    0x3113, 0x3125, 
    0x3113, 0x3128, 
    0x3113, 0x3128, 0x311a, 
    0x3113, 0x3128, 0x311b, 
    0x3113, 0x3128, 0x311e, 
    0x3113, 0x3128, 0x311f, 
    0x3113, 0x3128, 0x3122, 
    0x3113, 0x3128, 0x3123, 
    0x3113, 0x3128, 0x3124, 
    0x3113, 0x3128, 0x3125, 
    0x3114, 
    0x3114, 0x311a, 
    0x3114, 0x311c, 
    0x3114, 0x311e, 
    0x3114, 0x3120, 
    0x3114, 0x3121, 
    0x3114, 0x3122, 
    0x3114, 0x3123, 
    0x3114, 0x3124, 
    0x3114, 0x3125, 
    0x3114, 0x3128, 
    0x3114, 0x3128, 0x311b, 
    0x3114, 0x3128, 0x311e, 
    0x3114, 0x3128, 0x311f, 
    0x3114, 0x3128, 0x3122, 
    0x3114, 0x3128, 0x3123, 
    0x3114, 0x3128, 0x3124, 
    0x3114, 0x3128, 0x3125, 
    0x3115, 
    0x3115, 0x311a, 
    0x3115, 0x311c, 
    0x3115, 0x311e, 
    0x3115, 0x311f, 
    0x3115, 0x3120, 
    0x3115, 0x3121, 
    0x3115, 0x3122, 
    0x3115, 0x3123, 
    0x3115, 0x3124, 
    0x3115, 0x3125, 
    0x3115, 0x3128, 
    0x3115, 0x3128, 0x311a, 
    0x3115, 0x3128, 0x311b, 
    0x3115, 0x3128, 0x311e, 
    0x3115, 0x3128, 0x311f, 
    0x3115, 0x3128, 0x3122, 
    0x3115, 0x3128, 0x3123, 
    0x3115, 0x3128, 0x3124, 
    0x3116, 
    0x3116, 0x311c, 
    0x3116, 0x3120, 
    0x3116, 0x3121, 
    0x3116, 0x3122, 
    0x3116, 0x3123, 
    0x3116, 0x3124, 
    0x3116, 0x3125, 
    0x3116, 0x3128, 
    0x3116, 0x3128, 0x311b, 
    0x3116, 0x3128, 0x311f, 
    0x3116, 0x3128, 0x3122, 
    0x3116, 0x3128, 0x3123, 
    0x3116, 0x3128, 0x3125, 
    0x3117, 
    0x3117, 0x311a, 
    0x3117, 0x311c, 
    0x3117, 0x311e, 
    0x3117, 0x311f, 
    0x3117, 0x3120, 
    0x3117, 0x3121, 
    0x3117, 0x3122, 
    0x3117, 0x3123, 
    0x3117, 0x3124, 
    0x3117, 0x3125, 
    0x3117, 0x3128, 
    0x3117, 0x3128, 0x311b, 
    0x3117, 0x3128, 0x311f, 
    0x3117, 0x3128, 0x3122, 
    0x3117, 0x3128, 0x3123, 
    0x3117, 0x3128, 0x3125, 
    0x3118, 
    0x3118, 0x311a, 
    0x3118, 0x311c, 
    0x3118, 0x311e, 
    0x3118, 0x3120, 
    0x3118, 0x3121, 
    0x3118, 0x3122, 
    0x3118, 0x3123, 
    0x3118, 0x3124, 
    0x3118, 0x3125, 
    0x3118, 0x3128, 
    0x3118, 0x3128, 0x311b, 
    0x3118, 0x3128, 0x311f, 
    0x3118, 0x3128, 0x3122, 
    0x3118, 0x3128, 0x3123, 
    0x3118, 0x3128, 0x3125, 
    0x3119, 
    0x3119, 0x311a, 
    0x3119, 0x311c, 
    0x3119, 0x311e, 
    0x3119, 0x3120, 
    0x3119, 0x3121, 
    0x3119, 0x3122, 
    0x3119, 0x3123, 
    0x3119, 0x3124, 
    0x3119, 0x3125, 
    0x3119, 0x3128, 
    0x3119, 0x3128, 0x311b, 
    0x3119, 0x3128, 0x311f, 
    0x3119, 0x3128, 0x3122, 
    0x3119, 0x3128, 0x3123, 
    0x3119, 0x3128, 0x3125, 
    0x311a, 
    0x311b, 
    0x311c, 
    0x311e, 
    0x311f, 
    0x3120, 
    0x3121, 
    0x3122, 
    0x3123, 
    0x3124, 
    0x3125, 
    0x3126, 
    0x3127, 
    0x3127, 0x311a, 
    0x3127, 0x311b, 
    0x3127, 0x311d, 
    0x3127, 0x311e, 
    0x3127, 0x3120, 
    0x3127, 0x3121, 
    0x3127, 0x3122, 
    0x3127, 0x3123, 
    0x3127, 0x3124, 
    0x3127, 0x3125, 
    0x3128, 
    0x3128, 0x311a, 
    0x3128, 0x311b, 
    0x3128, 0x311e, 
    0x3128, 0x311f, 
    0x3128, 0x3122, 
    0x3128, 0x3123, 
    0x3128, 0x3124, 
    0x3128, 0x3125, 
    0x3129, 
    0x3129, 0x311d, 
    0x3129, 0x3122, 
    0x3129, 0x3123, 
    0x3129, 0x3125
    };
    
const TUint16 KZhuyinTypeTable[] = 
    {
    // 21 ESpellConsonant
    ESpellConsonant, ESpellConsonant, ESpellConsonant, ESpellConsonant, ESpellConsonant,
    ESpellConsonant, ESpellSpecialConsonnant, ESpellConsonant, ESpellSpecialConsonnant, ESpellConsonant,
    ESpellSpecialConsonnant, ESpellConsonant, ESpellConsonant, ESpellConsonant, ESpellConsonant,
    ESpellConsonant, ESpellConsonant, ESpellConsonant, ESpellSpecialConsonnant, ESpellSpecialConsonnant,
    ESpellSpecialConsonnant,
    //16 ESpellVowel
    ESpellVowel, ESpellVowel, ESpellVowel, ESpellVowel, ESpellVowel,
    ESpellVowel, ESpellVowel, ESpellVowel, ESpellVowel, ESpellVowel,
    ESpellVowel, ESpellVowel, ESpellVowel, ESpellVowel, ESpellVowel,
    ESpellVowel

    };    

const TUint16 KZhuyinNoFirstElem[] =
    {
    0x311D
    };

// Pinyin DB data
const TUint16 KPinyinBase = L'a';
const TUint16  KPinyinElementCount = 26;
const TUint16 KPinyinSpellsLength[] = 
    {
    1,2,2,3,2,2,3,3,4,3,3,3,4,2,4,4,3,3,4,2,
    2,2,3,3,4,3,2,3,4,3,4,4,5,4,3,4,5,3,5,4,
    3,4,5,5,6,4,4,4,2,4,3,2,4,3,3,3,2,3,3,4,
    3,2,3,3,4,2,3,4,4,3,4,3,4,3,2,4,3,3,3,1,
    2,2,3,2,2,3,4,3,3,4,2,3,2,2,3,3,4,3,2,3,
    3,4,4,3,2,3,4,4,5,3,3,3,2,3,3,4,3,2,3,3,
    4,2,3,4,3,2,3,4,4,5,3,3,3,2,3,4,5,4,3,3,
    4,5,3,2,4,3,3,2,3,3,4,3,2,3,3,4,4,3,2,3,
    4,4,5,3,3,3,2,3,3,4,3,2,3,4,2,3,4,5,4,3,
    3,4,3,2,4,3,2,4,3,3,2,3,1,2,3,3,4,3,2,3,
    3,4,2,4,4,3,3,4,3,2,3,2,1,2,3,3,4,3,2,3,
    3,4,2,2,3,4,5,4,3,3,4,3,4,3,2,4,3,2,3,1,
    2,2,3,3,4,3,3,3,4,2,4,4,3,3,4,2,3,2,2,3,
    4,5,4,3,3,4,5,3,2,4,3,3,3,4,3,2,3,4,2,4,
    3,2,4,3,3,3,2,3,3,4,3,2,3,4,3,4,4,5,4,3,
    4,4,5,3,4,3,4,5,5,6,4,4,4,2,4,3,2,4,3,3,
    3,2,3,3,4,3,2,4,2,4,4,3,4,4,3,2,4,3,3,3,
    2,3,3,4,3,3,4,2,2,2,3,4,5,4,3,3,4,5,3,2,
    4,3,3,2,3,4,3,2,2,3,4,2,4,3,2,4,3,3,2,3,
    3,4,3,2,3,3,4,3,4,4,5,4,3,4,4,5,3,5,4,3,
    4,5,5,6,4,4,4,2,4,3,2,4,3,3,3
    };
const TUint16 KPinyinSpellsLib[] =
    {
    0x61, //a
    0x61, 0x69, //ai
    0x61, 0x6e, //an
    0x61, 0x6e, 0x67, //ang
    0x61, 0x6f, //ao
    0x62, 0x61, //ba
    0x62, 0x61, 0x69, //bai
    0x62, 0x61, 0x6e, //ban
    0x62, 0x61, 0x6e, 0x67, //bang
    0x62, 0x61, 0x6f, //bao
    0x62, 0x65, 0x69, //bei
    0x62, 0x65, 0x6e, //ben
    0x62, 0x65, 0x6e, 0x67, //beng
    0x62, 0x69, //bi
    0x62, 0x69, 0x61, 0x6e, //bian
    0x62, 0x69, 0x61, 0x6f, //biao
    0x62, 0x69, 0x65, //bie
    0x62, 0x69, 0x6e, //bin
    0x62, 0x69, 0x6e, 0x67, //bing
    0x62, 0x6f, //bo
    0x62, 0x75, //bu
    0x63, 0x61, //ca
    0x63, 0x61, 0x69, //cai
    0x63, 0x61, 0x6e, //can
    0x63, 0x61, 0x6e, 0x67, //cang
    0x63, 0x61, 0x6f, //cao
    0x63, 0x65, //ce
    0x63, 0x65, 0x6e, //cen
    0x63, 0x65, 0x6e, 0x67, //ceng
    0x63, 0x68, 0x61, //cha
    0x63, 0x68, 0x61, 0x69, //chai
    0x63, 0x68, 0x61, 0x6e, //chan
    0x63, 0x68, 0x61, 0x6e, 0x67, //chang
    0x63, 0x68, 0x61, 0x6f, //chao
    0x63, 0x68, 0x65, //che
    0x63, 0x68, 0x65, 0x6e, //chen
    0x63, 0x68, 0x65, 0x6e, 0x67, //cheng
    0x63, 0x68, 0x69, //chi
    0x63, 0x68, 0x6f, 0x6e, 0x67, //chong
    0x63, 0x68, 0x6f, 0x75, //chou
    0x63, 0x68, 0x75, //chu
    0x63, 0x68, 0x75, 0x61, //chua
    0x63, 0x68, 0x75, 0x61, 0x69, //chuai
    0x63, 0x68, 0x75, 0x61, 0x6e, //chuan
    0x63, 0x68, 0x75, 0x61, 0x6e, 0x67, //chuang
    0x63, 0x68, 0x75, 0x69, //chui
    0x63, 0x68, 0x75, 0x6e, //chun
    0x63, 0x68, 0x75, 0x6f, //chuo
    0x63, 0x69, //ci
    0x63, 0x6f, 0x6e, 0x67, //cong
    0x63, 0x6f, 0x75, //cou
    0x63, 0x75, //cu
    0x63, 0x75, 0x61, 0x6e, //cuan
    0x63, 0x75, 0x69, //cui
    0x63, 0x75, 0x6e, //cun
    0x63, 0x75, 0x6f, //cuo
    0x64, 0x61, //da
    0x64, 0x61, 0x69, //dai
    0x64, 0x61, 0x6e, //dan
    0x64, 0x61, 0x6e, 0x67, //dang
    0x64, 0x61, 0x6f, //dao
    0x64, 0x65, //de
    0x64, 0x65, 0x69, //dei
    0x64, 0x65, 0x6e, //den
    0x64, 0x65, 0x6e, 0x67, //deng
    0x64, 0x69, //di
    0x64, 0x69, 0x61, //dia
    0x64, 0x69, 0x61, 0x6e, //dian
    0x64, 0x69, 0x61, 0x6f, //diao
    0x64, 0x69, 0x65, //die
    0x64, 0x69, 0x6e, 0x67, //ding
    0x64, 0x69, 0x75, //diu
    0x64, 0x6f, 0x6e, 0x67, //dong
    0x64, 0x6f, 0x75, //dou
    0x64, 0x75, //du
    0x64, 0x75, 0x61, 0x6e, //duan
    0x64, 0x75, 0x69, //dui
    0x64, 0x75, 0x6e, //dun
    0x64, 0x75, 0x6f, //duo
    0x65, //e
    0x65, 0x69, //ei
    0x65, 0x6e, //en
    0x65, 0x6e, 0x67, //eng
    0x65, 0x72, //er
    0x66, 0x61, //fa
    0x66, 0x61, 0x6e, //fan
    0x66, 0x61, 0x6e, 0x67, //fang
    0x66, 0x65, 0x69, //fei
    0x66, 0x65, 0x6e, //fen
    0x66, 0x65, 0x6e, 0x67, //feng
    0x66, 0x6f, //fo
    0x66, 0x6f, 0x75, //fou
    0x66, 0x75, //fu
    0x67, 0x61, //ga
    0x67, 0x61, 0x69, //gai
    0x67, 0x61, 0x6e, //gan
    0x67, 0x61, 0x6e, 0x67, //gang
    0x67, 0x61, 0x6f, //gao
    0x67, 0x65, //ge
    0x67, 0x65, 0x69, //gei
    0x67, 0x65, 0x6e, //gen
    0x67, 0x65, 0x6e, 0x67, //geng
    0x67, 0x6f, 0x6e, 0x67, //gong
    0x67, 0x6f, 0x75, //gou
    0x67, 0x75, //gu
    0x67, 0x75, 0x61, //gua
    0x67, 0x75, 0x61, 0x69, //guai
    0x67, 0x75, 0x61, 0x6e, //guan
    0x67, 0x75, 0x61, 0x6e, 0x67, //guang
    0x67, 0x75, 0x69, //gui
    0x67, 0x75, 0x6e, //gun
    0x67, 0x75, 0x6f, //guo
    0x68, 0x61, //ha
    0x68, 0x61, 0x69, //hai
    0x68, 0x61, 0x6e, //han
    0x68, 0x61, 0x6e, 0x67, //hang
    0x68, 0x61, 0x6f, //hao
    0x68, 0x65, //he
    0x68, 0x65, 0x69, //hei
    0x68, 0x65, 0x6e, //hen
    0x68, 0x65, 0x6e, 0x67, //heng
    0x68, 0x6d, //hm
    0x68, 0x6e, 0x67, //hng
    0x68, 0x6f, 0x6e, 0x67, //hong
    0x68, 0x6f, 0x75, //hou
    0x68, 0x75, //hu
    0x68, 0x75, 0x61, //hua
    0x68, 0x75, 0x61, 0x69, //huai
    0x68, 0x75, 0x61, 0x6e, //huan
    0x68, 0x75, 0x61, 0x6e, 0x67, //huang
    0x68, 0x75, 0x69, //hui
    0x68, 0x75, 0x6e, //hun
    0x68, 0x75, 0x6f, //huo
    0x6a, 0x69, //ji
    0x6a, 0x69, 0x61, //jia
    0x6a, 0x69, 0x61, 0x6e, //jian
    0x6a, 0x69, 0x61, 0x6e, 0x67, //jiang
    0x6a, 0x69, 0x61, 0x6f, //jiao
    0x6a, 0x69, 0x65, //jie
    0x6a, 0x69, 0x6e, //jin
    0x6a, 0x69, 0x6e, 0x67, //jing
    0x6a, 0x69, 0x6f, 0x6e, 0x67, //jiong
    0x6a, 0x69, 0x75, //jiu
    0x6a, 0x75, //ju
    0x6a, 0x75, 0x61, 0x6e, //juan
    0x6a, 0x75, 0x65, //jue
    0x6a, 0x75, 0x6e, //jun
    0x6b, 0x61, //ka
    0x6b, 0x61, 0x69, //kai
    0x6b, 0x61, 0x6e, //kan
    0x6b, 0x61, 0x6e, 0x67, //kang
    0x6b, 0x61, 0x6f, //kao
    0x6b, 0x65, //ke
    0x6b, 0x65, 0x69, //kei
    0x6b, 0x65, 0x6e, //ken
    0x6b, 0x65, 0x6e, 0x67, //keng
    0x6b, 0x6f, 0x6e, 0x67, //kong
    0x6b, 0x6f, 0x75, //kou
    0x6b, 0x75, //ku
    0x6b, 0x75, 0x61, //kua
    0x6b, 0x75, 0x61, 0x69, //kuai
    0x6b, 0x75, 0x61, 0x6e, //kuan
    0x6b, 0x75, 0x61, 0x6e, 0x67, //kuang
    0x6b, 0x75, 0x69, //kui
    0x6b, 0x75, 0x6e, //kun
    0x6b, 0x75, 0x6f, //kuo
    0x6c, 0x61, //la
    0x6c, 0x61, 0x69, //lai
    0x6c, 0x61, 0x6e, //lan
    0x6c, 0x61, 0x6e, 0x67, //lang
    0x6c, 0x61, 0x6f, //lao
    0x6c, 0x65, //le
    0x6c, 0x65, 0x69, //lei
    0x6c, 0x65, 0x6e, 0x67, //leng
    0x6c, 0x69, //li
    0x6c, 0x69, 0x61, //lia
    0x6c, 0x69, 0x61, 0x6e, //lian
    0x6c, 0x69, 0x61, 0x6e, 0x67, //liang
    0x6c, 0x69, 0x61, 0x6f, //liao
    0x6c, 0x69, 0x65, //lie
    0x6c, 0x69, 0x6e, //lin
    0x6c, 0x69, 0x6e, 0x67, //ling
    0x6c, 0x69, 0x75, //liu
    0x6c, 0x6f, //lo
    0x6c, 0x6f, 0x6e, 0x67, //long
    0x6c, 0x6f, 0x75, //lou
    0x6c, 0x75, //lu
    0x6c, 0x75, 0x61, 0x6e, //luan
    0x6c, 0x75, 0x6e, //lun
    0x6c, 0x75, 0x6f, //luo
    0x6c, 0x76, //lv
    0x6c, 0x76, 0x65, //lve
    0x6d, //m
    0x6d, 0x61, //ma
    0x6d, 0x61, 0x69, //mai
    0x6d, 0x61, 0x6e, //man
    0x6d, 0x61, 0x6e, 0x67, //mang
    0x6d, 0x61, 0x6f, //mao
    0x6d, 0x65, //me
    0x6d, 0x65, 0x69, //mei
    0x6d, 0x65, 0x6e, //men
    0x6d, 0x65, 0x6e, 0x67, //meng
    0x6d, 0x69, //mi
    0x6d, 0x69, 0x61, 0x6e, //mian
    0x6d, 0x69, 0x61, 0x6f, //miao
    0x6d, 0x69, 0x65, //mie
    0x6d, 0x69, 0x6e, //min
    0x6d, 0x69, 0x6e, 0x67, //ming
    0x6d, 0x69, 0x75, //miu
    0x6d, 0x6f, //mo
    0x6d, 0x6f, 0x75, //mou
    0x6d, 0x75, //mu
    0x6e, //n
    0x6e, 0x61, //na
    0x6e, 0x61, 0x69, //nai
    0x6e, 0x61, 0x6e, //nan
    0x6e, 0x61, 0x6e, 0x67, //nang
    0x6e, 0x61, 0x6f, //nao
    0x6e, 0x65, //ne
    0x6e, 0x65, 0x69, //nei
    0x6e, 0x65, 0x6e, //nen
    0x6e, 0x65, 0x6e, 0x67, //neng
    0x6e, 0x67, //ng
    0x6e, 0x69, //ni
    0x6e, 0x69, 0x61, //nia
    0x6e, 0x69, 0x61, 0x6e, //nian
    0x6e, 0x69, 0x61, 0x6e, 0x67, //niang
    0x6e, 0x69, 0x61, 0x6f, //niao
    0x6e, 0x69, 0x65, //nie
    0x6e, 0x69, 0x6e, //nin
    0x6e, 0x69, 0x6e, 0x67, //ning
    0x6e, 0x69, 0x75, //niu
    0x6e, 0x6f, 0x6e, 0x67, //nong
    0x6e, 0x6f, 0x75, //nou
    0x6e, 0x75, //nu
    0x6e, 0x75, 0x61, 0x6e, //nuan
    0x6e, 0x75, 0x6f, //nuo
    0x6e, 0x76, //nv
    0x6e, 0x76, 0x65, //nve
    0x6f, //o
    0x6f, 0x75, //ou
    0x70, 0x61, //pa
    0x70, 0x61, 0x69, //pai
    0x70, 0x61, 0x6e, //pan
    0x70, 0x61, 0x6e, 0x67, //pang
    0x70, 0x61, 0x6f, //pao
    0x70, 0x65, 0x69, //pei
    0x70, 0x65, 0x6e, //pen
    0x70, 0x65, 0x6e, 0x67, //peng
    0x70, 0x69, //pi
    0x70, 0x69, 0x61, 0x6e, //pian
    0x70, 0x69, 0x61, 0x6f, //piao
    0x70, 0x69, 0x65, //pie
    0x70, 0x69, 0x6e, //pin
    0x70, 0x69, 0x6e, 0x67, //ping
    0x70, 0x6f, //po
    0x70, 0x6f, 0x75, //pou
    0x70, 0x75, //pu
    0x71, 0x69, //qi
    0x71, 0x69, 0x61, //qia
    0x71, 0x69, 0x61, 0x6e, //qian
    0x71, 0x69, 0x61, 0x6e, 0x67, //qiang
    0x71, 0x69, 0x61, 0x6f, //qiao
    0x71, 0x69, 0x65, //qie
    0x71, 0x69, 0x6e, //qin
    0x71, 0x69, 0x6e, 0x67, //qing
    0x71, 0x69, 0x6f, 0x6e, 0x67, //qiong
    0x71, 0x69, 0x75, //qiu
    0x71, 0x75, //qu
    0x71, 0x75, 0x61, 0x6e, //quan
    0x71, 0x75, 0x65, //que
    0x71, 0x75, 0x6e, //qun
    0x72, 0x61, 0x6e, //ran
    0x72, 0x61, 0x6e, 0x67, //rang
    0x72, 0x61, 0x6f, //rao
    0x72, 0x65, //re
    0x72, 0x65, 0x6e, //ren
    0x72, 0x65, 0x6e, 0x67, //reng
    0x72, 0x69, //ri
    0x72, 0x6f, 0x6e, 0x67, //rong
    0x72, 0x6f, 0x75, //rou
    0x72, 0x75, //ru
    0x72, 0x75, 0x61, 0x6e, //ruan
    0x72, 0x75, 0x69, //rui
    0x72, 0x75, 0x6e, //run
    0x72, 0x75, 0x6f, //ruo
    0x73, 0x61, //sa
    0x73, 0x61, 0x69, //sai
    0x73, 0x61, 0x6e, //san
    0x73, 0x61, 0x6e, 0x67, //sang
    0x73, 0x61, 0x6f, //sao
    0x73, 0x65, //se
    0x73, 0x65, 0x6e, //sen
    0x73, 0x65, 0x6e, 0x67, //seng
    0x73, 0x68, 0x61, //sha
    0x73, 0x68, 0x61, 0x69, //shai
    0x73, 0x68, 0x61, 0x6e, //shan
    0x73, 0x68, 0x61, 0x6e, 0x67, //shang
    0x73, 0x68, 0x61, 0x6f, //shao
    0x73, 0x68, 0x65, //she
    0x73, 0x68, 0x65, 0x69, //shei
    0x73, 0x68, 0x65, 0x6e, //shen
    0x73, 0x68, 0x65, 0x6e, 0x67, //sheng
    0x73, 0x68, 0x69, //shi
    0x73, 0x68, 0x6f, 0x75, //shou
    0x73, 0x68, 0x75, //shu
    0x73, 0x68, 0x75, 0x61, //shua
    0x73, 0x68, 0x75, 0x61, 0x69, //shuai
    0x73, 0x68, 0x75, 0x61, 0x6e, //shuan
    0x73, 0x68, 0x75, 0x61, 0x6e, 0x67, //shuang
    0x73, 0x68, 0x75, 0x69, //shui
    0x73, 0x68, 0x75, 0x6e, //shun
    0x73, 0x68, 0x75, 0x6f, //shuo
    0x73, 0x69, //si
    0x73, 0x6f, 0x6e, 0x67, //song
    0x73, 0x6f, 0x75, //sou
    0x73, 0x75, //su
    0x73, 0x75, 0x61, 0x6e, //suan
    0x73, 0x75, 0x69, //sui
    0x73, 0x75, 0x6e, //sun
    0x73, 0x75, 0x6f, //suo
    0x74, 0x61, //ta
    0x74, 0x61, 0x69, //tai
    0x74, 0x61, 0x6e, //tan
    0x74, 0x61, 0x6e, 0x67, //tang
    0x74, 0x61, 0x6f, //tao
    0x74, 0x65, //te
    0x74, 0x65, 0x6e, 0x67, //teng
    0x74, 0x69, //ti
    0x74, 0x69, 0x61, 0x6e, //tian
    0x74, 0x69, 0x61, 0x6f, //tiao
    0x74, 0x69, 0x65, //tie
    0x74, 0x69, 0x6e, 0x67, //ting
    0x74, 0x6f, 0x6e, 0x67, //tong
    0x74, 0x6f, 0x75, //tou
    0x74, 0x75, //tu
    0x74, 0x75, 0x61, 0x6e, //tuan
    0x74, 0x75, 0x69, //tui
    0x74, 0x75, 0x6e, //tun
    0x74, 0x75, 0x6f, //tuo
    0x77, 0x61, //wa
    0x77, 0x61, 0x69, //wai
    0x77, 0x61, 0x6e, //wan
    0x77, 0x61, 0x6e, 0x67, //wang
    0x77, 0x65, 0x69, //wei
    0x77, 0x65, 0x6e, //wen
    0x77, 0x65, 0x6e, 0x67, //weng
    0x77, 0x6f, //wo
    0x77, 0x75, //wu
    0x78, 0x69, //xi
    0x78, 0x69, 0x61, //xia
    0x78, 0x69, 0x61, 0x6e, //xian
    0x78, 0x69, 0x61, 0x6e, 0x67, //xiang
    0x78, 0x69, 0x61, 0x6f, //xiao
    0x78, 0x69, 0x65, //xie
    0x78, 0x69, 0x6e, //xin
    0x78, 0x69, 0x6e, 0x67, //xing
    0x78, 0x69, 0x6f, 0x6e, 0x67, //xiong
    0x78, 0x69, 0x75, //xiu
    0x78, 0x75, //xu
    0x78, 0x75, 0x61, 0x6e, //xuan
    0x78, 0x75, 0x65, //xue
    0x78, 0x75, 0x6e, //xun
    0x79, 0x61, //ya
    0x79, 0x61, 0x6e, //yan
    0x79, 0x61, 0x6e, 0x67, //yang
    0x79, 0x61, 0x6f, //yao
    0x79, 0x65, //ye
    0x79, 0x69, //yi
    0x79, 0x69, 0x6e, //yin
    0x79, 0x69, 0x6e, 0x67, //ying
    0x79, 0x6f, //yo
    0x79, 0x6f, 0x6e, 0x67, //yong
    0x79, 0x6f, 0x75, //you
    0x79, 0x75, //yu
    0x79, 0x75, 0x61, 0x6e, //yuan
    0x79, 0x75, 0x65, //yue
    0x79, 0x75, 0x6e, //yun
    0x7a, 0x61, //za
    0x7a, 0x61, 0x69, //zai
    0x7a, 0x61, 0x6e, //zan
    0x7a, 0x61, 0x6e, 0x67, //zang
    0x7a, 0x61, 0x6f, //zao
    0x7a, 0x65, //ze
    0x7a, 0x65, 0x69, //zei
    0x7a, 0x65, 0x6e, //zen
    0x7a, 0x65, 0x6e, 0x67, //zeng
    0x7a, 0x68, 0x61, //zha
    0x7a, 0x68, 0x61, 0x69, //zhai
    0x7a, 0x68, 0x61, 0x6e, //zhan
    0x7a, 0x68, 0x61, 0x6e, 0x67, //zhang
    0x7a, 0x68, 0x61, 0x6f, //zhao
    0x7a, 0x68, 0x65, //zhe
    0x7a, 0x68, 0x65, 0x69, //zhei
    0x7a, 0x68, 0x65, 0x6e, //zhen
    0x7a, 0x68, 0x65, 0x6e, 0x67, //zheng
    0x7a, 0x68, 0x69, //zhi
    0x7a, 0x68, 0x6f, 0x6e, 0x67, //zhong
    0x7a, 0x68, 0x6f, 0x75, //zhou
    0x7a, 0x68, 0x75, //zhu
    0x7a, 0x68, 0x75, 0x61, //zhua
    0x7a, 0x68, 0x75, 0x61, 0x69, //zhuai
    0x7a, 0x68, 0x75, 0x61, 0x6e, //zhuan
    0x7a, 0x68, 0x75, 0x61, 0x6e, 0x67, //zhuang
    0x7a, 0x68, 0x75, 0x69, //zhui
    0x7a, 0x68, 0x75, 0x6e, //zhun
    0x7a, 0x68, 0x75, 0x6f, //zhuo
    0x7a, 0x69, //zi
    0x7a, 0x6f, 0x6e, 0x67, //zong
    0x7a, 0x6f, 0x75, //zou
    0x7a, 0x75, //zu
    0x7a, 0x75, 0x61, 0x6e, //zuan
    0x7a, 0x75, 0x69, //zui
    0x7a, 0x75, 0x6e, //zun
    0x7a, 0x75, 0x6f //zuo
    };
    
const TUint16 KPinyinTypeTable[] = 
{
    //a, b, c, d, e, f
    ESpellVowel, ESpellConsonant, ESpellSpecialConsonnant, 
    ESpellConsonant, ESpellVowel, ESpellConsonant,
    //g, h, i, j, k, l
    ESpellSpecialConsonnant, ESpellSpecialConsonnant, ESpellVowel, 
    ESpellConsonant, ESpellConsonant, ESpellConsonant,
    //m, n, o, p, q, r
    ESpellConsonant, ESpellSpecialConsonnant, ESpellVowel, 
    ESpellConsonant, ESpellConsonant, ESpellConsonant,
    //s, t, u, v, w, x
    ESpellSpecialConsonnant, ESpellConsonant, ESpellVowel, 
    ESpellVowel, ESpellConsonant, ESpellConsonant,
    //y, z
    ESpellConsonant, ESpellSpecialConsonnant
};

const TUint16 KPinyinNoFirstElem[] =
    {
    0x69, 0x76, 0x75
    };
    
// -----------------------------------------------------------------------------
// CPinyinAnalyserDbFeed::NewL
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPinyinAnalyserDbFeed* CPinyinAnalyserDbFeed::NewL( TPinyinAnalyserDbType aType )
    {
    CPinyinAnalyserDbFeed* self = new ( ELeave ) CPinyinAnalyserDbFeed();
    CleanupStack::PushL(self);
    self->ConstructL( aType ); 
    CleanupStack::Pop();
    return self;    
    }
    
CPinyinAnalyserDbFeed::~CPinyinAnalyserDbFeed()
    {
    
    }
// -----------------------------------------------------------------------------
// CPinyinAnalyserDbFeed::~CPinyinAnalyserDbFeed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//
CPinyinAnalyserDbFeed::CPinyinAnalyserDbFeed()
    {
    }

// -----------------------------------------------------------------------------
// CPinyinAnalyserDbFeed::~CPinyinAnalyserDbFeed
// (other items were commented in a header).
// -----------------------------------------------------------------------------
//    
void CPinyinAnalyserDbFeed::ConstructL( TPinyinAnalyserDbType aType )
    {
    
    if ( aType == EPinyinAnalyserTypePinyin )
        {
        iSpellBuf.Set( KPinyinSpellsLib, 
                       sizeof( KPinyinSpellsLib ) / sizeof( KPinyinSpellsLib[0] ) );
        iSpellLenBuf.Set( KPinyinSpellsLength, 
                       sizeof( KPinyinSpellsLength ) / sizeof ( KPinyinSpellsLength[0] ) );
        iSpellElemBase = KPinyinBase;
        iSpellElemLen  = KPinyinElementCount;
        iSpellTypeBuf.Set( KPinyinTypeTable, 
                       sizeof( KPinyinTypeTable ) / sizeof ( KPinyinTypeTable[0] ) );
        iNoFirstElements.Set( KPinyinNoFirstElem, 
                       sizeof( KPinyinNoFirstElem) / sizeof( KPinyinNoFirstElem[0] ) );
        }
    else if ( aType == EPinyinAnalyserTypeZhuyin )
        {
        iSpellBuf.Set( KZhuyinSpellsLib, 
                       sizeof( KZhuyinSpellsLib ) / sizeof( KZhuyinSpellsLib[0] ) );
        iSpellLenBuf.Set( KZhuyinSpellsLength, 
                       sizeof( KZhuyinSpellsLength ) / sizeof ( KZhuyinSpellsLength[0] ) );
        iSpellElemBase = KZhuyinBase;
        iSpellElemLen  = KZhuyinElementCount;
        iSpellTypeBuf.Set( KZhuyinTypeTable, 
                       sizeof( KZhuyinTypeTable ) / sizeof ( KZhuyinTypeTable[0] ) );
        iNoFirstElements.Set( KZhuyinNoFirstElem, 
                       sizeof( KZhuyinNoFirstElem) / sizeof( KZhuyinNoFirstElem[0] ) );
        }
    else
        {
        ASSERT( 0 );    
        }
    
    }
    