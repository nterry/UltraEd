#pragma once

#include <commdlg.h>
#include <stdio.h>
#include <string>
#include "../Scene.h"
#include "../resource.h"

using namespace std;

namespace UltraEd
{
    HBRUSH settingsBrush;

    COLORREF GetSceneSettingsRGBValue(HWND hWnd)
    {
        char buffer[4];

        int red = 0;
        GetDlgItemText(hWnd, IDC_EDIT_SCENE_COLOR_RED, buffer, 4);
        red = atoi(buffer);

        int green = 0;
        GetDlgItemText(hWnd, IDC_EDIT_SCENE_COLOR_GREEN, buffer, 4);
        green = atoi(buffer);

        int blue = 0;
        GetDlgItemText(hWnd, IDC_EDIT_SCENE_COLOR_BLUE, buffer, 4);
        blue = atoi(buffer);

        return RGB(red, green, blue);
    }

    void SetSceneSettingsRGBValue(HWND hWnd, COLORREF color)
    {
        char buffer[4];

        sprintf(buffer, "%i", GetRValue(color));
        SetDlgItemText(hWnd, IDC_EDIT_SCENE_COLOR_RED, buffer);

        sprintf(buffer, "%i", GetGValue(color));
        SetDlgItemText(hWnd, IDC_EDIT_SCENE_COLOR_GREEN, buffer);

        sprintf(buffer, "%i", GetBValue(color));
        SetDlgItemText(hWnd, IDC_EDIT_SCENE_COLOR_BLUE, buffer);
    }

    BOOL CALLBACK SceneSettingsProc(HWND hWndDlg, UINT message, WPARAM wParam, LPARAM lParam)
    {
        switch (message)
        {
            case WM_INITDIALOG:
            {
                CScene *scene = (CScene *)lParam;
                SetWindowLongPtr(hWndDlg, GWLP_USERDATA, (LPARAM)scene);
                if (scene)
                {
                    SetSceneSettingsRGBValue(hWndDlg, scene->GetBackgroundColor());
                }

                // Limit RGB fields to 3 characters max.
                SendDlgItemMessage(hWndDlg, IDC_EDIT_SCENE_COLOR_RED, EM_SETLIMITTEXT, 3, 0);
                SendDlgItemMessage(hWndDlg, IDC_EDIT_SCENE_COLOR_GREEN, EM_SETLIMITTEXT, 3, 0);
                SendDlgItemMessage(hWndDlg, IDC_EDIT_SCENE_COLOR_BLUE, EM_SETLIMITTEXT, 3, 0);
                break;
            }
            case WM_CTLCOLORSTATIC:
            {
                if ((HWND)lParam == GetDlgItem(hWndDlg, IDC_EDIT_SCENE_COLOR_PREVIEW)
                    && settingsBrush)
                {
                    return (INT_PTR)settingsBrush;
                }
                break;
            }
            case WM_COMMAND:
                switch (LOWORD(wParam))
                {
                    case IDC_EDIT_SCENE_COLOR_RED:
                    case IDC_EDIT_SCENE_COLOR_GREEN:
                    case IDC_EDIT_SCENE_COLOR_BLUE:
                    {
                        // Remove non-numerical characters and limit color value < 256.
                        if (HIWORD(wParam) == EN_CHANGE)
                        {
                            char buffer[4];
                            GetDlgItemText(hWndDlg, LOWORD(wParam), buffer, 4);

                            int val = atoi(buffer);
                            string cleanedValue = to_string(val);
                            if (val > 255) cleanedValue = "255";

                            // Stop sending update message when strings finally match.
                            if (strncmp(cleanedValue.c_str(), buffer, 3) != 0)
                                SetDlgItemText(hWndDlg, LOWORD(wParam), cleanedValue.c_str());

                            DeleteObject(settingsBrush);
                            settingsBrush = CreateSolidBrush(GetSceneSettingsRGBValue(hWndDlg));
                            InvalidateRect(GetDlgItem(hWndDlg, IDC_EDIT_SCENE_COLOR_PREVIEW), 0, 0);
                        }
                        break;
                    }
                    case IDC_BUTTON_SCENE_COLOR_CHOOSE:
                    {
                        COLORREF rgbCurrent = GetSceneSettingsRGBValue(hWndDlg);
                        COLORREF acrCustClr[16];
                        CHOOSECOLOR cc;
                        ZeroMemory(&cc, sizeof(cc));
                        cc.lStructSize = sizeof(cc);
                        cc.hwndOwner = hWndDlg;
                        cc.lpCustColors = (LPDWORD)acrCustClr;
                        cc.rgbResult = rgbCurrent;
                        cc.Flags = CC_FULLOPEN | CC_RGBINIT;

                        if (ChooseColor(&cc) == TRUE)
                        {
                            SetSceneSettingsRGBValue(hWndDlg, cc.rgbResult);
                        }
                        break;
                    }
                    case IDOK:
                    {
                        CScene *scene = (CScene *)GetWindowLongPtr(hWndDlg, GWLP_USERDATA);
                        if (scene) scene->SetBackgroundColor(GetSceneSettingsRGBValue(hWndDlg));
                    }
                    case IDCANCEL:
                    {
                        DeleteObject(settingsBrush);
                        EndDialog(hWndDlg, wParam);
                        return TRUE;
                    }
                }
        }
        return FALSE;
    }
}