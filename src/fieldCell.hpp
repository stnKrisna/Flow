#pragma once

#include <cstdint>
#include <cstddef>
#include <bitset>

#include "directions.hpp"
#include "field.hpp"

namespace flow {
    template <size_t maxNavLayer>
    class FieldCell {
        template <typename T, size_t S>
        friend class Field_t;

    public:
        FieldCell () :
            cellData({0}),
            usedDirectionLayer(0)
        {}

        static uint8_t newBuildId () {
            uint8_t curBuildId = globalBuildId & 0xF;

            globalBuildId = (++curBuildId) & 0xF;

            return curBuildId;
        }

        size_t getMaxNavLayer () {
            return maxNavLayer;
        }

        Direction_t getDirection (size_t layer);

        void setEntryDir (Direction_t direction) {
            cellData.accessDirection.setAccessDir(direction);
        }

        Direction_t getEntryDir () {
            return cellData.accessDirection.getAccessDir();
        }

        void setAllowDiagonal (bool allowDiag) {
            cellData.accessDirection.setAllowDiagonal(allowDiag);
        }

        bool getAllowDiagonal () {
            return cellData.accessDirection.getAllowDiagonal();
        }

        bool isWall () {
            return getEntryDir() == Directions::WALL;
        }

    private:
        static uint8_t globalBuildId;
        size_t usedDirectionLayer;
        Direction_t directions[maxNavLayer];

        union {
            struct {
                /* Access direction data is represented by an 8 bit bitset.
                 * Bit allocation looks like this:
                 *
                 * 0-3 -> Allow entry to cell from direction
                 * 4   -> Allow diagonal access
                 * 5-7 -> Unallocated
                 */
                std::bitset<8> data;

                void setAccessDir (Direction_t dir) {
                    const std::bitset<8> mask(0xF0);
                    data = (data & mask) | std::bitset<8>(dir);
                }

                Direction_t getAccessDir () {
                    const std::bitset<8> mask(0xF);
                    return (Direction_t)((data & mask).to_ulong());
                }

                void setAllowDiagonal (bool diagState) {
                    data.set(4, diagState);
                }

                bool getAllowDiagonal () {
                    return (data[4] == 1);
                }

            } accessDirection;

            uint16_t raw;
        } cellData;

    private:
        void incrementDirectionLayer ();

        void setDirection (size_t layer, Direction_t direction);
        void markDirAsWall (size_t layer);
        void markDirAsStop (size_t layer);
        bool canEnterFrom (Direction_t dir);

        void setBuildId (size_t layer, uint8_t buildId);
        uint8_t getBuildId (size_t layer);

        inline void setCellAsDest (size_t layer) {
            markDirAsStop(layer);
        }

        inline size_t maxLayer () {
            return maxNavLayer;
        }

        inline size_t availableDirectionLayer () {
            return maxNavLayer - usedDirectionLayer;
        }

        inline size_t getUsedLayer () {
            return usedDirectionLayer;
        }
    };
}

#include "fieldCell.cpp"
