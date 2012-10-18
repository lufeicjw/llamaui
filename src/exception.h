#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <exception>
#include <QString>

#define THROW( e )			Ion::Exception::throwException( ( e ), __FILE__, __func__, __LINE__ )

class Exception
:	public std::exception
{
public:
	Exception( QString what );
	Exception( const char* what );
	virtual ~Exception() throw() { }

	virtual const char* what() const throw();

protected:
	char _what[ 1024 ];
};

#if defined(Q_CC_GNU)
void throwException( const QString& exception, const QString& file, const QString& function, int line ) __attribute__((noreturn));
#elif defined(Q_CC_MSVC)
__declspec(noreturn) void throwException( const QString& exception, const QString& file, const QString& function, int line );
#else
void throwException( const QString& exception, const QString& file, const QString& function, int line );
#endif

#endif // EXCEPTION_H
