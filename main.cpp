#include <windows.h>
#include <iostream>
#include <cmath>
#define PI 3.141592


int grid_size = 10; // размер ячейки
int grid_width = 100; // Ширина таблицы
int grid_height = 100; // Высота таблицы

void draw_grid(int w, int h, int size)
{
    HWND handleWindow;
    handleWindow = GetConsoleWindow();
    HDC handleDC;
    handleDC = GetDC(handleWindow);


    HPEN penRegular;
    penRegular = CreatePen(PS_SOLID, 1, RGB(20, 20, 20));
    HPEN penMain;
    penMain = CreatePen(PS_SOLID, 1, RGB(100, 100, 100));


    for (int i = 0; i <= h; i++)
    {
        SelectObject(handleDC, penRegular);
        if (i % 5 == 0) {
            SelectObject(handleDC, penMain);
        }
        MoveToEx(handleDC, 0, i * size, nullptr);
        LineTo(handleDC, w * size, i * size);
    }
    for (int i = 0; i <= w; i++)
    {
        SelectObject(handleDC, penRegular);
        if (i % 5 == 0) {
            SelectObject(handleDC, penMain);
        }
        MoveToEx(handleDC, i * size, 0, nullptr);
        LineTo(handleDC, i * size, h * size);
    }
}

void setPixel(int x, int y, int sizePixel = 15, COLORREF color = RGB(255, 0, 0))
{
    x = x - 1;
    y = grid_height - y;

    HWND handleWindow;
    handleWindow = GetConsoleWindow();
    HDC handleDC;
    handleDC = GetDC(handleWindow);
    HBRUSH brush;
    brush = CreateSolidBrush(color);
    SelectObject(handleDC, brush);
    HPEN hPen = CreatePen(PS_SOLID, 1, color);
    SelectObject(handleDC, hPen);
    Rectangle(handleDC, x * sizePixel, y * sizePixel, x * sizePixel + sizePixel, y * sizePixel + sizePixel);
    DeleteObject(brush);

}

void line_bresenham(int x1, int y1, int x2, int y2, COLORREF color = RGB(255, 0, 0)) {

    int step_x = 1;
    int step_y = 1;
    int pixel_size = grid_size;
    if (x1 > x2) {
        step_x = -1;
    }
    if (y1 > y2) {
        step_y = -1;
    }
    setPixel(x2, y2, pixel_size, color);
    if ((x1 == x2) & (y1 == y2)) {
        return;
    }



    int delta_x = abs(x2 - x1);
    int delta_y = abs(y2 - y1);
    int error = 0;
    if (delta_x > delta_y) {
        int delta_error = (delta_y + 1);
        for (int i = x1; step_x * x1 < step_x * x2; i += step_x) {
            setPixel(x1, y1, pixel_size, color);
            error = error + delta_error;
            if (error >= (delta_x + 1)) {
                y1 += step_y;
                error = error - (delta_x + 1);
                setPixel(x1, y1, pixel_size, color);
            }
            x1 += step_x;
        }
    }
    else {
        int delta_error = (delta_x + 1);
        for (int i = y1; step_y * y1 < step_y * y2; i += step_y) {
            setPixel(x1, y1, pixel_size, color);
            error = error + delta_error;
            if (error >= (delta_y + 1)) {
                x1 += step_x;
                error = error - (delta_y + 1);
                setPixel(x1, y1, pixel_size, color);
            }
            y1 += step_y;
        }
    }
}

void line_DDA(int x1, int y1, int x2, int y2, COLORREF color = RGB(255, 0, 0)) {
    int pixel_size = grid_size;
    int dx = x2 - x1;
    int dy = y2 - y1;
    int steps;
    if (abs(dx) > abs(dy)) {
        steps = abs(dx);
    }
    else {
        steps = abs(dy);
    }
    float stepX = dx / (float)steps;
    float stepY = dy / (float)steps;
    float X = x1;
    float Y = y1;
    for (int i = 0; i <= steps; i++)
    {
        setPixel(X, Y, pixel_size, color);
        X += stepX;
        Y += stepY;
    }
}

void circle_bresenham(int x1, int y1, int r, COLORREF color = RGB(255, 0, 0)) {
    int stepX = 0;
    int stepY = r;
    int delta = 1 - 2 * r;
    int error;
    int sizePixel = grid_size;
    while (stepY >= stepX) {
        setPixel(x1 + stepX, y1 + stepY, sizePixel, color);
        setPixel(x1 + stepX, y1 - stepY, sizePixel, color);
        setPixel(x1 - stepX, y1 + stepY, sizePixel, color);
        setPixel(x1 - stepX, y1 - stepY, sizePixel, color);
        setPixel(x1 + stepY, y1 + stepX, sizePixel, color);
        setPixel(x1 + stepY, y1 - stepX, sizePixel, color);
        setPixel(x1 - stepY, y1 + stepX, sizePixel, color);
        setPixel(x1 - stepY, y1 - stepX, sizePixel, color);
        error = 2 * (delta + stepY) - 1;
        if ((delta < 0) && (error <= 0)) {
            stepX = stepX + 1;
            delta = delta + (2 * stepX + 1);
        }
        else if ((delta > 0) && (error > 0)) {
            stepY = stepY - 1;
            delta -= 2 * stepY + 1;
        }
        else {
            stepX = stepX + 1;
            stepY = stepY - 1;
            delta = delta + 2 * (stepX - stepY);
        }
    }
}

static void circ_8(int ix0, int iy0, int x, int y, COLORREF col)
{
    int sizePixel = grid_size;
    setPixel(ix0, iy0, sizePixel, col);
    setPixel(ix0, iy0, sizePixel, col);
    setPixel(ix0, iy0, sizePixel, col);
    setPixel(ix0, iy0, sizePixel, col);
}

void circle(int ix0, int iy0, int r, COLORREF color = RGB(255, 0, 0))
{
    int x, y, d;

    x = 0; y = r; d = 3 - (r << 1);
    while (x < y)
    {
        circ_8(ix0, iy0, x, y, color);
        if (d < 0) d += (x << 2) + 6; else d += ((x - y) << 2) + 10, --y;
        x++;
    }
    if (x == y) circ_8(ix0, iy0, x, y, color);
}

void triangle(int Ax, int Ay, int Bx, int By, int Cx, int Cy)
{
    int tmp;
    int X[3] = { Ax,Bx,Cx };
    int Y[3] = { Ay,By,Cy };
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (Y[j] < Y[j + 1])
            {
                tmp = Y[j + 1];
                Y[j + 1] = Y[j];
                Y[j] = tmp;
                tmp = X[j + 1];
                X[j + 1] = X[j];
                X[j] = tmp;

            }
    Cy = Y[0]; By = Y[1]; Ay = Y[2];
    Cx = X[0]; Bx = X[1]; Ax = X[2];


    int x1, x2;
    for (int sy = Ay; sy <= Cy; sy++)
    {
        x1 = Ax + (sy - Ay) * (Cx - Ax) / (Cy - Ay);
        if (sy < By)
            x2 = Ax + (sy - Ay) * (Bx - Ax) / (By - Ay);
        else
        {
            if (Cy == By)
                x2 = Bx;
            else
                x2 = Bx + (sy - By) * (Cx - Bx) / (Cy - By);
        }
        if (x1 > x2)
        {

            tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        line_DDA(x1, sy + 1, x2, sy + 1, RGB(0, 0, 255));
    }
}



int main()
{
    int x = 0;
    int y = 5;
    int x1 = 15;
    int y1 = 10;
    int x2 = 50;
    int y2 = 50;
    HANDLE handleWindow = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handleWindow, { 0,static_cast<short>(grid_size * grid_height / 16) });
    std::cout << "Choose an option:" << '\n'
    << "1. Grid " <<'\n'
    <<"2. bresenham" << '\n'
    <<"3. DDA" << '\n'
    << "4. Circle out of lines" << '\n'
    << "5. Circle out of lines DDA" << '\n'
    << "6. Circle" << '\n'
    << "7. triangle" << '\n'
    << "8. Exit" << '\n';
    int ans = 0;
    std::cin >> ans;
        switch (ans)
        {
            default:{
                std::cout<<"Not an option"<<'\n';
                break;
            }

            case 1:{
                draw_grid(grid_width, grid_height, grid_size);
                break;
            }
            case 2:{
                line_bresenham(x, y, x1, y1);
                break;
            }
            case 3:{
                line_DDA(x + 10, y, x1 + 10, y1);
                break;
            }
            case 4:{
                for (int i = 0; i < 360; i = i + 10) {
                    line_bresenham(30, 50, (30 + 30 * cos(PI / 180 * i)), (50 + 30 * sin(PI / 180 * i)), RGB(255, i, i));
                }
                break;
            }
            case 5: {
                for (int i = 0; i < 360; i = i + 10) {
                    line_DDA(100, 50, (100 + 30 * cos(PI / 180 * i)), (50 + 30 * sin(PI / 180 * i)), RGB(i, 255, i));
                }
                break;
            }
            case 6:{
                circle_bresenham(170, 50, 28, RGB(0, 0, 255));
                circle(30, 10, 6, RGB(0, 0, 255));
                break;
            }
            case 7:{
                triangle(x, y, x1, y1, x2, y2);
                break;
            }
        }

        SetConsoleCursorPosition(handleWindow, { 0,static_cast<short>(grid_size * grid_height / 16 + 8) });
    std::cin.get();
    std::cin.get();
    return 0;
    }






