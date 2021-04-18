#pragma once
#include <iostream>
#include <vector>

/*
*	enumeration for protocol flags
*/
enum class stt_flag : int32_t {

	NONE = -1,
	WAKEUP = 0,
	SLEEP = 1,
	MOVE = 2,
	STOP = 3,
	ORIGIN = 4,
	ACK = 5
};

/*
*	header struct, contains flag and size in bytes
*/
struct stt_header {

	stt_flag flag;
	uint32_t size;

	stt_header() {

		this->flag = stt_flag::NONE;
		this->size = 0;
	}
	stt_header(stt_flag flag, uint32_t size) {

		this->flag = flag;
		this->size = size;
	}
};

/*
*	protocol struct, contains header and a byte buffer, which size can be specified by the template
*/
template <int n>
struct stt_protocol {

	stt_header header;
	uint8_t buff[n];

	/*
	*	Sets the header flag to NONE and the size to zero
	*	Sets the whole buffer to 0
	*/
	stt_protocol() {
		this->header = stt_header(stt_flag::NONE, 0);
		std::memset(buff, 0, n);
	}

	/*
	*	Sets the specified header flag
	*	Sets size and buffer to zero
	*/
	stt_protocol(stt_flag flag) {

		this->header = stt_header(flag, 0);
		std::memset(buff, 0, n);
	}

	/*
	*	Function for pushing data into the buffer,
	*	Takes data and element count
	*	Copes data into the buffer
	*/
	template <typename T>
	stt_protocol& push(const T& data) {

		static_assert(std::is_standard_layout<T>::value, "must be standard type");
		uint32_t osize = this->header.size;
		std::memcpy(buff + osize, &data, sizeof(T));
		this->header.size += sizeof(T);

		return *this;
	}

	template <typename T>
	stt_protocol& push_range(T* data, uint32_t count) {

		static_assert(std::is_standard_layout<T>::value, "must be standard type");
		uint32_t osize = this->header.size;
		std::memcpy(buff + osize, data, count * sizeof(T));
		this->header.size += count * sizeof(T);
		return *this;
	}


	/*
	*	Returns a value of type T at the given index 
	*/
	template <typename T>
	T read(uint32_t index) {

		return *reinterpret_cast<T*>(this->buff + index * sizeof(T));
	}

	/*
	*	Returns a pointer of type T to the buffer at the given offset
	*/
	template <typename T>
	T* read_range(uint32_t offset) {

		return reinterpret_cast<T*>(this->buff + offset * sizeof(T));
	}

	/*
	*	Returns the size in bytes of the whole struct
	*/
	uint32_t size() const {
		return sizeof(this->header) + this->header.size;
	}
};