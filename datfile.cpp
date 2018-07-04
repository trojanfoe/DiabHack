/*
 * Diablo Hacking Utility
 *
 * Copyright (C)1997 Trojan Consulting Ltd.
 *
 * andy@trojanco.demon.co.uk
 *
 * $Header: /diabhack/datfile.cpp 1     2/03/99 21:21 Andy $
 */

#include "diabhack.h"

BOOL
LoadDatFile(LPCSTR pszFileName)
{
    FILE *fp;
    BOOL fReturn = FALSE;

    fp = fopen(pszFileName, "rt");
    if (fp != NULL) {
        char szLine[256];
        const int nMaxWords = 20;
        LPSTR pszWord[nMaxWords];
        CHack *pHack = NULL;
        unsigned uLine = 0;
        while (fgets(szLine, sizeof(szLine), fp) != NULL) {
            uLine++;
            int nLen = strlen(szLine);
            if (nLen == 0 || szLine[0] == '#' || szLine[0] == ';') {
                continue;
            }
            // Remove trailing spaces and newline...
            while (nLen > 0 && isspace(szLine[nLen-1])) {
                szLine[nLen-1] = '\0';
                nLen--;
            }
            int nWords = ParseLine(szLine, pszWord, nMaxWords);
            if (nWords == 0) {
                continue;
            }
            if (stricmp(pszWord[0], "start") == 0) {
                if (pHack != NULL) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK,
                        "%s(%u):\nExpected \"end\" keyword",
                        pszFileName, uLine);
                    break;
                }
                if (nWords < 3) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK,
                        "%s(%u):\n\"start\" keyword requires version and description",
                        pszFileName, uLine);
                    break;
                }
                // Undo what ParseLine did to the string, by removing any null between
                // after the 3rd word
                for (LPSTR psz = pszWord[2]; psz < szLine + nLen; psz++) {
                    if (*psz == '\0') {
                        *psz = ' ';
                    }
                }
                szLine[nLen] = '\0';

                // Check that the hack is unique (in terms of it's description)
                if (g_HackList.find(pszWord[2]) != NULL) {
                    if (MsgBox(MB_ICONEXCLAMATION|MB_OKCANCEL,
                        "%s(%u):\nHack \"%s\" is already loaded.\n"
                        "Press Cancel to abort loading file",
                        pszFileName, uLine, pszWord[2]) == IDOK &&
                        SkipToNextHack(fp, uLine)) {
                        continue;
                    }
                    break;
                }
                pHack = new CHack(pszWord[2], pszWord[1]);
                if (pHack == NULL) {
                    MsgBox(MB_ICONSTOP|MB_OK, "Memory allocation failure");
                    break;
                }
            } else if (stricmp(pszWord[0], "end") == 0) {
                if (pHack == NULL) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK,
                        "%s(%u):\n\"end\" keyword not expected; ignoring line",
                        pszFileName, uLine);
                    continue;
                }
                if (nWords != 1) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK,
                        "%s(%u):\nExpected \"start\" keyword; ignoring line",
                        pszFileName, uLine);
                    continue;
                }
                g_HackList.add(pHack);
                pHack = NULL;
            } else {
                // Hack
                if (nWords != 3) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK,
                        "%s(%u):\nExpected: <offset> <olddata> <newdata>; ignoring line",
                        pszFileName, uLine);
                    continue;
                }
                DWORD dwOffset;
                if (!ParseHexValue(pszWord[0], dwOffset)) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK,
                        "%s(%u):\nInvalid <offset> value \"%s\"",
                        pszFileName, uLine, pszWord[0]);
                    break;
                }
                if (strlen(pszWord[1]) != strlen(pszWord[2])) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK,
                        "%s(%u):\n<olddata> and <newdata> lengths are different (%u and %u)",
                        pszFileName, uLine, strlen(pszWord[1]), strlen(pszWord[2]));
                    break;
                }
                if (strlen(pszWord[1]) & 1) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK,
                        "%s(%u):\n<nolddata> and <newdata> lengths must be a multiple of two",
                        pszFileName, uLine);
                    break;
                }
                BYTE bOldData[1024], bNewData[1024];
                DWORD dwOldLen, dwNewLen;
                dwOldLen = ParseHexData(pszWord[1], bOldData, sizeof(bOldData));
                if (dwOldLen == 0) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK,
                        "%s(%u):\nInvalid <olddata> values",
                        pszFileName, uLine);
                    break;
                }
                dwNewLen = ParseHexData(pszWord[2], bNewData, sizeof(bNewData));
                if (dwNewLen == 0) {
                    MsgBox(MB_ICONEXCLAMATION|MB_OK,
                        "%s(%u):\nInvalid <newdata> values",
                        pszFileName, uLine);
                    break;
                }
                ASSERT(dwOldLen == dwNewLen);

                // Check that the offset/length pair are unique
                CHack *pExist = g_HackList.find(dwOffset, dwOldLen);
                if (pExist) {
                    if (MsgBox(MB_ICONEXCLAMATION|MB_OKCANCEL,
                        "%s(%u):\nThe hack \"%s\" already contains modifications which\n"
                        "cover offset 0x%lx, length 0x%lx.\n"
                        "Press Cancel to abort loading file",
                        pszFileName, uLine, pExist->desc(), dwOffset,
                        dwOldLen) == IDOK &&
                        SkipToNextHack(fp, uLine)) {
                        continue;
                    }
                    break;
                }

                CHackNode *pNode = new CHackNode(dwOffset, dwOldLen, bOldData, bNewData);
                if (pNode == NULL) {
                    MsgBox(MB_ICONSTOP|MB_OK, "Memory allocation failure");
                    break;
                }
                pHack->addNode(pNode);
            }
        }
        if (pHack != NULL) {
            delete pHack;
        }
        fclose(fp);
    } else {
        // fopen failed
        MsgBox(MB_ICONSTOP|MB_OK, "Unable to open file \"%s\":\n%s",
            pszFileName, strerror(NULL));
    }

    return fReturn;
}

BOOL
SaveDatFile(LPCSTR pszFileName)
{
    FILE *fp;
    BOOL fReturn = FALSE;

    fp = fopen(pszFileName, "wt");
    if (fp != NULL) {
        CHack *pHack = g_HackList.rootHack();
        while (pHack != NULL) {
            fprintf(fp, "start %s %s\n", pHack->version(), pHack->desc());
            CHackNode *pNode = pHack->rootNode();
            while (pNode != NULL) {
                LPSTR pszOldData = BinToAscii(pNode->oldData(), pNode->len());
                LPSTR pszNewData = BinToAscii(pNode->newData(), pNode->len());
                fprintf(fp, "%lx %s %s\n", pNode->offset(), pszOldData, pszNewData);
                delete [] pszOldData;
                delete [] pszNewData;
                pNode = pNode->next();
            }
            fprintf(fp, "end\n\n");

            pHack = pHack->next();
        }

        fclose(fp);
        fReturn = TRUE;
    } else {
        // fopen failed
        MsgBox(MB_ICONSTOP|MB_OK, "Unable to open file \"%s\":\n%s",
            pszFileName, strerror(NULL));
    }

    return fReturn;
}

BOOL
SkipToNextHack(FILE *fp, UINT &uLine)
{
    char szLine[256];
    while (fgets(szLine, sizeof(szLine), fp) != NULL) {
        uLine++;
        if (strlen(szLine) >= 3 && strnicmp(szLine, "end", 3) == 0 &&
            (isspace(szLine[3]) || szLine[3] == '\0')) {
            return TRUE;
        } else if (strlen(szLine) >= 5 && strnicmp(szLine, "start", 5) == 0 &&
            (isspace(szLine[5]) || szLine[5] == '\0')) {
            // Put the start line back; as it is needed by the parser
            for (int i = strlen(szLine) - 1; i >= 0; i--) {
                ungetc(szLine[i], fp);
            }
            uLine--;
            return TRUE;
        }
    }
    return FALSE;
}

int
ParseLine(LPSTR pszLine, LPSTR pszWord[], int nMaxWords)
{
    int nWords = 0;

    while (*pszLine != '\0' && nWords < nMaxWords) {
        while (isspace(*pszLine) && *pszLine != '\0') {
            pszLine++;
        }
        if (*pszLine == '\0') {
            break;
        }
        pszWord[nWords++] = pszLine;
        while (!isspace(*pszLine) && *pszLine != '\0') {
            pszLine++;
        }
        if (*pszLine == '\0') {
            break;
        }
        *pszLine++ = '\0';
    }
    pszWord[nWords] = NULL;
    return nWords;
}

