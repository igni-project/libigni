#ifndef _LIBIGNI_TYPES_H
#define _LIBIGNI_TYPES_H 1

typedef struct {
	union {
		float x;
		float r;
	};

	union {
		float y;
		float g;
	};

	union {
		float z;
		float b;
	};
}__attribute__((packed)) IgniVec3;

typedef struct {
	IgniVec3 location;
	IgniVec3 rotation;
	IgniVec3 scale;
}__attribute__((packed)) IgniTransform;

typedef struct {
	IgniVec3 location;
	IgniVec3 lookAt;
}__attribute__((packed)) IgniViewTransform;

#endif

