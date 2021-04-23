#pragma once
#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

/*
*	For reference, see https://docs.microsoft.com/en-us/previous-versions/ff802693(v=msdn.10)?redirectedfrom=MSDN
*/

namespace protocol {

	class stt_serial_exception : public std::exception {

	private:
		std::string message;

	public:
		stt_serial_exception(std::string&& message) noexcept;
		const char* what() const noexcept override;

	};

	class stt_serial
	{
	private:
		HANDLE hcomm;
		DWORD dweventmask;
		bool isopen;

	public:

		stt_serial();
		~stt_serial();

		/*
		*	Opens the specified port
		*	Arguments are portname and baudrate
		*/
		void open(const std::string& port, uint32_t baudrate) noexcept(false);

		/*
		*	Closes the currently open Port
		*/
		void close() noexcept(false);

		/*
		*	Reads from currently open Port
		*	Takes byte-buffer and bytes to reader
		*/
		uint32_t read(uint8_t* buffer, uint32_t bytes2read) noexcept(false);

		/*
		*	Writes to currently open Port
		*	Takes byte-buffer and bytes to write
		*/
		uint32_t write(uint8_t* buffer, uint32_t bytes2write) noexcept(false);

		/*
		*	Returns true if Port is open
		*/
		bool is_open() const noexcept;
		bool rx_available() noexcept;

	private:

		/*
		*	Add Portname Prefix if needed
		*/
		inline std::string prefix_port(const std::string& port);

		/*
		*	Set Baudrate via CommState
		*/
		void set_baudrate(uint32_t baudrate);
		void init_recievemask();
		void init_commtimeout();

	public:

		/*
		*	Returns a list of all available ports
		*/
		static std::vector<std::string> get_ports();

	};
}