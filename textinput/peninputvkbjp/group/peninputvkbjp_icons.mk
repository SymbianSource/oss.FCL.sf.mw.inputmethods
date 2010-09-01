#
# Copyright (c) 2002-2007 Nokia Corporation and/or its subsidiary(-ies).
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
# Description:  icons makefile for peninput vkb japanese
#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=\epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=\epoc32\data\z
endif

# ----------------------------------------------------------------------------
# Configuration for AknFep icons.
# ----------------------------------------------------------------------------

TARGETDIR=$(ZDIR)\resource\apps
HEADERDIR=\epoc32\include
ICONTARGETFILENAME=$(TARGETDIR)\peninputvkbjp.mif
HEADERFILENAME=$(HEADERDIR)\peninputvkbjp.mbg

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN :
	if exist $(ICONTARGETFILENAME)   del $(ICONTARGETFILENAME)
	if exist $(HEADERFILENAME)       del $(HEADERFILENAME)
	if exist $(TARGETDIR)\peninputhwrboxjp.mbm   del $(TARGETDIR)\peninputvkbjp.mbm

LIB : do_nothing

CLEANLIB : do_nothing

# ----------------------------------------------------------------------------
# NOTE 1: DO NOT DEFINE MASK FILE NAMES! They are included automatically by
# MifConv if the mask depth is defined.
#
# NOTE 2: Usually, source paths should not be included in the bitmap
# definitions. MifConv searches for the icons in all icon directories in a
# predefined order, which is currently \s60\icons, \s60\bitmaps2.
# The directory \s60\icons is included in the search only if the feature flag
# __SCALABLE_ICONS is defined.
# ----------------------------------------------------------------------------

RESOURCE:
	mifconv $(ICONTARGETFILENAME) /h$(HEADERFILENAME) /Ficonlist.txt

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(HEADERFILENAME)&& \
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
