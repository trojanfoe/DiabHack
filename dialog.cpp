/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/dialog.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

CHack *g_pHackDisplayProps = NULL;  // Used by DisplayHackPropsDlgProc

BOOL CALLBACK
ReadDiabloMemoryDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG: {
        SetWindowText(hwnd, "Read Diablo Memory");
        SetDlgItemText(hwnd, IDC_RWDM_DOIT, "&Read");
        HWND hwndEC = GetDlgItem(hwnd, IDC_RWDM_DATA);
        ASSERT(hwndEC != NULL);
        SendMessage(hwndEC, EM_SETREADONLY, TRUE, 0);
        }
        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDC_RWDM_DOIT: {
                DWORD dwOffset, dwLen;
                if (!GetDlgItemHex(hwnd, IDC_RWDM_OFFSET, "Offset", dwOffset) ||
                    !GetDlgItemHex(hwnd, IDC_RWDM_LEN, "Length", dwLen)) {
                    return TRUE;
                }
                LPBYTE pBuffer = new BYTE [dwLen];
                if (pBuffer == NULL) {
                    MsgBox(MB_ICONSTOP|MB_OK, "Memory allocation failure");
                    return TRUE;
                }
                CProc::ReportMemIoErrors(TRUE);
                HANDLE hProc = CProc::Open("Diablo");
                if (hProc != NULL) {
                    if (CProc::Read(hProc, dwOffset, pBuffer, dwLen)) {
                        LPSTR pszData = BinToAscii(pBuffer, dwLen);
                        SetDlgItemText(hwnd, IDC_RWDM_DATA, pszData);
                        delete [] pszData;
                    }
                    CProc::Close(hProc);
                }
                delete [] pBuffer;
            }
            return TRUE;

        case IDOK:
        case IDCANCEL:
            EndDialog(hwnd, LOWORD(wParam));
            return TRUE;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return FALSE;
}

BOOL CALLBACK
WriteDiabloMemoryDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG: {
            SetWindowText(hwnd, "Write Diablo Memory");
            SetDlgItemText(hwnd, IDC_RWDM_DOIT, "&Write");
            HWND hwndEC = GetDlgItem(hwnd, IDC_RWDM_LEN);
            ASSERT(hwndEC != NULL);
            SendMessage(hwndEC, EM_SETREADONLY, TRUE, 0);
        }
        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDC_RWDM_DOIT: {
                if (g_dwDangerousCommandNotifyFlag &&
                    DialogBox(g_hInst, MAKEINTRESOURCE(IDD_YESNO),
                    hwnd, (DLGPROC)DangerousCommandDlgProc) == IDNO) {
                    return TRUE;
                }

                DWORD dwOffset;
                if (!GetDlgItemHex(hwnd, IDC_RWDM_OFFSET, "Offset", dwOffset)) {
                    return TRUE;
                }
                HWND hwndEC = GetDlgItem(hwnd, IDC_RWDM_DATA);
                ASSERT(hwndEC != NULL);
                int nLen = GetWindowTextLength(hwndEC);
                if (nLen == 0) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK, "Data not specified");
                    SetFocus(hwndEC);
                    return TRUE;
                } else if (nLen & 1) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK,
                        "Data must be a multiple of two");
                    SetFocus(hwndEC);
                    return TRUE;
                }

                LPSTR pszString = new char [nLen + 1];
                if (pszString != NULL) {
                    GetWindowText(hwndEC, pszString, nLen+1);
                    int nBinLen = nLen / 2;
                    SetDlgItemHex(hwnd, IDC_RWDM_LEN, nBinLen);
                    LPBYTE pBuffer = new BYTE [nBinLen];
                    if (pBuffer != NULL) {
                        if (ParseHexData(pszString, pBuffer, nBinLen) > 0) {
                            CProc::ReportMemIoErrors(TRUE);
                            HANDLE hProc = CProc::Open("Diablo");
                            if (hProc != NULL) {
                                CProc::Write(hProc, dwOffset, pBuffer, nBinLen);
                                CProc::Close(hProc);
                            }
                        }
                        delete [] pBuffer;
                    } else {
                        MsgBox(MB_ICONSTOP|MB_OK, "Memory allocation failure");
                    }
                    delete [] pszString;
                } else {
                    MsgBox(MB_ICONSTOP|MB_OK, "Memory allocation failure");
                }
            }
            return TRUE;

        case IDOK:
        case IDCANCEL:
            EndDialog(hwnd, LOWORD(wParam));
            return TRUE;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return FALSE;
}

BOOL CALLBACK
DisplayHackPropsDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG: {
            char szBuffer[16384];
            szBuffer[0] = '\0';
            ASSERT(g_pHackDisplayProps != NULL);
            g_pHackDisplayProps->dumpContents(szBuffer);
            SetWindowText(hwnd, "Hack Properties");
            SetDlgItemText(hwnd, IDC_CDF_INFO, szBuffer);
        }
        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            EndDialog(hwnd, LOWORD(wParam));
            return TRUE;

        default:
            break;
        }
        break;
    default:
        break;
    }

    return FALSE;
}

#ifdef _DEBUG
#define VERFMT "Version %s [Debug]"
#else
#define VERFMT "Version %s"
#endif

BOOL CALLBACK
AboutDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG: {
        // Read our file information
            char szFileName[MAX_PATH], szVersion[80];
            strcpy(szVersion, "Unknown version");
            VERIFY(GetModuleFileName(g_hInst, szFileName, MAX_PATH));
            DWORD dwHandle;
            DWORD dwVerSize = GetFileVersionInfoSize(szFileName, &dwHandle);
            LPBYTE pVer = new BYTE [dwVerSize];
            if (pVer != NULL) {
                if (GetFileVersionInfo(szFileName, dwHandle, dwVerSize, pVer)) {
                    LPVOID lpv;
                    UINT nLen;
                    VERIFY(VerQueryValue(pVer,
                        "\\StringFileInfo\\080904b0\\ProductVersion",
                        &lpv, &nLen));
                    sprintf(szVersion, VERFMT, (char *)lpv);
                }
                delete pVer;
            }
            SetDlgItemText(hwnd, IDC_AB_VERSION, szVersion);
        }
        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            EndDialog(hwnd, LOWORD(wParam));
            return TRUE;
        default:
            break;
        }
        break;
    default:
        break;
    }

    return FALSE;
}

BOOL CALLBACK
HelpCreatingDatfilesDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG:
        SetWindowText(hwnd, "Creating .DAT files");
        SetDlgItemText(hwnd, IDC_CDF_INFO,  g_szCreatingDatfiles);
        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            EndDialog(hwnd, LOWORD(wParam));
            return TRUE;

        default:
            break;
        }
        break;
    default:
        break;
    }

    return FALSE;
}

BOOL CALLBACK
HelpSofticeCodesDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG:
        SetWindowText(hwnd, "Entering or converting SoftICE codes");
        SetDlgItemText(hwnd, IDC_CDF_INFO,  g_szSofticeCodes);
        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            EndDialog(hwnd, LOWORD(wParam));
            return TRUE;

        default:
            break;
        }
        break;
    default:
        break;
    }

    return FALSE;
}

BOOL CALLBACK
DiabloNotRunningDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG:
        SetWindowText(hwnd, "Diablo is not running");
        SetDlgItemText(hwnd, IDC_REM_MSG, g_szDiabloNotRunningMsg);
        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            if (IsDlgButtonChecked(hwnd, IDC_REM_DONTCOMEBACK)
                == BST_CHECKED) {
                g_dwDiabloNotRunningNotifyFlag = FALSE;
            }
            EndDialog(hwnd, LOWORD(wParam));
            return TRUE;

        default:
            break;
        }
        break;
    default:
        break;
    }

    return FALSE;
}

BOOL CALLBACK
DangerousCommandDlgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg) {
    case WM_INITDIALOG:
        SetWindowText(hwnd, "Hey! This is a dangerous command");
        SetDlgItemText(hwnd, IDC_YESNO_MSG, g_szDangerousCommandMsg);
        return TRUE;

    case WM_COMMAND:
        switch(LOWORD(wParam)) {
        case IDYES:
        case IDNO:
            if (IsDlgButtonChecked(hwnd, IDC_YESNO_DONTBOTHERAGAIN)
                == BST_CHECKED) {
                g_dwDangerousCommandNotifyFlag = FALSE;
            }
            EndDialog(hwnd, LOWORD(wParam));
            return TRUE;

        default:
            break;
        }
        break;
    default:
        break;
    }

    return FALSE;
}
