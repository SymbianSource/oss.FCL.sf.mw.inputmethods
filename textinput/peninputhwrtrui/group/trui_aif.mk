#
# Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
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
# Description:  icons makefile for project ?myapp
#

ifeq (WINS,$(findstring WINS, $(PLATFORM)))
ZDIR=$(EPOCROOT)Epoc32\release\$(PLATFORM)\$(CFG)\Z
else
ZDIR=$(EPOCROOT)Epoc32\data\z
endif

TARGETDIR = $(ZDIR)\resource\apps
HEADERDIR = $(EPOCROOT)epoc32\include
APPINFOFILENAME = $(TARGETDIR)\trui_aif.mif
ICONTARGETFILENAME = $(TARGETDIR)\trui_icons.mif
HEADERFILENAME = $(HEADERDIR)\trui_icons.mbg
ICONDIR = ..\data

do_nothing :
	@rem do_nothing

MAKMAKE : do_nothing

BLD : do_nothing

CLEAN : do_nothing

LIB : do_nothing

CLEANLIB : do_nothing

RESOURCE :
	mifconv $(ICONTARGETFILENAME) /H$(HEADERFILENAME) \
        /c32,1 qgn_indi_input_case_accented.svg \
        /c32,1 qgn_indi_input_case_capital.svg \
        /c32,1 qgn_indi_input_case_capital_cyrillic.svg \
        /c32,1 qgn_indi_input_case_capital_greek.svg \
	/c32,1 qgn_indi_input_case_numeric.svg \
	/c32,1 qgn_indi_input_case_small.svg \
	/c32,1 qgn_indi_input_case_small_cyrillic.svg \
	/c32,1 qgn_indi_input_case_small_greek.svg \
	/c32,1 qgn_indi_input_case_text_hebrew.svg \
	/c32,1 qgn_indi_input_case_special.svg \
        /c32,1 qgn_indi_input_char_next.svg \
	/c32,1 qgn_indi_input_char_prev.svg \
        /c32,1 qgn_indi_tb_char_grid.svg \
        /c32,1 qgn_indi_tb_edit_model.svg \
        /c32,1 qgn_indi_tb_model_preview.svg \
        /c32,1 qgn_indi_tb_remove.svg \
        /c32,1 qgn_prop_hwrtraining_shortcut_new.svg \
        /c32,1 qgn_prop_hwrtraining_shortcut_tab2.svg \
        /c32,1 qgn_prop_hwrtraining_tab2.svg \
        /c32,1 qgn_indi_mce_tb_text.svg \
	/c32,1 qgn_indi_marked_add.svg \
	/c32,1 $(ICONDIR)\qgn_graf_fep_bg_colour.svg \
	/c32,1 $(ICONDIR)\qgn_graf_fep_popup_shadow_tl.svg \
	/c32,1 $(ICONDIR)\qgn_graf_fep_popup_shadow_t.svg \
	/c32,1 $(ICONDIR)\qgn_graf_fep_popup_shadow_tr.svg \
	/c32,1 $(ICONDIR)\qgn_graf_fep_popup_shadow_b.svg \
	/c32,1 $(ICONDIR)\qgn_graf_fep_popup_shadow_bl.svg \
	/c32,1 $(ICONDIR)\qgn_graf_fep_popup_shadow_br.svg \
	/c32,1 $(ICONDIR)\qgn_graf_fep_popup_shadow_l.svg \
	/c32,1 $(ICONDIR)\qgn_graf_fep_popup_shadow_r.svg \
	/c32,1 qgn_indi_input_case_accented_cyrillic.svg

	mifconv $(APPINFOFILENAME)\
        /c32,1 qgn_menu_hwr_training.svg

FREEZE : do_nothing

SAVESPACE : do_nothing

RELEASABLES :
	@echo $(ICONTARGETFILENAME)

FINAL : do_nothing
