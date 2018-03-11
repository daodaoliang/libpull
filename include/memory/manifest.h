/** \file manifest.h
 * \brief Manifest identifying a memory object.
 * \author Antonio Langiu
 * \defgroup Memory
 * \{
 */
#ifndef MANIFEST_H_
#define MANIFEST_H_

#include "common/error.h"
#include "common/types.h"
#include "memory/simple_manifest.h"
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define FOREACH_ITEM(ITEM) \
    ITEM(version_t, version) \
    ITEM(platform_t, platform) \
    ITEM(address_t, size) \
    ITEM(address_t, offset) \
    ITEM(uint8_t*, server_key_x) \
    ITEM(uint8_t*, server_key_y) \
    ITEM(uint8_t*, digest) \
    ITEM(uint8_t*, vendor_signature_r, size_t* size) \
    ITEM(uint8_t*, vendor_signature_s, size_t* size) \
    ITEM(uint8_t*, server_signature_r, size_t* size) \
    ITEM(uint8_t*, server_signature_s, size_t* size)

/** The scope of this file is to define the interface of a manifest. It can be
 * implemented using different encodings, but each approach should implement
 * this interface to be usable by the library */

#define DEFINE_GETTER(type, name, ...) \
    type get_##name(const manifest_t* mt, ##__VA_ARGS__);

#define DEFINE_SETTER(type, name, ...) \
    void set_##name(manifest_t* mt, type name, ##__VA_ARGS__);

FOREACH_ITEM(DEFINE_GETTER)
FOREACH_ITEM(DEFINE_SETTER)

#undef DEFINE_GETTER
#undef DEFINE_SETTER

/** 
 * \brief Print manifest known values.
 * \param mt Pointer to a manifest structure.
 */
void print_manifest(const manifest_t* mt);

size_t get_vendor_digest_buffer(const manifest_t* mt, void** buffer);
size_t gen_server_digest_buffer(const manifest_t* mt, void** buffer);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* \} MANIFEST_H_ */
