#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <gmp.h>

#include "crypto.h"

#define RSA_PUBLIC_KEY  "1a29327a5795c0d26d5b12e1dc08a60ca031914f4d2e0f89641c6401c1e6d611b52ef83ba132141e4d01b7f4ae95d3ad37b6e2c2f6ad1d4a4f31e60638137457"
#define RSA_MOD_HEX     "1d96575073ac201e5e870f2083c674e0ab4e6e4306479fd9dfc2f6d73aefbe2e6f62d1deb2838825202bdd5e46ce2ecdf01932874927ed6e9ccbdd7347def877"

bool crypto_ctor(struct rsa_crypto *crypto)
{
  buffer_ctor(&crypto->buff);

  /* make encryption keys */
  mpz_init(crypto->encrptkey);
  mpz_set_str(crypto->encrptkey, RSA_PUBLIC_KEY, 16);
  mpz_init(crypto->encrptmod);
  mpz_set_str(crypto->encrptmod, RSA_MOD_HEX, 16);
  return true;
}

void crypto_dtor(struct rsa_crypto *crypto)
{
  mpz_clear(crypto->encrptkey);
  mpz_clear(crypto->encrptmod);
  buffer_dtor(&crypto->buff);
}

void encryptPacketWithKeys(struct rsa_crypto *crypto)
{
  /* copy data to package of smallest required length */
  size_t len = get_used(&crypto->buff);
  char *pkt = (char *) calloc(len, sizeof(char));
  get_data(&crypto->buff, pkt);

  /* make the encrypted data */
  mpz_t bigint_raw;
  mpz_init(bigint_raw);
  mpz_import(bigint_raw, len, 1, sizeof(char), 0, 0, pkt);
  free(pkt);
  
  mpz_t bigint_enc;
  mpz_init(bigint_enc);
  mpz_powm(bigint_enc, bigint_raw, crypto->encrptkey, crypto->encrptmod);
  mpz_clear(bigint_raw);

  /* convert enctrypted data to char* */
  size_t len_enc = 0;
  char *pkt_enc = (char *) mpz_export(NULL, &len_enc, 1, sizeof(char), 0, 0, bigint_enc);
  mpz_clear(bigint_enc);

  /* write to packet array */
  reset(&crypto->buff);
  put_1_byte(&crypto->buff, (uint8_t)len_enc);
  put_data(&crypto->buff, pkt_enc, len_enc);
  free(pkt_enc);
}
