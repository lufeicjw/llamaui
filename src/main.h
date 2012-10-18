#ifndef MAIN_H
#define MAIN_H

class QDeclarativeContext;
class QObject;
class KeyboardMap;

bool processCommandLine();
bool setupDatabase();
KeyboardMap *buildKeyMap( QObject* parent );
bool initKeyboardMap( QDeclarativeContext* context );
void initSoundEngine( QDeclarativeContext* context );
void initGameMenu( QDeclarativeContext* context );
void initEmulatorMenu( QDeclarativeContext* context );
void initSystemMenu( QDeclarativeContext* context );
void initGameLauncher( QDeclarativeContext* context );

#endif // MAIN_H
