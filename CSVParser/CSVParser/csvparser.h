#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>
#include <vector>

class CSVParser
{
public:
	using Matrix = std::vector<std::vector<std::string>>;

	static std::string extractFileName( const std::string &fullFileName );
	static std::string extractFilePath( const std::string &fullFileName );
	static std::string replaceAll( std::string str, const std::string &from, const std::string &to );
	static std::vector<std::string> combineMissplittedColumns( const std::vector<std::string> &seperatedColumns );
	static void maskColumnNewlines( std::vector<std::string> &seperatedColumns );
	static void unMaskColumnNewlines( std::vector<std::string> &rows );
	static bool isValidQuoted( const std::string &str );
	static size_t count( const std::string &str, const char ch );
	static bool isEven( const int &num );
	static std::string generateRandomString( const size_t stringLength );

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

	void setFullFileName( const std::string &fullFileName );

private:
	// deletes ---
	CSVParser( const CSVParser &obj ) = delete;
	CSVParser( CSVParser &&obj ) = delete;
	CSVParser& operator=( const CSVParser &obj ) = delete;
	CSVParser& operator=( CSVParser &&obj ) = delete;
	// ---

	// helper-methods ---
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