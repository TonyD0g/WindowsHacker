#include <windows.h>
#include <stdio.h>
#include <string.h>

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
	
	//printf("success\n");
}

int main() {
	DWORD dwMajorVersion, dwMinorVersion, dwBuildNumber;
	//TCHAR szCSDVersion[128];
	
	dwMajorVersion = LOBYTE(LOWORD(GetVersion()));
	dwMinorVersion = HIBYTE(LOWORD(GetVersion()));
	dwBuildNumber = 0;
	
	if (dwMajorVersion >= 6) {
		ULONGLONG ullConditionMask = VerSetConditionMask(
			VerSetConditionMask(
				VerSetConditionMask(
					0, VER_MAJORVERSION, VER_GREATER_EQUAL),
				VER_MINORVERSION, VER_GREATER_EQUAL),
			VER_SERVICEPACKMAJOR, VER_GREATER_EQUAL);
		
		if (VerifyVersionInfo(&(OSVERSIONINFOEX) { sizeof(OSVERSIONINFOEX), dwMajorVersion, dwMinorVersion, 0, 0, {0}, 0, 0, 0, 0, 0 }, VER_MAJORVERSION | VER_MINORVERSION | VER_SERVICEPACKMAJOR, ullConditionMask)) {
			DWORD version1 =  GetVersion();
			dwBuildNumber = (DWORD)(*(WORD*)&(version1));
		}
	}
	
	if (GetVersionEx((LPOSVERSIONINFO) & (OSVERSIONINFO) { sizeof(OSVERSIONINFO) })) {
		if (dwBuildNumber > 0) {
			char str[20];
			sprintf(str, "%ld", dwBuildNumber);
			outputFile(str,"helloworld");
			printf("Windows: Build %ld\n", dwBuildNumber);
		} else {
			char str[50];
			char str2[20];
			sprintf(str, "%ld", dwMajorVersion);
			sprintf(str2, "%ld", dwMinorVersion);
			strcat(str, str2);
			outputFile(str,"helloworld");
			printf("Windows: %ld.%ld\n", dwMajorVersion, dwMinorVersion);
		}
	} else {
		outputFile("error.txt","helloworld");
		printf("can't\n");
	}
	
	return 0;
}

