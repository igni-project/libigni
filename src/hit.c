#include "hit.h"
#include <stdio.h> /* perror() */
#include <sys/socket.h> /* send() */

int igniHitHitboxCreate(
	int fd,
	IgniHitElementId id
)
{
	struct HitboxCreateCmd {
		IgniHitOpcode opcode;
		IgniHitCmdHitboxCreate cmd;
	};

	struct HitboxCreateCmd hitboxCreate = {};
	hitboxCreate.opcode = IGNI_HIT_OP_HITBOX_CREATE;	
	hitboxCreate.cmd.hitboxId = id;	

	if (send(fd, &hitboxCreate, sizeof(hitboxCreate), 0) == -1) {
		perror("send() in igniHitHitboxCreate() failed");
		return -1;
	}

	return 0;
}

int igniHitHitboxTransform(
	int fd,
	IgniHitElementId id,
	IgniTransform tf
)
{
	struct HitboxTransformCmd {
		IgniHitOpcode opcode;
		IgniHitCmdHitboxTransform cmd;
	};

	struct HitboxTransformCmd hitboxTransform = {};
	hitboxTransform.opcode = IGNI_HIT_OP_HITBOX_TRANSFORM;	
	hitboxTransform.cmd.hitboxId = id;	

	hitboxTransform.cmd.xLoc = tf.location.x;
	hitboxTransform.cmd.yLoc = tf.location.y;
	hitboxTransform.cmd.zLoc = tf.location.z;
	
	hitboxTransform.cmd.xRot = tf.rotation.x;
	hitboxTransform.cmd.yRot = tf.rotation.y;
	hitboxTransform.cmd.zRot = tf.rotation.z;

	hitboxTransform.cmd.width = tf.scale.x;
	hitboxTransform.cmd.height = tf.scale.y;
	hitboxTransform.cmd.depth = tf.scale.z;

	if (send(fd, &hitboxTransform, sizeof(hitboxTransform), 0) == -1) {
		perror("send() in igniHitHitboxTransform() failed");
		return -1;
	}

	return 0;
}

int igniHitHitboxDelete(
	int fd,
	IgniHitElementId id
)
{
	struct HitboxDeleteCmd {
		IgniHitOpcode opcode;
		IgniHitCmdHitboxDelete cmd;
	};

	struct HitboxDeleteCmd hitboxDelete = {};
	hitboxDelete.opcode = IGNI_HIT_OP_HITBOX_CREATE;	
	hitboxDelete.cmd.hitboxId = id;	

	if (send(fd, &hitboxDelete, sizeof(hitboxDelete), 0) == -1) {
		perror("send() in igniHitHitboxDelete() failed");
		return -1;
	}

	return 0;
}

