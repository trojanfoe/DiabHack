/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/main.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

char g_szClassName[] = "Diabhack_class";
char g_szTitle[] = "Diablo Hacking Utility";

// Window position
int g_nMainX = CW_USEDEFAULT, g_nMainY = CW_USEDEFAULT;
int g_nMainCX = CW_USEDEFAULT, g_nMainCY = CW_USEDEFAULT;
// ListView column widths
int g_nLV0CX = 200, g_nLV1CX = 100, g_nLV2CX = 100;
// Notify the user that Diablo is not running
DWORD g_dwDiabloNotRunningNotifyFlag = TRUE;
// Notify the user that a command is dangerous
DWORD g_dwDangerousCommandNotifyFlag = TRUE;

// Main window handle
HWND g_hwndMain = NULL;

// Instance handle
HINSTANCE g_hInst = NULL;

// List of hacks
CHackList g_HackList;

LRESULT CALLBACK MainWndProc(HWND hwnd, UINT uMsg,
    WPARAM wParam, LPARAM lParam);

int APIENTRY
WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR pszCmdLine, int nCmdShow)
{
    WNDCLASSEX wcx;
    HWND hwnd;
    MSG msg;

    g_hInst = hInst;

    // Test if we're already running
    HWND hwndOther = FindWindowEx(NULL, NULL, g_szClassName, NULL);
    if (hwndOther != NULL) {
        ShowWindow(hwndOther, SW_RESTORE);
        SetForegroundWindow(hwndOther);
        return 0;
    }

    wcx.cbSize = sizeof(wcx);
    wcx.style = CS_HREDRAW|CS_VREDRAW;
    wcx.lpfnWndProc = MainWndProc;
    wcx.cbClsExtra = 0;
    wcx.cbWndExtra = 0;
    wcx.hInstance = hInst;
    wcx.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(IDI_DIABHACK));
    wcx.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcx.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcx.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
    wcx.lpszClassName = g_szClassName;
    wcx.hIconSm = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_DIABHACK),
        IMAGE_ICON, 16, 16, 0);

    if (!RegisterClassEx(&wcx)) {
        TRACE("Failed to register window class: %s\n", GetLastErrorText());
        return 1;
    }

    LoadCurrentWindowPositions();
    LoadNotifyFlags();

    // Validate window position (Windows 95 sometimes gets a corrupt
    // Registry)
    int nScrX = GetSystemMetrics(SM_CXFULLSCREEN);
    int nScrY = GetSystemMetrics(SM_CYFULLSCREEN);
    if ((g_nMainX != CW_USEDEFAULT && g_nMainX > nScrX) ||
        (g_nMainX != CW_USEDEFAULT && g_nMainY > nScrY) ||
        (g_nMainCX != CW_USEDEFAULT && g_nMainCX > nScrX) ||
        (g_nMainCY != CW_USEDEFAULT && g_nMainCY > nScrY)) {
        g_nMainX = CW_USEDEFAULT;
        g_nMainY = CW_USEDEFAULT;
        g_nMainCX = CW_USEDEFAULT;
        g_nMainCY = CW_USEDEFAULT;
        g_nLV0CX = 200;
        g_nLV1CX = 100;
        g_nLV2CX = 100;
    }

    hwnd = CreateWindow(g_szClassName, g_szTitle, WS_OVERLAPPEDWINDOW,
        g_nMainX, g_nMainY, g_nMainCX, g_nMainCY, NULL, NULL, hInst, NULL);
    if (hwnd == NULL) {
        TRACE("Failed to create main window: %s\n", GetLastErrorText());
        return 2;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK
MainWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HWND hwndLV, hwndTB;
    static char szFileName[MAX_PATH];

    switch (uMsg) {
    case WM_CREATE: {
            memset(szFileName, 0, sizeof(szFileName));
            g_hwndMain = hwnd;

            // Create the toolbar
            hwndTB = CreateToolbar();
            if (hwndTB == NULL) {
                return -1;
            }

            // Create the listview, which covers the entire client area
            hwndLV = CreateListView();
            if (hwndLV == NULL) {
                return -1;
            }

            // If Diablo is not running then tell the user
            if (g_dwDiabloNotRunningNotifyFlag && !CHack::IsDiabloRunning()) {
                DialogBox(g_hInst, MAKEINTRESOURCE(IDD_REMINDER),
                    hwnd, (DLGPROC)DiabloNotRunningDlgProc);
            }
            EnableRemoveAllButton(hwndTB, FALSE);
        }
        return 0;

    case WM_SIZE: {
            // Pass the size message straight to the toolbar window
            SendMessage(hwndTB, WM_SIZE, wParam, lParam);
            // Calculate the toolbar height, in order to reposition the listview
            // correctly
            RECT r;
            GetWindowRect(hwndTB, &r);
            int cx = r.bottom - r.top;      // Height of Toolbar
            MoveWindow(hwndLV, 0, cx, LOWORD(lParam), HIWORD(lParam) - cx, TRUE);
        }
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_FILE_OPENDAT:
            if (GetFileName(szFileName, sizeof(szFileName),
                "DiabHack Data Files (*.dat)\0*.dat\0All Files (*.*)\0*.*\0",
                "dat", FALSE)) {
                LoadDatFile(szFileName);
                CProc::ReportMemIoErrors(TRUE);
                RefreshListView(hwndLV);
                EnableRemoveAllButton(hwndTB,
                    g_HackList.rootHack() != NULL ? TRUE : FALSE);
#ifdef _DEBUG
                g_HackList.dump();
#endif // _DEBUG
            }
            return 0;

        case IDM_FILE_SAVEDAT:
            if (g_HackList.rootHack() == NULL) {
                MsgBox(MB_ICONEXCLAMATION|MB_OK,
                    "There is no hack information loaded");
                return 0;
            }
            if (GetFileName(szFileName, sizeof(szFileName),
                "DiabHack Data Files (*.dat)\0*.dat\0All Files (*.*)\0*.*\0",
                "dat", TRUE)) {
                SaveDatFile(szFileName);
            }
            return 0;

        case IDM_FILE_EXIT:
            // Exit the program
            PostMessage(hwnd, WM_CLOSE, 0, 0);
            return 0;

        case IDM_HACK_ACTIVATE:
            ActivateHacks(hwndLV, FALSE);
#ifdef _DEBUG
            g_HackList.dump();
#endif // _DEBUG
            return 0;

        case IDM_HACK_ACTIVATEALL:
            ActivateHacks(hwndLV, TRUE);
#ifdef _DEBUG
            g_HackList.dump();
#endif // _DEBUG
            return 0;

        case IDM_HACK_DEACTIVATE:
            DeactivateHacks(hwndLV, FALSE);
#ifdef _DEBUG
            g_HackList.dump();
#endif // _DEBUG
            return 0;

        case IDM_HACK_DEACTIVATEALL:
            DeactivateHacks(hwndLV, TRUE);
#ifdef _DEBUG
            g_HackList.dump();
#endif // _DEBUG
            return 0;

        case IDM_HACK_REFRESHSTATUS:
            CProc::ReportMemIoErrors(TRUE);
            RefreshListView(hwndLV);
#ifdef _DEBUG
            g_HackList.dump();
#endif // _DEBUG
            return 0;

        case IDM_HACK_REMOVE:
            RemoveSelectedHacks(hwndLV);
#ifdef _DEBUG
            g_HackList.dump();
#endif // _DEBUG
            return 0;

        case IDM_HACK_REMOVEALL:
            g_HackList.removeAll();
            CProc::ReportMemIoErrors(TRUE);
            RefreshListView(hwndLV);
            EnableRemoveAllButton(hwndTB, FALSE);
            return 0;

        case IDM_HACK_PROPERTIES:
            DisplayHackProperties(hwndLV);
            return 0;

        case IDM_UTILITY_READDIABLOMEMORY:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_READWRITEDIABLOMEMORY),
                hwnd, (DLGPROC)ReadDiabloMemoryDlgProc);
            return 0;

        case IDM_UTILITY_WRITEDIABLOMEMORY:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_READWRITEDIABLOMEMORY),
                hwnd, (DLGPROC)WriteDiabloMemoryDlgProc);
            return 0;

        case IDM_HELP_HOWCREATEDAT:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_INFO),
                hwnd, (DLGPROC)HelpCreatingDatfilesDlgProc);
            return 0;

        case IDM_HELP_HOWSOFTICECODES:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_INFO),
                hwnd, (DLGPROC)HelpSofticeCodesDlgProc);
            return 0;

        case IDM_HELP_ABOUT:
            DialogBox(g_hInst, MAKEINTRESOURCE(IDD_ABOUTBOX),
                hwnd, (DLGPROC)AboutDlgProc);
            return 0;

        default:
            break;
        }
        break;

    case WM_NOTIFY: {
            if (((LPNMHDR)lParam)->hwndFrom == hwndLV) {
                // ListView notify
                switch (((LPNMHDR)lParam)->code) {
                case NM_DBLCLK:
                    DblClkListView(hwndLV);
                    break;
                case NM_RCLICK:
                    RClickListView(hwndLV);
                    break;
                case LVN_COLUMNCLICK:
                    ColumnClickListView(hwndLV, ((NM_LISTVIEW *)lParam)->iSubItem);
                    break;
                default:
                    break;
                }
            } else {
                // Tooltips
                LPTOOLTIPTEXT lpToolTipText = (LPTOOLTIPTEXT)lParam;
                if (lpToolTipText->hdr.code == TTN_NEEDTEXT) {
                    char szBuffer[64];
                    // String ID of text == command ID
                    LoadString(g_hInst, lpToolTipText->hdr.idFrom,
                        szBuffer, sizeof(szBuffer));
                    lpToolTipText->lpszText = szBuffer;
                }
            }
            return 0;
        }
        break;

    case WM_INITMENUPOPUP: {
        // Enable/disable menu items depending on whether Diablo is running
            int nItems = GetMenuItemCount((HMENU)wParam);
            const UINT nEnabFlags = MF_BYCOMMAND|MF_ENABLED;
            const UINT nDisabFlags = MF_BYCOMMAND|MF_GRAYED;
            UINT nFlags = CHack::IsDiabloRunning() ? nEnabFlags : nDisabFlags;
            for (int nPos = 0; nPos < nItems; nPos++) {
                UINT nID = GetMenuItemID((HMENU)wParam, nPos);
                switch (nID) {
                case IDM_HACK_ACTIVATE:
                case IDM_HACK_ACTIVATEALL:
                case IDM_HACK_DEACTIVATE:
                case IDM_HACK_DEACTIVATEALL:
                case IDM_HACK_REFRESHSTATUS:
                case IDM_UTILITY_READDIABLOMEMORY:
                case IDM_UTILITY_WRITEDIABLOMEMORY:
                    EnableMenuItem((HMENU)wParam, nID, nFlags);
                    break;
                case IDM_HACK_REMOVE:
                case IDM_HACK_REMOVEALL:
                    EnableMenuItem((HMENU)wParam, nID,
                        g_HackList.rootHack() != NULL ? nEnabFlags :
                        nDisabFlags);
                default:
                    break;
                }
            }
        }
        return 0;

    case WM_ACTIVATE:
        // Change the toolbar buttons whenever the application is
        // activated/deactivated
        EnableHackButtons(hwndTB, CHack::IsDiabloRunning());
        RefreshListView(hwndLV);
        return 0;
        
    case WM_DESTROY:
        SaveCurrentWindowPositions(hwnd, hwndLV);
        SaveNotifyFlags();
        g_hwndMain = NULL;
        PostQuitMessage(0);
        return 0;

    default:
        break;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

