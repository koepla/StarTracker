#ifndef STARTRACKER_SERIAL_SERIAL_H
#define STARTRACKER_SERIAL_SERIAL_H

#include <windows.h>
#include <string>
#include <vector>

/*
*	For reference, see https://docs.microsoft.com/en-us/previous-versions/ff802693(v=msdn.10)?redirectedfrom=MSDN
*/

namespace Serial {

	class SerialException : public std::exception {

	private:
		std::string message;

	public:
		explicit SerialException(std::string&& message) noexcept;

		[[nodiscard]] virtual const char* what() const noexcept override;
	};

	class SerialPort
	{
	private:
		HANDLE hCom;
		DWORD dwEventMask;
		bool isOpen;

	public:
		SerialPort() noexcept;
		~SerialPort() noexcept(false);

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
		[[nodiscard]] uint32_t Read(uint8_t* buffer, uint32_t bytes2read) noexcept(false);

		/*
		*	Writes to currently open Port
		*	Takes byte-buffer and bytes to write
		*/
		[[nodiscard]] uint32_t Write(uint8_t* buffer, uint32_t bytes2write) noexcept(false);

		/*
		*	Returns true if Port is open
		*/
		[[nodiscard]] bool IsOpen() const noexcept;
		[[nodiscard]] uint32_t Available() const noexcept(false);

		void WaitComm() noexcept;


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

#endif // STARTRACKER_SERIAL_SERIAL_H