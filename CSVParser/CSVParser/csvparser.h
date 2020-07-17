/*

Author:			Tutor Exilius (http://www.exilius.de)

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.

*/


#ifndef CSVPARSER_H
#define CSVPARSER_H

#include <stdexcept>
#include <unordered_map>

#include "tableview.h"

// EXCEPTIONS ---
class ColumnNotFound : virtual public std::runtime_error
{
public:
    ColumnNotFound( const std::string &msg )
    : std::runtime_error( msg )
    {
    }
};

class FileNotFoundException final : virtual public std::runtime_error
{
public:
    FileNotFoundException( const std::string &msg )
    : std::runtime_error( msg )
    {
    }
};

class RowSizeLimitException : virtual public std::runtime_error
{
public:
    RowSizeLimitException( const std::string &msg )
        : std::runtime_error( msg )
    {
    }
};
// ---


class CSVParser final
{
public:
    // static ---
    static std::string extractFileName( const std::string &fullFileName );
    static std::string extractFilePath( const std::string &fullFileName );
    static std::string replaceAll( std::string str, const std::string &from, const std::string &to );
    static std::string generateRandomString( const size_t &stringLength );
    // ---

    CSVParser( const char &seperator = ',' );
    ~CSVParser();

    void parse( const std::string &fullFileName );
    void parse( const std::string &fullFileName, const char seperator );
    void save( const std::string &fullFileName ) const;

    std::string getFileName() const;
    std::string getFilePath() const;
    std::string getFullFileName() const;
    const Matrix &getCSVMatrix() const;
    std::string getCSVOutput() const;
    char getSeperator() const;
    TableView* getTableView( const std::string &name ) const;
    bool isUnique( const std::string &columnName );
    void set( const Point &index, const std::string &value );
    std::string get( const Point &index );
    std::string* refGet( const Point &index );
    TableView* createTableView( const std::string &name, const Point &from, const Point &to );
    std::vector<std::string>& operator[]( size_t index );

    // friends ---
    friend std::ostream &operator<<( std::ostream &out, const CSVParser &obj )
    {
        out << obj.toString();
        return out;
    }
private:
    // deletes ---
    CSVParser( const CSVParser &obj ) = delete;
    CSVParser( CSVParser &&obj ) = delete;
    CSVParser& operator=( const CSVParser &obj ) = delete;
    CSVParser& operator=( CSVParser &&obj ) = delete;
    // ---

    // helper-methods ---
    std::string toString() const;
    std::vector<std::string> combineMissplittedColumns( const std::vector<std::string> &seperatedColumns );
    void maskColumnNewlines( std::vector<std::string> &seperatedColumns );
    void unMaskColumnNewlines( std::vector<std::string> &rows );
    bool isValidQuoted( const std::string &str );
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
    // Pair: (unique name of table view, TableView)
    std::unordered_map<std::string, TableView*> tableViews;
};

#endif // CSVPARSER_H
