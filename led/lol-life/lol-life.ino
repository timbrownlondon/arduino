#include <Charliplexing.h>

// Life runs in a matrix world
// we need a matrix for the current state (cell[][])
// and a matric to record the next generation (next[][])
int8_t cell[DISPLAY_COLS][DISPLAY_ROWS];
int8_t next[DISPLAY_COLS][DISPLAY_ROWS];

// counters to decide when to reset the world
int  generation = 0;
uint8_t count = 0;

void update_display() {
  for (int8_t x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
    for (int8_t y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
      LedSign::Set(x, y, cell[x][y] * 2); // 2 is mid low brightness
    }
  }
}

// have all the cells died?
boolean all_dead() {
  for (int8_t x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
    for (int8_t y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
      if ( cell[x][y] ) {
        return false;
      }
    }
  }
  return true;
}


void set_next_generation() {
  for (int8_t x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
    for (int8_t y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
      next[x][y] = next_state(x, y);
    }
  }
}

int8_t next_state(int8_t x, int8_t y) {
  // return 0 (dead) or 1 (alive)
  // determine whether cell is alive in next generation
  // according to number of its alive neighbours

  // first let's check how many neighbours are alive
  int8_t alive_neighbours = 0;

  for (int8_t a = x - 1; a < x + 2; a++) {
    int8_t x_target = a;
    // wrap in x dimension
    if (x_target < 0) {
      x_target = DISPLAY_COLS - 1;
    }
    if (x_target > DISPLAY_COLS - 1) {
      x_target = 0;
    }

    for (int8_t b = y - 1; b < y + 2; b++) {
      int8_t y_target = b;
      // wrap in y dimension
      if (y_target < 0) {
        y_target = DISPLAY_ROWS - 1;
      }
      if (y_target > DISPLAY_ROWS - 1) {
        y_target = 0;
      }
      alive_neighbours += cell[x_target][y_target];
    }
  }
  // we have counted the cell we are evaluating
  // so we need to subtract that
  alive_neighbours -= cell[x][y];

  // now we have counted the living neighbours we know whether cell[x][y]
  // will be dead or alive in the next generation
  // a dead cell comes alive if it has three living neighbours
  if (cell[x][y] == 0 && alive_neighbours == 3) {
    return 1;
  }

  // an alive cell dies if it has more than 3 alive neighbours
  if (cell[x][y] == 1 && alive_neighbours > 3) {
    return 0;
  }

  // an alive cell stays alive if it has 2 or 3 alive neighbours
  if (cell[x][y] == 1 &&  (alive_neighbours == 2 || alive_neighbours == 3)) {
    return 1;

  }
  // otherwise the cell dies (or stays dead)
  return 0;
}

void set_random_world() {
  for (int8_t x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
    for (int8_t y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
      cell[x][y] = (random(2) ? 1 : 0);
    }
  }
}

void set_glider() {
  for (int8_t x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
    for (int8_t y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
      cell[x][y] = 0;
    }
  }
  cell[1][4] = 1;
  cell[2][3] = 1;
  cell[3][3] = 1;
  cell[3][4] = 1;
  cell[3][5] = 1;
}

void setup() {
  Serial.begin(9600);
  LedSign::Init(GRAYSCALE);  //Initializes the screen
  //set_glider();
  set_random_world();
}
void loop() {
  generation++;
  //Serial.println(generation);

  // reset the world from time to time
  if (all_dead()) {
    count++;
  }

  if (count > 10 or generation > 500) {
    count = generation = 0;
    set_random_world();
  }

  update_display();
  set_next_generation();

  // copy from next array to cell array
  for (int8_t x = 0; x < DISPLAY_COLS; x++) {   // 14 columns
    for (int8_t y = 0; y < DISPLAY_ROWS; y++) { // 9 rows
      cell[x][y] = next[x][y];
    }
  }
  delay(100);
}
