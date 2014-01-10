#ifndef CONTROLOBJECTSLAVE_H
#define CONTROLOBJECTSLAVE_H

#include <QObject>
#include <QSharedPointer>
#include <QString>

#include "configobject.h"

class ControlDoublePrivate;

// This class is the successor of ControlObjectThread. It should be used for new
// code. It is better named and may save some CPU time because it is connected
// only on demand. There are many ControlObjectThread instances where the changed
// signal is not needed. This change will save the set() caller for doing
// unnecessary checks for possible connections.
class ControlObjectSlave : public QObject {
    Q_OBJECT
  public:
    ControlObjectSlave(QObject* pParent = NULL);
    ControlObjectSlave(const QString& g, const QString& i, QObject* pParent = NULL);
    ControlObjectSlave(const char* g, const char* i, QObject* pParent = NULL);
    ControlObjectSlave(const ConfigKey& key, QObject* pParent = NULL);
    virtual ~ControlObjectSlave();

    void initialize(const ConfigKey& key);

    bool connectValueChanged(const QObject* receiver,
            const char* method, Qt::ConnectionType type = Qt::AutoConnection);
    bool connectValueChanged(
            const char* method, Qt::ConnectionType type = Qt::AutoConnection );

    bool connectParameterChanged(const QObject* receiver,
            const char* method, Qt::ConnectionType type = Qt::AutoConnection);
    bool connectParameterChanged(
            const char* method, Qt::ConnectionType type = Qt::AutoConnection );

    // Called from update();
    void emitValueChanged();
    void emitParameterChanged();

    inline bool valid() const { return m_pControl != NULL; }

    // Returns the value of the object. Thread safe, non-blocking.
    virtual double get();

    // Returns the parameterized value of the object. Thread safe, non-blocking.
    virtual double getParameter();

  public slots:
    // Set the control to a new value. Non-blocking.
    virtual void slotSet(double v);
    // Sets the control value to v. Thread safe, non-blocking.
    virtual void set(double v);
    // Sets the control parameterized value to v. Thread safe, non-blocking.
    virtual void setParameter(double v);
    // Resets the control to its default value. Thread safe, non-blocking.
    virtual void reset();

  signals:
    // This signal must not connected by connect(). Use connectValueChanged()
    // instead. It will connect to the base ControlDoublePrivate as well.
    void valueChanged(double);

    // This signal must not be connected by connect(). Use
    // connectParameterChanged() instead. It will connect to the base
    // ControlDoublePrivate as well.
    void parameterChanged(double);

  protected slots:
    // Receives the value from the master control and re-emits either
    // valueChanged(double) or valueChangedByThis(double) based on pSetter.
    virtual void slotValueChanged(double v, QObject* pSetter);

  protected:
    // Pointer to connected control.
    QSharedPointer<ControlDoublePrivate> m_pControl;

  private:
    bool m_bHasValueListener;
    bool m_bHasParameterListener;
};

#endif // CONTROLOBJECTSLAVE_H
