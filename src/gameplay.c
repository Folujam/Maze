#include "../dependencies/header.h"

/* Traffic light state */
static int TL_green = 1;          /* start green */
static double TL_timer = 0.0;
static double TL_target = 0.0;

/* Spirits */
static Spirit spirits[MAX_SPIRITS];
static int spiritsCount = 0;

/* Forward */
static double rand_range(double a, double b) {
    double t = (double)rand() / (double)RAND_MAX;
    return a + t * (b - a);
}
static void pick_next_phase(void) {
    if (TL_green) TL_target = rand_range(GREEN_MIN, GREEN_MAX);
    else          TL_target = rand_range(RED_MIN, RED_MAX);
}

void init_gameplay(void)
{
    srand((unsigned int)time(NULL));

    /* traffic light */
    TL_green = 1;
    TL_timer = 0.0;
    pick_next_phase();

    /* place some spirits in open cells */
    spiritsCount = 8; /* tune per stage */
    int placed = 0;
    for (int tries = 0; tries < 500 && placed < spiritsCount; ++tries) {
        int x = 2 + rand() % (MAP_WIDTH - 4);
        int y = 2 + rand() % (MAP_HEIGHT - 4);
        if (worldMap[x][y] == 0) {
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
    TL_timer += dt;
    if (TL_timer >= TL_target)
    {
        TL_timer = 0.0;
        TL_green = !TL_green;
        pick_next_phase();
    }

    for (int i = 0; i < spiritsCount; ++i)
    {
        if (!spirits[i].alive) continue;

        double nx = spirits[i].x + spirits[i].vx * dt;
        double ny = spirits[i].y + spirits[i].vy * dt;

        /* bounce off walls */
        if (nx < 1 || nx >= MAP_WIDTH-1 || worldMap[(int)nx][(int)spirits[i].y])
            spirits[i].vx = -spirits[i].vx; 
        else
            spirits[i].x = nx;

        if (ny < 1 || ny >= MAP_HEIGHT-1 || worldMap[(int)spirits[i].x][(int)ny])
        {
            spirits[i].vy = -spirits[i].vy;
        }
        else
        {
            spirits[i].y = ny;
        }
    }

}

int get_traffic_green(void) { return TL_green; }

void notify_player_moved(void)
{
    /* If player moves on RED, deduct 2 points */
    if (!TL_green) playerScore -= 2;
}

/* called after movement to see if a spirit is collected */
void try_collect_spirit(double px, double py)
{
    const double COLLECT_R2 = 0.25 * 0.25; /* 0.25 radius */
    for (int i = 0; i < spiritsCount; ++i) {
        if (!spirits[i].alive) continue;
        double dx = px - spirits[i].x;
        double dy = py - spirits[i].y;
        if (dx*dx + dy*dy <= COLLECT_R2) {
            spirits[i].alive = 0;
            playerScore += 3;
        }
    }
}

int get_spirits(const Spirit **out)
{
    if (out) *out = spirits;
    return spiritsCount;
}

int get_remaining_spirits(void)
{
    int left = 0;
    for (int i = 0; i < spiritsCount; ++i)
        if (spirits[i].alive) left++;
    return left;
}
