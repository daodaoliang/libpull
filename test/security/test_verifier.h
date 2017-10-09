#include "unity.h"
#include "sample_data.h"
#include "digest.h"
#include "sha256.h"
#include "error.h"
#include "ecc.h"
#include "memory_objects.h"
#include "verifier.h"
#include "metadata.h"
#include "mock_memory.h"
#include "memory_file_posix.h"
#include "simple_metadata.h"
#include "invalid_digest.h"

#define NO_MOCK(func) func##_StubWithCallback(func##_impl)

void test_verify_object(void) {
    NO_MOCK(memory_open);
    NO_MOCK(memory_read);
    NO_MOCK(memory_close);
    NO_MOCK(get_size);
    NO_MOCK(get_offset);
    NO_MOCK(to_digest);

    mem_object obj_t;
    pull_error err;
    err = verify_object(OBJ_1, digest_sha256, x, y, secp256r1, &obj_t);
    TEST_ASSERT_TRUE(!err);
}

void test_verify_object_invalid_object(void) {
    mem_object obj_t;
    pull_error err;
    err = verify_object(42, digest_sha256, x, y, secp256r1, &obj_t);
    TEST_ASSERT_TRUE(err);
}

void test_verify_object_invalid_read(void) {
    NO_MOCK(memory_open);
    memory_read_IgnoreAndReturn(MEMORY_READ_ERROR);
    NO_MOCK(memory_close);

    mem_object obj_t;
    pull_error err;
    err = verify_object(OBJ_1, digest_sha256, x, y, secp256r1, &obj_t);
    TEST_ASSERT_TRUE(err == MEMORY_READ_ERROR);
}

void test_verify_object_invalid_digest_init(void) {
    NO_MOCK(memory_open);
    NO_MOCK(memory_read);
    NO_MOCK(memory_close);

    mem_object obj_t;
    pull_error err;
    digest_func func = digest_sha256;
    func.init = invalid_init;
    err = verify_object(OBJ_1, func, x, y, secp256r1, &obj_t);
    TEST_ASSERT_TRUE(err == DIGEST_INIT_ERROR);
}

void test_verify_object_invalid_digest_update(void) {
    NO_MOCK(memory_open);
    NO_MOCK(memory_read);
    NO_MOCK(memory_close);
    NO_MOCK(get_size);
    NO_MOCK(get_offset);
    NO_MOCK(to_digest);

    mem_object obj_t;
    pull_error err;
    digest_func func = digest_sha256;
    func.update = invalid_update;
    err = verify_object(OBJ_1, func, x, y, secp256r1, &obj_t);
    TEST_ASSERT_TRUE(err == DIGEST_UPDATE_ERROR);
}

void test_verify_object_invalid_key(void) {
    NO_MOCK(memory_open);
    NO_MOCK(memory_read);
    NO_MOCK(memory_close);
    NO_MOCK(get_size);
    NO_MOCK(get_offset);
    NO_MOCK(to_digest);

    mem_object obj_t;
    pull_error err;
    err = verify_object(OBJ_1, digest_sha256, y, y, secp256r1, &obj_t);
    TEST_ASSERT_TRUE(err == VERIFICATION_FAILED_ERROR);
}

void test_verify_object_invalid_metadata_to_digest(void) {
    NO_MOCK(memory_open);
    NO_MOCK(memory_read);
    NO_MOCK(memory_close);
    NO_MOCK(get_size);
    NO_MOCK(get_offset);
    to_digest_IgnoreAndReturn(-1);

    mem_object obj_t;
    pull_error err;
    err = verify_object(OBJ_1, digest_sha256, x, y, secp256r1, &obj_t);
    TEST_ASSERT_TRUE(err == DIGEST_UPDATE_ERROR);
}

void test_verify_all(void) {
    test_verify_object();
    test_verify_object_invalid_object();
    test_verify_object_invalid_read();
    test_verify_object_invalid_digest_init();
    test_verify_object_invalid_digest_update();
    test_verify_object_invalid_key();
    test_verify_object_invalid_metadata_to_digest();
}
