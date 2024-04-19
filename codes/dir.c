//
//	HEKTREE - 17/04/2024
//
// dir.c
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
#include "dir.h"

queue_t \
	Queue = NULL;
int \
	IsPrintReport = FALSE, \
	IsPrintExtendedReport = FALSE;
float \
	ScanTime, \
	PreparationTime;
char \
	*TargetPath;
long int \
	TotalDirectoryCount, \
	LevelCount, \
	LevelCounters[ 1 + MAX_PATH ];
		
#define dir_is_owner(p)(!queue_is_empty(((dir_ptr)(p))->Children))
#define dir_is_last(p)((((dir_ptr)(p))->Parent != NULL)?(((dir_ptr)(p))->Sibling == NULL):(FALSE))
#define dir_connection(p)(dir_is_last((p))?("\xC0"):("\xC3"))
#define dir_padding(p)((((dir_ptr)(p))->Parent == NULL)?("\0"):((dir_is_owner((p)) && (!dir_is_last((p))))?("\xB3   "):("    ")))

void Create( void )
{ 
	Queue = new_queue(); 
	
	ScanTime = 0.0f, \
	PreparationTime = 0.0f;
	
	TotalDirectoryCount = 0L;
	LevelCount = 0;

	for (int i = 0; i < MAX_PATH; i++)
	{ LevelCounters[ i ] = -1L; }	
}

char *GetParentPaddings( dir_ptr inParent )
{
	dir_ptr \
		l_ptrParent = inParent;
	char \
		*l_strResult = "", \
		*l_strPadding;
		
	while (l_ptrParent != NULL)
	{
		l_strPadding = strdup( dir_padding(l_ptrParent) );
		
		if (l_strResult != NULL)
		{ 
			_concat(&l_strPadding, l_strResult);
			
			free( (void *)l_strResult );
			
			l_strResult = l_strPadding;
		}
		else
		{ l_strResult = l_strPadding; }
		
		l_ptrParent = l_ptrParent->Parent; 
	}
	
	return l_strResult;
}

char *ToString( dir_ptr inDirPtr )
{
	
	char \
		*l_strResult = inDirPtr->Name;
	
	if (inDirPtr->Parent != NULL)
	{
		l_strResult = GetParentPaddings( inDirPtr->Parent );
	
		_concat(&l_strResult, dir_connection(inDirPtr));
		_concat(&l_strResult, "\xC4\xC4\xC4");
			
		_concat(&l_strResult, inDirPtr->Name);
	}
	
	return l_strResult;
}

dir_ptr CreateDirPtr( const char *inName, dir_ptr inParent )
{
	dir_ptr \
		l_ptrDir = (dir_ptr)malloc( sizeof(struct dir_t) );
		
	l_ptrDir->Sibling = NULL;

	l_ptrDir->Children = NULL;

	l_ptrDir->Level = ((inParent != NULL) ? (1 + inParent->Level) : (0));

	l_ptrDir->Name = strdup( inName );

	if ((l_ptrDir->Parent = (inParent)) == NULL)
	{ queue_push( Queue, l_ptrDir ); }
	else
	{ queue_push( inParent->Children = ((inParent->Children == NULL) ? new_queue() : inParent->Children), l_ptrDir ); }
	
	return l_ptrDir;
}

void DestroyDirPtr( dir_ptr inDirPtr )
{
	queue_t \
		l_ptrChildren = inDirPtr->Children;
	
	free( inDirPtr->Name );
	
	inDirPtr->Children = (queue_t)NULL;
	
	if (!queue_is_empty( l_ptrChildren ))
	{ delete_queue( &l_ptrChildren, (free_handler_t)DestroyDirPtr ); }
	
	inDirPtr->Parent = inDirPtr->Sibling = (dir_ptr)NULL;
	
	inDirPtr->Level = 0;
	
	inDirPtr->Name = (char *)NULL;
	
	free( inDirPtr );
}

void Destroy( void )
{
	queue_t \
		l_ptrQueue = Queue;
		
	Queue = NULL;
	
	delete_queue( &l_ptrQueue, (free_handler_t)DestroyDirPtr );
	
	free( l_ptrQueue );
}

void ScanDir( const char *inPath, dir_ptr inDirPtr )
{
	WIN32_FIND_DATAA \
		l_varFindData; 
	HANDLE \
		l_hFindObject;
	int \
		l_iPathLength = strlen( inPath );
	char \
		*l_strPath = strdup( inPath ), \
		*l_strSubPath, \
		*l_strSearchPattern;

	if (l_strPath[ l_iPathLength - 1 ] != '\\')
	{ _concat(&l_strPath, "\\"); }
	
	l_strSearchPattern = strdup( l_strPath );
	_concat(&l_strSearchPattern, "*.*");
	
	if ((l_hFindObject = FindFirstFileA( l_strSearchPattern, &l_varFindData )) != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ((l_varFindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0) continue;
			if ((l_varFindData.dwFileAttributes & (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM)) != 0) continue;
			if (*((short *)&l_varFindData.cFileName[ 0 ]) == (short)'.') continue;
			if ((l_varFindData.cFileName[ 0 ] == '.') && (l_varFindData.cFileName[ 1 ] == '.')) continue;

			l_strSubPath = strdup( l_strPath );
			_concat(&l_strSubPath, &l_varFindData.cFileName[ 0 ]);

			ScanDir( l_strSubPath, CreateDirPtr( (const char *)&l_varFindData.cFileName[ 0 ], inDirPtr ) );
			
			free( (void *)l_strSubPath );
		}
		while (FindNextFileA( l_hFindObject, &l_varFindData ));
		
		FindClose( l_hFindObject );
	}
	
	free( (void *)l_strSearchPattern );
	free( (void *)l_strPath );
}

int DrawTreeView( const dir_ptr inDirPtr, void *inExtraParam )
{
	dir_ptr \
		l_ptrDir = inDirPtr;
		
	printf( "%s\r\n", ToString( l_ptrDir ) );
	
	TotalDirectoryCount++;
	LevelCount = max(LevelCount, l_ptrDir->Level);
	
	LevelCounters[ l_ptrDir->Level ] = max(0, LevelCounters[ l_ptrDir->Level ]);

	LevelCounters[ l_ptrDir->Level ]++;
	
	if (!queue_is_empty( l_ptrDir->Children )) 
	{ queue_explore( l_ptrDir->Children, (explore_handler_t)DrawTreeView, NULL ); }

	return !0;
}

#define MAX_REPORT_WIDTH (4)

void PrintTopLines( int inStartIndex )
{
	for (int j = 0; j < MAX_REPORT_WIDTH; j++)
	{
		if (LevelCounters[ (inStartIndex + j) ] == -1) break;
		printf( "\t\xDA\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xBF" );
	}
}

void PrintSeparatorLines( int inStartIndex )
{
	for (int j = 0; j < MAX_REPORT_WIDTH; j++)
	{ 
		if (LevelCounters[ (inStartIndex + j) ] == -1) break;
		printf( "\t\xC3\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xB4" ); 
	}
}

void PrintBottomLines( int inStartIndex )
{
	for (int j = 0; j < MAX_REPORT_WIDTH; j++)
	{
		if (LevelCounters[ (inStartIndex + j) ] == -1) break;
		printf( "\t\xC0\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xC4\xD9" );
	}
}

void PrintExtendedReport( void )
{
	int \
		i, j, \
		l_iEndOfLevel;

	for (i = 0; i < MAX_PATH; i += MAX_REPORT_WIDTH)
	{
		l_iEndOfLevel = FALSE;
		
		for (j = 0; ((j < MAX_REPORT_WIDTH) && (!l_iEndOfLevel)); j++)
		{ l_iEndOfLevel = (LevelCounters[ (i + j) ] == -1); }
		
		PrintTopLines( i );

		printf( "\r\n" );

		for (j = 0; j < MAX_REPORT_WIDTH; j++)
		{
			if (LevelCounters[ (i + j) ] == -1) break;
			printf( "\t\xB3 Level %3d \xB3", (i + j) );
		}
		
		printf( "\r\n" );
		
		PrintSeparatorLines( i );
		
		printf( "\r\n" );
		
		for (j = 0; j < MAX_REPORT_WIDTH; j++)
		{
			if (LevelCounters[ (i + j) ] == -1) break;
			printf( "\t\xB3 %9ld \xB3", LevelCounters[ (i + j) ] );
		}
		
		printf( "\r\n" );
			
		PrintBottomLines( i );
		
		printf( "\r\n" );
	
		if (l_iEndOfLevel) break;
	}
}

void PrintReport()
{
	printf( "\r\n\r\n*** REPORT:\r\n" );
	printf( "\r\n\tPath: '%s'\r\n\r\n", TargetPath );
	printf( "\tScan Time:        %.3fsec\r\n", ScanTime );
	printf( "\tPreparation Time: %.3fsec\r\n", PreparationTime );
	printf( "\tTotal Time:       %.3fsec\r\n", (ScanTime + PreparationTime) );
	printf( "\r\n\tTotal Directory:  %ld\r\n", TotalDirectoryCount );
	printf( "\tLevel Count:      %d\r\n\r\n", (1 + LevelCount) );

	if (IsPrintExtendedReport)
	{ PrintExtendedReport(); }
}

void Scan()
{ 
	DWORD \
		l_dwBeginTick;
	
	Create();
	
	l_dwBeginTick = GetTickCount();
	{ ScanDir( TargetPath, CreateDirPtr( TargetPath, NULL ) );  }
	ScanTime = ((GetTickCount() - l_dwBeginTick) / 1000.0f);
	
	l_dwBeginTick = GetTickCount();
	{ queue_explore( Queue, (explore_handler_t)DrawTreeView, NULL ); }
	PreparationTime = ((GetTickCount() - l_dwBeginTick) / 1000.0f);
	
	Destroy();
}
