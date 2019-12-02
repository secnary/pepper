#include "../DataDirectoryEntry.h"
#include "ManagedNative.h"

using namespace Pepper;

ClrNativeHeader::ClrNativeHeader(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{}

const char* ClrNativeHeader::getFieldName(int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* ClrNativeHeader::getFieldPtr(int index) const
{
    switch (index) {
        default: return nullptr;
    }
}
