#include <pe_structures/ImportDir.h>

#include <cstring>

#include <Identification.h>
#include <pe_structures/DataDirectoryEntry.h>

using namespace Pepper;

// each class related to regular imports must see the same `diskToMemDiff` value
size_t ImportDir::s_diskToMemDiff = 0;
template<> size_t& ImportDescriptor::s_pDiskToMemDiff = ImportDir::s_diskToMemDiff;
template<> size_t& ImportThunk32::s_pDiskToMemDiff    = ImportDir::s_diskToMemDiff;
template<> size_t& ImportThunk64::s_pDiskToMemDiff    = ImportDir::s_diskToMemDiff;

ImportDir::ImportDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{
    if (Ident::dirExists(*this)) {
        s_diskToMemDiff = m_diskToMemoryDifference;
        constexpr char null[sizeof(IMAGE_IMPORT_DESCRIPTOR)] = {0};
        const IMAGE_IMPORT_DESCRIPTOR* pid = static_cast<const IMAGE_IMPORT_DESCRIPTOR*>(dir());
        size_t i = 0;
        while (memcmp(&pid[i], &null, sizeof(null))) {
            m_descriptors.emplace_back(pe, fbytes, dirOffset() + (i * sizeof(null)));
            ++i;
        }
    }
}

const char* ImportDir::getFieldName(const int index)
{
    switch (index) {
        default: return "Import Descriptor";
    }
}

const void* ImportDir::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

