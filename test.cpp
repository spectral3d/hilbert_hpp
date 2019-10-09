#include "hilbert.hpp"
#include <cstdio>
#include <chrono>

// Test functions to encode & decode 8-bit 3D values.

std::array<uint8_t, 3>
IndexToPosition3D8BV1(uint32_t idx);

std::array<uint8_t, 3>
IndexToPosition3D8BV2(uint32_t idx);

uint32_t
PositionToIndex3D8BV1(std::array<uint8_t, 3> const &pos);

uint32_t
PositionToIndex3D8BV2(std::array<uint8_t, 3> const &pos);

// Convert 24 bit index to 8 bit 3D position using v1.
std::array<uint8_t, 3>
IndexToPosition3D8BV1(uint32_t idx)
{
    std::array<uint8_t, 3> tmp;

    tmp[0] = uint8_t((idx >> 16) & 0xff);
    tmp[1] = uint8_t((idx >> 8) & 0xff);
    tmp[2] = uint8_t(idx & 0xff);

    return hilbert::v1::IndexToPosition(tmp);
}

// Convert 8 bit 3D position to 24 bit index using v1.
uint32_t
PositionToIndex3D8BV1(std::array<uint8_t, 3> const &pos)
{
    std::array<uint8_t, 3> tmp = hilbert::v1::PositionToIndex(pos);

    uint32_t idx = (uint32_t(tmp[0]) << 16) +
                   (uint32_t(tmp[1]) << 8) +
                   (uint32_t(tmp[2]));
    return idx;
}

// Convert 24 bit index to 8 bit 3D position using v2.
std::array<uint8_t, 3>
IndexToPosition3D8BV2(uint32_t idx)
{
    std::array<uint8_t, 3> tmp;

    tmp[0] = uint8_t((idx >> 16) & 0xff);
    tmp[1] = uint8_t((idx >> 8) & 0xff);
    tmp[2] = uint8_t(idx & 0xff);

    return hilbert::v2::IndexToPosition(tmp);
}

// Convert 8 bit 3D position to 24 bit index using v2.
uint32_t
PositionToIndex3D8BV2(std::array<uint8_t, 3> const &pos)
{
    std::array<uint8_t, 3> tmp = hilbert::v2::PositionToIndex(pos);

    uint32_t idx = (uint32_t(tmp[0]) << 16) +
                   (uint32_t(tmp[1]) << 8) +
                   (uint32_t(tmp[2]));
    return idx;
}

int main(int, char **)
{
    std::array<uint8_t, 3> tmp;
    uint32_t tmp2;

    std::chrono::high_resolution_clock clock;

    printf("Timing: 10 * 2^24 encode/decode operations with v1 & v2.\n");
    printf("This may take some time.\n\n");

    auto t0 = clock.now();

    // Time encoding & decoding with V1.
    for(uint32_t n=0;n<10;n++)
    {
        // Encode and decode all 2^24 combinations of 8 bit 3D positions.
        for(uint32_t i=0;i<(1<<24);i++)
        {
            tmp = IndexToPosition3D8BV1(i);
            tmp2 = PositionToIndex3D8BV1(tmp);
        }
    }

    auto t1 = clock.now();

    // Time encoding & decoding with V2.
    for(uint32_t n=0;n<10;n++)
    {
        // Encode and decode all 2^24 combinations of 8 bit 3D positions.
        for(uint32_t i=0;i<(1<<24);i++)
        {
            tmp = IndexToPosition3D8BV2(i);
            tmp2 = PositionToIndex3D8BV2(tmp);
        }
    }

    // Dump timings.
    auto t2 = clock.now();

    auto d0 = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
    auto d1 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

    printf("v1 took %li ms\n", d0.count());
    printf("v2 took %li ms\n", d1.count());

    printf("\n");

    // Ensure that all 2^24 8 bit 3D positions are treated consistently by
    // v1 and v2.
    {
        printf("Consistency check.  Encode with v1, decode with v1\n");
        bool consistent(true);
        for(uint32_t i=0;i<(1<<24);i++)
        {
            tmp = IndexToPosition3D8BV1(i);
            tmp2 = PositionToIndex3D8BV1(tmp);

            if(i != tmp2)
            {
                printf("FAIL: %i != %i\n", i, tmp2);
                consistent = false;
            }
        }
        if(consistent)
        {
            printf("PASS\n\n");
        }
    }

    {
        printf("Consistency check.  Encode with v1, decode with v2\n");
        bool consistent(true);
        for(uint32_t i=0;i<(1<<24);i++)
        {
            tmp = IndexToPosition3D8BV1(i);
            tmp2 = PositionToIndex3D8BV2(tmp);

            if(i != tmp2)
            {
                printf("FAIL: %i != %i\n", i, tmp2);
                consistent = false;
            }
        }
        if(consistent)
        {
            printf("PASS\n\n");
        }
    }

    {
        printf("Consistency check.  Encode with v2, decode with v1\n");
        bool consistent(true);
        for(uint32_t i=0;i<(1<<24);i++)
        {
            tmp = IndexToPosition3D8BV2(i);
            tmp2 = PositionToIndex3D8BV1(tmp);

            if(i != tmp2)
            {
                printf("FAIL: %i != %i\n", i, tmp2);
                consistent = false;
            }
        }
        if(consistent)
        {
            printf("PASS\n\n");
        }
    }

    return 0;
}
