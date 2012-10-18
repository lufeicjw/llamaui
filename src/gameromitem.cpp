#include "gameromitem.h"

#include <QVariant>

GameRomItem::GameRomItem()
{
}

GameRomItem::GameRomItem( const QString& name, const QString& emulator, const QString& rom )
:	_name( name ),
	_emulator( emulator ),
	_rom( rom ),
	_favourite( false )
{
}

QString GameRomItem::id() const
{
	return _emulator + ':' + _rom;
}

QVariant GameRomItem::data( int role ) const
{
	switch ( role ) {
	case NameRole:
		return _name;

	case EmulatorRole:
		return _emulator;

	case RomRole:
		return _rom;

	case LastPlayedRole:
		return _lastPlayed;

	case PlayCountRole:
		return _playCount;

	case LastPlayReturnCodeRole:
		return _lastPlayReturnCode;

	case FavouriteRole:
		return _favourite;

	case DetailsRole:
		if ( _playCount == 0 )
			return QLatin1String( "Never played" );

		QString playString;

		if ( _playCount == 1 )
			playString = "Played once";
		else if ( _playCount == 2 )
			playString = "Played twice";
		else
			playString = QString( "Played %1 time%2" ).arg( _playCount ).arg( _playCount == 1 ? "" : "s" );

		return QString( "%1 and last played on %2 at %3" ).arg( playString ).arg( _lastPlayed.toString( "dd/MM/yyyy" ) ).arg( _lastPlayed.toString( "hh:mm" ) );
	}

	return QVariant();
}

QHash< int, QByteArray > GameRomItem::roleNames()
{
	QHash< int, QByteArray > hash;
	hash.insert( NameRole, "name" );
	hash.insert( EmulatorRole, "emulator" );
	hash.insert( RomRole, "rom" );
	hash.insert( LastPlayedRole, "lastPlayed" );
	hash.insert( PlayCountRole, "playCount" );
	hash.insert( LastPlayReturnCodeRole, "lastPlayReturnCode" );
	hash.insert( FavouriteRole, "favourite" );
	hash.insert( DetailsRole, "details" );

	return hash;
}

QString GameRomItem::name() const
{
	return _name;
}

QString GameRomItem::emulator() const
{
	return _emulator;
}

QString GameRomItem::rom() const
{
	return _rom;
}

QDateTime GameRomItem::lastPlayed() const
{
	return _lastPlayed;
}

void GameRomItem::setLastPlayed( const QDateTime& lastPlayed )
{
	_lastPlayed = lastPlayed;
}

int GameRomItem::playCount() const
{
	return _playCount;
}

void GameRomItem::setPlayCount( int playCount )
{
	_playCount = playCount;
}

void GameRomItem::incrementPlayCount()
{
	++_playCount;
}

int GameRomItem::lastPlayReturnCode() const
{
	return _lastPlayReturnCode;
}

void GameRomItem::setLastPlayReturnCode( int returnCode )
{
	_lastPlayReturnCode = returnCode;
}

bool GameRomItem::isFavourite() const
{
	return _favourite;
}

void GameRomItem::setFavourite( bool favourite )
{
	_favourite = favourite;
}
