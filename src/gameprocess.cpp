#include "gameprocess.h"

#include <QApplication>
#include <QDir>
#include <QEvent>
#include <QMutex>
#include <QProcess>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QtDebug>
#include <QTimerEvent>
#include "config.h"
#include "exception.h"

GameProcess::GameProcess( QObject *parent )
:	QObject( parent ),
	_attractStartTimerId( 0 ),
	_attractStopTimerId( 0 ),
	_gameProcess( 0 )
{
	qApp->installEventFilter( this );
	resetAttractTimer();
}

void GameProcess::startGame( const QString& emulator, const QString& rom )
{
	if ( _attractStartTimerId != 0 ) {
		killTimer( _attractStartTimerId );
		_attractStartTimerId = 0;
	}

	setLastPlayed( emulator, rom );

	QString romOptions;
	Config* config = Config::instance();

	if ( config->hasSection( rom ) )
		romOptions = config->value( rom, "romOptions" ).toString();

	QString section = "emulator_" + emulator;

	if ( !config->hasSection( section ) )
		return;

	QString program = config->value( section, "executable" ).toString();

	if ( program.isEmpty() )
		return;

	program += " ";

	if ( config->hasValue( section, "options" ) )
		program += config->value( section, "options" ).toString();
	else
		program += "%game%";

	program.replace( "%romOptions%", romOptions );
	program.replace( "%game%", rom );

	_lastEmulatorPlayed = emulator;
	_lastRomPlayed = rom;

	qDebug() << "Starting" << program;

	_gameProcess = new QProcess( this );
	connect( _gameProcess, SIGNAL(finished(int)), this, SLOT(gameFinished(int)) );
	connect( _gameProcess, SIGNAL(destroyed()), this, SLOT(gameProcessDestroyed()) );
	_gameProcess->start( program );
}

void GameProcess::startGame( const QString& rom )
{
	static QMutex mut;

	if ( !mut.tryLock() )
		return;

	try {
		QSqlQuery query( QSqlDatabase::database() );

		if ( !query.prepare( "select emulator from roms where rom = :rom" ) )
			throw Exception( query.lastError().text() );

		query.bindValue( ":rom", rom );

		if ( !query.exec() )
			throw Exception( query.lastError().text() );

		if ( !query.first() )
			throw Exception( "No matching emulator found for rom " + rom );

		startGame( query.value( 0 ).toString(), rom );
	}
	catch ( Exception e ) {
		qWarning() << e.what();
	}

	mut.unlock();
}

void GameProcess::setLastPlayed( const QString& emulator, const QString& rom )
{
	QSqlQuery query( QSqlDatabase::database() );

	if ( !query.prepare( "update roms set last_played = CURRENT_TIMESTAMP where emulator = :emulator and rom = :rom" ) ) {
		qWarning() << query.lastError().text();
		return;
	}

	query.bindValue( ":emulator", emulator );
	query.bindValue( ":rom", rom );

	if ( !query.exec() ) {
		qWarning() << query.lastError().text();
		return;
	}

	emit gameDataChanged( emulator, rom );
}

void GameProcess::updatePlayCountWithReturnCode( const QString& emulator, const QString& rom, int returnCode )
{
	QSqlQuery query( QSqlDatabase::database() );

	if ( returnCode == 0 ) {
		if ( !query.prepare( "update roms set last_played = datetime( 'now', 'localtime' ), plays = plays + 1, last_play_return_code = :return_code where emulator = :emulator and rom = :rom" ) ) {
			qWarning() << query.lastError().text();
			return;
		}
	} else {
		if ( !query.prepare( "update roms set last_played = datetime( 'now', 'localtime' ), last_play_return_code = :return_code where emulator = :emulator and rom = :rom" ) ) {
			qWarning() << query.lastError().text();
			return;
		}
	}

	query.bindValue( ":emulator", emulator );
	query.bindValue( ":rom", rom );
	query.bindValue( ":return_code", returnCode );

	if ( !query.exec() ) {
		qWarning() << query.lastError().text();
		return;
	}

	emit gameDataChanged( emulator, rom );
}

QString GameProcess::lastGamePlayed( const QString& emulator ) const
{
	QSqlQuery query( QSqlDatabase::database() );

	if ( !query.prepare( "select rom from roms where emulator = :emulator order by last_played desc limit 1" ) ) {
		qWarning() << query.lastError().text();
		return QString();
	}

	query.bindValue( ":emulator", emulator );

	if ( !query.exec() ) {
		qWarning() << query.lastError().text();
		return QString();
	}

	if ( query.first() )
		return query.value( 0 ).toString();

	return QString();
}

bool GameProcess::gameRunning() const
{
	return _gameProcess != 0;
}

bool GameProcess::eventFilter( QObject* obj, QEvent* event )
{
	if ( event->type() == QEvent::KeyPress )
		resetAttractTimer();

	return QObject::eventFilter( obj, event );
}

void GameProcess::resetAttractTimer()
{
	if ( gameRunning() )
		return;

	int timeout = Config::instance()->value( "attract", "timeout" ).toInt() * 1000;

	if ( timeout <= 0 )
		return;

	if ( _attractStartTimerId != 0 )
		killTimer( _attractStartTimerId );

	_attractStartTimerId = startTimer( timeout );
}

void GameProcess::timerEvent( QTimerEvent* event )
{
	if ( event->timerId() == _attractStartTimerId )
		startAttractMode();
	else if ( event->timerId() == _attractStopTimerId )
		stopAttractMode();
}

void GameProcess::startAttractMode()
{
	QStringList gameList = Config::instance()->values< QString >( "attract", "gameList" );

	if ( gameList.isEmpty() ) {
		qWarning() << "Attract game list is empty";
		return;
	}

	QString attractGame = gameList.at( rand() % gameList.count() );
	QStringList parts = attractGame.trimmed().split( ':' );

	QString rom, emulator;

	if ( parts.count() == 1 ) {
		rom = parts[ 0 ].trimmed();
	} else if ( parts.count() == 2 ) {
		emulator = parts[ 0 ].trimmed();
		rom = parts[ 1 ].trimmed();
	} else {
		qWarning() << "Invalid attract game specification" << attractGame;
		return;
	}

	if ( rom.isEmpty() ) {
		qWarning() << "Invalid attract game specification" << attractGame;
		return;
	}

	if ( emulator.isEmpty() ) {
		qDebug() << "Starting attract mode with" << rom;
		startGame( rom );
	} else {
		qDebug() << "Starting attract mode with" << rom << emulator;
		startGame( emulator, rom );
	}

	if ( gameRunning() ) {
		if ( _attractStopTimerId != 0 )
			killTimer( _attractStopTimerId );

		int duration = Config::instance()->value( "attract", "duration" ).toInt() * 1000;

		if ( duration > 0 )
			_attractStopTimerId = startTimer( duration );
	}
}

void GameProcess::stopAttractMode()
{
	if ( !_gameProcess )
		return;

	qDebug() << "Stopping attract game";
	killTimer( _attractStopTimerId );
	_attractStopTimerId = 0;
	_gameProcess->terminate();
}

void GameProcess::gameFinished( int returnCode )
{
	qDebug() << _lastEmulatorPlayed << _lastRomPlayed << "finished with return code" << returnCode;
	updatePlayCountWithReturnCode( _lastEmulatorPlayed, _lastRomPlayed, returnCode );
	_lastEmulatorPlayed.clear();
	_lastRomPlayed.clear();
	_gameProcess->deleteLater();
}

void GameProcess::gameProcessDestroyed()
{
	qDebug() << "Game process destroyed";
	_gameProcess = 0;
	resetAttractTimer();
}
