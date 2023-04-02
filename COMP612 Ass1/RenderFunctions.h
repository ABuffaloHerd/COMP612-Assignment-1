#pragma once

void render_sky(struct _shape* self);
void render_ground(float* f, int size);
void render_spy_balloon(struct _shape* self);
void render_snowman_face(struct _shape* s);
void label(float px, float py, char* text);
void render_missile(struct _shape* s);
void render_overlay(struct _shape* overlay);