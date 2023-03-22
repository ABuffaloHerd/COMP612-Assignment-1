#pragma once
#define MAX_PARTICLES 1000

typedef enum _ptype
{
	SNOW,
	DUST,
	SPARK
} ParticleType;

typedef enum _systype
{
	T_EXPLODE,
	T_SNOW
} SystemType;

typedef struct _particle
{
	float pos[2];
	float dy;
	float dx;
	float ddx;
	float ddy;
	float size;

	int mass;
	float colour[4];

	unsigned int lifetime;
	int active;

	ParticleType type;
} Particle;

typedef struct _particlesys
{
	Particle* particles[MAX_PARTICLES];
	SystemType systype;

	unsigned int active;
	unsigned int target;

	float center[2];
} ParticleSys;

ParticleSys* new_particle_system(SystemType);
void render_particle_system(ParticleSys*);
void set_density(ParticleSys* ps, int target);
void update_particle_system(ParticleSys* ps);
void trigger(ParticleSys* ps);