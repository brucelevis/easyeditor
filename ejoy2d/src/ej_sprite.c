#include "ej_sprite.h"
#include "spritepack.h"
#include "shader.h"
#include "matrix.h"
#include "shader.h"

#include "ps_3d.h"

#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <limits.h>

#define CC_DEGREES_TO_RADIANS(__ANGLE__) ((__ANGLE__) * 0.01745329252f) // PI / 180
#define CC_RADIANS_TO_DEGREES(__ANGLE__) ((__ANGLE__) * 57.29577951f) // PI * 180

int
sprite_size(struct dtex_package* pkg, int id) {
	struct sprite_pack* ej_pkg = pkg->ej_pkg;
	if (id < 0 || id >=	ej_pkg->n)
		return 0;
	int type = ej_pkg->type[id];
	if (type == TYPE_ANIMATION) {
		struct pack_animation * ani = (struct pack_animation *)ej_pkg->data[id];
		return sizeof(struct sprite) + (ani->component_number - 1) * sizeof(struct sprite *);
	} else if (type == TYPE_PARTICLE3D) {
		struct p3d_emitter_cfg* p3d_cfg = (struct p3d_emitter_cfg*)ej_pkg->data[id];
		return sizeof(struct sprite) + (p3d_cfg->symbol_count - 1) * sizeof(struct sprite*);
	} else if (type == TYPE_PARTICLE2D) {
		struct p2d_emitter_cfg* p2d_cfg = (struct p2d_emitter_cfg*)ej_pkg->data[id];
		return sizeof(struct sprite) + (p2d_cfg->symbol_count - 1) * sizeof(struct sprite*);
	} else {
		return sizeof(struct sprite);
	}
	return 0;
}

int
sprite_action(struct sprite *s, const char * action) {
	if (s->type != TYPE_ANIMATION) {
		return -1;
	}
	struct pack_animation *ani = s->s.ani;
	if (action == NULL) {
		if (ani->action == NULL) {
			return -1;
		}
		s->start_frame = ani->action[0].start_frame;
		s->total_frame = ani->action[0].number;
		s->frame = 0;
		return s->total_frame;
	} else {
		int i;
		for (i=0;i<ani->action_number;i++) {
			const char *name = ani->action[i].name;
			if (name) {
				if (strcmp(name, action)==0) {
					s->start_frame = ani->action[i].start_frame;
					s->total_frame = ani->action[i].number;
					s->frame = 0;
					return s->total_frame;
				}
			}
		}
		return -1;
	}
}

void
sprite_init(struct sprite * s, struct dtex_package* pkg, int id, int sz) {
	struct sprite_pack* ej_pkg = pkg->ej_pkg;

	if (id < 0 || id >=	ej_pkg->n)
		return;
	s->pkg = pkg;
	s->parent = NULL;
	s->t.mat = NULL;
	s->t.color = 0xffffffff;
	s->t.additive = 0;
	s->t.rmap = 0xff0000ff;
	s->t.gmap = 0x00ff00ff;
	s->t.bmap = 0x0000ffff;
	s->t.program = PROGRAM_DEFAULT;
	s->flags = 0;
	s->time = 0;
	s->name = NULL;
	s->id = id;
	s->type = ej_pkg->type[id];
	s->material = NULL;
	s->start_frame = 0;
	s->total_frame = 0;
	s->frame = 0;
	s->ext.p3d = NULL;
	if (s->type == TYPE_ANIMATION) {
		struct pack_animation * ani = (struct pack_animation *)ej_pkg->data[id];
		s->s.ani = ani;
		sprite_action(s, NULL);
		int i;
		int n = ani->component_number;
		assert(sz >= sizeof(struct sprite) + (n - 1) * sizeof(struct sprite *));
		for (i=0; i<n ;i++) {
			s->data.children[i] = NULL;
		}
	} else if (s->type == TYPE_PARTICLE3D) {
		struct p3d_emitter_cfg* cfg = (struct p3d_emitter_cfg*)ej_pkg->data[id];
		s->s.p3d_cfg = cfg;
		int n = cfg->symbol_count;
		assert(sz >= sizeof(struct sprite) + (n - 1) * sizeof(struct sprite *));
		for (int i = 0; i < n ; ++i) {
			s->data.children[i] = NULL;
		}
		struct p3d_emitter* et = p3d_emitter_create(cfg);
		et->active = true;
		s->ext.p3d = et;
	} else if (s->type == TYPE_PARTICLE2D) {
		struct p2d_emitter_cfg* cfg = (struct p2d_emitter_cfg*)ej_pkg->data[id];
		s->s.p2d_cfg = cfg;
		int n = cfg->symbol_count;
		assert(sz >= sizeof(struct sprite) + (n - 1) * sizeof(struct sprite *));
		for (int i = 0; i < n ; ++i) {
			s->data.children[i] = NULL;
		}
		struct p2d_emitter* et = p2d_emitter_create(cfg);
		et->active = true;
		s->ext.p2d = et;
	} else {
		s->s.pic = (struct pack_picture *)ej_pkg->data[id];
		memset(&s->data, 0, sizeof(s->data));
		assert(sz >= sizeof(struct sprite) - sizeof(struct sprite *));
		if (s->type == TYPE_PANNEL) {
			struct pack_pannel * pp = (struct pack_pannel *)ej_pkg->data[id];
			s->data.scissor = pp->scissor;
		}
	}
}

static inline int
propagate_frame(struct sprite *s, int i, bool force_child) {
	struct sprite *child = s->data.children[i];
	if (child == NULL || 
		(child->type != TYPE_ANIMATION && child->type != TYPE_PARTICLE3D && child->type != TYPE_PARTICLE2D)) {
			return 0;
	}
	if (child->flags & SPRFLAG_FORCE_INHERIT_FRAME) {
		return 1;
	}
	struct pack_animation * ani = s->s.ani;
	if (ani->component[i].id == ANCHOR_ID) {
		return 0;
	}
	if (force_child) {
		return 1;
	}
	if (ani->component[i].name == NULL) {
		return 1;
	}
	return 0;
}

int
sprite_setframe(struct sprite *s, int frame, bool force_child) {
	if (s == NULL || (s->type != TYPE_ANIMATION && s->type != TYPE_PARTICLE3D && s->type != TYPE_PARTICLE2D))
		return 0;

	s->frame = frame;

	int total_frame = s->total_frame;
	if (s->type == TYPE_ANIMATION) {
		int i;
		struct pack_animation * ani = s->s.ani;
		for (i=0;i<ani->component_number;i++) {
			if (propagate_frame(s, i, force_child)) {
				if (s->flags & SPRFLAG_RANDOM_CHILD_BASE_FRAME) {
					frame = frame + ((i * 134775813) % 307);
				}
				int t = sprite_setframe(s->data.children[i], frame, force_child);
				if (t > total_frame) {
					total_frame = t;
				}
			}
		}
	}
	return total_frame;
}

static inline uint32_t
color_mul(uint32_t c1, uint32_t c2) {
	int r1 = (c1 >> 24) & 0xff;
	int g1 = (c1 >> 16) & 0xff;
	int b1 = (c1 >> 8) & 0xff;
	int a1 = (c1) & 0xff;
	int r2 = (c2 >> 24) & 0xff;
	int g2 = (c2 >> 16) & 0xff;
	int b2 = (c2 >> 8) & 0xff;
	int a2 = c2 & 0xff;

	return (r1 * r2 /255) << 24 |
		(g1 * g2 /255) << 16 |
		(b1 * b2 /255) << 8 |
		(a1 * a2 /255) ;
}

static inline unsigned int
clamp(unsigned int c) {
	return ((c) > 255 ? 255 : (c));
}

static inline uint32_t
color_add(uint32_t c1, uint32_t c2) {
	int r1 = (c1 >> 16) & 0xff;
	int g1 = (c1 >> 8) & 0xff;
	int b1 = (c1) & 0xff;
	int r2 = (c2 >> 16) & 0xff;
	int g2 = (c2 >> 8) & 0xff;
	int b2 = (c2) & 0xff;
	return clamp(r1+r2) << 16 |
		clamp(g1+g2) << 8 |
		clamp(b1+b2);
}

struct sprite_trans *
sprite_trans_mul(struct sprite_trans *a, struct sprite_trans *b, struct sprite_trans *t, struct matrix *tmp_matrix) {
	if (b == NULL) {
		return a;
	}

	*t = *a;
	if (t->mat == NULL) {
		t->mat = b->mat;
	} else if (b->mat) {
		matrix_mul(tmp_matrix, t->mat, b->mat);
		t->mat = tmp_matrix;
	}
	if (t->color == 0xffffffff) {
		t->color = b->color;
	} else if (b->color != 0xffffffff) {
		t->color = color_mul(t->color, b->color);
	}
	if (t->additive == 0) {
		t->additive = b->additive;
	} else if (b->additive != 0) {
		t->additive = color_add(t->additive, b->additive);
	}

	if (t->rmap == 0xff0000ff) {
		t->rmap = b->rmap;
	}
	if (t->gmap == 0x00ff00ff) {
		t->gmap = b->gmap;
	}
	if (t->bmap == 0x0000ffff) {
		t->bmap = b->bmap;
	}

	if (t->program == PROGRAM_DEFAULT) {
		t->program = b->program;
	}

	return t;
}

int
sprite_component(struct sprite *s, int index) {
	if (s->type == TYPE_ANIMATION) {
		struct pack_animation *ani = s->s.ani;
		if (index < 0 || index >= ani->component_number) {
			return -1;
		}
		return ani->component[index].id;
	} else if (s->type == TYPE_PARTICLE3D) {
		struct p3d_emitter_cfg* p3d_cfg = s->s.p3d_cfg;
		if (index < 0 || index >= p3d_cfg->symbol_count) {
			return -1;
		}
		uint32_t id = (uint32_t)p3d_cfg->symbols[index].ud;
		return (id >> 16) & 0xffff;
	} else if (s->type == TYPE_PARTICLE2D) {
		struct p2d_emitter_cfg* p2d_cfg = (struct p2d_emitter_cfg*)s->s.p2d_cfg;
		if (index < 0 || index >= p2d_cfg->symbol_count) {
			return -1;
		}
		uint32_t id = (uint32_t)p2d_cfg->symbols[index].ud;
		return (id >> 16) & 0xffff;
	} else {
		return -1;
	}
}

const char *
sprite_childname(struct sprite *s, int index) {
	if (s->type != TYPE_ANIMATION)
		return NULL;
	struct pack_animation *ani = s->s.ani;
	if (index < 0 || index >= ani->component_number)
		return NULL;
	return ani->component[index].name;
}

void
sprite_mount(struct sprite *parent, int index, struct sprite *child) {
	assert(parent->type == TYPE_ANIMATION || parent->type == TYPE_PARTICLE3D || parent->type == TYPE_PARTICLE2D);
	if (child && child->type == TYPE_PARTICLE3D && parent && !parent->ext.p3d) {
		struct p3d_emitter* et = p3d_emitter_create(child->s.p3d_cfg);
		et->active = true;
		parent->ext.p3d = et;
	} else if (child && child->type == TYPE_PARTICLE2D && parent && !parent->ext.p2d) {
		struct p2d_emitter* et = p2d_emitter_create(child->s.p2d_cfg);
		et->active = true;
		parent->ext.p2d = et;
	}

	int num = 0;
	if (parent->type == TYPE_ANIMATION) {
		struct pack_animation *ani = parent->s.ani;
		num = ani->component_number;
	} else if (parent->type == TYPE_PARTICLE3D) {
		num = parent->s.p3d_cfg->symbol_count;
	} else if (parent->type == TYPE_PARTICLE2D) {
		num = parent->s.p2d_cfg->symbol_count;
	}

	assert(index >= 0 && index < num);
	struct sprite * oldc = parent->data.children[index];
	if (oldc) {
		oldc->parent = NULL;
		oldc->name = NULL;
	}
	parent->data.children[index] = child;
	if (child) {
		assert(child->parent == NULL);
		if (parent->type == TYPE_ANIMATION &&
			(child->flags & SPRFLAG_MULTIMOUNT) == 0) {
				struct pack_animation *ani = parent->s.ani;
				child->name = ani->component[index].name;
				child->parent = parent;
		}
		if (oldc && oldc->type == TYPE_ANCHOR) {
			if(oldc->flags & SPRFLAG_MESSAGE) {
				child->flags |= SPRFLAG_MESSAGE;
			} else {
				child->flags &= ~SPRFLAG_MESSAGE;
			}
		}
	}
}