#pragma once
#include <exception>
#include <string>

class ChiliException : public std::exception
{
public:
	ChiliException(int line, const char* file) noexcept;
};