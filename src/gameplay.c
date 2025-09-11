#include "../dependencies/header.h"
/**
 * Gameplay module: traffic light, spirits, scoring
 * 
 * Author: Firebr33da
 * 
 * This module handles the traffic light state (green/red),
 * spirits (moving collectibles), and scoring.
 * The player can move freely, but moving during red
 * deducts points. Collecting spirits adds points.
 * 
 * The traffic light switches between green and red
 * at random intervals within defined ranges.
 */
int playerCollected = 0;   /* number of spirits collected so far */

 /*--------------- Traffic light state --------------------*/
static int TL_green = 1;          /* start green */
static double TL_timer = 0.0; /* elapsed in current phase */
static double TL_target = 0.0; /* duration of current phase */
static Spirit spirits[MAX_SPIRITS];
/* Forward */
static double rand_range(double a, double b)
{
    double t = (double)rand() / (double)RAND_MAX;
    return a + t * (b - a);
}

static void pick_next_phase(void)
{
    if (TL_green) 
        TL_target = rand_range(GREEN_MIN, GREEN_MAX);
    else          
        TL_target = rand_range(RED_MIN, RED_MAX);
}

int get_traffic_green(void) { return TL_green; }

/* Called once **per actual player move**. If red, deduct 2 points. */

/* helper: respawn a new spirit into a free slot */
static void respawn_one_spirit(void)
{
    for (int i = 0; i < MAX_SPIRITS; i++) {
        if (!spirits[i].alive) {
            for (int tries = 0; tries < 200; ++tries) {
                int x = 2 + rand() % (MAP_WIDTH - 4);
                int y = 2 + rand() % (MAP_HEIGHT - 4);
                if (worldMap[x][y] == 0) {
                    spirits[i].x = x + 0.5;
                    spirits[i].y = y + 0.5;
                    spirits[i].vx = rand_range(-0.6, 0.6);
                    spirits[i].vy = rand_range(-0.6, 0.6);
                    spirits[i].alive = 1;
                    return;
                }
            }
        }
    }
}

void notify_player_moved(void)
{
    if (!TL_green) {
        if (playerCollected > 0) {
            playerCollected -= 1;   /* lose one collected */
            if (playerCollected < 0) playerCollected = 0;  /* never <0 */
            playerScore -= 3;
            if (playerScore < 0) playerScore = 0;

            respawn_one_spirit();   /* compensate immediately */
        }
    }
}

/* --------------------Spirits---------------------- */

static int spiritsCount = 0;

/**
 * init_gameplay - initializes gameplay elements
 * Return: void
 */
/* Called once **per actual player move**. If red, deduct 2 points. */
void init_gameplay(void)
{
    srand((unsigned int)time(NULL));

     /* set initial traffic-light phase */
    TL_green = 1;
    TL_timer = 0.0;
    pick_next_phase();

    /* place some spirits in open cells */
    spiritsCount = 8; /* tune per stage */
    int placed = 0;
    for (int tries = 0; tries < 500 && placed < spiritsCount; ++tries) {
        int x = 2 + rand() % (MAP_WIDTH - 4);
        int y = 2 + rand() % (MAP_HEIGHT - 4);
        if (worldMap[x][y] == 0)
        {
            spirits[placed].x = x + 0.5;
            spirits[placed].y = y + 0.5;
            spirits[placed].vx = rand_range(-0.6, 0.6);  /* random X speed */
            spirits[placed].vy = rand_range(-0.6, 0.6);  /* random Y speed */
            spirits[placed].alive = 1;
            placed++;
        }
    }
    spiritsCount = placed; /* in case fewer got placed */
}

void update_gameplay(double dt)
{
    /* advance traffic light */
    /*traffic light timer*/
    TL_timer += dt;
    if (TL_timer >= TL_target)
    {
        TL_timer = 0.0;
        TL_green = !TL_green;
        pick_next_phase();
    }

    /*----spirits update-----*/
    for (int i = 0; i < spiritsCount; ++i)
    {
        if (!spirits[i].alive)
            continue; /*never touch dead spirits*/

        double nx = spirits[i].x + spirits[i].vx * dt;
        double ny = spirits[i].y + spirits[i].vy * dt;

        /* bounce off walls */
        if (nx < 1 || nx >= MAP_WIDTH-1 || worldMap[(int)nx][(int)spirits[i].y])
            spirits[i].vx = -spirits[i].vx; 
        else
            spirits[i].x = nx;

        if (ny < 1 || ny >= MAP_HEIGHT-1 || worldMap[(int)spirits[i].x][(int)ny])
            spirits[i].vy = -spirits[i].vy;
        else
            spirits[i].y = ny;
    }

    
    /* check if all collected */
    if (get_remaining_spirits() == 0) {
        init_gameplay(); // reset stage with new spirits + light
        playerScore = 0; // reset collected (or keep cumulative, your choice)
    }


}


/* called after movement to see if a spirit is collected */
void try_collect_spirit(double px, double py)
{
    const double COLLECT_R2 = 0.70 * 0.70; /* 0.50 ( collection radius)*/
    for (int i = 0; i < spiritsCount; ++i)
    {
        if (!spirits[i].alive)
            continue;
        
        double dx = px - spirits[i].x;
        double dy = py - spirits[i].y;
        if (dx*dx + dy*dy <= COLLECT_R2)
        {
            spirits[i].alive = 0; /* collected */
            playerCollected += 1;
            playerScore += 3; /* +3 points */
            printf("Collected spirit %d at (%.2f, %.2f)\n", i, spirits[i].x, spirits[i].y);
            return;
        }
    }
}

int get_spirits(const Spirit **out)
{
    if (out) 
        *out = spirits;
        return spiritsCount;
}

int get_remaining_spirits(void)
{
    int left = 0;
    for (int i = 0; i < spiritsCount; ++i)
        if (spirits[i].alive)
            left++;
    return left;
}
