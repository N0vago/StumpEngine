#ifndef ST_AABB_H
#define ST_AABB_H
#include "Math/Typedefs.h"
#include "Math/Plane.h"
#include "Math/Vector3.h"
#include <vector>

namespace Math {
	class AABB
	{
	public:
		Vector3 position;
		Vector3 size;
		AABB() {};
		AABB(const Vector3& p_position, const Vector3& p_size) : position(p_position), size(p_size) {}

		num_fd GetVolume() const;
		bool HasZeroVolume() const;
		bool HasNoSurface() const;

		Vector3 GetCenter() const { return position * (size * 0.5); }

		bool IsEqualApprox(const AABB& p_aabb) const;
		bool IsIntersects(const AABB& p_aabb) const;
		bool IsIntersectsInclusive(const AABB& p_aabb) const;
		bool Encloses(const AABB& p_aabb) const;

		AABB Merge(const AABB& p_aabb);
		void MergeWith(const AABB& p_aabb);
		AABB Intersection(const AABB& p_aabb) const;
		bool IsIntersectsSegment(const Vector3& p_from, const Vector3& p_to, Vector3* r_intersectionPoint = nullptr, Vector3* r_normal = nullptr, num_fd* r_t = nullptr) const;
		bool IsIntersectsRay(const Vector3& p_from, const Vector3& p_dir, Vector3* r_intersectionPoint = nullptr, Vector3* r_normal = nullptr, num_fd* r_t = nullptr) const;
		bool IsSmitsIntersectRay(const Vector3& p_from, const Vector3& p_dir, num_fd r_tmin, num_fd r_tmax) const;

		bool IsIntersectsConvexShape(const Plane* p_planes, int p_planeCount, const Vector3* p_points, int p_pointCount) const;
		bool IsInsideConvexShape(const Plane* p_planes, int p_planeCount) const;
		bool IsIntersectsPlane(const Plane& p_plane) const;

		bool HasPoint(const Vector3& p_point) const;
		Vector3 GetSupport(const Vector3& p_normal) const;

		Vector3 GetLongestAxis() const;
		int GetLongestAxisIndex() const;
		num_fd GetLongestAxisSize() const;

		Vector3 GetShortestAxis() const;
		int GetShortestAxisIndex() const;
		num_fd GetShortestAxisSize() const;

		AABB Grow(const num_fd p_by) const;
		void GrowBy(const num_fd p_amount);

		void GetEdge(int p_index, Vector3& r_from, Vector3& r_to) const;
		Vector3 GetEndpoint(int p_point) const;

		AABB Expand(const Vector3& p_vector) const;
		void ExpandTo(const Vector3& p_vector);
		void ProjectRangeInPlane(const Plane& p_plane, num_fd& r_min, num_fd& r_max) const;
		bool CreateFromPoints(const std::vector<Vector3>& p_points);

		AABB Abs() const {
			return AABB(Vector3(position.x + MIN(size.x, 0), position.y + MIN(size.y, 0), position.z + MIN(size.z, 0)), size.Abs());
		}


		bool operator==(const AABB& p_aabb) const;
		bool operator!=(const AABB& p_aabb) const;
	};
}
#endif // ST_AABB_H
