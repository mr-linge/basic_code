# inotify 文件监控

inotify 是一个内核用于通知用户空间程序文件系统变化的机制.
inotify 的实现过程总结为以下两点:
当用户调用读写、创建或删除的系统调用时,内核会注入相应的事件触发函数来产生一个事件,并且添加到 inotify 的事件队列中。
唤醒等待读取事件的进程,当进程被唤醒后,用户就可以通过调用 read 函数来读取 inotify 事件队列中的事件,进而获知文件的变化。

## 1. inotify_init

int fd = inotify_init(void);
用于创建一个inotify的实例,然后返回inotify事件队列的文件描述符

## 2. inotify_add_watch

int wd = inotify_add_watch(int fd, const char* pathname, uint32_t mask);
添加"watch list",也就是监控列表。 可以是一个新的watch,也可以是一个已经存在的watch。其中 fd 就是 inotify_init() 的返回值,pathname 是要监控的目录或者文件的路径,mask 就是要监控的事件类型。见下文 inotify_event 中的 mask 字段
该函数执行成功则返回一个unique的watch描述符。返回值是一个inotify标识,注意不要和 inotify_init() 的返回值搞混淆了。inotify_init() 的返回值是在读取事件、注册监听文件时使用,而 inotify_add_watch() 的返回值用来判断返回的事件属于哪个监听的文件（后面介绍inotify_event结构时会看到）,以及移除监听文件时使用。

## 3. inotify_rm_watch

int inotify_rm_watch(int fd, int wd);
用于从 watch list 中移除监控的对象。其中 fd 是 inotify_init() 的返回值,wd 是 inotify_add_watch() 的返回值。

## 处理监控事件

struct inotify_event events[0x100] = {0};
// 检测事件是否发生,没有发生就会阻塞
unsigned long read_len = read(fd, events, 0x100 * sizeof(struct inotify_event));

struct inotify_event {
    int      wd;       /*监控对象的watch描述符*/
    uint32_t mask;     /*事件掩码*/
    uint32_t cookie;   /*和rename事件相关*/
    uint32_t len;      /*name字段的长度*/
    char     name[];   /*监控对象的文件或目录名*/
};

mask 所表示的事件监控类型如下:
事件                                    描述
IN_ACCESS                           文件被访问
IN_ATTRIB                           元数据被改变,例如权限、时间戳、扩展属性、链接数、UID、GID等
IN_CLOSE_WRITE                      打开用于写的文件被关闭
IN_CLOSE_NOWRITE                    不是打开用于写的文件被关闭
IN_CREATE                           在监控的目录中创建了文件或目录
IN_DELETE                           在监控的目录中删除了文件或目录
IN_DELETE_SELF                      监控的文件或目录本身被删除
IN_MODIFY                           文件被修改,这种事件会用到 inotify_event 中的 cookie
IN_MOVE_SELF                        监控的文件或目录本身被移动
IN_MOVED_FROM                       从监控的目录中移出文件
IN_MOVED_TO                         向监控的目录中移入文件
IN_OPEN                             文件或目录被打开
IN_ALL_EVENTS                       包含了上面提到的所有事件

name 注意事项:
如果是监控目录,此时目录下的文件触发事件,会输出对应的文件名。但是如果只监控文件,则无法输出对应更改的文件名。
