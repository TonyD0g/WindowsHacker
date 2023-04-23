#include<windows.h>
#include<stdio.h>
//调用windowsAPI去删除文件
int main()
{
    int a,b;
    printf("If you input 1,you will delete file\n");
    scanf("%d", &b);
    char File_Path[]=".exe";
    a= DeleteFileA(File_Path);
    if(b==1)
    {   if(a==1)
            printf("This File has been Deleted!\n");
    }


}