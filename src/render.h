#ifndef _LIBIGNI_RENDER_H
#define _LIBIGNI_RENDER_H 1

/// 
/// @brief Major version of Igni Render protocol
///
#define IGNI_RENDER_VERSION 0

#include "types.h"
#include <stdint.h>

/// 
/// @brief Command number
///
typedef uint8_t IgniRndOpcode;
enum {
	IGNI_RENDER_OP_NUL = 0,
	IGNI_RENDER_OP_CONFIGURE,

	IGNI_RENDER_OP_MESH_CREATE,
	IGNI_RENDER_OP_MESH_SET_SHADER,
	IGNI_RENDER_OP_MESH_BIND_TEXTURE,
	IGNI_RENDER_OP_MESH_TRANSFORM,
	IGNI_RENDER_OP_MESH_DELETE,

	IGNI_RENDER_OP_POINT_LIGHT_CREATE,
	IGNI_RENDER_OP_POINT_LIGHT_TRANSFORM,
	IGNI_RENDER_OP_POINT_LIGHT_SET_COLOUR,
	IGNI_RENDER_OP_POINT_LIGHT_DELETE,

	IGNI_RENDER_OP_TEXTURE_CREATE,
	IGNI_RENDER_OP_TEXTURE_DELETE,

	IGNI_RENDER_OP_VIEWPOINT_TRANSFORM
};

/// 
/// @brief Scene element identifier
///
/// \note
/// - No two elements of matching type within a scene can share the same ID. 
///
typedef uint32_t IgniRndElementId;
#define IGNI_RENDER_NULL_ELEMENT -1

/// 
/// @brief Shading mode
///
typedef uint8_t IgniRndShader;
enum {
	IGNI_RENDER_SHADER_SHADELESS = 0
};

/// 
/// @brief Texture targets
///
typedef uint8_t IgniRndTextureTarget;
enum {
	IGNI_RENDER_TEXTURE_TARGET_DIFFUSE = 0,

	IGNI_RENDER_TEXTURE_TARGET_COUNT
};

/// 
/// @brief Adjust connection properties
///
typedef struct {
	uint8_t majVersion;
}__attribute__((packed)) IgniRndCmdConfigure;

/// 
/// @brief Load mesh from file
///
typedef struct {
	IgniRndElementId meshId;
	uint8_t pathLen;
	char path[];
}__attribute__((packed)) IgniRndCmdMeshCreate;

/// 
/// @brief Apply shader to mesh
///
typedef struct {
	IgniRndElementId meshId;
	IgniRndShader shader;
}__attribute__((packed)) IgniRndCmdMeshSetShader;

/// 
/// @brief Bind texture to mesh's shader
///
typedef struct {
	IgniRndElementId meshId;
	IgniRndElementId textureId;
	IgniRndTextureTarget target;
}__attribute__((packed)) IgniRndCmdMeshBindTexture;

///
/// @brief Adjust mesh location, rotation and scale
///
typedef struct {
	IgniRndElementId meshId;
	float xLoc, yLoc, zLoc;
	float xRot, yRot, zRot;
	float xScale, yScale, zScale;
}__attribute__((packed)) IgniRndCmdMeshTransform;

/// 
/// @brief Remove mesh from scene
///
typedef struct {
	IgniRndElementId meshId;
}__attribute__((packed)) IgniRndCmdMeshDelete;

/// 
/// @brief Add point light to scene
///
typedef struct {
	IgniRndElementId pointLightId;
}__attribute__((packed)) IgniRndCmdPointLightCreate;

/// 
/// @brief Adjust point light location
///
typedef struct {
	IgniRndElementId pointLightId;
	float xLoc, yLoc, zLoc;
}__attribute__((packed)) IgniRndCmdPointLightTransform;

/// 
/// @brief Adjust point light colour properties
///
typedef struct {
	IgniRndElementId pointLightId;
	float r, g, b;
}__attribute__((packed)) IgniRndCmdPointLightSetColour;

/// 
/// @brief Remove point light from scene
///
typedef struct {
	IgniRndElementId pointLightId;
}__attribute__((packed)) IgniRndCmdPointLightDelete;

typedef struct {
	IgniRndElementId textureId;
	uint8_t pathLen;
	char path[];
}__attribute__((packed)) IgniRndCmdTextureCreate;

/// 
/// @brief Remove texture from scene
///
typedef struct {
	IgniRndElementId textureId;
}__attribute__((packed)) IgniRndCmdTextureDelete;

///
/// @brief Adjust viewpoint placement
///
typedef struct {
	float xLoc, yLoc, zLoc;
	float xLook, yLook, zLook;
	float fov;
}__attribute__((packed)) IgniRndCmdViewpointTransform;

///
/// @brief Open new Igni Render connection
///
/// @return Non-negative file descriptor. -1 if an error occurred.
///
int igniRndOpen();

///
/// @brief Load mesh from file
///
/// @param fd 		File descriptor of server socket
/// @param id 		Mesh identification number
/// @param path 	Relative or absolute mesh pathname
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndMeshCreate(
	int fd,
	IgniRndElementId id,
	const char* path
);

/// 
/// @brief Set mode of shading applied to mesh
///
/// @param fd 		File descriptor of server socket
/// @param id 		Mesh identification number
/// @param shader 	Shader type to apply
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndMeshSetShader(
	int fd,
	IgniRndElementId id,
	IgniRndShader shader
);

/// 
/// @brief Apply texture to mesh surface
///
/// @param fd 		File descriptor of server socket
/// @param meshId 	Mesh identification number
/// @param texId 	Texture identification number
/// @param target 	G-buffer which the texture affects
///
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndMeshBindTexture(
	int fd,
	IgniRndElementId meshId,
	IgniRndElementId texId,
	IgniRndTextureTarget target
);

/// 
/// @brief Set mesh location, rotation and scale
///
/// @param fd 		File descriptor of server socket
/// @param id 		Mesh identification number
/// @param tf		New transformation of mesh
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndMeshTransform(
	int fd,
	IgniRndElementId id,
	IgniTransform tf
);

/// 
/// @brief Remove mesh from scene
///
/// @param fd 		File descriptor of server socket
/// @param id 		Mesh identification number
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndMeshDelete(
	int fd,
	IgniRndElementId id
);

/// 
/// @brief Add point light to scene
///
/// @param fd 		File descriptor of server socket
/// @param id 		Point light identification number
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndPointLightCreate(
	int fd,
	IgniRndElementId id
);

/// 
/// @brief Set point light location
///
/// @param fd 		File descriptor of server socket
/// @param id 		Point light identification number
/// @param tf 		New location of point light
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndPointLightTransform(
	int fd,
	IgniRndElementId id,
	IgniVec3 tf
);

/// 
/// @brief Set point light colour
///
/// @param fd 		File descriptor of server socket
/// @param id 		Point light identification number
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndPointLightSetColour(
	int fd,
	IgniRndElementId id,
	IgniVec3 colour
);

/// 
/// @brief Remove point light from scene
///
/// @param fd 		File descriptor of server socket
/// @param id 		Point light identification number
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndPointLightDelete(
	int fd,
	IgniRndElementId id
);

///
/// @brief Load image texture
///
/// @param fd 		File descriptor of server socket
/// @param id 		Texture identification number
/// @param path 	Relative or absolute image pathname
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndTextureCreate(
	int fd,
	IgniRndElementId id,
	const char* path
);

/// 
/// @brief Remove texture from scene
///
/// @param fd 		File descriptor of server socket
/// @param id 		Texture identification number
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndTextureDelete(
	int fd,
	IgniRndElementId id
);

/// 
/// @brief Set viewpoint location and look target
/// 
///
/// @param fd 		File descriptor of server socket
/// @param tf 		Viewpoint location and look
/// @param fov 		Field of view in radians
/// @return 0 upon success. -1 to indicate an error.
///
int igniRndViewpointTransform(
	int fd,
	IgniViewTransform tf,
	float fov
);

#endif

