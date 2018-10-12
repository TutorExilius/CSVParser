#include "csvparser.h"

#include "file_not_found_exception.h"

#include <fstream>

// static ---
std::string CSVParser::extractFileName( const std::string &fullFileName )
{
	std::string tmpFileName = CSVParser::replaceAll( fullFileName, "/", "\\" );

	const size_t pos = tmpFileName.find_last_of( '\\' );

	if( pos != std::string::npos && pos+1 < tmpFileName.size() )
	{
		return std::move( tmpFileName.substr( pos+1 ) );
	}
	else
	{
		return "";
	}
}

std::string CSVParser::extractFilePath( const std::string &fullFileName )
{
	std::string tmpFileName = CSVParser::replaceAll( fullFileName, "/", "\\" );

	const size_t pos = tmpFileName.find_last_of( '\\' );
	
	if( pos != std::string::npos )
	{
		return std::move( tmpFileName.substr( 0, pos+1 ) );
	}
	else
	{
		return "";
	}
}

std::string CSVParser::replaceAll( std::string str, const std::string &from, const std::string &to )
{
	size_t start_pos = 0;

	while( (start_pos = str.find( from, start_pos ) ) != std::string::npos )
	{
		str.replace( start_pos, from.size(), to );
		start_pos += to.size();
	}

	return std::move( str );
}
// ---

CSVParser::CSVParser( const char seperator )
: seperator{ seperator }
{

}

CSVParser::~CSVParser() = default;

bool CSVParser::parse( const std::string &fullFileName )
{
	this->setFullFileName( fullFileName );

	bool parseSucceeded = false;

	std::ifstream inFile{ this->getFullFileName() };

	if( !inFile )
	{
		throw FileNotFoundException{ "Not found: '" + this->getFullFileName() + "'" };
	}

	// TODO:

	return parseSucceeded;
}

bool CSVParser::parse( const std::string &fullFileName, const char seperator )
{
	this->seperator = seperator;

	return this->parse( fullFileName );
}

std::string CSVParser::getFileName() const
{
	return this->fileName;
}

std::string CSVParser::getFilePath() const
{
	return this->filePath;
}

std::string CSVParser::getFullFileName() const
{
	return this->filePath + this->fileName;
}

void CSVParser::setFullFileName( const std::string &fullFileName )
{
	this->setFileName( CSVParser::extractFileName( fullFileName ) );
	this->setFilePath( CSVParser::extractFilePath( fullFileName ) );
}

void CSVParser::setFileName( const std::string &fileName )
{
	this->fileName = fileName;
}

void CSVParser::setFilePath( const std::string &filePath )
{
	this->filePath = filePath;
}