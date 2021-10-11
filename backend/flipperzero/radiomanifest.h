#pragma once

#include <QMap>
#include <QVector>
#include <QByteArray>
#include <QJsonValue>

#include "failable.h"

namespace Flipper {
namespace Zero {

class RadioManifest : public Failable
{
public:
    class Header {
    public:
        Header();
        Header(const QJsonValue &json);

        int version() const;
        time_t timestamp() const;

    private:
        int m_version;
        time_t m_timestamp;
    };

    class Condition {
    public:
        enum class Type {
            Unknown,
            Equals,
            Greater
        };

    Condition();
    Condition(const QString &text);

    Type type() const;
    const QString &version() const;

    private:
        Type m_type;
        QString m_version;
    };

    class FileInfo {
    public:
        FileInfo() = default;
        FileInfo(const QJsonValue &json);

        const QString &name() const;
        const QByteArray &sha256() const;
        const Condition &condition() const;
        uint32_t address() const;

    private:
        QString m_name;
        QByteArray m_sha256;
        Condition m_condition;
        uint32_t m_address;
    };

    using FileInfoMap = QMap<QString, FileInfo>;

    class Section {
    public:
        Section() = default;
        Section(const QJsonValue &json);

        const QString &version() const;
        const FileInfoMap &files() const;

    private:
        void readVersion(const QJsonValue &json);
        void readFiles(const QJsonValue &json);

        QString m_version;
        FileInfoMap m_files;
    };

    RadioManifest() = default;
    RadioManifest(const QByteArray &text);

    const Header &header() const;
    const Section &fus() const;
    const Section &radio() const;

private:
    Header m_header;
    Section m_fus;
    Section m_radio;
};

}
}

