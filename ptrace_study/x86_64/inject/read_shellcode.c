#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

typedef struct {
    char *message;
    unsigned long len;
} Message;

// 根据路径获取目录里的内容
void read_message(char *path, Message **msg) {
    struct stat buf;
    stat(path, &buf);

    *msg = (Message *) malloc(sizeof(Message));
    (*msg)->len = buf.st_size;
    (*msg)->message = (char *) malloc((*msg)->len);

    int fd = open(path, O_RDONLY, S_IRWXU | S_IRWXG | S_IRWXO);
    unsigned long size = read(fd, (*msg)->message, (*msg)->len);
    if (size < 0) {
        perror("open file fail!");
        exit(1);
    }
    close(fd);
}

// 把数据写到指定路径的目录里,如果目录不存在则先创建目录再写入内容
void write_message(char *path, Message *msg) {
    int fd = open(path, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXO);
    if (fd < 0) {
        perror("create directory fail!");
        exit(-1);
    }
    unsigned long size = write(fd, msg->message, msg->len);
    if (size < 0) {
        perror("open file fail!");
        exit(1);
    }
    close(fd);
}

char *path = "/home/dio/Repositories/C_study/ptrace_study/x86_64/inject/inject3/payload";

int main() {
    Message *msg = NULL;
    read_message(path, &msg);
    printf("message: %s\nlen: %lu\n", msg->message, msg->len);
    return 0;
}

