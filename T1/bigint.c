/* GUILHERME BERGER 1210518 */

#include "bigint.h"
#define INT_BYTES 4

/* Funções auxiliares não exportadas */

// res = a
static void big_copy (BigInt res, BigInt a){
	int i;
	for(i=0; i<NUM_BYTES; i++){
		res[i] = a[i];
	}
}

// res = 0
static void big_zero (BigInt res){
	int i;
	for(i=0; i<NUM_BYTES; i++){
		res[i] = 0;
	}
}

// res = ~res
static void big_not (BigInt res){
	int i;
	for(i=0; i<NUM_BYTES; i++){
		res[i] = ~res[i];
	}
}

// res = ~res + 1
// (in mathematical terms, the symmetric)
// (in bit terms, the two's complement)
static void big_symmetric (BigInt res){
	BigInt one;
	big_val(one, 1);

	big_not(res);
	big_sum(res, res, one);
}

// find out the signal of a BigInt, when interpreted as signed
// returns 1 for positive, -1 for negative
static int big_sig (BigInt a) {
	int last_byte = a[NUM_BYTES-1] & 0x80;
	return last_byte ? -1 : 1;
}

/* Atribuição */

/* res = val (extensão com sinal) */
void big_val (BigInt res, int val){
	int i;

	// first, convert the val as if it were unsigned
	big_uval(res, val);

	// next, find out if it's negative, and if so, fill the BigInt with 0xff bytes	
	if(res[INT_BYTES-1] >> 7){
		for(i=INT_BYTES; i<NUM_BYTES; i++){
			res[i] = 0xff;
		}
	}
}

/* res = uval (extensão sem sinal) */
void big_uval (BigInt res, unsigned int uval){
	int i;

	// first, let's make sure our result buffer is clean
	big_zero(res);

	// then, just get every byte in uval and write to the corresponding byte in our BigInt
	for(i=0; i<INT_BYTES; i++){
		res[i] = uval & 0xff;
		uval >>= 8;
	}
}


/* Operacoes aritméticas */

/* res = a + b */
void big_sum (BigInt res, BigInt a, BigInt b){
	int i;
	// a short can store safely the sum of two chars
	unsigned short buffer = 0;

	for(i=0; i<NUM_BYTES; i++){
		buffer += a[i] + b[i];

		// get the first byte of the buffer and assign it to the current BigInt byte
		res[i] = buffer & 0xff;

		// shift the buffer a byte so the carry is stored for the next byte sum
		buffer >>= 8;
	}	
}

/* res = a - b */
void big_sub (BigInt res, BigInt a, BigInt b){
	// a-b = a+(comp(b)) = a+(~b+1)
	BigInt b_comp;
	big_copy(b_comp, b);
	big_symmetric(b_comp);
	big_sum(res, a, b_comp);
}

/* res = a * b (sem sinal) */
void big_umul (BigInt res, BigInt a, BigInt b) {
	int i, j;
	unsigned short buffer;
	BigInt temp1, temp2;

	big_zero(temp2);

	for(i = 0; i < NUM_BYTES; i++) {
		big_zero(temp1);
		buffer = 0;

		for(j = 0; i+j < NUM_BYTES; j++) {
			buffer += a[i] * b[j];
			temp1[i+j] = buffer & 0xFF;
			buffer >>= 8;
		}

		big_sum(temp2, temp2, temp1);
	}
	big_copy(res, temp2);
}

/* res = a * b (com sinal) */
void big_mul (BigInt res, BigInt a, BigInt b) {
	int sig = 1;
	BigInt tempa;
	BigInt tempb;

	// a*b = |a| * |b| * sig(a) * sig(b)

	// make copies because we may need to modify them
	big_copy(tempa, a);
	big_copy(tempb, b);

	//let's multiply positive numbers, and keep track of the resulting signal
	if(big_sig(tempa) == -1) {
		big_symmetric(tempa);
		sig *= -1;
	}

	if(big_sig(tempb) == -1) {
		big_symmetric(tempb);
		sig *= -1;
	}

	big_umul(res, tempa, tempb);

	// if we have a negative signal, apply it now!
	if(sig == -1) {
		big_symmetric(res);
	}
}


/* Operacoes de deslocamento */

/* res = a << n */
void big_shl (BigInt res, BigInt a, int n){
	int i, subshift, index_offset;
	int carry = 0;
	BigInt temp;

	// first, we do a subshift, shifting by 0<subshift<8
	subshift = n%8;
	if(subshift != 0){
		// if n isn't a multiple of 8, shift left by n%8 and adjust n
		for(i=0; i<NUM_BYTES; i++){
			temp[i] = (a[i] << subshift) + carry;
			carry = a[i] >> (8 - subshift);
		}
		n = n - subshift;
	} else {
		big_copy(temp, a);
	}

	// now n is a multiple of 8, we don't need to change the insides of each char
	index_offset = n/8;
	for(i=0; i<NUM_BYTES; i++){
		if(i - index_offset >= 0) {
			res[i] = temp[i - index_offset];
		} else {
			res[i] = 0;
		}
	}
}

/* res = a >> n (lógico) */
void big_shr (BigInt res, BigInt a, int n){
	int i, subshift, index_offset;
	int carry = 0;
	BigInt temp;

	// first, we do a subshift, shifting by 0<subshift<8
	subshift = n%8;
	if(subshift != 0){
		// if n isn't a multiple of 8, shift right by n%8 and adjust n
		for(i=NUM_BYTES-1; i>=0; i--){
			temp[i] = (a[i] >> subshift) + (carry << (8 - subshift));
			carry = a[i] & ((2 << subshift) - 1);
		}
		n = n - subshift;
	} else {
		big_copy(temp, a);
	}

	// now n is a multiple of 8, we don't need to change the insides of each char
	index_offset = n/8;
	for(i=0; i<NUM_BYTES; i++){
		if(i + index_offset < NUM_BYTES) {
			res[i] = temp[i + index_offset];
		} else {
			res[i] = 0;
		}
	}
}



/* Comparação: retorna -1 (a < b), 0 (a == b), 1 (a > b) */

/* comparação com sinal */
int big_cmp (BigInt a, BigInt b){
	int siga, sigb;

	// first, find out their signals
	siga = big_sig(a);
	sigb = big_sig(b);

	if(siga != sigb){
		// if they have different signals, the result is obvious
		return siga;
	} else  {
		// if they have the same signal, we can just use big_ucmp
		return big_ucmp(a, b);
	}
}

/* comparação sem sinal */
int big_ucmp (BigInt a, BigInt b){
	int i;
	for(i=NUM_BYTES-1; i>=0; i--){
		if(a[i] < b[i]){
			return -1;
		} else if(a[i] > b[i]){
			return 1;
		}
	}
	return 0;
}
