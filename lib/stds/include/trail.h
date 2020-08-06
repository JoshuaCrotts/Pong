#ifndef TRAIL_H
#define TRAIL_H

#include "draw.h"
#include "stds.h"
#include "vec2.h"

extern struct app_t g_app;

extern void Stds_AddTextureTrail( struct entity_t *parent_entity, int16_t alpha_decay_rate,
                                  SDL_RendererFlip flip, bool is_transparent );

extern void Stds_AddSquareTrail( float x, float y, int32_t w, int32_t h, int16_t alpha_decay,
                                 SDL_Color *c );

extern void Stds_AddCircleTrail( float x, float y, int32_t r, int16_t alpha_decay, SDL_Color *c );

extern void Stds_TrailUpdate( struct trail_t *t );

extern void Stds_TrailDraw( struct trail_t *t );

#endif // TRAIL_H