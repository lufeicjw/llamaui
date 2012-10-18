#include "systemitem.h"

SystemItem::SystemItem( const QString& name, SystemProcess::Command command )
:	ModelItem(),
	_name( name ),
	_command( command )
{
}

QString SystemItem::id() const
{
	return QString::number( _command );
}

QVariant SystemItem::data( int role ) const
{
	switch ( role ) {
	case NameRole:
		return _name;

	case CommandRole:
		return _command;
	}

	return QVariant();
}

QHash< int, QByteArray > SystemItem::roleNames()
{
	QHash< int, QByteArray > hash;
	hash.insert( NameRole, "name" );
	hash.insert( CommandRole, "command" );

	return hash;
}
