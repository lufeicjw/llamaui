#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <QDir>
#include <QObject>

class SoundEngine
:	public QObject
{
Q_OBJECT
Q_ENUMS( Sound )

public:
	SoundEngine( QObject* parent = 0 );

	enum Sound {
		SelectSound,
		MoveSound,
		SwishSound,
		StartSound,
		ToggleOnSound,
		ToggleOffSound
	};

	void setSoundDir( const QDir& dir );
	void setSoundPlayer( const QString& player );

public slots:
	void playSound( Sound sound );

private:
	static QString soundToString( Sound sound );

private:
	QString _player;
	QDir _soundDir;
};

#endif // SOUNDENGINE_H
