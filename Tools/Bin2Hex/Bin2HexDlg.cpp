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
#include "Bin2Hex.h"
#include "Bin2HexDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBin2HexDlg dialog
CBin2HexDlg::CBin2HexDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBin2HexDlg::IDD, pParent)
{
}

void CBin2HexDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBin2HexDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CBin2HexDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_SELECTALL, &CBin2HexDlg::OnBnClickedSelectall)
END_MESSAGE_MAP()

TCHAR GetHexCharacter(unsigned char halfbyte) {
	if ((halfbyte & 0xF) < 10) {
		return (TCHAR)('0' + (halfbyte & 0xF));
	} else {
		return (TCHAR)('A' - 10 + (halfbyte & 0xF));
	}
}

CString HexEncode(unsigned char byte) {
	CString output = _T("0x");
	output.AppendChar(GetHexCharacter(byte >> 4));
	output.AppendChar(GetHexCharacter(byte));
	return output;
}

void CBin2HexDlg::ProcessFile(TCHAR *szFile)
{
	HANDLE handle = CreateFile(szFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (handle == INVALID_HANDLE_VALUE) {
		return;
	}
	CEdit pEdit;
	pEdit.Attach(GetDlgItem(IDC_OUTPUT)->GetSafeHwnd());
	pEdit.Clear();

	unsigned char buffer[1024];
	bool addedComma = true;
	CString output;
	DWORD written;
	int items = -1;
	while (ReadFile(handle, &buffer, 1024, &written, NULL) != 0) {
		for (unsigned int i = 0; i < written; i++) {
			if (addedComma == false) {
				output += _T(", ");
			}
			addedComma = false;
			items++;
			if (items >= 10) {
				items = 0;
				output += _T("\r\n");
			}
			
			output += HexEncode(buffer[i]);
		}

		if (written < 1024) {
			break;
		}
	}
	pEdit.SetWindowText(output);
	pEdit.Detach();
	CloseHandle(handle);
}

void CBin2HexDlg::OnBnClickedBrowse()
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
	openFileName.lpstrFilter =  _T("Binary files\0*.*\0");
	openFileName.lpstrDefExt = _T("");
	openFileName.lpstrTitle = _T("Select a binary file");
	openFileName.lStructSize = sizeof(OPENFILENAMEA);

	GetOpenFileName(&openFileName);
	if ((int)CommDlgExtendedError() == 0) {
		GetDlgItem(IDC_FILEPATHEDIT)->SetWindowText(strFileName);
	}
	ProcessFile(strFileName);
}

void CBin2HexDlg::OnBnClickedSelectall()
{
	CEdit pEdit;
	pEdit.Attach(GetDlgItem(IDC_OUTPUT)->GetSafeHwnd());
	pEdit.SetSel(0, -1, FALSE);
	pEdit.Detach();
}
