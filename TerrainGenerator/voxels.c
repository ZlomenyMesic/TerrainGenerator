#include <stdlib.h>
#include <stdio.h>
#include "voxels.h"
#include "constants.h"
#include "vectors.h"
#include "main.h"

Chunk* new_chunk(int ax, int ay) {
	Chunk* c = malloc(sizeof(Chunk));
	if (c != NULL) {
		c->x = ax;
		c->y = ay;

		c->voxels = malloc(CHUNK_SIZE * sizeof(VoxelType**));
		if (c->voxels == NULL) return NULL;
		for (int x = 0; x < CHUNK_SIZE; x++) {

			c->voxels[x] = malloc(CHUNK_SIZE * sizeof(VoxelType*));
			if (c->voxels[x] == NULL) return NULL;
			for (int y = 0; y < CHUNK_SIZE; y++) {

				c->voxels[x][y] = malloc(HEIGHT_LIMIT * sizeof(VoxelType));
				if (c->voxels[x][y] == NULL) return NULL;
				for (int z = 0; z < HEIGHT_LIMIT; z++) {
					c->voxels[x][y][z] = UNKNOWN;
				}
			}
		}
	}

	return c;
}

void initialize_chunks() {
	last_chunk_pos = vec2_new(0, 0);
	int px = camera.position.x / CHUNK_SIZE;
	int py = camera.position.z / CHUNK_SIZE;
	for (int x = 0; x < ACTIVE_ARR_SIZE; x++) {
		for (int y = 0; y < ACTIVE_ARR_SIZE; y++) {
			Chunk* c = new_chunk(x + px, y + py);
			fill_chunk(c);
			active_chunks[x][y] = c;
		}
	}
}

void fill_chunk(Chunk* chunk) {
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			chunk->voxels[x][y][0] = GRASS;
		}
	}
}

static void shift_chunks(Vector2 shift) {
	Chunk* new_active[ACTIVE_ARR_SIZE][ACTIVE_ARR_SIZE] = {0};

	for (int x = 0; x < ACTIVE_ARR_SIZE; x++) {
		for (int y = 0; y < ACTIVE_ARR_SIZE; y++) {
			Vector2 npos = vec2_new(x + shift.x, y + shift.y);
			if (npos.x >= 0 && npos.y >= 0 && npos.x < ACTIVE_ARR_SIZE && npos.y < ACTIVE_ARR_SIZE) {
				new_active[(int)npos.x][(int)npos.y] = active_chunks[x][y];
			}
		}
	}

	for (int x = 0; x < ACTIVE_ARR_SIZE; x++) {
		for (int y = 0; y < ACTIVE_ARR_SIZE; y++) {
			active_chunks[x][y] = new_active[x][y];
		}
	}
}

void update_active_chunks(void) {
	Vector2 chunk_pos = vec2_new((int)(camera.position.x / CHUNK_SIZE), (int)(camera.position.z / CHUNK_SIZE));
	Vector2 shift = vec2_new(last_chunk_pos.x - chunk_pos.x, last_chunk_pos.y - chunk_pos.y);

	shift_chunks(shift);

	int px = camera.position.x / CHUNK_SIZE;
	int py = camera.position.z / CHUNK_SIZE;

	for (int x = 0; x < ACTIVE_ARR_SIZE; x++) {
		for (int y = 0; y < ACTIVE_ARR_SIZE; y++) {
			if (active_chunks[x][y] == NULL || active_chunks[x][y] == 0) {
				Chunk* c = new_chunk(x + px, y + py);
				fill_chunk(c);
				active_chunks[x][y] = c;
			}
		}
	}

	last_chunk_pos = chunk_pos;
}

void render_active_chunks(void) {
	for (int ax = 0; ax < ACTIVE_ARR_SIZE; ax++) {
		for (int ay = 0; ay < ACTIVE_ARR_SIZE; ay++) {
			Chunk* current = active_chunks[ax][ay];
			if (current != NULL) {
				render_chunk(current);
			}
		}
	}
}

void render_chunk(Chunk* chunk) {
	DrawCubeWires(vec3_new((chunk->x * CHUNK_SIZE) + (CHUNK_SIZE / 2) + C_SHIFT, HEIGHT_LIMIT / 2, (chunk->y * CHUNK_SIZE) + (CHUNK_SIZE / 2) + C_SHIFT), CHUNK_SIZE, HEIGHT_LIMIT, CHUNK_SIZE, RED);

	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int y = 0; y < CHUNK_SIZE; y++) {
			for (int z = 0; z < HEIGHT_LIMIT; z++) {
				VoxelType type = chunk->voxels[x][y][z];
				if (type != NULL && type != UNKNOWN) {
					render_voxel(x + (chunk->x * CHUNK_SIZE), z, y + (chunk->y * CHUNK_SIZE), 1, 1, 1, type);
				}
			}
		}
	}
}

void render_voxel(int x, int y, int z, int sx, int sy, int sz, VoxelType type) {
	DrawCube(vec3_new(x + C_SHIFT, y, z + C_SHIFT), sx, sy, sz, GREEN);
}