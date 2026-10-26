#pragma once

#include <QString>

namespace ProcessPriority {
// Changes QtScrcpy itself, not adb or the Android server. Unsupported platforms
// are a successful no-op. Windows keeps this priority until changed or exit.
bool apply(const QString &priority, QString *error = nullptr);
}
