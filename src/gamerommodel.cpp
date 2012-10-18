#include "gamerommodel.h"

#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtDebug>
#include "config.h"
#include "gameromitem.h"

GameRomModel::GameRomModel( QObject* parent )
:	ListModel( parent )
{
	setRoleNames( GameRomItem::roleNames() );
	reload();
}

void GameRomModel::reload()
{
	QSqlQuery query( QSqlDatabase::database() );

	QString q = "select emulator, rom, description, last_played, plays, last_play_return_code, favourite from roms";

	QMap< QString, QVariant > boundValues;
	QStringList conditions;

	if ( _filters[ "mostrecent" ] || _filters[ "mostplayed" ] ) {
		if ( _filters[ "mostrecent" ] )
			conditions += "last_played is not null";

		if ( _filters[ "mostplayed" ] )
			conditions += "plays is not null";

		conditions += "plays > 0";
	}

	if ( _filters[ "favourites" ] )
		conditions += "favourite = '1'";

	if ( _filters[ "neverplayed" ] )
		conditions += " plays = 0";

	if ( _emulator != "all" ) {
		conditions += "emulator = :emulator";
		boundValues.insert( ":emulator", _emulator );
	}

	if ( !_searchText.isEmpty() ) {
		conditions += "description like :description";
		boundValues.insert( ":description", '%' + _searchText + '%' );
	}

	if ( !conditions.isEmpty() )
		q += " where " + conditions.join( " and " );

	q += " order by ";

	if ( _filters[ "mostrecent" ] )
		q += "last_played desc, ";

	if ( _filters[ "mostplayed" ] || _filters[ "favourites" ] )
		q += "plays desc, ";

	q += "description asc";

	if ( !query.prepare( q ) ) {
		qWarning() << query.lastError().text();
		return;
	}

	QMapIterator< QString, QVariant > it( boundValues );

	while ( it.hasNext() ) {
		it.next();
		query.bindValue( it.key(), it.value() );
	}

	if ( !query.exec() ) {
		qWarning() << query.lastError().text();
		return;
	}

	clear();

	beginResetModel();

	while ( query.next() ) {
		QString emulator = query.value( 0 ).toString();
		QString rom = query.value( 1 ).toString();
		QString name = query.value( 2 ).toString();

		GameRomItem* item = new GameRomItem( name, emulator, rom );
		item->setLastPlayed( query.value( 3 ).toDateTime() );
		item->setPlayCount( query.value( 4 ).toInt() );
		item->setLastPlayReturnCode( query.value( 5 ).toInt() );
		item->setFavourite( query.value( 6 ).toBool() );
		addItem( item );
	}

	endResetModel();
}

void GameRomModel::setEmulator( const QString& emulator )
{
	_emulator = emulator;
	reload();
}

QString GameRomModel::emulator() const
{
	return _emulator;
}

void GameRomModel::setSearchText( const QString& searchText )
{
	_searchText = searchText;
	reload();
}

QString GameRomModel::searchText() const
{
	return _searchText;
}

void GameRomModel::storeSearchText() const
{
	if ( _searchText.isEmpty() )
		return;

	QSqlQuery query( QSqlDatabase::database() );

	if ( !query.prepare( "insert or ignore into saved_search ( search_text ) values ( :saved_search )" ) ) {
		qWarning() << query.lastError().text();
		return;
	}

	query.bindValue( ":search_text", _searchText );

	if ( !query.exec() ) {
		qWarning() << query.lastError().text();
		return;
	}
}

QString GameRomModel::retrieveSearchText( const QString& partial ) const
{
	if ( partial.isEmpty() )
		return QString();

	QSqlQuery query( QSqlDatabase::database() );

	if ( !query.prepare( "select search_text from saved_search where search_text like :search_text order by search_text asc limit 1" ) ) {
		qWarning() << query.lastError().text();
		return QString();
	}

	query.bindValue( ":search_text", partial + '%' );

	if ( !query.exec() ) {
		qWarning() << query.lastError().text();
		return QString();
	}

	QString searchText;

	if ( query.first() )
		searchText = query.value( 0 ).toString();

	return searchText;
}

int GameRomModel::indexOfRom( const QString& emulator, const QString& rom ) const
{
	const int count = rowCount();

	for ( int i = 0; i < count; ++i ) {
		const QModelIndex idx = index( i );

		if ( data( idx, GameRomItem::EmulatorRole ) == emulator && data( idx, GameRomItem::RomRole ) == rom )
			return i;
	}

	return -1;
}

QString GameRomModel::romAtIndex( int i ) const
{
	return data( index( i ), GameRomItem::RomRole ).toString();
}

void GameRomModel::gameDataChanged( const QString& emulator, const QString& rom )
{
	if ( _emulator != "all" && _emulator != emulator )
		return;

	const int romIndex = indexOfRom( emulator, rom );

	if ( romIndex != -1 )
		reloadData( romIndex );
}

void GameRomModel::reloadData( int romIndex )
{
	GameRomItem* item = dynamic_cast< GameRomItem* >( itemAt( romIndex ) );

	if ( !item )
		return;

	QSqlQuery query( QSqlDatabase::database() );

	if ( !query.prepare( "select last_played, plays, last_play_return_code from roms where emulator = :emulator and rom = :rom" ) ) {
		qWarning() << query.lastError().text();
		return;
	}

	query.bindValue( ":emulator", item->emulator() );
	query.bindValue( ":rom", item->rom() );

	if ( !query.exec() ) {
		qWarning() << query.lastError().text();
		return;
	}

	if ( query.first() ) {
		item->setLastPlayed( query.value( 0 ).toDateTime() );
		item->setPlayCount( query.value( 1 ).toInt() );
		item->setLastPlayReturnCode( query.value( 2 ).toInt() );
	}

	emit dataChanged( index( romIndex ), index( romIndex ) );
}

int GameRomModel::count() const
{
	return rowCount();
}

void GameRomModel::toggleFavourite( const QString& emulator, const QString& rom )
{
	QSqlQuery query( QSqlDatabase::database() );

	if ( !query.prepare( "update roms set favourite = not favourite where emulator = :emulator and rom = :rom" ) ) {
		qWarning() << query.lastError().text();
		return;
	}

	query.bindValue( ":emulator", emulator );
	query.bindValue( ":rom", rom );

	if ( !query.exec() ) {
		qWarning() << query.lastError().text();
		return;
	}

	// Don't really need to do this, but I like to be thorough
	if ( !query.prepare( "select favourite from roms where emulator = :emulator and rom = :rom" ) ) {
		qWarning() << query.lastError().text();
		return;
	}

	query.bindValue( ":emulator", emulator );
	query.bindValue( ":rom", rom );

	if ( !query.exec() ) {
		qWarning() << query.lastError().text();
		return;
	}

	int romIndex = indexOfRom( emulator, rom );

	GameRomItem* item = static_cast< GameRomItem* >( itemAt( romIndex ) );

	if ( query.first() && item )
		item->setFavourite( query.value( 0 ).toBool() );

	emit dataChanged( index( romIndex ), index( romIndex ) );
}

void GameRomModel::setFilter( const QString& filter, bool set )
{
	_filters[ filter ] = set;
	reload();
}

bool GameRomModel::isFilterSet( const QString& filter ) const
{
	return _filters[ filter ];
}
