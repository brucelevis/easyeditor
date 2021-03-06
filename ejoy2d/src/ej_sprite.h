#ifndef EJOY_2D_SPRITE_H
#define EJOY_2D_SPRITE_H

#include "spritepack.h"
#include "matrix.h"

#include <dtex.h>

#include <lua.h>
#include <stdint.h>
#include <stdbool.h>

#define SPRFLAG_INVISIBLE               (0x00000001)
#define SPRFLAG_MESSAGE                 (0x00000002)
#define SPRFLAG_MULTIMOUNT              (0x00000004)
#define SPRFLAG_FORCE_INHERIT_FRAME     (0x00000008)
#define SPRFLAG_RANDOM_CHILD_BASE_FRAME (0x00000010)

struct material;

struct anchor_data {
	struct particle_system *ps;
	struct pack_picture *pic;
	struct matrix mat;
	struct ej_trail_t *trail;
};

struct sprite {
	struct dtex_package* pkg;
	struct sprite * parent;
	uint16_t type;
	uint16_t id;
	struct sprite_trans t;
	union {
		struct pack_animation *ani;
		struct pack_picture *pic;
		struct pack_polygon *poly;
		struct pack_label *label;
		struct pack_pannel *pannel;
		struct matrix *mat;
		struct p3d_emitter_cfg* p3d_cfg;
		struct p2d_emitter_cfg* p2d_cfg;
	} s;
	struct matrix mat;
	int start_frame;
	int total_frame;
	int frame;
	int flags;
	int time;
	const char *name;	// name for parent
	struct material *material;
	union {
		struct p3d_emitter* p3d;
		struct p2d_emitter* p2d;
	} ext;
	union {
		struct sprite * children[1];
		const char* text;
		int scissor;
		struct anchor_data *anchor;
	} data;
};


// sprite_size must be call before sprite_init
int sprite_size(struct dtex_package* pkg, int id);
void sprite_init(struct sprite *, struct dtex_package* pkg, int id, int sz);

int sprite_setframe(struct sprite *, int frame, bool force_child);

struct sprite_trans * sprite_trans_mul(struct sprite_trans *a, struct sprite_trans *b, struct sprite_trans *t, struct matrix *tmp_matrix);

// return sprite id in pack, -1 for end
int sprite_component(struct sprite *, int index);
const char * sprite_childname(struct sprite *, int index);
void sprite_mount(struct sprite *, int index, struct sprite *);

#endif
