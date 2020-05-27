#define pc reg[7]
#define sp reg[6]
#define MEMSIZE (64*1024)
#define display_status mem[0177564]
#define display_val mem[0177566]
#define keyboard_status mem[0177560]
#define keyboard_val mem[0177562]

#define NO_PARAMS 0
#define HAS_SS 2
#define HAS_DD 1
#define HAS_NN 4
#define HAS_R 8
#define HAS_XX 16

typedef unsigned char byte;
typedef unsigned short int word;
typedef word Adress;
extern int N, Z, C;

struct Argument {
	word val;		//
	word adr;		//
};


typedef struct {
	word mask;
	word opcode;
	char * name;
	void (*do_func) (struct Argument dd, struct Argument ss, unsigned int nn, unsigned int r, unsigned int xx);
	char params;
} Command;


word reg[8];
byte mem[MEMSIZE];

void b_write(Adress adr, byte b);
byte b_read(Adress adr);
void w_write(Adress adr, word w);
word w_read(Adress adr);
void load_file();
void mem_dump(Adress adr, word w);
void print_reg();
void trace(const char * fmt, ...);
