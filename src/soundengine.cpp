#include "soundengine.h"

#include <QProcess>
#include <QSound>
#include <QStringList>

SoundEngine::SoundEngine( QObject* parent )
:	QObject( parent )
{
}

void SoundEngine::setSoundDir( const QDir& dir )
{
	_soundDir = dir;
}

void SoundEngine::setSoundPlayer( const QString& player )
{
	_player = player;
}

void SoundEngine::playSound( Sound sound )
{
	QString program = _player;

	if ( program.contains( "%sound%" ) )
		program.replace( QLatin1String( "%sound%" ), _soundDir.absoluteFilePath( soundToString( sound ) ) );
	else
		program += " " + _soundDir.absoluteFilePath( soundToString( sound ) );

	QProcess* process = new QProcess( this );
	connect( process, SIGNAL(finished(int)), process, SLOT(deleteLater()) );
	process->start( program );
}

QString SoundEngine::soundToString( Sound sound )
{
	static QStringList sounds;

	if ( sounds.isEmpty() )
		sounds << "select" << "move" << "swish" << "start" << "toggle_on" << "toggle_off";

	return sounds[ sound ] + ".wav";
}
