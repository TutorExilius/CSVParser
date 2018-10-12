#ifndef FILENOTFOUNDEXCEPTION_H
#define FILENOTFOUNDEXCEPTION_H

#include <exception>
#include <string>

class FileNotFoundException final : public std::exception
{
public:
	explicit FileNotFoundException( const std::string &errorMessage );

	const char* what() const;

private:
	std::string errorMessage;
};

#endif // FILENOTFOUNDEXCEPTION_H