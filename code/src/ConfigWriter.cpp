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
#include <QtCore/QJsonValue>
#include <QtCore/QStringBuilder>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

namespace ConfigWriter
{

QJsonDocument writeToJson(const ConfigObjectNode &node)
{
    // Convert the configuration node to JSON
    const QJsonObject root {
        { QStringLiteral("config"), QJsonValue::fromVariant(node.toSimplifiedVariant()) }
    };

    return QJsonDocument(root);
}

// -------------------------------------------------------------------------------------------------

bool writeToFile(const ConfigObjectNode &node, const QString &filePath)
{
    // Convert the configuration node to JSON
    const auto doc = writeToJson(node);

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

} // namespace ConfigWriter

} // namespace CppConfigFramework
