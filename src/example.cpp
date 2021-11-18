#include "flow.hpp"

#include <iostream>

#define WIDTH 10
#define HEIGHT 8

void run () {
    flow::LayeredField<2> field(WIDTH, HEIGHT);

    char map[] = "\
wwwwwwwwww\
w........w\
www4ww1www\
www4ww1www\
www4ww1www\
www4ww1www\
w........w\
w........w";

    // Set map data
    for (auto cell = field.begin(); cell != field.end(); ++cell) {
        cell->setAllowDiagonal(true);
        switch (map[cell.idx]) {
            case '.':
                cell->setEntryDir(flow::Directions::NORTH |
                        flow::Directions::EAST |
                        flow::Directions::SOUTH |
                        flow::Directions::WEST);
                break;
            case '1':
                cell->setEntryDir(flow::Directions::NORTH);
                break;
            case '9':
                cell->setEntryDir(flow::Directions::NORTH_WEST);
                break;
            case '3':
                cell->setEntryDir(flow::Directions::NORTH_EAST);
                break;
            case '2':
                cell->setEntryDir(flow::Directions::EAST);
                break;
            case '4':
                cell->setEntryDir(flow::Directions::SOUTH);
                break;
            case '8':
                cell->setEntryDir(flow::Directions::WEST);
                break;
            case 'C':
                cell->setEntryDir(flow::Directions::SOUTH_WEST);
                break;
            case '6':
                cell->setEntryDir(flow::Directions::SOUTH_EAST);
                break;
            case 'w':
                cell->setEntryDir(0);
                cell->setAllowDiagonal(false);
                break;
        }
    }

    // Add point of interests
    flow::Field::PointOfInterests poi1 = {
        {3, 7},
        {4, 7},
        {5, 7},
        {6, 7},
    };
    flow::Field::PointOfInterests poi2 = {
        {3, 1},
        {4, 1},
        {5, 1},
        {6, 1},
    };

    field.addPointOfInterest(0, poi1);
    field.addPointOfInterest(1, poi2);

    // Print map data
    for (size_t i = 0; i < 2; ++i) {
        std::cout << "0 ";
        for (auto cell = field.begin(); cell != field.end(); ++cell) {
            if (cell.idx != 0 && cell.idx % WIDTH == 0)
                std::cout << std::endl << (cell.idx / WIDTH) << " ";

            switch (cell->getDirection(i)) {
                case flow::Directions::NORTH:
                    printf("↑ ");
                    break;
                case flow::Directions::SOUTH:
                    printf("↓ ");
                    break;
                case flow::Directions::EAST:
                    printf("→ ");
                    break;
                case flow::Directions::WEST:
                    printf("← ");
                    break;

                case flow::Directions::NORTH_EAST:
                    printf("↗ ");
                    break;
                case flow::Directions::NORTH_WEST:
                    printf("↖ ");
                    break;
                case flow::Directions::SOUTH_EAST:
                    printf("↘ ");
                    break;
                case flow::Directions::SOUTH_WEST:
                    printf("↙ ");
                    break;
                case flow::Directions::STOP:
                    printf("# ");
                    break;

                default:
                    printf("X ");
                    break;
            }
        }
        std::cout << std::endl << "  ";
        for (size_t i = 0; i < WIDTH; ++i)
            std::cout << i << " ";
        std::cout << std::endl << std::endl;
    }

    // Navigate using getNextCell
    {
        uint16_t x = 8;
        uint16_t y = 1;

        while ((x != 3 || x != 4 || x != 5 || x != 6) && (y != 7)) {
            float vx, vy;
            field.getDirection(0, x, y, &vx, &vy);

            std::cout << "{" << x << ", " << y << " | " << vx << ", " << vy << "} ";

            field.getNextCell(0, x, y, &x, &y);
        }
        std::cout << "{" << x << ", " << y << "} ";
    }

    std::cout << std::endl;

    {
        uint16_t x = 1;
        uint16_t y = 7;

        while ((x != 3 || x != 4 || x != 5 || x != 6) && (y != 1)) {
            float vx, vy;
            field.getDirection(1, x, y, &vx, &vy);

            std::cout << "{" << x << ", " << y << " | " << vx << ", " << vy << "} ";

            field.getNextCell(1, x, y, &x, &y);
        }
        std::cout << "{" << x << ", " << y << "} ";
    }
}

int main () {
    run();
    return 0;
}
