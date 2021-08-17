#pragma once
#include <iostream>
#include <vector>


namespace Protocol {

	/*
	*	enumeration for protocol flags
	*/
	enum class Command : uint8_t {

		NONE   = (1 << 0),
		WAKEUP = (1 << 1),
		SLEEP  = (1 << 2),
		MOVE   = (1 << 3),
		STOP   = (1 << 4),
		ORIGIN = (1 << 5),
		ACK    = (1 << 6)
	};

	/*
	*	header struct, contains flag and size in bytes
	*/
	struct Header {

		__declspec(align(1)) Command flag;
		__declspec(align(1)) uint8_t size;

		Header() : flag(Command::NONE), size(0) {

		}

		Header(Command flag, uint8_t size) : flag(flag), size(size) {

		}
	};

	/*
	*	protocol struct, contains header and a byte buffer, which size can be specified by the template
	*/
	template <uint16_t N>
	struct Package {

		static constexpr uint16_t BUFF_SIZE = N - sizeof(Header);

		__declspec(align(1)) Header header;
		__declspec(align(1)) uint8_t buff[BUFF_SIZE];

		/*
		*	Sets the header flag to NONE and the size to zero
		*	Sets the whole buffer to 0
		*/
		Package() : header(Header(Command::NONE, 0)) {

			static_assert(BUFF_SIZE >= 0, "Buffer size must not be negative!");

			std::memset(buff, 0, BUFF_SIZE);
		}

		/*
		*	Sets the specified header flag
		*	Sets size and buffer to zero
		*/
		Package(Command flag) : header(Header(flag, 0)) {

			static_assert(BUFF_SIZE >= 0, "Buffer size must not be negative!");

			std::memset(buff, 0, BUFF_SIZE);
		}

		Package& Clear() {
			
			std::memset(buff, 0, BUFF_SIZE);
			header.size = 0;
			header.flag = Command::NONE;

			return *this;
		}

		Package& SetFlag(Command flag) {

			header.flag = flag;

			return *this;
		}

		/*
		*	Function for pushing data into the buffer,
		*	Takes data and element count
		*	Copies data into the buffer
		*/
		template <typename T>
		Package& Push(const T& data) {

			uint8_t osize = header.size;
			std::memcpy(buff + osize, &data, sizeof(T));
			header.size += sizeof(T);

			return *this;
		}

		template <typename T>
		Package& PushRange(T* data, uint32_t count) {

			uint8_t osize = header.size;
			std::memcpy(buff + osize, data, count * sizeof(T));
			header.size += count * sizeof(T);
			return *this;
		}

		/*
		*	Returns a value of type T at the given index
		*/
		template <typename T>
		T Read(uint32_t index) {

			return *reinterpret_cast<T*>(buff + index * sizeof(T));
		}

		/*
		*	Returns a pointer of type T to the buffer at the given offset
		*/
		template <typename T>
		T* ReadRange(uint32_t offset) {

			return reinterpret_cast<T*>(buff + offset * sizeof(T));
		}

		/*
		*	Returns how many bytes of the Buffer are filled
		*/
		size_t GetSize() const {

			return static_cast<size_t>(header.size);
		}
	};

	typedef Package<8>   Pack8;
	typedef Package<16>  Pack16;
	typedef Package<32>  Pack32;
	typedef Package<64>  Pack64;
	typedef Package<128> Pack128;
}