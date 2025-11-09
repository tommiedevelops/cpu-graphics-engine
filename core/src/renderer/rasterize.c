#include "render.h"
#include "frag_shader.h"
#include "game_math/barycentric.h"
#include "game_math/bounds.h"
#include "triangle.h"
#include "framebuffer.h"

static inline bool inside_triangle(BaryCoords b){
	return (b.alpha > 0) && (b.beta > 0) && (b.gamma > 0) && (b.alpha <= 1) && (b.beta <= 1) && (b.gamma <= 1);
}

typedef struct Vec2i {
	int x, y;
} Vec2i;

void rasterize_pixel(Vec2i P, int w0, int w1, int w2, int area, FSin* out, VSout** v) {

	BaryCoords b = (BaryCoords){(float)w0/area, (float)w1/area, (float)w2/area};

	// Depth (Screen Space Z)
	const float depth = bary_mix1(b, v[0]->pos.z, v[1]->pos.z, v[2]->pos.z);	

	// Perspective Correct Attributes
	const float w_inv = bary_mix1(b, v[0]->w_inv, v[1]->w_inv, v[2]->w_inv);
	const Vec2f uv_over_w = bary_mix2(b, v[0]->uv_over_w, v[1]->uv_over_w, v[2]->uv_over_w);
	const Vec2f uv = vec2f_scale(uv_over_w, 1.0f/w_inv);

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

void rasterize_triangle(Renderer* r, FrameBuffer* fb, Triangle* tri, FragShaderF frag_shader) {
	
	FSin  fs_in;
	FSout fs_out;
	
	Bounds b = tri_get_bounds(tri);

	// Assuming CCW winding from 0 to 2
	
	VSout** v = tri->v;

	Vec2i V0 = (Vec2i){(int)floorf(v[0]->pos.x), (int)floorf(v[0]->pos.y)};
	Vec2i V1 = (Vec2i){(int)floorf(v[1]->pos.x), (int)floorf(v[1]->pos.y)};
	Vec2i V2 = (Vec2i){(int)floorf(v[2]->pos.x), (int)floorf(v[2]->pos.y)};

	int xmin = max_i(0, (int)ceilf(b.xmin));
	int ymin = max_i(0, (int)ceilf(b.ymin));
	int xmax = min_i(fb->width - 1, (int)floorf(b.xmax));
	int ymax = min_i(fb->height - 1, (int)floorf(b.ymax)); 

	// ignore redundat triangles
	if(xmin >= xmax || ymin >= ymax) return;

	Vec2i P = (Vec2i){xmin, ymin};

	int area = edge_func(V0, V1, V2); // area of tri
	if(area == 0) return;

	// Values at the start of each row
	int w0_row = edge_func(P, V0, V1); 
	int w1_row = edge_func(P, V1, V2);
	int w2_row = edge_func(P, V2, V0);

	// incremenets
	int A01 = (V0.y - V1.y), B01 = (V1.x - V0.x);
	int A12 = (V1.y - V2.y), B12 = (V2.x - V1.x);
	int A20 = (V2.y - V0.y), B20 = (V0.x - V2.x);

	for(P.y; P.y<= ymax; P.y++){

		int w0 = w0_row;
		int w1 = w1_row;
		int w2 = w2_row;

		for(P.x; P.x <= xmax; P.x++){
			
			if(w0 >= 0 && w1 >= 0 && w2 >= 0)
				rasterize_pixel(P,w0,w1,w2,area,&fs_in,tri->v);

			frag_shader(&fs_in, &fs_out, r->fs_u);
			frame_buffer_draw_pixel(fb,P.x,P.y,vec4f_to_rgba32(fs_out.color),fs_out.depth);
			
			// Step to the right
			w0 += A01;
			w1 += A12;
			w2 += A20;
		}

		// Step up one row
		w0_row += B01;
		w1_row += B12;
		w2_row += B20;
	}

}



