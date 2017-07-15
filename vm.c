/*
** Author - anuz
** This file is distibuted under XYZ license 
**/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define mask_binop 0x80000000
#define mask_op 0x7F000000
#define mask_data 0x00FFFFFF

/* do we need them as globals? */
static int32_t sp;
static int32_t ip;
static int32_t *data;
static FILE *of;
/* The objective of this program is to create a small instruction processing machine
 *
 *
 * */

struct instruction {
	uint32_t binop;
	uint32_t op;
	uint32_t op_data;

};

/*debug is good */
void disp_bin(uint32_t number)
{
	uint32_t shift = 1;
	shift <<= sizeof(uint32_t)*CHAR_BIT - 1;
	while(shift) {
		if(number & shift){
			printf("1");
		} else {
			printf("0");
		}
		shift >>= 1;
	}
	printf("\n");
}

/* So insert data into the array and move stack pointer up*/
void f(int32_t v)
{
	sp = sp - 1;
	data[sp] = v;
}
/* return data from array and move stack pointer down */
int32_t g()
{
	int32_t v = data[sp];
	sp++;
	return v;
}

int decode(struct instruction *ins, int32_t data)
{
	printf("data = %x\n", data);
	disp_bin(data);
	ins->binop = (data & mask_binop);
	printf("binop %x\n", ins->binop);
	disp_bin(ins->binop);
	ins->binop = ins->binop >> 31;
	disp_bin(ins->binop);
	ins->op = (data & mask_op);
	disp_bin(ins->op);
	printf("op %x\n", ins->op);
	ins->op = ins->op >> 24;
	disp_bin(ins->op);
	ins->op_data = data & mask_data;

	return 0;
}

int perform_action()
{
}

void pop()
{
	sp++;
}

void push(int32_t op_data)
{
	f(op_data);	
}

void load()
{
	int32_t addr = g();
	f(data[addr]);
}

void store()
{
	int32_t st_data = g();
	int32_t	addr = g();
	data[addr] = st_data;
}

void jmp()
{
	int32_t cond = g();
	int32_t addr = g();
	if(cond)
		ip = addr;
}	

void not()
{
	if(g())
		f(1);
	else
		f(0);
	
}

void putch()
{
	unsigned char byte;
	byte = g() & 0xff;
	fprintf(stdout, "%c",byte);
}


void getch()
{
	unsigned char byte;
	int32_t x;
	fread(&byte, sizeof(byte), 1, stdin);
	x = (int32_t)byte;
	f(x & 0xFF);
}

void halt()
{
	exit(EXIT_SUCCESS);
}

int32_t add(int32_t a, int32_t b)
{
	return a + b;
}

int32_t mul(int32_t a, int32_t b)
{
	return a * b;
}

int32_t divi(int32_t a, int32_t b)
{
	if(b)
		return a / b;
	else
		return 0;
}
int32_t sub(int32_t a, int32_t b)
{
	return a - b;
}
int32_t lt(int32_t a, int32_t b)
{
	return a < b;
}
int32_t eq(int32_t a, int32_t b)
{
	return a == b;
}
int32_t xor(int32_t a, int32_t b)
{
	return a ^ b;
}
int32_t and(int32_t a, int32_t b)
{
	return a & b;
}
int32_t or(int32_t a, int32_t b)
{
	return a | b;
}

int operations(struct instruction ins)
{
	int32_t binop = ins.binop;
	int32_t op = ins.op;
	int32_t op_data = ins.op_data;
	if(binop == 0) {
		switch(op) {
			case 0x0:
				pop();
				break;
			case 0x1:
				push(op_data);
				break;
			case 0x2:
				push(ip);
				break;
			case 0x3:
				push(sp);
				break;
			case 0x4:
				load();
				break;
			case 0x5:
				store();
				break;
			case 0x6:
				jmp();
				break;
			case 0x7:
				not();
				break;
			case 0x8:
				putch();
				break;
			case 0x9:
				getch();
				break;
			case 0xa:
				halt();
				break;
			default:
				fprintf(stderr, "Why does this value exist? %x \n", op);
		}
	} else {
		int32_t b = g();
		int32_t a = g();
		int32_t result;

		switch(op) {
			case 0x0:
				result = add(a, b);
				break;
			case 0x1:
				result = sub(a, b);
				break;
			case 0x2:
				result = mul(a, b);
				break;
			case 0x3:
				result = divi(a, b);
				break;
			case 0x4:
				result = and(a, b);
				break;
			case 0x5:
				result = or(a, b);
				break;
			case 0x6:
				result = xor(a, b);
				break;
			case 0x7:
				result = eq(a, b);
				break;
			case 0x8:
				result = lt(a, b);
				break;
		}
		f(result);
	}
}

unsigned createMask(unsigned a, unsigned b)
{
	unsigned r = 0;
	unsigned i;
	for (i=a; i<=b; i++)
		r |= 1 << i;

	return r;
}

int main (int argc, char*argv[])
{
	FILE *fp;
	int32_t data_size;
	int32_t image_size;
	int ret; /* keep a status variable for return values */
	unsigned int i;
	unsigned int actual_bytes = 0;
	int32_t line;
	ip = 0;

	if(argc != 2 ) {
		fprintf(stderr, "Usage: ./vm.out <taskfile>\n");
		return 1;
	}

	fp = fopen(argv[1], "r");
	if(!fp ) {
		fprintf(stderr, "Unable to open file\n");
		return 1;
	}

	//ret = fgets(buffer, sizeof(buffer), fp);
	fscanf(fp, "%x", &data_size);
	printf("%x\n", data_size); /* first line might not be the size of the
				      data in the file */
	fscanf(fp, "%x", &image_size);
	printf("%x\n", data_size);
	//while(fgets(buffer,sizeof(buffer), fp)){
	//}
	sp = data_size;
	data = (int32_t *)calloc(data_size, sizeof(int32_t));
	if(!data){
		fprintf(stderr, "Unable to allocate memory\n");
		return 1;
	}
//	printf("%x %x %x\n",createMask(31,31),createMask(24,30),createMask(0,23) );
	for(i = 0; i < image_size; i++) {
		/* should check for EOF */
		if(fscanf(fp, "%x", &line) == EOF)
			break;
		data[i] = line;
		actual_bytes++;
	}

	while(1)
	{
		uint32_t instruction = data[ip];
	//	decode(instruction);
	//	perform_action();
			break;
	}

	for(i = 0; i < actual_bytes; i++) {
		struct instruction ins;
		decode(&ins, data[i]);
	}

clear:
	free(data);
	fclose(fp);

        return 0;
}
