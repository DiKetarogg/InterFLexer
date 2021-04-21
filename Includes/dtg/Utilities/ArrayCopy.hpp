#ifndef DTG_ARRAYCOPY
#define DTG_ARRAYCOPY
#include <stddef.h>
//////////
///Not checking for nullptr intentionally.
/////////
namespace dtg {
	template <class T>
		T* ArrayCopy (T* dest, const T* source, size_t size) {
			while (size) {
				--size;
				dest[size] = source[size];
			}
			return dest;
		}
	template <class T>
		T* TerminatedArrayCopy (T* dest, const T* source, T terminator = 0) {
			size_t it = 0;
			while (source[it] != terminator) {
				dest[it] = source[it];
				++it;
			}
			dest[it] = source[it];
			return dest;
		}
}
#endif
