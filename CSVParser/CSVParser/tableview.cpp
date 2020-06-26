#include "tableview.h"
#include "csvparser.h"

#include <fstream>
#include <sstream>

TableView::TableView( Matrix &matrix, const Point &from, const Point &to, CSVParser *const csvParser )
: csvParser{ csvParser }
{
    for( size_t i = from.row; i <= to.row; ++i )
    {
        this->matrix.push_back( std::vector<RefString>{} );
        auto &rowVec = this->matrix.at( this->matrix.size() - 1 );

        for( size_t j = from.col; j <= to.col; ++j )
        {
            rowVec.push_back( this->csvParser->refGet( Point{ i, j } ) );
        }
    }
}

RefVec& TableView::operator[]( size_t index )
{
    return this->matrix.at( index );
}

std::string TableView::toString() const
{
    std::stringstream stream;

    for( const auto &row : this->matrix )
    {
        for( const auto &col : row )
        {
            stream << static_cast<std::string>( col ) << this->csvParser->getSeperator();
        }

        stream << "\n";
    }

    return stream.str();
}

void TableView::save( const std::string &fullFileName ) const
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

Groups TableView::groupByColumn( const std::string &columnName ) const
{
    const size_t columnIndex{ this->getColumnIndex( columnName ) };

    Groups groups;

    for( size_t i = 1; i < this->matrix.size(); ++i )
    {
       const RefString key = this->matrix[i][columnIndex];
       groups[key].push_back( this->matrix[i] );
    }

    return groups;
}

size_t TableView::getColumnIndex( const std::string &columnName ) const
{
    const RefVec columnNames( this->matrix.at( 0 ) );
    const auto columnIndexIt{ std::find( columnNames.cbegin(),
                                         columnNames.cend(),
                                         columnName ) };
    const size_t columnIndex{ static_cast<size_t>(
        std::distance( columnNames.cbegin(), columnIndexIt )
        ) };

    return columnIndex;
}

RefVec TableView::getColumnNames() const
{
    return this->matrix.at( 0 );
}

RefVec TableView::getColumnValues( const std::string &columnName ) const
{
    if( this->matrix.size() == 0 )
    {
        return RefVec{};
    }

    const RefVec columnNames( this->matrix[0] );
    const size_t columnIndex{ this->getColumnIndex( columnName ) };

    RefVec columnValues;

    for( size_t i = 1; i < this->matrix.size(); ++i )
    {
        columnValues.push_back( this->matrix[i][columnIndex] );
    }

    return columnValues;
}

CountedGroups TableView::countGroupsByColumn( const std::string &columnName ) const
{
    CountedGroups groupsCount;

    for( const auto &group : this->groupByColumn( columnName ) )
    {
        groupsCount[group.first] += group.second.size();
    }

    return groupsCount;
}
