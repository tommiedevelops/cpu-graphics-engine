#include "render.h"
#include "frag_shader.h"
#include "game_math/barycentric.h"
#include "game_math/bounds.h"
#include "triangle.h"
#include "framebuffer.h"

typedef struct Vec2i {
	int x, y;
} Vec2i;

Vec2i vec2i_sub(Vec2i a, Vec2i b){
	return (Vec2i){a.x - b.x, a.y - b.y};
}

void rasterize_pixel(Vec2i P,BaryCoords b, FSin* out, VSout** v) {

	// Depth (Screen Space Z)
	const float depth = bary_mix1(b, v[0]->pos.z, v[1]->pos.z, v[2]->pos.z);	
	// Perspective Correct Attributes
	const float w_inv = bary_mix1(b, v[0]->w_inv, v[1]->w_inv, v[2]->w_inv);
	const Vec2f uv_over_w 
		= bary_mix2(b, v[0]->uv_over_w, v[1]->uv_over_w, v[2]->uv_over_w);

	Vec2f uv = vec2f_scale(uv_over_w, 1.0f/w_inv);

	// Other linearly interpolated values
	Vec3f normal = vec3f_normalize(bary_mix3(b,v[0]->normal, v[1]->normal, v[2]->normal));
	Vec3f world_pos  = bary_mix3(b, v[0]->world_pos, v[1]->world_pos, v[2]->world_pos);

	*out = (FSin) {
		.world_pos = world_pos,
		.normal   = normal,
		.uv       = uv,	
		.depth    = depth
	};

}

static inline int max_i(int a, int b) {return a > b ? a : b; }
static inline int min_i(int a, int b) {return a < b ? a : b; }

static inline int edge_func(Vec2i P, Vec2i A, Vec2i B) {
	return (B.x - A.x)*(P.y - A.y) - (B.y - A.y)*(P.x - A.x);
}

static inline bool inside_triangle(int e01, int e12, int e20) {
	return (e01 >= 0) && (e12 >= 0) && (e20 >= 0);
}

static inline Vec2i to_pixel_center(Vec4f p) {
	return (Vec2i){ (int)floorf(p.x + 0.5f), (int)floorf(p.y + 0.5f) };
}

typedef struct {
	int e_row;  // edge value (xmin, current y)
	int step_x; // how the edge changes when x++
	int step_y; // how the edge changes when y++
} EdgeStepper;

static inline EdgeStepper make_edge(Vec2i P, Vec2i A, Vec2i B) {
	Vec2i d = vec2i_sub(B, A);
	
	return (EdgeStepper) {
		.e_row = edge_func(P, A, B),
		.step_x = -d.y,
		.step_y = d.x
	};
}

typedef struct { int xmin, ymin, xmax, ymax; } Recti;

static inline bool tri_clamped_bounds(const Triangle* tri, const FrameBuffer* fb, Recti* out) {
	Bounds b = tri_get_bounds(tri);

	int xmin = max_i(0, (int)ceilf(b.xmin));
	int ymin = max_i(0, (int)ceilf(b.ymin));
	int xmax = min_i(fb->width - 1, (int)floorf(b.xmax));
	int ymax = min_i(fb->height - 1, (int)floorf(b.ymax)); 

	if(xmin > xmax || ymin > ymax) return false;
	*out = (Recti){ xmin, ymin, xmax, ymax};
	return true;
}

static inline void compute_bary_coords(BaryCoords* out, int e12, int e20, int e01, int area) {
	*out = (BaryCoords){(float)e12/area, (float)e20/area, (float)e01/area};
}

void rasterize_triangle(Renderer* r, FrameBuffer* fb, Triangle* tri, FragShaderF frag_shader) {
	
	FSin  fs_in;
	FSout fs_out;
	
	Recti box;
	if(!tri_clamped_bounds(tri, fb, &box)) return;

	Vec2i V0 = to_pixel_center(tri->v[0]->pos); 
	Vec2i V1 = to_pixel_center(tri->v[1]->pos); 
	Vec2i V2 = to_pixel_center(tri->v[2]->pos); 
	Vec2i P = (Vec2i){box.xmin, box.ymin};

	EdgeStepper e01 = make_edge(P, V0, V1);
	EdgeStepper e12 = make_edge(P, V1, V2);
	EdgeStepper e20 = make_edge(P, V2, V0);

	int area = edge_func(V0, V1, V2);
	BaryCoords b;

	for(P.y = box.ymin; P.y <= box.ymax; P.y++){
		int e01_xy = e01.e_row;		
		int e12_xy = e12.e_row;
		int e20_xy = e20.e_row;
			
		for(P.x = box.xmin; P.x <= box.xmax; P.x++){
			if(inside_triangle(e01_xy,e12_xy,e20_xy)) {
				compute_bary_coords(&b, e12_xy, e20_xy, e01_xy, area);
				rasterize_pixel(P,b,&fs_in,tri->v);
				frag_shader(&fs_in, &fs_out, r->fs_u);
				frame_buffer_draw_pixel(fb,P.x,P.y,
					vec4f_to_rgba32(fs_out.color),fs_out.depth);
			}

			e01_xy += e01.step_x;
			e12_xy += e12.step_x;
			e20_xy += e20.step_x;
		}

		e01.e_row += e01.step_y;
		e12.e_row += e12.step_y;
		e20.e_row += e20.step_y;
	}

}



