#pragma once

#define PENDULUMDATA_OFFSET 0

void DrawPendulum(HWND hwnd, HDC hdc, PAINTSTRUCT* ps, const PendulumData* data);
void UpdatePendulum(HWND hwnd, PendulumData* data, INT interval);