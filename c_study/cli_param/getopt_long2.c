#include <stdio.h>
#include <getopt.h>

int t1, t2, t3, t4;

struct option longopts[] = {
    {"test1", no_argument, &t1, 0x100},
    {"test2", no_argument, &t2, 0x101},
    {"test3", required_argument, &t3, 0x102},
    {"test4", required_argument, &t4, 0x103},
    {"test5", required_argument, NULL, 0x104},
    {"test6", required_argument, NULL, 0x105},
    {0, 0, 0, 0},
};
/*
./optlong --test1
./optlong --test2
./optlong --test1 --test2
./optlong --test3 "p3"
./optlong --test4 "p4"
./optlong --test3 "p3" --test4 "p4"
./optlong --test5 "p5"
./optlong -l "pl"
./optlong --test1 --test2 --test3 "p3" --test4 "p4" --test5 "p5" -l "pl"
./optlong --test1 --test2 --test3 "p3" --test4 "p4" --test5 "p5" --test6 "p6" -l "pl"
*/
int main(int argc, char *argv[])
{
    int c;
    printf("optind:%d,opterr:%d,optopt:%d\n", optind, opterr, optopt);
    while ((c = getopt_long(argc, argv, "l:", longopts, NULL)) != -1)
    {
        printf("optind:%d,opterr:%d,optopt:%d\n", optind, opterr, optopt);
        switch (c)
        {
        case 0:
        {
            printf("t1:%d, t2:%d, t3:%d, t4:%d\n", t1, t2, t3, t4);
            if (t1 == 0x100)
            {
                printf("have option: --test1\n");
            }
            if (t2 == 0x101)
            {
                printf("have option: --test2\n");
            }
            if (t3 == 0x102)
            {
                printf("have option: --test3\n");
                printf("The argument of --test3 is %s\n", optarg);
            }
            if (t4 == 0x103)
            {
                printf("have option: --test4\n");
                printf("The argument of --test4 is %s\n", optarg);
            }
            t1 = t2 = t3 = t4 = 0;
        }
        break;
        case 0x104:
            printf("have option: --test5\n");
            printf("The argument of --test5 is %s\n", optarg);
            break;
        case 0x105:
            printf("have option: --test6\n");
            printf("The argument of --test6 is %s\n", optarg);
            break;
        case 'l':
            printf("have option: -l\n");
            printf("The argument of -l is %s\n", optarg);
            break;
        case '?':
            printf("Unknown option: %d\n", optopt);
            break;
        default:
            printf("default:%d\n", c);
            break;
        }
    }
    return 0;
}
