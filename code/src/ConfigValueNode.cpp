/* This file is part of C++ Config Framework.
 *
 * C++ Config Framework is free software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * C++ Config Framework is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with C++ Config
 * Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * \file
 *
 * Contains a class for the Value configuration node
 */

// Own header
#include <CppConfigFramework/ConfigValueNode.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigWriter.hpp>

// Qt includes
#include <QtCore/QDateTime>
#include <QtCore/QLine>
#include <QtCore/QLineF>
#include <QtCore/QPoint>
#include <QtCore/QPointF>
#include <QtCore/QRect>
#include <QtCore/QRectF>
#include <QtCore/QSize>
#include <QtCore/QSizeF>
#include <QtCore/QUrl>
#include <QtCore/QUuid>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

ConfigValueNode::ConfigValueNode(const QVariant &value, ConfigObjectNode *parent)
    : ConfigNode(parent),
      m_value(value)
{
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigValueNode::clone() const
{
    return std::make_unique<ConfigValueNode>(m_value, nullptr);
}

// -------------------------------------------------------------------------------------------------

ConfigNode::Type ConfigValueNode::type() const
{
    return ConfigNode::Type::Value;
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant() const
{
    return toSimplifiedVariant(m_value);
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::value() const
{
    return m_value;
}

// -------------------------------------------------------------------------------------------------

void ConfigValueNode::setValue(const QVariant &value)
{
    m_value = value;
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const bool value)
{
    return QVariant(value);
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const int64_t value)
{
    constexpr int64_t positiveLimit = 1LL << 52;
    constexpr int64_t negativeLimit = -positiveLimit;

    if ((negativeLimit <= value) && (value <= positiveLimit))
    {
        return QVariant(static_cast<double>(value));
    }

    return QVariant(QString::number(value));
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const uint64_t value)
{
    constexpr uint64_t limit = 1ULL << 52;

    if (value <= limit)
    {
        return QVariant(static_cast<double>(value));
    }

    return QVariant(QString::number(value));
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const double value)
{
    return QVariant(value);
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QString value)
{
    return QVariant(value);
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QTime &value)
{
    return QVariant(value.toString(Qt::ISODateWithMs));
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QDate &value)
{
    return QVariant(value.toString(Qt::ISODate));
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QDateTime &value)
{
    return QVariant(value.toString(Qt::ISODateWithMs));
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QSize &value)
{
    return QVariantMap {
        { QStringLiteral("width"),  toSimplifiedVariant(static_cast<double>(value.width())) },
        { QStringLiteral("height"), toSimplifiedVariant(static_cast<double>(value.height())) }
    };
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QSizeF &value)
{
    return QVariantMap {
        { QStringLiteral("width"),  QVariant(value.width()) },
        { QStringLiteral("height"), QVariant(value.height()) }
    };
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QPoint &value)
{
    return QVariantMap {
        { QStringLiteral("x"), toSimplifiedVariant(static_cast<double>(value.x())) },
        { QStringLiteral("y"), toSimplifiedVariant(static_cast<double>(value.y())) }
    };
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QPointF &value)
{
    return QVariantMap {
        { QStringLiteral("x"), QVariant(value.x()) },
        { QStringLiteral("y"), QVariant(value.y()) }
    };
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QLine &value)
{
    return QVariantMap {
        { QStringLiteral("x1"), toSimplifiedVariant(static_cast<double>(value.x1())) },
        { QStringLiteral("y1"), toSimplifiedVariant(static_cast<double>(value.y1())) },

        { QStringLiteral("x2"), toSimplifiedVariant(static_cast<double>(value.x2())) },
        { QStringLiteral("y2"), toSimplifiedVariant(static_cast<double>(value.y2())) }
    };
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QLineF &value)
{
    return QVariantMap {
        { QStringLiteral("x1"), QVariant(value.x1()) },
        { QStringLiteral("y1"), QVariant(value.y1()) },

        { QStringLiteral("x2"), QVariant(value.x2()) },
        { QStringLiteral("y2"), QVariant(value.y2()) }
    };
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QRect &value)
{
    return QVariantMap {
        { QStringLiteral("x"),      toSimplifiedVariant(static_cast<double>(value.x())) },
        { QStringLiteral("y"),      toSimplifiedVariant(static_cast<double>(value.y())) },

        { QStringLiteral("width"),  toSimplifiedVariant(static_cast<double>(value.width())) },
        { QStringLiteral("height"), toSimplifiedVariant(static_cast<double>(value.height())) }
    };
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QRectF &value)
{
    return QVariantMap {
        { QStringLiteral("x"), QVariant(value.x()) },
        { QStringLiteral("y"), QVariant(value.y()) },

        { QStringLiteral("width"),  QVariant(value.width()) },
        { QStringLiteral("height"), QVariant(value.height()) }
    };
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QVariant &value)
{
    // Check for a compatible numeric type in the QVariant value
    switch (value.userType())
    {
        case QMetaType::UnknownType:
        {
            return QVariant();
        }

        case qMetaTypeId<bool>():
        {
            return toSimplifiedVariant(value.value<bool>());
        }

        case qMetaTypeId<uint8_t>():
        case qMetaTypeId<uint16_t>():
        case qMetaTypeId<uint32_t>():
        case qMetaTypeId<uint64_t>():
        case qMetaTypeId<qulonglong>():
        {
            return toSimplifiedVariant(value.value<uint64_t>());
        }

        case qMetaTypeId<int8_t>():
        case qMetaTypeId<int16_t>():
        case qMetaTypeId<int32_t>():
        case qMetaTypeId<int64_t>():
        case qMetaTypeId<qlonglong>():
        {
            return toSimplifiedVariant(value.value<int64_t>());
        }

        case qMetaTypeId<float>():
        case qMetaTypeId<double>():
        {
            return toSimplifiedVariant(value.value<double>());
        }

        case qMetaTypeId<QTime>():
        {
            return toSimplifiedVariant(value.value<QTime>());
        }

        case qMetaTypeId<QDate>():
        {
            return toSimplifiedVariant(value.value<QDate>());
        }

        case qMetaTypeId<QDateTime>():
        {
            return toSimplifiedVariant(value.value<QDateTime>());
        }

        case qMetaTypeId<QChar>():
        {
            return toSimplifiedVariant(QString(value.value<QChar>()));
        }

        case qMetaTypeId<QString>():
        {
            return toSimplifiedVariant(value.value<QString>());
        }

        case qMetaTypeId<QByteArray>():
        {
            return toSimplifiedVariant(QString::fromUtf8(value.value<QByteArray>()));
        }

        case qMetaTypeId<QUrl>():
        {
            return toSimplifiedVariant(value.value<QUrl>().toString());
        }

        case qMetaTypeId<QUuid>():
        {
            return toSimplifiedVariant(value.value<QUuid>().toString());
        }

        case qMetaTypeId<QSize>():
        {
            return toSimplifiedVariant(value.value<QSize>());
        }

        case qMetaTypeId<QSizeF>():
        {
            return toSimplifiedVariant(value.value<QSizeF>());
        }

        case qMetaTypeId<QPoint>():
        {
            return toSimplifiedVariant(value.value<QPoint>());
        }

        case qMetaTypeId<QPointF>():
        {
            return toSimplifiedVariant(value.value<QPointF>());
        }

        case qMetaTypeId<QLine>():
        {
            return toSimplifiedVariant(value.value<QLine>());
        }

        case qMetaTypeId<QLineF>():
        {
            return toSimplifiedVariant(value.value<QLineF>());
        }

        case qMetaTypeId<QRect>():
        {
            return toSimplifiedVariant(value.value<QRect>());
        }

        case qMetaTypeId<QRectF>():
        {
            return toSimplifiedVariant(value.value<QRectF>());
        }

        default:
        {
            if (value.canConvert<QVariantList>())
            {
                return toSimplifiedVariant(value.value<QSequentialIterable>());
            }

            if (value.canConvert<QVariantMap>())
            {
                return toSimplifiedVariant(value.value<QAssociativeIterable>());
            }

            break;
        }
    }

    return {};
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QSequentialIterable &iterable)
{
    QVariantList list;

    for (const QVariant &value : iterable)
    {
        list.append(toSimplifiedVariant(value));
    }

    return list;
}

// -------------------------------------------------------------------------------------------------

QVariant ConfigValueNode::toSimplifiedVariant(const QAssociativeIterable &iterable)
{
    QVariantMap object;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Convert key to string
        const QVariant simplifiedKey = toSimplifiedVariant(it.key());
        QString key;

        switch (simplifiedKey.userType())
        {
            case QMetaType::UnknownType:
            {
                key = QString("");
                break;
            }

            case qMetaTypeId<bool>():
            {
                key = simplifiedKey.toBool() ? QStringLiteral("true")
                                             : QStringLiteral("false");
                break;
            }

            case qMetaTypeId<double>():
            case qMetaTypeId<QString>():
            {
                key = simplifiedKey.toString();
                break;
            }

            default:
            {
                return {};
            }
        }

        // Insert to object
        object.insert(key, toSimplifiedVariant(it.value()));
    }

    return object;
}

} // namespace CppConfigFramework

// -------------------------------------------------------------------------------------------------

bool operator==(const CppConfigFramework::ConfigValueNode &left,
                const CppConfigFramework::ConfigValueNode &right)
{
    return ((left.nodePath() == right.nodePath()) &&
            (left.value() == right.value()));
}

// -------------------------------------------------------------------------------------------------

bool operator!=(const CppConfigFramework::ConfigValueNode &left,
                const CppConfigFramework::ConfigValueNode &right)
{
    return !(left == right);
}
