# http 构造

socket 实现一个 http 发送和接收, 主要实现了接收数据的解析. 根据 header 中 Content-Length 来计算 需要接收的数据量
