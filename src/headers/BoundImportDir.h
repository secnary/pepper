#ifndef BOUNDIMPORT_H
#define BOUNDIMPORT_H

#include <vector>

#include "../generics/iDir.h"
#include "struct.h"

/*
// one for each imported module
typedef struct _IMAGE_IMPORT_DESCRIPTOR {
    int32_t ImportLookupTableRVA;  // pointer to array of thunk data
    int32_t TimeDateStamp;         // dll timestamp after binding
    int32_t ForwarderChain;        // index of first forwarder reference
    int32_t NameRVA;               // pointer to module name
    int32_t ImportAddressTableRVA; // pointer to import address table
} IMAGE_IMPORT_DESCRIPTOR, *PIMAGE_IMPORT_DESCRIPTOR;

// one for each imported function per module
typedef struct _IMAGE_THUNK_DATA32 {
    union {
        struct {
            int32_t OrdinalNumber : 31;
            int32_t OrdinalFlag   : 1;
        };
        int32_t HintNameTableRVA; // RVA to IMAGE_IMPORT_BY_NAME
    };
} IMAGE_THUNK_DATA32, *PIMAGE_THUNK_DATA32;

// one for each imported function per module
typedef struct _IMAGE_THUNK_DATA64 {
    union {
        struct {
            int64_t OrdinalNumber : 63;
            int64_t OrdinalFlag   : 1;
        };
        int64_t HintNameTableRVA; // RVA to IMAGE_IMPORT_BY_NAME
    };
} IMAGE_THUNK_DATA64, *PIMAGE_THUNK_DATA64;

typedef struct _IMAGE_IMPORT_BY_NAME {
    int16_t Hint; // Index into export name table pointer
    char    Name; // Imported function name as null-terminated ASCII string
} IMAGE_IMPORT_BY_NAME, *PIMAGE_IMPORT_BY_NAME;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

class BoundImportDir final : public IDirectory {
public:
    BoundImportDir()
    : IDirectory()
    {}

    BoundImportDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
//  const IMAGE_BOUND_IMPORT_DIRECTORY* imports() const { return (PIMAGE_)dir(); }
    const void* getFieldPtr(int index) const override;

    // static functions
    static const char* getFieldName(int index);
};
} // namespace Pepper

#endif