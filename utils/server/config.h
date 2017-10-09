#ifndef CONFIG_H_
#define CONFIG_H_

#ifndef COAP_SERVER_PORT
#define COAP_SERVER_PORT 5683
#endif

#ifndef COAPS_SERVER_PORT
#define COAPS_SERVER_PORT 5684
#endif

#define MAX_ADDR_LEN 32
#define MAX_PATH_LEN 128

#define DEFAULT_FIRMWARE_NAME "fake_firmware_dead"

#ifndef WITH_TESTING_RESOURCES
#define WITH_TESTING_RESOURCES 1
#endif

typedef struct psk_key_t {
    char* identity;
    unsigned char* key;
    size_t len;
} psk_key_t;

static const psk_key_t psk_key = {
    .identity = "CoAP",
    .key = (unsigned char*) "ThisIsOurSecret1",
    .len = 16
};

#define DEFAULT_CURVE SECP256R1
#define CURVE_LEN 32
static const unsigned char ecdsa_priv_key[CURVE_LEN] = {
    0xD9,0xE2,0x70,0x7A,0x72,0xDA,0x6A,0x05,
    0x04,0x99,0x5C,0x86,0xED,0xDB,0xE3,0xEF,
    0xC7,0xF1,0xCD,0x74,0x83,0x8F,0x75,0x70,
    0xC8,0x07,0x2D,0x0A,0x76,0x26,0x1B,0xD4
};
static const unsigned char ecdsa_pub_key_x[CURVE_LEN] = {
    0xD0,0x55,0xEE,0x14,0x08,0x4D,0x6E,0x06,
    0x15,0x59,0x9D,0xB5,0x83,0x91,0x3E,0x4A,
    0x3E,0x45,0x26,0xA2,0x70,0x4D,0x61,0xF2,
    0x7A,0x4C,0xCF,0xBA,0x97,0x58,0xEF,0x9A
};
static const unsigned char ecdsa_pub_key_y[CURVE_LEN] = {
    0xB4,0x18,0xB6,0x4A,0xFE,0x80,0x30,0xDA,
    0x1D,0xDC,0xF4,0xF4,0x2E,0x2F,0x26,0x31,
    0xD0,0x43,0xB1,0xFB,0x03,0xE2,0x2F,0x4D,
    0x17,0xDE,0x43,0xF9,0xF9,0xAD,0xEE,0x70
};
static const unsigned char client_x[CURVE_LEN] = {
    0x8b,0x27,0x39,0x67,0x01,0x4b,0x1c,0xae,
    0xfe,0x8a,0x18,0x6e,0xea,0x27,0x86,0x34,
    0x0e,0xea,0x35,0x3d,0x8c,0x65,0xf6,0x59,
    0xfc,0xcb,0x23,0xd7,0xfa,0xab,0x7b,0x18
};
static const unsigned char client_y[CURVE_LEN] = {
    0x14,0x75,0x33,0xec,0x17,0xb7,0x54,0x50,
    0xca,0x98,0x35,0xad,0x58,0xbe,0xd5,0xfa,
    0x48,0xbc,0xa0,0x24,0x81,0xba,0xfa,0x3d,
    0xcd,0x8d,0x5a,0x7f,0x40,0xbc,0x70,0x94
};

#endif
