#pragma once
// the funny rating of the earthquake depends on the camera boundaries and the magnitude.
typedef struct _eq
{
	int shaking; // yes or no
	float mag; // funny level
	unsigned int duration; // in frames not seconds
	unsigned int timer; // also in frames not seconds
} EARTHQUAKE;