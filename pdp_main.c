#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include "pdp.h"


int TRACE = 0;

void test_mem() {
	byte b0 = 0x0a;
	b_write(2, b0);
	byte bres = b_read(2);
	assert(b0 == bres);
	Adress a = 4;
	byte b1 = 0xcb;
	b0 = 0x0a;
	word w = 0xcb0a;
	b_write(a, b0);
	b_write(a+1, b1);
	word wres = w_read(a);
	assert(w == wres);
	w = 0x0a0b;
	w_write(a, w);
	wres = w_read(a);
	assert(w == wres);
}

void trace(const char * fmt, ...) {
	if (TRACE == 1){
		va_list ap;
		va_start(ap, fmt);
		vprintf(fmt, ap);
		va_end(ap);
	}
}

void key_check(int argc, char * argv[]){
	for (int i = 0; i < argc; i++){
		if (strcmp(argv[i], "-t") == 0)
			TRACE = 1;
	}
}

void run();

int main(int argc, char * argv[]) {
	display_status = 0x80;
	key_check(argc, argv);
	test_mem();
	load_file();
	//mem_dump(4, 2);
	run();
	return 0; 
}

void b_write(Adress adr, byte b) { 
	mem[adr] = b;
}

byte b_read(Adress adr) {
	return mem[adr];
}

void w_write(Adress adr, word w) {
	mem[adr] = w;
	mem[adr + 1] = w >> 8;
}

word w_read(Adress adr) {
	word w = ((word)mem[adr+1]) << 8;
	w = w | mem[adr];
	return w;
}

void load_file() {
	unsigned int adr_1;
	unsigned int N;
	while(scanf("%x%x", &adr_1, &N) == 2) {
		for(unsigned i = 0; i < N; i++) {
			unsigned int k;
			scanf("%x", &k);
			b_write(adr_1 + i, k);
		}
	}
}

void mem_dump(Adress adr, word w) {
	for(unsigned i = 0; i < w; i += 2) { 
		printf("%06o : %06o\n", adr + i, w_read(adr + i));
	}
}
/*
void load_file(int argc, char  * argv[]) {
	FILE * fin =  fopen(argv[1], "r");
	unsigned int adr_1;
	unsigned int N;
	while(scanf("%x%x", &adr_1, &N) == 2) {
		for(unsigned i = 0; i < N; i++) {
			unsigned int k;
			scanf("%x", &k);
			b_write(adr_1 + i, k);
		}
	}
}
*/
