#ifndef GAMEROMITEM_H
#define GAMEROMITEM_H

#include <QDateTime>
#include "modelitem.h"

class GameRomItem
:	public ModelItem
{
public:
	GameRomItem();
	GameRomItem( const QString& name, const QString& emulator, const QString& rom );

	enum Role {
		NameRole = Qt::UserRole + 1,
		EmulatorRole,
		RomRole,
		LastPlayedRole,
		PlayCountRole,
		LastPlayReturnCodeRole,
		FavouriteRole,
		DetailsRole
	};

	virtual QString id() const;
	virtual QVariant data( int role ) const;

	QString name() const;
	QString emulator() const;
	QString rom() const;
	QDateTime lastPlayed() const;
	void setLastPlayed( const QDateTime& lastPlayed );
	int playCount() const;
	void setPlayCount( int playCount );
	void incrementPlayCount();
	int lastPlayReturnCode() const;
	void setLastPlayReturnCode( int returnCode );
	bool isFavourite() const;
	void setFavourite( bool favourite );

	static QHash< int, QByteArray > roleNames();

private:
	QString _name;
	QString _emulator;
	QString _rom;
	QDateTime _lastPlayed;
	int _playCount;
	int _lastPlayReturnCode;
	bool _favourite;
};

#endif // GAMEROMITEM_H
