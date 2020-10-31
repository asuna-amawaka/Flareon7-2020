 //written by Asuna Amawaka for FlareOn7 10_-_break
 //uses bignum bn.c, bn.h from https://github.com/kokke/tiny-bignum-c
 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "bn.h"

/* O(log n) */
void pow_mod_faster(struct bn* a, struct bn* b, struct bn* n, struct bn* res)
{
	bignum_from_int(res, 1); /* r = 1 */

	struct bn tmpa;
	struct bn tmpb;
	struct bn tmp;
	bignum_assign(&tmpa, a);
	bignum_assign(&tmpb, b);

	while (1)
	{
		if (tmpb.array[0] & 1)     /* if (b % 2) */
		{
			bignum_mul(res, &tmpa, &tmp);  /*   r = r * a % m */
			bignum_mod(&tmp, n, res);
		}
		bignum_rshift(&tmpb, &tmp, 1); /* b /= 2 */
		bignum_assign(&tmpb, &tmp);

		if (bignum_is_zero(&tmpb))
			break;

		bignum_mul(&tmpa, &tmpa, &tmp);
		bignum_mod(&tmp, n, &tmpa);
		}
	}

//not sure if buggy, advise not to use
void modInverse(struct bn* a, struct bn* m, struct bn* c)
{
	struct bn x, tmp, result;
	bignum_init(&tmp);
	bignum_init(&result);

	bignum_mod(a, m, &result);
	bignum_assign(a, &result);
	bignum_init(&result);
	for (bignum_from_int(&x, 1); bignum_cmp(&x,m)==SMALLER; bignum_inc(&x)) {
		bignum_mul(a, &x, &tmp);
		bignum_mod(&tmp, m, &result);
		bignum_init(&tmp);
		bignum_from_int(&tmp, 1);
		if (bignum_cmp(&result,&tmp) == EQUAL) {
			bignum_assign(c, &x);
			return;
		}
	}
}

int getHexVal(char c)
{
	if(c >= '0' && c<= '9')
		return c - '0';
	else if(c >= 'a' && c<= 'f')
		return c - 'a' + 10;
	else if(c >= 'A' && c<= 'F')
		return c - 'A' + 10;
	else
		return -1;
}

void printAscii(char* hexStr)
{
	int hexLen = strlen(hexStr);
	char asciiStr[hexLen/2];
	for(int i = 0; i < hexLen; i +=2) {
		asciiStr[(hexLen-i)/2 - 1] = (getHexVal(hexStr[i])*16 + getHexVal(hexStr[i+1]));
	}
	asciiStr[hexLen/2] = 0x0;
	printf("%s",asciiStr);
}


void printBigNum(struct bn* bnum)
{
	char buf[8192];
	bignum_to_string(bnum, buf, sizeof(buf));
	int i = 0;
	while (buf[i] != 0) {
		printf("%c", buf[i]);
		i += 1;
	}
	printf("\n");
}

void main()
{
	char very_big_q_12A6[] = "d1cc3447d5a9e1e6adae92faaea8770db1fab16b1568ea13c3715f2aeba9d84f";
	char target1_g_11E3[] = "480022d87d1823880d9e4ef56090b54001d343720dd77cbc5bc5692be948236c";
	char h_1224[] = "c10357c7a53fa2f1ef4a5bf03a2d156039e7a57143000c8d8f45985aea41dd31";	
	char target2_c_1265[] = "d036c5d4e7eda23afceffbad4e087a48762840ebb18e3d51e4146f48c04697eb";
	//value of S^-1 calculated elsewhere
	char inv_S[] = "7d6990db0059850d8e02937be5e2ac7b9dfe6411de316c1e462762c24d647b5c";

	/*known*/
	struct bn q; /* very large number */
	struct bn g; /* generator */
	struct bn h; /* h = g^a mod q */
	struct bn c; /* cipher text */
	struct bn k; /* private key */

	/*calculated*/
	struct bn S;
	struct bn p;
	struct bn tmp;
	struct bn tmp2;
	struct bn result;
	struct bn i_S;

	bignum_init(&q);
	bignum_init(&g);
	bignum_init(&h);
	bignum_init(&c);

	bignum_from_string(&q, very_big_q_12A6, 64);
	bignum_from_string(&g, target1_g_11E3,  64);
	bignum_from_string(&h, h_1224,          64);
	bignum_from_string(&c, target2_c_1265,  64);
	bignum_from_string(&i_S, inv_S,  	64);
  	bignum_from_int(&k, 1);
	bignum_init(&tmp);
	bignum_init(&tmp2);
	bignum_init(&result);
	bignum_init(&S);
	bignum_init(&p);

	/*inspect strings*/
	printf("k = ");
	printBigNum(&k);
	printf("c = ");
	printBigNum(&c);
	printf("q = ");
	printBigNum(&q);
	printf("h = ");
	printBigNum(&h);
	printf("g = ");
	printBigNum(&g);

	/*start work*/
	pow_mod_faster(&h, &k, &q, &S);

	printf("S = ");
	printBigNum(&S);
	printf("S^-1 = ");
	printBigNum(&i_S);

	bignum_init(&tmp);
	bignum_init(&result);
	bignum_mul(&i_S, &S, &tmp);
	bignum_divmod(&tmp, &q, &tmp2, &result);
	printf("S^-1 * S (mod q) = ");
	printBigNum(&result);

	bignum_init(&tmp);
	bignum_init(&tmp2);

	bignum_mul(&i_S, &c, &tmp);
	bignum_divmod(&tmp, &q, &tmp2, &result);
	int buf[64];
	bignum_to_string(&result, buf, sizeof(buf));
	printf("S^-1 * c (mod q) = ");
	printBigNum(&result);

	printAscii(buf);
	printf("\n");

}


