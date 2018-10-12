#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <string>

class CSVParser
{
public:
	static std::string extractFileName( const std::string &fullFileName );
	static std::string extractFilePath( const std::string &fullFileName );
	static std::string replaceAll( std::string str, const std::string &from, const std::string &to );

	CSVParser( const char seperator = ';' );
	virtual ~CSVParser();

	bool parse( const std::string &fullFileName );
	bool parse( const std::string &fullFileName, const char seperator );

	std::string getFileName() const;
	std::string getFilePath() const;
	std::string getFullFileName() const;

	void setFullFileName( const std::string &fullFileName );

private:
	CSVParser( const CSVParser &obj ) = delete;
	CSVParser( CSVParser &&obj ) = delete;
	CSVParser& operator=( const CSVParser &obj ) = delete;
	CSVParser& operator=( CSVParser &&obj ) = delete;

	void setFileName( const std::string &fileName );
	void setFilePath( const std::string &filePath );

	std::string fileName;
	std::string filePath;
	char seperator;
};

#endif // CSVPARSER_H