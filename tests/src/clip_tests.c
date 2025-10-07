#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "clip.h"

void test_lerp(){
	printf("test_lerp\n");
	struct Vec4f v = vec4f_create(0.0,0.0,0.0,0.0);
	struct Vec4f u = vec4f_create(1.0,1.0,1.0, 0.0);
	struct Vec4f r = lerp(u,v,0.5f);
	struct Vec4f e = vec4f_create(0.5f,0.5f,0.5f, 0.0);

	assert(vec4f_are_equal(e,r));

	printf("success\n");
}

void test_sdf(){
	printf("test_sdf\n");
	struct Plane P;
	
	// x-z plane translated 1 unit in y direction
	// with normal facing the origin (inside is towards origin)
	P.n = vec4f_create(0.0,-1.0,0.0, 0.0);
	P.p = vec4f_create(0.0,1.0,0.0, 0.0);

	struct Vec4f v0 = vec4f_create(0.0, 2.0, 0.0, 0.0);
	struct Vec4f v1 = vec4f_create(0.0, 0.0, 0.0, 0.0);
	struct Vec4f v2 = vec4f_create(0.0,1.0,0.0, 0.0);

	assert( sdf(P,v0) > 0.0f );
	assert( sdf(P,v1) < 0.0f );
	assert( sdf(P,v2) == 0.0f );
	printf("success\n");
}

void test_inside(){
	printf("test_inside\n");
	printf("test_case_1\n");
	struct Plane P;
	P.n = vec4f_create(0.0f, 0.0f, -1.0f, 0.0);
	P.p = vec4f_create(0.0f, 0.0f, 1.0f, 0.0);

	struct Vec4f v0 = VEC4F_0;
	struct Vec4f v1 = vec4f_create(0.0, 2.0, 5.0, 0.0);	
	float eps = 0.01;
	assert(true == inside(P, v0, eps));
	assert(false == inside(P, v1, eps));

	printf("test_case_2\n");

	struct Vec4f v2 = vec4f_create(2.0f, 0.0f, 1.0f,0.0f);
	assert(true == inside(P, v2, eps));

	printf("success\n");
}

void test_intersect(){
	printf("test_intersect\n");

	printf("test case 1\n");
	struct Plane P;
	P.n = vec4f_create(-1.0f, 0.0f, 0.0f, 0.0f);
	P.p = vec4f_create(1.0f, 0.0f, 0.0f, 0.0f);

	struct Vec4f start = VEC4F_0;
	struct Vec4f end = vec4f_create(3.0f, 3.0f, 3.0f, 0.0f);

	struct Vec4f e = vec4f_create(1.0f, 1.0f, 1.0f,0.0f);
	struct Vec4f r = intersect(P, start, end);

	assert(vec4f_are_equal(e,r));

	printf("test case 2\n");
	struct Plane P1;
	P1.n = vec4f_create(-1.0f, -1.0f, -1.0f, 0.0f);
	P1.p = vec4f_create(3.0f,3.0f,3.0f, 0.0f);

	struct Vec4f start1 = VEC4F_0;
	struct Vec4f end1 = vec4f_create(3.0f, 3.0f, 3.0f,0.0f);

	struct Vec4f e1 = vec4f_create(3.0f, 3.0f, 3.0f, 0.0f);
	struct Vec4f r1 = intersect(P1, start, end);

	assert(vec4f_are_equal(e1,r1));


	printf("success\n");
}

void test_clip_against_plane_1(){

	printf("test_case_1\n");
	struct Plane P;
	P.n = vec4f_create(-1.0f, 0.0f, 0.0f, 0.0f);	
	P.p = vec4f_create(1.0f, 0.0f, 0.0f, 0.0f);

	struct Vec4f in1[9] = {0};
	struct Vec4f out1[9] = {0}; // added more space just in case
	in1[0] = vec4f_create(0.0f,2.0f,0.0f, 0.0f);
	in1[1] = vec4f_create(2.0f, 0.0f, 0.0f, 0.0f);
	in1[2] = vec4f_create(0.0f,-2.0f,0.0f, 0.0f);

	struct Vec4f expected[4] = {0};
	int expected_num_verts = 4;

	expected[0] = vec4f_create(1.0f,1.0f,0.0f, 0.0f);
	expected[1] = vec4f_create(1.0f,-1.0f,0.0f, 0.0f);
	expected[2] = vec4f_create(0.0f, -2.0f, 0.0f, 0.0f);
	expected[3] = vec4f_create(0.0f, 2.0f, 0.0f, 0.0f);

	int result = clip_against_plane(in1,3,P, out1);

	assert(result == expected_num_verts);

	for(int i = 0; i < expected_num_verts; i++){
		assert(vec4f_are_about_equal(expected[i], out1[i], 0.01f));
	}
}

void test_clip_against_plane_2(){
	printf("test_case_2\n");

	struct Plane P;
	P.n = vec4f_create(-1.0f, 0.0f, 0.0f, 0.0f);	
	P.p = vec4f_create(1.0f, 0.0f, 0.0f, 0.0f);

	struct Vec4f in2[9] = {0};
	struct Vec4f out2[9] = {0};
	
	in2[0] = vec4f_create(0.0f, 0.0f, 0.0f, 0.0f);
	in2[1] = vec4f_create(2.0f,2.0f,0.0f, 0.0f);
	in2[2] = vec4f_create(2.0f,-2.0f,0.0f, 0.0f);

	struct Vec4f expected2[3] = {0};
	int expected_num_verts2 = 3;

	expected2[0] = vec4f_create(1.0f,1.0f,0.0f, 0.0f);
	expected2[1] = vec4f_create(1.0f,-1.0f,0.0f, 0.0f);
	expected2[2] = vec4f_create(0.0f, 0.0f, 0.0f, 0.0f);

	int result2 = clip_against_plane(in2,3,P,out2);

	assert(result2 == expected_num_verts2);

	for(int i = 0; i < expected_num_verts2; i++){
		assert(vec4f_are_about_equal(expected2[i], out2[i], 0.01f));
	}


}

void test_clip_against_plane_3(){
	printf("test_case_3\n");

	struct Plane P;
	P.n = vec4f_create(0.0f, 0.0f, -1.0f, 0.0f);	
	P.p = vec4f_create(0.0f, 0.0f, 1.0f, 0.0f);

	struct Vec4f in3[9] = {0};
	struct Vec4f out3[9] = {0};
	
	in3[0] = vec4f_create(1.0f, 0.0f, 0.0f, 0.0f);
	in3[1] = vec4f_create(2.0f,0.0f,2.0f, 0.0f);
	in3[2] = vec4f_create(0.0f,0.0f,1.0f, 0.0f);

	struct Vec4f expected3[3] = {0};
	int expected_num_verts3 = 3;
		
	expected3[0] = vec4f_create(1.5f, 0.0f, 1.0f, 0.0f);
	expected3[1] = vec4f_create(0.0f,0.0f,1.0f, 0.0f);
	expected3[2] = vec4f_create(1.0f,0.0f,0.0f, 0.0f);

	int result3 = clip_against_plane(in3,3,P,out3);

	assert(result3 == expected_num_verts3);

	for(int i = 0; i < result3; i++){
		assert(vec4f_are_about_equal(expected3[i], out3[i], 0.01f));
	}
}

void test_clip_against_plane_4(){
	printf("test_case_4\n");
	
	struct Plane P;
	P.n = vec4f_create(0.0f, 0.0f, 1.0f, 0.0f);
	P.p = VEC4F_0;

	// Triangle in homogeneous clip space (w = 1 for all)
 	// A is behind near plane (z < 0); B and C are inside (z >= 0)
	
	struct Vec4f in[3] = {
		vec4f_create( 0.0f,  0.0f, -0.5f, 1.0f),  // A (outside)
		vec4f_create( 0.5f,  0.0f,  0.5f, 1.0f),  // B (inside)
		vec4f_create(-0.5f,  0.2f,  0.2f, 1.0f)   // C (inside)
	};

	struct Vec4f out[9] = {0};

	// clip_against_plane signature: (in, in_count, plane, out, &out_count)
	int out_count = clip_against_plane(in, 3, P, out);

	// Expected: 4 verts in this order (Sutherland–Hodgman result)
	// Edge A->B:  I1 then B
	// Edge B->C:  C
	// Edge C->A:  I2
	const int expected_count = 4;

	// Intersections on z=0 (linear in homogeneous coords)
	// A(-0.5) -> B(0.5): t = -zA/(zB - zA) = 0.5
	struct Vec4f I1 = vec4f_create( 0.25f,  0.0f, 0.0f, 1.0f);

	// A(-0.5) -> C(0.2): t = -zA/(zC - zA) = 0.5 / 0.7 ≈ 0.7142857
	struct Vec4f I2 = vec4f_create(-0.35714287f, 0.14285715f, 0.0f, 1.0f);

	struct Vec4f expected[4] = {
		I1,
		vec4f_create( 0.5f,  0.0f, 0.5f, 1.0f),   // B
		vec4f_create(-0.5f,  0.2f, 0.2f, 1.0f),   // C
		I2
	};

	// --- Asserts ---
	#define EPS 1e-5f
	assert(out_count == expected_count);
	for (int i = 0; i < expected_count; ++i) {
		assert(fabsf(out[i].x - expected[i].x) < EPS);
		assert(fabsf(out[i].y - expected[i].y) < EPS);
		assert(fabsf(out[i].z - expected[i].z) < EPS);
		assert(fabsf(out[i].w - expected[i].w) < EPS);
	}
}


void test_clip_against_plane(){
	printf("test_clip_against_plane\n");
	test_clip_against_plane_1();
	test_clip_against_plane_2();
	test_clip_against_plane_3();
	test_clip_against_plane_4();
	printf("success\n");
}

void test_clip_case_1(){
	// edge
	printf("test case 1\n");

	struct Plane P1;
	P1.n = vec4f_create(0.0f, 0.0f, -1.0f, 0.0f);
	P1.p = vec4f_create(0.0f, 0.0f, 1.0f, 0.0f);

	struct Plane P2;
	P2.n = vec4f_create(-1.0f, 0.0f, 0.0f, 0.0f);
	P2.p = vec4f_create(1.0f, 0.0f, 0.0f, 0.0f);

	struct Triangle tri = {0};
	tri.v0 = vec4f_create(1.0f, 0.0f, 0.0f, 0.0f);
	tri.v1 = vec4f_create(2.0f, 0.0f, 2.0f, 0.0f);
	tri.v2 = vec4f_create(0.0f, 0.0f, 1.0f, 0.0f);
	
	int num_planes = 2;
	struct Plane planes[2];
	planes[0] = P1;
	planes[1] = P2;

	struct ClipResult r = clip_tri(tri, planes, num_planes);

	assert(r.num_tris == 1);

	float eps = 0.01f;

	struct Vec4f v0_e = vec4f_create(1.0f, 0.0f, 1.0f, 0.0f);
	struct Vec4f v1_e = vec4f_create(0.0f, 0.0f, 1.0f, 0.0f);
	struct Vec4f v2_e = vec4f_create(1.0f, 0.0f, 0.0f, 0.0f);

	assert(vec4f_are_about_equal(r.tris[0].v0, v0_e, eps));
	assert(vec4f_are_about_equal(r.tris[0].v1, v1_e, eps));
	assert(vec4f_are_about_equal(r.tris[0].v2, v2_e, eps));
}

void test_clip_case_2(){
	// positive
	printf("test case 2\n");

	struct Plane P1;
	P1.n = vec4f_create(0.0f, 0.0f, -1.0f, 0.0f);
	P1.p = vec4f_create(0.0f, 0.0f, 3.0f, 0.0f);

	struct Triangle tri = {0};
	tri.v0 = vec4f_create(0.0f, 0.0f, 0.0f, 1.0f);
	tri.v1 = vec4f_create(1.0f, 0.0f, 1.0f, 1.0f);
	tri.v2 = vec4f_create(0.0f, 0.0f, 1.0f, 1.0f);
	
	int num_planes = 1;
	struct Plane planes[1];
	planes[0] = P1;

	struct ClipResult r = clip_tri(tri, planes, num_planes);

	assert(r.num_tris == 1);


	struct Vec4f v0_e = vec4f_create(1.0f, 0.0f, 1.0f, 1.0f);
	struct Vec4f v1_e = vec4f_create(0.0f, 0.0f, 1.0f, 1.0f);
	struct Vec4f v2_e = vec4f_create(0.0f, 0.0f, 0.0f, 1.0f);

	float eps = 0.01f;
	assert(vec4f_are_about_equal(r.tris[0].v0, v0_e, eps));
	assert(vec4f_are_about_equal(r.tris[0].v1, v1_e, eps));
	assert(vec4f_are_about_equal(r.tris[0].v2, v2_e, eps));
}

static inline void calculate_planes(struct Plane* planes){
	/* all normals facing 'inside'*/
	/* inside => -w<=x<=w, -w<=y<=w, 0<=z<=w*/
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

void test_clip_case_3(){
	// positive
	printf("test case 3\n");

	// assuming post projection matrix
	struct Plane planes[6] = {0};
	calculate_planes(planes);
	
}


void test_clip(){
	printf("test_clip\n");
	test_clip_case_1();
	test_clip_case_2();
	printf("success\n");
}

