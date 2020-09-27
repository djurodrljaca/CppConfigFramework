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
 * Contains methods for writing configuration files
 */

// Own header
#include <CppConfigFramework/ConfigWriter.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigDerivedObjectNode.hpp>
#include <CppConfigFramework/ConfigNodeReference.hpp>
#include <CppConfigFramework/ConfigValueNode.hpp>
#include <CppConfigFramework/LoggingCategories.hpp>

// Qt includes
#include <QtCore/QFile>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QStringBuilder>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

namespace ConfigWriter
{

namespace Internal
{

QJsonValue toJsonConfig(const ConfigValueNode &valueNode);
QJsonValue toJsonConfig(const ConfigObjectNode &objectNode);
QJsonValue toJsonConfig(const ConfigNodeReference &nodeReference);
QJsonValue toJsonConfig(const ConfigDerivedObjectNode &derivedObjectNode);

// -------------------------------------------------------------------------------------------------

QJsonValue toJsonConfig(const ConfigValueNode &valueNode)
{
    return valueNode.value();
}

// -------------------------------------------------------------------------------------------------

QJsonValue toJsonConfig(const ConfigObjectNode &objectNode)
{
    QJsonObject data;

    for (const auto &memberName : objectNode.names())
    {
        const auto *member = objectNode.member(memberName);

        switch (member->type())
        {
            case ConfigNode::Type::Value:
            {
                data.insert(QChar('#') % memberName, Internal::toJsonConfig(member->toValue()));
                break;
            }

            case ConfigNode::Type::Object:
            {
                data.insert(memberName, Internal::toJsonConfig(member->toObject()));
                break;
            }

            case ConfigNode::Type::NodeReference:
            {
                data.insert(QChar('&') % memberName,
                            Internal::toJsonConfig(member->toNodeReference()));
                break;
            }

            case ConfigNode::Type::DerivedObject:
            {
                data.insert(QChar('&') % memberName,
                            Internal::toJsonConfig(member->toDerivedObject()));
                break;
            }

            default:
            {
                return {};
            }
        }
    }

    return data;
}

// -------------------------------------------------------------------------------------------------

QJsonValue toJsonConfig(const ConfigNodeReference &nodeReference)
{
    return nodeReference.reference().path();
}

// -------------------------------------------------------------------------------------------------

QJsonValue toJsonConfig(const ConfigDerivedObjectNode &derivedObjectNode)
{
    QJsonObject data;

    // Base member
    QStringList bases;

    for (const auto &base : derivedObjectNode.bases())
    {
        bases.append(base.path());
    }

    if (bases.isEmpty())
    {
        // The "base" member is not needed
    }
    else if (bases.size() == 1)
    {
        // Add the single base as a string
        data.insert(QStringLiteral("base"), bases.first());
    }
    else
    {
        // Add the array of bases
        data.insert(QStringLiteral("base"), QJsonArray::fromStringList(bases));
    }

    // Config member
    data.insert(QStringLiteral("config"), Internal::toJsonConfig(derivedObjectNode.config()));

    return data;
}

} // namespace Internal

// -------------------------------------------------------------------------------------------------

QJsonDocument writeToJsonConfig(const ConfigObjectNode &node)
{
    // Convert the configuration node to JSON
    const QJsonObject root { { QStringLiteral("config"), Internal::toJsonConfig(node) } };

    return QJsonDocument(root);
}

// -------------------------------------------------------------------------------------------------

bool writeToJsonConfigFile(const ConfigObjectNode &node, const QString &filePath)
{
    // Convert the configuration node to JSON
    const auto doc = writeToJsonConfig(node);

    if (doc.isEmpty())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigWriter) << "JSON document is empty!";
        return false;
    }

    const QByteArray jsonData = doc.toJson();

    // Write configuration to file
    QFile file(filePath);

    if (!file.open(QIODevice::WriteOnly))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigWriter)
                << "Failed to open file:" << filePath;
        return false;
    }

    const qint64 writtenSize = file.write(jsonData);
    const qint64 jsonDataSize = static_cast<qint64>(jsonData.size());

    if (writtenSize != jsonDataSize)
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigWriter)
                << QString("Number of bytes written [%1] to the file [%2] does not match the "
                           "number of bytes [%3] in the JSON document")
                   .arg(writtenSize)
                   .arg(filePath)
                   .arg(jsonDataSize);
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

QJsonValue convertToJsonValue(const ConfigObjectNode &node)
{
    QJsonObject data;

    for (const auto &memberName : node.names())
    {
        const auto *member = node.member(memberName);

        switch (member->type())
        {
            case ConfigNode::Type::Value:
            {
                data.insert(memberName, member->toValue().value());
                break;
            }

            case ConfigNode::Type::Object:
            {
                data.insert(memberName, convertToJsonValue(member->toObject()));
                break;
            }

            case ConfigNode::Type::NodeReference:
            case ConfigNode::Type::DerivedObject:
            default:
            {
                return QJsonValue::Undefined;
            }
        }
    }

    return data;
}

} // namespace ConfigWriter

} // namespace CppConfigFramework
