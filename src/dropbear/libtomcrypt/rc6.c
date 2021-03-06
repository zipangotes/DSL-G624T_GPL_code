#include "mycrypt.h"

#ifdef RC6

const struct _cipher_descriptor rc6_desc =
{
    "rc6",
    3,
    8, 128, 16, 20,
    &rc6_setup,
    &rc6_ecb_encrypt,
    &rc6_ecb_decrypt,
    &rc6_test,
    &rc6_keysize
};

static const ulong32 stab[44] = {
0xb7e15163UL, 0x5618cb1cUL, 0xf45044d5UL, 0x9287be8eUL, 0x30bf3847UL, 0xcef6b200UL, 0x6d2e2bb9UL, 0x0b65a572UL,
0xa99d1f2bUL, 0x47d498e4UL, 0xe60c129dUL, 0x84438c56UL, 0x227b060fUL, 0xc0b27fc8UL, 0x5ee9f981UL, 0xfd21733aUL,
0x9b58ecf3UL, 0x399066acUL, 0xd7c7e065UL, 0x75ff5a1eUL, 0x1436d3d7UL, 0xb26e4d90UL, 0x50a5c749UL, 0xeedd4102UL,
0x8d14babbUL, 0x2b4c3474UL, 0xc983ae2dUL, 0x67bb27e6UL, 0x05f2a19fUL, 0xa42a1b58UL, 0x42619511UL, 0xe0990ecaUL,
0x7ed08883UL, 0x1d08023cUL, 0xbb3f7bf5UL, 0x5976f5aeUL, 0xf7ae6f67UL, 0x95e5e920UL, 0x341d62d9UL, 0xd254dc92UL,
0x708c564bUL, 0x0ec3d004UL, 0xacfb49bdUL, 0x4b32c376UL };

#ifdef CLEAN_STACK
static int _rc6_setup(const unsigned char *key, int keylen, int num_rounds, symmetric_key *skey)
#else
int rc6_setup(const unsigned char *key, int keylen, int num_rounds, symmetric_key *skey)
#endif
{
    ulong32 L[64], S[50], A, B, i, j, v, s, l;

    _ARGCHK(key != NULL);
    _ARGCHK(skey != NULL);

    /* test parameters */
    if (num_rounds != 0 && num_rounds != 20) { 
       return CRYPT_INVALID_ROUNDS;
    }

    /* key must be between 64 and 1024 bits */
    if (keylen < 8 || keylen > 128) {
       return CRYPT_INVALID_KEYSIZE;
    }

    /* copy the key into the L array */
    for (A = i = j = 0; i < (ulong32)keylen; ) { 
        A = (A << 8) | ((ulong32)(key[i++] & 255));
        if (!(i & 3)) {
           L[j++] = BSWAP(A);
           A = 0;
        }
    }

    /* handle odd sized keys */
    if (keylen & 3) { 
       A <<= (8 * (4 - (keylen&3))); 
       L[j++] = BSWAP(A); 
    }

    /* setup the S array */
    memcpy(S, stab, 44 * sizeof(stab[0]));

    /* mix buffer */
    s = 3 * MAX(44, j);
    l = j;
    for (A = B = i = j = v = 0; v < s; v++) { 
        A = S[i] = ROL(S[i] + A + B, 3);
        B = L[j] = ROL(L[j] + A + B, (A+B));
        if (++i == 44) { i = 0; }
        if (++j == l)  { j = 0; }
    }
    
    /* copy to key */
    for (i = 0; i < 44; i++) { 
        skey->rc6.K[i] = S[i];
    }
    return CRYPT_OK;
}

#ifdef CLEAN_STACK
int rc6_setup(const unsigned char *key, int keylen, int num_rounds, symmetric_key *skey)
{
   int x;
   x = _rc6_setup(key, keylen, num_rounds, skey);
   burn_stack(sizeof(ulong32) * 122);
   return x;
}
#endif

#ifdef CLEAN_STACK
static void _rc6_ecb_encrypt(const unsigned char *pt, unsigned char *ct, symmetric_key *key)
#else
void rc6_ecb_encrypt(const unsigned char *pt, unsigned char *ct, symmetric_key *key)
#endif
{
   ulong32 a,b,c,d,t,u, *K;
   int r;
   
   _ARGCHK(key != NULL);
   _ARGCHK(pt != NULL);
   _ARGCHK(ct != NULL);
   LOAD32L(a,&pt[0]);LOAD32L(b,&pt[4]);LOAD32L(c,&pt[8]);LOAD32L(d,&pt[12]);

   b += key->rc6.K[0];
   d += key->rc6.K[1];

#define RND(a,b,c,d) \
       t = (b * (b + b + 1)); t = ROL(t, 5); \
       u = (d * (d + d + 1)); u = ROL(u, 5); \
       a = ROL(a^t,u) + K[0];                \
       c = ROL(c^u,t) + K[1]; K += 2;   
    
   K = key->rc6.K + 2;
   for (r = 0; r < 20; r += 4) {
       RND(a,b,c,d);
       RND(b,c,d,a);
       RND(c,d,a,b);
       RND(d,a,b,c);
   }
   
#undef RND

   a += key->rc6.K[42];
   c += key->rc6.K[43];
   STORE32L(a,&ct[0]);STORE32L(b,&ct[4]);STORE32L(c,&ct[8]);STORE32L(d,&ct[12]);
}

#ifdef CLEAN_STACK
void rc6_ecb_encrypt(const unsigned char *pt, unsigned char *ct, symmetric_key *key)
{
   _rc6_ecb_encrypt(pt, ct, key);
   burn_stack(sizeof(ulong32) * 6 + sizeof(int));
}
#endif

#ifdef CLEAN_STACK
static void _rc6_ecb_decrypt(const unsigned char *ct, unsigned char *pt, symmetric_key *key)
#else
void rc6_ecb_decrypt(const unsigned char *ct, unsigned char *pt, symmetric_key *key)
#endif
{
   ulong32 a,b,c,d,t,u, *K;
   int r;

   _ARGCHK(key != NULL);
   _ARGCHK(pt != NULL);
   _ARGCHK(ct != NULL);
   
   LOAD32L(a,&ct[0]);LOAD32L(b,&ct[4]);LOAD32L(c,&ct[8]);LOAD32L(d,&ct[12]);
   a -= key->rc6.K[42];
   c -= key->rc6.K[43];
   
#define RND(a,b,c,d) \
       t = (b * (b + b + 1)); t = ROL(t, 5); \
       u = (d * (d + d + 1)); u = ROL(u, 5); \
       c = ROR(c - K[1], t) ^ u; \
       a = ROR(a - K[0], u) ^ t; K -= 2;
   
   K = key->rc6.K + 40;
   
   for (r = 0; r < 20; r += 4) {
       RND(d,a,b,c);
       RND(c,d,a,b);
       RND(b,c,d,a);
       RND(a,b,c,d);
   }
   
#undef RND

   b -= key->rc6.K[0];
   d -= key->rc6.K[1];
   STORE32L(a,&pt[0]);STORE32L(b,&pt[4]);STORE32L(c,&pt[8]);STORE32L(d,&pt[12]);
}

#ifdef CLEAN_STACK
void rc6_ecb_decrypt(const unsigned char *ct, unsigned char *pt, symmetric_key *key)
{
   _rc6_ecb_decrypt(ct, pt, key);
   burn_stack(sizeof(ulong32) * 6 + sizeof(int));
}
#endif

int rc6_test(void)
{
 #ifndef LTC_TEST
    return CRYPT_NOP;
 #else    
   static const struct {
       int keylen;
       unsigned char key[32], pt[16], ct[16];
   } tests[] = {
   {
       16,
       { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
         0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
       { 0x02, 0x13, 0x24, 0x35, 0x46, 0x57, 0x68, 0x79,
         0x8a, 0x9b, 0xac, 0xbd, 0xce, 0xdf, 0xe0, 0xf1 },
       { 0x52, 0x4e, 0x19, 0x2f, 0x47, 0x15, 0xc6, 0x23,
         0x1f, 0x51, 0xf6, 0x36, 0x7e, 0xa4, 0x3f, 0x18 }
   },
   {
       24,
       { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
         0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
         0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0,
         0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
       { 0x02, 0x13, 0x24, 0x35, 0x46, 0x57, 0x68, 0x79,
         0x8a, 0x9b, 0xac, 0xbd, 0xce, 0xdf, 0xe0, 0xf1 },
       { 0x68, 0x83, 0x29, 0xd0, 0x19, 0xe5, 0x05, 0x04,
         0x1e, 0x52, 0xe9, 0x2a, 0xf9, 0x52, 0x91, 0xd4 }
   },
   {
       32,
       { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
         0x01, 0x12, 0x23, 0x34, 0x45, 0x56, 0x67, 0x78,
         0x89, 0x9a, 0xab, 0xbc, 0xcd, 0xde, 0xef, 0xf0,
         0x10, 0x32, 0x54, 0x76, 0x98, 0xba, 0xdc, 0xfe },
       { 0x02, 0x13, 0x24, 0x35, 0x46, 0x57, 0x68, 0x79,
         0x8a, 0x9b, 0xac, 0xbd, 0xce, 0xdf, 0xe0, 0xf1 },
       { 0xc8, 0x24, 0x18, 0x16, 0xf0, 0xd7, 0xe4, 0x89,
         0x20, 0xad, 0x16, 0xa1, 0x67, 0x4e, 0x5d, 0x48 }
   }
   };
   unsigned char buf[2][16];
   int x, err;
   symmetric_key key;

   for (x  = 0; x < (int)(sizeof(tests) / sizeof(tests[0])); x++) {
      /* setup key */
      if ((err = rc6_setup(tests[x].key, tests[x].keylen, 0, &key)) != CRYPT_OK) {
         return err;
      }

      /* encrypt and decrypt */
      rc6_ecb_encrypt(tests[x].pt, buf[0], &key);
      rc6_ecb_decrypt(buf[0], buf[1], &key);

      /* compare */
      if (memcmp(buf[0], tests[x].ct, 16) || memcmp(buf[1], tests[x].pt, 16)) {
         return CRYPT_FAIL_TESTVECTOR;
      }
   }
   return CRYPT_OK;
  #endif
}

int rc6_keysize(int *desired_keysize)
{
   _ARGCHK(desired_keysize != NULL);
   if (*desired_keysize < 8) {
      return CRYPT_INVALID_KEYSIZE;
   } else if (*desired_keysize > 128) {
      *desired_keysize = 128;
   }
   return CRYPT_OK;
}

#endif /*RC6*/


