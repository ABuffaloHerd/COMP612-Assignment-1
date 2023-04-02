#include "UpdateFunctions.h"
#include "Shape.h"
#include "LinkedList.h"
#include "Particle.h"
#include "Earthquake.h"
#include <math.h>

// bad code
extern const float FRAME_TIME_SEC;
extern unsigned int frameStartTime;
extern LinkedList* rlistfg;
extern LinkedList* rlistbg;
extern ParticleSys* ps_explode;
extern EARTHQUAKE camshake;

extern void earfquak(int);

void collapse_snowman();

void update_spyballoon(Shape* sballoon)
{
	if (sballoon->pos[0] < -1.0f - sballoon->scale * 2)
	sballoon->pos[0] = 1.0f + sballoon->scale;
	else
	{
		sballoon->pos[0] -= 0.1f * FRAME_TIME_SEC;

		float offset = 0.02 * sin(0.001 * frameStartTime) * FRAME_TIME_SEC;
		sballoon->pos[1] += offset;
	}
}

void update_missile(Shape* missile)
{
	Shape* sballoon = find(rlistbg, "spyballoon");
	missile->pos[1] = sballoon->pos[1];
	if (missile->enabled)
	{
		missile->pos[0] += 2.5f * FRAME_TIME_SEC;

		if (missile->pos[0] > sballoon->pos[0])
		{
			trigger(ps_explode);
			missile->enabled = 0;
			sballoon->enabled = 0;
		}
	}
}

void update_overlay(Shape* ov)
{
	// don't update unless enabled
	if (!ov->enabled)
		return;
	if (ov->colour[0][3] < 1.0f)
		ov->colour[0][3] += 0.01f;

	printf("overlay update\n");
}

// 10 second duration so 600 frames
void update_meteor(Shape* meteor)
{
	static unsigned int lifetime = 0;
	if (!meteor->enabled)
		return;
	if (lifetime == 0)
	{
		meteor->pos[0] = -1.0f;
		meteor->pos[1] = 1.0f;
	}

	if (lifetime == 200)
		earfquak(120);

	meteor->pos[0] += 0.3f * FRAME_TIME_SEC;
	meteor->pos[1] += -0.3f * FRAME_TIME_SEC;
	
	if (lifetime == 400)
	{
		earfquak(120);
		Shape* ov = find(rlistfg, "overlay");
		ov->enabled = 1;
		ov->colour[0][3] = 0.0f;
		collapse_snowman();
	}

	if (lifetime > 600)
	{
		meteor->enabled = 0;
		lifetime = 0;
		printf("Meteor expire\n");
		find(rlistfg, "overlay")->enabled = 0;
	}
	else
		lifetime++;
}

void collapse_snowman()
{

}