#include "file_not_found_exception.h"

FileNotFoundException::FileNotFoundException( const std::string &errorMessage )
	: errorMessage{ errorMessage }
{
}

const char* FileNotFoundException::what() const
{
	return this->errorMessage.c_str();
}