#include "systemmodel.h"

#include "systemitem.h"

SystemModel::SystemModel( QObject* parent )
:	ListModel( parent )
{
	setRoleNames( SystemItem::roleNames() );
}

void SystemModel::addItem( const QString& name, SystemProcess::Command command )
{
	ListModel::addItem( new SystemItem( name, command ) );
}

SystemProcess::Command SystemModel::commandAtIndex( int i )
{
	return static_cast< SystemProcess::Command >( data( index( i ), SystemItem::CommandRole ).toInt() );
}
