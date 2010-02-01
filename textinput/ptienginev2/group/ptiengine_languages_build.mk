#
# Copyright (c) 2009 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0""
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description: 
#

BASEDIR=\EPOC32\RELEASE\$(PLATFORM)\$(CFG)

do_nothing:
	@rem do nothing

clear_ro:

CLEAN : clear_ro	

RESOURCE :	do_nothing

FREEZE :	do_nothing

BLD : 
	build_languages.bat $(PLATFORM) $(CFG)

MAKMAKE : do_nothing

LIB :	do_nothing

FINAL : do_nothing


SAVESPACE : do_nothing

RELEASABLES :

 	
