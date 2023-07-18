#include <windows.h>
#include <stdio.h>
// 点击windows桌面上最右下的日期
void moveMouseToBottomRight() {
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	// 计算目标位置
	int targetX = screenWidth - 20;
	int targetY = screenHeight - 1;
	
	// 移动鼠标
	SetCursorPos(targetX, targetY);
	
	// 模拟鼠标左键点击
	mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
	mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}

int main() {
	moveMouseToBottomRight();
	printf("helloworld");
	return 0;
}

