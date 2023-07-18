#include <windows.h>
#include <stdio.h>
// 微步沙箱的分辨率:	1440*900
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

int main() {
	// 获取屏幕分辨率
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	char str[20];
	sprintf(str, "%d", screenWidth);
	outputFile(str,"helloworld");
	
	sprintf(str, "%d", screenHeight);
	outputFile(str,"helloworld");
	
	printf("屏幕分辨率：%dx%d\n", screenWidth, screenHeight);
	
	return 0;
}

