#include <KAuth>

using namespace KAuth;

class Helper : public QObject
{
    Q_OBJECT

    public Q_SLOTS:

#ifndef NO_SYSTEMD
        ActionReply dbusaction(const QVariantMap &args);
#endif

        ActionReply read(const QVariantMap &args);
        ActionReply write(const QVariantMap &args);
};
