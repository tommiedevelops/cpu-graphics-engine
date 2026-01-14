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

void rasterize_pixel(Vec2i P, int w0, int w1, int w2, int area, FSin* out, VSout** v) {

	BaryCoords b = (BaryCoords){(float)w0/area, (float)w1/area, (float)w2/area};

	// Depth (Screen Space Z)
	const float depth = bary_mix1(b, v[0]->pos.z, v[1]->pos.z, v[2]->pos.z);	

	// Perspective Correct Attributes
	const Vec2f uv = bary_mix2(b, v[0]->uv, v[1]->uv, v[2]->uv);

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

void rasterize_triangle(Renderer* r, FrameBuffer* fb, Triangle* tri, FragShaderF frag_shader) {
	
	FSin  fs_in;
	FSout fs_out;
	
	Bounds b = tri_get_bounds(tri);

	int xmin = max_i(0, (int)ceilf(b.xmin));
	int ymin = max_i(0, (int)ceilf(b.ymin));
	int xmax = min_i(fb->width - 1, (int)floorf(b.xmax));
	int ymax = min_i(fb->height - 1, (int)floorf(b.ymax)); 

	if(xmin > xmax || ymin > ymax) return;

	// Assuming CCW winding from 0 to 2
	VSout** v = tri->v;
	Vec2i V0 = (Vec2i){(int)floorf(v[0]->pos.x), (int)floorf(v[0]->pos.y)};
	Vec2i V1 = (Vec2i){(int)floorf(v[1]->pos.x), (int)floorf(v[1]->pos.y)};
	Vec2i V2 = (Vec2i){(int)floorf(v[2]->pos.x), (int)floorf(v[2]->pos.y)};

	// delta vectors
	Vec2i A01 = vec2i_sub(V1,V0);
	Vec2i A12 = vec2i_sub(V2,V1);
	Vec2i A20 = vec2i_sub(V0,V2);

	Vec2i P = (Vec2i){xmin, ymin};

	// initial edge function for (xmin, ymin)
	int e01_row = edge_func(P,V0,V1);
	int e12_row = edge_func(P,V1,V2);
	int e20_row = edge_func(P,V2,V0);

	// area of triangle
	int area = edge_func(V0, V1, V2);
	if(area == 0) return;

	for(P.y = ymin; P.y <= ymax; P.y++){
		int e01 = e01_row;		
		int e12 = e12_row;
		int e20 = e20_row;
			
		for(P.x = xmin; P.x <= xmax; P.x++){
			
			if(inside_triangle(e01,e12,e20)) {
				rasterize_pixel(P,e01,e12,e20,area,&fs_in,tri->v);
				frag_shader(&fs_in, &fs_out, r->fs_u);
				frame_buffer_draw_pixel(fb,P.x,P.y,vec4f_to_rgba32(fs_out.color),fs_out.depth);
			}

			e01 -= A01.y;
			e12 -= A12.y;
			e20 -= A20.y;
		}

		e01_row += A01.x;
		e12_row += A12.x;
		e20_row += A20.x;
	}

}



