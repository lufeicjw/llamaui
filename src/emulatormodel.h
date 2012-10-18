#ifndef EMULATORMODEL_H
#define EMULATORMODEL_H

#include "listmodel.h"

class EmulatorModel
:	public ListModel
{
Q_OBJECT

public:
	explicit EmulatorModel( QObject* parent = 0 );

	Q_INVOKABLE int indexOfEmulator( const QString& name ) const;
	Q_INVOKABLE QString emulatorAtIndex( int i ) const;
	Q_INVOKABLE QString descriptionAtIndex( int i ) const;

public slots:
	void reload();
	void toggleItemAtIndex( int i );

};

#endif // EMULATORMODEL_H
