#include "geometry.h"
#include <math.h>

Vec2f vec2f_make(float x, float y) {
	Vec2f result = {
		.x = x,
		.y = y
	};
	return result;
}

Vec2f vec2f_add(Vec2f v1, Vec2f v2) {
	Vec2f result = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y
	};
	return result;
}

Vec2f vec2f_minus(Vec2f v1, Vec2f v2) {
	Vec2f result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y
	};
	return result;
}

Vec2f vec2f_multiply(Vec2f v1, float f) {
	Vec2f result = {
		.x = v1.x * f,
		.y = v1.y * f
	};
	return result;
}



Vec2i vec2i_make(int x, int y) {
	Vec2i result = {
		.x = x,
		.y = y
	};
	return result;
}
Vec2i vec2i_add(Vec2i v1, Vec2i v2) {
	Vec2i result = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y
	};
	return result;
}

Vec2i vec2i_minus(Vec2i v1, Vec2i v2) {
	Vec2i result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y
	};
	return result;
}

Vec2i vec2i_multiply(Vec2i v1, float f) {
	Vec2i result = {
		.x = v1.x * f,
		.y = v1.y * f
	};
	return result;
}

Vec3f vec3f_make(float x, float y, float z) {
	Vec3f result = {
		.x = x,
		.y = y,
		.z = z
	};
	return result;
}

Vec3f vec3f_add(Vec3f v1, Vec3f v2) {
	Vec3f result = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z
	};
	return result;
}

Vec3f vec3f_minus(Vec3f v1, Vec3f v2) {
	Vec3f result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z
	};
	return result;
}

Vec3f vec3f_multiply_f(Vec3f v1, float f) {
	Vec3f result = {
		.x = v1.x * f,
		.y = v1.y * f,
		.z = v1.z * f
	};
	return result;
}

float vec3f_multiply_v(Vec3f v1, Vec3f v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

Vec3f vec3f_cross(Vec3f a, Vec3f b) {
	Vec3f result = {
		.x = a.y*b.z-b.y*a.z,
		.y = a.z*b.x-b.z*a.x,
		.z = a.x*b.y-b.x*a.y
	};
	return result;
}

void vec3f_normalize(Vec3f *a, float l) {
	float num = l/sqrtf(a->x*a->x+a->y*a->y+a->z*a->z);
	*a = vec3f_multiply_f(*a, num);
}


Vec3i vec3i_make(int x, int y, int z) {
	Vec3i result = {
		.x = x,
		.y = y,
		.z = z
	};
	return result;
}

Vec3i vec3i_add(Vec3i v1, Vec3i v2) {
	Vec3i result = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z
	};
	return result;
}

Vec3i vec3i_minus(Vec3i v1, Vec3i v2) {
	Vec3i result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z
	};
	return result;
}

Vec3i vec3i_multiply_f(Vec3i v1, float f) {
	Vec3i result = {
		.x = v1.x * f,
		.y = v1.y * f,
		.z = v1.z * f
	};
	return result;
}

int vec3i_multiply_v(Vec3i v1, Vec3i v2) {
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

Vec3i vec3i_cross(Vec3i a, Vec3i b) {
	Vec3i result = {
		.x = a.y*b.z-b.y*a.z,
		.y = a.z*b.x-b.z*a.x,
		.z = a.x*b.y-b.x*a.y
	};
	return result;
}

void vec3i_normalize(Vec3i *a, int l) {
	float num = l/sqrt(a->x*a->x+a->y*a->y+a->z*a->z);
	*a = vec3i_multiply_f(*a, num);
}

Vec3i vec3f_to_i(Vec3f v) {
    return vec3i_make(v.x, v.y, v.z);
}

Vec3f vec3i_to_f(Vec3i v) {
    return vec3f_make(v.x, v.y, v.z);
}



