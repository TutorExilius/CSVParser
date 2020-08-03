/*
CSVParser

GNU GENERAL PUBLIC LICENSE
https://github.com/TutorExilius/CSVParser

Author:			Tutor Exilius (http://www.exilius.de)

GNU General Public License v3.0

Permissions of this strong copyleft license are conditioned on making available
complete source code of licensed works and modifications, which include larger
works using a licensed work, under the same license.Copyright and license notices
must be preserved.Contributors provide an express grant of patent rights.
*/


#include "csvparser.h"

#include <algorithm>
#include <cmath>
#include <cstring>

#include <filesystem>
namespace fs = std::filesystem;

#include <fstream>
#include <iterator>
#include <random>
#include <sstream>

// static ---
std::string CSVParser::extractFileName( const std::string &fullFileName )
{
    return fs::path( fullFileName ).filename().string();
}

std::string CSVParser::extractFilePath( const std::string &fullFileName )
{
    return fs::path( fullFileName ).parent_path().string();
}

std::string CSVParser::replaceAll( std::string str, const std::string &from, const std::string &to )
{
    size_t start_pos = 0;

    while( ( start_pos = str.find( from, start_pos ) ) != std::string::npos )
    {
        str.replace( start_pos, from.size(), to );
        start_pos += to.size();
    }

    return str;
}

std::string CSVParser::generateRandomString( const size_t &stringLength )
{
    const std::string maskingChars{ "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890" };

    std::random_device rd;      //Will be used to obtain a seed for the random number engine
    std::mt19937 gen( rd() );   //Standard mersenne_twister_engine seeded with rd()

    std::uniform_int_distribution<size_t> distrib( 0, maskingChars.size() - 1 );

    std::string str( stringLength, '\0' );

    for( size_t i = 0; i < stringLength; ++i )
    {
        const size_t randIndex = distrib( gen );
        str[i] = maskingChars[randIndex];
    }

    return str;
}
// ---

CSVParser::CSVParser( const char &seperator )
: seperator{ seperator }
{
}

CSVParser::~CSVParser()
{
    for( const auto &tv : this->tableViews )
    {
        delete tv.second;
    }
}

void CSVParser::parse( const std::string &fullFileName )
{
    this->setFullFileName( fullFileName );

    std::ifstream inFile{ this->getFullFileName() };

    if( !inFile )
    {
        const std::string msg{ this->getFullFileName() };

        throw FileNotFoundException{ "Not found: '" + msg + "'" };
    }

    // read CSV File ---
    std::vector<std::string> withSeperatorSeperatedData;
    std::string seperatedColum;

    while( std::getline( inFile, seperatedColum, this->seperator ) )
    {
        withSeperatorSeperatedData.push_back( seperatedColum );
    }

    inFile.close();
    // ---

    withSeperatorSeperatedData = CSVParser::combineMissplittedColumns( withSeperatorSeperatedData );

    CSVParser::maskColumnNewlines( withSeperatorSeperatedData );
    CSVParser::maskColumnSeperators( withSeperatorSeperatedData );

    std::vector<std::string> rows = CSVParser::createRows( withSeperatorSeperatedData );

    CSVParser::unMaskColumnNewlines( rows );

    this->mapCSVData( rows );
}

void CSVParser::parse( const std::string &fullFileName, const char seperator )
{
    this->seperator = seperator;
    this->parse( fullFileName );
}

void CSVParser::save( const std::string &fullFileName ) const
{
    std::ofstream outFile( fullFileName );

    if( !outFile )
    {
        std::cerr << "Error: Could not save " << fullFileName << std::endl;
        exit( -1 );
    }

    outFile << this->toString();
    outFile.close();
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
    return (fs::path( this->getFilePath() ) / this->getFileName()).string();
}

const Matrix &CSVParser::getCSVMatrix() const
{
    return this->csvDataMatrix;
}

std::string CSVParser::getCSVOutput() const
{
    std::stringstream ss;

    for( const auto &xVec : this->csvDataMatrix )
    {
        size_t cntColumn = 0;
        for( const auto &yVec : xVec )
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

char CSVParser::getSeperator() const
{
    return this->seperator;
}

TableView* CSVParser::getTableView( const std::string &name ) const
{
    auto tableViewIt = this->tableViews.find( name );

    if( tableViewIt != this->tableViews.end() )
    {
        return tableViewIt->second;
    }
    else
    {
        return nullptr;
    }
}

bool CSVParser::isUnique( const std::string &columnName )
{
    const auto pos = std::find( this->csvDataMatrix.at( 0 ).cbegin(),
                                this->csvDataMatrix.at( 0 ).cend(),
                                columnName );

    if( pos != this->csvDataMatrix.at( 0 ).end() )
    {
        const auto double_pos = std::find( pos + 1,
                                           this->csvDataMatrix.at( 0 ).cend(),
                                           columnName );

        const bool is_double = double_pos != this->csvDataMatrix.at( 0 ).end();
        return !is_double;
    }
    else
    {
        throw ColumnNotFound{ columnName + " not found" };
    }
}

void CSVParser::set( const Point &index, const std::string &value )
{
    this->csvDataMatrix.at( index.row ).at( index.col ) = value;
}

std::string CSVParser::get( const Point &index )
{
    return this->csvDataMatrix.at( index.row ).at( index.col );
}

std::string* CSVParser::refGet( const Point &index )
{
    return &(this->csvDataMatrix.at( index.row ).at( index.col ));
}

std::string CSVParser::toString() const
{
    std::stringstream stream;

    for( const auto &row : this->csvDataMatrix )
    {
        for( const auto &col : row )
        {
            stream << col << this->getSeperator();
        }

        stream << "\n";
    }

    return stream.str();
}

std::vector<std::string>& CSVParser::operator[]( size_t index )
{
    return this->csvDataMatrix[index];
}

TableView *CSVParser::createTableView( const std::string &name )
{
    if (this->csvDataMatrix.size() == 0)
        throw OutOfTableIndex( "No rows in csvDataMatrix" );
    else if( this->csvDataMatrix.at(0).size() == 0 )
        throw OutOfTableIndex( "No cols in csvDataMatrix" );

    const size_t rows = csvDataMatrix.size() - 1;
    const size_t columns = csvDataMatrix.at( 1 ).size() - 1;

    const Point from{ 0, 0 };
    const Point to{ rows, columns };

    return this->createTableView( name, from, to );
}

TableView* CSVParser::createTableView( const std::string &name, const Point &from, const Point &to )
{
    TableView *view = new TableView( from, to, this );

    // TODO: check uniqueness!
    this->tableViews[name] = view;

    return view;
}

std::vector<std::string> CSVParser::combineMissplittedColumns( const std::vector<std::string> &seperatedColumns )
{
    std::vector<std::string> recombined;
    std::string data;

    bool combineMode = false;

    for( const auto &column : seperatedColumns )
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

    return recombined;
}

void CSVParser::maskColumnNewlines( std::vector<std::string> &seperatedColumns )
{
    for( auto &column : seperatedColumns )
    {
        if( column.size() >= 2 )
        {
            auto startDoubleQuote = column.find_first_of( '\"' );
            auto endDoubleQuote = column.find_last_of( '\"' );

            if( startDoubleQuote != std::string::npos && endDoubleQuote != std::string::npos &&
                startDoubleQuote < endDoubleQuote )
            {
                if( std::count( column.cbegin(), column.cend(), '\"' ) > 2 )
                {
                    endDoubleQuote = column.substr( 0, endDoubleQuote - 1 ).find_last_of( '\"' );
                }

                std::string tmp = column.substr( startDoubleQuote, endDoubleQuote - startDoubleQuote );
                tmp = CSVParser::replaceAll( tmp, "\n", "\\n" );

                column = column.replace( startDoubleQuote, endDoubleQuote - startDoubleQuote, tmp );
            }
        }
    }
}

void CSVParser::unMaskColumnNewlines( std::vector<std::string> &rows )
{
    for( auto &row : rows )
    {
        row = CSVParser::replaceAll( row, "\\n", "\n" );
    }
}

bool CSVParser::isValidQuoted( const std::string &str )
{
    const size_t cnt = static_cast<size_t>( std::count( str.cbegin(), str.cend(), '\"' ) );
    return (cnt % 2 == 0);
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

std::vector<std::string> CSVParser::createRows( const std::vector<std::string> &seperatedColumns )
{
    std::vector<std::string> rows;
    std::stringstream ss;

    size_t cnt = 0;
    for( const auto &column : seperatedColumns )
    {
        ss << column;

        if( ++cnt < seperatedColumns.size() )
        {
            ss << this->seperator;
        }
    }

    std::string row;
    while( std::getline( ss, row ) )
    {
        rows.push_back( row );
    }

    return rows;
}

void CSVParser::maskColumnSeperators( std::vector<std::string> &rows )
{
    // find masking string, that can be used as seperator-masker ---
    bool foundUniqueString = false;
    size_t generateStrLenght = 3;

    do
    {
        const std::string randomString = CSVParser::generateRandomString( generateStrLenght );

        for( const auto &row : rows )
        {
            if( row.find( randomString ) != std::string::npos )
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
        row = CSVParser::replaceAll( row, std::string{ this->seperator }, this->seperatorMaskingStr );
    }
}

void CSVParser::mapCSVData( const std::vector<std::string> &rows )
{
    for( const auto &row : rows )
    {
        std::stringstream ss{ row };
        std::vector<std::string> columns;

        std::string column;
        while( std::getline( ss, column, this->seperator ) )
        {
            column = CSVParser::replaceAll( column, this->seperatorMaskingStr, std::string{ this->seperator } );
            columns.push_back( column );
        }

        csvDataMatrix.push_back( columns );
    }
}
