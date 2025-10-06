#include <stdio.h>
#include <assert.h>
#include <stdbool.h>

#include "clip.h"

void test_lerp(){
	printf("test_lerp\n");
	struct Vec3f v = vec3f_create(0.0,0.0,0.0);
	struct Vec3f u = vec3f_create(1.0,1.0,1.0);
	struct Vec3f r = lerp(u,v,0.5f);
	struct Vec3f e = vec3f_create(0.5f,0.5f,0.5f);

	assert(vec3f_are_equal(e,r));

	printf("success\n");
}

void test_sdf(){
	printf("test_sdf\n");
	struct Plane P;
	
	// x-z plane translated 1 unit in y direction
	// with normal facing the origin (inside is towards origin)
	P.n = vec3f_create(0.0,-1.0,0.0);
	P.p = vec3f_create(0.0,1.0,0.0);

	struct Vec3f v0 = vec3f_create(0.0, 2.0, 0.0);
	struct Vec3f v1 = vec3f_create(0.0, 0.0, 0.0);
	struct Vec3f v2 = vec3f_create(0.0,1.0,0.0);

	assert( sdf(P,v0) > 0.0f );
	assert( sdf(P,v1) < 0.0f );
	assert( sdf(P,v2) == 0.0f );
	printf("success\n");
}

void test_inside(){
	printf("test_inside\n");
	printf("test_case_1\n");
	struct Plane P;
	P.n = vec3f_create(0.0f, 0.0f, -1.0f);
	P.p = vec3f_create(0.0f, 0.0f, 1.0f);

	struct Vec3f v0 = VEC3F_0;
	struct Vec3f v1 = vec3f_create(0.0, 2.0, 5.0);	
	float eps = 0.01;
	assert(true == inside(P, v0, eps));
	assert(false == inside(P, v1, eps));

	printf("test_case_2\n");

	struct Vec3f v2 = vec3f_create(2.0f, 0.0f, 1.0f);
	assert(true == inside(P, v2, eps));

	printf("success\n");
}

void test_intersect(){
	printf("test_intersect\n");

	printf("test case 1\n");
	struct Plane P;
	P.n = vec3f_create(-1.0f, 0.0f, 0.0f);
	P.p = vec3f_create(1.0f, 0.0f, 0.0f);

	struct Vec3f start = VEC3F_0;
	struct Vec3f end = vec3f_create(3.0f, 3.0f, 3.0f);

	struct Vec3f e = vec3f_create(1.0f, 1.0f, 1.0f);
	struct Vec3f r = intersect(P, start, end);

	assert(vec3f_are_equal(e,r));

	printf("test case 2\n");
	struct Plane P1;
	P1.n = vec3f_create(-1.0f, -1.0f, -1.0f);
	P1.p = vec3f_create(3.0f,3.0f,3.0f);

	struct Vec3f start1 = VEC3F_0;
	struct Vec3f end1 = vec3f_create(3.0f, 3.0f, 3.0f);

	struct Vec3f e1 = vec3f_create(3.0f, 3.0f, 3.0f);
	struct Vec3f r1 = intersect(P1, start, end);

	assert(vec3f_are_equal(e1,r1));


	printf("success\n");
}

void test_clip_against_plane(){
	printf("test_clip_against_plane\n");

	printf("test_case_1\n");
	struct Plane P;
	P.n = vec3f_create(-1.0f, 0.0f, 0.0f);	
	P.p = vec3f_create(1.0f, 0.0f, 0.0f);

	struct Vec3f in1[9] = {0};
	struct Vec3f out1[9] = {0}; // added more space just in case
	in1[0] = vec3f_create(0.0f,2.0f,0.0f);
	in1[1] = vec3f_create(2.0f, 0.0f, 0.0f);
	in1[2] = vec3f_create(0.0f,-2.0f,0.0f);

	struct Vec3f expected[4] = {0};
	int expected_num_verts = 4;

	expected[0] = vec3f_create(1.0f,1.0f,0.0f);
	expected[1] = vec3f_create(1.0f,-1.0f,0.0f);
	expected[2] = vec3f_create(0.0f, -2.0f, 0.0f);
	expected[3] = vec3f_create(0.0f, 2.0f, 0.0f);

	int result = clip_against_plane(in1,3,P, out1);

	assert(result = expected_num_verts);

	for(int i = 0; i < expected_num_verts; i++){
		assert(vec3f_are_equal(expected[i], out1[i]));
	}

	printf("test_case_2\n");

	// uses same P as test case 1
	struct Vec3f in2[9] = {0};
	struct Vec3f out2[9] = {0};
	
	in2[0] = vec3f_create(0.0f, 0.0f, 0.0f);
	in2[1] = vec3f_create(2.0f,2.0f,0.0f);
	in2[2] = vec3f_create(2.0f,-2.0f,0.0f);

	struct Vec3f expected2[3] = {0};
	int expected_num_verts2 = 3;

	expected2[0] = vec3f_create(1.0f,1.0f,0.0f);
	expected2[1] = vec3f_create(1.0f,-1.0f,0.0f);
	expected2[2] = vec3f_create(0.0f, 0.0f, 0.0f);

	int result2 = clip_against_plane(in2,3,P,out2);

	assert(result2 = expected_num_verts2);

	for(int i = 0; i < expected_num_verts2; i++){
		assert(vec3f_are_equal(expected2[i], out2[i]));
	}

	printf("test_case_3\n");

	// uses same P as test case 1
	struct Vec3f in3[9] = {0};
	struct Vec3f out3[9] = {0};
	
	in3[0] = vec3f_create(1.0f, 0.0f, 0.0f);
	in3[1] = vec3f_create(2.0f,0.0f,2.0f);
	in3[2] = vec3f_create(0.0f,0.0f,1.0f);

	struct Vec3f expected3[3] = {0};
	int expected_num_verts3 = 3;
		
	expected3[0] = vec3f_create(1.0f, 0.0f, 0.0f);
	expected3[1] = vec3f_create(1.0f,0.0f,1.5f);
	expected3[2] = vec3f_create(0.0f,0.0f,1.0f);

	int result3 = clip_against_plane(in3,3,P,out3);

	assert(result3 = expected_num_verts3);

	printf("success\n");
}


void test_clip_case_1(){
	// edge
	printf("test case 1\n");
	struct Plane P2;
	P2.n = vec3f_create(-1.0f, 0.0f, 0.0f);
	P2.p = vec3f_create(1.0f, 0.0f, 0.0f);

	struct Plane P1;
	P1.n = vec3f_create(0.0f, 0.0f, -1.0f);
	P1.p = vec3f_create(0.0f, 0.0f, 1.0f);

	struct Triangle tri = {0};
	tri.v0 = vec4f_create(1.0f, 0.0f, 0.0f, 1.0f);
	tri.v1 = vec4f_create(2.0f, 0.0f, 2.0f, 1.0f);
	tri.v2 = vec4f_create(0.0f, 0.0f, 1.0f, 1.0f);
	
	int num_planes = 2;
	struct Plane planes[2];
	planes[0] = P1;
	planes[1] = P2;

	struct ClipResult r = clip_tri(tri, planes, num_planes);

	assert(r.num_tris == 1);

	float eps = 0.01f;

	struct Vec4f v0_e = vec4f_create(1.0f, 0.0f, 1.0f, 1.0f);
	struct Vec4f v1_e = vec4f_create(0.0f, 0.0f, 1.0f, 1.0f);
	struct Vec4f v2_e = vec4f_create(1.0f, 0.0f, 0.0f, 1.0f);

	assert(vec4f_are_about_equal(r.tris[0].v0, v0_e, eps));
	assert(vec4f_are_about_equal(r.tris[0].v1, v1_e, eps));
	assert(vec4f_are_about_equal(r.tris[0].v2, v2_e, eps));
}

void test_clip_case_2(){
	// positive
	printf("test case 2\n");

	struct Plane P1;
	P1.n = vec3f_create(0.0f, 0.0f, -1.0f);
	P1.p = vec3f_create(0.0f, 0.0f, 3.0f);

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

void test_clip(){
	printf("test_clip\n");
	test_clip_case_1();
	test_clip_case_2();
	printf("success\n");
}

