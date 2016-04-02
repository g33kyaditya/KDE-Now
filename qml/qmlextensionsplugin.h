#ifndef QMLEXTENSIONSPLUGIN_H
#define QMLEXTENSIONSPLUGIN_H

#include <QtQml/QQmlExtensionPlugin>

class QmlExtensionsPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")

public:
    virtual void initializeEngine(QQmlEngine *engine, const char *uri);
    virtual void registerTypes(const char *uri);
};

#endif // QMLEXTENSIONSPLUGIN_H
