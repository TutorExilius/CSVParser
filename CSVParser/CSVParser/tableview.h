#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include "definitions.h"

// Forward-Declarations
class CSVParser;

class TableView
{
public:
    TableView( const Point &from, const Point &to, CSVParser *const csvParser );

    RefVec& operator[]( size_t index );
    void save( const std::string &fullFileName ) const;

    // Matrix Operations ---
    Groups groupByColumn( const std::string &columnName ) const;
    size_t getColumnIndex( const std::string &columnName ) const;
    RefVec getColumnNames() const;
    RefVec getColumnValues( const std::string &columnName ) const;
    CountedGroups countGroupsByColumn( const std::string &columnName ) const;
    // ---

    // friends ---
    friend std::ostream &operator<<( std::ostream &out, const TableView &obj )
    {
        out << obj.toString();
        return out;
    }
private:
    // deletes ---
    TableView( const TableView& ) = delete;
    TableView( TableView&& ) = delete;
    TableView& operator=( TableView & ) = delete;
    TableView& operator=( TableView && ) = delete;
    // ---

    // helper-methods ---
    std::string toString() const;
    // ---

    CSVParser *csvParser;
    RefMatrix matrix;
};

#endif // TABLEVIEW_H
