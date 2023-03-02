#ifndef _GEOMETRY_H
#define _GEOMETRY_H

typedef struct {
	float x;
	float y;
} Vec2f;

Vec2f vec2f_make(float x, float y);

Vec2f vec2f_add(Vec2f v1, Vec2f v2);

Vec2f vec2f_minus(Vec2f v1, Vec2f v2);

Vec2f vec2f_multiply(Vec2f v1, float f);

void vec2f_set(Vec2f *v, int index, float value);

float vec2f_get(Vec2f *v, int index);


typedef struct {
	int x;
	int y;
} Vec2i;

Vec2i vec2i_make(int x, int y);

Vec2i vec2i_add(Vec2i v1, Vec2i v2);

Vec2i vec2i_minus(Vec2i v1, Vec2i v2);

Vec2i vec2i_multiply(Vec2i v1, float f);

void vec2i_set(Vec2i *v, int index, int value);

int vec2i_get(Vec2i *v, int index);

typedef struct {
	float x;
	float y;
	float z;
} Vec3f;

Vec3f vec3f_make(float x, float y, float z);

Vec3f vec3f_add(Vec3f v1, Vec3f v2);

Vec3f vec3f_minus(Vec3f v1, Vec3f v2);

Vec3f vec3f_multiply_f(Vec3f v1, float f);

float vec3f_multiply_v(Vec3f v1, Vec3f v2);

Vec3f vec3f_cross(Vec3f a, Vec3f b);

void vec3f_set(Vec3f *v, int index, float value);

float vec3f_get(Vec3f *v, int index);

float vec3f_norm(Vec3f v);


// http://www.fundza.com/vectors/normalize/
// a.xy / |a|
// a.y / |a|
// a.z / |a|
void vec3f_normalize(Vec3f *a, float l);


typedef struct {
	int x;
	int y;
	int z;
} Vec3i;

Vec3i vec3i_make(int x, int y, int z);

Vec3i vec3i_add(Vec3i v1, Vec3i v2);

Vec3i vec3i_minus(Vec3i v1, Vec3i v2);

Vec3i vec3i_multiply_f(Vec3i v1, float f);

int vec3i_multiply_v(Vec3i v1, Vec3i v2);

Vec3i vec3i_cross(Vec3i a, Vec3i b);

void vec3i_normalize(Vec3i *a, int l);

void vec3i_set(Vec3i *v, int index, int value);

int vec3i_get(Vec3i *v, int index);

/* trans */

Vec3i vec3f_to_i(Vec3f v);

Vec3f vec3i_to_f(Vec3i v);

Vec2i vec2f_to_i(Vec2f v);

Vec2f vec2i_to_f(Vec2i v);

typedef struct {
	float x;
	float y;
	float z;
    float w;
} Vec4f;

Vec4f vec4f_make(float x, float y, float z, float w);

Vec4f vec4f_add(Vec4f v1, Vec4f v2);

Vec4f vec4f_minus(Vec4f v1, Vec4f v2);

Vec4f vec4f_multiply_f(Vec4f v1, float f);

float vec4f_multiply_v(Vec4f v1, Vec4f v2);

Vec4f vec4f_cross(Vec3f a, Vec4f b);

float vec4f_get(Vec4f *v, int index);

void vec4f_set(Vec4f *v, int index, float value);

typedef struct {
	int x;
	int y;
	int z;
    int w;
} Vec4i;


Vec4i vec4i_make(float x, float y, float z, float w);

Vec4i vec4i_add(Vec4i v1, Vec4i v2);

Vec4i vec4i_minus(Vec4i v1, Vec4i v2);

Vec4i vec4i_multiply_f(Vec4i v1, float f);

float vec4i_multiply_v(Vec4i v1, Vec4i v2);

Vec4i vec4i_cross(Vec4i a, Vec4i b);

int vec4i_get(Vec4i *v, int index);

void vec4i_set(Vec4i *v, int index, int value);

Vec4i vec4f_to_i(Vec4f v);

Vec4f vec4i_to_f(Vec4i v);

// trans
// vec3f to vec4f
Vec4f vec3f_to_4f(Vec3f v, const float fill);

Vec3f vec4f_to_3f(Vec4f v);

#endif
