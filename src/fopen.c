#include "fopen.h"
#include <Windows.h>

int OpenFileDialog(char *filePath, int maxPathLength) {
    OPENFILENAMEA ofn;
    char szFile[MAX_PATH] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "PNG Files\0*.png\0All Files\0*.*\0";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    ofn.lpstrDefExt = "png";

    if (GetOpenFileNameA(&ofn)) {
        strncpy(filePath, szFile, maxPathLength - 1);
        filePath[maxPathLength - 1] = '\0';
        return 1;
    }
    return 0;
}

int SaveFileDialog(char *filePath, int maxPathLength) {
    OPENFILENAMEA ofn;
    char szFile[MAX_PATH] = {0};

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFilter = "PNG Files\0*.png\0All Files\0*.*\0";
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = MAX_PATH;
    ofn.Flags = OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = "png";

    if (GetSaveFileNameA(&ofn)) {
        strncpy(filePath, szFile, maxPathLength - 1);
        filePath[maxPathLength - 1] = '\0';
        return 1;
    }
    return 0;
}
