#pragma once
#define MAX_PARTICLES 1000

typedef struct _particle
{
	float pos[2];
	float dy;
	float dx;
	float size;

	int mass;
	float colour[4];

	unsigned int lifetime;
	int active;

} Particle;

typedef struct _particlesys
{
	Particle* particles[MAX_PARTICLES];

	unsigned int active;
} ParticleSys;

ParticleSys* new_particle_system(void);
void update_particle_system(ParticleSys*);
void render_particle_system(ParticleSys*);