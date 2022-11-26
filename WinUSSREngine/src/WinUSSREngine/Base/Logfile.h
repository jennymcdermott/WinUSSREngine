#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <Windows.h>

namespace WinUSSREngine
{
	typedef struct CVector { float x = 0.0f, y = 0.0f; };

	class Logfile
	{
	private:
		struct LogFileData;
	public:
		Logfile();
		virtual ~Logfile();

		void initLogging() noexcept;

		bool isOpen() const noexcept;

		void open(const std::string& filename);
		void flush() noexcept;
		void close() noexcept;

		void writeToLog(std::string text);    // Write To Log Function (LPCSTR)
		void writeToLog(LPCSTR text);		  // Write To Log Function (LPCSTR)
		void writeToLog(char text);			  // Write To Log Function (char)
		void writeHexToLog(LPCSTR text);	  // Write Hex To Log Function (LPCSTR)
		void writeHexToLog(char text);		  // Write Hex To Log Function (char)

		// Write Coords To Log (use this for coords of like a player/enemy or something)
		void writeCoordsToLog(const CVector& vFloat);
		void writeMousePosToLog(POINT pt); // This is to write the mouse position to a log file

		// Write Vector To Float Function vector(T) (use anything here unless it's for coords)
		template<typename T>
		void writeVectorToLog(std::vector<T> iVector);

		char toUpper(unsigned char keycode) noexcept;
		char toLower(unsigned char keycode) noexcept; // This will automatically convert the keys to lowercase if possible

		bool isShift() noexcept;	 // This will determine if shift is held
		bool isCapslock() noexcept; // This will determine if capslock is enabled

		bool isNumeric(unsigned char keycode) const noexcept;
		bool isAlphabetic(unsigned char keycode) const noexcept;
		bool isSpecialKey(unsigned char keycode) noexcept;

		bool isInitialized() const noexcept { return m_isInitialized; }
		bool keyListened(unsigned char keycode) noexcept; // KeyListened Function (Listens to keys that are Not 0-9 and A-Z)
	private:
		bool m_isInitialized = false;
		struct LogFileData
		{
			time_t now = 0;
			struct tm nowLocal {};
			std::fstream logfile{};
		};
		LogFileData m_Data;
	};
}