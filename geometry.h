#ifndef _GEOMETRY_H
#define _GEOMETRY_H

typedef struct {
	float x;
	float y;
} Vec2f;

inline Vec2f vec2f_make(float x, float y) {
	Vec2f result = {
		.x = x,
		.y = y
	};
	return result;
}

inline Vec2f vec2f_add(Vec2f v1, Vec2f v2) {
	Vec2f result = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y
	};
	return result;
}

inline Vec2f vec2f_minus(Vec2f v1, Vec2f v2) {
	Vec2f result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y
	};
	return result;
}

inline Vec2f vec2f_mutiply(Vec2f v1, float f) {
	Vec2f result = {
		.x = v1.x * f,
		.y = v1.y * f
	};
	return result;
}


typedef struct {
	int x;
	int y;
} Vec2i;

inline Vec2i vec2i_make(int x, int y) {
	Vec2i result = {
		.x = x,
		.y = y
	};
	return result;
}
inline Vec2i vec2i_add(Vec2i v1, Vec2i v2) {
	Vec2i result = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y
	};
	return result;
}

inline Vec2i vec2i_minus(Vec2i v1, Vec2i v2) {
	Vec2i result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y
	};
	return result;
}

inline Vec2i vec2i_mutiply(Vec2i v1, float f) {
	Vec2i result = {
		.x = v1.x * f,
		.y = v1.y * f
	};
	return result;
}
typedef struct {
	float x;
	float y;
	float z;
} Vec3f;

inline Vec3f vec3f_make(float x, float y, float z) {
	Vec3f result = {
		.x = x,
		.y = y,
		.z = z
	};
	return result;
}

inline Vec3f vec3f_add(Vec3f v1, Vec3f v2) {
	Vec3f result = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z
	};
	return result;
}

inline Vec3f vec3f_minus(Vec3f v1, Vec3f v2) {
	Vec3f result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z
	};
	return result;
}

inline Vec3f vec3f_multiply_f(Vec3f v1, float f) {
	Vec3f result = {
		.x = v1.x * f,
		.y = v1.y * f,
		.z = v1.z * f
	};
	return result;
}

inline Vec3f vec3f_multiply_v(Vec3f v1, Vec3f v2) {
	Vec3f result = {
		.x = v1.x * v2.x,
		.y = v1.y * v2.y,
		.z = v1.z * v2.z
	};
	return result;
}
typedef struct {
	int x;
	int y;
	int z;
} Vec3i;

inline Vec3i vec3i_make(int x, int y, int z) {
	Vec3i result = {
		.x = x,
		.y = y,
		.z = z
	};
	return result;
}

inline Vec3i vec3i_add(Vec3i v1, Vec3i v2) {
	Vec3i result = {
		.x = v1.x + v2.x,
		.y = v1.y + v2.y,
		.z = v1.z + v2.z
	};
	return result;
}

inline Vec3i vec3i_minus(Vec3i v1, Vec3i v2) {
	Vec3i result = {
		.x = v1.x - v2.x,
		.y = v1.y - v2.y,
		.z = v1.z - v2.z
	};
	return result;
}

inline Vec3i vec3i_multiply_f(Vec3i v1, float f) {
	Vec3i result = {
		.x = v1.x * f,
		.y = v1.y * f,
		.z = v1.z * f
	};
	return result;
}

inline Vec3i vec3i_multiply_v(Vec3i v1, Vec3i v2) {
	Vec3i result = {
		.x = v1.x * v2.x,
		.y = v1.y * v2.y,
		.z = v1.z * v2.z
	};
	return result;
}
#endif
