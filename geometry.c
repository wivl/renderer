#include "geometry.h"
#include <math.h>
#include <assert.h>

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


void vec2f_set(Vec2f *v, int index, float value) {
    assert(index < 2);
    switch (index) {
        case 0:
            v->x = value;
            break;
        case 1:
            v->y = value;
            break;
        default:
            break;
    }
}

float vec2f_get(Vec2f *v, int index) {
    assert(index < 2);
    float ans;
    switch (index) {
        case 0:
            ans = v->x;
            break;
        case 1:
            ans = v->y;
            break;
    }
    return ans;
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

void vec2i_set(Vec2i *v, int index, int value) {
    assert(index < 2);
    switch (index) {
        case 0:
            v->x = value;
            break;
        case 1:
            v->y = value;
            break;
        default:
            break;
    }
}

int vec2i_get(Vec2i *v, int index) {
    assert(index < 2);
    int ans;
    switch (index) {
        case 0:
            ans = v->x;
            break;
        case 1:
            ans = v->y;
            break;
    }
    return ans;
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

float vec3f_norm(Vec3f v) {
    return sqrtf(v.x*v.x+v.y*v.y+v.z*v.z);

}

void vec3f_normalize(Vec3f *a, float l) {
	float num = l/sqrtf(a->x*a->x+a->y*a->y+a->z*a->z);
	*a = vec3f_multiply_f(*a, num);
}

void vec3f_set(Vec3f *v, int index, float value) {
    assert(index < 3);
    switch (index) {
        case 0:
            v->x = value;
            break;
        case 1:
            v->y = value;
            break;
        case 2:
            v->z = value;
            break;
        default:
            break;
    }
}

float vec3f_get(Vec3f *v, int index) {
    assert(index < 3);
    float ans;
    switch (index) {
        case 0:
            ans = v->x;
            break;
        case 1:
            ans = v->y;
            break;
        case 2:
            ans = v->z;
            break;
    }
    return ans;
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

void vec3i_set(Vec3i *v, int index, int value) {
    assert(index < 3);
    switch (index) {
        case 0:
            v->x = value;
            break;
        case 1:
            v->y = value;
            break;
        case 2:
            v->z = value;
            break;
        default:
            break;
    }
}

int vec3i_get(Vec3i *v, int index) {
    assert(index < 3);
    int ans;
    switch (index) {
        case 0:
            ans = v->x;
            break;
        case 1:
            ans = v->y;
            break;
        case 2:
            ans = v->z;
            break;
    }
    return ans;
}
Vec3i vec3f_to_i(Vec3f v) {
    return vec3i_make((int)(v.x+0.5), (int)(v.y+0.5), (int)(v.z+0.5));
}

Vec3f vec3i_to_f(Vec3i v) {
    return vec3f_make((float)v.x, (float)v.y, (float)v.z);
}

// FIX: not sure if it has round issue
Vec2i vec2f_to_i(Vec2f v) {
    return vec2i_make((int)v.x, (int)v.y);
}

Vec2f vec2i_to_f(Vec2i v) {
    return vec2f_make((float)v.x, (float)v.y);
}


Vec4f vec4f_make(float x, float y, float z, float w) {
    Vec4f v = {
        .x = x,
        .y = y,
        .z = z,
        .w = w,
    };
    return v;
}

Vec4f vec4f_add(Vec4f v1, Vec4f v2) {
    return vec4f_make(
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z,
            v1.w + v2.w
            );
}

Vec4f vec4f_minus(Vec4f v1, Vec4f v2) {
    return vec4f_make(
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z,
            v1.w - v2.w
            );
}

Vec4f vec4f_multiply_f(Vec4f v1, float f) {
    return vec4f_make(
            v1.x * f,
            v1.y * f,
            v1.z * f,
            v1.w * f
            );
}

float vec4f_multiply_v(Vec4f v1, Vec4f v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

Vec4f vec4f_cross(Vec3f a, Vec4f b);

void vec4f_set(Vec4f *v, int index, float value) {
    assert(index < 4);
    switch (index) {
        case 0:
            v->x = value;
            break;
        case 1:
            v->y = value;
            break;
        case 2:
            v->z = value;
            break;
        case 3:
            v->w = value;
            break;
        default:
            break;
    }
}

float vec4f_get(Vec4f *v, int index) {
    assert(index < 4);
    float ans;
    switch (index) {
        case 0:
            ans = v->x;
            break;
        case 1:
            ans = v->y;
            break;
        case 2:
            ans = v->z;
            break;
        case 3:
            ans = v->w;
            break;
    }
    return ans;
}

Vec4i vec4i_make(float x, float y, float z, float w) {
    Vec4i v = {
        .x = x,
        .y = y,
        .z = z,
        .w = w,
    };
    return v;

}

Vec4i vec4i_add(Vec4i v1, Vec4i v2) {
    return vec4i_make(
            v1.x + v2.x,
            v1.y + v2.y,
            v1.z + v2.z,
            v1.w + v2.w
            );

}

Vec4i vec4i_minus(Vec4i v1, Vec4i v2) {
    return vec4i_make(
            v1.x - v2.x,
            v1.y - v2.y,
            v1.z - v2.z,
            v1.w - v2.w
            );

}

Vec4i vec4i_multiply_f(Vec4i v1, float f) {
    return vec4i_make(
            v1.x * f,
            v1.y * f,
            v1.z * f,
            v1.w * f
            );

}

float vec4i_multiply_v(Vec4i v1, Vec4i v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

Vec4i vec4i_cross(Vec4i a, Vec4i b);

void vec4i_set(Vec4i *v, int index, int value) {
    assert(index < 4);
    switch (index) {
        case 0:
            v->x = value;
            break;
        case 1:
            v->y = value;
            break;
        case 2:
            v->z = value;
            break;
        case 3:
            v->w = value;
            break;
        default:
            break;
    }
}

int vec4i_get(Vec4i *v, int index) {
    assert(index < 4);
    int ans;
    switch (index) {
        case 0:
            ans = v->x;
            break;
        case 1:
            ans = v->y;
            break;
        case 2:
            ans = v->z;
            break;
        case 3:
            ans = v->w;
            break;
    }
    return ans;
}

Vec4i vec4f_to_i(Vec4f v) {
    return vec4i_make((int)(v.x+0.5), (int)(v.y+0.5), (int)(v.z+0.5), (int)(v.w+0.5));
}

Vec4f vec4i_to_f(Vec4i v) {
    return vec4f_make((float)v.x, (float)v.y, (float)v.z, (float)v.w);
}


Vec4f vec3f_to_4f(Vec3f v, const float fill) {
    Vec4f result;
    for (int i = 4; i--; vec4f_set(&result, i, i < 3 ? vec3f_get(&v, i) : fill));
    return result;
}


Vec3f vec4f_to_3f(Vec4f v) {
    Vec3f result;
    for (int i = 3; i--; vec3f_set(&result, i, vec4f_get(&v, i)));
    return result;
}

