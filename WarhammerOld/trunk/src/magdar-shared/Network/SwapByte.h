#pragma once
#ifndef SWAPBYTE_H_
#define SWAPBYTE_H_

#include <Common.h>


/**
 * Byte swapping utility for endianness
 *
 */
namespace SwapByte
{


template <typename T> inline void Swap(T& value)
{
}

template <> inline void Swap(int32& value)
{
    value = ((((int32)(value) & (int32) 0x000000ffU) << 24) |
             (((int32)(value) & (int32) 0x0000ff00U) <<  8) |
             (((int32)(value) & (int32) 0x00ff0000U) >>  8) |
             (((int32)(value) & (int32) 0xff000000U) >> 24));

}

template <> inline void Swap(uint32& value)
{
    value = ((((uint32)(value) & (uint32) 0x000000ffU) << 24) |
             (((uint32)(value) & (uint32) 0x0000ff00U) <<  8) |
             (((uint32)(value) & (uint32) 0x00ff0000U) >>  8) |
             (((uint32)(value) & (uint32) 0xff000000U) >> 24));
}

template <> inline void Swap(uint16& value)
{
    value = ((((uint16)(value) & (uint16) 0x00ffU) << 8)  |
             (((uint16)(value) & (uint16) 0xff00U) >> 8));

}

template <> inline void Swap(int16& value)
{
    value = ((((int16)(value) & (int16) 0x00ffU) << 8)  |
             (((int16)(value) & (int16) 0xff00U) >> 8));
}

template <> inline void Swap(int64& value)
{
    value = ((((int64)(value) & (int64) 0x00000000000000ffULL) << 56) |
             (((int64)(value) & (int64) 0x000000000000ff00ULL) << 40) |
             (((int64)(value) & (int64) 0x0000000000ff0000ULL) << 24) |
             (((int64)(value) & (int64) 0x00000000ff000000ULL) <<  8) |
             (((int64)(value) & (int64) 0x000000ff00000000ULL) >>  8) |
             (((int64)(value) & (int64) 0x0000ff0000000000ULL) >> 24) |
             (((int64)(value) & (int64) 0x00ff000000000000ULL) >> 40) |
             (((int64)(value) & (int64) 0xff00000000000000ULL) >> 56));

}

template <> inline void Swap(uint64& value)
{
    value = ((((uint64)(value) & (uint64) 0x00000000000000ffULL) << 56) |
             (((uint64)(value) & (uint64) 0x000000000000ff00ULL) << 40) |
             (((uint64)(value) & (uint64) 0x0000000000ff0000ULL) << 24) |
             (((uint64)(value) & (uint64) 0x00000000ff000000ULL) <<  8) |
             (((uint64)(value) & (uint64) 0x000000ff00000000ULL) >>  8) |
             (((uint64)(value) & (uint64) 0x0000ff0000000000ULL) >> 24) |
             (((uint64)(value) & (uint64) 0x00ff000000000000ULL) >> 40) |
             (((uint64)(value) & (uint64) 0xff00000000000000ULL) >> 56));
}



};

#endif
