#ifndef DELAYIMPORT_H
#define DELAYIMPORT_H

#include <vector>

#include <generics/iDir.h>
#include <pe_structures/ImportsCommon.h>

/*
typedef struct _IMAGE_DELAY_IMPORT_DESCRIPTOR {
    int32_t  Attributes;                 // reserved, must be zero
    uint32_t NameRVA;                    // pointer to DLL's name
    uint32_t ModuleHandleRVA;            // pointer to DLL's module handle
    uint32_t DelayImportAddressTableRVA; // pointer to delay-load import address table
    uint32_t DelayImportNameTableRVA;    // pointer to delay-load import name table
    uint32_t BoundDelayImportTableRVA;   // pointer to bound delay-load address table
    uint32_t UnloadDelayImportTableRVA;  // pointer to unload delay-load address table
    int32_t  Timestamp;                  // dll timestamp after binding
} IMAGE_DELAY_IMPORT_DESCRIPTOR, *PIMAGE_DELAY_IMPORT_DESCRIPTOR;
*/

namespace Pepper {

class DataDirectoryEntry;

using DelayImportDescriptor = GenericImportDescriptor<IMAGE_DELAY_IMPORT_DESCRIPTOR>;

/* Variable-length array of DelayImportDescriptors.
 * length is equal to number of modules being imported from.
 */
class DelayImportDir final : public IDirectory {
public:
    DelayImportDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    const void* getFieldPtr(const int index) const override;
    const std::vector<DelayImportDescriptor>& descriptors() const
    {
        return m_descriptors;
    }

    size_t length() const { return m_descriptors.size(); }

    static const char* getFieldName(const int index);

    // classes that need special access to s_diskToMemDiff
    friend class GenericImportDescriptor<IMAGE_DELAY_IMPORT_DESCRIPTOR>;

private:
    std::vector<DelayImportDescriptor> m_descriptors{};
    static size_t s_diskToMemDiff;
};
} // namespace Pepper

#endif
