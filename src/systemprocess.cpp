#include "systemprocess.h"

#include <QtDebug>

#ifdef Q_OS_LINUX
#include <unistd.h>
#include <sys/types.h>
#include <sys/reboot.h>
#include <linux/reboot.h>
#endif // Q_OS_LINUX

SystemProcess::SystemProcess( QObject* parent )
:	QObject( parent )
{
}

bool SystemProcess::isCommandAvailable( Command command )
{
	switch ( command ) {
	case ShutdownCommand:
	case RebootCommand:
#ifdef Q_OS_LINUX
		return geteuid() != 0;
#else
		return false;
#endif // Q_OS_LINUX
	}

	return false;
}

QString SystemProcess::commandName( Command command )
{
	switch ( command ) {
	case ShutdownCommand:
		return "Shutdown system";

	case RebootCommand:
		return "Reboot system";
	}

	return QString();
}

void SystemProcess::executeCommand( Command command )
{
	int result = 0;

	switch ( command ) {
	case ShutdownCommand:
		sync();
		result = reboot( RB_POWER_OFF );
		break;

	case RebootCommand:
		sync();
		result = reboot( RB_AUTOBOOT );
		break;
	}
}
