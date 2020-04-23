#include "../Identification.h"
#include "../PeFile.h"
#include "../headers/DataDirectoryEntry.h"
#include "../headers/SectionHdr.h"
#include "iDir.h"

using namespace Pepper;

IDirectory::IDirectory(const PeFile& pe, const FileBytes& fileBytes, const DataDirectoryEntry& dataDirectoryEntry)
: IHeader(fileBytes, dataDirectoryEntry.rva())
, m_peFile(&pe)
, m_dataDirectoryEntry(&dataDirectoryEntry)
, m_sectionHeaderEntry(nullptr)
, m_diskToMemoryDifference(0)
{
    // linear search for containing section
    const SectionHeaders& sctns = pe.sectionHdrs();
    for (const auto& section : sctns.getSections()) {
        const uint32_t sctnBase = section.getStructPtr()->VirtualAddress;
        const uint32_t sctnSize = section.getStructPtr()->VirtualSize;
        const uint32_t sctnRaw  = section.getStructPtr()->PointerToRawData;
        if (sctnBase <= dataDirectoryEntry.rva()
        && dataDirectoryEntry.rva() < sctnBase + sctnSize) {
            m_sectionHeaderEntry = &section;
            m_diskToMemoryDifference = (sctnBase > sctnRaw) ? sctnBase - sctnRaw : sctnRaw - sctnBase;
            break;
        }
    }
}

/* pointer to base of directory only if the directory exists */
const void* IDirectory::dir() const
{
    return (Ident::dirExists(*this))
        ? &m_headerPtr[dirOffset()]
        : nullptr;
}

/* get size of directory */
uint32_t IDirectory::size() const
{
    return m_dataDirectoryEntry->size();
}

