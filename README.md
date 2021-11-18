![Flow cover image](https://raw.githubusercontent.com/stnKrisna/Flow/master/readme/flow.png)

- [Documentation](#documentation)
- [Planned features](#planned-features)
- [Example](#example)
  - [ASCII Map](#building-flow-field-from-ascii-map)
  - [One-way traffic](#one-way-traffic)
  - [STL-like cell iteration](#stl-like-cell-iteration)
  - [Grid-based navigation](#grid-based-navigation)
  - [Vector-based navigation](#vector-based-navigation)
- [Inspiration](#inspiration)

## Documentation

## Features
- **Data source agnostic** - Flow field can be created from any data source. You're in charge of any data to 2D grid conversion and we'll do the rest. See "[ASCII Map](#building-flow-field-from-ascii-map)" or "[One-way traffic](#one-way-traffic)" example.
- **STL-like iteration** - See "[STL-like cell iteration](#stl-like-cell-iteration)" for example.
- **Grid-based navigation** - See "[Grid-based navigation](#grid-based-navigation)" for example.
- **Vector-based navigation** - See "[Vector-based navigation](#vector-based-navigation)" for example.
- **Matrix/Vector library agnostic** - We don't care what math library you use. Just give us the address of the `X` & `Y` component, are you're good to go! See "[Grid-based navigation](#grid-based-navigation)" & "[Vector-based navigation](#vector-based-navigation)" for example.

## Planned features
- **Serialization**
- **Deserialization**
- **User embeded cell data**
- **Cell exit restriction** - Cell direction can only point to a certain direction.
- **Dynamic/real-time reaction** - Flow direction is able to adapt to a dynamic environment without having to recalculate every single cell.

## Example
Here is an example usage of this library.

### Building flow field from ASCII map
```c++
flow::Field field(10, 8);

char map[] = "\
wwwwwwwwww\
w........w\
www.ww.www\
www.ww.www\
www.ww.www\
www.ww.www\
w........w\
w........w";

for (auto cell = field.begin(); cell != field.end(); ++cell) {
  cell->setAllowDiagonal(true);
  switch (map[cell.idx]) {
    case '.':
      cell->setEntryDir(flow::Directions::NORTH |
              flow::Directions::EAST |
              flow::Directions::SOUTH |
              flow::Directions::WEST);
      break;
    case 'w':
      cell->setEntryDir(0);
      cell->setAllowDiagonal(false);
      break;
  }
}
```

### One-way traffic
```c++
char map[] = "\
wwwwwwwwww\
w........w\
www4ww1www\
www4ww1www\
www4ww1www\
www4ww1www\
w........w\
w........w";

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
    case '4':
      cell->setEntryDir(flow::Directions::SOUTH);
      break;
    case 'w':
      cell->setEntryDir(0);
      cell->setAllowDiagonal(false);
      break;
  }
}
```

### STL-like cell iteration
```c++
// Begin-end loop
for (auto itr = field.begin(); itr != field.end(); ++itr) {
  // Print the index of the iterator
  std::cout << itr.idx << std::endl;
  
  // You can use the arrow operator (->) to access the cell
  itr->setEntryDir(flow::Directions::NORTH |
    flow::Directions::EAST |
    flow::Directions::SOUTH |
    flow::Directions::WEST);
}

// Range based loop
for (auto& cell : field) {
  // Convert every cell to a wall
  cell.setEntryDir(0);
}

for (auto cell : field) {
  // Everything is now a wall
  std::cout << cell.isWall() << " ";
}
```

### Grid-based navigation
```c++
// BYO vector object
if (enemy.position == enemy.desiredPos) {
  uint16_t newPos_x = 0;
  uint16_t newPos_y = 0;
  
  field.getNextCell(0, (int)enemy.pos.x, (int)enemy.pos.y, &newPos_x, &newPos_y);
  
  enemy.desiredPos.x = newPos_x;
  enemy.desiredPos.y = newPos_y;
}
```

### Vector-based navigation
```c++
// BYO vector object
Vector2 enemyDir;

// Get the normalized vector of the direction
field.getDirection(0, (int)enemy.pos.x, (int)enemy.pos.y, &enemyDir.x, &enemyDir.y);

// BYO vector math
enemy.pos += enemy.speed * enemyDir;
```

## Inspiration
This project is inspired from this paper:

- S. Patil, J. Van Den Berg, S. Curtis, M. C. Lin, en D. Manocha, “Directing crowd simulations using navigation fields”, _IEEE transactions on visualization and computer graphics_, vol 17, no 2, bll 244–254, 2010.
