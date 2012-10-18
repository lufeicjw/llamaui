#ifndef SYSTEMMODEL_H
#define SYSTEMMODEL_H

#include "listmodel.h"
#include "systemprocess.h"

class SystemModel
:	public ListModel
{
Q_OBJECT

public:
	explicit SystemModel( QObject* parent = 0 );
	virtual ~SystemModel() { }

	void addItem( const QString& name, SystemProcess::Command command );

	Q_INVOKABLE SystemProcess::Command commandAtIndex( int i );
};

#endif // SYSTEMMODEL_H
