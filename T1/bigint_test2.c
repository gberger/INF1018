#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bigint.h"

long long ERRORS = 0;
char buffer[1024];

int rand_neg(){
	int r = rand() * RAND_MAX + rand();
	if(rand() % 2){
		r *= -1;
	}
	return r;
}

void big_dump(BigInt b, int newline){
	int i;
	printf("0x");
	for(i = NUM_BYTES - 1; i>=0; i--){
		printf("%02x", b[i]);
	}
	if(newline) printf("\n");
}

int big2int(BigInt b){
	return b[0] + (b[1] << 8) + (b[2] << 16) + (b[3] << 24);
}

int it2(char *msg, int a, int b){
	int bool = a != b;
	if(bool){
		ERRORS++;
		printf("ERROR: expected %d, got %d.", a, b);
		if(*msg){
			printf(" [%s]", msg);
		}
		printf("\n");
	}

	return bool;
}

int it(char *msg, int a, BigInt b){
	return it2(msg, a, big2int(b));
}

void test_val(){
	int i = rand_neg();
	BigInt b;

	big_val(b, i);
	it("val", i, b);
}

void test_uval(){
	unsigned int i = rand_neg();
	BigInt b;
	sprintf(buffer, "uval %d", i);

	big_uval(b, i);
	it(buffer, i, b);
}

void test_sum(){
	int a = rand_neg(), b = rand_neg();
	BigInt ba, bb, res;
	sprintf(buffer, "sum %d + %d", a, b);

	big_val(ba, a);
	big_val(bb, b);
	big_sum(res, ba, bb);
	it(buffer, a+b, res);
}

void test_sub(){
	int a = rand_neg(), b = rand_neg();
	BigInt ba, bb, res;
	sprintf(buffer, "sub %d - %d", a, b);

	big_val(ba, a);
	big_val(bb, b);
	big_sub(res, ba, bb);
	it(buffer, a-b, res);
}

void test_mul(){
	int a = rand_neg(), b = rand_neg();
	BigInt ba, bb, res;
	sprintf(buffer, "mul %d * %d", a, b);

	big_val(ba, a);
	big_val(bb, b);
	big_mul(res, ba, bb);
	it(buffer, a*b, res);
}

void test_umul(){
	unsigned int a = rand_neg(), b = rand_neg();
	BigInt ba, bb, res;
	sprintf(buffer, "umul %d * %d", a, b);

	big_uval(ba, a);
	big_uval(bb, b);
	big_umul(res, ba, bb);
	it(buffer, a*b, res);
}

void test_shl(){
	int a = rand_neg(), sh = rand() % 33;
	BigInt ba, res;
	sprintf(buffer, "shl %d << %d", a, sh);

	big_val(ba, a);
	big_shl(res, ba, sh);
	it(buffer, a<<sh, res);
}

void test_shr(){
	int a = rand_neg(), sh = rand() % 33;
	BigInt ba, res;
	sprintf(buffer, "shr %d >> %d", a, sh);

	big_val(ba, a);
	big_shr(res, ba, sh);
	it(buffer, a>>sh, res);
}

int cmp(int a, int b){
	return a < b ? -1 : (a == b ? 0 : 1);
}

void test_cmp(){
	int a = rand_neg(), b = rand_neg();
	BigInt ba, bb;
	sprintf(buffer, "cmp %d %d", a, b);

	if(rand() % 5 == 0){
		b = a;
	}

	big_val(ba, a);
	big_val(bb, b);
	it2(buffer, cmp(a,b), big_cmp(ba, bb));
}

int ucmp(unsigned int a, unsigned int b){
	return a < b ? -1 : (a == b ? 0 : 1);
}

void test_ucmp(){
	unsigned int a = rand_neg(), b = rand_neg();
	BigInt ba, bb;
	sprintf(buffer, "cmp %d %d", a, b);

	if(rand() % 5 == 0){
		b = a;
	}

	big_uval(ba, a);
	big_uval(bb, b);
	it2(buffer, ucmp(a, b), big_ucmp(ba, bb));
}

int main(void){ 
	time_t beginning, now;
	int how_long;
	int i = 0;
	int mill = 0;

	printf("For how many seconds? ");
	scanf("%d", &how_long);

	beginning = time(NULL);
	srand(beginning);

	do {
		now = time(NULL);

		test_val();
		test_uval();
		test_sum();
		test_sub();
		test_mul();
		test_umul();
		test_shl();
		test_shr();
		test_cmp();
		test_ucmp();

		test_etc();

		i++;
		if(i > 1000000) {
			i = 0;
			mill++;
			printf(".");
		}

	} while(difftime(now, beginning) < how_long);

	printf("\nRAN %d MILLION TIMES", mill);
	printf("\nTOTAL ERRORS: %d\n", ERRORS);
}