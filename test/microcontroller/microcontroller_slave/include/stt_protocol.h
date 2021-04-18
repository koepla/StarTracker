#ifndef STT_PROT_H
#define STT_PROT_H

#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

enum class stt_flag : int32_t {

    NONE = -1,
    WAKEUP = 0,
    SLEEP = 1,
    MOVE = 2,
    STOP = 3,
    ORIGIN = 4,
    ACK = 5
};

struct stt_header {

    stt_flag flag;
    uint32_t size;

    stt_header(){
        this->flag = stt_flag::NONE;
        this->size = 0;
    }
    stt_header(stt_flag flag, uint32_t size){
        this->flag = flag;
        this->size = size;
    }
};

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
		memset(buff, 0, n);
	}

	/*
	*	Sets the specified header flag
	*	Sets size and buffer to zero
	*/
	stt_protocol(stt_flag flag) {

		this->header = stt_header(flag, 0);
		memset(buff, 0, n);
	}

	/*
	*	Function for pushing data into the buffer,
	*	Takes data and element count
	*	Copes data into the buffer
	*/
	template <typename T>
	stt_protocol& push(const T& data, uint32_t count) {

		uint32_t osize = this->header.size;
		memcpy(buff + osize, &data, count * sizeof(T));
		this->header.size += count * sizeof(T);
		return *this;
	}


	/*
	*	Returns a pointer of type T to the buffer at the given index
	*/
	template <typename T>
	T* read(uint32_t index) {

		return reinterpret_cast<T*>(this->buff + index * sizeof(T));
	}

	/*
	*	Returns the size in bytes of the whole struct
	*/
	uint32_t size() const {
		return sizeof(this->header) + this->header.size;
	}
};


#endif