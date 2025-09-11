#include "../dependencies/header.h"

/**
 * 
 */

static int stage = 1;          /* current stage */
static int stageAttempts = 0;  /* failed attempts at current maze */
static Ghost ghosts[3];   /* array of ghosts */
static int doorX = 0, doorY = 0; /* door position */
static void place_maze_ghosts(void); /* place ghosts in maze */

/*-------API-------*/
int get_stage(void)
{
    return stage;
}

void init_maze_stage(void)
{
    stageAttempts = 0;
    int y, x, i;

    for (y = 0; y < MAP_HEIGHT; y++)
    {
        for (x = 0; x < MAP_WIDTH; x++)
        {
            if (worldMap[x][y] == 2) // door
            {
                doorX = x;
                doorY = y;
            }
        }
    }
    for (y = 2; y < MAP_HEIGHT - 2; ++y)
    worldMap[MAP_WIDTH/2][y] = 0; /* create a path in the middle */

    /* ----- place door at far end of corridor -----*/
    doorX = MAP_WIDTH/2;
    doorY = MAP_HEIGHT - 3;
    worldMap[doorX][doorY] = 2; /* place door --mark door with special value-- */

    for (i = 0; i < 13; i++)
    {
        int len = 3 + rand() % 5; /* lenght of turn*/
        int y = 3 + rand() % (MAP_HEIGHT - 6);
        int dir = (rand() % 2) ? 1 : -1; /* direction of turn */
        for (int x = MAP_WIDTH/2; x != MAP_WIDTH/2 + dir*len; x += dir) {
            if (x > 1 && x < MAP_WIDTH-2)
                worldMap[x][y] = 0;
        }
    }

    /* Place 3 ghosts at random wrong turns */
    place_maze_ghosts();
}
/* --------------------------------------------------- */
/* Place ghosts at random dead-ends */
static void place_maze_ghosts(void)
{
    for (int i = 0; i < 3; i++) {
        ghosts[i].active = 0;
        ghosts[i].chasing = 0;
    }

    int placed = 0;
    while (placed < 3) {
        int x = 2 + rand() % (MAP_WIDTH - 4);
        int y = 2 + rand() % (MAP_HEIGHT - 4);

        if (worldMap[x][y] == 0) {
            ghosts[placed].x = x + 0.5;
            ghosts[placed].y = y + 0.5;
            ghosts[placed].active = 1;
            ghosts[placed].chasing = 0;
            ghosts[placed].chaseTimer = 0.0;
            placed++;
        }
    }
}

/* --------------------------------------------------- */
/* Update ghosts: chase player for 6s, kill on contact, reset maze on fails */
void update_ghosts(double dt)
{
    for (int i = 0; i < 3; i++) {
        if (!ghosts[i].active) continue;

        double dx = posX - ghosts[i].x;
        double dy = posY - ghosts[i].y;
        double dist2 = dx*dx + dy*dy;

        if (!ghosts[i].chasing && dist2 < 9.0) { /* detect within 3 units */
            ghosts[i].chasing = 1;
            ghosts[i].chaseTimer = 6.0;
        }

        if (ghosts[i].chasing) {
            double len = sqrt(dx*dx + dy*dy);
            if (len > 0.1) {
                ghosts[i].x += (dx/len) * 0.05; /* ghost speed */
                ghosts[i].y += (dy/len) * 0.05;
            }
            ghosts[i].chaseTimer -= dt;
            if (ghosts[i].chaseTimer <= 0.0) {
                ghosts[i].chasing = 0;
            }
        }

        /* Collision: player dies */
        if (dist2 < 0.25) {
            stageAttempts++;
            if (stageAttempts >= 2) {
                init_maze_stage(); /* rearrange maze */
            } else {
                /* reset player at maze start */
                posX = 2.5; posY = 2.5;
            }
        }
    }

    /* Check if player reached door → next stage */
    if ((int)posX == doorX && (int)posY == doorY) {
        stage++;
        // TODO: call next stage initializer
        // e.g., init_gameplay(); or another init_maze_stage();
    }
}

/* --------------------------------------------------- */
/* Access for rendering */
const Ghost *get_ghosts(int *count)
{
    if (count) *count = 3;
    return ghosts;
}