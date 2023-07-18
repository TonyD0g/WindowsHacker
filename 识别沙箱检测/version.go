package main

import (
	"fmt"
	"io/ioutil"
	"syscall"
	"unsafe"
)

type rtlOsVersionInfoEx struct {
	dwOSVersionInfoSize uint32
	dwMajorVersion      uint32
	dwMinorVersion      uint32
	dwBuildNumber       uint32
	dwPlatformId        uint32
	szCSDVersion        [128]uint16
	wServicePackMajor   uint16
	wServicePackMinor   uint16
	wSuiteMask          uint16
	wProductType        byte
	wReserved           byte
}

func outputFile(filename string, content string) error {
	err := ioutil.WriteFile(filename, []byte(content), 0644)
	if err != nil {
		return err
	}
	return nil
}

func main() {
	var osvi rtlOsVersionInfoEx
	osvi.dwOSVersionInfoSize = uint32(unsafe.Sizeof(osvi))

	// 调用 GetVersionExW 函数
	kernel32 := syscall.NewLazyDLL("kernel32.dll")
	getVersionEx := kernel32.NewProc("GetVersionExW")
	ret, _, _ := getVersionEx.Call(uintptr(unsafe.Pointer(&osvi)))

	if ret == 0 {
		fmt.Println("无法获取Windows系统编译信息。")
		filename := "error.txt"
		content := "这是一个示例文本。\nHello, World!"

		err := outputFile(filename, content)
		if err != nil {
			fmt.Printf("无法输出文件：%s\n", err.Error())
			return
		}
		return
	}

	filename := fmt.Sprintf("%d_output.txt", osvi.dwBuildNumber)
	content := syscall.UTF16ToString(osvi.szCSDVersion[:])

	err := outputFile(filename, content)
	if err != nil {
		fmt.Printf("无法输出文件：%s\n", err.Error())
		return
	}

	fmt.Printf("Windows系统编译信息: Build %d, %s\n", osvi.dwBuildNumber, syscall.UTF16ToString(osvi.szCSDVersion[:]))
}
