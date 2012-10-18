#ifndef CONFIG_H
#define CONFIG_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariant>

class Config
:	public QObject
{
Q_OBJECT

public:
	Config( QObject* parent = 0 );
	virtual ~Config() { }

	static Config* instance();

	Q_INVOKABLE bool isEmpty() const;
	Q_INVOKABLE QVariant value( const QString& section, const QString& key ) const;
	Q_INVOKABLE void setValue( const QString& section, const QString& key, const QVariant& value );
	Q_INVOKABLE QStringList sectionNames() const;
	Q_INVOKABLE QList< QVariant > values( const QString& section, const QString& key ) const;
	Q_INVOKABLE QMap< QString, QVariant > section( const QString& section ) const;
	Q_INVOKABLE bool hasSection( const QString& section ) const;
	Q_INVOKABLE bool hasValue( const QString& section, const QString& key ) const;

	bool readConfig( const QString& file );

	template< typename T > static T value( const QString& section, const QString& key )
	{
		if ( !instance()->_reg.contains( section ) || !instance()->_reg[ section ].contains( key ) )
			return T();

		return QVariant( instance()->parseVariables( instance()->_reg[ section ][ key ].toString() ) ).value< T >();
	}

	template< typename T > static QList< T > values( const QString& section, const QString& key )
	{
		QList< T > values;

		if ( !instance()->_reg.contains( section ) || !instance()->_reg[ section ].contains( key ) )
			return values;

		foreach ( QVariant value, instance()->_reg[ section ].values( key ) )
			values += QVariant( instance()->parseVariables( value.toString() ) ).value< T >();

		return values;
	}

protected:
	QMap< QString, QMap< QString, QVariant > > _reg; /*!< This holds the loaded configuration. */

protected:
	QString parseVariables( QString string ) const;

private:
	Config( const Config& other );
	Config& operator=( const Config& other );

	void processConfigurationOption(const QString& seciton, const QStringList& options );

private:
	static Config* _globalInstance;
};

#endif // REGISTRY_H
