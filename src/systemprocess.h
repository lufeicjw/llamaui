#ifndef SYSTEMPROCESS_H
#define SYSTEMPROCESS_H

#include <QObject>

class SystemProcess
:	public QObject
{
Q_OBJECT
Q_ENUMS( Command )

public:
	explicit SystemProcess( QObject* parent = 0 );
	virtual ~SystemProcess() { }

	enum Command {
		ShutdownCommand,
		RebootCommand
	};

	static bool isCommandAvailable( Command command );
	static QString commandName( Command command );

public slots:
	void executeCommand( Command command );
};

#endif // SYSTEMPROCESS_H
