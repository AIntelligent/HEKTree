//
//	HEKTREE - 17/04/2024
//
// tools.c
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
#include "tools.h"

char *concat( const char *inTarget, const char *inSource )
{
	char \
		*l_strTarget = (char *)inTarget;
	int \
		l_iTargetLength = strlen( l_strTarget ), \
		l_iSourceLength = strlen( inSource );
	char \
		*l_strResult = (char *)malloc( 1 + l_iTargetLength + l_iSourceLength );

	strcat( strcpy( l_strResult, l_strTarget ), inSource );
	
	free( (void *)l_strTarget );
	
	return l_strResult;
}

int is_dir( const char *inPath )
{
	HANDLE \
		l_hFindObject;
	WIN32_FIND_DATAA \
		l_varFindData; 
	char \
		*l_strPath = strdup( inPath );
	int \
		i, \
		l_iResult = FALSE, \
		l_iPathLength = strlen( l_strPath );
		
	if (3 > l_iPathLength)
	{ return FALSE; }

	i = 0;
	while (l_strPath[ (l_iPathLength - (1 + i)) ] == '.') i++;
	
	l_strPath[ l_iPathLength - i ] = (char)0;
	
	if (l_strPath[ l_iPathLength - (i + 1) ] != '\\')
	{ l_strPath = concat( l_strPath, "\\" ); }
	
	i += (int)(i == 0);
	
	while (i-- > 0)
	{ l_strPath = concat(l_strPath, "."); }
	
	if ((l_hFindObject = FindFirstFileA( l_strPath, &l_varFindData )) != INVALID_HANDLE_VALUE)
	{
		l_iResult = ((l_varFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);
		FindClose( l_hFindObject );
	}
	
	free( (void *)l_strPath );
	
	return l_iResult;
}