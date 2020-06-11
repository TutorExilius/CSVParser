/*

Author:			Tutor Exilius (http://www.exilius.de)

Created:		12.10.2018
Last update:		11.06.2020

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.

*/


#include "csvparser.h"

#include "file_not_found_exception.h"

#include <cmath>
#include <cstring>
#include <fstream>
#include <sstream>

// static ---
std::wstring CSVParser::extractFileName( const std::wstring &fullFileName )
{
	std::wstring tmpFileName = CSVParser::replaceAll( fullFileName, L"/", L"\\" );

	const size_t pos = tmpFileName.find_last_of( L'\\' );

	if( pos != std::wstring::npos && pos + 1 < tmpFileName.size() )
	{
		return std::move( tmpFileName.substr( pos + 1 ) );
	}
	else if( tmpFileName.size() > 0 )
	{
		return tmpFileName;
	}
	else
	{
		return L"";
	}
}

std::wstring CSVParser::extractFilePath( const std::wstring &fullFileName )
{
	std::wstring tmpFileName = CSVParser::replaceAll( fullFileName, L"/", L"\\" );

	const size_t pos = tmpFileName.find_last_of( '\\' );

	if( pos != std::wstring::npos )
	{
		return std::move( tmpFileName.substr( 0, pos + 1 ) );
	}
	else
	{
		return L"";
	}
}

std::wstring CSVParser::replaceAll( std::wstring str, const std::wstring &from, const std::wstring &to )
{
	size_t start_pos = 0;

	while( ( start_pos = str.find( from, start_pos ) ) != std::wstring::npos )
	{
		str.replace( start_pos, from.size(), to );
		start_pos += to.size();
	}

	return std::move( str );
}

std::wstring CSVParser::generateRandomString( const size_t stringLength )
{
	std::wstring maskingChars{ L"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890" };

	std::wstring str( stringLength, L'\0' );

	for( size_t i = 0; i < stringLength; ++i )
	{
		const size_t randIndex = std::rand() % maskingChars.size();
		str[i] = maskingChars[randIndex];
	}

	return str;
}
// ---

CSVParser::CSVParser( const wchar_t seperator )
	: seperator{ seperator }
{
}

CSVParser::~CSVParser() = default;

void CSVParser::parse( const std::wstring &fullFileName )
{
	this->setFullFileName( fullFileName );

	std::wifstream inFile{ this->getFullFileName() };

	if( !inFile )
	{
		const std::wstring w_msg{ this->getFullFileName() };
		const std::string msg{ w_msg.cbegin(), w_msg.cend() };

		throw FileNotFoundException{ "Not found: '" + msg + "'" };
	}

	// read CSV File ---
	std::vector<std::wstring> withSeperatorSeperatedData;
	std::wstring seperatedColum;

	while( std::getline( inFile, seperatedColum, this->seperator ) )
	{
		withSeperatorSeperatedData.push_back( seperatedColum );
	}

	inFile.close();
	// ---

	withSeperatorSeperatedData = CSVParser::combineMissplittedColumns( withSeperatorSeperatedData );

	CSVParser::maskColumnNewlines( withSeperatorSeperatedData );
	CSVParser::maskColumnSeperators( withSeperatorSeperatedData );

	std::vector<std::wstring> rows = CSVParser::createRows( withSeperatorSeperatedData );

	CSVParser::unMaskColumnNewlines( rows );

	this->mapCSVData( rows );
}

void CSVParser::parse( const std::wstring &fullFileName, const wchar_t seperator )
{
	this->seperator = seperator;
	this->parse( fullFileName );
}

std::wstring CSVParser::getFileName() const
{
	return this->fileName;
}

std::wstring CSVParser::getFilePath() const
{
	return this->filePath;
}

std::wstring CSVParser::getFullFileName() const
{
	return this->getFilePath() + this->getFileName();
}

const CSVParser::Matrix& CSVParser::getCSVMatrix() const
{
	return this->csvDataMatrix;
}

std::wstring CSVParser::getCSVOutput() const
{
	std::wstringstream ss;

	for( const auto &xVec : this->csvDataMatrix )
	{
		size_t cntColumn = 0;
		for( const auto yVec : xVec )
		{
			ss << yVec;

			if( ++cntColumn < xVec.size() )
			{
				ss << this->seperator;
			}
		}
		ss << std::endl;
	}

	return ss.str();
}

wchar_t CSVParser::getSeperator() const
{
	return this->seperator;
}

std::vector<std::wstring> CSVParser::getColumn( const std::wstring &columnName ) const
{
	if( this->csvDataMatrix.size() == 0 )
	{
		return std::vector<std::wstring>{};
	}

	const auto itemIndex = [this, &columnName]() -> size_t const {
		const auto columnNames{this->csvDataMatrix.at(0)};

		for( size_t i=0; i< csvDataMatrix.size(); ++i )
		{
			if( columnName == columnNames.at(i) )
			{
				return i;
			}
		}
	} ;

	const size_t columnIndex{ itemIndex() };

	std::vector<std::wstring> columnValues;
		
	for( size_t i = 1; i < this->csvDataMatrix.size(); ++i )
	{
		columnValues.push_back( this->csvDataMatrix.at(i).at(columnIndex) );
	}

	return columnValues;
}

std::vector<std::wstring> CSVParser::combineMissplittedColumns( const std::vector<std::wstring> &seperatedColumns )
{
	std::vector<std::wstring> recombined;
	std::wstring data;

	bool combineMode = false;

	for( const auto column : seperatedColumns )
	{
		bool isValid = CSVParser::isValidQuoted( column );

		if( isValid && !combineMode )
		{
			if( data.size() > 0 )
			{
				data.pop_back();
				recombined.push_back( data );
				data.clear();
			}

			recombined.push_back( column );
		}
		else
		{
			bool finishCombination = false;

			if( combineMode && !isValid )
			{
				combineMode = false;
				finishCombination = true;
			}
			else if( !combineMode )
			{
				combineMode = true;
			}

			data += column + this->seperator;

			if( finishCombination )
			{
				if( data.size() > 0 )
				{
					data.pop_back();
					recombined.push_back( data );
					data.clear();
				}
			}
		}
	}

	if( data.size() > 0 )
	{
		recombined.push_back( data );
	}

	return std::move( recombined );
}

void CSVParser::maskColumnNewlines( std::vector<std::wstring> &seperatedColumns )
{
	for( auto &column : seperatedColumns )
	{
		if( column.size() >= 2 )
		{
			auto startDoubleQuote = column.find_first_of( L'\"' );
			auto endDoubleQuote = column.find_last_of( L'\"' );

			if( startDoubleQuote != std::string::npos && endDoubleQuote != std::string::npos &&
				startDoubleQuote < endDoubleQuote )
			{
				if( CSVParser::count( column, L'\"' ) > 2 )
				{
					endDoubleQuote = column.substr( 0, endDoubleQuote - 1 ).find_last_of( L'\"' );
				}

				std::wstring tmp = column.substr( startDoubleQuote, endDoubleQuote - startDoubleQuote );
				tmp = CSVParser::replaceAll( tmp, L"\n", L"\\n" );

				column = column.replace( startDoubleQuote, endDoubleQuote - startDoubleQuote, tmp );
			}
		}
	}
}

void CSVParser::unMaskColumnNewlines( std::vector<std::wstring> &rows )
{
	for( auto &row : rows )
	{
		row = CSVParser::replaceAll( row, L"\\n", L"\n" );
	}
}

bool CSVParser::isValidQuoted( const std::wstring &str )
{
	size_t cnt = CSVParser::count( str, L'\"' );

	if( CSVParser::isEven( static_cast<int>( cnt ) ) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t CSVParser::count( const std::wstring &str, const wchar_t ch )
{
	size_t cnt = 0;

	for( const auto &c : str )
	{
		if( ch == c )
		{
			++cnt;
		}
	}

	return cnt;
}

bool CSVParser::isEven( const int &num )
{
	return ( num % 2 == 0 );
}

void CSVParser::setFullFileName( const std::wstring &fullFileName )
{
	this->setFileName( CSVParser::extractFileName( fullFileName ) );
	this->setFilePath( CSVParser::extractFilePath( fullFileName ) );
}

void CSVParser::setFileName( const std::wstring &fileName )
{
	this->fileName = fileName;
}

void CSVParser::setFilePath( const std::wstring &filePath )
{
	this->filePath = filePath;
}

std::vector<std::wstring> CSVParser::createRows( const std::vector<std::wstring> &seperatedColumns )
{
	std::vector<std::wstring> rows;
	std::wstringstream ss;

	size_t cnt = 0;
	for( const auto &column : seperatedColumns )
	{
		ss << column;

		if( ++cnt < seperatedColumns.size() )
		{
			ss << this->seperator;
		}
	}

	std::wstring row;
	while( std::getline( ss, row ) )
	{
		rows.push_back( row );
	}

	return std::move( rows );
}

void CSVParser::maskColumnSeperators( std::vector<std::wstring> &rows )
{
	// find masking string, that can be used as seperator-masker ---
	bool foundUniqueString = false;

	size_t generateStrLenght = 3;
	do
	{
		const std::wstring randomString = CSVParser::generateRandomString( generateStrLenght );

		// 
		for( const auto &row : rows )
		{
			if( row.find( randomString ) != std::wstring::npos )
			{
				// found unique string in row, so break loop and generate new string
				break;
			}

			// randomString not found, so randomString is unique!
			this->seperatorMaskingStr = randomString;
			foundUniqueString = true;
		}

	} while( !foundUniqueString );
	// ---

	for( auto &row : rows )
	{
		row = CSVParser::replaceAll( row, std::wstring{ this->seperator }, this->seperatorMaskingStr );
	}
}

void CSVParser::mapCSVData( const std::vector<std::wstring> &rows )
{
	size_t rowsCnt = 0;
	size_t cntColumnTitles = 0;

	for( const auto &row : rows )
	{
		++rowsCnt;

		std::wstringstream ss{ row };
		std::vector<std::wstring> columns;

		std::wstring column;
		while( std::getline( ss, column, this->seperator ) )
		{
			column = CSVParser::replaceAll( column, this->seperatorMaskingStr, std::wstring{ this->seperator } );
			columns.push_back( column );
		}

		if( rowsCnt == 1 )
		{
			cntColumnTitles = columns.size();
		}

		csvDataMatrix.emplace_back( std::move( columns ) );
	}

	if( cntColumnTitles > 0 )
	{
		// resize vectors to column-title length
		for( auto &xVec : this->csvDataMatrix )
		{
			xVec.resize( cntColumnTitles );
		}
	}
	else
	{
		throw "ColumTitles not found!";
	}
}
