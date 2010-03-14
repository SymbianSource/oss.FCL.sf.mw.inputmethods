/*
* Copyright (c) 2005,2006 Choe Hwanjin
* All rights reserved.
* This component and the accompanying materials are made available
* under the terms of the License "Eclipse Public License v1.0"
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
#ifndef _STDBOOL_H
#define	_STDBOOL_H

#ifdef __SYMBIAN32__

#undef  bool
#undef  true
#undef  false

#define bool    int
#define true    1
#define false   0

#else
#include <sys/feature_tests.h>

#ifndef __cplusplus
#if defined(_STDC_C99) || defined(__C99FEATURES__) || __GNUC__ >= 3

#undef	bool
#undef	true
#undef	false

#define	bool	_Bool
#define	true	1
#define	false	0

#define	__bool_true_false_are_defined	1

#endif /* defined(_STDC_C99) || defined(__C99FEATURES__) || __GNUC__ >= 3 */
#endif /* __cplusplus */

#endif /* __SYMBIAN32__ */

#endif /* _STDBOOL_H */
