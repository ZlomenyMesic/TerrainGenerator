#include "raylib.h"
#include "vectors.h"

Vector2 vec2_new(float x, float y) {
	return (Vector2) { x, y };
}

Vector3 vec3_new(float x, float y, float z) {
	return (Vector3) {x, y, z};
}