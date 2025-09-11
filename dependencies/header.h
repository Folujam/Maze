#ifndef HEADER_H
#define HEADER_H

#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* ----constants-----*/
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define MINI_CELL_SIZE 6
#define MINI_MAP_WIDTH (MAP_WIDTH * MINI_CELL_SIZE)
#define MINI_MAP_HEIGHT (MAP_HEIGHT * MINI_CELL_SIZE)
#define MINI_MAP_PADDING 8

/* ------traffic light------ */
#define MAX_SPIRITS 32

/* Traffic light timing (randomized between these ranges, seconds) */
#define GREEN_MIN 3.0
#define GREEN_MAX 6.0
#define RED_MIN   2.0
#define RED_MAX   4.0


/*================ STRUCTS================*/

typedef struct Ghost
{
    double x, y;     /* position (map units) */
    int active;       /* 1 = active, 0 = collected */
    int chasing;       /* 1 = chasing, 0 = not chasing */
    double chaseTimer; /* time spent chasing */
}Ghost;


/* ----------------- Spirit type ---------------- */
typedef struct Spirit
{
    double x, y;     /* position (map units) */
    double vx, vy;   /* velocity */
    int alive;       /* 1 = active, 0 = collected */
} Spirit;
/* --------------SDL instance-------------*/
/**
* struct SDL_instance- struct to hold window && renderer
* this struct handles the sdlinstance to be shown on screen
* @window: a window to be created
* @renderer: vector to visualizer
*/
typedef struct SDL_instance
{
	SDL_Window *window;
	SDL_Renderer *renderer;
} SDL_Instance;

/* ---------- global player	variable declarations (extern) --------- */
extern double posX, posY;
extern double dirX, dirY;
extern double planeX, planeY;
extern double moveSpeed, rotSpeed;
extern int playerScore;
extern int playerCollected;

/*-------- Map declaration (extern) ------------ */
extern int worldMap[MAP_WIDTH][MAP_HEIGHT];


/* ------------ Init/loop -------- */
int init_instance(SDL_Instance *instance);
int poll_events(void);
void render_scene(SDL_Instance *instance);
void draw_minimap(SDL_Instance *instance);

/* ------------- Gameplay (spirits + traffic light) -------------- */
void init_gameplay(void);
void update_gameplay(double dt);

/* traffic light: movement always allowed; penalties handled on move */
int get_traffic_green(void);        /* 1=green, 0=red */
void notify_player_moved(void);     /* call once per player move (for red penalty) */

/*---------- spirits accessors---------- */
int  get_spirits(const Spirit **out);   /* returns count */
void try_collect_spirit(double px, double py); /* call after player moves */
int  get_remaining_spirits(void);/* alive count */

/* Stage handling */
int  get_stage(void);
void init_maze_stage(void);
void update_ghosts(double dt);
const Ghost *get_ghosts(int *count);


#endif
