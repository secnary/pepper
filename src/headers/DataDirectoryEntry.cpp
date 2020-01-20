#include "DataDirectoryEntry.h"

using namespace Pepper;

const char* DataDirectoryEntry::getFieldName(const int index)
{
    switch (index) {
        case VIRTUAL_ADDRESS: return "Virtual Address";
        case SIZE           : return "Size";
        default             : return "<UNKNOWN>";
    }
}

const void* DataDirectoryEntry::getFieldPtr(const int index) const
{
    switch (index) {
        case VIRTUAL_ADDRESS: return &entry()->VirtualAddress;
        case SIZE           : return &entry()->Size;
        default             : return nullptr;
    }
}

