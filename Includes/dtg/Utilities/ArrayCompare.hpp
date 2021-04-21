#ifndef DTG_UTILITIES_ARRAYCOMPARE_HPP
#define DTG_UTILITIES_ARRAYCOMPARE_HPP

namespace dtg {

	template <class T>
	T TerminatedArrayCompare(const T* first, const T* second) {
		while (*first && *first == *second) {
			++first;
			++second;
		}
		return *first - *second;
	}

	template <class T>
	T ArrayCompare(const T* first, const T* second, size_t length) {
		while (length && *first == *second) {
			--length;
			++first;
			++second;
		}
		return *first - *second;
	}


}

#endif
