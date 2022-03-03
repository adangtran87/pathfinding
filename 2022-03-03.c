/*
 * Imagine that we have two dimensional grid
 * Two types of blocks in the grid
 * one you can move through, ones that you cannot move
 *
 * There is a guaranteed path from the bottom right to the top left corner
 * Write an algorithm to find the shortest path there
 * There is a guaranteed way to get there
 * Find the fewest number of blocks to get there
 */

// Start at bottom right and start at top left
// Grid will
// Can move diagonal
//

/*
 * D 0 0 0 1 1
 * 0 0 1 0 0 0
 * 0 0 0 0 0 0
 * 0 0 1 1 1 0
 * 0 0 1 0 0 0
 * 0 0 1 0 0 S(C)
 *
 * D 0 0 0 1 1
 * 0 0 1 0 0 0
 * 0 0 0 0 0 0
 * 0 0 1 1 1 0
 * 0 0 1 0 C 0
 * 0 0 1 0 0 S
 *
 * D 0 0 0 1 1
 * 0 0 1 0 0 0
 * 0 0 0 0 0 0
 * 0 0 1 1 1 0
 * 0 0 1 C 0 0
 * 0 0 1 0 0 S
 */
#include <stdint.h>
#include <stdio.h>

#define TARGET_X (0)
#define TARGET_Y (0)

//boolean isBlocked(uint32_t *grid, uint32_t x, uint32_t y) {
//  uint32_t index = y*x + x;
//  // Bounds check for index

//  if (grid[index]) {
//    return true;
//  } else {
//    return false;
//  }
//}

//void setSquare(*grid, x, y value) {

//}

//void pushCoordinate();
//void popCoordinate();

//// Target is 0
//uint64_t distanceToOrigin(uint32_t x, uint32_t y) {
//  return (uint32_t)(x*x + y*y);
//}

//void getSurroundingCoordinates(uint32_t x, uint32_t y, uint32_t*surrounding_x, uint32_t* surrounding_y) {
//  // Have the array t
//}

//int removeBlockedCooridinates(uint32_t *surrounding_x, uint32_t *surrounding_y, uint32_t *valid_x, uint32_t *valid_y) {
//  int num_valid = 0;
//  for (int i = 0; i < 8; i++) {
//    if (isBlocked(x_array[i], y_array[i])
//        // add to valid_x, valid_y
//  }

//  return num_valid
//}

//uint32_t find_fewest_steps(uint32_t *grid, uint32_t x_size, uint32_t y_size) {
//  uint32_t start_x = x_size - 1;
//  uint32_t start_y = y_size - 1;
//  uint32_t end_x = 0;
//  uint32_t end_y = 0;

//  uint32_t surrounding_x[8];

//  uint32_t surrounding_y[8];

//  uint32_t valid_x[8];
//  uint32_t valid_y[8];

//  uint32_t cost[8];

//  //
//  // 1,1; 1,0; 0,1; -1,0; 1

//  // Destination is 0, so want to subtract and get there
//  while((start_x != 0) && (start_y != 0)) {

//    // From a given current location
//    // Get all surrounding coordinates
//    getSurroundingCoordinates(start_x, start_y, &surrounding_x, &surrounding_y)
//    // Remove coordinates that are blocked
//    int num_valid_coordinates = remove_blocked_coordinates(&surrounding_x, &surrounding_y, &valid_x, &valid_y)
//    // Calculate the cost of coordinates that are unblocked
//    for (int i = 0; i < num_valid_coordinates; i++) {
//      cost[i] = distanceToTarget(valid_x[i], valid_y[i]);
//    }
//    // Move towards the coordinate with lowest cost/distance
//    int min_index = 0;
//    for (int i = 1; i < num_valid_coordinates; i++) {
//      if (cost[min_index] > cost[i]) {
//        min_index = i;
//      }
//    }
//    // If moving towards the square is bad, discount that square by adding cost it
//    start_x = valid_x[min_index];
//    start_y = valid_x[min_index];

//    // Pop back to previous square and choose the next best cost

//  }

//}

int main() {
  printf("Hello World");
  return 0;
}
