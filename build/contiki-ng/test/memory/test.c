#include "contiki.h"
#include "../../target_headers.h"
#include "memory_firefly.h"
#include "cpu.h"
#include <stdio.h>
#include <string.h>

#define CHECK(cond, ...) \
    if (cond) { \
        printf(__VA_ARGS__); \
        goto final; \
    } else { \
        printf("CHECK PASSED: " __VA_ARGS__); \
    }

#define BUFFER_SIZE 2048/sizeof(int)

PROCESS(memory_test, "Test memory");
AUTOSTART_PROCESSES(&memory_test);

PROCESS_THREAD(memory_test, ev, data) {
    PROCESS_BEGIN();
    int i;
    int memory_buffer[BUFFER_SIZE];
    int memory_test[BUFFER_SIZE];
    for (i=0; i<BUFFER_SIZE; i++) {
        memory_buffer[i] = i;
    }
    mem_object obj;
    pull_error err = memory_open(&obj, OBJ_1, WRITE_ALL);
    CHECK(err, "Error opening memory object 2 %s\n", err_as_str(err));
//    int not_correctly_ereased = 0;
    int value = memory_read(&obj, memory_test, BUFFER_SIZE, 0);
    /*for (i=0; i<BUFFER_SIZE; i++) {
        if (memory_test[i] != 0xffffffff) {
            printf("%d\n", i);
            not_correctly_ereased = 1;
            break;
        }
    }
    CHECK(value != BUFFER_SIZE, "Error reading ereased memory\n");
    CHECK(not_correctly_ereased, "Memory not correctly ereased\n");*/
    INTERRUPTS_DISABLE();
    value = memory_write(&obj, memory_buffer, BUFFER_SIZE, 0);
    CHECK(value != BUFFER_SIZE, "Error writing buffer");
    err = memory_close(&obj);
    INTERRUPTS_ENABLE();
    CHECK(err, "Error closing object %s\n", err_as_str(err));
    err = memory_open(&obj, OBJ_1, READ_ONLY);
    CHECK(err, "Error reopening in reading mode %s\n", err_as_str(err));
    value = memory_read(&obj, memory_test, BUFFER_SIZE, 0);
    CHECK(value != BUFFER_SIZE, "Error reading memory %s\n", err_as_str(err));
    int res = memcmp(memory_buffer, memory_test, BUFFER_SIZE);
    CHECK(res != 0, "The readed data is different from the writted one\n");
    printf("Success\n");
final:
    for (i=0; i<30; i++) {
        printf("%02x, ", memory_test[i]);
    }
    while (1) {
       // NOP 
    }
    PROCESS_END();
}
