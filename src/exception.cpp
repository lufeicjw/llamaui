#include "exception.h"

Exception::Exception( QString what )
:	exception()
{
	strncpy( _what, what.toLatin1(), sizeof( _what ) - 1 );
}

Exception::Exception( const char* what )
:	exception()
{
	strncpy( _what, what, sizeof( _what ) - 1 );
}

const char* Exception::what() const throw()
{
	return _what;
}

void throwException( const QString& exception, const QString& file, const QString& function, int line )
{
	Exception e( exception );
	qWarning( "%s: %s: %u: Exception thrown: '%s'.", qPrintable( file ), qPrintable( function ), line, e.what() );
	throw( e );
}
