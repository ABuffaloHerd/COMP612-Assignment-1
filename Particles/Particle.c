#include "Particle.h"
#include "freeglut.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

extern const float FRAME_TIME_SEC;

Particle* new_particle(void);

ParticleSys* new_particle_system(void)
{
	ParticleSys* ps = (ParticleSys*)malloc(sizeof(ParticleSys));

	ps->active = 0;
	
	// insert particles
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		ps->particles[x] = new_particle();
	}

	return ps;
}

Particle* new_particle(void)
{
	Particle* p = (Particle*)malloc(sizeof(Particle));

	// setup particle
	p->active = 1;
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
	glBegin(GL_POINTS);
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		Particle* current = ps->particles[x];

		// Skip?
		if (current->active == 0)
		{
			printf("Skipped particle\n");
			continue;
		}

		// set stuff
		glColor4fv(current->colour);
		glPointSize(current->size);
		
		// Draw
		glVertex2fv(current->pos);
	}

	glEnd();
}

void update_particle_system(ParticleSys* ps)
{
	for (int x = 0; x < MAX_PARTICLES; x++)
	{
		Particle* current = ps->particles[x];
		
		current->pos[1] -= current->dy * FRAME_TIME_SEC;
		current->lifetime++;

		if (current->pos[x] < -1.2f)
			current->pos[1] = 1.1f;
		//printf("Particle information:\n");
		//printf("pos: %f %f, size: %f\n", current->pos[0], current->pos[1], current->size);
	}

}