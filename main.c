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

long FRAME = 0;
int GAME_OVER = 0;
int SCORE = 0;

void draw_grid(void);
void draw_snake(snake *);
void move(snake *);
void move_head(snake *);
void move_body(snake *);
void set_direction(snake *);
int wall_collision(snake *);
void draw_fruit(fruit *);
int fruit_collision(snake *, fruit *);
void handle_collision(snake *, fruit *);
int get_random_x(void);
int get_random_y(void);
void grow_snake(snake *);
int self_collision(snake *);
void deallocate_snake_mem(snake *);

int main()
{
  InitWindow(SCREENWIDTH, SCREENHEIGHT, "snake");

  int start_pos = TILES / 2 * TILE_WIDTH;
  snake player = {start_pos, start_pos, start_pos, start_pos, TILE_WIDTH, TILE_WIDTH, RIGHT, NULL};
  fruit fruit = {get_random_x(), get_random_y()};
  SetTargetFPS(60); // Set our game to run at 60 FRAMEs-per-second

  // Main game loop
  while (!WindowShouldClose()) // Detect window close button or ESC key
  {
    // Update
    set_direction(&player);
    move(&player);
    handle_collision(&player, &fruit);

    // Draw
    BeginDrawing();
    if (GAME_OVER)
    {
      ClearBackground(RAYWHITE);
      DrawText("GAME OVER", SCREENWIDTH / 2 - 70, SCREENHEIGHT / 2 - 100, 24, BLACK);
    }
    else
    {
      ClearBackground(RAYWHITE);
      draw_snake(&player);
      draw_fruit(&fruit);
      draw_grid();
      FRAME++;
    }
    char score_string[4];
    itoa(SCORE, score_string, 10);
    DrawText(score_string, SCREENWIDTH / 2, SCREENHEIGHT / 2, 24, BLACK);
    EndDrawing();
  }

  CloseWindow();

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
  //Draws the head of the snake
  DrawRectangle(s->x, s->y, s->width, s->height, DARKGREEN);
  // Draws body of snake
  while (s->tail != NULL)
  {
    s = s->tail;
    DrawRectangle(s->x, s->y, s->width, s->height, GREEN);
  }
}

void move(snake *s)
{
  if (FRAME % 10 == 0)
  {
    move_head(s);
    move_body(s);
  }
}

void move_head(snake *s)
{
  switch (s->dir)
  {
  case UP:
    s->prev_y = s->y;
    s->prev_x = s->x;
    s->y -= TILE_WIDTH;
    break;
  case DOWN:
    s->prev_y = s->y;
    s->prev_x = s->x;
    s->y += TILE_WIDTH;
    break;
  case LEFT:
    s->prev_x = s->x;
    s->prev_y = s->y;
    s->x -= TILE_WIDTH;
    break;
  case RIGHT:
    s->prev_x = s->x;
    s->prev_y = s->y;
    s->x += TILE_WIDTH;
    break;
  default:
    break;
  }
}

void move_body(snake *s)
{
  while (s->tail != NULL)
  {
    s->tail->prev_x = s->tail->x;
    s->tail->prev_y = s->tail->y;
    s->tail->x = s->prev_x;
    s->tail->y = s->prev_y;
    s = s->tail;
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
  }
  return 0;
}

void handle_collision(snake *s, fruit *f)
{
  if (wall_collision(s) || self_collision(s))
  {
    GAME_OVER = 1;
    return;
  }
  if (fruit_collision(s, f))
  {
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
  // append a new snake struct to the tail
  s->tail = new_tail;
  new_tail->x = s->prev_x;
  new_tail->y = s->prev_y;
  new_tail->prev_x = new_tail->x;
  new_tail->prev_y = new_tail->y;
  new_tail->width = s->width;
  new_tail->height = s->height;
  new_tail->tail = NULL;
  SCORE++;
}

int self_collision(snake *s)
{
  int head_x = s->x;
  int head_y = s->y;
  while (s->tail != NULL)
  {
    s = s->tail;
    if (s->x == head_x && s->y == head_y)
    {
      return 1;
    }
  }
  return 0;
}
