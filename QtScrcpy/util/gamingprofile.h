#pragma once

#include <QString>
#include <QtGlobal>

class GamingProfile
{
public:
    // On failure, leave this profile unchanged and return a diagnostic.
    bool load(const QString &filePath, QString *error = nullptr);

    quint16 maxSize = 1024;
    quint32 maxFps = 60;
    quint32 videoBitRate = 6000000;
    QString processPriority = QStringLiteral("High");
};
