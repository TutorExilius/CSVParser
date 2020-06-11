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


#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <unordered_map>
#include <vector>

class CSVParser
{
public:
	// defines ---
	using Groups = std::unordered_map<std::wstring, std::vector<std::vector<std::wstring>>>;
	using CountGroups = std::unordered_map<std::wstring, int>;
	using Matrix = std::vector<std::vector<std::wstring>>;
	// ---

	// static ---
	static std::wstring extractFileName( const std::wstring &fullFileName );
	static std::wstring extractFilePath( const std::wstring &fullFileName );
	static std::wstring replaceAll( std::wstring str, const std::wstring &from, const std::wstring &to );
	static std::wstring generateRandomString( const size_t stringLength );
	// ---

	CSVParser( const wchar_t seperator = L';' );
	virtual ~CSVParser();

	void parse( const std::wstring &fullFileName );
	void parse( const std::wstring &fullFileName, const wchar_t seperator );

	std::wstring getFileName() const;
	std::wstring getFilePath() const;
	std::wstring getFullFileName() const;
	const Matrix& getCSVMatrix() const;
	std::wstring getCSVOutput() const;
	wchar_t getSeperator() const;

	// Matrix Operations ---
	std::vector<std::wstring> getColumnValues( const std::wstring &columnName ) const;
	Groups groupByColumn( const std::wstring &columnName ) const;
	CountGroups countedGroupsByColumn( const std::wstring &columnName ) const;
	// ---

private:
	// deletes ---
	CSVParser( const CSVParser &obj ) = delete;
	CSVParser( CSVParser &&obj ) = delete;
	CSVParser& operator=( const CSVParser &obj ) = delete;
	CSVParser& operator=( CSVParser &&obj ) = delete;
	// ---

	// helper-methods ---
	std::vector<std::wstring> combineMissplittedColumns( const std::vector<std::wstring> &seperatedColumns );
	void maskColumnNewlines( std::vector<std::wstring> &seperatedColumns );
	void unMaskColumnNewlines( std::vector<std::wstring> &rows );
	bool isValidQuoted( const std::wstring &str );
	size_t count( const std::wstring &str, const wchar_t ch );
	bool isEven( const int &num );
	void setFullFileName( const std::wstring &fullFileName );
	void setFileName( const std::wstring &fileName );
	void setFilePath( const std::wstring &filePath );
	std::vector<std::wstring> createRows( const std::vector<std::wstring> &seperatedColumns );
	void maskColumnSeperators( std::vector<std::wstring> &rows );
	void mapCSVData( const std::vector<std::wstring> &rows );
	size_t getColumnIndex( const std::wstring &columnName ) const;
	// ---

	std::wstring fileName;
	std::wstring filePath;
	wchar_t seperator;
	Matrix csvDataMatrix;
	std::wstring seperatorMaskingStr;
};

#endif // CSVPARSER_H
