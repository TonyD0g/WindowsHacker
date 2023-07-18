#include <windows.h>
#include <stdio.h>

// 让鼠标向左移动100个像素点,沙箱好像只能执行一次(即输出一次像素点文件)
void outputFile(const char *filename, const char *content) {
	FILE *file;
	
	// 打开文件，如果文件不存在则创建
	file = fopen(filename, "w");
	if (file == NULL) {
		printf("can't open %s！\n", filename);
		return;
	}
	
	// 写入内容到文件
	fprintf(file, "%s\n", content);
	
	// 关闭文件
	fclose(file);
	
	printf("success\n");
}

void moveMouseLeft(int pixels) {
	// 获取当前鼠标位置
	POINT currentPosition;
	GetCursorPos(&currentPosition);
	char str[20];
	sprintf(str, "%ld", currentPosition.x);
	outputFile(str,"helloworld");
	
	// 计算目标位置
	int targetX = currentPosition.x - pixels;
	int targetY = currentPosition.y;
	
	sprintf(str, "%d", targetX);
	outputFile(str,"helloworld");
	
	// 移动鼠标
	SetCursorPos(targetX, targetY);
}

int main() {
	// 将鼠标向左移动100个像素点
	moveMouseLeft(100);
	
	return 0;
}

