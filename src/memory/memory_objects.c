#include <stdio.h>
#include <string.h>

#include "memory/memory_objects.h"
#include "memory/metadata.h"
#include "common/logger.h"
#include "common/error.h"
#include "common/settings.h"
#include "common/external.h"

pull_error get_newest_firmware(obj_id* obj, uint16_t* version, mem_object* obj_t) {
    metadata mt;
    pull_error err;
    uint8_t i;
    for (i=0; memory_objects[i] > 0; i++) {
        err = read_firmware_metadata(memory_objects[i], &mt, obj_t);
        if (err) {
            log_error(err, "Failure reading firmware metadata for object %d\n", memory_objects[i]);
            return GET_NEWEST_ERROR;
        }
        if (i == 0) {
            *version = mt.vendor.version;
            *obj = memory_objects[i];
        } else if (mt.vendor.version > *version) {
            *version = mt.vendor.version;
            *obj = memory_objects[i];
        }
    }
    return PULL_SUCCESS;
}

pull_error get_oldest_firmware(obj_id* obj, uint16_t* version, mem_object* obj_t) {
    metadata mt;
    pull_error err;
    uint8_t i;
    for (i=0; memory_objects[i] > 0; i++) {
        err = read_firmware_metadata(memory_objects[i], &mt, obj_t);
        if (err != PULL_SUCCESS) {
            log_error(err, "Failure reading firmware metadata for obj %d\n", memory_objects[i]);
            return GET_OLDEST_ERROR;
        }
        if (i == 0) {
            *version = mt.vendor.version;
            *obj = memory_objects[i];
        } else if (mt.vendor.version < *version) {
            *version= mt.vendor.version;
            log_debug("Oldest version is %x\n", mt.vendor.version);
            *obj = memory_objects[i];
        }
    }
    return PULL_SUCCESS;
}

pull_error copy_firmware(obj_id s, obj_id d, mem_object* src, mem_object* dst) {
    pull_error err;
    metadata srcmt;
    int firmware_size = 0;
    err = read_firmware_metadata(s, &srcmt, src);
    if (err) {
        log_error(err, "Error reading source metadata for object %d\n", s);
        return COPY_FIRMWARE_ERROR;
    }
    firmware_size = get_size(&srcmt) + get_offset(&srcmt);
    log_debug("Firmware size is %d\n", firmware_size);
    err = memory_open(src, s);
    if (err) {
        log_error(err, "Impossible to open source object %d\n", s);
        return COPY_FIRMWARE_ERROR;
    }
    err = memory_open(dst, d);
    if (err) {
        log_error(err, "Impossible to open destination object %d\n", d);
        memory_close(src);
        return COPY_FIRMWARE_ERROR;
    }
    int offset = 0, step = MEMORY_OBJ_BUFFER_SIZE, readed = 0;
    uint8_t buffer[MEMORY_OBJ_BUFFER_SIZE];
    while (offset < firmware_size && 
            (readed = memory_read(src, buffer, step, offset)) > 0) {
        if (memory_write(dst, buffer, readed, offset) != readed) {
            err = COPY_FIRMWARE_ERROR;
            log_error(err, "Error writing to destination object %d\n", d);
            goto finish;
        }
        offset+=readed;
        if (offset + step > firmware_size) {
            step = firmware_size - offset;
        }
    }
    if (readed <= 0) {
        err = COPY_FIRMWARE_ERROR;
        log_error(err, "Error while copying firmware\n");
        goto finish;
    }
    err = PULL_SUCCESS;
finish:
    memory_close(src);
    memory_close(dst);
    return err;
}

pull_error read_firmware_metadata(obj_id id, metadata* mt, mem_object* obj) {
    pull_error err = memory_open(obj, id);
    if (err) {
        log_error(err, "Failure opening object %d\n", id);
        return READ_METADATA_ERROR;
    }
    if (memory_read(obj, (uint8_t*) mt, sizeof(metadata), 0) != sizeof(metadata)) {
        log_error(MEMORY_READ_ERROR, "Failure reading object %d\n", id);
        return READ_METADATA_ERROR;
    }
    memory_close(obj);
    return PULL_SUCCESS;
}

pull_error write_firmware_metadata(obj_id id, const metadata* mt, mem_object* obj) {
    pull_error err = memory_open(obj, id) != PULL_SUCCESS;
    if (err) {
        log_error(err, "Failure opening firmware\n");
        return WRITE_METADATA_ERROR;
    }
    if (memory_write(obj, (uint8_t*) mt, sizeof(metadata), 0) != sizeof(metadata)) {
        memory_close(obj);
        log_error(MEMORY_WRITE_ERROR, "Failure writing metadata into object %d\n", id);
        return WRITE_METADATA_ERROR;
    }
    memory_close(obj);
    return PULL_SUCCESS;
}

pull_error invalidate_object(obj_id id, mem_object* obj_t) {
    metadata nullmt;
    memset(&nullmt, 0, sizeof(metadata));
    pull_error err = write_firmware_metadata(id, &nullmt, obj_t);
    if (err) {
        log_error(err, "Failure writing firmware metadata on object%d\n", id);
        return INVALIDATE_OBJECT_ERROR;
    }
    return PULL_SUCCESS;
}
