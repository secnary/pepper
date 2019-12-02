#ifndef EXPORT_H
#define EXPORT_H

#include "../Identification.h"
#include "../generics/iDir.h"
#include "struct.h"

/*
typedef struct _IMAGE_EXPORT_DIRECTORY {
    int32_t ExportFlags;           // reserved, must be zero
    int32_t TimeDateStamp;         // export data creation time
    int16_t MajorVersion;          // major version number
    int16_t MinorVersion;          // minor version number
    int32_t NameRVA;               // pointer to dll name
    int32_t OrdinalBase;           // ordinal starting index
    int32_t AddressTableEntries;   // number of export table entries
    int32_t NumberOfNamePointers;  // number of ordinal table entries
    int32_t ExportAddressTableRVA; // pointer to export table
    int32_t NamePointerRVA;        // pointer to name table
    int32_t OrdinalTableRVA;       // pointer to ordinal table
} IMAGE_EXPORT_DIRECTORY, *PIMAGE_EXPORT_DIRECTORY;
*/

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

/* Variable-length array of RVAs to .text section
 */
class ExportAddressTable final : public IHeader {
private:
    int m_length{};
    static int s_diskToMemDiff; // RVAs in .edata point to .text
    const int32_t* addresses() const { return (int32_t*)hdr(); }
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ExportAddressTable()
    : IHeader()
    {}

    ExportAddressTable(const PeFile &pe, const FileBytes &fbytes, int raw, int len);

    // member functions
    const void* getFieldPtr(int index) const override;

    int32_t codeRva(int index) const
    {
        int32_t* ret = (int32_t*)getFieldPtr(index);
        return (ret == nullptr) ? -1 : *ret;
    }

    int32_t codeRaw(int index) const
    {
        int32_t* ret = (int32_t*)getFieldPtr(index);
        return (ret == nullptr) ? -1 : (*ret) - s_diskToMemDiff;
    }

    int length() const { return m_length; }

    // static functions
    static const char* getFieldName(int index);
};

/* Variable-length array of RVAs to function names
 */
class ExportNameTable final : public IHeader {
private:
    int m_length{};
    static int s_diskToMemDiff;
    const int32_t* addresses() const { return (int32_t*)hdr(); }
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ExportNameTable()
    : IHeader()
    {}

    ExportNameTable(const PeFile &pe, const FileBytes &fbytes, int raw, int len);

    // member functions
    const void* getFieldPtr(int index) const override;

    // get array element or -1 if out of range
    int32_t nameRva(int index) const
    {
        return (0 <= index && index < length())
        ? addresses()[index] : -1;
    }

    // get ASCII name or nullptr if out of range
    const char* funcName(int index) const
    {
        return (0 <= index && index < length())
        ? &mem()[nameRva(index) - s_diskToMemDiff]
        : nullptr;
    }

    int length() const { return m_length; }

    // static functions
    static const char* getFieldName(int index);
};

/* Variable-length array of sequential numbers as int16_t
 */
class ExportOrdinalTable final : public IHeader {
private:
    int m_length{};
    const int16_t* ordinals() const { return (int16_t*)hdr(); }
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ExportOrdinalTable()
    : IHeader()
    {}

    ExportOrdinalTable(const FileBytes &fbytes, int raw, int len);

    // member functions
    const void* getFieldPtr(int index) const override;

    // get array element or -1 if out of range
    int16_t ordinal(int index) const
    {
        int16_t *ret = (int16_t*)getFieldPtr(index);
        return (ret == nullptr) ? -1 : *ret;
    }

    int length() const { return m_length; }

    // static functions
    static const char* getFieldName(int index);
};

/* A single EXPORT_DIRECTORY structure.
 * Contains pointers to variable-length arrays of export-related data.
 */
class ExportDir final : public IDirectory {
private:
    ExportAddressTable m_addrTable;
    ExportNameTable    m_nameTable;
    ExportOrdinalTable m_ordTable;
public:
    enum Fields {
        EXPORT_FLAGS,
        TIMESTAMP,
        MAJOR_VERSION,
        MINOR_VERSION,
        NAME_RVA,
        ORDINAL_BASE,
        ADDRESS_TABLE_ENTRIES,
        NUMBER_OF_NAME_POINTERS,
        EXPORT_ADDRESS_TABLE_RVA,
        NAME_POINTER_RVA,
        ORDINAL_TABLE_RVA,
        _NUM_FIELDS,
    };

    ExportDir()
    : IDirectory()
    , m_addrTable()
    , m_nameTable()
    , m_ordTable()
    {}

    ExportDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
    const void* getFieldPtr(int index) const override;
    const IMAGE_EXPORT_DIRECTORY* xport() const { return (PIMAGE_EXPORT_DIRECTORY)dir(); }

    const ExportAddressTable* eat() const { return Ident::dirExists(*this) ? &m_addrTable : nullptr; }
    const ExportNameTable*    ent() const { return Ident::dirExists(*this) ? &m_nameTable : nullptr; }
    const ExportOrdinalTable* eot() const { return Ident::dirExists(*this) ? &m_ordTable  : nullptr; }
    const char* dllName() const { return &mem()[xport()->NameRVA - m_diffOfRvaRaw]; }

    // static functions
    static const char* getFieldName(int index);
};
} // namespace Pepper

#endif