#ifndef GAMEROMMODEL_H
#define GAMEROMMODEL_H

#include "listmodel.h"

class GameRomModel
:	public ListModel
{
Q_OBJECT
Q_PROPERTY( QString emulator READ emulator WRITE setEmulator )
Q_PROPERTY( QString searchText READ searchText WRITE setSearchText )

signals:
	void emulatorChanged();
	void searchTextChanged();

public:
	explicit GameRomModel( QObject* parent = 0 );
	virtual ~GameRomModel() { }

	QString emulator() const;
	QString searchText() const;

	Q_INVOKABLE int indexOfRom( const QString& emulator, const QString& rom ) const;
	Q_INVOKABLE QString romAtIndex( int i ) const;
	Q_INVOKABLE int count() const;
	Q_INVOKABLE QString retrieveSearchText( const QString& partial ) const;

public slots:
	void reload();
	void setEmulator( const QString& emulator );
	void setSearchText( const QString& searchText );
	void storeSearchText() const;
	void gameDataChanged( const QString& emulator, const QString& rom );
	void toggleFavourite( const QString& emulator, const QString& rom );
	void setFilter( const QString& filter, bool set );
	bool isFilterSet( const QString& filter ) const;

private:
	void reloadData( int romIndex );

private:
	QString _emulator;
	QString _searchText;
	QMap< QString, bool > _filters;
};

#endif // GAMEROMMODEL_H
