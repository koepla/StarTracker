#ifndef STARTRACKER_SERIAL_PACKAGE_H
#define STARTRACKER_SERIAL_PACKAGE_H

#include <vector>

namespace Serial {

	class PackageException : public std::exception {

	private:
		std::string message;

	public:
		explicit PackageException(std::string&& message) : message(std::move(message)) { }
		[[nodiscard]] virtual const char* what() const noexcept override { return message.c_str(); }
	};

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

		__declspec(align(1)) Command Flag;
		__declspec(align(1)) uint8_t Size;

		Header() : Flag(Command::NONE), Size(0) {

		}

		Header(Command flag, uint8_t size) : Flag(flag), Size(size) {

		}
	};

	/*
	*	protocol struct, contains header and a byte buffer, which size can be specified by the template
	*/
	template <uint16_t N>
	class Package {

	private:
		static constexpr uint16_t BUFFER_SIZE = N - sizeof(Header);

		__declspec(align(1)) Header header;
		__declspec(align(1)) uint8_t buffer[BUFFER_SIZE];

	public:
		/*
		*	Sets the header flag to NONE and the size to zero
		*	Sets the whole buffer to 0
		*/
		Package() : header(Command::NONE, 0) {

			static_assert(BUFFER_SIZE >= 0, "Buffer size must not be negative!");

			std::memset(buffer, 0, BUFFER_SIZE);
		}

		/*
		*	Sets the specified header flag
		*	Sets size and buffer to zero
		*/
		Package(Command flag) : header(flag, 0) {

			static_assert(BUFFER_SIZE >= 0, "Buffer size must not be negative!");

			std::memset(buffer, 0, BUFFER_SIZE);
		}

		Package& Clear() noexcept {
			
			std::memset(buffer, 0, BUFFER_SIZE);
			header.Size = 0;
			header.Flag = Command::NONE;

			return *this;
		}

		Package& SetFlag(Command flag) noexcept {

			header.Flag = flag;

			return *this;
		}

		/*
		*	Function for pushing data into the buffer,
		*	Takes data and element count
		*	Copies data into the buffer
		*/
		template <typename T>
		Package& Push(const T& data) noexcept(false) {

			if (sizeof(T) + header.Size > BUFFER_SIZE) {

				throw PackageException("Push would exceed remaining buffer size");
			}

			uint8_t currentSize = header.Size;
			std::memcpy(buffer + currentSize, &data, sizeof(T));
			header.Size += sizeof(T);

			return *this;
		}

		template <typename T>
		Package& PushRange(const T* data, uint32_t count) noexcept(false) {

			if ((sizeof(T) * count) + header.Size > BUFFER_SIZE) {

				throw PackageException("Push would exceed remaining buffer size");
			}

			uint8_t currentSize = header.Size;
			std::memcpy(buffer + currentSize, data, count * sizeof(T));
			header.Size += count * sizeof(T);
			return *this;
		}

		/*
		*	Returns a value of type T at the given index
		*/
		template <typename T>
		[[nodiscard]] T Read(uint32_t index) noexcept(false) {

			if ((index) * sizeof(T) > (BUFFER_SIZE - sizeof(T))) {

				throw PackageException("Index out of range");
			}

			return *reinterpret_cast<T*>(buffer + index * sizeof(T));
		}

		/*
		*	Returns a pointer of type T to the buffer at the given offset
		*/
		template <typename T>
		[[nodiscard]] T* ReadRange(uint32_t offset) noexcept(false) {

			if ((offset) * sizeof(T) > (BUFFER_SIZE - sizeof(T))) {

				throw PackageException("Offset out of range");
			}

			return reinterpret_cast<T*>(buffer + offset * sizeof(T));
		}

		/*
		*	Returns how many bytes of the Buffer are filled
		*/
		[[nodiscard]] size_t GetSize() const noexcept {

			return static_cast<size_t>(header.Size);
		}

		[[nodiscard]] Command GetFlag() const noexcept {

			return header.Flag;
		}
	};

	typedef Package<8>   Pack8;
	typedef Package<16>  Pack16;
	typedef Package<32>  Pack32;
	typedef Package<64>  Pack64;
	typedef Package<128> Pack128;
}

#endif // STARTRACKER_SERIAL_PACKAGE_H