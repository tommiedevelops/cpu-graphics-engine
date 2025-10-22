#include "clip.h"
#include "lerp.h"
#include "plane.h"
#include "vert_shader.h"

static inline void copy_vals(Vec4f* from, Vec4f* to, int num_verts){
	for(int i = 0; i < num_verts; i++){
		to[i] = from[i];
	}
}

static void lerp_vertex(const VSout* u, const VSout* v, VSout* res, float t) {
	// clamp t
	t = (t > 1.0f) ? 1.0f : t;
	t = (t < 0.0f) ? 0.0f : t;
	res->pos = lerp_vec4f(u->pos, v->pos, t);
	res->view_pos = lerp_vec3f(u->view_pos, v->view_pos, t);
	res->normal = lerp_vec3f(u->normal, v->normal, t);
	res->uv_over_w = lerp_vec2f(u->uv_over_w, v->uv_over_w, t);
	res->w_inv = lerp_float(u->w_inv, v->w_inv, t);
}

static void prepare_clipping_planes(struct Plane4* planes){
	/* all normals facing 'plane4_inside'*/
	/* plane4_inside => -w<=x<=w, -w<=y<=w, 0<=z<=w*/
	//top (y = w)
	planes[0].n = vec4f_create(0.0f, -1.0f, 0.0f, 1.0f);
	planes[0].p = vec4f_create(0.0f, 1.0f, 0.0f, 1.0f);

	//bottom (y = -w)
	planes[1].n = vec4f_create(0.0f, 1.0f, 0.0f, 1.0f);
	planes[1].p = vec4f_create(0.0f, -1.0f, 0.0f, 1.0f);

	// left (x = -w)
	planes[2].n = vec4f_create(1.0f, 0.0f, 0.0f, 1.0f);
	planes[2].p = vec4f_create(-1.0f, 0.0f, 0.0f, 1.0f);

	// right (x = w)
	planes[3].n = vec4f_create(-1.0f, 0.0f, 0.0f, 1.0f);
	planes[3].p = vec4f_create(1.0f, 0.0f, 0.0f, 1.0f);

	// near (z = 0)
	planes[4].n = vec4f_create(0.0f, 0.0f, 1.0f, 0.0f);
	planes[4].p = VEC4F_0;

	// far (z = w)
	planes[5].n = vec4f_create(0.0f, 0.0f, -1.0f, 1.0f);
	planes[5].p = vec4f_create(0.0f, 0.0f, 1.0f, 1.0f);
}

static void clip_edge_against_plane(VSout* s, VSout* e, Plane4 P, VSout** clip_out, int* out_n){

	const bool sIn = plane4_inside(P,s->pos);
	const bool eIn = plane4_inside(P,e->pos);
	const bool intersects_plane = (sIn != eIn);

	if(intersects_plane) {
		const float t = plane4_compute_intersect_t(P,s->pos,e->pos);
		lerp_vertex(s,e,s,t); //write intersection to s
		clip_out[*out_n++] = s;
		if(eIn) clip_out[*out_n++] = e;
		return;
	} 

	if(sIn && eIn) clip_out[*out_n++] = e;

}

void clip_poly_against_plane(VSout** clip_in, int in_n, Plane4 P, VSout** clip_out, int* out_n){

	for(int v = 0; v < in_n; v++) {
		VSout* s = clip_in[v];
		VSout* e = clip_in[(v+1)%in_n];
		clip_edge_against_plane(s, e, P, clip_out, out_n);
	}

}

static void swap_ptrs(VSout** ptr_1, VSout** ptr_2){
	VSout** temp;	
	temp = ptr_1;
	ptr_1 = ptr_2;
	ptr_2 = temp;
}

static int clip_poly_against_planes(const Plane4* planes, int planes_n, VSout** clip_in, int in_n, VSout** clip_out) {

	VSout** A = clip_in;
	VSout** B = clip_out;
	
	int out_n = 0;
	for(size_t i = 0; i < planes_n; i++){
		clip_poly_against_plane(A, in_n, planes[i], B, &out_n);
		swap_ptrs(A,B);
		in_n = out_n;
	}

	clip_out = A; // Because I know there are an even number of planes
	return out_n;
}

static int construct_triangles(const int out_n, Triangle* tris_out, VSout** clip_out) {
	// triangle construction through 'fanning' out the polygon
	int num_tris = out_n - 2; 
	for(int k = 0; k < num_tris; k++) {
		tris_out[k].v[0] = clip_out[0];
		tris_out[k].v[1] = clip_out[k];
		tris_out[k].v[2] = clip_out[k+1];	
	}
}

static inline void prepare_clip_input(const Triangle* tri, VSout** in) {
	for(int i = 0; i < 3; i++) in[i] = tri->v[i];
}

// assuming verts represents a convex polygon that is in clockwise order
int clip_tri(const Triangle* tri, Triangle* tris_out){

	if(!tri || !tris_out) return 0;
	if(!tri->v || !tris_out->v) return 0;

	int in_n = 3;

	VSout* clip_in[9];
	VSout* clip_out[9];

	prepare_clip_input(tri, clip_in);

	int planes_n = 6;
	Plane4 planes[planes_n];
	prepare_clipping_planes(planes);

	int out_n = clip_poly_against_planes(planes, planes_n, clip_in, in_n, clip_out);
	int num_tris = construct_triangles(out_n, tris_out, clip_out);

	return num_tris;
}


