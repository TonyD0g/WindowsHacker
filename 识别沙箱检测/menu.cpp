#include <iostream>
#include <Windows.h>
#include <fstream>
#include <string>

int main() {
	// 定义右键菜单项的注册表路径
	const char* regPath = "DesktopBackground\\Shell";
	
	// 打开注册表项
	HKEY hKey;
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, regPath, 0, KEY_READ, &hKey) != ERROR_SUCCESS) {
		std::cout << "无法打开注册表项" << std::endl;
		return 1;
	}
	
	// 获取子键数量
	DWORD numSubKeys = 0;
	if (RegQueryInfoKey(hKey, NULL, NULL, NULL, &numSubKeys, NULL, NULL, NULL, NULL, NULL, NULL, NULL) != ERROR_SUCCESS) {
		std::cout << "无法获取子键数量" << std::endl;
		RegCloseKey(hKey);
		return 1;
	}
	
	// 遍历子键并输出菜单项名称
	for (DWORD i = 0; i < numSubKeys; ++i) {
		char subKeyName[256];
		DWORD subKeyNameSize = sizeof(subKeyName);
		
		// 获取子键名称
		if (RegEnumKeyEx(hKey, i, subKeyName, &subKeyNameSize, NULL, NULL, NULL, NULL) == ERROR_SUCCESS) {
			std::cout << subKeyName << std::endl;
			std::string fileName = subKeyName;
			fileName += ".txt";
			std::ofstream outputFile(fileName);
			outputFile.close();
		}
	}
	
	// 关闭注册表项
	RegCloseKey(hKey);
	
	return 0;
}

