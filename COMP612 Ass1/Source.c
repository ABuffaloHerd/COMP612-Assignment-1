/******************************************************************************
 *
 * Animation v1.0 (23/02/2021)
 *
 * This template provides a basic FPS-limited render loop for an animated scene.
 *
 ******************************************************************************/

#include <Windows.h>
#include <freeglut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "LinkedList.h"
#include "Shape.h"
#include "RenderFunctions.h"
#include "Particle.h"
#include "UpdateFunctions.h"

 /******************************************************************************
  * Animation & Timing Setup
  ******************************************************************************/

  // Target frame rate (number of Frames Per Second).
#define TARGET_FPS 60				

// Ideal time each frame should be displayed for (in milliseconds).
const unsigned int FRAME_TIME = 1000 / TARGET_FPS;

// Frame time in fractional seconds.
// Note: This is calculated to accurately reflect the truncated integer value of
// FRAME_TIME, which is used for timing, rather than the more accurate fractional
// value we'd get if we simply calculated "FRAME_TIME_SEC = 1.0f / TARGET_FPS".
const float FRAME_TIME_SEC = (1000 / TARGET_FPS) / 1000.0f;

// Time we started preparing the current frame (in milliseconds since GLUT was initialized).
unsigned int frameStartTime = 0;

/******************************************************************************
 * Keyboard Input Handling Setup
 ******************************************************************************/

 // Define all character keys used for input (add any new key definitions here).
 // Note: USE ONLY LOWERCASE CHARACTERS HERE. The keyboard handler provided converts all
 // characters typed by the user to lowercase, so the SHIFT key is ignored.

#define KEY_EXIT			27 // Escape key.
#define KEY_R				114 
#define KEY_W				119
#define KEY_X				120
#define KEY_Y				121
#define KEY_Z				122

/******************************************************************************
 * GLUT Callback Prototypes
 ******************************************************************************/

void display(void);
void reshape(int width, int h);
void keyPressed(unsigned char key, int x, int y);
void idle(void);

/******************************************************************************
 * Animation-Specific Function Prototypes (add your own here)
 ******************************************************************************/

void main(int argc, char** argv);
void init(void);
void think(void);
inline float rtoi(int rgb);

/******************************************************************************
 * Animation-Specific Setup (Add your own definitions, constants, and globals here)
 ******************************************************************************/
#define GROUND_ARRAY_SIZE 10
// LINKED RENDERLIST
LinkedList* rlistbg; // backgrounds
LinkedList* rlistfg; // foregrounds

ParticleSys* ps;
ParticleSys* ps_explode;

int scene;

// GROUND ARRAY
float groundfarray[GROUND_ARRAY_SIZE];

void generate_ground()
{
	// generate GROUND_ARRAY_SIZE values between -0.5 and -0.3
	for (int x = 0; x < GROUND_ARRAY_SIZE; x++)
	{
		float f = (float)rand() / (float)(RAND_MAX / 0.2f);
		f -= 0.3f; // shift the range down
		groundfarray[x] = f;
	}

	printf("Regenerated ground values\n");
}

void detonate()
{
	Shape* missile = find(rlistbg, "missile");
	Shape* balloon = find(rlistbg, "spyballoon");
	if (balloon->enabled)
	{
		missile->enabled = 1;
		missile->pos[0] = -1.0f;
		missile->pos[1] = balloon->pos[1];
		printf("This is AWACS SkyEye Fox 2!\n");
	}
	else
	{
		balloon->pos[0] = 1.2f;
		balloon->enabled = 1;
		printf("Balloon enabled!\n");
	}
}

 /******************************************************************************
  * Entry Point (don't put anything except the main function here)
  ******************************************************************************/

void main(int argc, char** argv)
{
	// Initialize the OpenGL window.
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Seducing the Unbelievably Adulterous Capture Target! I'll Defy the Female Lead, Even if I Have to Surrender to the Master?!");

	// Set up the scene.
	init();

	// Disable key repeat (keyPressed or specialKeyPressed will only be called once when a key is first pressed).
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

	// Register GLUT callbacks.
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyPressed);
	glutIdleFunc(idle);

	// Record when we started rendering the very first frame (which should happen after we call glutMainLoop).
	frameStartTime = (unsigned int)glutGet(GLUT_ELAPSED_TIME);

	// Enter the main drawing loop (this will never return).
	glutMainLoop();
}

/******************************************************************************
 * GLUT Callbacks (don't add any other functions here)
 ******************************************************************************/

 /*
	 Called when GLUT wants us to (re)draw the current animation frame.

	 Note: This function must not do anything to update the state of our simulated
	 world. Animation (moving or rotating things, responding to keyboard input,
	 etc.) should only be performed within the think() function provided below.
 */

void armageddon(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glutSwapBuffers();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// backgroundloop first
	for (Node* current = rlistbg->head; current != NULL; current = current->next)
	{
		// render or na
		if (!current->shape_ptr->enabled)
		{
			continue;
		}
		// figure out what shape we're rendering
		switch (current->shape_ptr->type)
		{
		case SHAPE_SQUARE:
			render_square(current->shape_ptr);
			break;
		case SHAPE_TRIANGLE:
			render_triangle(current->shape_ptr);
			break;
		case SHAPE_CIRCLE:
			render_circle(current->shape_ptr);
			break;
		case SHAPE_POLY:
			render_poly(current->shape_ptr);
			break;
		case SHAPE_CUSTOM:
			current->shape_ptr->custom(current->shape_ptr);
			break;
		}
	}
	// put explode behind ground
	render_particle_system(ps_explode);

	render_ground(&groundfarray, GROUND_ARRAY_SIZE);
	// hyucking do it again for the foreground list
	for (Node* current = rlistfg->head; current != NULL; current = current->next)
	{
		// figure out what shape we're rendering
		switch (current->shape_ptr->type)
		{
		case SHAPE_SQUARE:
			render_square(current->shape_ptr);
			break;
		case SHAPE_TRIANGLE:
			render_triangle(current->shape_ptr);
			break;
		case SHAPE_CIRCLE:
			render_circle(current->shape_ptr);
			break;
		case SHAPE_POLY:
			render_poly(current->shape_ptr);
			break;
		case SHAPE_CUSTOM:
			current->shape_ptr->custom(current->shape_ptr);
			break;
		}
	}

	// Particle system madness
	render_particle_system(ps);

	// labels
	char particle[255];
	sprintf_s(particle, 255, "Particles: %d", ps->active);
	label(-1.0f, 0.9f, particle);

	char info[255];
	sprintf_s(info, 255, "Y: Destroy '''weather''' balloon / send a new one\nX: Disable snow\nZ: Enable snow");
	label(-1.0f, 0.8f, info);

	glutSwapBuffers();
}

/*
	Called when the OpenGL window has been resized.
*/
void reshape(int width, int h)
{
}

/*
	Called each time a character key (e.g. a letter, number, or symbol) is pressed.
*/
void keyPressed(unsigned char key, int x, int y)
{
	switch (tolower(key)) 
	{
	case KEY_X:
		set_density(ps, 0);
		break;
	case KEY_Y:
		detonate();
		break;
	case KEY_Z:
		set_density(ps, 1000);
		break;
	case KEY_R:
		generate_ground();
		break;
	case KEY_W:
		if (scene)
		{
			scene = 0;
			glutDisplayFunc(display);
		}
		else
		{
			scene = 1;
			glutDisplayFunc(armageddon);
		}
		break;
	case KEY_EXIT:
		exit(0);
		break;
	}

	glutPostRedisplay();
}

/*
	Called by GLUT when it's not rendering a frame.

	Note: We use this to handle animation and timing. You shouldn't need to modify
	this callback at all. Instead, place your animation logic (e.g. moving or rotating
	things) within the think() method provided with this template.
*/
void idle(void)
{
	// Wait until it's time to render the next frame.

	unsigned int frameTimeElapsed = (unsigned int)glutGet(GLUT_ELAPSED_TIME) - frameStartTime;
	if (frameTimeElapsed < FRAME_TIME)
	{
		// This frame took less time to render than the ideal FRAME_TIME: we'll suspend this thread for the remaining time,
		// so we're not taking up the CPU until we need to render another frame.
		unsigned int timeLeft = FRAME_TIME - frameTimeElapsed;
		Sleep(timeLeft);
	}

	// Begin processing the next frame.

	frameStartTime = glutGet(GLUT_ELAPSED_TIME); // Record when we started work on the new frame.

	think(); // Update our simulated world before the next call to display().

	glutPostRedisplay(); // Tell OpenGL there's a new frame ready to be drawn.
}

/******************************************************************************
 * Animation-Specific Functions (Add your own functions at the end of this section)
 ******************************************************************************/

 /*
	 Initialise OpenGL and set up our scene before we begin the render loop.
 */
void init(void)
{
	// bl e n d i n g
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// initialize the renderlists
	rlistbg = new_ll();
	rlistfg = new_ll();

	// scene test
	int scene = 0;

	// TODO: split into helper functions

	// setup particle system
	ps = new_particle_system(T_SNOW);
	set_density(ps, MAX_PARTICLES);

	ps_explode = new_particle_system(T_EXPLODE);

	// initialize a single square the size of the screen with a custom function
	Shape* sky = new_custom_shape("sky", render_sky);
	insert_back(rlistbg, sky);

	// render some mountains for cool stone effect.
	// Transparent background mountains first
	Shape* mountain1 = new_shape("bgmountain1", 3, -0.3f, -0.3f, 0.9f, 0,
		rtoi(145), rtoi(223), rtoi(255), 1.0f,
		rtoi(112), rtoi(112), rtoi(112), 1.0f,
		SHAPE_TRIANGLE);

	Shape* mountain2 = new_shape("bgmountain2", 3, 0.6f, -0.3f, 0.7f, 0,
		rtoi(145), rtoi(223), rtoi(255), 1.0f,
		rtoi(180), rtoi(180), rtoi(180), 1.0f,
		SHAPE_TRIANGLE);
	insert_back(rlistbg, mountain2);
	insert_back(rlistbg, mountain1);

	// Chinese spy balloon
	Shape* spyballoon = new_custom_shape("spyballoon", render_spy_balloon);
	spyballoon->scale = 0.085f;
	spyballoon->pos[1] = 0.8f;
	spyballoon->update = update_spyballoon;
	insert_back(rlistbg, spyballoon);

	// Missile
	Shape* missile = new_custom_shape("missile", render_missile);
	missile->enabled = 0;
	missile->pos[0] = 10.0f;
	missile->pos[1] = 10.0f;
	missile->update = update_missile;
	insert_back(rlistbg, missile);

	// mr snowman
	Shape* snowman_arse = new_shape("snowman_ass", 0, -0.4, -0.4, 0.3f, 0,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.7f, 0.7f, 0.7f, 1.0f,
		SHAPE_CIRCLE);

	insert_back(rlistfg, snowman_arse);

	Shape* snowman_torso = new_shape("snowman_torso", 0, -0.4, -0.1, 0.25f, 0,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.7f, 0.7f, 0.7f, 1.0f,
		SHAPE_CIRCLE);

	insert_back(rlistfg, snowman_torso);

	Shape* snowman_head = new_shape("snowman_head", 0, -0.4, 0.2f, 0.22f, 0,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.7f, 0.7f, 0.7f, 1.0f,
		SHAPE_CIRCLE);

	insert_back(rlistfg, snowman_head);

	// face
	Shape* face = new_custom_shape("face", render_snowman_face);
	face->pos[0] = -0.4f;
	face->pos[1] = 0.2f;
	insert_back(rlistfg, face);

	// Do the random ground.
	// Seed random
	srand(time(0));
	generate_ground();
}

/*
	Advance our animation by FRAME_TIME milliseconds.

	Note: Our template's GLUT idle() callback calls this once before each new
	frame is drawn, EXCEPT the very first frame drawn after our application
	starts. Any setup required before the first frame is drawn should be placed
	in init().
*/
void think(void)
{
	// update all
	for (Node* current = rlistbg->head; current != NULL; current = current->next)
	{
		Shape* shape = current->shape_ptr;
		if (shape->update)
			shape->update(shape);
	}

	for (Node* current = rlistfg->head; current != NULL; current = current->next)
	{
		Shape* shape = current->shape_ptr;
		if (shape->update)
			shape->update(shape);
	}

	// manually set explosion coords
	Shape* sballoon = find(rlistbg, "spyballoon");
	ps_explode->center[0] = sballoon->pos[0];
	ps_explode->center[1] = sballoon->pos[1];

	update_particle_system(ps);
	update_particle_system(ps_explode);

	glutPostRedisplay();
}

/******************************************************************************/