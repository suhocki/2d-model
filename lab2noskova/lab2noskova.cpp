// lab2noskova.cpp: определяет точку входа для приложения.
//

#include "stdafx.h"
#include "lab2noskova.h"
#include "math.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
int pointsAmount;
int xMPos;
int yMPos;
int size = 8;
bool StartMouseTracking = false;
int bnClickAmount;
int scale;
bool catchMousePosForScale;
bool catchMousePosForAngle;
float fscale;

COLORREF color = RGB(0,0,180);
POINT *points;
POINT CoordForMoving;
POINT DifBetweenPoints;
POINT relativePoint;
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
HWND hWnd;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	GetVertex(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	GetPoint(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	GetScale(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	GetAngle(HWND, UINT, WPARAM, LPARAM);

void				onPaint(HWND hWnd);
void				DrawLine(HDC gHDC, int x1, int y1, int x2, int y2, int size, int color);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB2NOSKOVA, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB2NOSKOVA));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  НАЗНАЧЕНИЕ: регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB2NOSKOVA));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB2NOSKOVA);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   НАЗНАЧЕНИЕ: сохраняет обработку экземпляра и создает главное окно.
//
//   КОММЕНТАРИИ:
//
//        В данной функции дескриптор экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится на экран главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить дескриптор экземпляра в глобальной переменной

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  НАЗНАЧЕНИЕ:  обрабатывает сообщения в главном окне.
//
//  WM_COMMAND — обработать меню приложения
//  WM_PAINT — отрисовать главное окно
//  WM_DESTROY — отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
	case WM_CREATE:
		DialogBox(hInst, MAKEINTRESOURCE(IDD_LAB2NOSKOVA_DIALOG), hWnd, GetVertex);
		break;

	case WM_MOUSEMOVE:
		xMPos = LOWORD(lParam);
		yMPos = HIWORD(lParam);
		break;

	case WM_LBUTTONDOWN:
		
		if (pointsAmount > bnClickAmount) {
			points[bnClickAmount].x = xMPos;
			points[bnClickAmount].y = yMPos;
			bnClickAmount++;
			InvalidateRect(hWnd, 0, 0);
		}
		if (StartMouseTracking) {
			CoordForMoving.x = xMPos;
			CoordForMoving.y = yMPos;
			StartMouseTracking = false;
			
			DifBetweenPoints.x = CoordForMoving.x - points[0].x;
			DifBetweenPoints.y = CoordForMoving.y - points[0].y;
			for (int i = 0; i < pointsAmount; i++) {
				points[i].x += DifBetweenPoints.x;
				points[i].y += DifBetweenPoints.y;
			}
			DifBetweenPoints.x = 0;
			DifBetweenPoints.y = 0;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
		}
		if (catchMousePosForScale) {
			catchMousePosForScale = false;
			relativePoint.x = xMPos;
			relativePoint.y = yMPos;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG3), hWnd, GetScale);
			
		}
		if (catchMousePosForAngle) {
			catchMousePosForAngle = false;
			relativePoint.x = xMPos;
			relativePoint.y = yMPos;
			DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG4), hWnd, GetAngle);

		}
		break;

    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_MOUSE_CHOOSE:
				StartMouseTracking = true;
                break;
			case IDM_ENTER_COORD:
				DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG2), hWnd, GetPoint);
				break;
			case ID_SET_SCALE:
				MessageBox(NULL, "Выберите точку, относительно которой будет производится масштабирование.", "Ок", MB_OK);
				catchMousePosForScale = true;
				break;
			case ID_SET_ANGLE:
				MessageBox(NULL, "Выберите точку, относительно которой будет производится поворачивание.", "Ок", MB_OK);
				catchMousePosForAngle = true;
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
			onPaint(hWnd);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

INT_PTR CALLBACK GetPoint(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			TCHAR s[255];
			GetDlgItemText(hDlg, IDC_EDIT1, s, 9);
			CoordForMoving.x = _ttoi(s);
			GetDlgItemText(hDlg, IDC_EDIT2, s, 9);
			CoordForMoving.y = _ttoi(s);

			DifBetweenPoints.x = CoordForMoving.x - points[0].x;
			DifBetweenPoints.y = CoordForMoving.y - points[0].y;
			for (int i = 0; i < pointsAmount; i++) {
				points[i].x += DifBetweenPoints.x;
				points[i].y += DifBetweenPoints.y;
			}
			DifBetweenPoints.x = 0;
			DifBetweenPoints.y = 0;
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);

			EndDialog(hDlg, LOWORD(wParam));

			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK GetVertex(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			TCHAR s[255];
			GetDlgItemText(hDlg, IDC_EDIT1, s, 9);
			pointsAmount = _ttoi(s);
			points = new POINT[pointsAmount];
			EndDialog(hDlg, LOWORD(wParam));

			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK GetScale(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			TCHAR s[255];
			GetDlgItemText(hDlg, IDC_EDIT2, s, 9);
			fscale = _ttof(s);
			EndDialog(hDlg, LOWORD(wParam));
			for (int i = 0; i < pointsAmount; i++) {
				points[i].x = points[i].x * fscale + relativePoint.x * (1 - fscale);
				points[i].y = points[i].y * fscale + relativePoint.y * (1 - fscale);
			}
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK GetAngle(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK)
		{
			TCHAR s[255];
			GetDlgItemText(hDlg, IDC_EDIT3, s, 9);
			fscale = _ttof(s) * 3.142/180;
			EndDialog(hDlg, LOWORD(wParam));
			for (int i = 0; i < pointsAmount; i++) {
				points[i].x = relativePoint.x * (1 - cos(fscale)) + points[i].x * cos(fscale) - points[i].y * sin(fscale) + relativePoint.y*sin(fscale);
				points[i].y = relativePoint.y * (1 - cos(fscale)) + points[i].y * cos(fscale) + points[i].x * sin(fscale) - relativePoint.x*sin(fscale);
			}
			InvalidateRect(hWnd, NULL, TRUE);
			UpdateWindow(hWnd);
			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}

void DrawLine(HDC gHDC, int x1, int y1, int x2, int y2, int size, int color)
{
	static HPEN hpen = NULL;
	static HPEN oldPen = NULL;

	hpen = CreatePen(PS_SOLID, size, color);
	oldPen = (HPEN)SelectObject(gHDC, hpen);
	MoveToEx(gHDC, x1, y1, NULL);
	LineTo(gHDC, x2, y2);
	SelectObject(gHDC, oldPen);
	DeleteObject(hpen);
}

void onPaint(HWND hWnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: Добавьте сюда любой код прорисовки, использующий HDC...
	if (pointsAmount == bnClickAmount) {
		for (int i = 0; i < pointsAmount - 1; i++)
			DrawLine(hdc,
				points[i].x,
				points[i].y,
				points[i + 1].x,
				points[i + 1].y,
				size,
				color);

		DrawLine(hdc,
			points[pointsAmount - 1].x,
			points[pointsAmount - 1].y,
			points[0].x,
			points[0].y,
			size,
			color);

	}
	EndPaint(hWnd, &ps);
}
