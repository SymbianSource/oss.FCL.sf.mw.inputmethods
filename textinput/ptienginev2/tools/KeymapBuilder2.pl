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
# Description:   Builds a keymap code file from given keymapping data file.
#
($time_second, $time_minute, $time_hour,          # time/date
    $date_day, $date_month, $date_year,
    $day_week, $day_year, $isdst) = localtime(time);

if ($date_year < 70) {$date_year += 2000;}        # add century
    else {$date_year += 1900;}
				   	                			      			 	                                                     			  
#
# Variables for qwerty data handling
#
$has_qwerty_data = 0;
$has_deadkey_data = 0;

$custom_matrix_width = 0;
$custom_matrix_height = 0;

#
# Qwerty keyboard type
#
$current_keyboard_data_block = "";

#
# Variants for numeric key mapping definition handling
#
$numkeyscount = 22;
@numkeynames = ("+","*","#","p","w","1","2","3","4","5","6","7","8","9","0",";",".",",","-","E","?","/"); 

###########################################################
#
# Default scan code <-> coordinate bindings for 12-key
# (these can be overrided with scan_binding keyword).
#
###########################################################
$scan_bind{'12-KEY 1 1'} = "EPtiKey1";
$scan_bind{'12-KEY 1 2'} = "EPtiKey2";
$scan_bind{'12-KEY 1 3'} = "EPtiKey3";
$scan_bind{'12-KEY 2 1'} = "EPtiKey4";
$scan_bind{'12-KEY 2 2'} = "EPtiKey5";
$scan_bind{'12-KEY 2 3'} = "EPtiKey6";
$scan_bind{'12-KEY 3 1'} = "EPtiKey7";
$scan_bind{'12-KEY 3 2'} = "EPtiKey8";
$scan_bind{'12-KEY 3 3'} = "EPtiKey9";
$scan_bind{'12-KEY 4 1'} = "EPtiKeyStar";
$scan_bind{'12-KEY 4 2'} = "EPtiKey0";
$scan_bind{'12-KEY 4 3'} = "EPtiKeyHash";

###########################################################
#
# Default scan code <-> coordinate bindings for std qwerty
# (these can be overrided with scan_binding keyword).
#
###########################################################
# no binding for (1 1), it is backlight key in E70
$scan_bind{'QWERTY STANDARD 1 2'} = "EPtiKeyQwerty1";
$scan_bind{'QWERTY STANDARD 1 3'} = "EPtiKeyQwerty2";
$scan_bind{'QWERTY STANDARD 1 4'} = "EPtiKeyQwerty3";
$scan_bind{'QWERTY STANDARD 1 5'} = "EPtiKeyQwerty4";
$scan_bind{'QWERTY STANDARD 1 6'} = "EPtiKeyQwerty5";
$scan_bind{'QWERTY STANDARD 1 7'} = "EPtiKeyQwerty6";
$scan_bind{'QWERTY STANDARD 1 8'} = "EPtiKeyQwerty7";
$scan_bind{'QWERTY STANDARD 1 9'} = "EPtiKeyQwerty8";
$scan_bind{'QWERTY STANDARD 1 10'} = "EPtiKeyQwerty9";
$scan_bind{'QWERTY STANDARD 1 11'} = "EPtiKeyQwerty0";
# no binding for (1 12), it is backspace in E70

$scan_bind{'QWERTY STANDARD 2 1'} = "EPtiKeyQwertyPlus";
$scan_bind{'QWERTY STANDARD 2 2'} = "EPtiKeyQwertyQ";
$scan_bind{'QWERTY STANDARD 2 3'} = "EPtiKeyQwertyW";
$scan_bind{'QWERTY STANDARD 2 4'} = "EPtiKeyQwertyE";
$scan_bind{'QWERTY STANDARD 2 5'} = "EPtiKeyQwertyR";
$scan_bind{'QWERTY STANDARD 2 6'} = "EPtiKeyQwertyT";
$scan_bind{'QWERTY STANDARD 2 7'} = "EPtiKeyQwertyY";
$scan_bind{'QWERTY STANDARD 2 8'} = "EPtiKeyQwertyU";
$scan_bind{'QWERTY STANDARD 2 9'} = "EPtiKeyQwertyI";
$scan_bind{'QWERTY STANDARD 2 10'} = "EPtiKeyQwertyO";
$scan_bind{'QWERTY STANDARD 2 11'} = "EPtiKeyQwertyP";
$scan_bind{'QWERTY STANDARD 2 12'} = "EPtiKeyQwertyMinus";

$scan_bind{'QWERTY STANDARD 3 1'} = "EPtiKeyQwertyHash";
$scan_bind{'QWERTY STANDARD 3 2'} = "EPtiKeyQwertyA";
$scan_bind{'QWERTY STANDARD 3 3'} = "EPtiKeyQwertyS";
$scan_bind{'QWERTY STANDARD 3 4'} = "EPtiKeyQwertyD";
$scan_bind{'QWERTY STANDARD 3 5'} = "EPtiKeyQwertyF";
$scan_bind{'QWERTY STANDARD 3 6'} = "EPtiKeyQwertyG";
$scan_bind{'QWERTY STANDARD 3 7'} = "EPtiKeyQwertyH";
$scan_bind{'QWERTY STANDARD 3 8'} = "EPtiKeyQwertyJ";
$scan_bind{'QWERTY STANDARD 3 9'} = "EPtiKeyQwertyK";
$scan_bind{'QWERTY STANDARD 3 10'} = "EPtiKeyQwertyL";
$scan_bind{'QWERTY STANDARD 3 11'} = "EPtiKeyQwertySemicolon";
$scan_bind{'QWERTY STANDARD 3 12'} = "EPtiKeyQwertyApostrophe";

# no binding for (4 1)
$scan_bind{'QWERTY STANDARD 4 2'} = "EPtiKeyQwertyZ";
$scan_bind{'QWERTY STANDARD 4 3'} = "EPtiKeyQwertyX";
$scan_bind{'QWERTY STANDARD 4 4'} = "EPtiKeyQwertyC";
$scan_bind{'QWERTY STANDARD 4 5'} = "EPtiKeyQwertyV";
$scan_bind{'QWERTY STANDARD 4 6'} = "EPtiKeyQwertyB";
$scan_bind{'QWERTY STANDARD 4 7'} = "EPtiKeyQwertyN";
$scan_bind{'QWERTY STANDARD 4 8'} = "EPtiKeyQwertyM";
$scan_bind{'QWERTY STANDARD 4 9'} = "EPtiKeyQwertyComma";
$scan_bind{'QWERTY STANDARD 4 10'} = "EPtiKeyQwertyFullstop";
$scan_bind{'QWERTY STANDARD 4 11'} = "EPtiKeyQwertySlash";
# no binding for (4 12), it is enter in E70

###########################################################
#
# Default scan code <-> coordinate bindings for 4x10 qwerty
# (these can be overrided with scan_binding keyword).
#
###########################################################
$scan_bind{'QWERTY 4X10 1 1'} = "EPtiKeyQwerty1";
$scan_bind{'QWERTY 4X10 1 2'} = "EPtiKeyQwerty2";
$scan_bind{'QWERTY 4X10 1 3'} = "EPtiKeyQwerty3";
$scan_bind{'QWERTY 4X10 1 4'} = "EPtiKeyQwerty4";
$scan_bind{'QWERTY 4X10 1 5'} = "EPtiKeyQwerty5";
$scan_bind{'QWERTY 4X10 1 6'} = "EPtiKeyQwerty6";
$scan_bind{'QWERTY 4X10 1 7'} = "EPtiKeyQwerty7";
$scan_bind{'QWERTY 4X10 1 8'} = "EPtiKeyQwerty8";
$scan_bind{'QWERTY 4X10 1 9'} = "EPtiKeyQwerty9";
$scan_bind{'QWERTY 4X10 1 10'} = "EPtiKeyQwerty0";

$scan_bind{'QWERTY 4X10 2 1'} = "EPtiKeyQwertyA";
$scan_bind{'QWERTY 4X10 2 2'} = "EPtiKeyQwertyS";
$scan_bind{'QWERTY 4X10 2 3'} = "EPtiKeyQwertyD";
$scan_bind{'QWERTY 4X10 2 4'} = "EPtiKeyQwertyF";
$scan_bind{'QWERTY 4X10 2 5'} = "EPtiKeyQwertyG";
$scan_bind{'QWERTY 4X10 2 6'} = "EPtiKeyQwertyH";
$scan_bind{'QWERTY 4X10 2 7'} = "EPtiKeyQwertyJ";
$scan_bind{'QWERTY 4X10 2 8'} = "EPtiKeyQwertyK";
$scan_bind{'QWERTY 4X10 2 9'} = "EPtiKeyQwertyL";
# No binding for (2 10), it is backspace

$scan_bind{'QWERTY 4X10 3 1'} = "EPtiKeyQwertyZ";
$scan_bind{'QWERTY 4X10 3 2'} = "EPtiKeyQwertyX";
$scan_bind{'QWERTY 4X10 3 3'} = "EPtiKeyQwertyC";
$scan_bind{'QWERTY 4X10 3 4'} = "EPtiKeyQwertyV";
$scan_bind{'QWERTY 4X10 3 5'} = "EPtiKeyQwertyB";
$scan_bind{'QWERTY 4X10 3 6'} = "EPtiKeyQwertyN";
$scan_bind{'QWERTY 4X10 3 7'} = "EPtiKeyQwertyM";
$scan_bind{'QWERTY 4X10 3 8'} = "EPtiKeyQwertyComma";
$scan_bind{'QWERTY 4X10 3 9'} = "EPtiKeyQwertyFullstop";
# No binding for (3 10), it is enter

# No binding for (4 1)
# No binding for (4 2), it is shift
#$scan_bind{'QWERTY 4X10 4 3'} = "EPtiKeyQwertySlash";
$scan_bind{'QWERTY 4X10 4 4'} = "EStdKeySquareBracketRight";
#$scan_bind{'QWERTY 4X10 4 5'} = "EStdKeySpace";
#$scan_bind{'QWERTY 4X10 4 6'} = "EStdKeySpace";
# No binding for (4 5), it is space
# No binding for (4 6), it is space
$scan_bind{'QWERTY 4X10 4 7'} = "EStdKeySquareBracketLeft";
$scan_bind{'QWERTY 4X10 4 8'} = "EStdKeySingleQuote";
# No binding for (4 8), it is ctrl;
# No binding for (4 9), it is shift
# No binding for (4 10), it is chr

###########################################################
#
# Default scan code <-> coordinate bindings for 3x11 qwerty
# (these can be overrided with scan_binding keyword).
#
###########################################################
$scan_bind{'QWERTY 3X11 1 1'} = "EPtiKeyQwertyQ";
$scan_bind{'QWERTY 3X11 1 2'} = "EPtiKeyQwertyW";
$scan_bind{'QWERTY 3X11 1 3'} = "EPtiKeyQwertyE";
$scan_bind{'QWERTY 3X11 1 4'} = "EPtiKeyQwertyR";
$scan_bind{'QWERTY 3X11 1 5'} = "EPtiKeyQwertyT";
$scan_bind{'QWERTY 3X11 1 6'} = "EPtiKeyQwertyY";
$scan_bind{'QWERTY 3X11 1 7'} = "EPtiKeyQwertyU";
$scan_bind{'QWERTY 3X11 1 8'} = "EPtiKeyQwertyI";
$scan_bind{'QWERTY 3X11 1 9'} = "EPtiKeyQwertyO";
$scan_bind{'QWERTY 3X11 1 10'} = "EPtiKeyQwertyP";
# no binding for (3 11), it is backspace

$scan_bind{'QWERTY 3X11 2 1'} = "EPtiKeyQwertyA";
$scan_bind{'QWERTY 3X11 2 2'} = "EPtiKeyQwertyS";
$scan_bind{'QWERTY 3X11 2 3'} = "EPtiKeyQwertyD";
$scan_bind{'QWERTY 3X11 2 4'} = "EPtiKeyQwertyF";
$scan_bind{'QWERTY 3X11 2 5'} = "EPtiKeyQwertyG";
$scan_bind{'QWERTY 3X11 2 6'} = "EPtiKeyQwertyH";
$scan_bind{'QWERTY 3X11 2 7'} = "EPtiKeyQwertyJ";
$scan_bind{'QWERTY 3X11 2 8'} = "EPtiKeyQwertyK";
$scan_bind{'QWERTY 3X11 2 9'} = "EPtiKeyQwertyL";
$scan_bind{'QWERTY 3X11 2 10'} = "EPtiKeyQwertyComma";
# No binding for (2 11), it is enter

# No binding for (3 1), it is shift
$scan_bind{'QWERTY 3X11 3 2'} = "EPtiKeyQwertyZ";
$scan_bind{'QWERTY 3X11 3 3'} = "EPtiKeyQwertyX";
$scan_bind{'QWERTY 3X11 3 4'} = "EPtiKeyQwertyC";
$scan_bind{'QWERTY 3X11 3 5'} = "EPtiKeyQwertyV";
$scan_bind{'QWERTY 3X11 3 6'} = "EPtiKeyQwertyB";
$scan_bind{'QWERTY 3X11 3 7'} = "EPtiKeyQwertyN";
$scan_bind{'QWERTY 3X11 3 8'} = "EPtiKeyQwertyM";
$scan_bind{'QWERTY 3X11 3 9'} = "EStdKeySpace";
# No binding for (3 10), it is space
# No binding for (3 11), it is fn

###########################################################
#
# Default scan code <-> coordinate bindings for half qwerty
# (these can be overrided with scan_binding keyword). 
#
###########################################################
# TO BE DEFINED (these are working asumptions).
$scan_bind{'QWERTY HALF 1 1'} = "EPtiKeyQwertyQ"; 
$scan_bind{'QWERTY HALF 1 2'} = "EPtiKeyQwerty1";
$scan_bind{'QWERTY HALF 1 3'} = "EPtiKeyQwerty2";
$scan_bind{'QWERTY HALF 1 4'} = "EPtiKeyQwerty3";
$scan_bind{'QWERTY HALF 1 5'} = "EPtiKeyQwertyO";

$scan_bind{'QWERTY HALF 2 1'} = "EPtiKeyQwertyA";
$scan_bind{'QWERTY HALF 2 2'} = "EPtiKeyQwerty4";
$scan_bind{'QWERTY HALF 2 3'} = "EPtiKeyQwerty5";
$scan_bind{'QWERTY HALF 2 4'} = "EPtiKeyQwerty6";
$scan_bind{'QWERTY HALF 2 5'} = "EPtiKeyQwertyL";

$scan_bind{'QWERTY HALF 3 1'} = "EPtiKeyQwertyZ";
$scan_bind{'QWERTY HALF 3 2'} = "EPtiKeyQwerty7";
$scan_bind{'QWERTY HALF 3 3'} = "EPtiKeyQwerty8";
$scan_bind{'QWERTY HALF 3 4'} = "EPtiKeyQwerty9";
$scan_bind{'QWERTY HALF 3 5'} = "EPtiKeyQwertyFullstop";

# No binding
$scan_bind{'QWERTY HALF 4 2'} = "EPtiKeyQwertyChr";
$scan_bind{'QWERTY HALF 4 3'} = "EPtiKeyQwertySpace";
$scan_bind{'QWERTY HALF 4 4'} = "EPtiKeyQwertyLeftShift";
# No binding, it is enter

#
# Global variables.
#
$inputpath = shift(@ARGV);
$filename  = shift(@ARGV);
$langname = shift(@ARGV);
$variant_name = shift(@ARGV);
$blocks_in_use = shift(@ARGV);
$inputfile = $inputpath . $filename; 

$langconstant = "ELang${langname}";

if ($langname eq "Galician")
    {
    $langconstant = "KLangGalician";	
    }
if ($langname eq "Basque")
    {
    $langconstant = "KLangBasque";	
    }


$output_dll = "PtiKeymappings_";
$outputdir = "..\/languages\/";
$mmpext = ".mmp";
$dlluid = $langdll{$langname};
$impluid = $langimpl{$langname};
$mtindex = 0;
$print_not_latin_language = 0;

$has_numeric_mode_data = 0;

$for_zhuyin_tag_value = "0x2462";
$for_pinyin_tag_value = "0x2460";
$for_stroke_tag_value = "0x2461";
$for_cangjie_tag_value = "0x2463";
$get_all_marker_value = "0x2464";

$qwerty_num_keys = 48;

#############################################################
#
# Subrountine for counting characters in data row
#
#############################################################
sub countchars {
     ($instr)=@_; 
     @details=split /,/, $instr;	
     $numchars = $#details + 1;
   }

##############################################################   
#
# Subroutine for converting qwerty key cordinates into 
# EPtiKey constants.
#   
##############################################################
sub convert_coordinates
    {
	$converted_coordinates = $scan_bind{$_[0]};	
    }  

##############################################################
#
# Subroutine print_data_block for exporting a data block.
# Parameters: boardname constant
#             number of rows in keyboard matrix 
#             number of collumns in keyboard matrix
#             data block name
#
##############################################################
sub print_data_block
    {
    my $boardname = shift;
    my $rows = shift;
    my $cols = shift;
    my $blockname = shift;
            
    print(OUTFILE "//\n");	
    print(OUTFILE "// $boardname keymap data table\n");	
    print(OUTFILE "//\n");		
    print(OUTFILE "const TUint16 ${boardname}Data_table_${langname}[] =\n");
    print(OUTFILE "    {\n");

    $mtindex = 0;

    for ($i = 1; $i <= $rows; $i++)
       {            
       for ($j = 1; $j <= $cols; $j++)
           { 
           $keyname = "$blockname $i $j";                 
           &countchars($keys{$keyname});
           $bindnumchrs{$keyname} = $numchars;
           $bindindex{$keyname} = $mtindex;
           $mtindex = $mtindex + $bindnumchrs{$keyname};
           if ($numchars > 0)
               {	              
               print(OUTFILE "    $keys{$keyname},\n");	
               }
           }
       }  
   
    for ($i=1; $i <= $rows; $i++)
        {
        for ($j = 1; $j <= $cols; $j++)
            {
            $keyname = "$blockname $i $j";                       
            &countchars($keys_u{$keyname});
            $bindnumchrs_u{$keyname} = $numchars;
            $bindindex_u{$keyname} = $mtindex;
            $mtindex = $mtindex + $bindnumchrs_u{$keyname};
            if ($numchars > 0)
                {            
                print(OUTFILE "    $keys_u{$keyname},\n");	
                }
            }
        }
	if ($has_chr_keys{$blockname} == 1)
        {    
        for ($i = 1; $i <= $rows; $i++)
            {
            for ($j = 1; $j <= $cols; $j++)
                {
                $keyname = "$blockname $i $j";
                &countchars($keys_chr{$keyname});
                $chr_bindnumchrs{$keyname} = $numchars;
                $chr_bindindex{$keyname} = $mtindex;
                $mtindex = $mtindex + $chr_bindnumchrs{$keyname};
                if ($numchars > 0)
                    {	
                    print(OUTFILE "    $keys_chr{$keyname},\n");	
                    }
                }
            }

        for ($i = 1; $i <= $rows; $i++)
            {
            for ($j = 1; $j <= $cols; $j++)
                {            
                $keyname = "$blockname $i $j"; 
                &countchars($keys_chr_u{$keyname});
                $chr_bindnumchrs_u{$keyname} = $numchars;
                $chr_bindindex_u{$keyname} = $mtindex;
                $mtindex = $mtindex + $chr_bindnumchrs_u{$keyname};
                if ($numchars > 0)
                    {
                    print(OUTFILE "    $keys_chr_u{$keyname},\n");	
                    }    
                }            
            }                            
        }  
    if ($has_fn_keys{$blockname} == 1)
        {    
        for ($i = 1; $i <= $rows; $i++)
            {
            for ($j = 1; $j <= $cols; $j++)
                {
                $keyname = "$blockname $i $j";
                &countchars($keys_fn{$keyname});
                $fn_bindnumchrs{$keyname} = $numchars;
                $fn_bindindex{$keyname} = $mtindex;
                $mtindex = $mtindex + $fn_bindnumchrs{$keyname};
                if ($numchars > 0)
                    {	
                    print(OUTFILE "    $keys_fn{$keyname},\n");	
                    }
                }
            }

        for ($i = 1; $i <= $rows; $i++)
            {
            for ($j = 1; $j <= $cols; $j++)
                {            
                $keyname = "$blockname $i $j"; 
                &countchars($keys_fn_u{$keyname});
                $fn_bindnumchrs_u{$keyname} = $numchars;
                $fn_bindindex_u{$keyname} = $mtindex;
                $mtindex = $mtindex + $fn_bindnumchrs_u{$keyname};
                if ($numchars > 0)
                    {
                    print(OUTFILE "    $keys_fn_u{$keyname},\n");	
                    }    
                }            
            }                            
        }
        
    print(OUTFILE "    };\n");    
    print(OUTFILE "\n");
    print(OUTFILE "const TInt K${boardname}DataSize_${langname} = $mtindex;\n");  
    print(OUTFILE "\n");
    
    my $keybindingsize = 0;

    print(OUTFILE "\n");
    print(OUTFILE "const TPtiKeyBinding ${boardname}KeyBindings_table_${langname}[] =\n");
    print(OUTFILE "    {\n");     
  
    for ($i = 1; $i <= $rows; $i++)
       {
       for ($j = 1; $j <= $cols; $j++)
           {           
           $keyname = "$blockname $i $j";                
           $scanbindname = $scan_bind{$keyname};
           if (length($scanbindname) > 0)
               {
               print(OUTFILE "        {$scanbindname, EPtiCaseLower, $bindindex{$keyname}, $bindnumchrs{$keyname}},\n");                
               $keybindingsize++;
               }
           }
       }

    for ($i = 1; $i <= $rows; $i++)
        {
        for ($j = 1; $j <= $cols; $j++)
            {
            $keyname = "$blockname $i $j";  
            $scanbindname = $scan_bind{$keyname};                          
            if (length($scanbindname) > 0)
               {            
               print(OUTFILE "        {$scanbindname, EPtiCaseUpper, $bindindex_u{$keyname}, $bindnumchrs_u{$keyname}},\n");
               $keybindingsize++;
               }
            }
        }
    if ($has_chr_keys{$blockname} == 1)
           {      
           for ($i = 1; $i <= $rows; $i++)
               {
               for ($j = 1; $j <= $cols; $j++)
                   {
                   $keyname = "$blockname $i $j";  
                   $scanbindname = $scan_bind{$keyname}; 
                   if (length($scanbindname) > 0 && $chr_bindnumchrs{$keyname} > 0)
                       {                                                   
                       print(OUTFILE "        {$scanbindname, EPtiCaseChrLower, $chr_bindindex{$keyname}, $chr_bindnumchrs{$keyname}},\n"); 
                       $keybindingsize++;
                       }
                   }
               }           
           
           for ($i = 1; $i <= $rows; $i++)
               {
               for ($j = 1; $j <= $cols; $j++)
                   {
                   $keyname = "$blockname $i $j";                
                   $scanbindname = $scan_bind{$keyname};               
                   if (length($scanbindname) > 0 && $chr_bindnumchrs_u{$keyname} > 0)
                       {    
                       print(OUTFILE "        {$scanbindname, EPtiCaseChrUpper, $chr_bindindex_u{$keyname}, $chr_bindnumchrs_u{$keyname}},\n"); 
                       $keybindingsize++;
                       }
                   }
               }                                                  
           }                    
    if ($has_fn_keys{$blockname} == 1)
           {      
           for ($i = 1; $i <= $rows; $i++)
               {
               for ($j = 1; $j <= $cols; $j++)
                   {
                   $keyname = "$blockname $i $j";  
                   $scanbindname = $scan_bind{$keyname}; 
                   if (length($scanbindname) > 0 && $fn_bindnumchrs{$keyname} > 0)
                       {                                                   
                       print(OUTFILE "        {$scanbindname, EPtiCaseFnLower, $fn_bindindex{$keyname}, $fn_bindnumchrs{$keyname}},\n"); 
                       $keybindingsize++;
                       }
                   }
               }           
           
           for ($i = 1; $i <= $rows; $i++)
               {
               for ($j = 1; $j <= $cols; $j++)
                   {
                   $keyname = "$blockname $i $j";                
                   $scanbindname = $scan_bind{$keyname};               
                   if (length($scanbindname) > 0 && $fn_bindnumchrs_u{$keyname} > 0)
                       {    
                       print(OUTFILE "        {$scanbindname, EPtiCaseFnUpper, $fn_bindindex_u{$keyname}, $fn_bindnumchrs_u{$keyname}},\n"); 
                       $keybindingsize++;
                       }
                   }
               }                                                  
           }

    print(OUTFILE "    };\n");    
    print(OUTFILE "\n");

    print(OUTFILE "const TInt KMax${boardname}Index_${langname} = ${keybindingsize};\n");      
    
    print(OUTFILE "\n");   

    #
    # Add data table for dead keys
    #
    if ($has_deadkeys{$blockname} == 1)
	    {
        print(OUTFILE "\n");
        print(OUTFILE "//\n");	
        print(OUTFILE "// Dead key data table for ${boardname}\n");	
        print(OUTFILE "//\n");		
        print(OUTFILE "const TUint16 ${boardname}DeadKeyData_table_${langname}[] =\n");	
        print(OUTFILE "    {\n");

        $dead_key_table_rows = 0;
        $dead_index = 0;
        for ($i = 1; $i <= $rows; $i++)
            {
            for ($j = 1; $j <= $cols; $j++)
                {
                $keyname = "$blockname $i $j";                 
           			&countchars($keys{$keyname});
                if ( $numchars > 0 )
                		{
                		&countchars($deadkeys{$keyname});
                		if ($numchars > 0)
                    		{   
							print(OUTFILE "    $dead_index, $deadkeys{$keyname},\n");	
                			$dead_key_table_rows++;
                    		}
                		$dead_index++;
                    }
                }
            }
  
       for ($i = 1; $i <= $rows; $i++)
           {
           for ($j = 1; $j <= $cols; $j++)
               {
               $keyname = "$blockname $i $j";                 
               &countchars($keys{$keyname});
							 if ( $numchars > 0 )
                		{
               			$keyname = "$blockname $i $j";                     
               			&countchars($deadkeys_u{$keyname});
               			if ($numchars > 0)
                   			{
                   			print(OUTFILE "    $dead_index, $deadkeys_u{$keyname},\n");	
	               			$dead_key_table_rows++;
                   			}
                     	$dead_index++;
               			}
               }
           }
           
    print(OUTFILE "    };\n");
    print(OUTFILE "\n");	
    print(OUTFILE "const TInt K${boardname}DeadKeyTableRows_${langname} = $dead_key_table_rows;\n");	
    print(OUTFILE "\n");	
	}       
	   	   
    #	
    # Add data table for numeric mode keys
    #	
    if ($has_numeric_data{$blockname} == 1)
	    {	
        print(OUTFILE "\n//\n");	
        print(OUTFILE "// Numeric key data table for ${boardname}\n");	
        print(OUTFILE "//\n");		
        print(OUTFILE "const TPtiNumericKeyBinding ${boardname}NumericKeysData_table_${langname}[] =\n");
        print(OUTFILE "	{\n");

        $countnumericentries = 0;
        for ($aaa = 0; $aaa < $numkeyscount; $aaa++)
	        {	
	        $keystr = "$blockname $numkeynames[$aaa]";
            if (index ($numeric_mode_key{$keystr}, "(") >= 0)
		        {			
		        print(OUTFILE "	{");
		
		        $leftind = index($numeric_mode_key{$keystr}, "(");
    	        $rightind = index($numeric_mode_key{$keystr}, ")");
    	        $keyname = substr($numeric_mode_key{$keystr}, $leftind + 1, $rightind - $leftind - 1); 			

		        &convert_coordinates("$blockname $keyname");
		        if (index($converted_coordinates, "EPti") >= 0)
			        {
			        print(OUTFILE "\'$numkeynames[$aaa]\',");
			        print(OUTFILE "$converted_coordinates,");
  		            if (index($numeric_mode_key{$keystr}, "fn_upper") >= 0)
				        {
				        print(OUTFILE "EPtiCaseFnUpper");							
				        }
  		            elsif (index($numeric_mode_key{$keystr}, "fn_lower") >= 0)
				        {
				        print(OUTFILE "EPtiCaseFnLower");							
				        }            				        
			        elsif (index($numeric_mode_key{$keystr}, "upper") >= 0)
				        {
				        print(OUTFILE "EPtiCaseUpper");							
				        }
			        elsif (index($numeric_mode_key{$keystr}, "chr_lower") >= 0)
				        {
				        print(OUTFILE "EPtiCaseChrLower");							
				        }				        				        
			        elsif (index($numeric_mode_key{$keystr}, "chr_upper") >= 0)
				        {
				        print(OUTFILE "EPtiCaseChrUpper");							
				        }				        				        				        
			        else
				        {
				        print(OUTFILE "EPtiCaseLower");											
				        }							
			        }
		        else
			        {
			        die ("ERROR: Invalid numeric key coordinates for $langname\n");
			        }	
		
		        print(OUTFILE "},\n");
		        $countnumericentries++;	
		        }
		    }
		    		    
        print(OUTFILE "	};\n");	
        print(OUTFILE "\nconst TInt KNum${boardname}NumericKeysEntries_${langname} = $countnumericentries;\n");				    	
	    }	   
	   	   
    #	
    # Add data table for vietnamese vowel sequences
    #		
    if (index($langname, "Vietnamese") >= 0 && ( $blockname eq "QWERTY STANDARD" || $blockname eq "QWERTY 4X10" || $blockname eq "QWERTY 3X11") )
	    {
        print(OUTFILE "\n");	
        # Vietnamese vowel sequence data is currently statically defined here. 
        # Parser section for it can be implemented later if needed.
        print(OUTFILE "const TVowelSequence ${boardname}VowelSeq_table_${langname}[] =\n");
        print(OUTFILE "	{\n");

		print(OUTFILE "	{TPtiKey(0x0041), TPtiKey(0x01AF), {0x0103, 0x0102}},\n");
        print(OUTFILE "	{TPtiKey(0x0041), TPtiKey(0x0041), {0x00e2, 0x00c2}},\n");
        print(OUTFILE "	{TPtiKey(0x0045), TPtiKey(0x0045), {0x00ea, 0x00ca}},\n");
        print(OUTFILE "	{TPtiKey(0x004F), TPtiKey(0x01AF), {0x01a1, 0x01a0}},\n");
        print(OUTFILE "	{TPtiKey(0x004F), TPtiKey(0x004F), {0x00f4, 0x00d4}},\n");
        print(OUTFILE "	{TPtiKey(0x0055), TPtiKey(0x01AF), {0x01b0, 0x01af}},\n");
        print(OUTFILE "	{TPtiKey(0x0044), TPtiKey(0x0044), {0x0111, 0x0110}},\n");
        print(OUTFILE "	{TPtiKey(0x01AF), TPtiKey(0x01AF), {0x0077, 0x0057}}\n");
        
        print(OUTFILE "	};\n");	
        print(OUTFILE "\nconst TInt KNum${boardname}VowelSeqEntries_${langname} = 8;\n");	
	    }			   	   	    
   }      

#
# print_data_block ends
#


##############################################################
#
# Parse file and fill tables.
#
##############################################################

# Open cscan binding file if present
if ( ! open(SCANBINDINGFILE,"${inputpath}scan_binding.txt") )
{
	#print("\nINFORMATION: No scan binding file found\n");
}

$line_number = 1;
@master_raw_data=<SCANBINDINGFILE>;
foreach $line (@master_raw_data)
{
	if ((index($line, "#") >= 0))
	{
		# Comment line, just skip
	}
	elsif (index($line, "[KEYBOARD BLOCK") >= 0)
	{
		# New qwerty keyboard data block is starting.
		if (index($line, "QWERTY STANDARD]") >= 0)
		{
			$current_keyboard_data_block = "QWERTY STANDARD";
		}
		elsif (index($line, "12-KEY]") >= 0)
		{
			$current_keyboard_data_block = "12-KEY";
		}
		elsif (index($line, "QWERTY 4X10") >= 0)
		{
			$current_keyboard_data_block = "QWERTY 4X10";
		}
		elsif (index($line, "QWERTY 3X11") >= 0)
		{
			$current_keyboard_data_block = "QWERTY 3X11";
		}
		elsif (index($line, "QWERTY HALF") >= 0)
		{
			$current_keyboard_data_block = "QWERTY HALF";
		}
		elsif (index($line, "QWERTY CUSTOM") >= 0)
		{
			$current_keyboard_data_block = "QWERTY CUSTOM";
		}
		else
		{
			die "Syntax error: Unknown keyboard data block in file ${inputpath}scan_binding.txt line ${line_number}\n";
		}
	}
	else
	{
		if (index($line, "scan_binding") >= 0)
		{
			# Parse coordinates
			$leftind = index($line, "(");
			$rightind = index($line, ")");

			if ($leftind < 0 || $rightind < 0 || $rightind <= $leftind)
			{
				die ("Syntax error in ${inputpath}scan_binding.txt line ${line_number}\n");
			}

			$keyname = substr($line, $leftind + 1, $rightind - $leftind - 1);
			$full_key_name = "$current_keyboard_data_block $keyname";

			$scan_bind{$full_key_name} = substr($line, $rightind + 1, length($line) - $rightind - 2);
		}
	}
	$line_number = $line_number + 1;
}
close(SCANBINDINGFILE);

if ( ! open(INFILE, "${inputfile}") )
    {
    die "Could not open input file ${inputfile}"; 	
    }

$line_number = 1;

#
# The main parser loop
#
@raw_data=<INFILE>;
foreach $line (@raw_data)
	{
	if ((index($line, "#") >= 0) && (index($line, "numeric_mode_key") < 0))
	    {
	    # Comment line, just skip
	    }	    	    
	elsif (index($line, "[KEYBOARD BLOCK") >= 0)	
        {
         # New qwerty keyboard data block is starting.
        if (index($line, "QWERTY STANDARD]") >= 0)
            {
            $current_keyboard_data_block = "QWERTY STANDARD"; 
            $has_data{"QWERTY STANDARD"} = 1;                                                                 
	        $has_qwerty_data = 1;	                        
            }
        elsif (index($line, "12-KEY]") >= 0)
            {
           	$has_data{"12-KEY"} = 1;		
            $current_keyboard_data_block = "12-KEY";           	
            }            
        elsif (index($line, "QWERTY 4X10") >= 0)
            {
            $current_keyboard_data_block = "QWERTY 4X10";    
            $has_data{"QWERTY 4X10"} = 1;                                 	
            $has_qwerty_data = 1;                                        
            } 
        elsif (index($line, "QWERTY 3X11") >= 0)
            {
            $current_keyboard_data_block = "QWERTY 3X11";    
            $has_data{"QWERTY 3X11"} = 1;                                 	
            $has_qwerty_data = 1;                                        
            }             
        elsif (index($line, "QWERTY HALF") >= 0)
            {
            $current_keyboard_data_block = "QWERTY HALF";
            $has_data{"QWERTY HALF"} = 1;                        	                                     
            $has_qwerty_data = 1;
            }    
        elsif (index($line, "QWERTY CUSTOM") >= 0)
            {
            $current_keyboard_data_block = "QWERTY CUSTOM";
            $has_data{"QWERTY CUSTOM"} = 1;                        	                                     
            $has_qwerty_data = 1;            
            }            
        else
            {    
            die "Syntax error: Unknown keyboard data block in file ${inputfile} line ${line_number}\n";
            }
        }
    else
        {            
        #
        # Convert special tags.
        #
        while (index($line, "ForZhuyin") >= 0)
        	{        	
        	substr($line, index($line, "ForZhuyin"), 9) = $for_zhuyin_tag_value;          	
        	}

        while (index($line, "ForPinyin") >= 0)
        	{
        	substr($line, index($line, "ForPinyin"), 9) = $for_pinyin_tag_value;          	
        	}
        	
 		while (index($line, "ForStroke") >= 0)
        	{
        	substr($line, index($line, "ForStroke"), 9) = $for_stroke_tag_value;          	
        	}        	
        	
 		while (index($line, "ForCangjie") >= 0)
        	{
        	substr($line, index($line, "ForCangjie"), 10) = $for_cangjie_tag_value;          	
        	}        	      
        	
 		while (index($line, "GetAllMarker") >= 0)
        	{        	
        	substr($line, index($line, "GetAllMarker"), 12) = $get_all_marker_value;          	
        	}  
        	
        #
        # Convert hexdecimal formats to C-format.
        #	      	      
 		while (index($line, "\\U") >= 0)
        	{
        	substr($line, index($line, "\\U"), 2) = "0x";          	
        	}  
               	
		while (index($line, "U+") >= 0)
        	{
        	substr($line, index($line, "U+"), 2) = "0x";          	
        	}   
        	
		#
		# Parse possible language code
		#        	        	      		
		if (index($line, "language_code") >= 0)
			{		
			$langconstant = substr($line, 13, length($line) - 14);
			}        	
               	           
		#            
        # Parse key data.
        #  
		if ((index($line, "key") >= 0) && (index($line, "numeric_mode") < 0) && (index($line, "dead_key_data") < 0) && (index($line, "deadkeydata") < 0))
		    {
		    if ($current_keyboard_data_block eq "")
		        {      
		        die "ERROR: No keyboard block was specified in file ${inputfile} line ${line_number}\n";
		        }
		    
		    # Parse coordinates
	        $leftind = index($line, "(");
		    $rightind = index($line, ")");			
		    		
	        if ($leftind < 0 || $rightind < 0 || $rightind <= $leftind)
		       	{
		    	die ("Syntax error in ${inputfile} line ${line_number}\n");
		       	}		
		       	
	        $keyname = substr($line, $leftind + 1, $rightind - $leftind - 1); 			       	    		
            $full_key_name = "$current_keyboard_data_block $keyname";	        
		    	
	        if (index($line, "fn_upper") > 3)			
			    {
                if ($current_keyboard_data_block eq "12-KEY")
                    {	                  
           	        die "ERROR: Fn-keys not supported for given keyboard block in file ${inputfile} line ${line_number}\n"; 		        	                            	    	
		    	   	}
                else
                    {                               
                    $has_fn_keys{$current_keyboard_data_block} = 1;
   		    	   	$keys_fn_u{$full_key_name} = substr($line, $rightind + 1, length($line) - $rightind - 2);		                                            
                    }		    	   			    	   	          		    
			    }
            elsif (index($line, "fn_lower") > 3)
                {
                if ($current_keyboard_data_block eq "12-KEY")
                    {	                  
           	        die "ERROR: Fn-keys not supported for given keyboard block in file ${inputfile} line ${line_number}\n"; 		        	                            	    	
		    	   	}
                else
                    {                               
                    $has_fn_keys{$current_keyboard_data_block} = 1;
   		    	   	$keys_fn{$full_key_name} = substr($line, $rightind + 1, length($line) - $rightind - 2);		                                            
                    }		    	                                      
                }
			elsif (index($line, "chr_lower") > 3)
                {
                if ($current_keyboard_data_block eq "12-KEY")
                    {	                  
           	        die "ERROR: chr-keys not supported for given keyboard block in file ${inputfile} line ${line_number}\n"; 		        	                            	    	
		    	   	}
                else
                    {                               
                    $has_chr_keys{$current_keyboard_data_block} = 1;
   		    	   	$keys_chr{$full_key_name} = substr($line, $rightind + 1, length($line) - $rightind - 2);
                    }		    	                                      
                }
            elsif (index($line, "chr_upper") > 3)
                {
                if ($current_keyboard_data_block eq "12-KEY")
                    {	                  
           	        die "ERROR: chr-keys not supported for given keyboard block in file ${inputfile} line ${line_number}\n"; 		        	                            	    	
		    	   	}
                else
                    {                               
                    $has_chr_keys{$current_keyboard_data_block} = 1;
   		    	   	$keys_chr_u{$full_key_name} = substr($line, $rightind + 1, length($line) - $rightind - 2);
                    }		    	                                      
                }        			    			  		    		    
			elsif (index($line, "upper") > 3)
		        {	
	    	   	$keys_u{$full_key_name} = substr($line, $rightind + 1, length($line) - $rightind - 2);				        	    					    		    	        		  		    			    	                  
		        }		  			    
			else  # has to be lower   
				{	
                $keys{$full_key_name} = substr($line, $rightind + 1, length($line) - $rightind - 2);		                        								    		        		    	   		    			    			    			    			    							
				}		        		    		 		  		    
 		    }
 		    
		if (index($line, "latin_like_language") >= 0)
		    {
            $print_not_latin_language = 0;
     		if (index($line, " no") >= 0 or index($line, "=no") >= 0)
	            {
                $print_not_latin_language = 1;				
			    }
            }
            
		if (index($line, "numeric_mode_key") >= 0)
		    {
		    $has_numeric_mode_data = 1;	
		    $has_numeric_data{$current_keyboard_data_block} = 1;	        
			$leftind = index($line, "(");
			$rightind = index($line, ")");
			
		    if ($leftind < 0 || $rightind < 0 || $rightind <= $leftind)
		       	{
		    	die ("Syntax error in file ${inputfile} line ${line_number}\n");
		       	}	
		       	
            $numeric_mode_key_name = substr($line, 17, 1); 		       		    
		    $full_numeric_mode_key_name = "$current_keyboard_data_block $numeric_mode_key_name";
            $numeric_mode_key{$full_numeric_mode_key_name} = substr($line, 19);		    		    		       	           			       	
            }
            	           	 
        if (index($line, "dead_key_data") >= 0 || index($line, "deadkeydata") >= 0)
        	{        	        	
			$has_deadkey_data = 1;
			$has_deadkeys{$current_keyboard_data_block} = 1;	
			
    		$leftind = index($line, "(");
	    	$rightind = index($line, ")");

	    	if ($leftind < 0 || $rightind < 0 || $rightind <= $leftind)
	        	{
	    		die ("Syntax error in file ${inputfile} line ${line_number}\n");
	        	}

	        $keyname = substr($line, $leftind + 1, $rightind - $leftind - 1); 
            $full_key_name = "$current_keyboard_data_block $keyname";	   	        
	        				    	        
			if (index($line, "upper") > 4)
		        {	
    	    	$deadkeys_u{$full_key_name} = substr($line, $rightind + 1, length($line) - $rightind - 2);		                        		        	  			        		        	    
		        }
			else
				{
	    	    $deadkeys{$full_key_name} = substr($line, $rightind + 1, length($line) - $rightind - 2);		                        		        	  			        		        	    		 														    							
				}		        		           	
        	}    
        	
        if (index($line, "matrix") >= 0)
            {
            if ($current_keyboard_data_block eq "QWERTY CUSTOM")
                {               
                @parts = split(/ /, $line);
                $custom_matrix_width = $parts[1];
                $custom_matrix_height = $parts[2];                                                
                }
            else
                {    
                die "ERROR: Custom matrix supported only by custom block in file ${inputfile} line ${line_number}\n"; 
                }
            } 
            
        if (index($line, "scan_binding") >= 0)
            {
            # Parse coordinates
	        $leftind = index($line, "(");
		    $rightind = index($line, ")");			
		    		
	        if ($leftind < 0 || $rightind < 0 || $rightind <= $leftind)
		       	{
		    	die ("Syntax error in ${inputfile} line ${line_number}\n");
		       	}		
		       	
	        $keyname = substr($line, $leftind + 1, $rightind - $leftind - 1); 			       	    		
            $full_key_name = "$current_keyboard_data_block $keyname";	                  
            
            $scan_bind{$full_key_name} = substr($line, $rightind + 1, length($line) - $rightind - 2);            
            }                   	        	
        }
    
    $line_number = $line_number + 1;
	}
	
# All done, close the file and handle the results.	
close(INFILE);


##############################################################
#
# Open output cpp-file
#
##############################################################

if ( ! open(OUTFILE, ">>${outputdir}${output_dll}${variant_name}.cpp") )
    {
    die "Could not open input file ${output_dll}${variant_name}.cpp\n"; 	
    }


##############################################################
#
# Open output h-file
#
##############################################################

if ( ! open(HEADER, ">>${outputdir}${output_dll}${variant_name}.h") )
    {
    die "Could not open input file ${output_dll}${variant_name}.h\n"; 	
    }

##############################################################
#
# Allow config-file to cut off data blocks
#
##############################################################

if (index($blocks_in_use, "12-KEY") < 0)
    {
    $has_data{"12-KEY"} = 0;
    }
if (index($blocks_in_use, "STANDARD") < 0)
    {
    $has_data{"QWERTY STANDARD"} = 0;
    $has_deadkeys{"QWERTY STANDARD"} = 0; 
    $has_numeric_data{"QWERTY STANDARD"} = 0;      
    }
if (index($blocks_in_use, "4X10") < 0)
    {
    $has_data{"QWERTY 4X10"} = 0;
    $has_deadkeys{"QWERTY 4X10"} = 0;        
    $has_numeric_data{"QWERTY 4X10"} = 0;   
    }
if (index($blocks_in_use, "3X11") < 0)
    {
    $has_data{"QWERTY 3X11"} = 0;
    $has_deadkeys{"QWERTY 3X11"} = 0;        
    $has_numeric_data{"QWERTY 3X11"} = 0;   
    }    
if (index($blocks_in_use, "HALF") < 0)
    {
    $has_data{"QWERTY HALF"} = 0;
    $has_deadkeys{"QWERTY HALF"} = 0;        
    $has_numeric_data{"QWERTY HALF"} = 0;   
    }
if (index($blocks_in_use, "CUSTOM") < 0)
    {
    $has_data{"QWERTY CUSTOM"} = 0;
    $has_deadkeys{"QWERTY CUSTOM"} = 0;        
    $has_numeric_data{"QWERTY CUSTOM"} = 0;   
    }        

##############################################################
#
# Add data tables for multittapping keymap data.
#
##############################################################
if ($has_data{"12-KEY"} == 1)
    {
    &print_data_block("Multitap", 4, 3, "12-KEY");
    }

##############################################################
#
# Add data tables for standard 4x12 qwerty mapping data.
#
##############################################################
if ($has_data{"QWERTY STANDARD"} == 1)
    {
    &print_data_block("StandardQwerty", 4, 12, "QWERTY STANDARD");    	
    }  

##############################################################
#
# Add data tables for 4x10 qwerty mapping data.
#
##############################################################
if ($has_data{"QWERTY 4X10"} == 1)
    {
    &print_data_block("MiniQwerty", 4, 10, "QWERTY 4X10");  
    }   

##############################################################
#
# Add data tables for 3x11 qwerty mapping data.
#
##############################################################
if ($has_data{"QWERTY 3X11"} == 1)
    {
    &print_data_block("MiniQwerty3x11", 3, 11, "QWERTY 3X11");  
    }   
                               
##############################################################
#
# Add data table for half qwerty mapping data.
#
##############################################################
if ($has_data{"QWERTY HALF"} == 1)
    {
    &print_data_block("HalfQwerty", 4, 5, "QWERTY HALF");      
    }    
    
##############################################################
#
# Add data table for custom qwerty mapping data.
#
##############################################################
if ($has_data{"QWERTY CUSTOM"} == 1 && $custom_matrix_width > 0)
    {
    &print_data_block("CustomQwerty", $custom_matrix_width, $custom_matrix_height, "QWERTY CUSTOM");  
    }        
    
##############################################################    
#    
# Add class declaration to header file
#   
##############################################################
print(HEADER "\n//\n"); 
print(HEADER "// Class declaration for language $langname \n"); 
print(HEADER "//\n");
print(HEADER "class CPtiLangDataImpl${langname} : public CPtiKeyMapData\n");
print(HEADER "    {\n");
print(HEADER "    public:\n");
print(HEADER "        static CPtiLangDataImpl${langname}* NewL();\n");
print(HEADER "        TInt LanguageCode() const;\n");
print(HEADER "        const TUint16* KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const;\n"); 
print(HEADER "        const TPtiKeyBinding* KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const;\n");
if ($has_deadkey_data == 1)
	{
# There is dead key data, add a method for dead key data table.	
print(HEADER "        const TUint16* DeadKeyDataArray(TPtiKeyboardType aKeyboardType, TInt& aNumRowsInArray) const;\n");
	}
		
if ($has_numeric_mode_data == 1)
	{
# There is numeric data, add a method for numeric data table.	
print(HEADER "        const TPtiNumericKeyBinding* NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const;\n");        	
	}	
	
if (index($langname, "Vietnamese") >= 0)
	{
print(HEADER "        const TVowelSequence* VowelSequenceTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const;\n");
	}
   
if ($print_not_latin_language == 1) {
print(HEADER "        TBool SuitableForLatinOnlyMode() const;\n");
   }
print(HEADER "    };\n");
print(HEADER "\n");

###################################################
#
# Add function bodies
#
###################################################

print(OUTFILE "//\n"); 
print(OUTFILE "// Class implementation.\n"); 
print(OUTFILE "//\n");
print(OUTFILE "\n");

#
# Create body for NewL
#
print(OUTFILE "CPtiLangDataImpl${langname}* CPtiLangDataImpl${langname}::NewL()\n");
print(OUTFILE "    {\n");
print(OUTFILE "    return new (ELeave) CPtiLangDataImpl${langname}();\n");
print(OUTFILE "    }\n");
print(OUTFILE "\n");
print(OUTFILE "\n");

#
# Create body for CPtiLangDataImpl::LanguageCode()
#
print(OUTFILE "TInt CPtiLangDataImpl${langname}::LanguageCode() const\n");
print(OUTFILE "    {\n");
print(OUTFILE "    return ${langconstant};\n");
print(OUTFILE "    }\n");
print(OUTFILE "\n");
print(OUTFILE "\n");
		
#
# Create body for CPtiLangDataImpl::KeyData()
#	
print(OUTFILE "const TUint16* CPtiLangDataImpl${langname}::KeyData(TPtiKeyboardType aKeyboardType, TInt& aDataSize) const\n");
print(OUTFILE "    {\n");

if ($has_data{"12-KEY"} == 1)
    {
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboard12Key)\n");
    print(OUTFILE "        {\n");
    print(OUTFILE "        aDataSize = KMultitapDataSize_${langname};\n");
    print(OUTFILE "        return MultitapData_table_${langname};\n");	
    print(OUTFILE "        }\n");
    }
    
if ($has_data{"QWERTY STANDARD"} == 1)
    {
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty4x12)\n");
    print(OUTFILE "        {\n");
    print(OUTFILE "        aDataSize = KStandardQwertyDataSize_${langname};\n");
    print(OUTFILE "        return StandardQwertyData_table_${langname};\n");
    print(OUTFILE "        }\n");
    print(OUTFILE "\n");        
    }
    
if ($has_data{"QWERTY 4X10"} == 1)
    {
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty4x10)\n");
    print(OUTFILE "        {\n");
    print(OUTFILE "        aDataSize = KMiniQwertyDataSize_${langname};\n");
    print(OUTFILE "        return MiniQwertyData_table_${langname};\n");
    print(OUTFILE "        }\n");
    print(OUTFILE "\n");        
    }    
        
if ($has_data{"QWERTY 3X11"} == 1)
    {
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty3x11)\n");
    print(OUTFILE "        {\n");
    print(OUTFILE "        aDataSize = KMiniQwerty3x11DataSize_${langname};\n");
    print(OUTFILE "        return MiniQwerty3x11Data_table_${langname};\n");
    print(OUTFILE "        }\n");
    print(OUTFILE "\n");        
    }            
        
if ($has_data{"QWERTY HALF"} == 1)
    {
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboardHalfQwerty)\n");
    print(OUTFILE "        {\n");
    print(OUTFILE "        aDataSize = KHalfQwertyDataSize_${langname};\n");
    print(OUTFILE "        return HalfQwertyData_table_${langname};\n");
    print(OUTFILE "        }\n");
    print(OUTFILE "\n");        
    }  
    
if ($has_data{"QWERTY CUSTOM"} == 1)
    {
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboardCustomQwerty)\n");
    print(OUTFILE "        {\n");
    print(OUTFILE "        aDataSize = KCustomQwertyDataSize_${langname};\n");
    print(OUTFILE "        return CustomQwertyData_table_${langname};\n");
    print(OUTFILE "        }\n");
    print(OUTFILE "\n");        
    }        
            
print(OUTFILE "    aDataSize = 0;\n");
print(OUTFILE "    return NULL;\n");	
print(OUTFILE "    }\n");

#
# Create body for CPtiLangDataImpl::KeyBindingTable()
#
print(OUTFILE "\n");	
print(OUTFILE "\n");						
print(OUTFILE "const TPtiKeyBinding* CPtiLangDataImpl${langname}::KeyBindingTable(TPtiKeyboardType aKeyboardType, TInt& aNumItems) const\n");
print(OUTFILE "    {\n");

if ($has_data{"12-KEY"} == 1)
    {
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboard12Key)\n");
    print(OUTFILE "        {\n");    
    print(OUTFILE "        aNumItems = KMaxMultitapIndex_${langname};\n");
    print(OUTFILE "        return MultitapKeyBindings_table_${langname};\n");			
    print(OUTFILE "        }\n");
    }
    
if ($has_data{"QWERTY STANDARD"} == 1)
    {    
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty4x12)\n");
    print(OUTFILE "        {\n");
    print(OUTFILE "        aNumItems = KMaxStandardQwertyIndex_${langname};\n");
    print(OUTFILE "        return StandardQwertyKeyBindings_table_${langname};\n");
    print(OUTFILE "        }\n");
    }
    
if ($has_data{"QWERTY 4X10"} == 1)
    {    
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty4x10)\n");
    print(OUTFILE "        {\n");
    print(OUTFILE "        aNumItems = KMaxMiniQwertyIndex_${langname};\n");
    print(OUTFILE "        return MiniQwertyKeyBindings_table_${langname};\n");
    print(OUTFILE "        }\n");
    }    
    
if ($has_data{"QWERTY 3X11"} == 1)
    {    
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty3x11)\n");
    print(OUTFILE "        {\n");
    print(OUTFILE "        aNumItems = KMaxMiniQwerty3x11Index_${langname};\n");
    print(OUTFILE "        return MiniQwerty3x11KeyBindings_table_${langname};\n");
    print(OUTFILE "        }\n");
    }        
    
if ($has_data{"QWERTY HALF"} == 1)
    {    
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboardHalfQwerty)\n");
    print(OUTFILE "        {\n");
    print(OUTFILE "        aNumItems = KMaxHalfQwertyIndex_${langname};\n");
    print(OUTFILE "        return HalfQwertyKeyBindings_table_${langname};\n");
    print(OUTFILE "        }\n");
    }        
    
if ($has_data{"QWERTY CUSTOM"} == 1)
    {    
    print(OUTFILE "    if (aKeyboardType == EPtiKeyboardCustomQwerty)\n");
    print(OUTFILE "        {\n");
    print(OUTFILE "        aNumItems = KMaxCustomQwertyIndex_${langname};\n");
    print(OUTFILE "        return CustomQwertyKeyBindings_table_${langname};\n");
    print(OUTFILE "        }\n");
    }                
    
print(OUTFILE "\n");            
print(OUTFILE "    aNumItems = 0;\n");
print(OUTFILE "    return NULL;\n");	    
print(OUTFILE "    }\n");
  
  
if ($has_qwerty_data == 1) {
print(OUTFILE "\n");
print(OUTFILE "\n");
				
#
# Add body for dead key data table method.
#
if ($has_deadkey_data == 1)
	{						
    print(OUTFILE "const TUint16* CPtiLangDataImpl${langname}::DeadKeyDataArray(TPtiKeyboardType aKeyboardType, TInt& aNumRowsInArray) const\n");
    print(OUTFILE "    {\n");

    if ($has_deadkeys{"QWERTY STANDARD"} == 1)
        {
        print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty4x12)\n");
        print(OUTFILE "        {\n");
        print(OUTFILE "        aNumRowsInArray = KStandardQwertyDeadKeyTableRows_${langname};\n");
        print(OUTFILE "        return StandardQwertyDeadKeyData_table_${langname};\n");
        print(OUTFILE "        }\n");
        print(OUTFILE "\n");
        }
    
    if ($has_deadkeys{"QWERTY 4X10"} == 1)
        {
        print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty4x10)\n");
        print(OUTFILE "        {\n");
        print(OUTFILE "        aNumRowsInArray = KMiniQwertyDeadKeyTableRows_${langname};\n");
        print(OUTFILE "        return MiniQwertyDeadKeyData_table_${langname};\n");
        print(OUTFILE "        }\n");
        print(OUTFILE "\n");
        }    
        
    if ($has_deadkeys{"QWERTY 3X11"} == 1)
        {
        print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty3x11)\n");
        print(OUTFILE "        {\n");
        print(OUTFILE "        aNumRowsInArray = KMiniQwerty3x11DeadKeyTableRows_${langname};\n");
        print(OUTFILE "        return MiniQwerty3x11DeadKeyData_table_${langname};\n");
        print(OUTFILE "        }\n");
        print(OUTFILE "\n");
        }            
    
    if ($has_deadkeys{"QWERTY CUSTOM"} == 1)
        {
        print(OUTFILE "    if (aKeyboardType == EPtiKeyboardCustomQwerty)\n");
        print(OUTFILE "        {\n");
        print(OUTFILE "        aNumRowsInArray = KCustomQwertyDeadKeyTableRows_${langname};\n");
        print(OUTFILE "        return CustomQwertyDeadKeyData_table_${langname};\n");
        print(OUTFILE "        }\n");
        print(OUTFILE "\n");
        }        
    
    print(OUTFILE "    aNumRowsInArray = 0;\n");
    print(OUTFILE "    return NULL;\n");		
    print(OUTFILE "    }\n");
    print(OUTFILE "\n");	
    print(OUTFILE "\n");
    }

#
# Add body for numeric mode data table method.
#
if ($has_numeric_mode_data == 1)
	{						
    print(OUTFILE "const TPtiNumericKeyBinding* CPtiLangDataImpl${langname}::NumericModeKeysTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const\n");
    print(OUTFILE "    {\n");

    if ($has_numeric_data{"QWERTY STANDARD"} == 1)
        {
        print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty4x12)\n");
        print(OUTFILE "        {\n");
        print(OUTFILE "        aNumEntries = KNumStandardQwertyNumericKeysEntries_${langname};\n");
        print(OUTFILE "        return StandardQwertyNumericKeysData_table_${langname};\n");
        print(OUTFILE "        }\n");
        print(OUTFILE "\n");
        }
    
    if ($has_numeric_data{"QWERTY 4X10"} == 1)
        {
        print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty4x10)\n");
        print(OUTFILE "        {\n");
        print(OUTFILE "        aNumEntries = KNumMiniQwertyNumericKeysEntries_${langname};\n");
        print(OUTFILE "        return MiniQwertyNumericKeysData_table_${langname};\n");
        print(OUTFILE "        }\n");
        print(OUTFILE "\n");
        }    
        
    if ($has_numeric_data{"QWERTY 3X11"} == 1)
        {
        print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty3x11)\n");
        print(OUTFILE "        {\n");
        print(OUTFILE "        aNumEntries = KNumMiniQwerty3x11NumericKeysEntries_${langname};\n");
        print(OUTFILE "        return MiniQwerty3x11NumericKeysData_table_${langname};\n");
        print(OUTFILE "        }\n");
        print(OUTFILE "\n");
        }            
    
    if ($has_numeric_data{"QWERTY CUSTOM"} == 1)
        {
        print(OUTFILE "    if (aKeyboardType == EPtiKeyboardCustomQwerty)\n");
        print(OUTFILE "        {\n");
        print(OUTFILE "        aNumEntries = KNumCustomQwertyNumericKeysEntries_${langname};\n");
        print(OUTFILE "        return CustomQwertyNumericKeysData_table_${langname};\n");
        print(OUTFILE "        }\n");
        print(OUTFILE "\n");
        }        
    
    print(OUTFILE "    aNumEntries = 0;\n");
    print(OUTFILE "    return NULL;\n");		
    print(OUTFILE "    }\n");
    print(OUTFILE "\n");	
    print(OUTFILE "\n");
	}	

#    
# Add body for vowel sequence data table.   
#    
if (index($langname, "Vietnamese") >= 0)
	{						
print(OUTFILE "const TVowelSequence* CPtiLangDataImpl${langname}::VowelSequenceTable(TPtiKeyboardType aKeyboardType, TInt& aNumEntries) const\n");
print(OUTFILE "    {\n");

if ($has_data{"QWERTY STANDARD"} == 1)
    {
print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty4x12)\n");
print(OUTFILE "        {\n");
print(OUTFILE "        aNumEntries = KNumStandardQwertyVowelSeqEntries_${langname};\n");
print(OUTFILE "        return StandardQwertyVowelSeq_table_${langname};\n");
print(OUTFILE "        }\n");
print(OUTFILE "\n");
    }

if ($has_data{"QWERTY 4X10"} == 1)
	{
print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty4x10)\n");
print(OUTFILE "        {\n");
print(OUTFILE "        aNumEntries = KNumMiniQwertyVowelSeqEntries_${langname};\n");
print(OUTFILE "        return MiniQwertyVowelSeq_table_${langname};\n");
print(OUTFILE "        }\n");
print(OUTFILE "\n");
    }       

if ($has_data{"QWERTY 3X11"} == 1)
	{
print(OUTFILE "    if (aKeyboardType == EPtiKeyboardQwerty3x11)\n");
print(OUTFILE "        {\n");
print(OUTFILE "        aNumEntries = KNumMiniQwerty3x11VowelSeqEntries_${langname};\n");
print(OUTFILE "        return MiniQwerty3x11VowelSeq_table_${langname};\n");
print(OUTFILE "        }\n");
print(OUTFILE "\n");
    }  
print(OUTFILE "    aNumEntries = 0;\n");
print(OUTFILE "    return NULL;\n");			
print(OUTFILE "    }\n");		
print(OUTFILE "\n");	
print(OUTFILE "\n");
	}    
  }

if ($print_not_latin_language == 1) {
print(OUTFILE "\nTBool CPtiLangDataImpl${langname}::SuitableForLatinOnlyMode() const\n");
print(OUTFILE "    {\n");
print(OUTFILE "    return EFalse;\n");
print(OUTFILE "    }\n");
print(OUTFILE "\n");
print(OUTFILE "\n");
   }

close(OUTFILE);

# End of file