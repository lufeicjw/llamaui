#include "keyboardmap.h"

#include <QKeySequence>
#include <QStringList>
#include <QtDebug>
#include "qtkeymap.h"

KeyboardMap::KeyboardMap( QObject* parent )
:	QObject( parent )
{
}

QSet< Qt::Key > KeyboardMap::keysToSet( const QString& keys ) const
{
	QSet< Qt::Key > keySet;

	if ( !keys.contains( '+' ) ) {
		// Simple single-key mapping
		int keyCode = keyFromString( keys );

		if ( keyCode != -1 )
			keySet.insert( static_cast< Qt::Key >( keyCode ) );
	} else {
		// Complex multiple-key mapping
		foreach ( QString key, keys.split( '+' ) ) {
			int keyCode = keyFromString( key );

			if ( keyCode == -1 ) {
				keySet.clear();
				break;
			}

			keySet.insert( static_cast< Qt::Key >( keyCode ) );
		}
	}

	return keySet;
}

bool KeyboardMap::mapActionToKey( KeyboardMap::Action action, const QString& key )
{
	QSet< Qt::Key > keys = keysToSet( key );

	if ( keys.isEmpty() )
		return false;

	_keyMap.insertMulti( action, keys );
	return true;
}

void KeyboardMap::mapActionToKey( KeyboardMap::Action action, Qt::Key key )
{
	_keyMap.insertMulti( action, QSet< Qt::Key >() << key );
}

void KeyboardMap::mapActionToKeys( KeyboardMap::Action action, QList< Qt::Key > keys )
{
	_keyMap.insertMulti( action, keys.toSet() );
}

int KeyboardMap::actionForKey( int key ) const
{
	return _keyMap.key( QSet< Qt::Key >() << static_cast< Qt::Key >( key ), NoAction );
}

int KeyboardMap::actionForKeys( QList< int > keys ) const
{
	QSet< Qt::Key > set;

	foreach ( int key, keys )
		set.insert( static_cast< Qt::Key >( key ) );

	return _keyMap.key( set, NoAction );
}

int KeyboardMap::actionForKeys( const QString& keys ) const
{
	return _keyMap.key( keysToSet( keys ) );
}

void KeyboardMap::addKeyToQueue( int key )
{
	_keyQueue.insert( static_cast< Qt::Key >( key ) );
}

int KeyboardMap::queueToAction() const
{
	return _keyMap.key( _keyQueue, NoAction );
}

int KeyboardMap::queueToActionAndClear()
{
	int action = queueToAction();
	clearQueue();

	return action;
}

void KeyboardMap::clearQueue()
{
	_keyQueue.clear();
}

struct {
	KeyboardMap::Action action;
	const char* string;
} ActionMap[] = {
	{ KeyboardMap::NoAction, "noAction" },
	{ KeyboardMap::UpAction, "upAction" },
	{ KeyboardMap::DownAction, "downAction" },
	{ KeyboardMap::LeftAction, "leftAction" },
	{ KeyboardMap::RightAction, "rightAction" },
	{ KeyboardMap::SelectAction, "selectAction" },
	{ KeyboardMap::EmulatorMenuAction, "emulatorMenuAction" },
	{ KeyboardMap::SearchMenuAction, "searchMenuAction" },
	{ KeyboardMap::SystemMenuAction, "systemMenuAction" },
	{ KeyboardMap::ToggleFavouriteAction, "toggleFavouriteAction" },
	{ KeyboardMap::NoAction, 0 }
};

QString KeyboardMap::actionToString( KeyboardMap::Action action )
{
	for ( int i = 0; ActionMap[ i ].string; ++i ) {
		if ( ActionMap[ i ].action == action )
			return ActionMap[ i ].string;
	}

	return QString();
}

KeyboardMap::Action KeyboardMap::actionFromString( const QString& action )
{
	for ( int i = 0; ActionMap[ i ].string; ++i ) {
		if ( action == ActionMap[ i ].string )
			return ActionMap[ i ].action;
	}

	return NoAction;
}

QStringList KeyboardMap::actionStrings()
{
	QStringList actions;

	for ( int i = 0; ActionMap[ i ].string; ++i )
		actions += ActionMap[ i ].string;

	return actions;
}
