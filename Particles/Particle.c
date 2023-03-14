#include "Particle.h"
#include "freeglut.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define PARTICLE_INCREASE 2

extern const float FRAME_TIME_SEC;

Particle* new_particle(void);
void activate(ParticleSys* ps, int count);
void recycle_particle(Particle* p);

ParticleSys* new_particle_system(void)
{
	ParticleSys* ps = (ParticleSys*)malloc(sizeof(ParticleSys));

	ps->active = 0;
	
	// insert particles
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		ps->particles[x] = new_particle();
	}

	//ps->particles[0]->active = 1;
	ps->target = 0;

	return ps;
}

Particle* new_particle(void)
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
		if (current->active == 0)
		{
			//printf("Skipped particle\n");
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

void update_particle_system(ParticleSys* ps)
{
	// update the particle system

	if (ps->active != ps->target)
	{
		activate(ps, PARTICLE_INCREASE);
	}

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
			recycle_particle(ps->particles[x]);
		}
		//printf("Particle information:\n");
		//printf("pos: %f %f, size: %f\n", current->pos[0], current->pos[1], current->size);
	}

}

void activate(ParticleSys* ps, int count)
{
	int index = 0;

	if (ps->active + count > MAX_PARTICLES)
	{
		count = MAX_PARTICLES - ps->active;
	}

	// find the first inactive particle and activate it. then move on count times and do the same
	while (ps->particles[index]->active)
		index++;

	// loop broken, destination reached
	for (int x = index; x < index + count; x++)
	{
		if (x > MAX_PARTICLES) continue;
		ps->particles[x]->active = 1;
		ps->active++;
	}

}

void set_density(ParticleSys* ps, int target)
{
	ps->target = target;
	if (target > MAX_PARTICLES) target = MAX_PARTICLES;
}

void recycle_particle(Particle* p)
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
	p->mass = rand() % 11; // between 0 and 11

	// based on mass, calculate a size and dY
	p->size = (float)p->mass * 0.5f;
	p->dy = p->mass * 0.1f;
}