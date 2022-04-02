#ifndef STARTRACKER_SERIAL_SERIAL_H
#define STARTRACKER_SERIAL_SERIAL_H

#include <StarTracker/Core/Core.hpp>
#include <string>
#include <vector>

/**
*	For reference, see https://docs.microsoft.com/en-us/previous-versions/ff802693(v=msdn.10)?redirectedfrom=MSDN
*/

namespace StarTracker::Utils::Serial {

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
		std::string fileName;

	public:
		SerialPort() noexcept;
		~SerialPort() noexcept(false);

		/**
		* @brief Opens the specified COM port
		*
		* @param port name of the COM port
		* 
		* @param baudRate baudRate (self explaining)
		*
		* @throws SerialException if the COM port does not exist or could not be opened
		* 
		*/
		void Open(const std::string& port, uint32_t baudRate) noexcept(false);

		/**
		* @brief Closes the currently open COM port
		*
		* @throws SerialException if the COM port could not be closed
		* 
		*/
		void Close() noexcept(false);

		/**
		* @brief Reads from the currently open COM port
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
		[[nodiscard]] uint32_t Read(uint8_t* buffer, uint32_t bytes2read, bool waitForRx = true) noexcept(false);

		/**
		* @brief Writes to the currently open COM port
		*
		* @param buffer buffer for the to be written bytes
		*
		* @param bytes2write number of bytes to write
		*
		* @return number of bytes that where successfully written
		*
		* @throws SerialException if the write fails
		* 
		*/
		[[nodiscard]] uint32_t Write(uint8_t* buffer, uint32_t bytes2write) noexcept(false);

		/**
		* @brief Checks if the COM port is open
		* 
		* @return bool if the port is open or not
		*
		*/
		[[nodiscard]] bool IsOpen() noexcept;

		/**
		* @brief Checks if the COM port still exists
		* 
		* @return bool if the COM port exists or not
		*
		*/
		[[nodiscard]] bool IsGood() noexcept;

		/**
		* @brief Number of bytes that are in the serial buffer of the COM port
		*
		* @return number of bytes
		* 
		* @throws SerialException if the port is not open of the status of the port could not be checked
		*
		*/
		[[nodiscard]] uint32_t Available() noexcept(false);

		/**
		* @brief Waits for the COM port to receive bytes
		*
		*/
		void WaitComm() noexcept;


	private:
		/**
		* @brief Prefixes the portName with \\.\ due to windows weird port names
		*
		* @param port name of the to be prefixed port
		*
		* @return the prefixed portName
		* 
		*/
		[[nodiscard]] static inline std::string prefixPort(const std::string& port) noexcept;

	private:
		/**
		* @brief Sets the baudRate
		*
		* @param baudRate baudRate (self explaining)
		*
		* @throws SerialException if the baudRate could not be set
		* 
		*/
		void setBaudRate(uint32_t baudRate) noexcept(false);

		/**
		* @brief Initializes the receive mask for WaitComm
		*
		* @throws SerialException if the receive mask could not be set
		* 
		*/
		void initReceiveMask() noexcept(false);

		/**
		* @brief Sets the baudRate
		*
		* @throws SerialException if the timeouts baudRate be set
		* 
		*/
		void initTimeouts() noexcept(false);

	public:

		/**
		* @brief List of available ports
		* 
		* @return list of available ports as std::vector
		*
		* @throws SerialException if the port names could not be retrieved
		* 
		*/
		[[nodiscard]] static std::vector<std::string> GetPortNames() noexcept(false);

	};
}

#endif // STARTRACKER_SERIAL_SERIAL_H