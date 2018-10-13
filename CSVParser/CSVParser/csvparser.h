/*

Author:			Tutor Exilius (http://www.exilius.de)

Created:		12.10.2018
Last update:	13.10.2018

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.

*/


#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <vector>

class CSVParser
{
public:
	// defines ---
	using Matrix = std::vector<std::vector<std::string>>;
	// ---

	// static ---
	static std::string extractFileName( const std::string &fullFileName );
	static std::string extractFilePath( const std::string &fullFileName );
	static std::string replaceAll( std::string str, const std::string &from, const std::string &to );
	static std::string generateRandomString( const size_t stringLength );
	// ---

	CSVParser( const char seperator = ';' );
	virtual ~CSVParser();

	void parse( const std::string &fullFileName );
	void parse( const std::string &fullFileName, const char seperator );

	std::string getFileName() const;
	std::string getFilePath() const;
	std::string getFullFileName() const;
	const Matrix& getCSVMatrix() const;
	std::string getCSVOutput() const;
	char getSeperator() const;

private:
	// deletes ---
	CSVParser( const CSVParser &obj ) = delete;
	CSVParser( CSVParser &&obj ) = delete;
	CSVParser& operator=( const CSVParser &obj ) = delete;
	CSVParser& operator=( CSVParser &&obj ) = delete;
	// ---

	// helper-methods ---
	std::vector<std::string> combineMissplittedColumns( const std::vector<std::string> &seperatedColumns );
	void maskColumnNewlines( std::vector<std::string> &seperatedColumns );
	void unMaskColumnNewlines( std::vector<std::string> &rows );
	bool isValidQuoted( const std::string &str );
	size_t count( const std::string &str, const char ch );
	bool isEven( const int &num );
	void setFullFileName( const std::string &fullFileName );
	void setFileName( const std::string &fileName );
	void setFilePath( const std::string &filePath );
	std::vector<std::string> createRows( const std::vector<std::string> &seperatedColumns );
	void maskColumnSeperators( std::vector<std::string> &rows );
	void mapCSVData( const std::vector<std::string> &rows );
	// ---

	std::string fileName;
	std::string filePath;
	char seperator;
	Matrix csvDataMatrix;
	std::string seperatorMaskingStr;
};

#endif // CSVPARSER_H