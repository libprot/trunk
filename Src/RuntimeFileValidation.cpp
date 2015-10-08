/*
    Libprot library, v. 0.0.1. Copyright (c) 2015, Alexander Ilin

    e-mail: alexander.ilin83 (at) gmail.com  
    https://github.com/libprot/trunk

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

#include <windows.h>
#include <../inc/RuntimeFileValidation.h>

bool CRuntimeFileValidation::isFileValid()
{
	// every PE file should have valid checksum
	// if it was reset -> someone modified our application
	HANDLE hFile = GetModuleHandle(NULL);

	if (IsBadReadPtr(hFile, sizeof(PIMAGE_DOS_HEADER))) {
		return false;
	}
	char* pImage = reinterpret_cast<char*>(hFile); 

	PIMAGE_DOS_HEADER pDosHeader;
	pDosHeader = reinterpret_cast<PIMAGE_DOS_HEADER>(pImage);
	
	PIMAGE_NT_HEADERS pNtHeader;
	pNtHeader = reinterpret_cast<PIMAGE_NT_HEADERS>(pDosHeader->e_lfanew + pImage);

	return pNtHeader->OptionalHeader.CheckSum != 0;
}
