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
#include <stdbool.h>
#include <stdlib.h>

#define TARGET_X (0)
#define TARGET_Y (0)

typedef struct {
  int8_t x;
  int8_t y;
} Coord_t;

typedef struct {
  Coord_t coord;
  uint32_t cost;
  bool visited;
} CostMapNode_t;

uint32_t coordToIndex(Coord_t *coord, int8_t x_max) {
  return coord->y*x_max + coord->x;
}

bool isBlocked(uint8_t *grid, Coord_t *coord, int8_t x_max) {
  uint32_t index = coordToIndex(coord, x_max);
  return (grid[index] == 1);
}

// Target is 0 so just square coordinates to get distance
uint32_t distanceToOrigin(Coord_t *coord) {
  // Technically can still overflow if x close to 64k
  return ((coord->x*coord->x) + (coord->y*coord->y));
}

#define NUM_SURROUND (8)

//Check if coordinate is within bounds
bool validCoord(Coord_t *coord, int8_t x_max, int8_t y_max) {
  return ((0 <= coord->x) && (coord->x < x_max) && (0 <= coord->y) && (coord->y < y_max));
}

void fillCostMap(CostMapNode_t *costMap, uint8_t *grid, int8_t x_max, int8_t y_max) {
  Coord_t node;
  // Go through each coordinate and calculate the cost
  for (int j = 0; j < y_max; j++) {
    node.y = j;
    for (int i = 0; i < x_max; i++) {
      node.x = i;

      uint32_t index = coordToIndex(&node, x_max);
      if (isBlocked(grid, &node, x_max)) {
        costMap[index].cost = 0xFFFF;
      } else {
        costMap[index].cost = distanceToOrigin(&node);
      }
    }
  }
}

/**
 * getSurroundingCoordinates
 *
 * Get the surrounding coordinates for a given coordinate
 * Compare against max x,y
 */
uint32_t getSurroundingCost(Coord_t* current, CostMapNode_t * costMap, uint8_t *grid, int8_t x_max, int8_t y_max) {
  uint32_t numValid = 0;
  Coord_t surroundTransform[NUM_SURROUND] = {
    { //left
      .x = -1,
      .y = 0,
    },
    { // up
      .x = 0,
      .y = -1,
    },
    { // Right
      .x = 1,
      .y = 0,
    },
    { // down
      .x = 0,
      .y = 1,
    },
    { //up left
      .x = -1,
      .y = -1,
    },
    { //up right
      .x = 1,
      .y = -1,
    },
    { //down left
      .x = -1,
      .y = 1,
    },
    { //down right
      .x = 1,
      .y = 1,
    },
  };

  for (uint32_t i = 0; i < NUM_SURROUND; i++) {
    Coord_t transform = surroundTransform[i];
    Coord_t newCoord = {
      .x = (current->x + transform.x),
      .y = (current->y + transform.y),
    };

    if (validCoord(&newCoord, x_max, y_max) && !isBlocked(grid, &newCoord, x_max)) {
      CostMapNode_t *node = &costMap[coordToIndex(&newCoord, x_max)];
      // Valid surrounding coordinates, calculate cost
      node->cost = distanceToOrigin(&newCoord);
      numValid++;
    }
  }

  return numValid;
}

void printCostMap(CostMapNode_t *costMap, int8_t x_size, int8_t y_size) {
  for (int i = 0; i < (x_size * y_size); i++) {
    if (i % x_size == 0) {
      printf("\n");
    }
    printf("%d\t", costMap[i].cost);
  }
  printf("\n");
}

uint32_t find_path(uint8_t *grid, int8_t x_size, int8_t y_size) {
  Coord_t current = {
    .x = x_size - 1,
    .y = x_size - 1,
  };

  CostMapNode_t *costMap = malloc(sizeof(CostMapNode_t) * x_size * y_size);

  fillCostMap(costMap, grid, x_size, y_size);

  // Debug
  printCostMap(costMap, x_size, y_size);

  /* // Destination is 0, so want to subtract and get there */
  /* while((start_x != 0) && (start_y != 0)) { */

  /*   // From a given current location */
  /*   // Get all surrounding coordinates */
  /*   getSurroundingCoordinates(start_x, start_y, &surrounding_x, &surrounding_y) */
  /*   // Remove coordinates that are blocked */
  /*   int num_valid_coordinates = remove_blocked_coordinates(&surrounding_x, &surrounding_y, &valid_x, &valid_y) */
  /*   // Calculate the cost of coordinates that are unblocked */
  /*   for (int i = 0; i < num_valid_coordinates; i++) { */
  /*     cost[i] = distanceToTarget(valid_x[i], valid_y[i]); */
  /*   } */
  /*   // Move towards the coordinate with lowest cost/distance */
  /*   int min_index = 0; */
  /*   for (int i = 1; i < num_valid_coordinates; i++) { */
  /*     if (cost[min_index] > cost[i]) { */
  /*       min_index = i; */
  /*     } */
  /*   } */
  /*   // If moving towards the square is bad, discount that square by adding cost it */
  /*   start_x = valid_x[min_index]; */
  /*   start_y = valid_x[min_index]; */

  /*   // Pop back to previous square and choose the next best cost */

  /* } */

  free(costMap);

  return 0;
}

typedef struct {
  uint32_t answer;
  int8_t x_size;
  int8_t y_size;
  uint8_t *grid;
} TestCase_t;

#define TEST0_X (5)
#define TEST0_Y (5)
uint8_t grid0[TEST0_X*TEST0_Y] = {
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
  0, 0, 0, 0, 0,
};

// Expected to take diagonal
TestCase_t test0 = {
  .grid = grid0,
  .x_size = TEST0_X,
  .y_size = TEST0_Y,
  .answer = 4,
};

#define TEST1_X (7)
#define TEST1_Y (7)
uint8_t grid1[7*7] = {
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 1, 1, 0,
  0, 0, 0, 1, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
};

// Expected to take diagonal
TestCase_t test1 = {
  .grid = grid1,
  .x_size = TEST1_X,
  .y_size = TEST1_Y,
  .answer = 9,
};

#define NUM_TEST_CASES (2)
TestCase_t *testCases[NUM_TEST_CASES] = {
  &test0,
  &test1,
};

int main() {
  for(int i = 0; i < NUM_TEST_CASES; i++) {
    TestCase_t *test = testCases[i];
    uint32_t numSteps = find_path(test->grid, test->x_size, test->y_size);
    printf("Number of steps: %d\n", numSteps);
  }
  return 0;
}
