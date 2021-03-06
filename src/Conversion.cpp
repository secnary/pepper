#include <Conversion.h>

#include <Types.h>
#include <PeFile.h>
#include <pe_structures/OptionalHdr.h>

/* Non-member non-friend functions to convert addresses between types */

using namespace Pepper;

namespace
{
/* Compare addresses in the section headers to find the difference between
 * Relative Virtual Addresses (RVAs) and file offsets.
 */
uint32_t calculateDiff(const PeFile& pe, const addr_t sourceAddr, Convert::AddrType sourceType)
{
    int base, size, oppo;
    if (sourceType == Convert::AddrType::RVA) {
        base = SectionHeaderEntry::VIRTUAL_ADDRESS;
        size = SectionHeaderEntry::VIRTUAL_SIZE;
        oppo = SectionHeaderEntry::POINTER_TO_RAW_DATA;
    } else if (sourceType == Convert::AddrType::RAW) {
        base = SectionHeaderEntry::POINTER_TO_RAW_DATA;
        size = SectionHeaderEntry::SIZE_OF_RAW_DATA;
        oppo = SectionHeaderEntry::VIRTUAL_ADDRESS;
    } else {
        return 0;
    }

    // linear search for containing section
    const SectionHeaders& sctns = pe.sectionHdrs();
    for (const auto& section : sctns.getSections()) {
        const uint32_t sctnBase = *static_cast<const uint32_t*>(section.getFieldPtr(base));
        const uint32_t sctnSize = *static_cast<const uint32_t*>(section.getFieldPtr(size));
        if (sctnBase <= sourceAddr && sourceAddr < sctnBase + sctnSize) {
            const uint32_t sctnOppo = *static_cast<const uint32_t*>(section.getFieldPtr(oppo));
            return (sctnBase > sctnOppo) ? sctnBase - sctnOppo : sctnOppo - sctnBase;
        }
    }

    return 0;
}
} // namespace

uint32_t Convert::getRvaToRawDiff(const PeFile& pe, const addr_t rva)
{
    return calculateDiff(pe, rva, AddrType::RVA);
}

uint32_t Convert::getRawToRvaDiff(const PeFile& pe, const addr_t raw)
{
    return calculateDiff(pe, raw, AddrType::RAW);
}

/* Convert an {AVA,RVA,RAW} to an {AVA,RVA,RAW}.
 * RAW < RVA < AVA, so comparisons are required
 * for conversions to smaller types.
 * Returns 0 if the conversion fails.
 */
addr_t Convert::convertAddr(const PeFile& pe, const addr_t addr, AddrType src, AddrType dst)
{
    const OptionalHeader& poh = pe.optionalHdr();
    uint64_t diff = 0;

    if (src == dst) {
        return addr;
    }

    // RVA is middle ground; Add ImageBase or subtract DiskToMem diff
    if (src == AddrType::RVA) {
        if (dst == AddrType::AVA) return addr + poh.imageBase();
        // convert to RAW
        diff = getRvaToRawDiff(pe, addr);
        return (addr > diff) ? addr - diff
                             : 0;
    }

    // Always subtract ImageBase when converting from an AVA
    if (src == AddrType::AVA) {
        const addr_t base = poh.imageBase();
        if (dst == AddrType::RVA) return (addr > base) ? addr - base
                                             : 0;
        // convert to RAW
        diff = getRvaToRawDiff(pe, addr - base);
        return (addr > base + diff) ? addr - base - diff
                                    : 0;
    }

    // Always add DiskToMem diff when converting from a RAW
    if (src == AddrType::RAW) {
        diff = getRawToRvaDiff(pe, addr);
        if (dst == AddrType::AVA) return addr + poh.imageBase() + diff;
        return addr + diff;
    }

    return 0;
}
