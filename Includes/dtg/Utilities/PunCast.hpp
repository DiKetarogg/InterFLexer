#ifndef DTG_UTILITIES_PUNCAST_HPP
#define DTG_UTILITIES_PUNCAST_HPP

namespace dtg {

template <class T, class A>
T& pun_cast(const A& arg) {
	return *reinterpret_cast<T*>(&arg);
}

}

#endif
