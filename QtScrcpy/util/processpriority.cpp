#include "processpriority.h"

#include <QtGlobal>
#ifdef Q_OS_WIN
#include <windows.h>
#endif

bool ProcessPriority::apply(const QString &priority, QString *error)
{
    if (error) {
        error->clear();
    }
    if (priority != QStringLiteral("High") && priority != QStringLiteral("Normal")) {
        if (error) {
            *error = QStringLiteral("Unsupported process priority: %1").arg(priority);
        }
        return false;
    }
#ifdef Q_OS_WIN
    const DWORD priorityClass = priority == QStringLiteral("High")
        ? HIGH_PRIORITY_CLASS : NORMAL_PRIORITY_CLASS;
    if (!SetPriorityClass(GetCurrentProcess(), priorityClass)) {
        const DWORD code = GetLastError();
        if (error) {
            *error = QStringLiteral("SetPriorityClass failed (Windows error %1).")
                         .arg(static_cast<qulonglong>(code));
        }
        return false;
    }
#endif
    return true;
}
