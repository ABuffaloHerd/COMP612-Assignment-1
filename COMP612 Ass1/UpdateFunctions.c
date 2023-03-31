#include "UpdateFunctions.h"
#include "Shape.h"
#include "LinkedList.h"
#include "Particle.h"
#include <math.h>

// bad code
extern const float FRAME_TIME_SEC;
extern unsigned int frameStartTime;
extern LinkedList* rlistfg;
extern LinkedList* rlistbg;
extern ParticleSys* ps_explode;

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