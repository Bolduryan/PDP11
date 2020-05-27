#include <stdio.h>
#include "pdp.h"
#include <stdlib.h>

int N, Z, C;
void set_NZ(word w){
	if (w >> 15){
		N = 1;
		Z = 0;
	}
	else if (w == 0){
		N = 0;
		Z = 1;
	}
	else{
		N = 0;
		Z = 0;
	}
}

void set_C(word w){
	if (w >> 15)
		C = 1;
	else 
		C = 0;
}

void do_mov(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (dd.adr < 8) {
		reg[dd.adr] = ss.val;
		set_NZ(ss.val);
	}
	else {
		w_write(dd.adr, ss.val);
		set_NZ(ss.val);
	}
};


void do_movb(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (dd.adr < 8){
		if (ss.val >> 7 == 1){
			reg[dd.adr] = 0xFF00 | ss.val;
			set_NZ(reg[dd.adr]);
		}
		else{
			reg[dd.adr] = 0x0000 | ss.val;
			set_NZ(reg[dd.adr]);
		}
	}
	else{
		b_write(dd.adr, ss.val);
		set_NZ(ss.val);
	}
	if (dd.adr == 0177566)
		printf("%c", display_val);
};


void do_add(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (dd.adr < 8 && ss.adr < 8) {
		reg[dd.adr] = reg[dd.adr] + reg[ss.adr];
		set_NZ(reg[dd.adr]);
		set_C(reg[dd.adr] >> 1);
	}
	else if (dd.adr < 8 && ss.adr > 8){
		reg[dd.adr] = reg[dd.adr] + ss.val;
		set_NZ(reg[dd.adr]);
		set_C(reg[dd.adr] >> 1);
	}
	else {
		w_write(dd.adr, w_read(dd.adr) + w_read(ss.adr));
		set_NZ(w_read(dd.adr));
		set_C(w_read(dd.adr) >> 1);
	}
};


void print_reg() {
	trace("R0:%o R1:%o R2:%o R3:%o \nR4:%o R5:%o R6:%o R7:%o\n", reg[0], reg[1], reg[2], reg[3], reg[4], reg[5], reg[6], reg[7]);
};

void do_halt() {
	trace("\n------------ halted -------------\n");
	print_reg();
	exit(0);
};


void do_inc(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (dd.adr < 8)
		reg[dd.adr] += 1;
	else
		w_write(dd.adr, w_read(dd.adr) + 1);
};


void do_incb(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	b_write(dd.adr, b_read(dd.adr) + 1);
};

void do_sob(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	reg[r] -= 1;
	if (reg[r] != 0)
		pc -= nn;
};

void do_clr(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (dd.adr < 8)
		reg[dd.adr] = 0;
	else
		w_write(dd.adr, 0);
};

void do_tst(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (dd.adr < 8)
		set_NZ(reg[dd.adr]);
	else
		set_NZ(w_read(dd.adr));
};

void do_tstb(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (dd.adr < 8)
		set_NZ(reg[dd.adr] << 8);
	else
		set_NZ(b_read(dd.adr) << 8);
};

void do_cmp(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (dd.adr < 8 && ss.adr < 8){
		word w = reg[ss.adr] - reg[dd.adr];
		set_NZ(w);
		set_C(w >> 1);
	}
	else{
		word w = w_read(ss.adr) - w_read(dd.adr);
		set_NZ(w);
		set_C(w >> 1);
	}
};

void do_cmpb(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (dd.adr < 8 && ss.adr < 8){
		byte w = reg[ss.adr] - reg[dd.adr];
		set_NZ(w << 8);
		set_C(w << 7);
	}
	else{
		byte w = w_read(ss.adr) - w_read(dd.adr);
		set_NZ(w << 8);
		set_C(w << 7);
	}
};

void do_ccc(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	N = 0;
	Z = 0;
	C = 0;
};

void do_cln(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	N = 0;
};

void do_clz(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	Z = 0;
};

void do_clc(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	C = 0;
};

void do_scc(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	N = 1;
	Z = 1;
	C = 1;
};

void do_sec(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	C = 1;
};

void do_sen(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	N = 1;
};

void do_sez(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	Z = 1;
};

void do_nop(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
};

void do_br(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	pc = pc + 2*xx;
};

void do_bcc(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (C == 0)
		do_br(dd, ss, nn, r, xx);
};

void do_beq(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (Z == 1)
		do_br(dd, ss, nn, r, xx);
};

void do_bpl(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	if (N == 0)
		do_br(dd, ss, nn, r, xx);
};

void do_jsr(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	w_write(sp, reg[r]);
	sp += 2;
	reg[r] = pc;
	pc = dd.adr;

};

void do_rts(struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx) {
	pc = reg[r];
	sp -= 2;
	reg[r] = w_read(sp);
};

Command cmd[] = {
	{0170000, 0010000, "mov", do_mov, 0x03},
	{0170000, 0060000, "add", do_add, 0x03},
	{0xFFFF, 0000000, "halt", do_halt, 0x00},
	{0170000, 0110000, "movb", do_movb, 0x03},
	{0177700, 0015200, "inc", do_inc, 0x01},
	{0177700, 0115200, "incb", do_incb, 0x01},
	{0177000, 0077000, "sob", do_sob, 0x0C},
	{0177700, 0005000, "clr", do_clr, 0x01},
	{0177700, 0005700, "tst", do_tst, 0x01},
	{0177700, 0105700, "tstb", do_tstb, 0x01},
	{0170000, 0020000, "cmp", do_cmp, 0x03},
	{0170000, 0120000, "cmpb", do_cmpb, 0x03},
	{0177777, 0000257, "ccc", do_ccc, 0x00},
	{0177777, 0000241, "clc", do_clc, 0x00},
	{0177777, 0000250, "cln", do_cln, 0x00},
	{0177777, 0000244, "clz", do_clz, 0x00},
	{0177777, 0000277, "scc", do_scc, 0x00},
	{0177777, 0000261, "sec", do_sec, 0x00},
	{0177777, 0000270, "sen", do_sen, 0x00},
	{0177777, 0000264, "sez", do_sez, 0x00},
	{0177777, 0000240, "nop", do_nop, 0x00},
	{0xFF00, 0x0100, "br", do_br, 0x10},
	{0xFF00, 0x8600, "bcc",do_bcc, 0x10},
	{0xFF00, 0x0300, "beq", do_beq, 0x10},
	{0xFF00, 0x8000, "bpl", do_bpl, 0x10},
	{0177000, 0004000, "jsr", do_jsr, 0x09},
	{0177770, 0000200, "rts", do_rts, 0x08}
};
