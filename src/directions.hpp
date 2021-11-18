#pragma once

namespace flow {
    typedef uint8_t Direction_t;

    namespace Directions {
        typedef enum Directions_t : Direction_t {
            /* Cardinal direction enum.
             *
             * Since we have 4 cardinal direction, we can represent
             * each direction using 4 bit bitmap like so:
             *
             * N = 0x1
             * E = 0x2
             * S = 0x4
             * W = 0x8
             *
             */

            NORTH      = 0x1,
            NORTH_WEST = 0x1 | 0x8,
            NORTH_EAST = 0x1 | 0x2,

            EAST       = 0x2,
            WEST       = 0x8,

            SOUTH      = 0x4,
            SOUTH_WEST = 0x4 | 0x8,
            SOUTH_EAST = 0x4 | 0x2,
        } Directions_t;

        typedef enum DirectionControl_ : Direction_t {
            /* Direction control enum
             *
             * Since 0b0000 is not used to denote directions and
             * 0b1111 is an invalid direction, we can use the remaining
             * 2 space as a control value.
             *
             */

            STOP       = 0x0,
            WALL       = 0X0,
            DEST       = 0XF,
        } DirectionControl_t;

        static Direction_t negateDir (Direction_t dir) {
            // http://graphics.stanford.edu/~seander/bithacks.html#SwappingBitsXOR
            const uint8_t i = 0, j = 2; // Bit position to swap
            const uint8_t n = 2; // number of consecutive bits in each sequence
            const uint8_t b = dir; // bits to swap reside in b

            const uint8_t x = ((b >> i) ^ (b >> j)) & ((1U << n) - 1); // XOR temporary
            return (b ^ ((x << i) | (x << j)));
        }
    }
}
