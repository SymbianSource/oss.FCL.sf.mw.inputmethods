/*
* Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies). 
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
* Description:               AknFep Domain Publish & Subscribe keys.
 *
*/















#ifndef AKNFEP_INTERNAL_PS_KEYS_H
#define AKNFEP_INTERNAL_PS_KEYS_H

const TUid KPSUidAknFep = { 0x100056de };

/**
 * The Fn key State
 *
 * Possible values:
 * 0: NONE
 * 1: Next
 * 2: Pressed
 * 3: Locked
 */
const TUint32 KAknFepFnKeyState = 0x00000001;

/**
 * The Predictive Setting Dialog State
 *
 * Possible values:
 * 0: Closed
 * 1: Launched
 */
const TUint32 KAknFepSettingDialogState = 0x00000002;

/**
 * The virtual keyboard type
 *
 * Possible values:
 * 0: None
 * 1: Conventional 12 phone keypad
 * 2: 4x12 Qwerty keyboard.
 * 3: EPtiKeyboardQwerty4x10
 * 4: EPtiKeyboardQwerty3x11
 * 5: Half qwerty.
 * 6: EPtiKeyboardCustomQwerty
 */   
const TUint32 KAknFepVirtualKeyboardType = 0x00000003;

/**
 * Touch Input status. This key is updated by touch input according to the
 * opening and closing virtual keyboard events. 
 *
 * Possible values:
 * 0: virtual keyboard type is not active.
 * 1: virtual keyboard type is active.
 */  
const TUint32 KAknFepTouchInputActive = 0x00000004;

#endif

// End of file
