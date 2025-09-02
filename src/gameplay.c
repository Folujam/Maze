#include "../dependencies/header.h"

extern int playerScore;

static int trafficGreen = 1;
static double trafficTimer = 0.0;
static double trafficPhaseDur = T_GREEN;

static Obstacle obs[MAX_OBS];
static int obsCount = 0;

void init_gameplay(void)
{
    /* start green */
    trafficGreen = 1;
    trafficTimer = 0.0;
    trafficPhaseDur = T_GREEN;

    /* spawn a few obstacles in open space */
    obsCount = 5;
    /* pick some safe starting spots */
    double starts[5][4] = {
        {8, 8,  0.7,  0.4},
        {12, 6, -0.5,  0.6},
        {6, 14, 0.8,  -0.3},
        {16, 10,-0.6, -0.5},
        {10, 16,0.5,   0.7}
    };
    for (int i = 0; i < obsCount; ++i) {
        obs[i].x = starts[i][0];
        obs[i].y = starts[i][1];
        obs[i].vx = starts[i][2];
        obs[i].vy = starts[i][3];
        obs[i].r = 0.35;
    }
}

static int is_wall(double x, double y)
{
    if (x < 0 || x >= MAP_WIDTH || y < 0 || y >= MAP_HEIGHT) return 1;
    return worldMap[(int)x][(int)y] != 0;
}

void update_gameplay(double dt)
{
    /* traffic logic */
    trafficTimer += dt;
    if (trafficTimer >= trafficPhaseDur) {
        trafficTimer = 0.0;
        trafficGreen = !trafficGreen;
        trafficPhaseDur = trafficGreen ? T_GREEN : T_RED;
    }

    /* move obstacles */
    for (int i = 0; i < obsCount; ++i)
    {
        double nx = obs[i].x + obs[i].vx * dt;
        double ny = obs[i].y + obs[i].vy * dt;

        /* bounce on walls */
        if (!is_wall(nx, obs[i].y))
            obs[i].x = nx;
        else 
            obs[i].vx = -obs[i].vx;
        if (!is_wall(obs[i].x, ny)) obs[i].y = ny; else obs[i].vy = -obs[i].vy;

        /* simple obstacle-obstacle bounce (very rough) */
        for (int j = i + 1; j < obsCount; ++j) 
        {
            double dx = obs[i].x - obs[j].x;
            double dy = obs[i].y - obs[j].y;
            double rr = obs[i].r + obs[j].r;
            if (dx*dx + dy*dy < rr*rr)
            {
                double tvx = obs[i].vx; obs[i].vx = obs[j].vx; obs[j].vx = tvx;
                double tvy = obs[i].vy; obs[i].vy = obs[j].vy; obs[j].vy = tvy;
            }
        }
    }
}

/* movement permission for player */
int can_player_move(void)
{
    /*return trafficGreen ? 1 : 0;*/
    return 1; /* always allow movement, but check collisions separately */
}

/* new: check traffic light penalty */
void check_traffic_violation(int moved)
{
    if (moved && !get_traffic_green()) 
    {
        playerScore -= 2;
    }
}

/* circle collision vs player point (treat player as point for simplicity) */
int will_collide_with_obstacles(double nx, double ny)
{
    for (int i = 0; i < obsCount; ++i) {
        double dx = nx - obs[i].x;
        double dy = ny - obs[i].y;
        /*if (dx*dx + dy*dy < (obs[i].r * obs[i].r)) return 1;*/
        double dist2 = dx*dx + dy*dy;
        if (dist2 < (obs[i].r * obs[i].r)) 
        {
            playerScore -= 2;
            return 1;
        }
    }
    return 0;
}

void check_passed_obstacles(double px, double py)
{
    for (int i = 0; i < obsCount; ++i) {
        double dx = px - obs[i].x;
        double dy = py - obs[i].y;
        double dist2 = dx*dx + dy*dy;
        if (dist2 > (obs[i].r * obs[i].r) * 6.0) { /* safely passed */
            playerScore += 3;
        }
    }
}
/* OPTIONAL: expose for rendering the HUD/minimap obstacles */
int get_traffic_green(void) { return trafficGreen; }
int get_obstacles(const Obstacle **out) { if (out) *out = obs; return obsCount; }
