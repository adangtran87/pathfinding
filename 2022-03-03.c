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

#define STUCK_COST (UINT32_MAX-1)

typedef struct {
  int8_t x;
  int8_t y;
} Coord_t;

typedef struct {
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
        costMap[index].cost = UINT32_MAX;
      } else {
        costMap[index].cost = distanceToOrigin(&node);
      }
    }
  }
}

uint32_t getNodeCost(Coord_t *node, CostMapNode_t *costMap, int8_t x_max) {
  return costMap[coordToIndex(node, x_max)].cost;
}

typedef struct CoordStackNode{
  Coord_t coord;
  struct CoordStackNode *next;
} CoordStackNode_t;

CoordStackNode_t *head = NULL;

void stackAddNode(Coord_t *coord) {
  CoordStackNode_t *newNode = malloc(sizeof(CoordStackNode_t));

  newNode->coord.x = coord->x;
  newNode->coord.y = coord->y;
  newNode->next = head;
  /* printf("Adding %d,%d\n", newNode->coord.x, newNode->coord.y); */
  head = newNode;
}

void stackPopNode() {
  CoordStackNode_t *prev_head = head;
  head = head->next;
  /* printf("Popping %d,%d\n", prev_head->coord.x, prev_head->coord.y); */
  free(prev_head);
}

void freeStack() {
  while (head != NULL) {
    stackPopNode();
  }
}

void printStack() {
  while (head != NULL) {
    printf("%d,%d; ", head->coord.x, head->coord.y);
    stackPopNode();
  }
}

uint32_t countStack() {
  uint32_t numEntries = 0;
  CoordStackNode_t *node = head->next;
  while(node != NULL) {
    node = node->next;
    numEntries++;
  }
  return numEntries;
}

/**
 * getNextNode
 *
 * Get the surrounding coordinates for a given coordinate
 * Compare against max x,y
 */
bool getNextNode(Coord_t* current, CostMapNode_t * costMap, uint8_t *grid, int8_t x_max, int8_t y_max) {
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

  // Current Cost
  bool newNode = false;
  uint32_t currentIndex = coordToIndex(current, x_max);
  uint32_t currentCost = costMap[currentIndex].cost;
  Coord_t minCoord;
  Coord_t newCoord;

  for (uint32_t i = 0; i < NUM_SURROUND; i++) {
    Coord_t transform = surroundTransform[i];
    newCoord.x = (current->x + transform.x);
    newCoord.y = (current->y + transform.y);

    if (validCoord(&newCoord, x_max, y_max) && !isBlocked(grid, &newCoord, x_max)) {
      uint32_t newCost = getNodeCost(&newCoord, costMap, x_max);
      if (newCost < currentCost) {
        currentCost = newCost;
        minCoord.x = newCoord.x;
        minCoord.y = newCoord.y;
        newNode = true;
      }
      numValid++;
    }
  }

  // If surrounding nodes are not less cost, we're stuck. Increase current node
  // cost to not visit it again.
  if (!newNode) {
    costMap[currentIndex].cost = STUCK_COST;
    stackPopNode();
  } else {
    stackAddNode(&minCoord);
  }

  return newNode;
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
  Coord_t start = {
    .x = x_size - 1,
    .y = x_size - 1,
  };
  stackAddNode(&start);

  CostMapNode_t *costMap = malloc(sizeof(CostMapNode_t) * x_size * y_size);

  fillCostMap(costMap, grid, x_size, y_size);

  // Debug
  printCostMap(costMap, x_size, y_size);

  Coord_t *current = &head->coord;
  while ((current->x != 0) || (current->y != 0)) {
    getNextNode(current, costMap, grid, x_size, y_size);
    current = &head->coord;
    /* printf("Current: %d,%d\n", current->x, current->y); */
  }

  printCostMap(costMap, x_size, y_size);
  free(costMap);

  return countStack();
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

#define TEST2_X (7)
#define TEST2_Y (7)
uint8_t grid2[7*7] = {
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
  0, 0, 0, 1, 0, 1, 0,
  0, 0, 0, 1, 0, 0, 0,
  0, 0, 0, 1, 0, 0, 0,
  0, 0, 0, 0, 0, 0, 0,
};

// Expected to take diagonal
TestCase_t test2 = {
  .grid = grid2,
  .x_size = TEST2_X,
  .y_size = TEST2_Y,
  .answer = 7,
};

TestCase_t *testCases[] = {
  &test0,
  &test1,
  &test2,
};

int main() {
  for(int i = 0; i < (sizeof(testCases) / sizeof(TestCase_t*)); i++) {
  /* for(int i = 0; i < 1; i++) { */
    printf("\n");
    TestCase_t *test = testCases[i];
    uint32_t numSteps = find_path(test->grid, test->x_size, test->y_size);
    if (numSteps == test->answer) {
      printf("Number of steps: %d\n", numSteps);
      printStack();
    } else {
      printf("numSteps %d does not match answer: %d\n", numSteps, test->answer);
    }
  }
  return 0;
}
