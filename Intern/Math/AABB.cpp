
#include <glpch.h>
#define _SWAP
#include "AABB.h"

float AABB::GetArea() const
{
	return size.x * size.y * size.z;
}

bool AABB::operator==(const AABB& p_rval) const {
	return ((position == p_rval.position) && (size == p_rval.size));
}

bool AABB::operator!=(const AABB& p_rval) const {
	return ((position != p_rval.position) || (size != p_rval.size));
}

void AABB::MergeWith(const AABB& p_aabb) {
	Vector3 beg_1, beg_2;
	Vector3 end_1, end_2;
	Vector3 min, max;

	beg_1 = position;
	beg_2 = p_aabb.position;
	end_1 = Vector3(size.x, size.y, size.z) + beg_1;
	end_2 = Vector3(p_aabb.size.x, p_aabb.size.y, p_aabb.size.z) + beg_2;

	min.x = (beg_1.x < beg_2.x) ? beg_1.x : beg_2.x;
	min.y = (beg_1.y < beg_2.y) ? beg_1.y : beg_2.y;
	min.z = (beg_1.z < beg_2.z) ? beg_1.z : beg_2.z;

	max.x = (end_1.x > end_2.x) ? end_1.x : end_2.x;
	max.y = (end_1.y > end_2.y) ? end_1.y : end_2.y;
	max.z = (end_1.z > end_2.z) ? end_1.z : end_2.z;

	position = min;
	size = max - min;
}


bool AABB::IsEqualApprox(const AABB& p_aabb) const {
	return Vector3::Abs(position) == Vector3::Abs(p_aabb.position) && Vector3::Abs(size) == Vector3::Abs(p_aabb.size);
}


AABB AABB::Intersection(const AABB& p_aabb) const 
{
	Vector3 srcMin = position;
	Vector3 srcMax = position + size;
	Vector3 dstMin = p_aabb.position;
	Vector3 dstMax = p_aabb.position + p_aabb.size;

	Vector3 min, max;

	if (srcMin.x > dstMax.x || srcMax.x < dstMin.x) {
		return AABB();
	}
	else {
		min.x = (srcMin.x > dstMin.x) ? srcMin.x : dstMin.x;
		max.x = (srcMax.x < dstMax.x) ? srcMax.x : dstMax.x;
	}

	if (srcMin.y > dstMax.y || srcMax.y < dstMin.y) {
		return AABB();
	}
	else {
		min.y = (srcMin.y > dstMin.y) ? srcMin.y : dstMin.y;
		max.y = (srcMax.y < dstMax.y) ? srcMax.y : dstMax.y;
	}

	if (srcMin.z > dstMax.z || srcMax.z < dstMin.z) {
		return AABB();
	}
	else {
		min.z = (srcMin.z > dstMin.z) ? srcMin.z : dstMin.z;
		max.z = (srcMax.z < dstMax.z) ? srcMax.z : dstMax.z;
	}

	return AABB(min, max - min);
}
bool AABB::IntersectsRay(const Vector3& p_from, const Vector3& p_dir, Vector3* r_clip, Vector3* r_normal) const {
	Vector3 c1, c2;
	Vector3 end = position + size;
	float near = -1e20;
	float far = 1e20;
	int axis = 0;

	for (int i = 0; i < 3; i++) {
		if (p_dir[i] == 0) {
			if ((p_from[i] < position[i]) || (p_from[i] > end[i])) {
				return false;
			}
		}
		else { // ray not parallel to planes in this direction
			c1[i] = (position[i] - p_from[i]) / p_dir[i];
			c2[i] = (end[i] - p_from[i]) / p_dir[i];

			if (c1[i] > c2[i]) {
				SWAP(c1, c2);
			}
			if (c1[i] > near) {
				near = c1[i];
				axis = i;
			}
			if (c2[i] < far) {
				far = c2[i];
			}
			if ((near > far) || (far < 0)) {
				return false;
			}
		}
	}

	if (r_clip) {
		*r_clip = c1;
	}
	if (r_normal) {
		*r_normal = Vector3();
		(*r_normal)[axis] = p_dir[axis] ? -1 : 1;
	}

	return true;
}

bool AABB::IntersectsSegment(const Vector3& p_from, const Vector3& p_to, Vector3* r_clip, Vector3* r_normal) const 
{
	float min = 0, max = 1;
	int axis = 0;
	float sign = 0;

	for (int i = 0; i < 3; i++) {
		float seg_from = p_from[i];
		float seg_to = p_to[i];
		float box_begin = position[i];
		float box_end = box_begin + size[i];
		float cmin, cmax;
		float csign;

		if (seg_from < seg_to) {
			if (seg_from > box_end || seg_to < box_begin) {
				return false;
			}
			float length = seg_to - seg_from;
			cmin = (seg_from < box_begin) ? ((box_begin - seg_from) / length) : 0;
			cmax = (seg_to > box_end) ? ((box_end - seg_from) / length) : 1;
			csign = -1.0;

		}
		else {
			if (seg_to > box_end || seg_from < box_begin) {
				return false;
			}
			float length = seg_to - seg_from;
			cmin = (seg_from > box_end) ? (box_end - seg_from) / length : 0;
			cmax = (seg_to < box_begin) ? (box_begin - seg_from) / length : 1;
			csign = 1.0;
		}

		if (cmin > min) {
			min = cmin;
			axis = i;
			sign = csign;
		}
		if (cmax < max) {
			max = cmax;
		}
		if (max < min) {
			return false;
		}
	}

	Vector3 rel = p_to - p_from;

	if (r_normal) {
		Vector3 normal;
		normal[axis] = sign;
		*r_normal = normal;
	}

	if (r_clip) {
		*r_clip = p_from + rel * min;
	}

	return true;
}

bool AABB::IntersectsPlane(const Plane& p_plane) const 
{
	Vector3 points[8] = {
		Vector3(position.x, position.y, position.z),
		Vector3(position.x, position.y, position.z + size.z),
		Vector3(position.x, position.y + size.y, position.z),
		Vector3(position.x, position.y + size.y, position.z + size.z),
		Vector3(position.x + size.x, position.y, position.z),
		Vector3(position.x + size.x, position.y, position.z + size.z),
		Vector3(position.x + size.x, position.y + size.y, position.z),
		Vector3(position.x + size.x, position.y + size.y, position.z + size.z),
	};

	bool over = false;
	bool under = false;

	for (int i = 0; i < 8; i++) {
		if (p_plane.DistanceTo(points[i]) > 0) {
			over = true;
		}
		else {
			under = true;
		}
	}

	return under && over;
}


Vector3 AABB::GetLongestAxis() const 
{
	Vector3 axis(1, 0, 0);
	float max_size = size.x;

	if (size.y > max_size) {
		axis = Vector3(0, 1, 0);
		max_size = size.y;
	}

	if (size.z > max_size) {
		axis = Vector3(0, 0, 1);
	}

	return axis;
}

int AABB::GetLongestAxisIndex() const 
{
	int axis = 0;
	float max_size = size.x;

	if (size.y > max_size) {
		axis = 1;
		max_size = size.y;
	}

	if (size.z > max_size) {
		axis = 2;
	}

	return axis;
}

Vector3 AABB::GetShortestAxis() const 
{
	Vector3 axis(1, 0, 0);
	float max_size = size.x;

	if (size.y < max_size) {
		axis = Vector3(0, 1, 0);
		max_size = size.y;
	}

	if (size.z < max_size) {
		axis = Vector3(0, 0, 1);
	}

	return axis;
}

int AABB::GetShortestAxisIndex() const 
{
	int axis = 0;
	float max_size = size.x;

	if (size.y < max_size) {
		axis = 1;
		max_size = size.y;
	}

	if (size.z < max_size) {
		axis = 2;
	}

	return axis;
}


AABB AABB::Merge(const AABB& p_with) const 
{
	AABB aabb = *this;
	aabb.MergeWith(p_with);
	return aabb;  
}

AABB AABB::Expand(const Vector3& p_vector) const 
{
	AABB aabb = *this;
	aabb.ExpandTo(p_vector);
	return aabb;
}

AABB AABB::Grow(float p_by) const 
{
	AABB aabb = *this;
	aabb.GrowBy(p_by);
	return aabb;
}

void AABB::GetEdge(int p_edge, Vector3& r_from, Vector3& r_to) const {
	switch (p_edge) {
	case 0: {
		r_from = Vector3(position.x + size.x, position.y, position.z);
		r_to = Vector3(position.x, position.y, position.z);
	} break;
	case 1: {
		r_from = Vector3(position.x + size.x, position.y, position.z + size.z);
		r_to = Vector3(position.x + size.x, position.y, position.z);
	} break;
	case 2: {
		r_from = Vector3(position.x, position.y, position.z + size.z);
		r_to = Vector3(position.x + size.x, position.y, position.z + size.z);

	} break;
	case 3: {
		r_from = Vector3(position.x, position.y, position.z);
		r_to = Vector3(position.x, position.y, position.z + size.z);

	} break;
	case 4: {
		r_from = Vector3(position.x, position.y + size.y, position.z);
		r_to = Vector3(position.x + size.x, position.y + size.y, position.z);
	} break;
	case 5: {
		r_from = Vector3(position.x + size.x, position.y + size.y, position.z);
		r_to = Vector3(position.x + size.x, position.y + size.y, position.z + size.z);
	} break;
	case 6: {
		r_from = Vector3(position.x + size.x, position.y + size.y, position.z + size.z);
		r_to = Vector3(position.x, position.y + size.y, position.z + size.z);

	} break;
	case 7: {
		r_from = Vector3(position.x, position.y + size.y, position.z + size.z);
		r_to = Vector3(position.x, position.y + size.y, position.z);

	} break;
	case 8: {
		r_from = Vector3(position.x, position.y, position.z + size.z);
		r_to = Vector3(position.x, position.y + size.y, position.z + size.z);

	} break;
	case 9: {
		r_from = Vector3(position.x, position.y, position.z);
		r_to = Vector3(position.x, position.y + size.y, position.z);

	} break;
	case 10: {
		r_from = Vector3(position.x + size.x, position.y, position.z);
		r_to = Vector3(position.x + size.x, position.y + size.y, position.z);

	} break;
	case 11: {
		r_from = Vector3(position.x + size.x, position.y, position.z + size.z);
		r_to = Vector3(position.x + size.x, position.y + size.y, position.z + size.z);

	} break;
	}
}
