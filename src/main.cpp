#include "main.h"

#include <QApplication>
#include <QDeclarativeContext>
#include <QDeclarativeEngine>
#include <QDeclarativeView>
#include <QDir>
#include <QFileInfo>
#include <QSqlDatabase>
#include <QStringList>
#include <QtDebug>
#include <QtDeclarative>
#include "config.h"
#include "emulatormodel.h"
#include "gameprocess.h"
#include "gamerommodel.h"
#include "keyboardmap.h"
#include "soundengine.h"
#include "systemmodel.h"
#include "systemprocess.h"

KeyboardMap* keyMap = 0;
SoundEngine* soundEngine = 0;
GameRomModel* gameRomModel = 0;
EmulatorModel* emulatorModel = 0;
SystemModel* systemModel = 0;
SystemProcess* systemProcess = 0;
GameProcess* gameProcess = 0;

int main( int argc, char** argv )
{
	QApplication app( argc, argv );

	Config* config = new Config( &app );

	// If we can't process the command-line or set up the database
	// there's no point continuing
	if ( !processCommandLine() || !setupDatabase() )
		return 0;

	QDeclarativeView view;

	view.connect( view.engine(), SIGNAL(quit()), SLOT(close()) );
	view.setResizeMode( QDeclarativeView::SizeRootObjectToView );

	// Register custom types for access to certail enums from QML
	qmlRegisterType< SoundEngine >( "net.randalflagg.llamaui", 1, 0, "SoundEngine" );
	qmlRegisterType< KeyboardMap >( "net.randalflagg.llamaui", 1, 0, "KeyboardMap" );
	qmlRegisterType< SystemProcess >( "net.randalflagg.llamaui", 1, 0, "SystemProcess" );

	QDeclarativeContext* rootContext = view.rootContext();

	// Initalise the keyboard key/action mapper
	if ( !initKeyboardMap( rootContext ) )
		return 0;

	rootContext->setContextProperty( "config", config );
	initSoundEngine( rootContext );
	initGameMenu( rootContext );
	initEmulatorMenu( rootContext );
	initSystemMenu( rootContext );
	initGameLauncher( rootContext );

	// Create the interface
	QDir dir( Config::instance()->value( "paths", "qmlDir" ).toString() );
	view.setSource( QUrl::fromLocalFile( dir.absoluteFilePath( "main.qml" ) ) );
	view.showFullScreen();

	// Hide the mouse cursor
	app.setOverrideCursor( QCursor( Qt::BlankCursor ) );

	return app.exec();
}

bool processCommandLine()
{
	QStringList args = QCoreApplication::arguments();
	QStringList::iterator start = args.begin();
	QStringList::iterator end = args.end();
	bool configurationProcessed = false;

	for ( QStringList::iterator it = start; it != end; ++it ) {
		if ( ( *it == "-config" || *it == "-c" ) && it != end )
			configurationProcessed = Config::instance()->readConfig( *( ++it ) );
	}

	if ( !configurationProcessed ) {
		QStringList paths = QStringList() << "/etc/llamaui.conf" << "/usr/local/llamaui/etc/llamaui.conf";

		foreach ( QString path, paths ) {
			if ( QFile::exists( path ) && Config::instance()->readConfig( path ) ) {
				configurationProcessed = true;
				break;
			}
		}
	}

	return configurationProcessed;
}

bool setupDatabase()
{
	QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );

	if ( !db.isValid() ) {
		qWarning() << "Cannot create an SQLite database instance.";
		return false;
	}

	db.setDatabaseName( Config::instance()->value( "paths", "databasePath" ).toString() );

	if ( !db.open() ) {
		qWarning() << "Cannot open the database.";
		return false;
	}

	return true;
}

KeyboardMap* buildKeyMap( QObject* parent )
{
	if ( !Config::instance()->hasSection( "keys" ) ) {
		qCritical() << "Configuration does not have a 'keys' section.";
		return 0;
	}

	KeyboardMap* map = new KeyboardMap( parent );

	foreach ( QString actionString, KeyboardMap::actionStrings() ) {
		KeyboardMap::Action action = KeyboardMap::actionFromString( actionString );

		foreach ( QString key, Config::values< QString >( "keys", actionString ) )
			map->mapActionToKey( action, key );
	}

	return map;
}

bool initKeyboardMap( QDeclarativeContext* context )
{
	keyMap = buildKeyMap( qApp );

	if ( !keyMap )
		return false;

	context->setContextProperty( "keyMap", keyMap );
	return true;
}

void initSoundEngine( QDeclarativeContext* context )
{
	soundEngine = new SoundEngine( qApp );
	soundEngine->setSoundDir( Config::instance()->value( "paths", "soundsDir" ).toString() );
	soundEngine->setSoundPlayer( Config::instance()->value( "sound", "player" ).toString() );
	context->setContextProperty( "soundEngine", soundEngine );
}

void initGameMenu( QDeclarativeContext* context )
{
	gameRomModel = new GameRomModel( qApp );

	if ( gameProcess )
		gameRomModel->connect( gameProcess, SIGNAL(gameDataChanged(QString,QString)), SLOT(gameDataChanged(QString,QString)) );

	context->setContextProperty( "gameRomModel", gameRomModel );
}

void initEmulatorMenu( QDeclarativeContext* context )
{
	emulatorModel = new EmulatorModel( qApp );
	context->setContextProperty( "emulatorModel", emulatorModel );
}

void maybeAddSystemMenuItem( SystemProcess::Command command )
{
	if ( !systemModel )
		return;

	if ( SystemProcess::isCommandAvailable( command ) )
		systemModel->addItem( SystemProcess::commandName( command ), command );
}

void initSystemMenu( QDeclarativeContext* context )
{
	systemProcess = new SystemProcess( qApp );
	systemModel = new SystemModel( qApp );

	maybeAddSystemMenuItem( SystemProcess::ShutdownCommand );
	maybeAddSystemMenuItem( SystemProcess::RebootCommand );

	context->setContextProperty( "systemModel", systemModel );
	context->setContextProperty( "systemProcess", systemProcess );
}

void initGameLauncher( QDeclarativeContext* context )
{
	gameProcess = new GameProcess( qApp );

	if ( gameRomModel )
		gameRomModel->connect( gameProcess, SIGNAL(gameDataChanged(QString,QString)), SLOT(gameDataChanged(QString,QString)) );

	context->setContextProperty( "gameProcess", gameProcess );
}
