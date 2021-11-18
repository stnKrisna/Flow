#pragma once

#include <cstdint>
#include <cstddef>

#include <iterator>
#include <vector>
#include <array>
#include <queue>

#include "directions.hpp"

namespace flow {
    template <size_t maxNavLayer>
    class FieldCell;

    template <typename DimensionType, size_t MaxNavLayer>
    class Field_t {
    public:
        using CellType = FieldCell<MaxNavLayer>;

        /// Target (point of interest) Vector2 represented as an array. Index 0 for x, index 1 for y
        using Vec2 = std::array<DimensionType, 2>;

        /// List of point of interest
        using PointOfInterests = std::vector<Vec2>;

    public:
        Field_t (DimensionType _width, DimensionType _height) :
            width(_width),
            height(_height)
        {
            cells = new CellType[_width * _height];
        };

        ~Field_t () {
            delete[] cells;
        };

        struct forward_iterator {
            using iterator_category = std::forward_iterator_tag;
            using difference_type   = std::ptrdiff_t;
            using value_type        = CellType;
            using pointer           = value_type*;
            using reference         = value_type&;

            size_t idx;

            forward_iterator(pointer ptr) : m_ptr(ptr) {
                idx = 0;
            }

            reference operator*() const { return *m_ptr; }
            pointer operator->() { return m_ptr; }

            forward_iterator& operator++() { ++m_ptr; ++idx; return *this; }
            forward_iterator operator++(int) { forward_iterator tmp = *this; ++(*this); ++idx; return tmp; }

            friend bool operator== (const forward_iterator& a, const forward_iterator& b) { return a.m_ptr == b.m_ptr; };
            friend bool operator!= (const forward_iterator& a, const forward_iterator& b) { return a.m_ptr != b.m_ptr; };

        private:
            pointer m_ptr;
        };

        forward_iterator begin () {
            return forward_iterator(&cells[0]);
        }

        forward_iterator end () {
            return forward_iterator(&cells[width * height]);
        }

        Field_t<DimensionType, MaxNavLayer> * addPointOfInterest (size_t layer, const PointOfInterests& poi);

        /// Get cardinal direction from a coordinate
        Direction_t getDirection (size_t layer, DimensionType x, DimensionType y);

        /// Get direction vector from a coordinate
        void getDirection (size_t layer, DimensionType x, DimensionType y, float * vX, float * vY);

        /// Get direction vector from a coordinate
        void getDirection (size_t layer, DimensionType x, DimensionType y, double * vX, double * vY);

        /// Get direction vector from a coordinate (Warning: If the direction is NW, NE, SW, or SE, the resulting vector will be 0. Use getNextCell instead)
        void getDirection (size_t layer, DimensionType x, DimensionType y, int * vX, int * vY);

        /// Get the next cell's coordinate from a coordinate point
        void getNextCell (size_t layer, DimensionType x, DimensionType y, DimensionType * nX, DimensionType * nY);

    private:
        DimensionType width;
        DimensionType height;

        CellType * cells;

    private:
        size_t vec2ToArrayIdx (DimensionType x, DimensionType y) {
            const auto col = x;
            const auto row = y;
            const auto rowLength = width; // Number of column in a row

            return (row * rowLength) + col;
        }

        size_t vec2ToArrayIdx (Vec2 in) {
            return vec2ToArrayIdx(in[0], in[1]);
        }

        size_t moveIndexByDirection (size_t idx, Direction_t dir) {
            if (idx == (size_t)-1)
                return -1;

            switch (dir) {
                case Directions::NORTH:
                    return (idx / width > 0 ? idx - width : -1);
                case Directions::SOUTH:
                    return (idx / width < (height - 1) ? idx + width : -1);
                case Directions::EAST:
                    return (idx % width < (width - 1) ? idx + 1 : -1);
                case Directions::WEST:
                    return (idx % width > 0 ? idx - 1 : -1);

                case Directions::NORTH_EAST:
                    return moveIndexByDirection( moveIndexByDirection(idx, Directions::NORTH), Directions::EAST );
                case Directions::NORTH_WEST:
                    return moveIndexByDirection( moveIndexByDirection(idx, Directions::NORTH), Directions::WEST );
                case Directions::SOUTH_EAST:
                    return moveIndexByDirection( moveIndexByDirection(idx, Directions::SOUTH), Directions::EAST );
                case Directions::SOUTH_WEST:
                    return moveIndexByDirection( moveIndexByDirection(idx, Directions::SOUTH), Directions::WEST );

                default:
                    return -1;
            }
        }
    };

    template <size_t MaxNavLayer>
    using LayeredField = Field_t<uint16_t, MaxNavLayer>;

    using Field = LayeredField<1>;
}

#include "field.cpp"
