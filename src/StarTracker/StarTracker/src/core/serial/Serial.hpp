#ifndef STARTRACKER_SERIAL_SERIAL_H
#define STARTRACKER_SERIAL_SERIAL_H

#include <windows.h>
#include <string>
#include <vector>

/**
*	For reference, see https://docs.microsoft.com/en-us/previous-versions/ff802693(v=msdn.10)?redirectedfrom=MSDN
*/

namespace StarTracker::Serial {

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

		/**
		* Opens the specified COM port
		*
		* @param port name of the COM port
		* 
		* @param baudrate baudrate (self explaining)
		*
		* @throws SerialException if the COM port doesn't exist or couldn't be opened
		* 
		*/
		void Open(const std::string& port, uint32_t baudrate) noexcept(false);

		/**
		* Closes the currently open COM port
		*
		* @throws SerialException if the COM port couldn't be closed
		* 
		*/
		void Close() noexcept(false);

		/**
		* Reads from the currently open COM port
		*
		* @param buffer buffer where the read bytes are moved to
		*
		* @param bytes2read number of bytes to read
		* 
		* @return number of bytes that where successfully read
		*
		* @throws SerialException if the read fails
		* 
		*/
		[[nodiscard]] uint32_t Read(uint8_t* buffer, uint32_t bytes2read) noexcept(false);

		/**
		* Writes to the currently open COM port
		*
		* @param buffer buffer for the to be written bytes
		*
		* @param bytes2read number of bytes to write
		*
		* @return number of bytes that where successfully written
		*
		* @throws SerialException if the write fails
		* 
		*/
		[[nodiscard]] uint32_t Write(uint8_t* buffer, uint32_t bytes2write) noexcept(false);

		/**
		* Checks if the COM port is open
		* 
		* @return bool if the port is open or not
		*
		*/
		[[nodiscard]] bool IsOpen() const noexcept;

		/**
		* Number of bytes that are in the serial buffer of the COM port
		*
		* @return number of bytes
		* 
		* @throws SerialException if the port is not open of the status of the port could not be checked
		*
		*/
		[[nodiscard]] uint32_t Available() const noexcept(false);

		/**
		* Waites for the COM port to receive bytes
		*
		*/
		void WaitComm() noexcept;


	private:
		/**
		* Prefixes the portname with \\.\ due to windows weird port names
		*
		* @param port name of the to be prefixed port
		*
		* @return the prefixed portname
		* 
		*/
		[[nodiscard]] static inline std::string prefixPort(const std::string& port) noexcept;

	private:
		/**
		* Sets the baudrate
		*
		* @param baudrate baudrate (self explaining)
		*
		* @throws SerialException if the baudrate couldn't be set
		* 
		*/
		void setBaudrate(uint32_t baudrate) noexcept(false);

		/**
		* Initializes the receive mask for WaitComm
		*
		* @throws SerialException if the receive mask couldn't be set
		* 
		*/
		void initReceiveMask() noexcept(false);

		/**
		* Sets the baudrate
		*
		* @param charCount multiplier for the COM timeouts
		*
		* @throws SerialException if the timeouts couldn't be set
		* 
		*/
		void initTimeouts(uint32_t charCount) noexcept(false);

	public:

		/**
		* List of available ports
		* 
		* @return list of available ports as std::vector
		*
		* @throws SerialException if the port names couldn't be retrieved
		* 
		*/
		[[nodiscard]] static std::vector<std::string> GetPortNames() noexcept(false);

	};
}

#endif // STARTRACKER_SERIAL_SERIAL_H