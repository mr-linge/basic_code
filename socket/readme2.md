# 续

异步
int flags = fcntl(sockfd, F_GETFL, 0);
fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

## sendfile

实现文件传输(linux可用, Darwin上不可用)
`#include <sys/sendfile.h>`
ssize_t sendfile(int out_fd, int in_fd, off_t *offset, size_t count);
