#ifndef KEYBOARDMAP_H
#define KEYBOARDMAP_H

#include <QMultiMap>
#include <QObject>
#include <QSet>
#include <QString>

class KeyboardMap
:	public QObject
{
Q_OBJECT
Q_ENUMS( Action )

public:
	explicit KeyboardMap( QObject* parent = 0 );

	enum Action {
		NoAction,
		UpAction,
		DownAction,
		LeftAction,
		RightAction,
		SelectAction,
		EmulatorMenuAction,
		SearchMenuAction,
		SystemMenuAction,
		ToggleFavouriteAction
	};

	void mapActionToKey( Action action , Qt::Key key );
	void mapActionToKeys( Action action, QList< Qt::Key > keys );
	bool mapActionToKey( KeyboardMap::Action action, const QString& key );

	Q_INVOKABLE int actionForKey( int key ) const;
	Q_INVOKABLE int actionForKeys( const QString& keys ) const;
	Q_INVOKABLE int actionForKeys( QList< int > keys ) const;
	Q_INVOKABLE int queueToAction() const;
	Q_INVOKABLE int queueToActionAndClear();
	Q_INVOKABLE void clearQueue();

	static QString actionToString( Action action );
	static Action actionFromString( const QString& action );
	static QStringList actionStrings();

public slots:
	void addKeyToQueue( int key );

private:
	QSet< Qt::Key > keysToSet( const QString& keys ) const;

private:
	QMultiMap< Action, QSet< Qt::Key > > _keyMap;
	QSet< Qt::Key > _keyQueue;
};

#endif // KEYBOARDMAP_H
