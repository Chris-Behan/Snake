#include "raylib.h"
#include "stdlib.h"

//----------------------------------------------------------------------------------
// Some Defines
//----------------------------------------------------------------------------------
#define SCREENWIDTH 456
#define SCREENHEIGHT 456
#define TILES 12
#define TILE_WIDTH SCREENWIDTH / TILES

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef enum
{
  UP,
  DOWN,
  LEFT,
  RIGHT
} direction;

typedef struct
{
  int x;
  int y;
  int size;
  int width;
  int height;
  direction dir;
} snake;

typedef struct
{
  int x;
  int y;
  int eaten;
} fruit;
//------------------------------------------------------------------------------------
// Global Variables Declaration
//------------------------------------------------------------------------------------

int board[TILES][TILES];
long frame = 0;
int game_over = 0;
//------------------------------------------------------------------------------------
// Module Functions Declaration (local)
//------------------------------------------------------------------------------------
void draw_grid(void);
void draw_snake(snake *);
void move_snake(snake *);
void set_direction(snake *);
int wall_collision(snake *);
void draw_fruit(fruit *);
int fruit_collision(snake *, fruit *);
void handle_collision(snake *, fruit *);

int main()
{
  // Initialization
  //--------------------------------------------------------------------------------------

  InitWindow(SCREENWIDTH, SCREENHEIGHT, "snake");

  snake player = {TILES / 2 * TILE_WIDTH, TILES / 2 * TILE_WIDTH, 1, TILE_WIDTH, TILE_WIDTH, RIGHT};
  fruit fruit = {rand() % TILES, rand() % TILES, 0};
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
      //ClearBackground(RAYWHITE);
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
  //if (wall_collision(s))
  //{
  //  game_over = 1;
  //  return;
  //}
  DrawRectangle(s->x, s->y, s->width, s->height, MAGENTA);
}

void move_snake(snake *s)
{
  if (frame % 10 == 0)
  {
    switch (s->dir)
    {
    case UP:
      s->y -= TILE_WIDTH;
      break;
    case DOWN:
      s->y += TILE_WIDTH;
      break;
    case LEFT:
      s->x -= TILE_WIDTH;
      break;
    case RIGHT:
      s->x += TILE_WIDTH;
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
    f->x = rand() % TILES;
    f->y = rand() % TILES;
  }
  DrawRectangle(f->x * TILE_WIDTH, f->y * TILE_WIDTH, TILE_WIDTH, TILE_WIDTH, RED);
}

int fruit_collision(snake *s, fruit *f)
{
  if (s->x == f->x && s->y == f->y)
  {
    s->size += 1;
    f->eaten = 1;
  }
}
void handle_collision(snake *s, fruit *f)
{
  if (wall_collision(s))
  {
    game_over = 1;
    return;
  }
}