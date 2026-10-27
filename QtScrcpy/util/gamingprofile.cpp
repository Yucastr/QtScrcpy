#include "gamingprofile.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QJsonValue>
#include <cmath>
#include <limits>

bool GamingProfile::load(const QString &filePath, QString *error)
{
    const auto fail = [error](const QString &message) {
        if (error) {
            *error = message;
        }
        return false;
    };
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return fail(QStringLiteral("Cannot read %1: %2").arg(filePath, file.errorString()));
    }
    QJsonParseError parseError;
    const QJsonDocument document = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !document.isObject()) {
        return fail(QStringLiteral("Invalid gaming profile JSON in %1: %2")
                        .arg(filePath, parseError.error == QJsonParseError::NoError
                                           ? QStringLiteral("expected an object") : parseError.errorString()));
    }
    const QJsonObject object = document.object();
    const auto validInteger = [&object](const QString &key, double maximum) {
        const QJsonValue value = object.value(key);
        const double number = value.toDouble(-1);
        return value.isDouble() && std::isfinite(number) && number > 0
            && number <= maximum && std::floor(number) == number;
    };
    if (!validInteger(QStringLiteral("max_size"), std::numeric_limits<quint16>::max())
        || !validInteger(QStringLiteral("max_fps"), std::numeric_limits<quint32>::max())
        || !validInteger(QStringLiteral("video_bit_rate"), std::numeric_limits<quint32>::max())) {
        return fail(QStringLiteral("Gaming profile requires positive integers: max_size (1..65535), "
                                   "max_fps and video_bit_rate (1..4294967295)."));
    }
    const QString priority = object.value(QStringLiteral("process_priority")).toString();
    if (priority != QStringLiteral("High") && priority != QStringLiteral("Normal")) {
        return fail(QStringLiteral("Gaming profile process_priority must be High or Normal."));
    }
    maxSize = static_cast<quint16>(object.value(QStringLiteral("max_size")).toDouble());
    maxFps = static_cast<quint32>(object.value(QStringLiteral("max_fps")).toDouble());
    videoBitRate = static_cast<quint32>(object.value(QStringLiteral("video_bit_rate")).toDouble());
    processPriority = priority;
    if (error) {
        error->clear();
    }
    return true;
}
