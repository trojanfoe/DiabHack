/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/listview.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

static int CALLBACK CompareColumnFunc(LPARAM lParam1, LPARAM lParam2,
    LPARAM lParamSort);

// Imagelist icons
static UINT LVImages[] = {
    IDI_STATEUNKNOWN,
    IDI_STATEACTIVE,
    IDI_STATEINACTIVE,
    IDI_STATEERROR
};
const int nLVImages = sizeof(LVImages) / sizeof(LVImages[0]);

HWND
CreateListView()
{
    InitCommonControls();

    // Create the child list view
    HWND hwndLV = CreateWindowEx(WS_EX_CLIENTEDGE, WC_LISTVIEW, "",
        WS_CHILD|WS_VISIBLE|LVS_REPORT,
        0, 0, 0, 0, g_hwndMain, NULL, g_hInst, NULL);
    if (hwndLV == NULL) {
        TRACE("Failed to create listview child window: %s\n",
            GetLastErrorText());
        return NULL;
    }

    // Initialise the Image List for the list view
    HIMAGELIST hilLarge, hilSmall;
    hilLarge = ImageList_Create(32, 32, TRUE, nLVImages, 0);
    ASSERT(hilLarge != NULL);
    hilSmall = ImageList_Create(16, 16, TRUE, nLVImages, 0);
    ASSERT(hilSmall != NULL);
    for (int i = 0; i < nLVImages; i++) {
        HICON hIcon = LoadIcon(g_hInst, MAKEINTRESOURCE(LVImages[i]));
        ASSERT(hIcon != NULL);
        VERIFY(ImageList_AddIcon(hilLarge, hIcon) == i);
        VERIFY(ImageList_AddIcon(hilSmall, hIcon) == i);
        DeleteObject(hIcon);
    }
    ListView_SetImageList(hwndLV, hilLarge, LVSIL_NORMAL);
    ListView_SetImageList(hwndLV, hilSmall, LVSIL_SMALL);

    // Add the columns to the list view
    LV_COLUMN lvc;
    lvc.mask = LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT|LVCF_WIDTH;
    lvc.fmt = LVCFMT_LEFT;
    lvc.cx = g_nLV0CX;
    lvc.pszText = "Hack Description";
    lvc.iSubItem = 0;
    VERIFY(ListView_InsertColumn(hwndLV, 0, &lvc) == 0);
    lvc.cx = g_nLV1CX;
    lvc.pszText = "Diablo Version";
    lvc.iSubItem = 1;
    VERIFY(ListView_InsertColumn(hwndLV, 1, &lvc) == 1);
    lvc.cx = g_nLV2CX;
    lvc.pszText = "State";
    lvc.iSubItem = 2;
    VERIFY(ListView_InsertColumn(hwndLV, 2, &lvc) == 2);

    return hwndLV;
}

void
RefreshListView(HWND hwnd)
{
    CProc::ReportMemIoErrors(FALSE);
    ListView_DeleteAllItems(hwnd);
    CHack *pHack = g_HackList.rootHack();
    int nItem = 0;
    LV_ITEM lvi;
    lvi.mask = LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM|LVIF_STATE;
    lvi.iSubItem = 0;
    lvi.state = lvi.stateMask = 0;
    while (pHack != NULL) {
        lvi.iItem = nItem;
        lvi.pszText = (LPSTR)pHack->desc();
        int nImageIndex;
        LPCSTR pszState;
        if (CHack::IsDiabloRunning()) {
            pHack->getState();
        } else {
            pHack->state(CHack::STATE_UNKNOWN);
        }
        InterpretHackState(pHack, pszState, nImageIndex);
        lvi.iImage = nImageIndex;
        lvi.lParam = (LPARAM)pHack;
        nItem = ListView_InsertItem(hwnd, &lvi);
        ASSERT(nItem != -1);
        ListView_SetItemText(hwnd, nItem, 1, (LPSTR)pHack->version());
        ListView_SetItemText(hwnd, nItem, 2, (LPSTR)pszState);
        nItem++;
        pHack = pHack->next();
    }
    CProc::ReportMemIoErrors(TRUE);
}

void
DblClkListView(HWND hwnd)
{
    if (!CHack::IsDiabloRunning()) {
        return;
    }

    // Determine where the mouse was during the double-click
    LV_HITTESTINFO hti;
	DWORD dwPos = GetMessagePos();
    hti.pt.x = LOWORD(dwPos);
    hti.pt.y = HIWORD(dwPos);
    ScreenToClient(hwnd, &hti.pt);

    // See if an item was "hit" during the double-click
    LV_ITEM lvi;
    memset(&lvi, 0, sizeof(lvi));
    lvi.iItem = ListView_HitTest(hwnd, &hti);
    if (lvi.iItem != -1) {
        lvi.mask = LVIF_PARAM;
        VERIFY(ListView_GetItem(hwnd, &lvi));
        CHack *pHack = (CHack *)lvi.lParam;
        ASSERT(pHack != NULL);
        CProc::ReportMemIoErrors(FALSE);
        pHack->getState();
        CProc::ReportMemIoErrors(TRUE);
        if (pHack->state() == CHack::STATE_ACTIVE) {
            pHack->deactivateHack();
        } else if (pHack->state() == CHack::STATE_INACTIVE) {
            pHack->activateHack();
        }
        UpdateHackState(hwnd, pHack);
    }
}

void
RClickListView(HWND hwnd)
{
   // Determine where the mouse was during the double-click
    LV_HITTESTINFO hti;
	DWORD dwPos = GetMessagePos();
    POINT ptScr;
    ptScr.x = LOWORD(dwPos);
    ptScr.y = HIWORD(dwPos);
    hti.pt.x = LOWORD(dwPos);
    hti.pt.y = HIWORD(dwPos);
    ScreenToClient(hwnd, &hti.pt);

    // See if an item was "hit" during the double-click
    LV_ITEM lvi;
    memset(&lvi, 0, sizeof(lvi));
    lvi.iItem = ListView_HitTest(hwnd, &hti);
    if (lvi.iItem != -1) {
        lvi.mask = LVIF_PARAM;
        ListView_SetItemState(hwnd, lvi.iItem, LVIS_SELECTED, 0);
        VERIFY(ListView_GetItem(hwnd, &lvi));
        CHack *pHack = (CHack *)lvi.lParam;
        ASSERT(pHack != NULL);
        if (CHack::IsDiabloRunning()) {
            CProc::ReportMemIoErrors(FALSE);
            pHack->getState();
            CProc::ReportMemIoErrors(TRUE);
        } else {
            pHack->state(CHack::STATE_UNKNOWN);
        }

        HMENU hMenu = CreatePopupMenu();
        const UINT uEnabFlags = MF_STRING|MF_ENABLED;
        const UINT uDisabFlags = MF_STRING|MF_GRAYED;
        AppendMenu(hMenu, (pHack->state() == CHack::STATE_INACTIVE) ?
            uEnabFlags : uDisabFlags, IDM_HACK_ACTIVATE, "&Activate");
        AppendMenu(hMenu, (pHack->state() == CHack::STATE_ACTIVE) ? 
            uEnabFlags : uDisabFlags, IDM_HACK_DEACTIVATE, "&Deactivate");
        AppendMenu(hMenu, uEnabFlags, IDM_HACK_REMOVE, "&Remove...");
        AppendMenu(hMenu, MF_SEPARATOR, 0, (LPCSTR)NULL);
        AppendMenu(hMenu, uEnabFlags, IDM_HACK_PROPERTIES, "&Properties...");
        // The TPM_NONOTIFY flag is used so that the WM_INITPOPUPMENU
        // notification is not sent to the main window procedure (which
        // will either enable or disable all items in the popup menu
        // depending on whether Diablo is running or not, rather than
        // on an individual hack basis, as we have initialised above).
        TrackPopupMenu(hMenu,
            TPM_LEFTALIGN|TPM_TOPALIGN|TPM_RIGHTBUTTON|TPM_NONOTIFY,
            ptScr.x, ptScr.y, 0, g_hwndMain, NULL);
    }
}

void
ColumnClickListView(HWND hwnd, int nColumn)
{
    ListView_SortItems(hwnd, CompareColumnFunc, nColumn);
}

// Used by ColumnClickListView
static int CALLBACK
CompareColumnFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    ASSERT(lParam1 != NULL);
    ASSERT(lParam2 != NULL);
    ASSERT(lParamSort <= 3);
    switch (lParamSort) {
    case 0:
        // Hack description
        return strcmp(((CHack *)lParam1)->desc(), ((CHack *)lParam2)->desc());
    case 1:
        // Version
        return strcmp(((CHack *)lParam1)->version(), ((CHack *)lParam2)->version());
    case 2:
        // State
        return ((CHack *)lParam1)->state() - ((CHack *)lParam2)->state();
    default:
        break;
    }
    return 0;
}

void
ActivateHacks(HWND hwnd, BOOL fAll)
{
    CHack *pHack;
    if (fAll) {
        pHack = g_HackList.rootHack();
        while (pHack != NULL) {
            pHack->activateHack();
            UpdateHackState(hwnd, pHack);
            pHack = pHack->next();
        }
    } else {
        int nCurr = -1, nSelected = 0;
        while ((nCurr = ListView_GetNextItem(hwnd, nCurr,
            LVNI_SELECTED)) != -1) {
            nSelected++;
            LV_ITEM lvi;
            memset(&lvi, 0, sizeof(lvi));
            lvi.mask = LVIF_PARAM;
            lvi.iItem = nCurr;
            lvi.iSubItem = 0;
            VERIFY(ListView_GetItem(hwnd, &lvi));
            pHack = (CHack *)lvi.lParam;
            ASSERT(pHack != NULL);
            pHack->activateHack();
            UpdateHackState(hwnd, pHack);
        }
        if (nSelected == 0) {
            MsgBox(MB_ICONEXCLAMATION|MB_OK, "No hacks selected");
        }
    }
}

void
DeactivateHacks(HWND hwnd, BOOL fAll)
{
    CHack *pHack;
    if (fAll) {
        pHack = g_HackList.rootHack();
        while (pHack != NULL) {
            pHack->deactivateHack();
            UpdateHackState(hwnd, pHack);
            pHack = pHack->next();
        }
    } else {
        int nCurr = -1, nSelected = 0;
        while ((nCurr = ListView_GetNextItem(hwnd, nCurr,
            LVNI_SELECTED)) != -1) {
            nSelected++;
            LV_ITEM lvi;
            memset(&lvi, 0, sizeof(lvi));
            lvi.mask = LVIF_PARAM;
            lvi.iItem = nCurr;
            lvi.iSubItem = 0;
            VERIFY(ListView_GetItem(hwnd, &lvi));
            pHack = (CHack *)lvi.lParam;
            ASSERT(pHack != NULL);
            pHack->deactivateHack();
            UpdateHackState(hwnd, pHack);
        }
        if (nSelected == 0) {
            MsgBox(MB_ICONEXCLAMATION|MB_OK, "No hacks selected");
        }
    }
}
void
RemoveSelectedHacks(HWND hwnd)
{
    CHack *pHack;
    int nItem, nSelected = 0;
    while ((nItem = ListView_GetNextItem(hwnd, -1, LVNI_SELECTED)) != -1) {
        nSelected++;
        LV_ITEM lvi;
        memset(&lvi, 0, sizeof(lvi));
        lvi.mask = LVIF_PARAM;
        lvi.iItem = nItem;
        lvi.iSubItem = 0;
        VERIFY(ListView_GetItem(hwnd, &lvi));
        pHack = (CHack *)lvi.lParam;
        ASSERT(pHack != NULL);
        g_HackList.remove(pHack);
        ListView_DeleteItem(hwnd, nItem);
    }
    if (nSelected == 0) {
        MsgBox(MB_ICONEXCLAMATION|MB_OK, "No hacks selected");
    }
}


void UpdateHackState(HWND hwnd, CHack *pHack)
{
    LV_FINDINFO lvfi;
    LV_ITEM lvi;
    memset(&lvfi, 0, sizeof(lvfi));
    memset(&lvi, 0, sizeof(lvi));
    lvfi.flags = LVFI_PARAM;
    lvfi.lParam = (LPARAM)pHack;
    lvi.iItem = ListView_FindItem(hwnd, -1, &lvfi);
    if (lvi.iItem != -1) {
        lvi.mask = LVIF_IMAGE;
        LPCSTR pszState;
        InterpretHackState(pHack, pszState, lvi.iImage);
        VERIFY(ListView_SetItem(hwnd, &lvi));
        ListView_SetItemText(hwnd, lvi.iItem, 2, (LPSTR)pszState);
    }
}

void
DisplayHackProperties(HWND hwnd)
{
    LV_ITEM lvi;
    memset(&lvi, 0, sizeof(lvi));
    lvi.iItem = ListView_GetNextItem(hwnd, -1, LVNI_SELECTED);
    if (lvi.iItem != -1) {
        lvi.mask = LVIF_PARAM;
        VERIFY(ListView_GetItem(hwnd, &lvi));
        g_pHackDisplayProps = (CHack *)lvi.lParam;
        ASSERT(g_pHackDisplayProps != NULL);
        DialogBox(g_hInst, MAKEINTRESOURCE(IDD_INFO), g_hwndMain,
            (DLGPROC)DisplayHackPropsDlgProc);
    } else {
        MessageBeep(MB_ICONEXCLAMATION);
    }
}

void
InterpretHackState(CHack *pHack, LPCSTR &pszState, int &nImageIndex)
{
    if (!CHack::IsDiabloRunning()) {
        nImageIndex = IDX_STATEUNKNOWN;
        pszState = "Diablo not running";
        return;
    }

    switch (pHack->state()) {
    case CHack::STATE_UNKNOWN:
    default:
        nImageIndex = IDX_STATEUNKNOWN;
        pszState = "Unknown";
        break;
    case CHack::STATE_ACTIVE:
        nImageIndex = IDX_STATEACTIVE;
        pszState = "Active";
        break;
    case CHack::STATE_INACTIVE:
        nImageIndex = IDX_STATEINACTIVE;
        pszState = "Inactive";
        break;
    case CHack::STATE_ERROR:
        nImageIndex = IDX_STATEERROR;
        pszState = "Error";
        break;
    }
}
