#ifndef STARTRACKER_FIRMWARE_PACKAGE_H
#define STARTRACKER_FIRMWARE_PACKAGE_H

#include <Arduino.h>

#pragma pack(push, 1)

namespace Protocol {

	/*
	*	enumeration for protocol flags
	*/
	enum class Command : uint8_t {

		NONE   = (1 << 0),
		WAKEUP = (1 << 1),
		SLEEP  = (1 << 2),
		MOVE   = (1 << 3),
		CONF   = (1 << 4),
		ORIGIN = (1 << 5),
		ACK    = (1 << 6)
	};

	/*
	*	header struct, contains flag and size in bytes
	*/
	struct Header {

		Command Flag;
		uint8_t Size;

		Header() : Flag{Command::NONE}, Size{0} {


		}

		Header(Command flag, uint8_t size) : Flag{flag}, Size{size} {


		}
	};

	/*
	*	protocol struct, contains header and a byte buffer, which size can be specified by the template
	*	maximum size is 255 bytes
	*/
	template <uint16_t N>
	struct Package {

		static constexpr uint16_t BUFF_SIZE = N - sizeof(Protocol::Header);

		Protocol::Header Header;
		uint8_t buff[BUFF_SIZE];

		/*
		*	Sets the header flag to NONE and the size to zero
		*	Sets the whole buffer to 0
		*/
		Package() : Header{Protocol::Header{Command::NONE, 0}} {

			static_assert(BUFF_SIZE >= 0, "Buffer size must not be less than zero");

			memset(buff, 0, BUFF_SIZE);
		}

		/*
		*	Sets the specified header flag
		*	Sets size and buffer to zero
		*/
		Package(Command flag) : Header{Protocol::Header{flag, 0}} {

			static_assert(BUFF_SIZE >= 0, "Buffer size must not be less than zero");

			memset(buff, 0, BUFF_SIZE);
		}

		Package& Clear() {
			
			memset(buff, 0, BUFF_SIZE);
			Header.Size = 0;
			Header.Flag = Command::NONE;

			return *this;
		}

		Package& SetFlag(Command flag) {

			Header.Flag = flag;

			return *this;
		}

		/*
		*	Function for pushing data into the buffer,
		*	Takes data and element count
		*	Copes data into the buffer
		*/
		template <typename T>
		Package& Push(const T& data) {

			uint8_t osize = Header.Size;
			memcpy(buff + osize, &data, sizeof(T));
			Header.Size += sizeof(T);

			return *this;
		}

		template <typename T>
		Package& PushRange(const T* data, uint8_t count) {

			uint8_t osize = Header.Size;
			memcpy(buff + osize, data, count * sizeof(T));
			Header.Size += count * sizeof(T);
			return *this;
		}

		/*
		*	Returns a value of type T at the given index
		*/
		template <typename T>
		T Read(uint8_t index) {

			return *reinterpret_cast<T*>(buff + index * sizeof(T));
		}

		/*
		*	Returns a pointer of type T to the buffer at the given offset
		*/
		template <typename T>
		T* ReadRange(uint8_t offset) {

			return reinterpret_cast<T*>(buff + offset * sizeof(T));
		}

		/*
		*	Returns how many bytes of the buffer are in use
		*/
		size_t GetSize() const {

			return static_cast<size_t>(Header.Size);
		}

		Command GetFlag() const {

			return Header.Flag;
		}
	};

	typedef Package<8>   Pack8;
	typedef Package<16>  Pack16;
	typedef Package<32>  Pack32;
	typedef Package<64>  Pack64;
	typedef Package<128> Pack128;
}

#pragma pack(pop)

#endif // STARTRACKER_FIRMWARE_PACKAGE_H