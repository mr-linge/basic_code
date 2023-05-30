# socket
socket起源于Unix，而Unix/Linux基本哲学之一就是“一切皆文件”，都可以用“打开open –> 读写write/read –> 关闭close”模式来操作。Socket就是该模式的一个实现，socket即是一种特殊的文件，一些socket函数就是对其进行的操作（读/写IO、打开、关闭）

## socket的基本操作
### socket()函数
int socket(int domain, int type, int protocol);
socket函数对应于普通文件的打开操作。普通文件的打开操作返回一个文件描述字，而socket()用于创建一个socket描述符（socket descriptor），它唯一标识一个socket。这个socket描述字跟文件描述字一样，后续的操作都有用到它，把它作为参数，通过它来进行一些读写操作。

正如可以给fopen的传入不同参数值，以打开不同的文件。创建socket的时候，也可以指定不同的参数创建不同的socket描述符.
socket函数的三个参数分别为：
domain：即协议域，又称为协议族（family）。常用的协议族有，AF_INET、AF_INET6、AF_LOCAL（或称AF_UNIX，Unix域socket）、AF_ROUTE等等。协议族决定了socket的地址类型，在通信中必须采用对应的地址，如AF_INET决定了要用ipv4地址（32位的）与端口号（16位的）的组合、AF_UNIX决定了要用一个绝对路径名作为地址。
type：指定socket类型。常用的socket类型有，SOCK_STREAM、SOCK_DGRAM、SOCK_RAW、SOCK_PACKET、SOCK_SEQPACKET等等（socket的类型有哪些？）。
protocol：故名思意，就是指定协议。常用的协议有，IPPROTO_TCP、IPPTOTO_UDP、IPPROTO_SCTP、IPPROTO_TIPC等，它们分别对应TCP传输协议、UDP传输协议、STCP传输协议、TIPC传输协议（这个协议我将会单独开篇讨论！）。
注意：并不是上面的type和protocol可以随意组合的，如SOCK_STREAM不可以跟IPPROTO_UDP组合。当protocol为0时，会自动选择type类型对应的默认协议。

当调用socket创建一个socket时，返回的socket描述字它存在于协议族（address family，AF_XXX）空间中，但没有一个具体的地址。如果想要给它赋值一个地址，就必须调用bind()函数，否则就当调用connect()、listen()时系统会自动随机分配一个端口。

### bind()函数
正如上面所说bind()函数把一个地址族中的特定地址赋给socket。例如对应AF_INET、AF_INET6就是把一个ipv4或ipv6地址和端口号组合赋给socket
int bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
函数的三个参数分别为：
sockfd：即socket描述字，它是通过socket()函数创建了，唯一标识一个socket。bind()函数就是将给这个描述字绑定一个名字。
addr：一个const struct sockaddr *指针，指向要绑定给sockfd的协议地址。这个地址结构根据地址创建socket时的地址协议族的不同而不同，
如ipv4对应的是：
struct sockaddr_in {
    sa_family_t    sin_family; /* address family: AF_INET */
    in_port_t      sin_port;   /* port in network byte order */
    struct in_addr sin_addr;   /* internet address */
};

/* Internet address. */
struct in_addr {
    uint32_t       s_addr;     /* address in network byte order */
};
ipv6对应的是：
struct sockaddr_in6 { 
    sa_family_t     sin6_family;   /* AF_INET6 */ 
    in_port_t       sin6_port;     /* port number */ 
    uint32_t        sin6_flowinfo; /* IPv6 flow information */ 
    struct in6_addr sin6_addr;     /* IPv6 address */ 
    uint32_t        sin6_scope_id; /* Scope ID (new in 2.4) */ 
};

struct in6_addr { 
    unsigned char   s6_addr[16];   /* IPv6 address */ 
};
Unix域对应的是：
#define UNIX_PATH_MAX    108

struct sockaddr_un { 
    sa_family_t sun_family;               /* AF_UNIX */ 
    char        sun_path[UNIX_PATH_MAX];  /* pathname */ 
};
addrlen：对应的是地址的长度。
通常服务器在启动的时候都会绑定一个众所周知的地址（如ip地址+端口号），用于提供服务，客户就可以通过它来接连服务器；而客户端就不用指定，有系统自动分配一个端口号和自身的ip地址组合。这就是为什么通常服务器端在listen之前会调用bind()，而客户端就不会调用，而是在connect()时由系统随机生成一个。
### listen()、connect()函数
如果作为一个服务器，在调用socket()、bind()之后就会调用listen()来监听这个socket，如果客户端这时调用connect()发出连接请求，服务器端就会接收到这个请求。

int listen(int sockfd, int backlog);
int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
listen函数的第一个参数即为要监听的socket描述字，第二个参数为相应socket可以排队的最大连接个数。socket()函数创建的socket默认是一个主动类型的，listen函数将socket变为被动类型的，等待客户的连接请求。

connect函数的第一个参数即为客户端的socket描述字，第二参数为服务器的socket地址，第三个参数为socket地址的长度。客户端通过调用connect函数来建立与TCP服务器的连接。

### accept()函数
TCP服务器端依次调用socket()、bind()、listen()之后，就会监听指定的socket地址了。TCP客户端依次调用socket()、connect()之后就向TCP服务器发送了一个连接请求。TCP服务器监听到这个请求之后，就会调用accept()函数接收请求，这样连接就建立好了。之后就可以开始网络I/O操作了，即类同于普通文件的读写I/O操作。

int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
accept函数的第一个参数为服务器的socket描述字，第二个参数为指向struct sockaddr *的指针，用于返回客户端的协议地址，第三个参数为协议地址的长度。如果accpet成功，那么其返回值是由内核自动生成的一个全新的描述字，代表与返回客户的TCP连接。

注意：accept的第一个参数为服务器的socket描述字，是服务器开始调用socket()函数生成的，称为监听socket描述字；而accept函数返回的是已连接的socket描述字。一个服务器通常只创建一个监听socket描述字，它在该服务器的生命周期内一直存在。内核为每个由服务器进程接收的客户连接创建了一个已连接socket描述字，当服务器完成了对某个客户的服务，相应的已连接socket描述字就被关闭。

### read()、write()等函数
至此服务器与客户已经建立好连接了。可以调用网络I/O进行读写操作了，即实现了网咯中不同进程之间的通信！网络I/O操作有下面几组：
read()/write()
recv()/send()
readv()/writev()
recvmsg()/sendmsg()
recvfrom()/sendto()
推荐使用recvmsg()/sendmsg()函数，这两个函数是最通用的I/O函数，实际上可以把上面的其它函数都替换成这两个函数。

#### recvmsg()/sendmsg()
#include <sys/types.h>
#include <sys/socket.h>
ssize_t recvmsg(int sockfd, struct msghdr *msg, int flags);
ssize_t sendmsg(int sockfd, struct msghdr *msg, int flags);

参数说明:
第1个参数
sockfd      创建的sockfd
第2个参数
struct msghdr 
{
    void *msg_name;           /* 指向地址结构 */
    int msg_namelen;          /* 地址结构长度 */
    struct iovec *msg_iov;    /* 数据 */
    int msg_iovlen;           /* 数据区个数 */
    void *msg_control;        /* 控制信息 */
    socklen_t msg_controllen; /* 控制信息缓冲区长度 */
    unsigned int msg_flags;   /* 接收信息的标志 */
}
msg_name 和 msg_namelen：这两个成员用于套接字未连接的场合（如 UDP 套接字）。类似于 recvfrom 和 sendto 的第五个和第六个参数：msg_name 指向一个套接字地址结构，调用者在其中存放接收者或发送者的协议地址。如果无需知名协议地址，msg_name 应为 NULL。msg_namelen 对于 sendmsg 是一个值参数，对于 recvmsg 是一个值-结果参数。

msg_iov 和 msg_iovlen：输入或者输出缓冲区数组。类似于 readv 或 writev 的第二个和第三个参数。

对于 recvmsg 和 sendmsg，我们必须区别他们两个标志变量，一个是传递的 flags 参数，另一个是传递的 msghdr 结构的 msg_flags 成员，它传递的是引用。

msg_flags ：只有 recvmsg 使用 msg_flags 成员。recvmsg 被调用时，flags 参数被赋值到 msg_flags 成员，并由内核使用其值驱动接受处理过程。内核还依据 recvmsg 的结果更新 msg_flags 成员的值。而 sendmsg 则忽略 msg_flags 成员，因为它直接使用 flags 参数驱动发送处理过程。这一点意味着如果想在某个 sendmsg 调用中设置 MSG_DONTWAIT 标志，就把 flags 参数设置为该值，而msg_flags 成员设置为该值没有作用。

第3个参数
flags   用于传入控制信息，一般包括以下几个
MSG_DONTROUTE               send可用
MSG_DONWAIT                 send与recv都可用
MSG_PEEK                    recv可用
MSG_WAITALL                 recv可用
MSG_OOB                     send可用
MSG_EOR                     send recv都可用

返回值:
成功返回读入或者写出的字节数，出错返回 -1 并设置 errno。

#### recv()/send()
##### send函数
int send( SOCKET s, const char FAR *buf, int len, int flags);
不论是客户还是服务器应用程序都用send函数来向TCP连接的另一端发送数据。
客户程序一般用send函数向服务器发送请求，而服务器则通常用send函数来向客户程序发送应答。
（1）第一个参数指定发送端套接字描述符；
（2）第二个参数指明一个存放应用程序要发送数据的缓冲区；
（3）第三个参数指明实际要发送的数据的字节数；
（4）第四个参数一般置0。
这里只描述同步Socket的send函数的执行流程。当调用该函数时，send先比较待发送数据的长度len和套接字s的发送缓冲的长度， 如果len大于s的发送缓冲区的长度，该函数返回SOCKET_ERROR；如果len小于或者等于s的发送缓冲区的长度，那么send先检查协议是否正在发送s的发送缓冲中的数据，如果是就等待协议把数据发送完，如果协议还没有开始发送s的发送缓冲中的数据或者s的发送缓冲中没有数据，那么send就比较s的发送缓冲区的剩余空间和len，如果len大于剩余空间大小send就一直等待协议把s的发送缓冲中的数据发送完，如果len小于剩余空间大小send就仅仅把buf中的数据copy到剩余空间里（注意并不是send把s的发送缓冲中的数据传到连接的另一端的，而是协议的，send仅仅是把buf中的数据copy到s的发送缓冲区的剩余空间里）。

返回值：成功返回发送的字节数；失败返回-1，同时errno被设置
如果send函数copy数据成功，就返回实际copy的字节数，如果send在copy数据时出现错误，那么send就返回SOCKET_ERROR；如果send在等待协议传送数据时网络断开的话，那么send函数也返回SOCKET_ERROR。

要注意send函数把buf中的数据成功copy到s的发送缓冲的剩余空间里后它就返回了，但是此时这些数据并不一定马上被传到连接的另一端。如果协议在后续的传送过程中出现网络错误的话，那么下一个Socket函数就会返回SOCKET_ERROR。（每一个除send外的Socket函数在执行的最开始总要先等待套接字的发送缓冲中的数据被协议传送完毕才能继续，如果在等待时出现网络错误，那么该Socket函数就返回SOCKET_ERROR）。

注意：在Unix系统下，如果send在等待协议传送数据时网络断开的话，调用send的进程会接收到一个SIGPIPE信号，进程对该信号的默认处理是进程终止。

##### recv函数
int recv( SOCKET s, char FAR *buf, int len, int flags );
不论是客户还是服务器应用程序都用recv函数从TCP连接的另一端接收数据。
（1）第一个参数指定接收端套接字描述符；
（2）第二个参数指明一个缓冲区，该缓冲区用来存放recv函数接收到的数据；
（3）第三个参数指明buf的长度；
（4）第四个参数一般置0。
这里只描述同步Socket的recv函数的执行流程。当应用程序调用recv函数时，recv先等待s的发送缓冲中的数据被协议传送完毕，如果协议在传送s的发送缓冲中的数据时出现网络错误，那么recv函数返回SOCKET_ERROR，如果s的发送缓冲中没有数据或者数据被协议成功发送完毕后，recv先检查套接字s的接收缓冲区，如果s接收缓冲区中没有数据或者协议正在接收数据，那么recv就一直等待，只到协议把数据接收完毕。当协议把数据接收完毕，recv函数就把s的接收缓冲中的数据copy到buf中（注意协议接收到的数据可能大于buf的长度，所以在这种情况下要调用几次recv函数才能把s的接收缓冲中的数据copy完。recv函数仅仅是copy数据，真正的接收数据是协议来完成的），recv函数返回其实际copy的字节数。如果recv在copy时出错，那么它返回SOCKET_ERROR；如果recv函数在等待协议接收数据时网络中断了，那么它返回0。

返回值：成功返回发送的字节数；失败返回-1，同时errno被设置

注意：在Unix系统下，如果recv函数在等待协议接收数据时网络断开了，那么调用recv的进程会接收到一个SIGPIPE信号，进程对该信号的默认处理是进程终止。

tcp协议本身是可靠的,并不等于应用程序用tcp发送数据就一定是可靠的.不管是否阻塞,send发送的大小,并不代表对端recv到多少的数据.

在阻塞模式下, send函数的过程是将应用程序请求发送的数据拷贝到发送缓存中发送并得到确认后再返回.但由于发送缓存的存在,表现为:如果发送缓存大小比请求发送的大小要大,那么send函数立即返回,同时向网络中发送数据;否则,send向网络发送缓存中不能容纳的那部分数据,并等待对端确认后再返回(接收端只要将数据收到接收缓存中,就会确认,并不一定要等待应用程序调用recv);

在非阻塞模式下,send函数的过程仅仅是将数据拷贝到协议栈的缓存区而已,如果缓存区可用空间不够,则尽能力的拷贝,返回成功拷贝的大小;如缓存区可用空间为0,则返回-1,同时设置errno为EAGAIN.

### close()函数
在服务器与客户端建立连接之后，会进行一些读写操作，完成了读写操作就要关闭相应的socket描述字，好比操作完打开的文件要调用fclose关闭打开的文件。

#include <unistd.h>
int close(int fd);
close一个TCP socket的缺省行为时把该socket标记为以关闭，然后立即返回到调用进程。该描述字不能再由调用进程使用，也就是说不能再作为read或write的第一个参数。

注意：close操作只是使相应socket描述字的引用计数-1，只有当引用计数为0的时候，才会触发TCP客户端向服务器发送终止连接请求。