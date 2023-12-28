
struct HTTP_DATA
{
    char *header;
    unsigned long header_length;
    char *body;
    unsigned long body_length;
};

struct HTTP_HEAD_INFO
{
    char http_type[0x10];
    char router[0x200];
    char content_type[0x100];
    unsigned long content_length;
};

// struct FILE_INFO
// {
//     char file_name[0x100];
//     char file_path[0x100];
// };
