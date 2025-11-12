#include "Matrix4x4.h"

void Matrix4x4::SetIdentity() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix[i][j] = (i == j) ? 1 : 0;
		}
	}
}

void Matrix4x4::SetZero() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			matrix[i][j] = 0;
		}
	}
}

void Matrix4x4::SetPerspective(num_fd p_fovy_degrees, num_fd p_aspect, num_fd p_near, num_fd p_far, bool p_flip_fov) {
	if (p_flip_fov) {
		p_fovy_degrees = get_fovy(p_fovy_degrees, 1.0 / p_aspect);
	}

	num_fd sine, cotangent, deltaZ;
	num_fd radians = p_fovy_degrees / 2.0 * Math::PI / 180.0;

	deltaZ = p_far - p_near;
	sine = Math::Sin(radians);

	if ((deltaZ == 0) || (sine == 0) || (p_aspect == 0)) {
		return;
	}
	cotangent = Math::Cos(radians) / sine;

	SetIdentity();

	matrix[0][0] = cotangent / p_aspect;
	matrix[1][1] = cotangent;
	matrix[2][2] = -(p_far + p_near) / deltaZ;
	matrix[2][3] = -1;
	matrix[3][2] = -2 * p_near * p_far / deltaZ;
	matrix[3][3] = 0;
}

void Matrix4x4::SetForHMD(int p_eye, num_fd p_aspect, num_fd p_intraocular_dist, num_fd p_display_width, num_fd p_display_to_lens, num_fd p_oversample, num_fd p_z_near, num_fd p_z_far) {
	// we first calculate our base frustum on our values without taking our lens magnification into account.
	num_fd f1 = (p_intraocular_dist * 0.5) / p_display_to_lens;
	num_fd f2 = ((p_display_width - p_intraocular_dist) * 0.5) / p_display_to_lens;
	num_fd f3 = (p_display_width / 4.0) / p_display_to_lens;

	// now we apply our oversample factor to increase our FOV. how much we oversample is always a balance we strike between performance and how much
	// we're willing to sacrifice in FOV.
	num_fd add = ((f1 + f2) * (p_oversample - 1.0)) / 2.0;
	f1 += add;
	f2 += add;
	f3 *= p_oversample;

	// always apply KEEP_WIDTH aspect ratio
	f3 /= p_aspect;

	switch (p_eye) {
	case 1: { // left eye
		SetFrustum(-f2 * p_z_near, f1 * p_z_near, -f3 * p_z_near, f3 * p_z_near, p_z_near, p_z_far);
	}; break;
	case 2: { // right eye
		SetFrustum(-f1 * p_z_near, f2 * p_z_near, -f3 * p_z_near, f3 * p_z_near, p_z_near, p_z_far);
	}; break;
	default: { // mono, does not apply here!
	}; break;
	};
};


void Matrix4x4::SetOrthogonal(num_fd p_left, num_fd p_right, num_fd p_bottom, num_fd p_top, num_fd p_near, num_fd p_far) {
	SetIdentity();

	matrix[0][0] = 2 / (p_right - p_left);
	matrix[3][0] = -((p_right + p_left) / (p_right - p_left));
	matrix[1][1] = 2 / (p_top - p_bottom);
	matrix[3][1] = -((p_top + p_bottom) / (p_top - p_bottom));
	matrix[2][2] = -2 / (p_far - p_near);
	matrix[3][2] = -((p_far + p_near) / (p_far - p_near));
	matrix[3][3] = 1.0;
}

void Matrix4x4::SetOrthogonal(num_fd p_size, num_fd p_aspect, num_fd p_znear, num_fd p_zfar, bool p_flip_fov) {
	if (!p_flip_fov) {
		p_size *= p_aspect;
	}

	SetOrthogonal(-p_size / 2, +p_size / 2, -p_size / p_aspect / 2, +p_size / p_aspect / 2, p_znear, p_zfar);
}

void Matrix4x4::SetFrustum(num_fd p_left, num_fd p_right, num_fd p_bottom, num_fd p_top, num_fd p_near, num_fd p_far) {

	num_fd* te = &matrix[0][0];
	num_fd x = 2 * p_near / (p_right - p_left);
	num_fd y = 2 * p_near / (p_top - p_bottom);

	num_fd a = (p_right + p_left) / (p_right - p_left);
	num_fd b = (p_top + p_bottom) / (p_top - p_bottom);
	num_fd c = -(p_far + p_near) / (p_far - p_near);
	num_fd d = -2 * p_far * p_near / (p_far - p_near);

	te[0] = x;
	te[1] = 0;
	te[2] = 0;
	te[3] = 0;
	te[4] = 0;
	te[5] = y;
	te[6] = 0;
	te[7] = 0;
	te[8] = a;
	te[9] = b;
	te[10] = c;
	te[11] = -1;
	te[12] = 0;
	te[13] = 0;
	te[14] = d;
	te[15] = 0;
}


num_fd Matrix4x4::GetZFar() const {
	const num_fd* matrix = (const num_fd*)this->matrix;
	Plane new_plane = Plane(matrix[3] - matrix[2],
		matrix[7] - matrix[6],
		matrix[11] - matrix[10],
		matrix[15] - matrix[14]);

	new_plane.normal = -new_plane.normal;
	new_plane.Normalize();

	return new_plane.d;
}
num_fd Matrix4x4::GetZNear() const {
	const num_fd* matrix = (const num_fd*)this->matrix;
	Plane new_plane = Plane(matrix[3] + matrix[2],
		matrix[7] + matrix[6],
		matrix[11] + matrix[10],
		-matrix[15] - matrix[14]);

	new_plane.Normalize();
	return new_plane.d;
}

glm::mat4 Matrix4x4::GetGlmMat4() const {
	glm::mat4 res;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			res[i][j] = matrix[i][j];
		}
	}
	return res;
}

Vector3 Matrix4x4::GetViewportHalfExtents() const {
	const num_fd* matrix = (const num_fd*)this->matrix;
	///////--- Near Plane ---///////
	Plane near_plane = Plane(matrix[3] + matrix[2],
		matrix[7] + matrix[6],
		matrix[11] + matrix[10],
		-matrix[15] - matrix[14]);
	near_plane.Normalize();

	///////--- Right Plane ---///////
	Plane right_plane = Plane(matrix[3] - matrix[0],
		matrix[7] - matrix[4],
		matrix[11] - matrix[8],
		-matrix[15] + matrix[12]);
	right_plane.Normalize();

	Plane top_plane = Plane(matrix[3] - matrix[1],
		matrix[7] - matrix[5],
		matrix[11] - matrix[9],
		-matrix[15] + matrix[13]);
	top_plane.Normalize();

	Vector3 res;
	near_plane.Intersect3(right_plane, top_plane, &res);

	return res;
}

bool Matrix4x4::GetEndpoints(const Matrix3x4& p_transform, Vector3* p_8points) const {
	std::vector<Plane> planes = GetProjectionPlanes(Matrix3x4());
	const Planes intersections[8][3] = {
		{ PLANE_FAR, PLANE_LEFT, PLANE_TOP },
		{ PLANE_FAR, PLANE_LEFT, PLANE_BOTTOM },
		{ PLANE_FAR, PLANE_RIGHT, PLANE_TOP },
		{ PLANE_FAR, PLANE_RIGHT, PLANE_BOTTOM },
		{ PLANE_NEAR, PLANE_LEFT, PLANE_TOP },
		{ PLANE_NEAR, PLANE_LEFT, PLANE_BOTTOM },
		{ PLANE_NEAR, PLANE_RIGHT, PLANE_TOP },
		{ PLANE_NEAR, PLANE_RIGHT, PLANE_BOTTOM },
	};

	for (int i = 0; i < 8; i++) {
		Vector3 point;
		bool res = planes[intersections[i][0]].Intersect3(planes[intersections[i][1]], planes[intersections[i][2]], &point);
		p_8points[i] = p_transform.XForm(point);
	}

	return true;
}

std::vector<Plane> Matrix4x4::GetProjectionPlanes(const Matrix3x4& p_transform) const {
	/** Fast Plane Extraction from combined modelview/projection matrices.
	 * References:
	 * https://web.archive.org/web/20011221205252/http://www.markmorley.com/opengl/frustumculling.html
	 * https://web.archive.org/web/20061020020112/http://www2.ravensoft.com/users/ggribb/plane%20extraction.pdf
	 */

	std::vector<Plane> planes;

	const num_fd* matrix = (const num_fd*)this->matrix;

	Plane new_plane;

	///////--- Near Plane ---///////
	new_plane = Plane(matrix[3] + matrix[2],
		matrix[7] + matrix[6],
		matrix[11] + matrix[10],
		matrix[15] + matrix[14]);

	new_plane.normal = -new_plane.normal;
	new_plane.Normalize();

	planes.push_back(p_transform.XForm(new_plane));

	///////--- Far Plane ---///////
	new_plane = Plane(matrix[3] - matrix[2],
		matrix[7] - matrix[6],
		matrix[11] - matrix[10],
		matrix[15] - matrix[14]);

	new_plane.normal = -new_plane.normal;
	new_plane.Normalize();

	planes.push_back(p_transform.XForm(new_plane));

	///////--- Left Plane ---///////
	new_plane = Plane(matrix[3] + matrix[0],
		matrix[7] + matrix[4],
		matrix[11] + matrix[8],
		matrix[15] + matrix[12]);

	new_plane.normal = -new_plane.normal;
	new_plane.Normalize();

	planes.push_back(p_transform.XForm(new_plane));

	///////--- Top Plane ---///////
	new_plane = Plane(matrix[3] - matrix[1],
		matrix[7] - matrix[5],
		matrix[11] - matrix[9],
		matrix[15] - matrix[13]);

	new_plane.normal = -new_plane.normal;
	new_plane.Normalize();

	planes.push_back(p_transform.XForm(new_plane));

	///////--- Right Plane ---///////
	new_plane = Plane(matrix[3] - matrix[0],
		matrix[7] - matrix[4],
		matrix[11] - matrix[8],
		matrix[15] - matrix[12]);

	new_plane.normal = -new_plane.normal;
	new_plane.Normalize();

	planes.push_back(p_transform.XForm(new_plane));

	///////--- Bottom Plane ---///////
	new_plane = Plane(matrix[3] + matrix[1],
		matrix[7] + matrix[5],
		matrix[11] + matrix[9],
		matrix[15] + matrix[13]);

	new_plane.normal = -new_plane.normal;
	new_plane.Normalize();

	planes.push_back(p_transform.XForm(new_plane));

	return planes;
}


Matrix4x4 Matrix4x4::Inverse() const {
	Matrix4x4 cm = *this;
	cm.Invert();
	return cm;
}

void Matrix4x4::Invert() {
	int i, j, k;
	int pvt_i[4], pvt_j[4]; /* Locations of pivot matrix */
	num_fd pvt_val; /* Value of current pivot element */
	num_fd hold; /* Temporary storage */
	num_fd determinat; /* Determinant */

	determinat = 1.0;
	for (k = 0; k < 4; k++) {
		/** Locate k'th pivot element **/
		pvt_val = matrix[k][k]; /** Initialize for search **/
		pvt_i[k] = k;
		pvt_j[k] = k;
		for (i = k; i < 4; i++) {
			for (j = k; j < 4; j++) {
				if (Math::Abs(matrix[i][j]) > Math::Abs(pvt_val)) {
					pvt_i[k] = i;
					pvt_j[k] = j;
					pvt_val = matrix[i][j];
				}
			}
		}

		/** Product of pivots, gives determinant when finished **/
		determinat *= pvt_val;
		if (Math::Abs(determinat) < (num_fd)1e-7) {
			return; //(false);  /** Matrix is singular (zero determinant). **/
		}

		/** "Interchange" rows (with sign change stuff) **/
		i = pvt_i[k];
		if (i != k) { /** If rows are different **/
			for (j = 0; j < 4; j++) {
				hold = -matrix[k][j];
				matrix[k][j] = matrix[i][j];
				matrix[i][j] = hold;
			}
		}

		/** "Interchange" columns **/
		j = pvt_j[k];
		if (j != k) { /** If columns are different **/
			for (i = 0; i < 4; i++) {
				hold = -matrix[i][k];
				matrix[i][k] = matrix[i][j];
				matrix[i][j] = hold;
			}
		}

		/** Divide column by minus pivot value **/
		for (i = 0; i < 4; i++) {
			if (i != k) {
				matrix[i][k] /= (-pvt_val);
			}
		}

		/** Reduce the matrix **/
		for (i = 0; i < 4; i++) {
			hold = matrix[i][k];
			for (j = 0; j < 4; j++) {
				if (i != k && j != k) {
					matrix[i][j] += hold * matrix[k][j];
				}
			}
		}

		/** Divide row by pivot **/
		for (j = 0; j < 4; j++) {
			if (j != k) {
				matrix[k][j] /= pvt_val;
			}
		}

		/** Replace pivot by reciprocal (at last we can touch it). **/
		matrix[k][k] = 1.0 / pvt_val;
	}

	/* That was most of the work, one final pass of row/column interchange */
	/* to finish */
	for (k = 4 - 2; k >= 0; k--) { /* Don't need to work with 1 by 1 corner*/
		i = pvt_j[k]; /* Rows to swap correspond to pivot COLUMN */
		if (i != k) { /* If rows are different */
			for (j = 0; j < 4; j++) {
				hold = matrix[k][j];
				matrix[k][j] = -matrix[i][j];
				matrix[i][j] = hold;
			}
		}

		j = pvt_i[k]; /* Columns to swap correspond to pivot ROW */
		if (j != k) { /* If columns are different */
			for (i = 0; i < 4; i++) {
				hold = matrix[i][k];
				matrix[i][k] = -matrix[i][j];
				matrix[i][j] = hold;
			}
		}
	}
}

Matrix4x4::Matrix4x4() {
	SetIdentity();
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& p_matrix) const {
	Matrix4x4 new_matrix;

	for (int j = 0; j < 4; j++) {
		for (int i = 0; i < 4; i++) {
			num_fd ab = 0;
			for (int k = 0; k < 4; k++) {
				ab += matrix[k][i] * p_matrix.matrix[j][k];
			}
			new_matrix.matrix[j][i] = ab;
		}
	}

	return new_matrix;
}

void Matrix4x4::SetLightBias() {
	num_fd* m = &matrix[0][0];

	m[0] = 0.5;
	m[1] = 0.0;
	m[2] = 0.0;
	m[3] = 0.0;
	m[4] = 0.0;
	m[5] = 0.5;
	m[6] = 0.0;
	m[7] = 0.0;
	m[8] = 0.0;
	m[9] = 0.0;
	m[10] = 0.5;
	m[11] = 0.0;
	m[12] = 0.5;
	m[13] = 0.5;
	m[14] = 0.5;
	m[15] = 1.0;
}

num_fd Matrix4x4::GetAspect() const {
	Vector3 vp_he = GetViewportHalfExtents();
	return vp_he.x / vp_he.y;
}

bool Matrix4x4::IsOrthogonal() const {
	return matrix[3][3] == 1.0;
}

num_fd Matrix4x4::GetFOV() const {
	const num_fd* matrix = (const num_fd*)this->matrix;

	Plane right_plane = Plane(matrix[3] - matrix[0],
		matrix[7] - matrix[4],
		matrix[11] - matrix[8],
		-matrix[15] + matrix[12]);
	right_plane.Normalize();

	if ((matrix[8] == 0) && (matrix[9] == 0)) {
		return Math::ToRadians(Math::Acos(Math::Abs(right_plane.normal.x))) * 2;
	}
	else {
		// our frustum is asymmetrical need to calculate the left planes angle separately..
		Plane left_plane = Plane(matrix[3] + matrix[0],
			matrix[7] + matrix[4],
			matrix[11] + matrix[8],
			matrix[15] + matrix[12]);
		left_plane.Normalize();

		return Math::ToRadians(Math::Acos(Math::Abs(left_plane.normal.x))) + Math::ToRadians(Math::Acos(Math::Abs(right_plane.normal.x)));
	}
}

void Matrix4x4::MakeScale(const Vector3& p_scale) {
	SetIdentity();
	matrix[0][0] = p_scale.x;
	matrix[1][1] = p_scale.y;
	matrix[2][2] = p_scale.z;
}

Matrix4x4::operator Matrix3x4() const {
	Matrix3x4 tr;
	const num_fd* m = &matrix[0][0];

	tr.mat3.elements[0][0] = m[0];
	tr.mat3.elements[1][0] = m[1];
	tr.mat3.elements[2][0] = m[2];

	tr.mat3.elements[0][1] = m[4];
	tr.mat3.elements[1][1] = m[5];
	tr.mat3.elements[2][1] = m[6];

	tr.mat3.elements[0][2] = m[8];
	tr.mat3.elements[1][2] = m[9];
	tr.mat3.elements[2][2] = m[10];

	tr.origin.x = m[12];
	tr.origin.y = m[13];
	tr.origin.z = m[14];

	return tr;
}

Matrix4x4::Matrix4x4(const Matrix3x4& p_transform) {
	const Matrix3x4& tr = p_transform;
	num_fd* m = &matrix[0][0];

	m[0] = tr.mat3.elements[0][0];
	m[1] = tr.mat3.elements[1][0];
	m[2] = tr.mat3.elements[2][0];
	m[3] = 0.0;
	m[4] = tr.mat3.elements[0][1];
	m[5] = tr.mat3.elements[1][1];
	m[6] = tr.mat3.elements[2][1];
	m[7] = 0.0;
	m[8] = tr.mat3.elements[0][2];
	m[9] = tr.mat3.elements[1][2];
	m[10] = tr.mat3.elements[2][2];
	m[11] = 0.0;
	m[12] = tr.origin.x;
	m[13] = tr.origin.y;
	m[14] = tr.origin.z;
	m[15] = 1.0;
}

Matrix4x4::Matrix4x4(glm::mat4 p_mat4) {
	num_fd* m = &matrix[0][0];
	m[0] = p_mat4[0][0];
	m[1] = p_mat4[1][0];
	m[2] = p_mat4[2][0];
	m[3] = p_mat4[3][0];
	m[4] = p_mat4[0][1];
	m[5] = p_mat4[1][1];
	m[6] = p_mat4[2][1];
	m[7] = p_mat4[3][1];
	m[8] = p_mat4[0][2];
	m[9] = p_mat4[1][2];
	m[10] = p_mat4[2][2];
	m[11] = p_mat4[3][2];
	m[12] = p_mat4[0][3];
	m[13] = p_mat4[1][3];
	m[14] = p_mat4[2][3];
	m[15] = p_mat4[3][3];
}

Matrix4x4::~Matrix4x4() {
}