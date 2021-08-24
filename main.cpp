#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include <QTimer>
#include <QTime>

#define MethodA //Registered type
//#define MethodB //Singeltons
//#define MethodC //exposed c++ instance

#if defined MethodA
class SomeCppClass : public QObject {

    Q_OBJECT

    Q_PROPERTY(int zaehler READ zaehler WRITE setZaehler NOTIFY zaehlerChanged)
    Q_PROPERTY(QString someString READ someString CONSTANT)

public:
    explicit SomeCppClass (QObject *parent = nullptr): QObject(parent){
        connect(&m_timer, &QTimer::timeout, this, [=]()->void{
            setZaehler(zaehler() +1);
        });
    }

    [[nodiscard]] constexpr int zaehler() const noexcept { return m_zaehler; }
    void setZaehler(int newZaehler) noexcept {
        if (m_zaehler == newZaehler)
            return;
        m_zaehler = newZaehler;
        emit zaehlerChanged(m_zaehler);
    }

    [[nodiscard]] const QString &someString() const noexcept { return m_someString; }

    Q_INVOKABLE void starteZaehler() {m_timer.start(1000);} //1 Sekunde
    Q_INVOKABLE QString currentTime() {return QTime::currentTime().toString();}

signals:
    void zaehlerChanged(int);

private:
    QTimer m_timer;
    int m_zaehler{0};
    QString m_someString{"Hallo Welt"};
};
#endif

#if defined MethodB
class SomeCppClass : public QObject {

    Q_OBJECT

    Q_PROPERTY(int zaehler READ zaehler WRITE setZaehler NOTIFY zaehlerChanged)
    Q_PROPERTY(QString someString READ someString CONSTANT)

    explicit SomeCppClass (QObject *parent = nullptr): QObject(parent){
        connect(&m_timer, &QTimer::timeout, this, [=]()->void{
            setZaehler(zaehler() +1);
        });
    }
    static QObject *ClassPointer;
public:
    static SomeCppClass *getInstance() {
            if(ClassPointer)
                return  qobject_cast<SomeCppClass*>(SomeCppClass::ClassPointer);

            auto instance = new SomeCppClass(nullptr);
            ClassPointer = instance;

            return  instance;
        }

        static QObject *instance(QQmlEngine *engine, QJSEngine *) {
            if(!ClassPointer){
                ClassPointer = new SomeCppClass(nullptr);
            }

            QMetaObject::invokeMethod(ClassPointer, [=]()->void{
                engine->setObjectOwnership(ClassPointer, QQmlEngine::CppOwnership);
            }, Qt::QueuedConnection);
            return ClassPointer;
        }

        static void resolve() {
            if (ClassPointer){
                ClassPointer->deleteLater();
                ClassPointer = nullptr;
            }
        }

    [[nodiscard]] constexpr int zaehler() const noexcept { return m_zaehler; }
    void setZaehler(int newZaehler) noexcept {
        if (m_zaehler == newZaehler)
            return;
        m_zaehler = newZaehler;
        emit zaehlerChanged(m_zaehler);
    }

    [[nodiscard]] const QString &someString() const noexcept { return m_someString; }

    Q_INVOKABLE void starteZaehler() {m_timer.start(1000);} //1 Sekunde
    Q_INVOKABLE QString currentTime() {return QTime::currentTime().toString();}

signals:
    void zaehlerChanged(int);

private:
    QTimer m_timer;
    int m_zaehler{0};
    QString m_someString{"Hallo Welt"};
};

QObject * SomeCppClass::ClassPointer = nullptr;

#endif

#if defined MethodC
#include <QQmlContext>
class SomeCppClass : public QObject {

    Q_OBJECT

    Q_PROPERTY(int zaehler READ zaehler WRITE setZaehler NOTIFY zaehlerChanged)
    Q_PROPERTY(QString someString READ someString CONSTANT)

public:
    explicit SomeCppClass (QObject *parent = nullptr): QObject(parent){
        connect(&m_timer, &QTimer::timeout, this, [=]()->void{
            setZaehler(zaehler() +1);
        });
    }

    [[nodiscard]] constexpr int zaehler() const noexcept { return m_zaehler; }
    void setZaehler(int newZaehler) noexcept {
        if (m_zaehler == newZaehler)
            return;
        m_zaehler = newZaehler;
        emit zaehlerChanged(m_zaehler);
    }

    [[nodiscard]] const QString &someString() const noexcept { return m_someString; }

    Q_INVOKABLE void starteZaehler() {m_timer.start(1000);} //1 Sekunde
    Q_INVOKABLE QString currentTime() {return QTime::currentTime().toString();}

signals:
    void zaehlerChanged(int);

private:
    QTimer m_timer;
    int m_zaehler{0};
    QString m_someString{"Hallo Welt"};
};
#endif

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //Allows QML to create instances of this class
#if defined MethodA
    qmlRegisterType<SomeCppClass>("NameForImport", 1, 0, "NameForInstance");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
#endif

#if defined MethodB
#if QT_VERSION >= 0x050E00
    qmlRegisterSingletonInstance("SomeCppSingleton", 1,0, "SomeCppSingletonPointer", SomeCppClass::getInstance());
#else
    qmlRegisterSingletonType<SomeCppClass>("SomeCppSingleton", 1,0, "SomeCppSingletonPointer", SomeCppClass::instance);
#endif
    const QUrl url(QStringLiteral("qrc:/main2.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
#endif

#if defined MethodC
    SomeCppClass someCppClassInstance;
    engine.rootContext()->setContextProperty("ExposedCppInstance", &someCppClassInstance);
    const QUrl url(QStringLiteral("qrc:/main3.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);
#endif

    return app.exec();
}
#include "main.moc"
