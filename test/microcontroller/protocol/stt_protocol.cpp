#include <iostream>
#include <vector>

/* 
*	enumeration for protocol flags 
*/
enum class stt_flag {
	
	NONE = -1,
	WAKEUP = 0,
	SLEEP = 1,
	MOVE = 2,
	STOP = 3,
	ORIGIN = 4,
	ACK
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
	stt_protocol& push(const T& data, uint32_t count) {

		uint32_t osize = this->header.size;
		std::memcpy(buff + osize, &data, count * sizeof(T));
		this->header.size += count * sizeof(T);

		return *this;
	}

	/*
	*	Returns the size in bytes of the whole struct
	*/
	uint32_t size() const {
		return sizeof(this->header) + this->header.size;
	}
};

/*
*	Helper union if a byte buffer for the stt_protocol struct is needed
*/
template <int n>
union stt_protocol_bytes {

	stt_protocol<n> transfer;
	uint8_t buff[n + sizeof(stt_header)];

	stt_protocol_bytes() {
		this->transfer = stt_protocol<n>();
	}

	stt_protocol_bytes(stt_protocol<n> transfer) {
		this->transfer = transfer;
	}
};

int main(int argc, char** argv) {

	constexpr int BUFF_SIZE = 2048;
	stt_protocol<BUFF_SIZE> transfer(stt_flag::ACK);
	transfer.push<int>(100, 1);
	transfer.push<int>(200, 1);
	transfer.push<char>('a', 1);

	std::cout << transfer.size() << std::endl;

	stt_protocol_bytes<BUFF_SIZE> buffer;
	buffer.transfer = transfer;

	/* Simulates communication with arduino by copying the buffer */
	stt_protocol<BUFF_SIZE> micro;
	stt_protocol_bytes<BUFF_SIZE> mbuffer;
	memcpy(mbuffer.buff, buffer.buff, BUFF_SIZE);
	micro = mbuffer.transfer;

	/* Prints the same size as the original stt_protocol */
	std::cout << micro.size() << std::endl;

	return 0;
}