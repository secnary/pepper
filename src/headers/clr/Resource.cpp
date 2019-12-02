#include "../DataDirectoryEntry.h"
#include "Resource.h"

using namespace Pepper;

ClrResource::ClrResource(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde)
: IDirectory(pe, fbytes, dde)
{}

const char* ClrResource::getFieldName(int index)
{
    switch (index) {
        default: return "<UNKNOWN>";
    }
}

const void* ClrResource::getFieldPtr(int index) const
{
    switch (index) {
        default: return nullptr;
    }
}
