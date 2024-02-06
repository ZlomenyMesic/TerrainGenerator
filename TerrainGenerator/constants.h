#pragma once

#include "raylib.h"

#define WIN_WIDTH 1920
#define WIN_HEIGHT 1080

#define FIELD_OF_VIEW 60

#define TARGET_FRAMES_PER_SECOND 60

#define MOVE_SPEED 1.3
#define SENSITIVITY 0.2

#define FORWARD KEY_W
#define BACKWARD KEY_S
#define LEFT KEY_A
#define RIGHT KEY_D
#define UP KEY_SPACE
#define DOWN KEY_LEFT_SHIFT

#define CHUNK_SIZE 32
#define HEIGHT_LIMIT 128
#define RENDER_DIST 6
#define ACTIVE_ARR_SIZE ((RENDER_DIST * 2) + 1)
#define C_SHIFT (ACTIVE_ARR_SIZE * CHUNK_SIZE / -2)