#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 弱特征,查看测试模式是否开启了
int main() {
    FILE* pipe;
    char buffer[128];
    char outcome[128];
    char* command = "bcdedit /enum";  // 替换为你要执行的实际命令

    // 执行命令并读取输出
    pipe = popen(command, "r");
    if (pipe == NULL) {
        printf("can't exec\n");
        return 1;
    }

    // 逐行读取输出并打印
    while (fgets(buffer, sizeof(buffer), pipe) != NULL) {
        if(strstr(buffer, "flightsigning") != NULL){
            printf("%s", buffer);
            strcpy(outcome, buffer);
            break;
        }
    }

    FILE* file;
    if(strstr(outcome,"No")!=NULL){
        char* filename = "NO.txt";  // 替换为你想要输出的文件名
        file = fopen(filename, "w");
    }else  {
        char* filename = "YES.txt";  // 替换为你想要输出的文件名
        file = fopen(filename, "w");
    }
    fprintf(file, "Hello, World!\n");
    fclose(file);
    pclose(pipe);

    return 0;
}
