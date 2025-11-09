#include "render.h"
#include "frag_shader.h"
#include "game_math/barycentric.h"
#include "game_math/bounds.h"
#include "triangle.h"
#include "framebuffer.h"

static inline bool inside_triangle(BaryCoords b){
	return (b.alpha > 0) && (b.beta > 0) && (b.gamma > 0) && (b.alpha <= 1) && (b.beta <= 1) && (b.gamma <= 1);
}

bool rasterize_pixel(int x, int y, const Triangle* tri, FSin* out) {

	VSout* const* v = tri->v;

	Vec2f A = (Vec2f){v[0]->pos.x, v[0]->pos.y};
	Vec2f B = (Vec2f){v[1]->pos.x, v[1]->pos.y};
	Vec2f C = (Vec2f){v[2]->pos.x, v[2]->pos.y};
	Vec2f P = (Vec2f){ x, y };

	BaryCoords b = cartesian_to_bary(A,B,C,P);
	if(!inside_triangle(b)) return false;

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

	return true;
}

static inline int max_i(int a, int b) {return a > b ? a : b; }
static inline int min_i(int a, int b) {return a < b ? a : b; }

void rasterize_triangle(Renderer* r, FrameBuffer* fb, Triangle* tri, FragShaderF frag_shader) {
	
	FSin  fs_in;
	FSout fs_out;
	
	Bounds b = tri_get_bounds(tri);
	
	int xmin = max_i(0, (int)ceilf(b.xmin));
	int ymin = max_i(0, (int)ceilf(b.ymin));
	int xmax = min_i(fb->width - 1, (int)floorf(b.xmax));
	int ymax = min_i(fb->height - 1, (int)floorf(b.ymax)); 

	if(xmin > xmax || ymin > ymax) return;
	for(int y = ymin; y <= ymax; y++){
	    for(int x = xmin; x <= xmax; x++){
                  if( !rasterize_pixel(x,y,tri, &fs_in) ) continue;
		  frag_shader(&fs_in, &fs_out, r->fs_u);
		  frame_buffer_draw_pixel(fb,x,y,vec4f_to_rgba32(fs_out.color),fs_out.depth);
	    }
	}
	
}



