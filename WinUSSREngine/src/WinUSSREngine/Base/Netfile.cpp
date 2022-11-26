#include "NetFile.h"


namespace WinUSSREngine
{
	
	void NetFile::readfile(const std::string& file) noexcept(true)
	{
		std::fstream stream(file, std::fstream::in | std::fstream::binary);
		m_rawFileData = { std::istreambuf_iterator<char>(stream),  std::istreambuf_iterator<char>() };
	}

	void NetFile::writefile(const std::string& file) noexcept(true)
	{
		std::fstream filestr(file, std::fstream::out | std::fstream::binary);
		filestr.write(m_rawFileData.data(), m_rawFileData.size());
	}

	void NetFile::writeraw(const std::string& bytes) noexcept(true)
	{
		if (!m_rawFileData.empty())return;
		for (auto byte : bytes)
			m_rawFileData.push_back(byte);
	}

	void NetFile::writeraw(const std::vector<char>& bytes) noexcept(true)
	{
		if (!m_rawFileData.empty())return;
		for (auto byte : bytes)
			m_rawFileData.push_back(byte);
	}


	std::vector<char> NetFile::raw() noexcept(true) { return m_rawFileData; }

	const int NetFile::size() const noexcept(true) { return m_rawFileData.size(); }
	
	std::string NetFile::string() noexcept(true)
	{
		std::stringstream Stream;
		for (auto bytes : m_rawFileData)Stream << bytes;
		return Stream.str();
	}

	void NetFile::flush() noexcept(true) { m_rawFileData = std::vector<char>(); }
}