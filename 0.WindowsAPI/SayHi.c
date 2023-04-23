#include<windows.h>
/*
    青绿色的为类型名,蓝色的为变量名
    HINSTANCE:当前句柄
*/


int WINAPI  WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,PSTR szCmdLine,int iCmdShow)
{
    //MessageBox(句柄，内容，标题，操作);
    MessageBox(NULL,TEXT("Hello World!"),TEXT("This is My first WindowsAPI program"),MB_OK);
    return 0;
}
