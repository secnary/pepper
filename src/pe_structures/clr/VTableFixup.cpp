#include <pe_structures/DataDirectoryEntry.h>
#include <pe_structures/clr/VTableFixup.h>

using namespace Pepper;

ClrVTableFixup::ClrVTableFixup(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde)
: IDirectory(pe, fbytes, dde)
{}

const char* ClrVTableFixup::getFieldName(const int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* ClrVTableFixup::getFieldPtr(const int index) const
{
    switch (index) {
        default: return nullptr;
    }
}

