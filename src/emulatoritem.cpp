#include "emulatoritem.h"

#include <QVariant>

EmulatorItem::EmulatorItem()
:	_isCheckable( false ),
	_isChecked( false )
{
}

EmulatorItem::EmulatorItem( const QString& name, const QString& description, const QString& executable, const QString& options )
:	_name( name ),
	_description( description ),
	_executable( executable ),
	_options( options ),
	_isCheckable( false ),
	_isChecked( false )
{
}

EmulatorItem::EmulatorItem( const QString& name, const QString& description, bool isCheckable, bool checked )
:	_name( name ),
	_description( description ),
	_isCheckable( isCheckable ),
	_isChecked( checked )
{
}

QString EmulatorItem::id() const
{
	return _name;
}

QVariant EmulatorItem::data( int role ) const
{
	switch ( role ) {
	case NameRole:
		return _name;

	case DescriptionRole:
		return _description;

	case ExecutableRole:
		return _executable;

	case OptionsRole:
		return _options;

	case IsCheckableRole:
		return _isCheckable;

	case IsCheckedRole:
		return _isChecked;
	}

	return QVariant();
}

QString EmulatorItem::name() const
{
	return _name;
}

QString EmulatorItem::description() const
{
	return _description;
}

QString EmulatorItem::executable() const
{
	return _executable;
}

QString EmulatorItem::options() const
{
	return _options;
}

bool EmulatorItem::isCheckable() const
{
	return _isCheckable;
}

bool EmulatorItem::isChecked() const
{
	return _isChecked;
}

void EmulatorItem::setChecked( bool checked )
{
	_isChecked = checked;
}

bool EmulatorItem::toggle()
{
	_isChecked = !_isChecked;
	return _isChecked;
}

QHash< int, QByteArray > EmulatorItem::roleNames()
{
	static QHash< int, QByteArray > roles;

	if ( roles.isEmpty() ) {
		roles.insert( NameRole, "name" );;
		roles.insert( DescriptionRole, "description" );
		roles.insert( ExecutableRole, "executable" );
		roles.insert( OptionsRole, "options" );
		roles.insert( IsCheckableRole, "isCheckable" );
		roles.insert( IsCheckedRole, "isChecked" );
	}

	return roles;
}
