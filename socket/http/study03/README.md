# http 构造

发送数据完毕关闭 writing side, 对端的 recv 返回 0 可以作为接收完数据的标志.
适合自定议 client 和 server 间的通信协议, 现在大部分应用都不支持
