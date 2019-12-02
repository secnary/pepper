#ifndef GLOBALPOINTER_H
#define GLOBALPOINTER_H

#include "../generics/iDir.h"
#include "struct.h"

namespace Pepper {

// forward declarations
class PeFile;
class FileBytes;
class DataDirectoryEntry;

class GlobalPointerDir final : public IDirectory {
public:
    GlobalPointerDir()
    : IDirectory()
    {}

    GlobalPointerDir(const PeFile &pe, const FileBytes &fbytes, const DataDirectoryEntry &dde);

    // member functions
//  const IMAGE_* NAME() const { return (PIMAGE_)dir(); }
    const void* getFieldPtr(int index) const override;

    // static functions
    static const char* getFieldName(int index);
};
} // namespace Pepper

#endif