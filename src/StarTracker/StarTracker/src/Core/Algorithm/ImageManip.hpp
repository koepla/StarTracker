#ifndef STARTRACKER_CORE_ALGO_IMAGEMANIP_H
#define STARTRACKER_CORE_ALGO_IMAGEMANIP_H

#include <vector>

namespace StarTracker::Core::Algo {

	class ImageManip {

	public:
		/**
		* Stacks two images by averaging the values of the data arrays
		*
		* @param img1 pointer to the image data of the first image
		* 
		* @param img2 pointer to the image data of the second image
		*
		* @param size number of elements in the data array (should be width * height)
		*
		* @return stacked image data as std::vector
		* 
		*/
		template <typename T>
		[[nodiscard]] static std::vector<T> Stack(const T* img1, const T* img2, std::size_t size) {

			std::vector<T> out{ size };

			for (auto i = std::size_t{ 0 }; i < size; i++) {

				out[i] = (((*(img1 + i)) + (*(img2 + i))) / 2);
			}

			return out;
		}
	};
}

#endif // STARTRACKER_CORE_ALGO_IMAGEMANIP_H