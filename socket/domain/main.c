#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

// 一个IP地址的最大长度限制
#define IP_LEN 64

/* 函数名：printDomainNameInfo
 * 作用：打印域名相关信息
 * 参数：
 *		 domain_name：域名
 * 返回值：
 *       -1：失败
 *       ==0：成功
 */
int printDomainNameInfo(char *domain_name)
{
	struct hostent *host = NULL;
	host = gethostbyname(domain_name);
	if (NULL == host)
	{
		return -1;
	}

	printf("domain name:%s\n", domain_name);
	printf("h_name:%s\n", host->h_name);

	int i;
	for (i = 0; host->h_aliases[i] != NULL; i++)
	{
		printf("h_aliases%d:%s\n", i + 1, host->h_aliases[i]);
	}

	printf("h_addrtype:%d (%s)\n", host->h_addrtype, (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");
	printf("h_length:%d\n", host->h_length);

	char tmpStr[128] = {0};
	for (i = 0; host->h_addr_list[i] != NULL; i++)
	{
		memset(tmpStr, 0, sizeof(tmpStr));
		inet_ntop(host->h_addrtype, host->h_addr_list[i], tmpStr, IP_LEN);
		printf("Ip%d:%s\n", i + 1, tmpStr);
	}

	return 0;
}

/* 函数名：getIpFromDomainName
 * 作用：根据域名获取IP
 * 参数：
 *		 domain_name：域名
 * 		 ip：获取到的IP
 * 返回值：
 *       -1：失败
 *       ==0：成功
 */
int getIpFromDomainName(char *domain_name, char *ip, unsigned char ipLen)
{
	struct hostent *host = NULL;
	host = gethostbyname(domain_name);
	if (NULL == host)
	{
		return -1;
	}

	inet_ntop(host->h_addrtype, host->h_addr, ip, ipLen);

	return 0;
}

/* 函数名：getAllIpFromDomainName
 * 作用：根据域名获取所有IP
 * 参数：
 *		 domain_name：域名
 * 		 ipList：获取到的所有IP，以空格分割
 *       len：ipList的可用长度
 * 返回值：
 *       -1：失败
 *       >=0：获取到的IP个数
 */
int getAllIpFromDomainName(char *domain_name, char *ipList, unsigned char _ipListLen, unsigned char ipLen)
{
	struct hostent *host = NULL;
	host = gethostbyname(domain_name);
	if (NULL == host)
	{
		return -1;
	}

	int i;
	int len = 0;
	int ipCnt = 0; // IP的个数
	int pos = 0;
	char tmpIp[IP_LEN];
	for (i = 0; host->h_addr_list[i] != NULL; i++)
	{
		memset(tmpIp, 0, sizeof(tmpIp));
		inet_ntop(host->h_addrtype, host->h_addr_list[i], tmpIp, ipLen);
		len = strlen(tmpIp);
		if (len > 0 && pos < _ipListLen)
		{
			ipCnt++;
			strcpy(&ipList[pos], tmpIp);
			pos += len;
			ipList[pos++] = ' ';
		}
	}

	return ipCnt;
}

int main(int argc, char *argv[])
{
	if (argc < 2)
	{
		printf("argc should more then 1, list this:./main www.baidu.com\n");
		return -1;
	}

	// 获取域名相关信息
	int ret = printDomainNameInfo(argv[1]);
	if (ret == 0)
	{
		printf("printDomainNameInfo Ok!\n");
	}
	else
	{
		printf("printDomainNameInfo Err, domain name is:%s, please check it again!\n", argv[1]);
	}
	puts("**************************************");

	char ip[128] = {0};
	// 获取首个IP
	ret = getIpFromDomainName(argv[1], (char *)ip, IP_LEN);
	if (ret == 0)
	{
		printf("ip:%s\n", ip);
	}
	else
	{
		printf("get ip from domain failed.\n");
	}

	memset(ip, 0, sizeof(ip));

	puts("**************************************");
	// 获取所有IP
	ret = getAllIpFromDomainName(argv[1], (char *)ip, sizeof(ip) - 1, IP_LEN);
	if (ret > 0)
	{
		printf("All ip:%s\n", ip);
	}
	else
	{
		printf("get all ip from domain failed.\n");
	}

	return 0;
}
