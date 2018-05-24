#ifndef DEFAULT_CONFIGS_H_
#define DEFAULT_CONFIGS_H_

#include "common/libpull.h"

static uint8_t x[32] = {
    0x8b,0x27,0x39,0x67,0x01,0x4b,0x1c,0xae,0xfe,0x8a,0x18,0x6e,0xea,0x27,0x86,0x34,
    0x0e,0xea,0x35,0x3d,0x8c,0x65,0xf6,0x59,0xfc,0xcb,0x23,0xd7,0xfa,0xab,0x7b,0x18
};
static uint8_t y[32] = {
    0x14,0x75,0x33,0xec,0x17,0xb7,0x54,0x50,0xca,0x98,0x35,0xad,0x58,0xbe,0xd5,0xfa,
    0x48,0xbc,0xa0,0x24,0x81,0xba,0xfa,0x3d,0xcd,0x8d,0x5a,0x7f,0x40,0xbc,0x70,0x94
};

static identity_t identity_g = {
    .udid = 0x1234,
    .random = 0x5678
};

#if defined(CONN_DTLS_ECDSA) || defined(CONN_DTLS_PSK)

#ifdef CONN_DTLS_ECDSA
#define SERVER_KEY_SIZE 32
/// XXX This is no more supported since it is not supported by Contiki-NG
static const uint8_t dtls_server_x_g[32] = {
    0xcf,0x9c,0xf2,0xf8,0x44,0x8b,0xe2,0x7f,0xff,0xc9,0xe1,0x69,0xdb,0x84,0x7c,0xdf,
    0xe6,0xd6,0xa9,0xd3,0x5b,0xbf,0xe9,0x99,0xc9,0x48,0x47,0xe9,0x5a,0x21,0x58,0xdb
};
static const uint8_t dtls_server_y_g[32] = {
    0x20,0x90,0x2f,0xeb,0xfe,0x7f,0x00,0x00,0x38,0x90,0x2f,0xeb,0xfe,0x7f,0x00,0x00,
    0xa0,0x8f,0x2f,0xeb,0xfe,0x7f,0x00,0x00,0xe8,0x90,0x2f,0xeb,0xfe,0x7f,0x00,0x00
};

int verify_key(uint8_t key_size, const uint8_t* server_pub_x, const uint8_t* server_pub_y) {
    if (key_size == SERVER_KEY_SIZE && 
        memcmp(server_pub_x, dtls_server_x_g, key_size) == 0 &&
        memcmp(server_pub_y, dtls_server_y_g, key_size) == 0) {
        return 1;
    }
    return 0;
}
static uint8_t priv[32] = {
    0x47,0x94,0x08,0xba,0x3c,0x18,0xcf,0xcc,0x21,0x6c,0x09,0x2b,0x81,0x57,0x66,0xfc,
    0x8f,0x74,0x4a,0x12,0x00,0x41,0x72,0xb7,0xca,0x9d,0xae,0x9b,0x40,0xc9,0x37,0xf9
};
static uint8_t pubx[32] = {
    0xa3,0x31,0x85,0x2a,0xe8,0x08,0x43,0xc0,0x48,0xd5,0x5e,0x03,0xd2,0x4a,0xc5,0x8c,
    0x0a,0xf0,0x00,0x85,0x99,0xe2,0x7f,0xec,0xc2,0x49,0xc8,0xe7,0x22,0xd3,0xff,0x37
};
static uint8_t puby[32] = {
    0x28,0xbe,0x69,0xbf,0xd8,0x1d,0xde,0x1c,0xab,0x12,0x35,0xb4,0x1c,0x39,0x19,0xf1,
    0x25,0xf9,0x29,0x87,0x6b,0x11,0xef,0xa3,0x4f,0x32,0xfc,0x0c,0xe8,0xc8,0x59,0xd8
};

static dtls_ecdsa_data_t dtls_ecdsa_data = {
    .curve = DTLS_CURVE_SECP256R1,
    .priv_key = priv,
    .pub_key_x = pubx,
    .pub_key_y = puby,
    .verify_key = verify_key
};

#endif /* CONN_DTLS_ECDSA */
#endif /* defined(CONN_DTLS_ECDSA) || defined(CONN_DTLS_PSK) */

#endif /* DEFAULT_CONFIGS_H_ */

