#ifndef GAMEPROCESS_H
#define GAMEPROCESS_H

#include <QObject>

class QEvent;
class QProcess;
class QTimerEvent;

class GameProcess
:	public QObject
{
Q_OBJECT

signals:
	void gameDataChanged( const QString& emulator, const QString& rom );

public:
	explicit GameProcess( QObject* parent = 0 );

	Q_INVOKABLE QString lastGamePlayed( const QString& emulator ) const;
	Q_INVOKABLE bool gameRunning() const;

public slots:
	void startGame( const QString& emulator, const QString& rom );
	void startGame( const QString& rom );
	void startAttractMode();
	void stopAttractMode();

protected:
	virtual bool eventFilter( QObject* obj, QEvent* event );
	virtual void timerEvent( QTimerEvent* event );

private:
	void setLastPlayed( const QString& emulator, const QString& rom );
	void updatePlayCountWithReturnCode( const QString& emulator, const QString& rom, int returnCode );

private:
	int _attractStartTimerId;
	int _attractStopTimerId;
	QProcess* _gameProcess;
	QString _lastEmulatorPlayed;
	QString _lastRomPlayed;

private slots:
	void resetAttractTimer();
	void gameFinished( int returnCode );
	void gameProcessDestroyed();
};

#endif // GAMEPROCESS_H
