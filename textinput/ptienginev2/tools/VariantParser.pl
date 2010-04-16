#
# Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).
# All rights reserved.
# This component and the accompanying materials are made available
# under the terms of "Eclipse Public License v1.0"
# which accompanies this distribution, and is available
# at the URL "http://www.eclipse.org/legal/epl-v10.html".
#
# Initial Contributors:
# Nokia Corporation - initial contribution.
#
# Contributors:
#
# Description:   Sets up bld.inf file for language dlls.
#

($time_second, $time_minute, $time_hour,          # time/date
    $date_day, $date_month, $date_year,
    $day_week, $day_year, $isdst) = localtime(time);

if ($date_year < 70) {$date_year += 2000;}        # add century
    else {$date_year += 1900;}

$keymapdata_interface_uid = "10281896";
$keymapsdirstart = "..\/keymaps\/";
$keymapsdir = $keymapsdirstart;
$langdir = "..\/languages\/";
$output_dll = "PtiKeymappings_";
$iby_file_prefix_path = "..\/rom\/";
$iby_file_prefix = "PtiKeymappings_variant_"; 
$VariantFolder = "";

@lang_name_array = ();

$use_block_12_key = 0;
$use_block_qwerty_standard = 0;
$use_block_qwerty_mini = 0;
$use_block_qwerty_3x11 = 0;
$use_block_qwerty_half = 0;
$use_block_custom = 0;

#############################################
# Create empty body for languages 
# bld.inf file
#############################################

# Make sure the language directory is in there.
mkdir "$langdir";

unlink("${langdir}bld.inf");
if ( ! open(INFFILE,">${langdir}bld.inf") )
    { 
	  die "Could not create bld.inf file for language databases!"; 
    } 

print(INFFILE "/*\n");
print(INFFILE "* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).\n");
print(INFFILE "* All rights reserved.\n");
print(INFFILE "* This component and the accompanying materials are made available\n");
print(INFFILE "* under the terms of the License \"Eclipse Public License v1.0\"\n");
print(INFFILE "* which accompanies this distribution, and is available\n");
print(INFFILE "* at the URL "http://www.eclipse.org/legal/epl-v10.html".\n");
print(INFFILE "*\n");
print(INFFILE "* Initial Contributors:\n");
print(INFFILE "* Nokia Corporation - initial contribution.\n");
print(INFFILE "*\n");
print(INFFILE "* Contributors:\n");
print(INFFILE "*\n");
print(INFFILE "* Description: Build info file for language keymapping dlls.\n");
print(INFFILE "*\n");
print(INFFILE "*/\n");
print(INFFILE "\n\n\n\n\n\n\n\n\n\n\n\n");


# Open configuration source file
if ( ! open(MASTERVARFILE,"MasterVariants.txt") )
    { 
	  die "ERROR: Could not find MastersVariants.txt!"; 
    } 
	
@master_raw_data=<MASTERVARFILE>;
foreach $line (@master_raw_data)
{
	if (index($line, "#") >= 0)
	{
		# Comment line, just skip
	}
	elsif (index($line, "USE Folder") >= 0)
	{
		$use_block_12_key = 0;
		$use_block_qwerty_standard = 0;
		$use_block_qwerty_mini = 0;
		$use_block_qwerty_3x11 = 0;
		$use_block_qwerty_half = 0;
		$use_block_custom = 0;
		chop($line);   # Remove '\n'
		@words = split(/ +/,$line);    # Split into words.

		$VariantFolder = $keymapsdirstart . $words[2];
		# Open configuration source file
		if ( ! open(VARFILE,"${VariantFolder}\/Variants.txt") )
		{
			print("\nWARNING: Could not find ${VariantFolder}\\Variants.txt! \n");
			goto endblock;
		}

		@raw_data=<VARFILE>;
		foreach $line (@raw_data)
		{
			if (index($line, "#") >= 0)
			{
				# Comment line, just skip
			}
			elsif (index($line, "[PACKAGE") >= 0)
			{
				# Finalize and close files from previous round.
				print(CHHFILE "#endif\n");
				print(CHHFILE "\n// End of file. \n");
				close(CHHFILE);

				#
				# Finalize factory class from previous round.
				#
				print(CFFFILE "\n");
				print(CFFFILE "    return NULL;\n");
				print(CFFFILE "    }\n");
				print(CFFFILE "\n");
				print(CFFFILE "\n");
				print(CFFFILE "void CPtiVariant${variant_name}DataFactory::ListLanguagesL(RArray<TInt>& aResult)\n");
				print(CFFFILE "    {\n");

				for ($ii = 0; $ii < scalar(@lang_name_array); $ii++)
				{
					if (@lang_name_array[$ii] eq "Basque" || @lang_name_array[$ii] eq "Galician")
					{
						print(CFFFILE "    aResult.AppendL(KLang@lang_name_array[$ii]);\n");
					}
					else
					{
						print(CFFFILE "    aResult.AppendL(ELang@lang_name_array[$ii]);\n");
					}
				}

				print(CFFFILE "    }\n");
				print(CFFFILE "\n");
				print(CFFFILE "// End of file\n");
				close(CFFFILE);

				# Start a new variant
				$impl_uid = "";
				$dll_uid = "";
				$variant_name = "";

				$wins_only = 0;
				$no_wins = 0;

				while (scalar(@lang_name_array) > 0)
				{
					# Clear array.
					pop(@lang_name_array);
				}

				$line =~ s/<.*?>//sg;
				@words = grep(/./,("\n",split(/[^[:alnum:]]+/s,$line)));

				$variant_name = $words[1] . "_" . $words[2];
				$impl_uid = $words[3];
				$dll_uid = $words[4];
				$target_name = $output_dll . $variant_name;

				# Do we want to build only for wins, not for wins at all or for both wins and armv5
				if ($words[5] eq "NOWINS")
				{
					$no_wins = 1;
				}
				elsif ($words[5] eq "WINSONLY")
				{
					$wins_only = 1;
				}

				print("**** Generating variant $variant_name ****\n");
				print("    implementation uid: $impl_uid \n");
				print("    dll uid: $dll_uid \n");

				##############################################################
				#
				# Create ecom resource file.
				#
				##############################################################
				unlink("${langdir}${dll_uid}.rss");
				if ( ! open(RESFILE,">${langdir}${dll_uid}.rss") )
				{
					die "Could not create res file for $language";
				}

				print(RESFILE "/*\n");
				print(RESFILE "* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).\n");
				print(RESFILE "* All rights reserved.\n");
				print(RESFILE "* This component and the accompanying materials are made available\n");
				print(RESFILE "* under the terms of the License \"Eclipse Public License v1.0\"\n");
				print(RESFILE "* which accompanies this distribution, and is available\n");
				print(RESFILE "* at the URL "http://www.eclipse.org/legal/epl-v10.html".\n");
				print(RESFILE "*\n");
				print(RESFILE "* Initial Contributors:\n");
				print(RESFILE "* Nokia Corporation - initial contribution.\n");
				print(RESFILE "*\n");
				print(RESFILE "* Contributors:\n");
				print(RESFILE "*\n");
				print(RESFILE "* Description: PtiEngine language keymapping ecom resource file.\n");
				print(RESFILE "*\n");
				print(RESFILE "*/\n");
				print(RESFILE "\n\n\n\n\n\n\n\n\n\n\n\n");

				print(RESFILE "#include <eikon.rh>\n");
				print(RESFILE "#include <ecom/registryinfo.rh>\n");
				print(RESFILE "\n");
				print(RESFILE "\n");
				print(RESFILE "RESOURCE REGISTRY_INFO theInfo\n");
				print(RESFILE "	{\n");
				print(RESFILE "	dll_uid = 0x${dll_uid};\n");
				print(RESFILE " 	interfaces = \n");
				print(RESFILE "		{\n");
				print(RESFILE "		INTERFACE_INFO\n");
				print(RESFILE "			{\n");
				print(RESFILE "			interface_uid = 0x${keymapdata_interface_uid};\n");
				print(RESFILE "			implementations =\n");
				print(RESFILE "				{\n");
				print(RESFILE "				IMPLEMENTATION_INFO \n");
				print(RESFILE "					{\n");
				print(RESFILE "					implementation_uid = 0x${impl_uid};  // Variant $variant_name\n");
				print(RESFILE "					version_no = 1;\n");
				print(RESFILE "					display_name = \"PtiEngine keymappings\";\n");
				print(RESFILE "					}\n");
				print(RESFILE "				};\n");
				print(RESFILE "			}\n");
				print(RESFILE "		};\n");
				print(RESFILE "	}\n");
				print(RESFILE "\n");
				print(RESFILE "// End of file\n");
				print(RESFILE "\n");

				close(RESFILE);


				#############################################################
				#
				# Create MPP file.
				#
				##############################################################
				unlink("${target_name}.mmp");
				if ( ! open(MMPFILE,">${langdir}${target_name}.mmp") )
				{
					die "Could not create mmp file for variant $variant_name \n";
				}

				print(MMPFILE "/*\n");
				print(MMPFILE "* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).\n");
				print(MMPFILE "* All rights reserved.\n");
				print(MMPFILE "* This component and the accompanying materials are made available\n");
				print(MMPFILE "* under the terms of the License \"Eclipse Public License v1.0\"\n");
				print(MMPFILE "* which accompanies this distribution, and is available\n");
				print(MMPFILE "* at the URL "http://www.eclipse.org/legal/epl-v10.html".\n");
				print(MMPFILE "*\n");
				print(MMPFILE "* Initial Contributors:\n");
				print(MMPFILE "* Nokia Corporation - initial contribution.\n");
				print(MMPFILE "*\n");
				print(MMPFILE "* Contributors:\n");
				print(MMPFILE "*\n");
				print(MMPFILE "* Description: Project file for $langname keymapping data dll.\n");
				print(MMPFILE "*\n");
				print(MMPFILE "*/\n");
				print(MMPFILE "\n\n\n\n\n\n\n\n\n\n\n\n");

				print(MMPFILE "#include <platform_paths.hrh>\n");
				print(MMPFILE "\n");
				print(MMPFILE "TARGET       ${target_name}.dll\n");
				print(MMPFILE "CAPABILITY   CAP_ECOM_PLUGIN\n");
				print(MMPFILE "targettype   PLUGIN\n");
				print(MMPFILE "UID	     0x10009D8D 0x$dll_uid \n");
				print(MMPFILE "\n");
				print(MMPFILE "sourcepath . \n");
				print(MMPFILE "\n");
				print(MMPFILE "userinclude . \n");
				print(MMPFILE "\n");
				print(MMPFILE "MW_LAYER_SYSTEMINCLUDE\n");
				print(MMPFILE "\n");
				#print(MMPFILE "systeminclude   ..\/INC\n");
				print(MMPFILE "\n");
				print(MMPFILE "source ${output_dll}${variant_name}.cpp\n");
				print(MMPFILE "source PtiFactory_${variant_name}.cpp\n");
				print(MMPFILE "\n");
				print(MMPFILE "START RESOURCE ${dll_uid}.rss\n");
				print(MMPFILE "TARGET ${target_name}.rsc\n");
				print(MMPFILE "END\n");
				print(MMPFILE "\n");
				print(MMPFILE "library	EUSER.LIB BAFL.LIB EFSRV.LIB\n");
				print(MMPFILE "library PtiEngine.lib\n");
				print(MMPFILE "\n");
				print(MMPFILE "START WINS\n");
				print(MMPFILE "	baseaddress	0x7dd10000\n");
				print(MMPFILE "END\n");
				print(MMPFILE "\n");

				close(MMPFILE);

				#
				# Add this mmp to bld.inf file
				#
				print(INFFILE "\nPRJ_MMPFILES\n");
				if ($no_wins == 1)
				{
					print(INFFILE "#if !defined(WINS)\n");
				}
				elsif ($wins_only == 1)
				{
					print(INFFILE "#if defined(WINS)\n");
				}

				print(INFFILE "${target_name}.mmp\n");

				if ($no_wins == 1 || $wins_only == 1)
				{
					print(INFFILE "#endif\n");
				}

				#############################################
				# Create and open empty body for variantion iby-file.
				# It will be filled later.
				#############################################
				$iby_file_name = $iby_file_prefix_path.$iby_file_prefix.${variant_name}."\.iby";
				unlink("$iby_file_name");
				if ( ! open(IBYFILE,">$iby_file_name") )
				{
					die "ERROR: Could not create variation iby-file\n";
				}

				print(IBYFILE "/*\n");
				print(IBYFILE "* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).\n");
				print(IBYFILE "* All rights reserved.\n");
				print(IBYFILE "* This component and the accompanying materials are made available\n");
				print(IBYFILE "* under the terms of the License \"Eclipse Public License v1.0\"\n");
				print(IBYFILE "* which accompanies this distribution, and is available\n");
				print(IBYFILE "* at the URL "http://www.eclipse.org/legal/epl-v10.html".\n");
				print(IBYFILE "*\n");
				print(IBYFILE "* Initial Contributors:\n");
				print(IBYFILE "* Nokia Corporation - initial contribution.\n");
				print(IBYFILE "*\n");
				print(IBYFILE "* Contributors:\n");
				print(IBYFILE "*\n");
				print(IBYFILE "* Description: This iby-file was generated by VariantParser.pl\n");
				print(IBYFILE "*\n");
				print(IBYFILE "*/\n");
				print(IBYFILE "\n\n\n\n\n\n\n\n\n\n\n\n");
				print(IBYFILE "#ifndef __PTI_VARIATION_${variant_name}_IBY__\n");
				print(IBYFILE "#define __PTI_VARIATION_${variant_name}_IBY__\n");
				print(IBYFILE "\n");

				#
				# Update iby-file
				#
				if ($wins_only == 0)
				{
					print(IBYFILE "\n");
					print(IBYFILE "ECOM_PLUGIN(${target_name}.dll, ${impl_uid}.rsc)\n");
					print(IBYFILE "\n");
				}

				print(IBYFILE "\n");
				print(IBYFILE "#endif //__PTI_VARIATION_${variant_name}_IBY__\n");
				print(IBYFILE "\n");
				close(IBYFILE);
				
				if ($wins_only != 1)
				{
					print(INFFILE "\nPRJ_EXPORTS\n");
					$exportpath = "\/epoc32\/rom\/include\/".$iby_file_prefix.${variant_name}."\.iby";
					print(INFFILE "$iby_file_name    $exportpath\n");
				}

				#############################################################
				#
				# Create body for main CPP file.
				#
				#############################################################
				unlink("${langdir}${output_dll}${variant_name}.cpp");
				if ( ! open(CPPFILE,">${langdir}${output_dll}${variant_name}.cpp") )
				{
					die "Could not create main cpp file for variant ${variant_name}\n";
				}

				print(CPPFILE "/*\n");
				print(CPPFILE "* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).\n");
				print(CPPFILE "* All rights reserved.\n");
				print(CPPFILE "* This component and the accompanying materials are made available\n");
				print(CPPFILE "* under the terms of the License \"Eclipse Public License v1.0\"\n");
				print(CPPFILE "* which accompanies this distribution, and is available\n");
				print(CPPFILE "* at the URL "http://www.eclipse.org/legal/epl-v10.html".\n");
				print(CPPFILE "*\n");
				print(CPPFILE "* Initial Contributors:\n");
				print(CPPFILE "* Nokia Corporation - initial contribution.\n");
				print(CPPFILE "*\n");
				print(CPPFILE "* Contributors:\n");
				print(CPPFILE "*\n");
				print(CPPFILE "* Description: Keymapping data implementation for ${langconstant}\n");
				print(CPPFILE "*\n");
				print(CPPFILE "*/\n");
				print(CPPFILE "\n\n\n\n\n\n\n\n\n\n\n\n");

				print(CPPFILE "#include <PtiKeyMapData.h>\n");
				print(CPPFILE "\n#include <languages.hrh>\n");
				print(CPPFILE "\n");
				print(CPPFILE "#include \"${output_dll}${variant_name}.h\"\n");
				print(CPPFILE "\n");
				close(CPPFILE);

				#############################################################
				#
				# Create body for main header file.
				#
				#############################################################
				unlink("${langdir}${output_dll}${variant_name}.h");
				if ( ! open(CHHFILE,">${langdir}${output_dll}${variant_name}.h") )
				{
					die "Could not create main header file for variant ${variant_name}\n";
				}

				print(CHHFILE "/*\n");
				print(CHHFILE "* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).\n");
				print(CHHFILE "* All rights reserved.\n");
				print(CHHFILE "* This component and the accompanying materials are made available\n");
				print(CHHFILE "* under the terms of the License \"Eclipse Public License v1.0\"\n");
				print(CHHFILE "* which accompanies this distribution, and is available\n");
				print(CHHFILE "* at the URL "http://www.eclipse.org/legal/epl-v10.html".\n");
				print(CHHFILE "*\n");
				print(CHHFILE "* Initial Contributors:\n");
				print(CHHFILE "* Nokia Corporation - initial contribution.\n");
				print(CHHFILE "*\n");
				print(CHHFILE "* Contributors:\n");
				print(CHHFILE "*\n");
				print(CHHFILE "* Description: Keymapping data implementation for ${langconstant}\n");
				print(CHHFILE "*\n");
				print(CHHFILE "*/\n");
				print(CHHFILE "\n\n\n\n\n\n\n\n\n\n\n\n");
          
				print(CHHFILE "#ifndef __PTI_LANGUAGES_${variant_name}_H__\n");
				print(CHHFILE "#define __PTI_LANGUAGES_${variant_name}_H__\n");
				print(CHHFILE "\n");
				print(CHHFILE "#include <PtiKeyMapData.h>\n");
				print(CHHFILE "\n");
				close(CHHFILE);


				#############################################################
				#
				# Create body for factory cpp file.
				#
				#############################################################
				unlink("${langdir}PtiFactory_${variant_name}.cpp");
				if ( ! open(CFFFILE,">${langdir}PtiFactory_${variant_name}.cpp") )
				{
					die "Could not create main cpp file for variant ${variant_name}\n";
				}

				print(CFFFILE "/*\n");
				print(CFFFILE "* Copyright (c) 2007 Nokia Corporation and/or its subsidiary(-ies).\n");
				print(CFFFILE "* All rights reserved.\n");
				print(CFFFILE "* This component and the accompanying materials are made available\n");
				print(CFFFILE "* under the terms of the License \"Eclipse Public License v1.0\"\n");
				print(CFFFILE "* which accompanies this distribution, and is available\n");
				print(CFFFILE "* at the URL "http://www.eclipse.org/legal/epl-v10.html".\n");
				print(CFFFILE "*\n");
				print(CFFFILE "* Initial Contributors:\n");
				print(CFFFILE "* Nokia Corporation - initial contribution.\n");
				print(CFFFILE "*\n");
				print(CFFFILE "* Contributors:\n");
				print(CFFFILE "*\n");
				print(CFFFILE "* Description: Keymapping data implementation for ${langconstant}\n");
				print(CFFFILE "*\n");
				print(CFFFILE "*/\n");
				print(CFFFILE "\n\n\n\n\n\n\n\n\n\n\n\n");
				print(CFFFILE "#include <ECom/ImplementationProxy.h>\n");
				print(CFFFILE "\n#include <Languages.hrh>\n");
				print(CFFFILE "\n");
				print(CFFFILE "#include \"${output_dll}${variant_name}.h\"\n");
				print(CFFFILE "\n");


				print(CFFFILE "class CPtiVariant${variant_name}DataFactory : public CPtiKeyMapDataFactory\n");
				print(CFFFILE "    {\n");
				print(CFFFILE "    public:\n");
				print(CFFFILE "        CPtiVariant${variant_name}DataFactory() {}\n");
				print(CFFFILE "        static CPtiVariant${variant_name}DataFactory* NewL();\n");
				print(CFFFILE "        MPtiKeyMapData* KeyMapDataForLanguageL(TInt aLanguageCode);\n");
				print(CFFFILE "        void ListLanguagesL(RArray<TInt>& aResult);\n");
				print(CFFFILE "    };\n");
				print(CFFFILE "\n");
				print(CFFFILE "\n");
				print(CFFFILE "CPtiVariant${variant_name}DataFactory* CPtiVariant${variant_name}DataFactory::NewL()\n");
				print(CFFFILE "    {\n");
				print(CFFFILE "    return new (ELeave) CPtiVariant${variant_name}DataFactory();\n");
				print(CFFFILE "    }\n");
				print(CFFFILE "\n");
				print(CFFFILE "\n");
				print(CFFFILE "const TImplementationProxy ImplementationTable[] =\n");
				print(CFFFILE "    {\n");
				print(CFFFILE "    IMPLEMENTATION_PROXY_ENTRY(0x$impl_uid, CPtiVariant${variant_name}DataFactory::NewL)\n");
				print(CFFFILE "    };\n");
				print(CFFFILE "\n");
				print(CFFFILE "\n");
				print(CFFFILE "EXPORT_C const TImplementationProxy* ImplementationGroupProxy(TInt& aTableCount)\n");
				print(CFFFILE "    {\n");
				print(CFFFILE "    aTableCount = sizeof(ImplementationTable) / sizeof(TImplementationProxy);\n");
				print(CFFFILE "    return ImplementationTable;\n");
				print(CFFFILE "    }\n");
				print(CFFFILE "\n");
				print(CFFFILE "\n");
				print(CFFFILE "MPtiKeyMapData* CPtiVariant${variant_name}DataFactory::KeyMapDataForLanguageL(TInt aLanguageCode)\n");
				print(CFFFILE "    {\n");

			}
			elsif (index($line, ".txt") > 0)
			{
				$line =~ s/<.*?>//sg;
				@words = grep(/./,("\n",split(/[^_[:alnum:]]+/s,$line)));

				$lang_name = $words[0];
				$map_file_name = $words[1] . ".txt";

				print("    Adding $map_file_name for $lang_name \n");

				#check that the given file exists
				if ( ! open(LANGFILE,"${keymapsdir}${map_file_name}") )
				{
					die "ERROR: Unable to locate $map_file_name \n";
				}
				close(LANGFILE);

				push(@lang_name_array, $lang_name);

				#
				# Update factory file
				#
				if ($lang_name eq "Basque" || $lang_name eq "Galician")
				{
					print(CFFFILE "    if (aLanguageCode == KLang${lang_name})\n");
				}
				else
				{
					print(CFFFILE "    if (aLanguageCode == ELang${lang_name})\n");
				}
				print(CFFFILE "        {\n");
				print(CFFFILE "        return CPtiLangDataImpl${lang_name}::NewL();\n");
				print(CFFFILE "        }\n");

				$blocks_in_use = "";
				if ($use_block_12_key == 1)
				{
					$blocks_in_use = $blocks_in_use . "12-KEY_";
				}
				if ($use_block_qwerty_standard == 1)
				{
					$blocks_in_use = $blocks_in_use . "STANDARD_";
				}
				if ($use_block_qwerty_mini == 1)
				{
					$blocks_in_use = $blocks_in_use . "4X10_";
				}
				if ($use_block_qwerty_3x11 == 1)
				{
					$blocks_in_use = $blocks_in_use . "3X11_";
				}
				if ($use_block_qwerty_half == 1)
				{
					$blocks_in_use = $blocks_in_use . "HALF_";
				}
				if ($use_block_custom == 1)
				{
					$blocks_in_use = $blocks_in_use . "CUSTOM_";
				}

				if ($blocks_in_use eq "")
				{
					die "ERROR: No data blocks selected (see USE BLOCK keyword)\n";
				}

				system("perl KeymapBuilder2.pl $keymapsdir $map_file_name $lang_name $variant_name $blocks_in_use");

				#
				# Finalize header file.
				#
				if ( ! open(CHHFILE,">>${langdir}${output_dll}${variant_name}.h") )
				{
					die "Could not finalize header file for variant ${variant_name}\n";
				}
			}
			elsif (index($line, "USE BLOCK") >= 0)
			{
				if (index($line, "12-KEY") >= 0)
				{
					$use_block_12_key = 1;
				}
				elsif (index($line, "QWERTY STANDARD") >= 0)
				{
					$use_block_qwerty_standard = 1;
				}
				elsif (index($line, "QWERTY 4X10") >= 0)
				{
					$use_block_qwerty_mini = 1;
				}
				elsif (index($line, "QWERTY 3X11") >= 0)
				{
					$use_block_qwerty_3x11 = 1;
				}
				elsif (index($line, "QWERTY HALF") >= 0)
				{
					$use_block_qwerty_half = 1;
				}
				elsif (index($line, "CUSTOM") >= 0)
				{
					$use_block_custom = 1;
				}
			}
			elsif (index($line, "USE DATASET") >= 0)
			{

				chop($line);   # Remove '\n'
				@words = split(/ +/,$line);    # Split into words.

				$keymapsdir = $keymapsdirstart;
				$keymapsdir = $keymapsdir . $words[2] . "\/";
			}
		}

endblock:
	}
}


# Close bld.inf file.
close(INFFILE);

# Finalize and close factory file for last round.
print(CFFFILE "\n");
print(CFFFILE "    return NULL;\n");
print(CFFFILE "    }\n");
print(CFFFILE "\n");
print(CFFFILE "\n");
print(CFFFILE "void CPtiVariant${variant_name}DataFactory::ListLanguagesL(RArray<TInt>& aResult)\n");
print(CFFFILE "    {\n");
     
for ($ii = 0; $ii < scalar(@lang_name_array); $ii++)
     {    
     if (@lang_name_array[$ii] eq "Basque" || @lang_name_array[$ii] eq "Galician")
            {
            print(CFFFILE "    aResult.AppendL(KLang@lang_name_array[$ii]);\n");        	                  
            }
      else
            {     
            print(CFFFILE "    aResult.AppendL(ELang@lang_name_array[$ii]);\n");
            }
     }

print(CFFFILE "    }\n");
print(CFFFILE "\n");
print(CFFFILE "// End of file\n");
close(CFFFILE);

# Finalize and close header file for last round.
print(CHHFILE "#endif\n");         
print(CHHFILE "\n// End of file. \n");         
close(CHHFILE);    

#
# Close remaining resources.
#
close(VARFILE);

close(MASTERVARFILE);

# End of file