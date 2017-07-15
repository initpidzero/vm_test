/*
** Author - Anuz
** This file is distibuted under XYZ license
** The objective of this program is to create a small instruction processing machine
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

/* no reason to pass three arguments every time */
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
	--sp;
	data[sp] = v;
	//printf("f() %x %x\n", sp, data[sp]);
}

/* return data from array and move stack pointer down */
int32_t g()
{
	int32_t v = data[sp];
	++sp;
	//printf("g() %x %x\n", sp, data[sp]);
	return v;
}

/* Deciper the 32 bit value into more reasonable 
 * 1 bit binop
 * 7 bit op
 * 24 bit operand
 */

int decode(struct instruction *ins, int32_t data)
{
//	printf("data = %x\n", data);
//	disp_bin(data);
	ins->binop = (data & mask_binop);
//	printf("binop %x\n", ins->binop);
//	disp_bin(ins->binop);
	ins->binop = ins->binop >> 31;
//	disp_bin(ins->binop);
	ins->op = (data & mask_op);
//	disp_bin(ins->op);
//	printf("op %x\n", ins->op);
	ins->op = ins->op >> 24;
//	disp_bin(ins->op);
	ins->op_data = data & mask_data;

	return 0;
}

/* A better name would be more desirable
 * Covert hex code into desirable 
 * output in simple ascii readable format 
 * this might solely exist for debugging?
 */
int perform_action(struct instruction ins)
{
	int32_t binop = ins.binop;
	int32_t op = ins.op;
	int32_t op_data = ins.op_data;

	if(binop == 0) {
		switch(op) {
			case 0x0:
				fprintf(of, "pop() \n");
				break;
			case 0x1:
				fprintf(of, "push(%x) \n",op_data);
				break;
			case 0x2:
				fprintf(of, "push(%x) ip\n",ip);
				break;
			case 0x3:
				fprintf(of, "push(%x) sp\n",sp);
				break;
			case 0x4:
				fprintf(of, "load() \n");
				break;
			case 0x5:
				fprintf(of, "store() \n");
				break;
			case 0x6:
				fprintf(of, "jmp() \n");
				break;
			case 0x7:
				fprintf(of, "not() \n");
				break;
			case 0x8:
				fprintf(of, "putch() \n");
				break;
			case 0x9:
				fprintf(of, "getch() \n");
				break;
			case 0xa:
				fprintf(of, "halt() \n");
				break;
			default:
				fprintf(of, "Why does this value exist? %x \n", op);
		}
	} else {
		int32_t b = g();
		int32_t a = g();
		int32_t result;

		switch(op) {
			case 0x0:
				fprintf(of, "add(%x %x) \n",a, b);
				break;
			case 0x1:
				fprintf(of, "sub(%x %x) \n",a, b);
				break;
			case 0x2:
				fprintf(of, "mul(%x %x) \n",a, b);
				break;
			case 0x3:
				fprintf(of, "divi(%x %x) \n",a, b);
				break;
			case 0x4:
				fprintf(of, "and(%x %x) \n",a, b);
				break;
			case 0x5:
				fprintf(of, "or(%x %x) \n",a, b);
				break;
			case 0x6:
				fprintf(of, "xor(%x %x) \n",a, b);
				break;
			case 0x7:
				fprintf(of, "eq(%x %x) \n",a, b);
				break;
			case 0x8:
				fprintf(of, "lt(%x %x) \n",a, b);
				break;
			default:
				fprintf(of, "Why does this value exist? %x \n", op);
		}
	}

}

/* series of basic instructions */
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
	if(g() == 0)
		f(1);
	else
		f(0);
	
}

/* to avoid conflict with libarary name */
void putch()
{
	unsigned char byte;
	byte = g() & 0xff;
	fprintf(stdout, "%c",byte);
}

/* to avoid conflict with libarary name */
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

/* Arthmetic and binary operations */
int32_t add(int32_t a, int32_t b)
{
	return a + b;
}

int32_t mul(int32_t a, int32_t b)
{
	return a * b;
}

/* to avoid conflict with libarary name */
int32_t divi(int32_t a, int32_t b)
{
	if(b)
		return a / b;
	else /* clearly don't want to divide by zero */
		return 0; 
}

int32_t sub(int32_t a, int32_t b)
{
	return a - b;
}

/* this is purely less than and not less than equal to */
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

/* This function actually carries out action as decoded from the hex values 
 *
 */
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
			default:
				fprintf(stderr, "Why does this value exist? %x \n", op);
		}
	//	printf(" a %x b %x result %x\n", a, b, result);
		f(result);
	}

	return 0;
}

unsigned createMask(unsigned a, unsigned b)
{
	unsigned r = 0;
	unsigned i;
	for (i=a; i<=b; i++)
		r |= 1 << i;

	return r;
}

int main (int argc, char *argv[])
{
	FILE *fp;
	int32_t data_size;
	int32_t image_size;
	int ret; /* keep a status variable for return values */
	unsigned int i;
	unsigned int actual_bytes = 0;
	int32_t line;

	ip = 0; /* instruction pointer*/

	if(argc < 3 ) {
		fprintf(stderr, "Usage: ./vm.out <taskfile> <outputfile>\n");
		return 1;
	}

	fp = fopen(argv[1], "r");
	if(!fp ) {
		fprintf(stderr, "Unable to open file\n");
		return 1;
	}

	fscanf(fp, "%x", &data_size);
	//printf("%x\n", data_size); 
	
	fscanf(fp, "%x", &image_size);
//	printf("%x\n", data_size);

	sp = data_size; /*stack pointer  */

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

	if(argv[2] != NULL )
		of = fopen(argv[2], "w");
	else
		of = fopen("./vm.asm", "w");

	if(!of ) {
		fprintf(stderr, "Unable to open file\n");
		return 1;
	}

	//while(ip < image_size) {
	while(1) {
		struct instruction ins;
		uint32_t inst = data[ip];
		ip++;
		decode(&ins, inst);
	//	perform_action(ins); /* debug asm file generator */
		operations(ins);
	}

clear:
	free(data);
	fclose(fp);
	fclose(of);


        return 0;
}
