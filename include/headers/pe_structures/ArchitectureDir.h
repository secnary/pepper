#ifndef ARCHITECTURE_H
#define ARCHITECTURE_H

#include <generics/iDir.h>

namespace Pepper {

class PeFile;
class FileBytes;
class DataDirectoryEntry;

class ArchitectureDir final : public IDirectory {
public:
    enum Fields {
        _NUM_FIELDS,
    };

    ArchitectureDir(const PeFile& pe, const FileBytes& fbytes, const DataDirectoryEntry& dde);

    const void* getFieldPtr(const int index) const override;
//  const IMAGE_ARCHITECTURE_DIRECTORY* getStructPtr() const { return static_cast<const IMAGE_ARCHITECTURE_DIRECTORY*>(dir()); }

    static const char* getFieldName(const int index);
};
} // namespace Pepper

#endif
