#ifndef SYSTEMITEM_H
#define SYSTEMITEM_H

#include <QByteArray>
#include <QHash>
#include <QString>
#include <QVariant>
#include "modelitem.h"
#include "systemprocess.h"

class SystemItem
:	public ModelItem
{
public:
	SystemItem( const QString& name, SystemProcess::Command command );

	enum Role {
		NameRole = Qt::UserRole + 1,
		CommandRole
	};

	virtual QString id() const;
	virtual QVariant data( int role ) const;

	static QHash< int, QByteArray > roleNames();

private:
	QString _name;
	SystemProcess::Command _command;
};

#endif // SYSTEMITEM_H
