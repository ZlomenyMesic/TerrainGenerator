//
// renderer/main.c
// judeslavia
//

#include <stdio.h>
#include "raylib.h"
#include "main.h"
#include "constants.h"
#include "vectors.h"
#include "voxels.h"

static void initialize(void) {
	InitWindow(WIN_WIDTH, WIN_HEIGHT, NULL);

	camera.position = vec3_new(0, 0, 0);
	camera.target = vec3_new(0, 0, 1);
	camera.up = vec3_new(0, 1, 0);

	camera.fovy = FIELD_OF_VIEW;
	camera.projection = CAMERA_PERSPECTIVE;

	DisableCursor();
	SetTargetFPS(TARGET_FRAMES_PER_SECOND);

	update_active_chunks();
	initialize_chunks();
}

static void update(void) {
	Vector3 movement = vec3_new(
		(IsKeyDown(FORWARD) * MOVE_SPEED) - (IsKeyDown(BACKWARD) * MOVE_SPEED),
		(IsKeyDown(RIGHT) * MOVE_SPEED) - (IsKeyDown(LEFT) * MOVE_SPEED),
		(IsKeyDown(UP) * MOVE_SPEED) - (IsKeyDown(DOWN) * MOVE_SPEED));

	Vector2 mouse_delta = GetMouseDelta();
	Vector3 rotation = vec3_new(mouse_delta.x * SENSITIVITY, mouse_delta.y * SENSITIVITY, 0.0f);

	UpdateCameraPro(&camera, movement, rotation, 0.0f);

	update_active_chunks();
}

static void draw(void) {
	BeginDrawing();
	BeginMode3D(camera);

	ClearBackground(BLACK);

	render_active_chunks();

	EndMode3D();

	DrawText(TextFormat("Cam position:\n\nX: %0.2f\n\nY: %0.2f\n\nZ: %0.2f\n\n", camera.position.x, camera.position.y, camera.position.z), 30, 30, 30, WHITE);

	EndDrawing();
}

int main() {

	initialize ();

	while (!WindowShouldClose()) {
		update();
		draw();
	}

	return 0;
}