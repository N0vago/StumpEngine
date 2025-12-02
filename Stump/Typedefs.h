#ifndef ST_TYPEDEFS_H
#define ST_TYPEDEFS_H

#define SWAP(m_x, m_y) __swap_tmpl((m_x), (m_y))
template <class T>
inline void __swap_tmpl(T& x, T& y) {
	T aux = x;
	x = y;
	y = aux;
}


#ifndef SGN
#define SGN(m_v) (((m_v) < 0) ? (-1.0f) : (+1.0f))
#endif

#ifdef NUM_FD_IS_DOUBLE
typedef double num_fd;
#else
typedef float num_fd;
#endif

enum ClockDirection {
	CLOCKWISE = 0,
	COUNTERCLOCKWISE = 1
};


#endif