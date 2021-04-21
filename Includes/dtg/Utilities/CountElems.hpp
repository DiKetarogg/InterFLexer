#ifndef DTG_COUNTELEMS
#define DTG_COUNTELEMS

namespace dtg {

	template <class T>
	size_t countElems(const T* arr, T terminator = 0) {
		size_t it = 0;
		while (arr[it] != terminator)
			++it;
		return it;
	}



}

#endif
