#ifndef _IMAGEMANIP_H_
#define _IMAGEMANIP_H_

#include <vector>

namespace Algo {

	class ImageManip {

	public:

		template <typename T>
		static std::vector<T> Stack(const T* img1, const T* img2, uint32_t size) {

			std::vector<T> out(size);

			for (uint32_t i = 0; i < size; i++) {

				out[i] = (((*(img1 + i)) + (*(img2 + i))) / 2);
			}

			return out;
		}
	};
}

#endif // _IMAGEMANIP_H_