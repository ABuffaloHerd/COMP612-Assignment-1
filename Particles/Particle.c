#include "Particle.h"
#include "freeglut.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define PARTICLE_DIFF 2

#define PARTICLE_TYPE_DUST 1 << 1
#define PARTICLE_TYPE_SPARK 1 << 2

extern const float FRAME_TIME_SEC;

Particle* new_particle_snow(void);
void activate(ParticleSys* ps, int count);
void deactivate(ParticleSys* ps, int count);
void recycle_particle_snow(Particle* p);

ParticleSys* new_particle_system(void)
{
	ParticleSys* ps = (ParticleSys*)malloc(sizeof(ParticleSys));

	ps->active = 0;

	// insert particles
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		ps->particles[x] = new_particle_snow();
	}

	//ps->particles[0]->active = 1;
	ps->target = 0;
	ps->center[0] = -1.0f;
	ps->center[1] = -1.0f;

	return ps;
}

Particle* new_particle_snow(void)
{
	Particle* p = (Particle*)malloc(sizeof(Particle));

	// setup particle
	p->active = 0;
	p->pos[1] = 1.1f;

	p->colour[0] = 1.0f;
	p->colour[1] = 1.0f;
	p->colour[2] = 1.0f;
	p->colour[3] = 1.0f;

	// randomized values
	p->pos[0] = (float)rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
	p->dx = (float)rand() / ((float)RAND_MAX / 0.2f) - 1.0f;
	p->mass = rand() % 11; // between 0 and 11

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
		activate(ps, PARTICLE_DIFF);
	}
	else if (ps->active > ps->target)
		deactivate(ps, PARTICLE_DIFF);

	// update every particle
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		Particle* current = ps->particles[x];
		if (!current->active)
			continue;

		current->pos[1] -= current->dy * FRAME_TIME_SEC;
		current->lifetime++;

		if (current->pos[1] <= -1.2f)
		{
			recycle_particle_snow(ps->particles[x]);
		}
		//printf("Particle information:\n");
		//printf("pos: %f %f, size: %f\n", current->pos[0], current->pos[1], current->size);
	}

}

void activate(ParticleSys* ps)
{
	int index = 0;
	// find the first inactive particle and activate it.
	while (ps->particles[index]->active)
		index++;

	ps->particles[index]->active = 1;
	ps->active++;
}

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

	p->colour[0] = 1.0f;
	p->colour[1] = 1.0f;
	p->colour[2] = 1.0f;
	p->colour[3] = 1.0f;

	// randomized values
	p->pos[0] = (float)rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
	p->dx = (float)rand() / ((float)RAND_MAX / 0.2f) - 1.0f;
	p->mass = rand() % 11; // between 0 and 10

	// based on mass, calculate a size and dY
	p->size = (float)p->mass * 0.5f;
	p->dy = p->mass * 0.1f;
}

/**********************
* EXPLOSION PARTICLES *
***********************/

void update_particle_explode(ParticleSys* ps)
{
	// update the particle system

	if (ps->active < ps->target)
	{
		activate(ps, PARTICLE_DIFF);
	}
	else if (ps->active > ps->target)
		deactivate(ps, PARTICLE_DIFF);

	// update every particle
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		Particle* current = ps->particles[x];
		if (!current->active)
			continue;

		current->pos[0] += current->dx * FRAME_TIME_SEC;
		current->pos[1] -= current->dy * FRAME_TIME_SEC;
		current->lifetime++;

		if (current->pos[1] <= -1.2f)
		{
			//recycle_particle_snow(ps->particles[x]);
		}
		//printf("Particle information:\n");
		//printf("pos: %f %f, size: %f\n", current->pos[0], current->pos[1], current->size);
	}
}

Particle* new_particle_explode(ParticleSys* ps, unsigned int type)
{
	Particle* p = (Particle*)malloc(sizeof(Particle));

	// setup particle
	p->active = 0;
	p->pos[1] = 1.1f;

	// dust or spark?
	if (type & PARTICLE_TYPE_DUST)
	{
		p->colour[0] = 0.9f;
		p->colour[1] = 0.9f;
		p->colour[2] = 0.9f;
		p->colour[3] = 1.0f;
	}
	else
	{
		p->colour[0] = 0.980f;
		p->colour[1] = 0.098f;
		p->colour[2] = 0.098f;
		p->colour[3] = 1.0f;
	}

	// randomized values
	p->pos[0] = (float)rand() / ((float)RAND_MAX / 2.0f) - 1.0f;
	p->dx = (float)rand() / ((float)RAND_MAX / 0.2f) - 1.0f;
	p->mass = rand() % 11 + 1; // between 1 and 11

	// based on mass, calculate a size and dY
	p->size = (float)p->mass * 0.5f;
	p->dy = p->mass * 0.1f;

	return p;
}