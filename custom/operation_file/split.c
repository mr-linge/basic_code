#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
//定义每次获取的文件大小
#define READSIZE 1024
 
int main(){
    printf("------大文件分割------\n\n");
    //定义写入文件的路径变量
    char filePath[255];
    //定义读写文件指针
    FILE *in, *out=nullptr;
    printf("请输入要分割的文件完整路径:");
    scanf("%s", &filePath);
    in = fopen(filePath, "r");
    if(in == nullptr){
        printf("打开源文件失败\n");
        exit(0);
    }
    printf("请输入要分割的文件大小:(单位：G/M/K)");
    char splitSize[50]; //用户输入的字符
    char newFileSize[50] = "";  //存放处理后的数字字符
    unsigned int newFileSizeNum;  //转换字符为数字
    char unit;
    scanf("%s", &splitSize);
    //判断有效性及单位
    bool err = false;
    for(unsigned int i=0; i<strlen(splitSize); i++){
        //前几位必须是数字
        if(i < (strlen(splitSize)-1)){
            //判断是否为数字
            if(splitSize[i] >= '0' && splitSize[i] <= '9'){
                newFileSize[i] = splitSize[i];
            }else{
                err = true;
                break;
            }
        }else{
            //判断单位是否有效
            if(splitSize[i]=='G' || splitSize[i]=='M' || splitSize[i]=='K'){
                unit = splitSize[i];
                break;
            }else{
                err = true;
                break;
            }
        }
    }
    if(err == true){
        printf("输入的格式错误");
        exit(0);
    }
    newFileSizeNum = atoi(newFileSize);
    //按单位处理为字节大小
    switch (unit) {
    case 'G':{
        newFileSizeNum *= 1024 * 1024 * 1024;
        break;
    }
    case 'M':{
        newFileSizeNum *= 1024 * 1024;
        break;
    }
    case 'K':{
        newFileSizeNum *= 1024;
        break;
    }
    }
    //对文件进入分割
    char tmp[READSIZE];
    int countNum = 0;
    int partNum = 1;
    char newFilePath[255];
    while (!feof(in)) {
        //如果当前写入的文件大小已经达到用户设置的文件大小，则进入初始化
        if(countNum >= newFileSizeNum){
            countNum = 0;
            partNum ++;
            fclose(out);
            out = nullptr;
        }
        fgets(tmp, READSIZE, in);
 
        //如果out为空指针，则进入打开文件
        if(out == nullptr){
            //建议新的文件
            strcpy(newFilePath, filePath);
            strcat(newFilePath, "_");
            sprintf(newFilePath, "%s%d", newFilePath, partNum);
            //打开新文件
            out = fopen(newFilePath, "a+");
        }
        //写文件
        fputs(tmp, out);
        //统计实际写入的文件大小
        countNum += strlen(tmp);
    }
    //关闭读、写的文件
    fclose(in);
    fclose(out);
 
    printf("文件分割完成,本次共分割了%d个新文件", partNum);
 
    return 0;
}
