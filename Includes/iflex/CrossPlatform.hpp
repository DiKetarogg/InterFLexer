#ifndef IFLEX_CROSSPLATFORM_HPP
#define IFLEX_CROSSPLATFORM_HPP

#if defined(__GNUC__) or defined(__clang__)
	#define IFLEX_FORCE_INLINE inline __attribute__((always_inline))
#elif defined(__MSVC__)
	#define IFLEX_FORCE_INLINE() inline __forceinline
#else
	#define IFLEX_FORCE_INLINE inline
#endif



#endif
