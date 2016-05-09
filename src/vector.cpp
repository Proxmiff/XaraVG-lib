


#include "camtypes.h"
#include "vector.h"


// We want better memory tracking
#define new CAM_DEBUG_NEW

using namespace std;

// Functions follow

#ifdef _DEBUG
template <class T>
ostream& operator << (ostream& os, Vector3<T>& v)
{
	os << "(" << v.v0 << "," << v.v1 << "," << v.v2 << ")\n";
	return os;
}
#endif
