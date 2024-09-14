#include "render.h"
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <sys/un.h>
#include <stdlib.h>
#include <linux/limits.h>


int igniRndOpen()
{
	/* The Igni renderer recieves data through a UNIX domain socket. */

	char* socketPath = getenv("IGNI_RENDER_SRV");

	if (!socketPath) {
		printf("Could not find environment variable 'IGNI_RENDER_SRV'.\n");
		return -1;
	}

	struct sockaddr_un svAddr;
	svAddr.sun_family = AF_UNIX;
	strncpy(svAddr.sun_path, socketPath, sizeof(svAddr.sun_path));

	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	if (connect(fd, (struct sockaddr*)&svAddr, sizeof(svAddr)) == -1) {
		perror("Failed to connect to server");
	}

	/* The new connection tells the server about itself. */

	struct ConfigureCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdConfigure cmd;
	};

	struct ConfigureCmd_t configureCmd = {};
	configureCmd.opcode = IGNI_RENDER_OP_CONFIGURE;
	configureCmd.cmd.majVersion = 0;

	if (send(fd, &configureCmd, sizeof(configureCmd), 0) == -1) {
		perror("send() in igniRndOpen() failed");
		return -1;
	}

	return fd;
}

/* One-time commands 
 *
 * All the following functions work similarly: 
 * Step 1. Generate a packet from parameters given. 
 * Step 2. Send the packet to the server. 
 * Step 3. Return */

int igniRndMeshCreate(
	int fd,
	IgniRndElementId id,
	const char* path
)
{
	struct MeshCreateCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdMeshCreate cmd;
	};

	/* PATH_MAX could be some crazy large number so it's best to store this
	 * on the stack. */

	struct MeshCreateCmd_t* meshCreate;
	meshCreate = malloc(sizeof(struct MeshCreateCmd_t) + PATH_MAX);

	meshCreate->opcode = IGNI_RENDER_OP_MESH_CREATE;
	meshCreate->cmd.meshId = id;
	if (!realpath(path, meshCreate->cmd.path)) {
		perror("realpath() in igniRndMeshCreate() failed");
		free(meshCreate);
		return -1;
	}
	meshCreate->cmd.pathLen = strlen(meshCreate->cmd.path);
	size_t meshCreateSz = sizeof(*meshCreate) + meshCreate->cmd.pathLen;

	int sendResult = send(fd, meshCreate, meshCreateSz, 0);

	free(meshCreate);

	if (sendResult == -1) {
		perror("send() in igniRndMeshCreate() failed");
		return -1;
	}

	return 0;
}


int igniRndMeshSetShader(
	int fd,
	IgniRndElementId id,
	IgniRndShader shader
)
{
	struct MeshSetShaderCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdMeshSetShader cmd;
	};

	struct MeshSetShaderCmd_t meshSetShader = {};
	meshSetShader.opcode = IGNI_RENDER_OP_MESH_SET_SHADER;
	meshSetShader.cmd.meshId = id;
	meshSetShader.cmd.shader = shader;

	if (send(fd, &meshSetShader, sizeof(meshSetShader), 0) == -1) {
		perror("send() in igniRndMeshSetShader() failed");
		return -1;
	}

	return 0;
}


int igniRndMeshBindTexture(
	int fd,
	IgniRndElementId meshId,
	IgniRndElementId texId,
	IgniRndTextureTarget target
)
{
	struct MeshBindTextureCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdMeshBindTexture cmd;
	};

	struct MeshBindTextureCmd_t meshBindTexture = {};
	meshBindTexture.opcode = IGNI_RENDER_OP_MESH_BIND_TEXTURE;
	meshBindTexture.cmd.textureId = texId;
	meshBindTexture.cmd.meshId = meshId;

	if (send(fd, &meshBindTexture, sizeof(meshBindTexture), 0) == -1) {
		perror("send() in igniRndMeshBindTexture() failed");
		return -1;
	}

	return 0;
}


int igniRndMeshTransform(
	int fd,
	IgniRndElementId id,
	IgniTransform tf
)
{
	struct MeshTransformCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdMeshTransform cmd;
	};

	struct MeshTransformCmd_t meshTransform = {};
	meshTransform.opcode = IGNI_RENDER_OP_MESH_TRANSFORM;
	meshTransform.cmd.meshId = id;

	meshTransform.cmd.xLoc = tf.location.x;
	meshTransform.cmd.yLoc = tf.location.y;
	meshTransform.cmd.zLoc = tf.location.z;
	
	meshTransform.cmd.xRot = tf.rotation.x;
	meshTransform.cmd.yRot = tf.rotation.y;
	meshTransform.cmd.zRot = tf.rotation.z;

	meshTransform.cmd.xScale = tf.scale.x;
	meshTransform.cmd.yScale = tf.scale.y;
	meshTransform.cmd.zScale = tf.scale.z;

	if (send(fd, &meshTransform, sizeof(meshTransform), 0) == -1) {
		perror("send() in igniRndMeshTransform() failed");
		return -1;
	}

	return 0;
}


int igniRndMeshDelete(
	int fd,
	IgniRndElementId id
)
{
	struct MeshDeleteCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdMeshDelete cmd;
	};

	struct MeshDeleteCmd_t meshDelete = {};
	meshDelete.opcode = IGNI_RENDER_OP_MESH_DELETE;
	meshDelete.cmd.meshId = id;

	if (send(fd, &meshDelete, sizeof(meshDelete), 0) == -1) {
		perror("send() in igniRndMeshDelete() failed");
		return -1;
	}

	return 0;
}


int igniRndPointLightCreate(
	int fd,
	IgniRndElementId id
)
{
	struct PointLightCreateCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdPointLightCreate cmd;
	};

	struct PointLightCreateCmd_t pointLightCreate = {};
	pointLightCreate.opcode = IGNI_RENDER_OP_POINT_LIGHT_CREATE;
	pointLightCreate.cmd.pointLightId = id;

	if (send(fd, &pointLightCreate, sizeof(pointLightCreate), 0) == -1) {
		perror("send() in igniRndPointLightCreate() failed");
		return -1;
	}

	return 0;
}


int igniRndPointLightTransform(
	int fd,
	IgniRndElementId id,
	IgniVec3 tf
)
{
	struct PointLightTransformCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdPointLightTransform cmd;
	};

	struct PointLightTransformCmd_t pointLightTransform = {};
	pointLightTransform.opcode = IGNI_RENDER_OP_POINT_LIGHT_TRANSFORM;
	pointLightTransform.cmd.pointLightId = id;
	pointLightTransform.cmd.xLoc = tf.x;
	pointLightTransform.cmd.yLoc = tf.y;
	pointLightTransform.cmd.zLoc = tf.z;

	if (send(fd, &pointLightTransform, sizeof(pointLightTransform), 0) == -1) {
		perror("send() in igniRndPointLightTransform() failed");
		return -1;
	}

	return 0;
}


int igniRndPointLightSetColour(
	int fd,
	IgniRndElementId id,
	IgniVec3 colour
)
{
	struct PointLightSetColour_t {
		IgniRndOpcode opcode;
		IgniRndCmdPointLightSetColour cmd;
	};

	struct PointLightSetColour_t pointLightSetColour = {};
	pointLightSetColour.opcode = IGNI_RENDER_OP_POINT_LIGHT_SET_COLOUR;
	pointLightSetColour.cmd.pointLightId = id;
	pointLightSetColour.cmd.r = colour.r;
	pointLightSetColour.cmd.g = colour.g;
	pointLightSetColour.cmd.b = colour.b;

	if (send(fd, &pointLightSetColour, sizeof(pointLightSetColour), 0) == -1) {
		perror("send() in igniRndPointLightSetColour() failed");
		return -1;
	}

	return 0;
}


int igniRndPointLightDelete(
	int fd,
	IgniRndElementId id
)
{
	struct PointLightDeleteCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdPointLightDelete cmd;
	};
	
	struct PointLightDeleteCmd_t pointLightDelete = {};
	pointLightDelete.opcode = IGNI_RENDER_OP_POINT_LIGHT_DELETE;
	pointLightDelete.cmd.pointLightId = id;

	if (send(fd, &pointLightDelete, sizeof(pointLightDelete), 0) == -1) {
		perror("send() in igniRndPointLightDelete() failed");
		return -1;
	}

	return 0;
}


int igniRndTextureCreate(
	int fd,
	IgniRndElementId id,
	const char* path
)
{
	struct createTextureCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdTextureCreate cmd;
	};

	struct createTextureCmd_t* createTex;
	createTex = malloc(sizeof(struct createTextureCmd_t) + PATH_MAX);

	createTex->opcode = IGNI_RENDER_OP_TEXTURE_CREATE;
	createTex->cmd.textureId = id;

	if (!realpath(path, createTex->cmd.path)) {
		perror("realpath() in igniRndTextureCreate() failed");
		free(createTex);
		return -1;
	}
	createTex->cmd.pathLen = strlen(createTex->cmd.path);
	size_t texCreateSz = sizeof(*createTex) + createTex->cmd.pathLen;

	int sendResult = send(fd, createTex, texCreateSz, 0);

	free(createTex);

	if (sendResult == -1) {
		perror("send() in igniRndTextureCreate() failed");
		return -1;
	}

	return 0;
}


int igniRndTextureDelete(
	int fd,
	IgniRndElementId id
)
{
	struct deleteTextureCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdTextureDelete cmd;
	};

	struct deleteTextureCmd_t deleteTex = {};
	deleteTex.opcode = IGNI_RENDER_OP_TEXTURE_DELETE;
	deleteTex.cmd.textureId = id;

	if (send(fd, &deleteTex, sizeof(deleteTex), 0) == -1) {
		perror("send() in igniRndTextureDelete() failed");
		return -1;
	}

	return 0;
}


int igniRndViewpointTransform(
	int fd,
	IgniViewTransform tf,
	float fov
)
{
	struct tfViewpointCmd_t {
		IgniRndOpcode opcode;
		IgniRndCmdViewpointTransform cmd;
	};

	struct tfViewpointCmd_t tfViewpoint = {};
	tfViewpoint.opcode = IGNI_RENDER_OP_VIEWPOINT_TRANSFORM;	
	tfViewpoint.cmd.fov = fov;
	
	tfViewpoint.cmd.xLoc = tf.location.x;
	tfViewpoint.cmd.yLoc = tf.location.y;
	tfViewpoint.cmd.zLoc = tf.location.z;

	tfViewpoint.cmd.xLook = tf.lookAt.x;
	tfViewpoint.cmd.yLook = tf.lookAt.y;
	tfViewpoint.cmd.zLook = tf.lookAt.z;

	if (send(fd, &tfViewpoint, sizeof(tfViewpoint), 0) == -1) {
		perror("send() in igniRndViewpointTransform() failed");
		return -1;
	}

	return 0;
}


