#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <iostream>
#include <windows.h>
#include <string>
#include <vector>

/*
*	For reference, see https://docs.microsoft.com/en-us/previous-versions/ff802693(v=msdn.10)?redirectedfrom=MSDN
*/

namespace Protocol {

	class SerialException : public std::exception {

	private:
		std::string message;

	public:

		SerialException(std::string&& message) noexcept;

		[[nodiscard]] virtual const char* what() const noexcept override;
	};

	class Serial
	{
	private:
		HANDLE hCom;
		DWORD dwEventMask;
		bool isOpen;

	public:

		Serial();
		~Serial();

		/*
		*	Opens the specified port
		*	Arguments are portname and baudrate
		*/
		void Open(const std::string& port, uint32_t baudrate) noexcept(false);

		/*
		*	Closes the currently open Port
		*/
		void Close() noexcept(false);

		/*
		*	Reads from currently open Port
		*	Takes byte-buffer and bytes to reader
		*/
		uint32_t Read(uint8_t* buffer, uint32_t bytes2read) noexcept(false);

		/*
		*	Writes to currently open Port
		*	Takes byte-buffer and bytes to write
		*/
		uint32_t Write(uint8_t* buffer, uint32_t bytes2write) noexcept(false);

		/*
		*	Returns true if Port is open
		*/
		bool IsOpen() const noexcept;

		void WaitComm() noexcept;

		uint32_t Available() noexcept(false);

	private:

		/*
		*	Add Portname Prefix if needed
		*/
		[[nodiscard]] inline std::string prefixPort(const std::string& port);

		/*
		*	Set Baudrate via CommState
		*/
		void setBaudrate(uint32_t baudrate);
		void initReceiveMask();
		void initTimeouts(uint32_t charCount);

	public:

		/*
		*	Returns a list of all available ports
		*/
		[[nodiscard]] static std::vector<std::string> GetPortNames();

	};
}

#endif // _SERIAL_H_