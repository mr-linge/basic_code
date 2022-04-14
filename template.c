#include <stdio.h>

typedef struct doc_type{
        char *key;
        char *value;
} HTTP_CONTENT_TYPE;

HTTP_CONTENT_TYPE http_content_type[] = {
        { "html","text/html" },
        { "gif" ,"image/gif" },
        { "jpeg","image/jpeg" }
};


int main(int argc, char * argv[]) {
	for(int i = 0; i < sizeof(http_content_type)/sizeof(HTTP_CONTENT_TYPE); i++) {
		printf("%s:%s\n",http_content_type[i].key,http_content_type[i].value);
	}
	
	return 0;
}
