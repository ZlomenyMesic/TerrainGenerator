#pragma once

#include "constants.h"

typedef enum {
	UNKNOWN = 0,
	GRASS = 1,
	WATER = 2,
	STONE = 3
} VoxelType;

typedef struct {
	float x, y;

	VoxelType*** voxels;
} Chunk;

Chunk* active_chunks[ACTIVE_ARR_SIZE][ACTIVE_ARR_SIZE];
Vector2 last_chunk_pos;

Chunk* new_chunk(int ax, int ay);

void initialize_chunks();

void fill_chunk(Chunk* chunk);

void update_active_chunks(void);

void render_active_chunks(void);

void render_chunk(Chunk* chunk);

void render_voxel(int x, int y, int z, int sx, int sy, int sz, VoxelType type);