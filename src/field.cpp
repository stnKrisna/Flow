#include "field.hpp"

#ifndef field_cpp
#define field_cpp

#define cellIdxToVec2(idx) (index % 3, Math.floor(index / 3))

#include <iostream>
#include <stdexcept>
#include "fieldCell.hpp"

namespace flow {

template <typename T, size_t S>
Field_t<T, S> * Field_t<T, S>::addPointOfInterest (size_t layer, const PointOfInterests& poi) {
    uint8_t buildId = CellType::newBuildId();
    std::queue<size_t> cellQueue;

    // Load POIs to cell queue and mark them as the destination
    for (auto point : poi) {
        const auto cellIdx = vec2ToArrayIdx(point);
        cellQueue.push(cellIdx);
        cells[cellIdx].setDirection(layer, Directions::DEST);
        cells[cellIdx].setBuildId(layer, buildId);
    }

    const Direction_t directions[9] = {
        Directions::NORTH,
        Directions::EAST,
        Directions::SOUTH,
        Directions::WEST,
        Directions::NORTH_WEST,
        Directions::NORTH_EAST,
        Directions::SOUTH_EAST,
        Directions::SOUTH_WEST,
        Directions::STOP,
    };

    while (!cellQueue.empty()) {
        const auto cellIdx = cellQueue.front();
        cellQueue.pop();

        for (auto i = 0; directions[i] != Directions::STOP; ++i) {
            bool currentDirIsDiag = (directions[i] == Directions::NORTH_EAST) | (directions[i] == Directions::SOUTH_EAST) | (directions[i] == Directions::SOUTH_WEST) | (directions[i] == Directions::NORTH_WEST);

            const auto neighbourCellIdx = moveIndexByDirection(cellIdx, directions[i]);
            if (neighbourCellIdx != (size_t)(-1)) {
                // Skip cell if the build ID is the same as the current build ID
                if (cells[neighbourCellIdx].getBuildId(layer) == buildId)
                    continue;

                // Skip cell if wall. Also mark it as a wall
                if (cells[neighbourCellIdx].isWall()) {
                    cells[neighbourCellIdx].setBuildId(layer, buildId);
                    cells[neighbourCellIdx].markDirAsWall(layer);
                    continue;
                }

                // Skip diagonal if diagonal direction is not allowed
                if (currentDirIsDiag && !cells[neighbourCellIdx].getAllowDiagonal())
                    continue;

                // Check if the direction to the current cell is valid from the neighbour or not
                auto dirFromNeighbourToCurrentCell = Directions::negateDir(directions[i]);
                if (!cells[cellIdx].canEnterFrom(dirFromNeighbourToCurrentCell))
                    continue;

                // All check pass. Set the direction to the current cell and mark it as part of the current build
                cells[neighbourCellIdx].setBuildId(layer, buildId);
                cells[neighbourCellIdx].setDirection(layer, dirFromNeighbourToCurrentCell);
                cellQueue.push(neighbourCellIdx);
            }
        }
    }

    return this;
}

#define NULL_GUARD(i) if (i == nullptr)\
                             throw std::runtime_error("NULL pointer exception")

#define DIR2VEC(dir, x, y, mag) {\
    x = y = 0; \
    switch (dir & (Directions::NORTH | Directions::SOUTH)) {\
        case Directions::NORTH:\
            y = -1; ++mag; break;\
        case Directions::SOUTH:\
            y = 1; ++mag; break;\
    }\
\
    switch (dir & (Directions::WEST | Directions::EAST)) {\
        case Directions::EAST:\
            x = 1; ++mag; break;\
        case Directions::WEST:\
            x = -1; ++mag; break;\
    }\
}

/// Get cardinal direction from a coordinate
template <typename T, size_t S>
Direction_t Field_t<T, S>::getDirection (size_t layer, T x, T y) {
    return cells[vec2ToArrayIdx(x, y)].getDirection(layer);
}

/// Get direction vector from a coordinate
template <typename T, size_t S>
void Field_t<T, S>::getDirection (size_t layer, T x, T y, float * vX, float * vY) {
    NULL_GUARD(vX);
    NULL_GUARD(vY);

    int mag = 0;
    float _x, _y;
    DIR2VEC(getDirection(layer, x, y), _x, _y, mag);
    (*vX) = _x / mag;
    (*vY) = _y / mag;
}

/// Get direction vector from a coordinate
template <typename T, size_t S>
void Field_t<T, S>::getDirection (size_t layer, T x, T y, double * vX, double * vY) {
    NULL_GUARD(vX);
    NULL_GUARD(vY);

    int mag = 0;
    double _x, _y;
    DIR2VEC(getDirection(layer, x, y), _x, _y, mag);
    (*vX) = _x / mag;
    (*vY) = _y / mag;
}

/// Get direction vector from a coordinate
template <typename T, size_t S>
void Field_t<T, S>::getDirection (size_t layer, T x, T y, int * vX, int * vY) {
    NULL_GUARD(vX);
    NULL_GUARD(vY);

    int mag = 0;
    int _x, _y;
    DIR2VEC(getDirection(layer, x, y), _x, _y, mag);
    (*vX) = _x / mag;
    (*vY) = _y / mag;
}

/// Get the next cell's coordinate from a coordinate point
template <typename T, size_t S>
void Field_t<T, S>::getNextCell (size_t layer, T x, T y, T * nX, T * nY) {
    NULL_GUARD(nX);
    NULL_GUARD(nY);

    int mag = 0;
    int _x, _y;
    DIR2VEC(getDirection(layer, x, y), _x, _y, mag);
    (*nX) += _x;
    (*nY) += _y;
}

#undef DIR2VEC
#undef NULL_GUARD

} // namespace flow

#endif // field_cpp
