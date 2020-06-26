#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include "definitions.h"

// Forward-Declarations
class CSVParser;

class TableView
{
public:
    TableView( Matrix &matrix, const Point &from, const Point &to, CSVParser *const csvParser );

    void setMatrix( const RefMatrix &matrix );
    void set( Point index, const std::string &value );
    const std::string get( Point index ) const;

    RefVec& operator[]( size_t index );
    std::string toString() const;
    void save( const std::string &fullFileName ) const;

    // Matrix Operations ---
    Groups groupByColumn( const std::string &columnName ) const;
    size_t getColumnIndex( const std::string &columnName ) const;
    RefVec getColumnNames() const;
    RefVec getColumnValues( const std::string &columnName ) const;
    CountedGroups countGroupsByColumn( const std::string &columnName ) const;
    // ---

private:
    // deletes ---
    TableView( const TableView& ) = delete;
    TableView( TableView&& ) = delete;
    TableView& operator=( TableView & ) = delete;
    TableView& operator=( TableView && ) = delete;
    // ---

    CSVParser *csvParser;
    RefMatrix matrix;
};

#endif // TABLEVIEW_H