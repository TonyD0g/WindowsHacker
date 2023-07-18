#include <windows.h>
#include <stdio.h>

// 微步沙箱执行后会直接生成error文件,或者直接不生成
void outputFile(const char *filename, const char *content) {
	FILE *file;
	
	// 打开文件，如果文件不存在则创建
	file = fopen(filename, "w");
	if (file == NULL) {
		printf("can't open %s\n", filename);
		return;
	}
	
	// 写入内容到文件
	fprintf(file, "%s\n", content);
	
	// 关闭文件
	fclose(file);
	
	printf("success\n");
}

int main() {
	DWORD drivesMask = GetLogicalDrives();
	int flag = 0;
	
	printf("Windows系统中的盘符和大小：\n");
	
	for (char drive = 'A'; drive <= 'Z'; drive++) {
		if (drivesMask & 1) {
			char rootPath[4] = { drive, ':', '\\', '\0' };
			ULARGE_INTEGER freeBytesAvailable, totalBytes, totalFreeBytes;
			
			if (GetDiskFreeSpaceEx(rootPath, &freeBytesAvailable, &totalBytes, &totalFreeBytes)) {
				flag = 1;
				char str[20];
				sprintf(str, "%llu", totalBytes.QuadPart / (1024 * 1024));
				
				printf(" %s size:	%llu MB\n", rootPath, totalBytes.QuadPart / (1024 * 1024));
				
				outputFile(str,"helloworld");
			}
		}
		
		drivesMask >>= 1;
	}
	if(flag != 1){
		outputFile("error","helloworld");
	}
	return 0;
}

