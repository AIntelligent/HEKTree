//
//	HEKTREE - 17/04/2024
//
// main.c
//
// Author:
//       Hakan Emre Kartal <hek@nula.com.tr>
//
// Copyright (c) 2024 Hakan Emre Kartal
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <direct.h>

#include "dir.h"


#define PARAM_ABOUT_SHORT						"-A"
#define PARAM_ABOUT								"--about"

#define PARAM_HELP_SHORT 						"-H"
#define PARAM_HELP 								"--help"

#define PARAM_PRINT_REPORT_SHORT 			"-R"
#define PARAM_PRINT_REPORT 					"--report"

#define PARAM_PRINT_EXTENDED_REPORT_SHORT "-E"
#define PARAM_PRINT_EXTENDED_REPORT 		"--extended"

	
char *GetAppName( void )
{	
	static char \
		*AppName = NULL;
		
	if (AppName != NULL)
	{ return AppName; }

	char \
		l_arrResult[ 1 + MAX_PATH ], \
		*l_strAppFullPathName;
	int \
		i, \
		l_iAppNameLength, \
		j;
		
	GetModuleFileNameA( (HANDLE)NULL, &l_arrResult[ 0 ], MAX_PATH );
	l_strAppFullPathName = strdup( l_arrResult );
	
	l_arrResult[ 0 ] = (char)0;
	
	i = strlen( l_strAppFullPathName );	
	while ((--i > 0) && (l_strAppFullPathName[ i ] != '.'));
	
	l_iAppNameLength = j = 0;
	while ((--i > 0) && (l_strAppFullPathName[ i ] != '\\')) 
	{ l_iAppNameLength++; }
	
	strncpy( &l_arrResult[ 0 ], &l_strAppFullPathName[ i + 1 ], l_iAppNameLength );
	l_arrResult[ l_iAppNameLength ] = (char)0;
	
	free( (void *)l_strAppFullPathName );
	
	return AppName = strdup( &l_arrResult[ 0 ] );
}


void About( void )
{
	printf( "\r\nHEK DIRECTORY TREEVIEW (%s), Version 2.0\r\n\r\n", GetAppName() );
	
	printf( "The program was originally written by Hakan E. Kartal in 1996\r\n" );
	printf( "using Turbo Pascal 6.0 for MS-DOS operaing systems, and was\r\n" );
	printf( "written in 2024 using C for MS Windows operating systems,\r\n" );
	printf( "remaining faithful to the original\xFB.\r\n\r\n" );
	printf( "https://github.com/AIntelligent, hek@nula.com.tr\r\n\r\n\r\n" );
	printf( "1: https://github.com/AIntelligent/MSDOS/tree/8c10759459cbc325bb3951b2b51de73e2b428746/HEKAGAC\r\n" );
}

void Help( void )
{
	printf( "\r\n*** HELP:\r\n\r\n" );
	printf( "What does it to?\r\n\r\n" );
	printf( "\tThis program show the connection diagram of the specified folder\r\n" );
	printf( "\tand all its subfolders as a tree.\r\n" );
	printf( "\r\nHow to usage:\r\n\r\n" );
	
	printf( "\t%s [Path] %s[%s] %s[%s] %s[%s] %s[%s]\r\n", GetAppName(),
			  PARAM_ABOUT_SHORT, PARAM_ABOUT, PARAM_HELP_SHORT, PARAM_HELP,
			  PARAM_PRINT_REPORT_SHORT, PARAM_PRINT_REPORT, 
			  PARAM_PRINT_EXTENDED_REPORT_SHORT, PARAM_PRINT_EXTENDED_REPORT );
	
	printf( "\r\nParameters:\r\n\r\n" );
	printf( "\t[Path]: Folder to scan.\r\n" );
	printf( "\t\tNote: If the target folder is not specified, the active folder -\r\n" );
	printf( "\t\twill be scanned.\r\n\r\n" );
	printf( "\t%s[%s]: Show about information.\r\n", PARAM_ABOUT_SHORT, PARAM_ABOUT );
	printf( "\t%s[%s]: Show this informations.\r\n", PARAM_HELP_SHORT, PARAM_HELP );
	printf( "\t%s[%s]: Show summary report.\r\n", PARAM_PRINT_REPORT_SHORT, PARAM_PRINT_REPORT );
	printf( "\t%s[%s]: Show extended summary report.\r\n", PARAM_PRINT_EXTENDED_REPORT_SHORT, PARAM_PRINT_EXTENDED_REPORT );
	printf( "\t\t\tNote: This parameter can only be used with -\r\n" );
	printf( "\t\t\tthe %s[%s] parameter.\r\n", PARAM_PRINT_REPORT_SHORT, PARAM_PRINT_REPORT );

}

int main( int inArgumentCount, char *inArguments[] ) 
{
	char \
		l_strTargetPath[ 1 + MAX_PATH ] = { '\0' };
	int \
		i;

	GetAppName();
	
	for (i = 1; i < inArgumentCount; i++)
	{
		if ((strcmpi( inArguments[ i ], PARAM_HELP_SHORT ) == 0)
			|| (strcmpi( inArguments[ i ], PARAM_HELP ) == 0))
		{
			Help();
			return ERROR_SUCCESS;
		}
		else if ((strcmpi( inArguments[ i ], PARAM_ABOUT_SHORT ) == 0)
			|| (strcmpi( inArguments[ i ], PARAM_ABOUT ) == 0))
		{
			About();
			return ERROR_SUCCESS;
		}
		else if (is_dir( inArguments[ i ] ))
		{ strcpy( &l_strTargetPath[ 0 ], inArguments[ i ] ); }
		else if ((strcmpi( inArguments[ i ], PARAM_PRINT_REPORT_SHORT ) == 0)
			|| (strcmpi( inArguments[ i ], PARAM_PRINT_REPORT ) == 0))
		{ IsPrintReport = TRUE; }
		else if ((strcmpi( inArguments[ i ], PARAM_PRINT_EXTENDED_REPORT_SHORT ) == 0)
			|| (strcmpi( inArguments[ i ], PARAM_PRINT_EXTENDED_REPORT ) == 0))
		{ IsPrintExtendedReport = TRUE; }
	}
	
	if (l_strTargetPath[ 0 ] == '\0')
	{ getcwd( &l_strTargetPath[ 0 ], MAX_PATH ); }
	
	TargetPath = strdup( &l_strTargetPath[ 0 ] );
	Scan();

	if (IsPrintReport)
	{ PrintReport(); }
	
	return ERROR_SUCCESS;
}