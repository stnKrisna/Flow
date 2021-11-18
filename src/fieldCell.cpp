#include "fieldCell.hpp"

#ifndef field_cell_cpp
#define field_cell_cpp

#include <stdexcept>

#define VALIDATE_LAYER(layer) if(layer > maxLayer() || layer < 0) { \
                                          std::cout << __FILE__ << ":" << __LINE__ << std::endl; \
                                          throw std::range_error("Layer out of range");}

#define setDirectionMap(existing, newVal) ((existing & 0xF0) + (newVal & 0x0F))

namespace flow {

template <size_t maxNavLayer>
uint8_t FieldCell<maxNavLayer>::globalBuildId = 0;

template <size_t maxNavLayer>
Direction_t FieldCell<maxNavLayer>::getDirection (size_t layer) {
    VALIDATE_LAYER(layer);

    return directions[layer] & 0x0F;
}

template <size_t maxNavLayer>
void FieldCell<maxNavLayer>::incrementDirectionLayer () {
    ++usedDirectionLayer;
}

template <size_t maxNavLayer>
void FieldCell<maxNavLayer>::setDirection (size_t layer, Direction_t direction) {
    VALIDATE_LAYER(layer);

    directions[layer] = setDirectionMap(directions[layer], direction);
}

template <size_t maxNavLayer>
void FieldCell<maxNavLayer>::markDirAsWall (size_t layer) {
    VALIDATE_LAYER(layer);

    directions[layer] = setDirectionMap(directions[layer], Directions::WALL);
}

template <size_t maxNavLayer>
void FieldCell<maxNavLayer>::markDirAsStop (size_t layer) {
    VALIDATE_LAYER(layer);

    directions[layer] = setDirectionMap(directions[layer], Directions::STOP);
}

template <size_t maxNavLayer>
void FieldCell<maxNavLayer>::setBuildId (size_t layer, uint8_t buildId) {
    VALIDATE_LAYER(layer);

    directions[layer] = (buildId << 4) + (directions[layer] & 0xF);
}

template <size_t maxNavLayer>
uint8_t FieldCell<maxNavLayer>::getBuildId (size_t layer) {
    VALIDATE_LAYER(layer);

    return (directions[layer] >> 4);
}

template <size_t maxNavLayer>
bool FieldCell<maxNavLayer>::canEnterFrom (Direction_t dir) {
    auto entryWhitelist = getEntryDir();

    const bool passFilter = (entryWhitelist & dir) != 0;
    const bool passInvFilter = ((~entryWhitelist) & dir) == 0;
    return (passFilter && passInvFilter);
}

} // namespace flow

#undef setDirectionMap
#undef VALIDATE_LAYER

#endif // field_cell_cpp
