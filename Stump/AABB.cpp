#include "AABB.h"
#include <assert.h>
num_fd AABB::GetVolume() const
{
	return size.x * size.y * size.z;
}

bool AABB::HasZeroVolume() const
{
    return (size.x <= 0 || size.y <= 0 || size.z <= 0);
}

bool AABB::HasNoSurface() const
{
    return (size.x <= 0 && size.y <= 0 && size.z <= 0);
}

bool AABB::IsEqualApprox(const AABB& p_aabb) const
{
    return position.IsEqualApprox(p_aabb.position) && size.IsEqualApprox(p_aabb.size);
}

bool AABB::IsIntersects(const AABB& p_aabb) const
{
    if (position.x >= (p_aabb.position.x + p_aabb.size.x)) {
        return false;
    }
    if ((position.x + size.x) <= p_aabb.position.x) {
        return false;
    }
    if (position.y >= (p_aabb.position.y + p_aabb.size.y)) {
        return false;
    }
    if ((position.y + size.y) <= p_aabb.position.y) {
        return false;
    }
    if (position.z >= (p_aabb.position.z + p_aabb.size.z)) {
        return false;
    }
    if ((position.z + size.z) <= p_aabb.position.z) {
        return false;
    }

    return true;
}

bool AABB::IsIntersectsInclusive(const AABB& p_aabb) const
{
    if (position.x > (p_aabb.position.x + p_aabb.size.x)) {
        return false;
    }
    if ((position.x + size.x) < p_aabb.position.x) {
        return false;
    }
    if (position.y > (p_aabb.position.y + p_aabb.size.y)) {
        return false;
    }
    if ((position.y + size.y) < p_aabb.position.y) {
        return false;
    }
    if (position.z > (p_aabb.position.z + p_aabb.size.z)) {
        return false;
    }
    if ((position.z + size.z) < p_aabb.position.z) {
        return false;
    }

    return true;
}

bool AABB::Encloses(const AABB& p_aabb) const
{
    Vector3 src_min = position;
    Vector3 src_max = position + size;
    Vector3 dst_min = p_aabb.position;
    Vector3 dst_max = p_aabb.position + p_aabb.size;

    return (
        (src_min.x <= dst_min.x) &&
        (src_max.x > dst_max.x) &&
        (src_min.y <= dst_min.y) &&
        (src_max.y > dst_max.y) &&
        (src_min.z <= dst_min.z) &&
        (src_max.z > dst_max.z));
}

AABB AABB::Merge(const AABB& p_aabb)
{
    AABB aabb = *this;
    aabb.MergeWith(p_aabb);
    return aabb;
}

void AABB::MergeWith(const AABB& p_aabb)
{
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

AABB AABB::Intersection(const AABB& p_aabb) const
{
    Vector3 src_min = position;
    Vector3 src_max = position + size;
    Vector3 dst_min = p_aabb.position;
    Vector3 dst_max = p_aabb.position + p_aabb.size;

    Vector3 min, max;

    if (src_min.x > dst_max.x || src_max.x < dst_min.x) {
        return AABB();
    }
    else {
        min.x = (src_min.x > dst_min.x) ? src_min.x : dst_min.x;
        max.x = (src_max.x < dst_max.x) ? src_max.x : dst_max.x;
    }

    if (src_min.y > dst_max.y || src_max.y < dst_min.y) {
        return AABB();
    }
    else {
        min.y = (src_min.y > dst_min.y) ? src_min.y : dst_min.y;
        max.y = (src_max.y < dst_max.y) ? src_max.y : dst_max.y;
    }

    if (src_min.z > dst_max.z || src_max.z < dst_min.z) {
        return AABB();
    }
    else {
        min.z = (src_min.z > dst_min.z) ? src_min.z : dst_min.z;
        max.z = (src_max.z < dst_max.z) ? src_max.z : dst_max.z;
    }

    return AABB(min, max - min);
}

bool AABB::IsIntersectsSegment(const Vector3& p_from, const Vector3& p_to, Vector3* r_intersectionPoint, Vector3* r_normal, num_fd* r_t) const
{
    num_fd min = 0, max = 1;
    int axis = 0;
    num_fd sign = 0;

    for (int i = 0; i < 3; i++) {
        num_fd seg_from = p_from[i];
        num_fd seg_to = p_to[i];
        num_fd box_begin = position[i];
        num_fd box_end = box_begin + size[i];
        num_fd cmin, cmax;
        num_fd csign;

        if (seg_from < seg_to) {
            if (seg_from > box_end || seg_to < box_begin) {
                return false;
            }
            num_fd length = seg_to - seg_from;
            cmin = (seg_from < box_begin) ? ((box_begin - seg_from) / length) : 0;
            cmax = (seg_to > box_end) ? ((box_end - seg_from) / length) : 1;
            csign = -1.0;

        }
        else {
            if (seg_to > box_end || seg_from < box_begin) {
                return false;
            }
            num_fd length = seg_to - seg_from;
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

    if (r_intersectionPoint) {
        *r_intersectionPoint = p_from + rel * min;
    }

    return true;
}

bool AABB::IsIntersectsRay(const Vector3& p_from, const Vector3& p_dir, Vector3* r_intersectionPoint, Vector3* r_normal, num_fd* r_t) const
{
    Vector3 c1, c2;
    Vector3 end = position + size;
    num_fd near = -1e20;
    num_fd far = 1e20;
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

    if (r_intersectionPoint) {
        *r_intersectionPoint = c1;
    }
    if (r_normal) {
        *r_normal = Vector3();
        (*r_normal)[axis] = p_dir[axis] ? -1 : 1;
    }

    return true;
}

bool AABB::IsSmitsIntersectRay(const Vector3& p_from, const Vector3& p_dir, num_fd r_tmin, num_fd r_tmax) const
{
    num_fd divx = 1 / p_dir.x;
    num_fd divy = 1 / p_dir.y;
    num_fd divz = 1 / p_dir.z;

    Vector3 upbound = position + size;
    num_fd tmin, tmax, tymin, tymax, tzmin, tzmax;
    if (p_dir.x >= 0) {
        tmin = (position.x - p_from.x) * divx;
        tmax = (upbound.x - p_from.x) * divx;
    }
    else {
        tmin = (upbound.x - p_from.x) * divx;
        tmax = (position.x - p_from.x) * divx;
    }
    if (p_dir.y >= 0) {
        tymin = (position.y - p_from.y) * divy;
        tymax = (upbound.y - p_from.y) * divy;
    }
    else {
        tymin = (upbound.y - p_from.y) * divy;
        tymax = (position.y - p_from.y) * divy;
    }
    if ((tmin > tymax) || (tymin > tmax)) {
        return false;
    }
    if (tymin > tmin) {
        tmin = tymin;
    }
    if (tymax < tmax) {
        tmax = tymax;
    }
    if (p_dir.z >= 0) {
        tzmin = (position.z - p_from.z) * divz;
        tzmax = (upbound.z - p_from.z) * divz;
    }
    else {
        tzmin = (upbound.z - p_from.z) * divz;
        tzmax = (position.z - p_from.z) * divz;
    }
    if ((tmin > tzmax) || (tzmin > tmax)) {
        return false;
    }
    if (tzmin > tmin) {
        tmin = tzmin;
    }
    if (tzmax < tmax) {
        tmax = tzmax;
    }
    return ((tmin < r_tmax) && (tmax > r_tmin));
}

bool AABB::IsIntersectsConvexShape(const Plane* p_planes, int p_planeCount, const Vector3* p_points, int p_pointCount) const
{
    Vector3 half_extents = size * 0.5f;
    Vector3 ofs = position + half_extents;

    for (int i = 0; i < p_planeCount; i++) {
        const Plane& p = p_planes[i];
        Vector3 point(
            (p.normal.x > 0) ? -half_extents.x : half_extents.x,
            (p.normal.y > 0) ? -half_extents.y : half_extents.y,
            (p.normal.z > 0) ? -half_extents.z : half_extents.z);
        point += ofs;
        if (p.IsPointOver(point)) {
            return false;
        }
    }

    // Make sure all points in the shape aren't fully separated from the AABB on
    // each axis.
    int bad_point_counts_positive[3] = { 0 };
    int bad_point_counts_negative[3] = { 0 };

    for (int k = 0; k < 3; k++) {
        for (int i = 0; i < p_pointCount; i++) {
            if (p_points[i].coord[k] > ofs.coord[k] + half_extents.coord[k]) {
                bad_point_counts_positive[k]++;
            }
            if (p_points[i].coord[k] < ofs.coord[k] - half_extents.coord[k]) {
                bad_point_counts_negative[k]++;
            }
        }

        if (bad_point_counts_negative[k] == p_pointCount) {
            return false;
        }
        if (bad_point_counts_positive[k] == p_pointCount) {
            return false;
        }
    }

    return true;
}

bool AABB::IsInsideConvexShape(const Plane* p_planes, int p_planeCount) const
{
    Vector3 half_extents = size * 0.5f;
    Vector3 ofs = position + half_extents;

    for (int i = 0; i < p_planeCount; i++) {
        const Plane& p = p_planes[i];
        Vector3 point(
            (p.normal.x < 0) ? -half_extents.x : half_extents.x,
            (p.normal.y < 0) ? -half_extents.y : half_extents.y,
            (p.normal.z < 0) ? -half_extents.z : half_extents.z);
        point += ofs;
        if (p.IsPointOver(point)) {
            return false;
        }
    }

    return true;
}

bool AABB::IsIntersectsPlane(const Plane& p_plane) const
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

bool AABB::HasPoint(const Vector3& p_point) const
{
    if (p_point.x < position.x) {
        return false;
    }
    if (p_point.y < position.y) {
        return false;
    }
    if (p_point.z < position.z) {
        return false;
    }
    if (p_point.x > position.x + size.x) {
        return false;
    }
    if (p_point.y > position.y + size.y) {
        return false;
    }
    if (p_point.z > position.z + size.z) {
        return false;
    }

    return true;
}

Vector3 AABB::GetSupport(const Vector3& p_normal) const
{
    Vector3 half_extents = size * 0.5f;
    Vector3 ofs = position + half_extents;

    return Vector3(
        (p_normal.x > 0) ? -half_extents.x : half_extents.x,
        (p_normal.y > 0) ? -half_extents.y : half_extents.y,
        (p_normal.z > 0) ? -half_extents.z : half_extents.z) +
        ofs;
}

Vector3 AABB::GetLongestAxis() const
{
    Vector3 axis(1, 0, 0);
    num_fd max_size = size.x;

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
    num_fd max_size = size.x;

    if (size.y > max_size) {
        axis = 1;
        max_size = size.y;
    }

    if (size.z > max_size) {
        axis = 2;
    }

    return axis;
}

num_fd AABB::GetLongestAxisSize() const
{
    num_fd max_size = size.x;

    if (size.y > max_size) {
        max_size = size.y;
    }

    if (size.z > max_size) {
        max_size = size.z;
    }

    return max_size;
}

Vector3 AABB::GetShortestAxis() const
{
    Vector3 axis(1, 0, 0);
    num_fd max_size = size.x;

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
    num_fd max_size = size.x;

    if (size.y < max_size) {
        axis = 1;
        max_size = size.y;
    }

    if (size.z < max_size) {
        axis = 2;
    }

    return axis;
}

num_fd AABB::GetShortestAxisSize() const
{
    num_fd max_size = size.x;

    if (size.y < max_size) {
        max_size = size.y;
    }

    if (size.z < max_size) {
        max_size = size.z;
    }

    return max_size;
}

AABB AABB::Grow(const num_fd p_by) const
{
    AABB aabb = *this;
    aabb.GrowBy(p_by);
    return aabb;
}

void AABB::GrowBy(const num_fd p_amount)
{
    position.x -= p_amount;
    position.y -= p_amount;
    position.z -= p_amount;
    size.x += 2 * p_amount;
    size.y += 2 * p_amount;
    size.z += 2 * p_amount;
}

void AABB::GetEdge(int p_index, Vector3& r_from, Vector3& r_to) const
{
	assert(p_index < 0 || p_index >= 12);
    switch (p_index) {
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

Vector3 AABB::GetEndpoint(int p_point) const
{
    switch (p_point) {
    case 0:
        return Vector3(position.x, position.y, position.z);
    case 1:
        return Vector3(position.x, position.y, position.z + size.z);
    case 2:
        return Vector3(position.x, position.y + size.y, position.z);
    case 3:
        return Vector3(position.x, position.y + size.y, position.z + size.z);
    case 4:
        return Vector3(position.x + size.x, position.y, position.z);
    case 5:
        return Vector3(position.x + size.x, position.y, position.z + size.z);
    case 6:
        return Vector3(position.x + size.x, position.y + size.y, position.z);
    case 7:
        return Vector3(position.x + size.x, position.y + size.y, position.z + size.z);
    };
}

AABB AABB::Expand(const Vector3& p_vector) const
{
    AABB aabb = *this;
    aabb.ExpandTo(p_vector);
    return aabb;
}

void AABB::ExpandTo(const Vector3& p_vector)
{
    Vector3 begin = position;
    Vector3 end = position + size;

    if (p_vector.x < begin.x) {
        begin.x = p_vector.x;
    }
    if (p_vector.y < begin.y) {
        begin.y = p_vector.y;
    }
    if (p_vector.z < begin.z) {
        begin.z = p_vector.z;
    }

    if (p_vector.x > end.x) {
        end.x = p_vector.x;
    }
    if (p_vector.y > end.y) {
        end.y = p_vector.y;
    }
    if (p_vector.z > end.z) {
        end.z = p_vector.z;
    }

    position = begin;
    size = end - begin;
}

void AABB::ProjectRangeInPlane(const Plane& p_plane, num_fd& r_min, num_fd& r_max) const
{
    Vector3 half_extents = size * 0.5f;
    Vector3 center(position.x + half_extents.x, position.y + half_extents.y, position.z + half_extents.z);

    num_fd length = p_plane.normal.Abs().Dot(half_extents);
    num_fd distance = p_plane.DistanceTo(center);
    r_min = distance - length;
    r_max = distance + length;
}

bool AABB::CreateFromPoints(const std::vector<Vector3>& p_points)
{
    if (!p_points.size()) {
        return false;
    }

    Vector3 begin = p_points[0];
    Vector3 end = begin;

    for (int n = 1; n < p_points.size(); n++) {
        const Vector3& pt = p_points[n];

        if (pt.x < begin.x) {
            begin.x = pt.x;
        }
        if (pt.y < begin.y) {
            begin.y = pt.y;
        }
        if (pt.z < begin.z) {
            begin.z = pt.z;
        }

        if (pt.x > end.x) {
            end.x = pt.x;
        }
        if (pt.y > end.y) {
            end.y = pt.y;
        }
        if (pt.z > end.z) {
            end.z = pt.z;
        }
    }

    position = begin;
    size = end - begin;

    return true;
}

bool AABB::operator==(const AABB& p_aabb) const
{
    return ((position == p_aabb.position) && (size == p_aabb.size));
}

bool AABB::operator!=(const AABB& p_aabb) const
{
    return ((position != p_aabb.position) || (size != p_aabb.size));
}
