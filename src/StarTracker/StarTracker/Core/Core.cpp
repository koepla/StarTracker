#include "Core.hpp"

void* operator new(std::size_t size) {

	return StarTracker::Core::Allocator::Reserve(size);
}

void operator delete(void* block) {

	StarTracker::Core::Allocator::Free(block);
}