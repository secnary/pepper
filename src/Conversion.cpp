#include "Conversion.h"

#include "PeFile.h"
#include "generics/iDir.h"
#include "headers/OptionalHdr.h"

/* Non-member non-friend functions to addresses from one type to another */

using namespace Pepper;

/* Compare addresses in the section headers to find the difference between
 * RVAs and file offsets.
 */
static int32_t getDiffRvaRaw(const PeFile &pe, int32_t addr, Convert::AddrType type)
{
    int base, size, oppo;
    if (type == Convert::RVA) {
        base = SectionHeaderEntry::VIRTUAL_ADDRESS;
        size = SectionHeaderEntry::VIRTUAL_SIZE;
        oppo = SectionHeaderEntry::POINTER_TO_RAW_DATA;
    } else if (type == Convert::RAW) {
        base = SectionHeaderEntry::POINTER_TO_RAW_DATA;
        size = SectionHeaderEntry::SIZE_OF_RAW_DATA;
        oppo = SectionHeaderEntry::VIRTUAL_ADDRESS;
    } else {
        return 0;
    }

    // linear search for containing section
    SectionHeaders *sctns = (SectionHeaders*)pe.getHeaderPtr(PeFile::SECTION);
    for (const auto &section : sctns->sections()) {
        int32_t sctnBase = *(int32_t*)section.getFieldPtr(base);
        int32_t sctnSize = *(int32_t*)section.getFieldPtr(size);
        if (sctnBase <= addr && addr < sctnBase + sctnSize) {
            return abs(sctnBase - *(int32_t*)section.getFieldPtr(oppo));
        }
    }

    return 0;
}

/* Use an RVA address to get the difference between RVAs and RAWs */
int32_t Convert::getRvaToRawDiff(const PeFile &pe, int64_t rva)
{
    return getDiffRvaRaw(pe, rva, RVA);
}

/* Use a RAW address to get the difference between RVAs and RAWs */
int32_t Convert::getRawToRvaDiff(const PeFile &pe, int64_t raw)
{
    return getDiffRvaRaw(pe, raw, RAW);
}

/* Convert an {AVA,RVA,RAW} to an {AVA,RVA,RAW}.
 * RAW < RVA < AVA, so comparisons are required
 * for conversions to smaller types.
 * Returns -1 if the conversion fails.
 */
int64_t Convert::convertAddr(const PeFile &pe, int64_t addr, AddrType src, AddrType dst)
{
    OptionalHeader *poh = (OptionalHeader*)pe.getHeaderPtr(PeFile::OPTIONAL);
    int64_t diff = 0;

    if (src == dst)
        return addr;

    // RVA is middle ground; Add ImageBase or subtract DiskToMem diff
    if (src == RVA) {
        if (dst == AVA) return addr + poh->imageBase();
        // convert to RAW
        diff = getRvaToRawDiff(pe, addr);
        return (addr > diff) ? addr - diff
                             : -1;
    }

    // Always subtract ImageBase when converting from an AVA
    if (src == AVA) {
        int64_t base = poh->imageBase();
        if (dst == RVA) return (addr > base) ? addr - base
                                             : -1;
        // convert to RAW
        diff = getRvaToRawDiff(pe, addr - base);
        return (addr > base + diff) ? addr - base - diff
                                    : -1;
    }

    // Always add DiskToMem diff when converting from a RAW
    if (src == RAW) {
        diff = getRawToRvaDiff(pe, addr);
        if (dst == AVA) return addr + poh->imageBase() + diff;
        return addr + diff;
    }

    return -1;
}