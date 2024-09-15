#ifndef _LIBIGNI_HIT_H
#define _LIBIGNI_HIT_H 1

/// 
/// @brief Major version of Igni Hit protocol
///
#define IGNI_HIT_VERSION 0

#include "types.h"
#include <stdint.h>

/// 
/// @brief Command number
///
typedef uint8_t IgniHitOpcode;
enum {
	IGNI_HIT_OP_NUL = 0,
	IGNI_HIT_OP_CONFIGURE,

	IGNI_HIT_OP_HITBOX_CREATE,
	IGNI_HIT_OP_HITBOX_TRANSFORM,
	IGNI_HIT_OP_HITBOX_DELETE
};

/// 
/// @brief Event number
///
typedef uint8_t IgniHitEvent;
enum {
	IGNI_HIT_EVENT_NUL = 0,

	IGNI_HIT_EVENT_HITBOX_TRIGGER,
	IGNI_HIT_EVENT_HITBOX_RELEASE
};

/// 
/// @brief Scene element identifier
///
/// \note
/// - No two elements of matching type within a scene can share the same ID. 
///
typedef uint32_t IgniHitElementId;
#define IGNI_HIT_NULL_ELEMENT -1

/// 
/// @brief Adjust connection properties
///
typedef struct {
	uint8_t majVersion;
}__attribute__((packed)) IgniHitCmdConfigure;

/// 
/// @brief Add hitbox to scene
///
typedef struct {
	IgniHitElementId hitboxId;
}__attribute__((packed)) IgniHitCmdHitboxCreate;

///
/// @brief Set hitbox location, rotation and dimensions
///
typedef struct {
	IgniHitElementId hitboxId;
	float xLoc, yLoc, zLoc;
	float xRot, yRot, zRot;
	float width, height, depth;
}__attribute__((packed)) IgniHitCmdHitboxTransform;

///
/// @brief Remove hitbox from scene
///
typedef struct {
	IgniHitElementId hitboxId;
}__attribute__((packed)) IgniHitCmdHitboxDelete;

/// 
/// @brief Hitbox intersects another hitbox after transformation
///
typedef struct {
	IgniHitElementId hitboxId;
}__attribute__((packed)) IgniHitEventHitboxTrigger;

/// 
/// @brief Hitbox no longer intersects a hitbox that previously did intersect
///
typedef struct {
	IgniHitElementId hitboxId;
}__attribute__((packed)) IgniHitEventHitboxRelease;

/// 
/// @brief Add hitbox to scene
///
/// @param fd 		File descriptor of server socket
/// @param id 		Hitbox identification number
/// @return 0 upon success. -1 to indicate an error.
///
int igniHitHitboxCreate(
	int fd,
	IgniHitElementId id
);

/// 
/// @brief Set hitbox location, rotation and dimensions
///
/// @param fd 		File descriptor of server socket
/// @param id 		Hitbox identification number
/// @param tf 		Hitbox transformation
/// @return 0 upon success. -1 to indicate an error.
///
int igniHitHitboxTransform(
	int fd,
	IgniHitElementId id,
	IgniTransform tf
);

/// 
/// @brief Remove hitbox from scene
/// 
/// @param fd 		File descriptor of server socket
/// @param id 		Hitbox identification number
/// @return 0 upon success. -1 to indicate an error.
///
int igniHitHitboxDelete(
	int fd,
	IgniHitElementId id
);


#endif

