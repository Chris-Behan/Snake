#include "raylib.h"
#include "stdlib.h"
#include "stdio.h"

#define SCREENWIDTH 456
#define SCREENHEIGHT 456
#define TILES 12
#define TILE_WIDTH SCREENWIDTH / TILES

typedef enum
{
  UP,
  DOWN,
  LEFT,
  RIGHT
} direction;

typedef struct snake
{
  int x;
  int y;
  int prev_x;
  int prev_y;
  int size;
  int width;
  int height;
  direction dir;
  struct snake *tail;
} snake;

typedef struct
{
  int x;
  int y;
  int eaten;
} fruit;

int board[TILES][TILES];
long frame = 0;
int game_over = 0;

void draw_grid(void);
void draw_snake(snake *);
void move_snake(snake *);
void set_direction(snake *);
int wall_collision(snake *);
void draw_fruit(fruit *);
int fruit_collision(snake *, fruit *);
void handle_collision(snake *, fruit *);
int get_random_x(void);
int get_random_y(void);
void grow_snake(snake *);
int main()
{
  // Initialization
  //--------------------------------------------------------------------------------------

  InitWindow(SCREENWIDTH, SCREENHEIGHT, "snake");

  snake player = {TILES / 2 * TILE_WIDTH, TILES / 2 * TILE_WIDTH, 1, 0, 0, TILE_WIDTH, TILE_WIDTH, RIGHT, NULL};
  fruit fruit = {get_random_x(), get_random_y()};
  SetTargetFPS(60); // Set our game to run at 60 frames-per-second
  //--------------------------------------------------------------------------------------

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    //----------------------------------------------------------------------------------

    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();
    if (game_over)
    {
      ClearBackground(RAYWHITE);
      DrawText("GAME OVER", SCREENWIDTH / 2 - 70, SCREENHEIGHT / 2 - 100, 24, BLACK);
    }
    else
    {
      ClearBackground(RAYWHITE);
      draw_grid();
      draw_snake(&player);
      draw_fruit(&fruit);
      handle_collision(&player, &fruit);
      frame++;
    }

    EndDrawing();
    //----------------------------------------------------------------------------------
  }

  // De-Initialization
  //--------------------------------------------------------------------------------------
  CloseWindow(); // Close window and OpenGL context
  //--------------------------------------------------------------------------------------

  return 0;
}

void draw_grid(void)
{
  for (int i = 0; i < TILES; i++)
  {
    DrawLine((SCREENWIDTH / TILES * i), 0, (SCREENHEIGHT / TILES * i), SCREENHEIGHT, BLACK);
    DrawLine(0, (SCREENHEIGHT / TILES * i), SCREENWIDTH, (SCREENHEIGHT / TILES * i), BLACK);
  }
}

void draw_snake(snake *s)
{
  set_direction(s);
  move_snake(s);
  DrawRectangle(s->x, s->y, s->width, s->height, MAGENTA);
  while (s->tail != NULL)
  {
    int new_x = s->prev_x;
    int new_y = s->prev_y;
    s = s->tail;
    s->prev_x = s->x;
    s->prev_y = s->y;
    s->x = new_x;
    s->y = new_y;
    DrawRectangle(s->x, s->y, s->width, s->height, MAGENTA);
  }
}

void move_snake(snake *s)
{
  if (frame % 10 == 0)
  {
    switch (s->dir)
    {
    case UP:
      s->prev_y = s->y;
      s->y -= TILE_WIDTH;
      s->prev_x = s->x;
      break;
    case DOWN:
      s->prev_y = s->y;
      s->y += TILE_WIDTH;
      s->prev_x = s->x;
      break;
    case LEFT:
      s->prev_x = s->x;
      s->x -= TILE_WIDTH;
      s->prev_y = s->y;
      break;
    case RIGHT:
      s->prev_x = s->x;
      s->x += TILE_WIDTH;
      s->prev_y = s->y;
      break;
    default:
      break;
    }
  }
}

void set_direction(snake *s)
{
  if (IsKeyDown(KEY_UP))
  {
    s->dir = UP;
  }
  else if (IsKeyDown(KEY_DOWN))
  {
    s->dir = DOWN;
  }
  else if (IsKeyDown(KEY_LEFT))
  {
    s->dir = LEFT;
  }
  else if (IsKeyDown(KEY_RIGHT))
  {
    s->dir = RIGHT;
  }
}

int wall_collision(snake *s)
{
  if (s->x < 0)
  {
    return 1;
  }
  if (s->x >= TILES * TILE_WIDTH)
  {
    return 1;
  }
  if (s->y < 0)
  {
    return 1;
  }
  if (s->y >= TILES * TILE_WIDTH)
  {
    return 1;
  }
  return 0;
}

void draw_fruit(fruit *f)
{
  if (f->eaten)
  {
    f->x = get_random_x();
    f->y = get_random_y();
  }
  DrawRectangle(f->x, f->y, TILE_WIDTH, TILE_WIDTH, RED);
}

int fruit_collision(snake *s, fruit *f)
{
  if (s->x == f->x && s->y == f->y)
  {
    return 1;
    //s->size += 1;
    //f->eaten = 1;
  }
  return 0;
}
void handle_collision(snake *s, fruit *f)
{
  if (wall_collision(s))
  {
    game_over = 1;
    return;
  }
  if (fruit_collision(s, f))
  {
    s->size += 1;
    f->eaten = 1;
    grow_snake(s);
  }
  else
  {
    f->eaten = 0;
  }
}

int get_random_x(void)
{
  return rand() % TILES * TILE_WIDTH;
}

int get_random_y(void)
{
  return rand() % TILES * TILE_WIDTH;
}

void grow_snake(snake *s)
{
  // loop through every part of the snake until we are at its tail
  while (s->tail != NULL)
  {
    s = s->tail;
  }
  struct snake *new_tail = (struct snake *)malloc(sizeof(*new_tail));
  s->tail = new_tail;
  new_tail->x = s->x;
  new_tail->y = s->y;
  new_tail->prev_x = s->prev_x;
  new_tail->prev_y = s->prev_y;
  new_tail->width = s->width;
  new_tail->height = s->height;
  new_tail->size = s->size;
  new_tail->dir = s->dir;
  new_tail->tail = NULL;
}