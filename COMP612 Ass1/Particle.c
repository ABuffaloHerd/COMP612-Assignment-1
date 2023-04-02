#include "Particle.h"
#include "freeglut.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define PARTICLE_DIFF 2

#define PARTICLE_TYPE_DUST 1 << 1
#define PARTICLE_TYPE_SPARK 1 << 2

extern const float FRAME_TIME_SEC;

// spaghetti mountain
Particle* new_particle_snow(void);
Particle* new_particle_explode(float px, float py, ParticleType type);
void activate(ParticleSys* ps);
void deactivate(ParticleSys* ps);
void recycle_particle_snow(Particle* p);
void recycle_particle_explode(Particle* p, float px, float py);
void update_particle_snow(ParticleSys* ps);
void update_particle_explode(ParticleSys* ps);

void update_particle_system(ParticleSys* ps)
{
	if (ps->systype == T_SNOW)
		update_particle_snow(ps);
	else
		update_particle_explode(ps);
}


ParticleSys* new_particle_system(SystemType t)
{
	ParticleSys* ps = (ParticleSys*)malloc(sizeof(ParticleSys));

	ps->active = 0;

	ps->target = 0;
	ps->center[0] = -1.0f;
	ps->center[1] = -1.0f;

	if (t == T_EXPLODE)
	{
		// insert particles
		for (int x = 0; x < MAX_PARTICLES - 300; x++)
		{
			ps->particles[x] = new_particle_explode(ps->center[0], ps->center[1], DUST);
		}

		for (int x = MAX_PARTICLES - 300; x < MAX_PARTICLES; x++)
		{
			ps->particles[x] = new_particle_explode(ps->center[0], ps->center[1], SPARK);
		}

		ps->systype = T_EXPLODE;
	}
	else
	{
		for (int x = 0; x < MAX_PARTICLES; x++)
		{
			ps->particles[x] = new_particle_snow();
		}

		ps->systype = T_SNOW;
	}
	return ps;
}

Particle* new_particle_snow(void)
{
	Particle* p = (Particle*)malloc(sizeof(Particle));

	// setup particle
	p->active = 0;
	p->pos[1] = 1.0f;

	p->colour[0] = 1.0f;
	p->colour[1] = 1.0f;
	p->colour[2] = 1.0f;

	// randomized values
	p->pos[0] = (float)rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
	p->dx = (float)rand() / ((float)RAND_MAX / 0.2f) - 1.0f;
	p->mass = rand() % 11 + 1; // between 0 and 11
	p->colour[3] = ((float)rand() / (float)RAND_MAX) * 0.8f + 0.2f;

	// based on mass, calculate a size and dY
	p->size = (float)p->mass * 0.5f;
	p->dy = p->mass * 0.1f;

	return p;
}

void render_particle_system(ParticleSys* ps)
{
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		Particle* current = ps->particles[x];

		// Skip?
		if (!current->active)
		{
			continue;
		}

		// set stuff
		glColor4fv(current->colour);
		glPointSize(current->size);

		// Draw
		glBegin(GL_POINTS);
		glVertex2fv(current->pos);
		glEnd();
	}

}

void update_particle_snow(ParticleSys* ps)
{
	// update the particle system

	if (ps->active < ps->target)
	{
		activate(ps);
	}
	else if (ps->active > ps->target)
		deactivate(ps);

	// update every particle
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		Particle* current = ps->particles[x];
		if (!current->active)
			continue;

		current->pos[1] -= current->dy * FRAME_TIME_SEC;
		current->lifetime++;

		if (current->pos[1] <= -1.0f)
		{
			recycle_particle_snow(ps->particles[x]);
		}
		//printf("Particle information:\n");
		//printf("pos: %f %f, size: %f\n", current->pos[0], current->pos[1], current->size);
	}

}

// activates one particle per call
void activate(ParticleSys* ps)
{
	int index = 0;
	// find the first inactive particle and activate it.
	while (ps->particles[index]->active)
		index++;

	ps->particles[index]->active = 1;
	ps->active++;
}

// deactivates one particle per call
void deactivate(ParticleSys* ps)
{
	int index = 0;
	// first active particle
	while (!ps->particles[index]->active)
	{
		if (index + 1 > MAX_PARTICLES)
		{
			index = MAX_PARTICLES;
			break;
		}
		else index++;
	}

	ps->particles[index]->active = 0;
	ps->active--;
}

void set_density(ParticleSys* ps, int target)
{
	if (target > MAX_PARTICLES) target = MAX_PARTICLES;
	if (target < 0) target = 0;
	ps->target = target;

}

void recycle_particle_snow(Particle* p)
{
	// setup particle

	p->pos[1] = 1.1f;
	p->active = 1;

	p->colour[0] = 1.0f;
	p->colour[1] = 1.0f;
	p->colour[2] = 1.0f;

	// randomized values
	p->pos[0] = (float)rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
	p->dx = (float)rand() / ((float)RAND_MAX / 0.2f) - 1.0f;
	p->mass = rand() % 11; // between 0 and 10
	p->colour[3] = ((float)rand() / (float)RAND_MAX) * 0.8f + 0.2f;

	// based on mass, calculate a size and dY
	p->size = (float)p->mass * 0.7f;
	p->dy = p->mass * 0.1f;

	if (p->size < 0.1f)
	{
		// there was a printf here that said something rude when this condition was triggered
		// that's because without this check the snow would disappear into sub visible sizes.
		p->size = 0.1f;
	}

	if (p->dy < 0.1f)
	{
		// there was a printf here that said something rude when this condition was tirggered
		// that's because without this check the snow particles wouldn't move and therefore wouldn't appear
		p->dy = 1.0f;
	}
}

/**********************
* EXPLOSION PARTICLES *
***********************/

void update_particle_explode(ParticleSys* ps)
{
	// update every particle
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		Particle* current = ps->particles[x];
		if (!current->active) // reset position if inactive
		{
			recycle_particle_explode(current, ps->center[0], ps->center[1]);
			continue;
		}

		current->pos[0] += current->dx * FRAME_TIME_SEC;
		current->pos[1] += current->dy * FRAME_TIME_SEC;
		current->lifetime++;

		current->dx += current->ddx;
		current->dy += current->ddy;

		if (current->pos[1] <= -1.2f)
		{
			current->active = 0;
		}
		//printf("Particle information:\n");
		//printf("pos: %f %f, size: %f\n", current->pos[0], current->pos[1], current->size);
	}
}

Particle* new_particle_explode(float px, float py, ParticleType type)
{
	Particle* p = (Particle*)malloc(sizeof(Particle));

	// setup particle
	p->active = 0;
	p->pos[0] = px;
	p->pos[1] = py;

	// dust or spark?
	if (type == DUST)
	{
		p->colour[0] = 0.1f;
		p->colour[1] = 0.1f;
		p->colour[2] = 0.1f;
		p->colour[3] = 1.0f;
		p->size = (float)(rand() % 6 + 5);
		p->dy = ((float)rand() / RAND_MAX) * 18.0f - 9.0f;
		p->dx = ((float)rand() / RAND_MAX) * 18.0f - 9.0f;
		p->ddx = 0.0f;
		p->ddy = -0.3f;
		p->mass = 1;
		p->lifetime = 50;
		p->type = DUST;
	}
	else
	{
		p->colour[0] = 0.980f;
		p->colour[1] = 0.908f;
		p->colour[2] = 0.098f;
		p->colour[3] = 1.0f;
		p->size = (float)(rand() % 8 + 8);
		p->dy = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;
		p->dx = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;
		p->ddx = 0.0f;
		p->ddy = -0.5f;
		p->mass = 1;
		p->lifetime = 60;
		p->type = SPARK;
	}

	return p;
}

// Activates all particles at once
void trigger(ParticleSys* ps)
{
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		ps->particles[x]->active = 1;
	}
}

void recycle_particle_explode(Particle* p, float px, float py)
{
	// ok we do it the hard way
	p->active = 0;
	p->pos[0] = px;
	p->pos[1] = py;

	if (p->type == DUST)
	{
		p->colour[0] = 0.1f;
		p->colour[1] = 0.1f;
		p->colour[2] = 0.1f;
		p->colour[3] = 1.0f;
		p->size = (float)(rand() % 6 + 5);
		p->dy = ((float)rand() / RAND_MAX) * 18.0f - 9.0f;
		p->dx = ((float)rand() / RAND_MAX) * 18.0f - 9.0f;
		p->ddx = 0.0f;
		p->ddy = -0.2f;
		p->mass = 1;
		p->lifetime = 50;
	}
	else
	{
		p->colour[0] = 0.980f;
		p->colour[1] = 0.908f;
		p->colour[2] = 0.098f;
		p->colour[3] = 1.0f;
		p->size = (float)(rand() % 8 + 8);
		p->dy = ((float)rand() / RAND_MAX) * 10.0f - 5.0f;
		p->dx = ((float)rand() / RAND_MAX) * 14.0f - 7.0f;
		p->ddx = 0.0f;
		p->ddy = -0.45f;
		p->mass = 1;
		p->lifetime = 60;
	}
}