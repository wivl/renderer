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


typedef struct {
	int x;
	int y;
} Vec2i;

Vec2i vec2i_make(int x, int y);

Vec2i vec2i_add(Vec2i v1, Vec2i v2);

Vec2i vec2i_minus(Vec2i v1, Vec2i v2);

Vec2i vec2i_multiply(Vec2i v1, float f);

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

/* trans */

Vec3i vec3f_to_i(Vec3f v);

Vec3f vec3i_to_f(Vec3i v);

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


Vec4i vec4f_to_i(Vec4f v);

Vec4f vec4i_to_f(Vec4i v);



#endif
