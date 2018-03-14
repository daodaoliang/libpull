#include "memory/simple_manifest.h"
#include "memory/manifest.h"
#include "common/libpull.h"
#include <string.h>

#ifdef SIMPLE_MANIFEST

version_t get_version_impl(const manifest_t* mt) {
    return mt->vendor.version;
}
platform_t get_platform_impl(const manifest_t* mt) {
    return mt->vendor.platform;
}
address_t get_size_impl(const manifest_t* mt) {
    return mt->vendor.size;
}
address_t get_offset_impl(const manifest_t* mt) {
    return mt->vendor.offset;
}
uint8_t* get_digest_impl(const manifest_t* mt) {
    return (uint8_t*) mt->vendor.digest;
}
uint8_t* get_server_key_x_impl(const manifest_t* mt) {
    return (uint8_t*) mt->vendor.server_key_x;
}
uint8_t* get_server_key_y_impl(const manifest_t* mt) {
    return (uint8_t*) mt->vendor.server_key_y;
}

void set_version_impl(manifest_t* mt, version_t version) {
    mt->vendor.version = version;
}
void set_platform_impl(manifest_t* mt, platform_t platform) {
    mt->vendor.platform = platform;
}
void set_size_impl(manifest_t* mt, address_t size) {
    mt->vendor.size = size;
}
void set_offset_impl(manifest_t* mt, address_t offset) {
    mt->vendor.offset = offset;
}
void set_digest_impl(manifest_t* mt, uint8_t* digest) {
    memcpy(mt->vendor.digest, digest, 32); // XXX hardcoded value
}
void set_server_key_x_impl(manifest_t* mt, uint8_t* server_key_x) {
    memcpy(mt->vendor.server_key_x, server_key_x, 32); // XXX hardcoded value
}
void set_server_key_y_impl(manifest_t* mt, uint8_t* server_key_y) {
    memcpy(mt->vendor.server_key_y, server_key_y, 32); // XXX hardcoded value
}

uint8_t* get_vendor_signature_r_impl(const manifest_t *mt, uint8_t* size) {
    *size = 32;
    return (uint8_t*) (mt->vendor_signature_r);
}
uint8_t* get_vendor_signature_s_impl(const manifest_t *mt, uint8_t* size) {
    *size = 32;
    return (uint8_t*) (mt->vendor_signature_s);
}
uint8_t* get_server_signature_r_impl(const manifest_t *mt, uint8_t* size) {
    *size = 32;
    return (uint8_t*) mt->server_signature_r;
}
uint8_t* get_server_signature_s_impl(const manifest_t *mt, uint8_t* size) {
    *size = 32;
    return (uint8_t*) mt->server_signature_s;
}


void set_vendor_signature_r_impl(manifest_t *mt, uint8_t* vendor_signature_r, uint8_t *size) {
    memcpy(mt->vendor_signature_r, vendor_signature_r, *size);
}
void set_vendor_signature_s_impl(manifest_t *mt, uint8_t* vendor_signature_s, uint8_t *size) {
    memcpy(mt->vendor_signature_s, vendor_signature_s, *size);
}
void set_server_signature_r_impl(manifest_t *mt, uint8_t* server_signature_r, uint8_t *size) {
    memcpy(mt->server_signature_r, server_signature_r, *size);
}
void set_server_signature_s_impl(manifest_t *mt, uint8_t* server_signature_s, uint8_t *size) {
    memcpy(mt->server_signature_s, server_signature_s, *size);
}

static pull_error verify_data_impl(uint8_t* data, digest_func f, const uint8_t *pub_x,
                     const uint8_t *pub_y, ecc_curve curve, uint8_t* r, uint8_t* s) {
    digest_ctx ctx;
    if (f.init(&ctx) != PULL_SUCCESS) {
        return GENERIC_ERROR; // TODO specialize error
    }
    f.update(&ctx, data, sizeof(vendor_manifest_t));
    uint8_t* hash = (uint8_t*) f.finalize(&ctx);
    if (ecc_verify(pub_x, pub_y, r, s, hash, f.size, curve) != PULL_SUCCESS) {
        return GENERIC_ERROR;
    }
    return PULL_SUCCESS;
}

pull_error verify_manifest_vendor_impl(manifest_t* mt, digest_func f, const uint8_t *pub_x,
                    const uint8_t *pub_y, ecc_curve curve) {
    return verify_data_impl((uint8_t*) &mt->vendor, f, pub_x, pub_y, curve, mt->vendor_signature_r,
            mt->vendor_signature_s); 
}

pull_error verify_manifest_server_impl(manifest_t* mt, digest_func f, const uint8_t *pub_x,
        const uint8_t *pub_y, ecc_curve curve) {
    return verify_data_impl((uint8_t*) &mt->server, f, pub_x, pub_y, curve, mt->server_signature_r,
            mt->server_signature_s);
}

static pull_error sign_data_impl(uint8_t* data, digest_func f, const uint8_t *private_key,
                                    uint8_t* signature_buffer, ecc_curve curve) {
    digest_ctx ctx;
    pull_error err = f.init(&ctx);
    if (err) {
        return GENERIC_ERROR; // TODO specialize error
    }
    f.update(&ctx, data, sizeof(vendor_manifest_t));
    uint8_t* hash = (uint8_t*) f.finalize(&ctx);
    if (ecc_sign(private_key, signature_buffer, hash, f.size, curve) != PULL_SUCCESS) {
        return GENERIC_ERROR;
    }
    return PULL_SUCCESS;
}

pull_error sign_manifest_vendor_impl(manifest_t* mt, digest_func f, const uint8_t *private_key,
                                   uint8_t* signature_buffer, ecc_curve curve) {
    pull_error err = sign_data_impl((uint8_t*) &mt->vendor, f, private_key, signature_buffer, curve);
    set_vendor_signature_r_impl(mt, signature_buffer, &curve.curve_size);
    set_vendor_signature_s_impl(mt, signature_buffer+curve.curve_size, &curve.curve_size);
    return err;
}

pull_error sign_manifest_server_impl(manifest_t* mt, digest_func f, const uint8_t *private_key,
                                   uint8_t* signature_buffer, ecc_curve curve) {
    pull_error err = sign_data_impl((uint8_t*) &mt->server, f, private_key, signature_buffer, curve);
    set_server_signature_r_impl(mt, signature_buffer, &curve.curve_size);
    set_server_signature_s_impl(mt, signature_buffer+curve.curve_size, &curve.curve_size);
    return err;
}

void print_manifest_impl(const manifest_t* mt) {
    log_info("Platform: %04x\n", mt->vendor.platform);
    log_info("Version: %04x\n", mt->vendor.version);
    log_info("Size: %d\n", (int) mt->vendor.size);
}

#endif /* SIMPLE_MANIFEST */