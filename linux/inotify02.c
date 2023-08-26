#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/inotify.h>

#define EVENT_NUM 12

// directory handle event descrition
const char *event_str[EVENT_NUM] =
    {
        "IN_ACCESS",
        "IN_MODIFY",
        "IN_ATTRIB",
        "IN_CLOSE_WRITE",
        "IN_CLOSE_NOWRITE",
        "IN_OPEN",
        "IN_MOVED_FROM",
        "IN_MOVED_TO",
        "IN_CREATE",
        "IN_DELETE",
        "IN_DELETE_SELF",
        "IN_MOVE_SELF"};

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("input file path\n");
        exit(1);
    }
    char *path1 = argv[1];
    char *path2 = argv[2];

    int fd = inotify_init();
    if (fd < 0)
    {
        fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    int wd1 = inotify_add_watch(fd, path1, IN_ALL_EVENTS);
    if (wd1 < 0)
    {
        fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    int wd2 = inotify_add_watch(fd, path2, IN_ALL_EVENTS);
    if (wd2 < 0)
    {
        fprintf(stderr, "%s:%d %s\n", __FILE__, __LINE__, strerror(errno));
        exit(1);
    }

    // inotify_rm_watch(fd, wd2); // delete inotiry
    unsigned long index;
    struct inotify_event event;
    unsigned long event_size = sizeof(struct inotify_event);

    // printf("event_size:%lu\n", event_size);
    struct inotify_event events[0x100] = {0};
    // 检测事件是否发生，没有发生就会阻塞
    unsigned long read_len;
loop:
    read_len = read(fd, events, 0x100 * event_size);
    printf("read_len:%lu\n", read_len);

    // 如果read的返回值，小于inotify_event大小出现错误
    if (read_len < event_size)
    {
        printf("could not get event!\n");
        return -1;
    }

    index = 0;
    while (read_len > 0)
    {
        event = events[index];
        printf("wd      :%u\n", event.wd);
        printf("mask    :%u\n", event.mask);
        printf("len     :%u\n", event.len);
        for (unsigned long i = 0; i < EVENT_NUM; i++)
        {
            // 判断事件是否发生
            if ((event.mask >> i) & 1)
            {
                if (event.len > 0)
                { // 该监控项为目录或目录下的文件时
                    fprintf(stdout, "%s --- %s\n", event.name, event_str[i]);
                }
                else if (event.len == 0)
                { // 该监控项为文件时
                    if (event.wd == wd1)
                    {
                        fprintf(stdout, "%s --- %s\n", argv[1], event_str[i]);
                    }
                }
            }
        }
        read_len -= event_size;
        printf("==============%lu==============\n", index);
    }

    goto loop;
    return 0;
}
