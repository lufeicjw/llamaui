#include "emulatormodel.h"

#include <QStringList>
#include "config.h"
#include "emulatoritem.h"

#include <QtDebug>

EmulatorModel::EmulatorModel( QObject* parent )
:	ListModel( parent)
{
	setRoleNames( EmulatorItem::roleNames() );
	reload();
}

void EmulatorModel::reload()
{
	Config* config = Config::instance();

	clear();

	beginResetModel();

	addItem( new EmulatorItem( "all", "All emulators" ) );

	foreach ( QString section, config->sectionNames() ) {
		if ( !section.startsWith( "emulator_" ) )
			continue;

		const QString name = section.mid( section.indexOf( '_' ) + 1 );

		if ( name.isEmpty() )
			continue;

		const QString description = config->value( section, "description" ).toString();
		const QString executable = config->value( section, "executable" ).toString();
		const QString options = config->value( section, "options" ).toString();

		addItem( new EmulatorItem( name, description, executable, options ) );
	}

	addItem( new EmulatorItem( "favourites", "Favourites", true, false ) );
	addItem( new EmulatorItem( "mostrecent", "Recently played", true, false ) );
	addItem( new EmulatorItem( "mostplayed", "Most played", true, false ) );
	addItem( new EmulatorItem( "neverplayed", "Never played", true, false ) );

	endResetModel();
}

int EmulatorModel::indexOfEmulator( const QString& name ) const
{
	int count = rowCount();

	for ( int i = 0; i < count; ++i ) {
		if ( data( index( i ), EmulatorItem::NameRole ) == name )
			return i;
	}

	return -1;
}

QString EmulatorModel::emulatorAtIndex( int i ) const
{
	return data( index( i ), EmulatorItem::NameRole ).toString();
}

QString EmulatorModel::descriptionAtIndex( int i ) const
{
	return data( index( i ), EmulatorItem::DescriptionRole ).toString();
}

void EmulatorModel::toggleItemAtIndex( int i )
{
	EmulatorItem* item = static_cast< EmulatorItem* >( itemAt( i ) );

	if ( !item )
		return;

	item->toggle();

	const QModelIndex idx = index( i );
	emit dataChanged( idx, idx );
}
