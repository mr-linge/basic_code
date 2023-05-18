#include <stdio.h>
#include <getopt.h>

int t1, t2, t3, t4;

struct option longopts[] = {
    {"test1", no_argument, &t1, 1},
    {"test2", no_argument, &t2, 2},
    {"test3", required_argument, &t3, 3},
    {"test4", required_argument, &t4, 4},
    {"test5", required_argument, NULL, 5},
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
./optlong --test1 --test2 --test3 "p3" --test4 "p4" -l "pl"
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
            if (t1 == 1)
            {
                printf("--test1 is going t1:%d \n", t1);
            }
            if (t2 == 2)
            {
                printf("--test2 is going t2:%d \n", t2);
            }
            if (t3 == 3)
            {
                printf("The argument of --test3 is %s\n", optarg);
            }
            if (t4 == 4)
            {
                printf("The argument of --test4 is %s\n", optarg);
            }
            t1 = t2 = t3 = t4 = 0;
        }
        break;
        case 5:
            printf("The argument of --test5 is %s\n", optarg);
            break;
        case 'l':
            printf("HAVE option: -l\n");
            printf("The argument of -l is %s\n", optarg);
            break;
        case '?':
            printf("Unknown option: %c\n", (char)optopt);
            break;
        }
    }
    return 0;
}