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
#include "stdafx.h"
#include "PECheckSumCalc.h"
#include "PECheckSumCalcDlg.h"
#include <ImageHlp.h>
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CPECheckSumCalcDlg dialog

CPECheckSumCalcDlg::CPECheckSumCalcDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPECheckSumCalcDlg::IDD, pParent)
{
}

void CPECheckSumCalcDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPECheckSumCalcDlg, CDialog)
    ON_BN_CLICKED(IDC_FINDBUTTON, &CPECheckSumCalcDlg::OnBnOpenDialogClicked)
    ON_BN_CLICKED(IDC_PROCESSBUTTON, &CPECheckSumCalcDlg::OnBnProcessClicked)
END_MESSAGE_MAP()

void writeProcess(LPVOID lpBaseAddress, LPCVOID lpBuffer, int nSize)
{
	DWORD dwProtectValue = PAGE_NOACCESS;
	HANDLE hProcess = GetCurrentProcess();
	BOOL bResult = WriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, NULL);
	if (bResult == FALSE) 
	{
		bResult = VirtualProtect(lpBaseAddress, nSize, PAGE_WRITECOPY, &dwProtectValue); 
		if (bResult != FALSE) {
			WriteProcessMemory(hProcess, lpBaseAddress, lpBuffer, nSize, NULL);
		}
	}
	VirtualProtect(lpBaseAddress, nSize, dwProtectValue, &dwProtectValue);
}
void CPECheckSumCalcDlg::OnBnOpenDialogClicked()
{
    // simple open dialog to get file name
    OPENFILENAME openFileName;
    ZeroMemory(&openFileName, sizeof(openFileName));
    
    TCHAR strFileName[MAX_PATH];
    ZeroMemory(strFileName, MAX_PATH);
 
    openFileName.hwndOwner = AfxGetMainWnd()->GetSafeHwnd();
    openFileName.nMaxFile = MAX_PATH;
    openFileName.Flags = OFN_EXPLORER;
    openFileName.lpstrFile = strFileName;
    openFileName.lpstrFilter =  _T("Executable files\0*.exe;*.dll\0");
    openFileName.lpstrDefExt = _T("");
    openFileName.lpstrTitle = _T("Select executable file");
    openFileName.lStructSize = sizeof(OPENFILENAMEA);

    GetOpenFileName(&openFileName);
    if ((int)CommDlgExtendedError() == 0) {
        GetDlgItem(IDC_FILEPATHEDIT)->SetWindowText(strFileName);
    }
}


void CPECheckSumCalcDlg::OnBnProcessClicked()
{
    // Needs to open a file for reading & writing
    GetDlgItem(IDC_FILEPATHEDIT)->GetWindowText(m_strFileName);
    HANDLE hFile = CreateFile(m_strFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        return;
    }
    DWORD dwFileSize = GetFileSize(hFile, NULL);

    // the mapping is easiest way to calculate check sum and rewrite it
    HANDLE hFileMapping = CreateFileMapping(hFile, NULL, PAGE_READWRITE, 0, 0, NULL);
    if (hFileMapping == NULL) {
        CloseHandle(hFile);
        return;
    }

    DWORD dwHdrCheckSum, dwCheckSum;
    LPVOID pBaseAddr = MapViewOfFile(hFileMapping, FILE_MAP_WRITE, 0, 0, dwFileSize);
    if (pBaseAddr == NULL) {
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return;
    }
    PIMAGE_NT_HEADERS pImageNTHdr = CheckSumMappedFile(pBaseAddr, dwFileSize, &dwHdrCheckSum, &dwCheckSum);
    if (pImageNTHdr == NULL) {
        UnmapViewOfFile(pBaseAddr);
        CloseHandle(hFileMapping);
        CloseHandle(hFile);
        return;
    }
    // Save the control check sum
    pImageNTHdr->OptionalHeader.CheckSum = dwCheckSum;
    UnmapViewOfFile(pBaseAddr);
    CloseHandle(hFileMapping);
    CloseHandle(hFile);
	EndDialog(0);
}
