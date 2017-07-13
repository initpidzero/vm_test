/*
** Author - anuz
** This file is distibuted under XYZ license 
**/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>

#define mask_binop 0x80000000
#define mask_operation 0x7F000000
#define mask_data 0x00FFFFFF

/* do we need them as globals? */
static int32_t sp;
static int32_t ip;

/* The objective of this program is to create a small instruction processing machine
 *
 *
 * */

struct instruction {
	uint32_t binop;
	uint32_t operation;
	uint32_t oper_data;

};

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

int f(int32_t v, int32_t *data)
{
	sp = sp - 1;
	data[sp] = v;
}

int g(int32_t *data,)
{
	int32_t v = data[sp]
	sp++;
	return v;
}

int decode(struct instruction ins, int32_t data)
{
	printf("data = %x\n", data);
	disp_bin(data);
	ins.binop = (data & mask_binop);
	printf("binop %x\n", ins.binop);
	disp_bin(ins.binop);
	ins.binop = ins.binop >> 31;
	disp_bin(ins.binop);
	ins.operation = (data & mask_operation);
	disp_bin(ins.operation);
	printf("operation %x\n", ins.operation);
	ins.operation = ins.operation >> 24;
	disp_bin(ins.operation);
	ins.oper_data = data & mask_data;

	return 0;
}

int perform_action()
{
}

int operations(int binop, int op)
{
	switch(binop) {
	case 0:
		switch(op) {
		case 0x0:
		case 0x1:
		case 0x2:
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x6:
		case 0x7:
		case 0x8:
		case 0x9:
		case 0xa:
		break;
		}
	case 1:
		switch(op) {
		case 0x0:
		case 0x1:
		case 0x2:
		case 0x3:
		case 0x4:
		case 0x5:
		case 0x6:
		case 0x7:
		case 0x8:
		break;
		}
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
	int32_t *data;
	int32_t image_size;
	int ret; /* keep a status variable for return values */
	unsigned int i;
	unsigned int actual_bytes = 0;
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
	fscanf(fp, "%x", &data_size);
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
	for(i = 0; i < data_size; i++) {
		/* should check for EOF */
		if(fscanf(fp, "%x", &image_size) == EOF)
			break;
		data[i] = image_size;
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
		//uint32_t binop;
		//uint32_t operation;	
		//uint32_t oper_data;
		struct instruction ins;
		decode(ins, data[i]);
		}
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
	int32_t *data;
	int32_t image_size;
	int ret; /* keep a status variable for return values */
	unsigned int i;
	unsigned int actual_bytes = 0;
	uint32_t ip = 0;
	uint32_t sp;

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
	fscanf(fp, "%x", &data_size);
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
	for(i = 0; i < data_size; i++) {
		/* should check for EOF */
		if(fscanf(fp, "%x", &image_size) == EOF)
			break;
		data[i] = image_size;
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
		//uint32_t binop;
		//uint32_t operation;	
		//uint32_t oper_data;
		struct instruction ins;
		decode(ins, data[i]);
		printf("binop = %x, opeation = %x, oper_data = %x\n", ins.binop,ins.operation, ins.oper_data);

	}


clear:
	free(data);
	fclose(fp);

        return 0;
}
