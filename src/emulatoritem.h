#ifndef EMULATORITEM_H
#define EMULATORITEM_H

#include <QString>
#include "modelitem.h"

class EmulatorItem
:	public ModelItem
{
public:
	EmulatorItem();
	EmulatorItem( const QString& name, const QString& description, const QString& executable = QString(), const QString& options = QString() );
	EmulatorItem( const QString& name, const QString& description, bool isCheckable, bool checked = false );

	enum Role {
		NameRole = Qt::UserRole + 1,
		DescriptionRole,
		ExecutableRole,
		OptionsRole,
		IsCheckableRole,
		IsCheckedRole
	};

	virtual QString id() const;
	virtual QVariant data( int role ) const;

	QString name() const;
	QString description() const;
	QString executable() const;
	QString options() const;
	bool isCheckable() const;
	bool isChecked() const;
	void setChecked( bool checked );
	bool toggle();

	static QHash< int, QByteArray > roleNames();

private:
	QString _name;
	QString _description;
	QString _executable;
	QString _options;
	bool _isCheckable;
	bool _isChecked;
};

#endif // EMULATORITEM_H
