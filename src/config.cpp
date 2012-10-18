#include "config.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QtDebug>
#include <stdarg.h>

Config* Config::_globalInstance = 0;

Config::Config( QObject* parent )
:	QObject( parent )
{
	if ( _globalInstance ) {
		qWarning() << "Config already instantiated.";
		return;
	}

	_globalInstance = this;
}

Config::Config( const Config& /*other*/ )
{
}

Config& Config::operator=( const Config& /*other*/ )
{
	return *this;
}

Config* Config::instance()
{
	return _globalInstance;
}

bool Config::isEmpty() const
{
	return _reg.isEmpty();
}

bool Config::readConfig( const QString& file )
{
	QFile f( file );

	if ( !f.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
		qWarning() << "Unable to open the configuration file" << file << f.errorString();
		return false;
	}

	QDir currentDir( QDir::current() );
	QFileInfo info( file );
	QDir::setCurrent( info.absoluteDir().absolutePath() );

	QString section;
	QStringList options;

	while ( !f.atEnd() ) {
		QString line = f.readLine().trimmed();

		if ( line.isEmpty() || line[ 0 ] == ';' || line[ 0 ] == '#' )
			continue;

		if ( line.startsWith( "include" ) ) {
			int start = line.indexOf( '(' );
			int end = line.indexOf( ')' );

			if ( start == -1 || end == -1 )
				continue;

			QString file = line.mid( start + 1, end - start - 1 ).trimmed();

			if ( file.startsWith( '\"' ) && file.endsWith( '\"' ) )
				file = file.mid( 1, file.length() - 2 );

			readConfig( file );
			continue;
		}

		if ( line[ 0 ] == '[' ) {
			if ( !options.isEmpty() ) {
				processConfigurationOption( section, options );
				options.clear();
			}

			section = line.mid( 1, line.length() - 2).trimmed();
		} else {
			if ( options.isEmpty() ) {
				options += line.trimmed();
				continue;
			}

			if ( !line.contains( '=' ) ) {
				options += line.trimmed();
				continue;
			}

			processConfigurationOption( section, options );
			options.clear();
			options += line;
		}
	}

	f.close();

	if ( !options.isEmpty() )
		processConfigurationOption( section, options );

	QDir::setCurrent( currentDir.absolutePath() );

	return true;
}

void Config::processConfigurationOption( const QString& section, const QStringList& options )
{
	QStringList bits = options.join( "" ).split( "=" );

	if ( bits.count() < 2 ) {
		qWarning() << "Invalid configuration options" << options << "in section" << section;
		return;
	}

	QString key( bits[ 0 ].trimmed() );

	foreach ( QString value, bits[ 1 ].trimmed().split( ',' ) )
		_reg[ section ].insertMulti( key, value.trimmed() );
}

QVariant Config::value( const QString& section, const QString& key ) const
{
	if ( !_reg.contains( section ) ) {
		qWarning() << "Section" << section << "not found";
		return QVariant();
	}

	if ( !_reg[ section ].contains( key ) ) {
		qWarning() << "Section" << section << "has no key" << key;
		return QVariant();
	}

	return QVariant( parseVariables( _reg[ section ][ key ].toString() ) );
}

QList< QVariant > Config::values( const QString& section, const QString& key ) const
{
	if ( !_reg.contains( section ) ) {
		qWarning() << "Section" << section << "not found";
		return QList< QVariant >();
	}

	if ( !_reg[ section ].contains( key ) ) {
		qWarning() << "Section" << section << "has no key" << key;
		return QList< QVariant >();
	}

	QList< QVariant > values;

	foreach ( QVariant value, _reg[ section ].values( key ) )
		values += QVariant( parseVariables( value.toString() ) );

	return values;
}

void Config::setValue( const QString& section, const QString& key, const QVariant& value )
{
	_reg[ section ][ key ] = value;
}

QStringList Config::sectionNames() const
{
	return _reg.keys();
}

QMap< QString, QVariant > Config::section( const QString& section ) const
{
	if ( !_reg.contains( section ) ) {
		qWarning() << "Section" << section << "not found";
		return QMap< QString, QVariant >();
	}

	return _reg[ section ];
}

bool Config::hasSection( const QString& section ) const
{
	return _reg.contains( section );
}

bool Config::hasValue( const QString& section, const QString& key ) const
{
	if ( !_reg.contains( section ) ) {
		qWarning() << "Section" << section << "not found";
		return false;
	}

	return _reg[ section ].contains( key );
}

QString Config::parseVariables( QString string ) const
{
	QRegExp regExp( "\\$\\{(\\w+)\\}", Qt::CaseSensitive, QRegExp::RegExp2 );

	while ( regExp.indexIn( string, 0 ) != -1 ) {
		QString key = regExp.cap( 1 );
		QString value;

		if ( key == "pwd" )
			value = QDir::currentPath();
		else
			value = Config::value< QString >( "system", key );

		string.replace( regExp.cap( 0 ), value );
	}

	regExp.setPattern( "\\$\\{(\\w+)::(\\w+)\\}" );

	while ( regExp.indexIn( string, 0 ) != -1 ) {
		QString section = regExp.cap( 1 );
		QString key = regExp.cap( 2 );
		QString value = Config::value< QString >( section, key );

		if ( !value.isEmpty() )
			string.replace( regExp.cap( 0 ), value );
	}

	return string;
}
