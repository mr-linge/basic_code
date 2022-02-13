//#include <stdio.h>

int func0(){
  //      printf("************** this is in so library ...\n");
	return 10;
}

int func1(int p1) {
	return (p1 + 10);
}

int func2(int p1, int p2) {
	return (p1 + p2);
}

int hook_func10(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10) {
        return 0x99;
}

int hook_func20(int p1, int p2, int p3, int p4, int p5, int p6, int p7, int p8, int p9, int p10,int p11, int p12, int p13, int p14, int p15, int p16, int p17, int p18, int p19, int p20) {
        return 0x999;
}


void read_mem(unsigned long vaddr,unsigned long len) {
    //    printf("Hello, this is read_mem ...\n");
        //uint8_t * tmp_addr = (uint8_t *)vaddr;
        //for(unsigned long i = 0; i < len; i++) {
        //        printf("%02x ",*(tmp_addr + i));
        //}
        //puts("");
}

