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
 * Contains a private implementation for ConfigReader class
 */

// Own header
#include "ConfigReaderImpl.hpp"

// C++ Config Framework includes
#include "DebugHelper.hpp"

// Qt includes
#include <QtCore/QFile>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtCore/QRegularExpression>
#include <QtCore/QStringBuilder>
#include <QtCore/QtDebug>

// System includes

// Forward declarations

// Macros
#define DEBUG_METHOD(METHOD)    CPPCONFIGFRAMEWORK_DEBUG_METHOD("ConfigReader::Impl::" METHOD)

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

ConfigReader::Impl::Impl()
    : m_sourceNode(),
      m_destinationNode(),
      m_referenceResolutionMaxCycles(100u)
{
}

// -------------------------------------------------------------------------------------------------

uint32_t ConfigReader::Impl::referenceResolutionMaxCycles() const
{
    return m_referenceResolutionMaxCycles;
}

// -------------------------------------------------------------------------------------------------

void ConfigReader::Impl::setReferenceResolutionMaxCycles(
        const uint32_t referenceResolutionMaxCycles)
{
    m_referenceResolutionMaxCycles = referenceResolutionMaxCycles;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigReader::Impl::read(const QString &filePath,
                                                     const QString &sourceNode,
                                                     const QString &destinationNode)
{
    // Validate source node
    if ((!ConfigNode::isAbsoluteNodePath(sourceNode)) ||
        (!ConfigNode::validateNodePath(sourceNode)))
    {
        qDebug() << DEBUG_METHOD("read") << "Error: invalid source node path:" << sourceNode;
        return {};
    }

    m_sourceNode = sourceNode;

    // Validate destination node
    if ((!ConfigNode::isAbsoluteNodePath(destinationNode)) ||
        (!ConfigNode::validateNodePath(destinationNode)))
    {
        qDebug() << DEBUG_METHOD("read")
                 << "Error: invalid destination node path:" << destinationNode;
        return {};
    }

    m_destinationNode = destinationNode;

    // Open file
    if (!QFile::exists(filePath))
    {
        qDebug() << DEBUG_METHOD("read") << "Error: file at path was not found:" << filePath;
        return {};
    }

    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << DEBUG_METHOD("read") << "Error: failed to open file at path:" << filePath;
        return {};
    }

    // Read the contents (JSON format)
    QJsonParseError error {};
    const QByteArray fileContents = file.readAll();
    const auto doc = QJsonDocument::fromJson(fileContents, &error);

    if (error.error != QJsonParseError::NoError)
    {
        constexpr int contextMaxLength = 20;
        const int contextBeforeIndex = std::max(0, error.offset - contextMaxLength);
        const int contextBeforeLength = std::min(error.offset, contextMaxLength);

        qDebug() << DEBUG_METHOD("read")
                 << "Error: failed to parse the file contents:"
                    "\n    file path:" << filePath
                 << "\n    offset:" << error.offset
                 << "\n    error:" << error.errorString()
                 << "\n    context before error:" << fileContents.mid(contextBeforeIndex,
                                                                      contextBeforeLength)
                 << "\n    context at error:" << fileContents.mid(error.offset, contextMaxLength);
        return {};
    }

    if (!doc.isObject())
    {
        qDebug() << DEBUG_METHOD("read") << "Error: failed to read includes";
        return {};
    }

    const auto rootObject = doc.object();

    // TODO: add "environment variables" member?

    // Read 'includes' member
    auto completeConfig = readIncludesMember(rootObject);

    if (!completeConfig)
    {
        qDebug() << DEBUG_METHOD("read") << "Error: failed to read the 'includes' member";
        return {};
    }

    // Read 'config' member
    auto configMember = readConfigMember(rootObject);

    if (!configMember)
    {
        qDebug() << DEBUG_METHOD("read") << "Error: failed to read the 'config' member";
        return {};
    }

    if (!completeConfig->applyObject(*configMember))
    {
        return {};
    }

    // Resolve references
    for (uint32_t i = 0; i < m_referenceResolutionMaxCycles; i++)
    {
        switch (resolveReferences(completeConfig.get()))
        {
            case ReferenceResolutionResult::Resolved:
            {
                // Successfully resolved all references, return the fully resolved config
                return completeConfig;
            }

            case ReferenceResolutionResult::Unresolved:
            {
                break;
            }

            case ReferenceResolutionResult::Error:
            {
                qDebug() << DEBUG_METHOD("read") << "Error: failed to resolve references";
                break;
            }
        }
    }

    qDebug() << DEBUG_METHOD("read") << "Error: failed to fully resolve references";
    return {};
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigReader::Impl::readIncludesMember(const QJsonObject &rootObject)
{
    // Check if the root object has any includes
    const auto includesValue = rootObject.value(QStringLiteral("includes"));

    if (includesValue.isNull())
    {
        // No includes
        return std::make_unique<ConfigNode>(ConfigNode::Type::Null);
    }

    if (!includesValue.isArray())
    {
        qDebug() << DEBUG_METHOD("readIncludesMember")
                 << "Error: the 'includes' member in the root JSON Object is not a JSON array!";
        return {};
    }

    // Iterate over all includes and extract their value
    auto includesConfig = std::make_unique<ConfigNode>(ConfigNode::Type::Object);
    const auto includesArray = includesValue.toArray();

    for (int i = 0; i < includesArray.size(); i++)
    {
        // Include item must be a JSON Object
        const auto includeValue = includesArray.at(i);

        if (!includeValue.isObject())
        {
            qDebug() << DEBUG_METHOD("readIncludesMember")
                     << QString("Error: element at index [%1] is not a JSON Object!").arg(i);
            return {};
        }

        // TODO: implement
        //const auto include = includeValue.toObject();

        // TODO: check configuration file type

        // Extract file path
        // TODO: implement

        // Extract source node
        // TODO: implement

        // Extract destination node
        // TODO: implement

        // Read config file
        // TODO: implement

        // Merge the config file contents to our own configuration parameter data structure
        // TODO: implement --> includesConfig->apply(config)
    }

    return includesConfig;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigReader::Impl::readConfigMember(const QJsonObject &rootObject)
{
    // The root object must contain the 'config' member (but it can be an empty object)
    const auto configValue = rootObject.value(QStringLiteral("config"));

    if (configValue.isNull())
    {
        // No configuration
        return std::make_unique<ConfigNode>(ConfigNode::Type::Null);
    }

    if (!configValue.isObject())
    {
        qDebug() << DEBUG_METHOD("readConfigMember")
                 << "Error: the 'config' member in the root JSON Object is not a JSON Object!";
        return {};
    }

    // Read 'config' object
    auto config = readJsonObject(configValue.toObject(), QStringLiteral("/"));

    if (!config)
    {
        qDebug() << DEBUG_METHOD("readConfigMember")
                 << "Error: failed to read the 'config' member in the root JSON Object!";
        return {};
    }

    return config;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigReader::Impl::readJsonValue(const QJsonValue &jsonValue,
                                                              const QString &currentNodePath)
{
    switch (jsonValue.type())
    {
        case QJsonValue::Null:
        {
            return std::make_unique<ConfigNode>();
        }

        case QJsonValue::Bool:
        case QJsonValue::Double:
        case QJsonValue::String:
        {
            auto valueNode = std::make_unique<ConfigNode>(ConfigNode::Type::Value);
            valueNode->setValue(jsonValue.toVariant());
            return valueNode;
        }

        case QJsonValue::Array:
        {
            return readJsonArray(jsonValue.toArray(), currentNodePath);
        }

        case QJsonValue::Object:
        {
            return readJsonObject(jsonValue.toObject(), currentNodePath);
        }

        case QJsonValue::Undefined:
        {
            qDebug() << DEBUG_METHOD("readJsonValue")
                     << "Error: unexpected 'Undefined' JSON value at path:" << currentNodePath;
            return {};
        }
    }

    qDebug() << DEBUG_METHOD("readJsonValue")
             << "Error: unsupported JSON value type at path:" << currentNodePath;
    return {};
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigReader::Impl::readJsonArray(const QJsonArray &jsonArray,
                                                              const QString &currentNodePath)
{
    auto arrayNode = std::make_unique<ConfigNode>(ConfigNode::Type::Array);

    for (int i = 0; i < jsonArray.size(); i++)
    {
        auto elementNode = readJsonValue(jsonArray.at(i),
                                         ConfigNode::appendNodeToPath(currentNodePath,
                                                                      QString::number(i)));

        if (!elementNode)
        {
            return {};
        }

        arrayNode->appendElement(std::move(*elementNode));
    }

    return arrayNode;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigReader::Impl::readJsonObject(const QJsonObject &jsonObject,
                                                               const QString &currentNodePath)
{
    auto objectNode = std::make_unique<ConfigNode>(ConfigNode::Type::Object);

    for (auto it = jsonObject.begin(); it != jsonObject.end(); it++)
    {
        QString memberName = it.key();

        // Check for "decorators" in the member name (reference type or Value node)
        QChar decorator('\0');

        if (hasDecorator(memberName))
        {
            decorator = memberName.at(0);
            memberName = memberName.mid(1);
        }

        // Validate member name
        if (!ConfigNode::validateNodeName(memberName))
        {
            qDebug() << DEBUG_METHOD("readJsonObject")
                     << QString("Error: invalid member name [%1] in path [%2]")
                        .arg(memberName, currentNodePath);
            return {};
        }

        // Check if a member with the same name already exists
        if (objectNode->containsMember(memberName))
        {
            qDebug() << DEBUG_METHOD("readJsonObject")
                     << QString("Error: object already contains a member with the same name "
                                "[%1] in path [%2]").arg(memberName, currentNodePath);
            return {};
        }

        // Create a node based considering the decorator
        std::unique_ptr<ConfigNode> memberNode;
        const QString memberNodePath = ConfigNode::appendNodeToPath(currentNodePath, memberName);

        switch (decorator.toLatin1())
        {
            case '#':
            {
                // Explicit Value node (even if it is an Array or an Object JSON type)
                memberNode = std::make_unique<ConfigNode>(ConfigNode::Type::Value);
                memberNode->setValue(it.value().toVariant());
                break;
            }

            case '&':
            {
                // One of the reference types
                if (it.value().isString())
                {
                    memberNode = readReferenceNode(it.value().toString(), memberNodePath);
                }
                else if (it.value().isArray())
                {
                    memberNode = readDerivedArrayNode(it.value().toArray(), memberNodePath);
                }
                else if (it.value().isObject())
                {
                    memberNode = readDerivedObjectNode(it.value().toObject(), memberNodePath);
                }
                else
                {
                    qDebug() << DEBUG_METHOD("readJsonObject")
                             << "Error: unsupported reference type at path:" << memberNodePath;
                    return {};
                }
                break;
            }

            default:
            {
                // No decorators, just an ordinary node
                memberNode = readJsonValue(it.value(), memberNodePath);
                break;
            }
        }

        if (!memberNode)
        {
            return {};
        }

        // Add member to the object
        objectNode->setMember(memberName, std::move(*memberNode));
    }

    return objectNode;
}

// -------------------------------------------------------------------------------------------------

bool ConfigReader::Impl::hasDecorator(const QString &memberName)
{
    static QRegularExpression regex("^[&#]");

    return regex.match(memberName).hasMatch();
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigReader::Impl::readReferenceNode(const QString &nodeReference,
                                                                  const QString &currentNodePath)
{
    if (!ConfigNode::validateNodePath(nodeReference, currentNodePath))
    {
        qDebug() << DEBUG_METHOD("readReferenceNode")
                 << QString("Error: invalid node reference [%1] with current path [%2]")
                    .arg(nodeReference, currentNodePath);
        return {};
    }

    auto node = std::make_unique<ConfigNode>(ConfigNode::Type::NodeReference);
    node->setNodeReference(nodeReference);

    return node;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigReader::Impl::readDerivedArrayNode(const QJsonArray &jsonArray,
                                                                     const QString &currentNodePath)
{
    auto derivedArrayNode = std::make_unique<ConfigNode>(ConfigNode::Type::DerivedArray);

    for (const auto &jsonValue : jsonArray)
    {
        // Each item in the array must be a JSON Object with an "element" member
        if (!jsonValue.isObject())
        {
            qDebug() << DEBUG_METHOD("readDerivedArrayNode")
                     << "Error: unsupported JSON type for an item in the derived array at path:"
                     << currentNodePath;
            return {};
        }

        const auto arrayItem = jsonValue.toObject();

        if (arrayItem.size() != 1)
        {
            qDebug() << DEBUG_METHOD("readDerivedArrayNode")
                     << "Error: unsupported JSON type for an item in the derived array at path:"
                     << currentNodePath;
            return {};
        }

        QChar decorator('\0');
        QString elementName = arrayItem.begin().key();

        if (hasDecorator(elementName))
        {
            decorator = elementName.at(0);
            elementName = elementName.mid(1);
        }

        if (elementName != QStringLiteral("element"))
        {
            qDebug() << DEBUG_METHOD("readDerivedArrayNode")
                     << QString("Error: unexpected name of the derived array element [%1] "
                                "at path [%2]").arg(elementName, currentNodePath);
            return {};
        }

        // Extract the config node from the element
        const auto elementValue = arrayItem.begin().value();
        std::unique_ptr<ConfigNode> elementNode;

        switch (decorator.toLatin1())
        {
            case '#':
            {
                // Explicit Value node (even if it is an Array or an Object JSON type)
                elementNode = std::make_unique<ConfigNode>(ConfigNode::Type::Value);
                elementNode->setValue(elementValue.toVariant());
                break;
            }

            case '&':
            {
                // One of the reference types
                if (elementValue.isString())
                {
                    elementNode = readReferenceNode(elementValue.toString(), currentNodePath);
                }
                else if (elementValue.isArray())
                {
                    elementNode = readDerivedArrayNode(elementValue.toArray(), currentNodePath);
                }
                else if (elementValue.isObject())
                {
                    elementNode = readDerivedObjectNode(elementValue.toObject(), currentNodePath);
                }
                else
                {
                    qDebug() << DEBUG_METHOD("readDerivedArrayNode")
                             << "Error: unsupported reference type at path:" << currentNodePath;
                    return {};
                }
                break;
            }

            default:
            {
                // No decorators, just an ordinary node
                elementNode = readJsonValue(elementValue, currentNodePath);
                break;
            }
        }

        if (!elementNode)
        {
            return {};
        }

        // Add element to the array
        derivedArrayNode->derivedArray()->push_back(std::move(*elementNode));
    }

    return derivedArrayNode;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigNode> ConfigReader::Impl::readDerivedObjectNode(
        const QJsonObject &jsonObject, const QString &currentNodePath)
{
    // Extract bases
    if (!jsonObject.contains(QStringLiteral("base")))
    {
        qDebug() << DEBUG_METHOD("readDerivedObjectNode")
                 << "Error: a derived object doesn't have the 'base' member"
                 << currentNodePath;
        return {};
    }

    QStringList bases;
    const auto baseValue = jsonObject.value(QStringLiteral("base"));

    if (baseValue.isString())
    {
        // Single base
        bases.append(baseValue.toString());
    }
    else if (baseValue.isArray())
    {
        for (const auto &item : baseValue.toArray())
        {
            if (!item.isString())
            {
                qDebug() << DEBUG_METHOD("readDerivedObjectNode")
                         << "Error: unsupported JSON type for an item in the 'base' member at "
                            "path:"
                         << currentNodePath;
                return {};
            }

            bases.append(item.toString());
        }

        if (bases.isEmpty())
        {
            qDebug() << DEBUG_METHOD("readDerivedObjectNode")
                     << "Error: the 'base' member is empty at path:" << currentNodePath;
            return {};
        }
    }
    else
    {
        qDebug() << DEBUG_METHOD("readDerivedObjectNode")
                 << "Error: unsupported JSON type for the 'base' member at path:"
                 << currentNodePath;
        return {};
    }

    // Extract config
    std::unique_ptr<ConfigNode> config = std::make_unique<ConfigNode>(ConfigNode::Type::Null);

    if (jsonObject.contains(QStringLiteral("config")))
    {
        const auto configValue = jsonObject.value(QStringLiteral("config"));

        if (configValue.isObject())
        {
            // Read overrides for the object derived from bases
            config = readJsonObject(configValue.toObject(), currentNodePath);

            if (!config)
            {
                qDebug() << DEBUG_METHOD("readDerivedObjectNode")
                         << "Error: failed to read the overrides for the object derived from bases"
                            "at path:"
                         << currentNodePath;
                return {};
            }
        }
        else if (configValue.isNull())
        {
            // No overrides for the object derived from bases
        }
        else
        {
            qDebug() << DEBUG_METHOD("readDerivedObjectNode")
                     << "Error: unsupported JSON type for the 'config' member at path:"
                     << currentNodePath;
            return {};
        }
    }

    // Create derived object node
    auto node = std::make_unique<ConfigNode>(ConfigNode::Type::DerivedObject);
    node->derivedObject()->setBases(bases);
    node->derivedObject()->config() = std::move(*config);

    return node;
}

// -------------------------------------------------------------------------------------------------

ConfigReader::Impl::ReferenceResolutionResult ConfigReader::Impl::resolveReferences(
        ConfigNode *node)
{
    Q_ASSERT(node != nullptr);

    switch (node->type())
    {
        case ConfigNode::Type::Null:
        case ConfigNode::Type::Value:
        {
            return ReferenceResolutionResult::Resolved;
        }

        case ConfigNode::Type::Array:
        {
            return resolveArrayReferences(node);
        }

        case ConfigNode::Type::Object:
        {
            return resolveObjectReferences(node);
        }

        case ConfigNode::Type::NodeReference:
        {
            return resolveNodeReference(node);
        }

        case ConfigNode::Type::DerivedArray:
        {
            return resolveDerivedArrayReferences(node);
        }

        case ConfigNode::Type::DerivedObject:
        {
            return resolveDerivedObjectReferences(node);
        }

        default:
        {
            break;
        }
    }

    qDebug() << DEBUG_METHOD("resolveReferences")
             << QString("Error: unsupported node type [%1] at path [%2]")
                .arg(ConfigNode::typeToString(node->type()), node->absoluteNodePath());
    return ReferenceResolutionResult::Error;
}

// -------------------------------------------------------------------------------------------------

ConfigReader::Impl::ReferenceResolutionResult ConfigReader::Impl::resolveArrayReferences(
        ConfigNode *node)
{
    Q_ASSERT(node != nullptr);
    ReferenceResolutionResult result = ReferenceResolutionResult::Resolved;

    for (auto *element : node->elements())
    {
        switch (resolveReferences(element))
        {
            case ReferenceResolutionResult::Resolved:
            {
                continue;
            }

            case ReferenceResolutionResult::Unresolved:
            {
                result = ReferenceResolutionResult::Unresolved;
                continue;
            }

            case ReferenceResolutionResult::Error:
            default:
            {
                return ReferenceResolutionResult::Error;
            }
        }
    }

    return result;
}

// -------------------------------------------------------------------------------------------------

ConfigReader::Impl::ReferenceResolutionResult ConfigReader::Impl::resolveObjectReferences(
        ConfigNode *node)
{
    Q_ASSERT(node != nullptr);
    ReferenceResolutionResult result = ReferenceResolutionResult::Resolved;

    for (const auto &memberName : node->memberNames())
    {
        switch (resolveReferences(node->member(memberName)))
        {
            case ReferenceResolutionResult::Resolved:
            {
                continue;
            }

            case ReferenceResolutionResult::Unresolved:
            {
                result = ReferenceResolutionResult::Unresolved;
                continue;
            }

            case ReferenceResolutionResult::Error:
            default:
            {
                return ReferenceResolutionResult::Error;
            }
        }
    }

    return result;
}

// -------------------------------------------------------------------------------------------------

ConfigReader::Impl::ReferenceResolutionResult ConfigReader::Impl::resolveNodeReference(
        ConfigNode *node)
{
    Q_ASSERT(node != nullptr);

    // Try to get the referenced node
    ConfigNode *parentNode = node->parent();

    if (parentNode == nullptr)
    {
        qDebug() << DEBUG_METHOD("resolveReferences")
                 << "Error: reference node has no parent!";
        return ReferenceResolutionResult::Error;
    }

    const ConfigNode *referencedNode = parentNode->nodeAtPath(node->nodeReference());

    if (referencedNode == nullptr)
    {
        // Unable to resolve the node reference (yet)
        return ReferenceResolutionResult::Unresolved;
    }

    *node = std::move(referencedNode->clone());
    node->setParent(parentNode);

    // Check if the resulting node is fully resolved
    return (isFullyResolved(*node) ? ReferenceResolutionResult::Resolved
                                   : ReferenceResolutionResult::Unresolved);
}

// -------------------------------------------------------------------------------------------------

ConfigReader::Impl::ReferenceResolutionResult ConfigReader::Impl::resolveDerivedArrayReferences(
        ConfigNode *node)
{
    Q_ASSERT(node != nullptr);

    // Try to get the parent node
    ConfigNode *parentNode = node->parent();

    if (parentNode == nullptr)
    {
        qDebug() << DEBUG_METHOD("resolveDerivedArrayReferences")
                 << "Error: derived object node has no parent!";
        return ReferenceResolutionResult::Error;
    }

    // Resolve references for each element
    int index = 0;
    auto result = ReferenceResolutionResult::Resolved;

    for (auto &elementNode : *node->derivedArray())
    {
        if (elementNode.isRoot())
        {
            elementNode.setParent(node);
        }

        switch (resolveReferences(&elementNode))
        {
            case ReferenceResolutionResult::Resolved:
            {
                break;
            }

            case ReferenceResolutionResult::Unresolved:
            {
                result = ReferenceResolutionResult::Unresolved;
                break;
            }

            case ReferenceResolutionResult::Error:
            default:
            {
                qDebug() << DEBUG_METHOD("resolveDerivedArrayReferences")
                         << QString("Error: failed to resolve the element node with index [%1] "
                                    "at path [%2]")
                            .arg(QString::number(index), node->absoluteNodePath());
                return ReferenceResolutionResult::Error;
            }
        }

        index++;
    }

    if (result != ReferenceResolutionResult::Resolved)
    {
        return result;
    }

    // References for all elements were resolved, convert this node to an Array node
    auto derivedArrayNode = std::make_unique<ConfigNode>(ConfigNode::Type::Array, parentNode);

    for (const auto &elementNode : *node->derivedArray())
    {
        derivedArrayNode->appendElement(elementNode.clone());
    }

    *node = std::move(*derivedArrayNode);
    return ReferenceResolutionResult::Resolved;
}

// -------------------------------------------------------------------------------------------------

ConfigReader::Impl::ReferenceResolutionResult ConfigReader::Impl::resolveDerivedObjectReferences(
        ConfigNode *node)
{
    Q_ASSERT(node != nullptr);

    // Try to get the parent node
    ConfigNode *parentNode = node->parent();

    if (parentNode == nullptr)
    {
        qDebug() << DEBUG_METHOD("resolveDerivedObjectReferences")
                 << "Error: derived object node has no parent!";
        return ReferenceResolutionResult::Error;
    }

    // Derive the config node from the all of the base nodes
    auto derivedObjectNode = std::make_unique<ConfigNode>(ConfigNode::Type::Object, parentNode);

    for (const QString &base : node->derivedObject()->bases())
    {
        // Try to find the base node
        const ConfigNode *baseNode = parentNode->nodeAtPath(base);

        if (baseNode == nullptr)
        {
            // Unable to find the base node (yet)
            return ReferenceResolutionResult::Unresolved;
        }

        // Check if the base node is fully resolved
        if (!isFullyResolved(*baseNode))
        {
            // Base node is not resolved (yet)
            return ReferenceResolutionResult::Unresolved;
        }

        // Apply the base to the derived object node
        if (!derivedObjectNode->applyObject(*baseNode))
        {
            qDebug() << DEBUG_METHOD("resolveDerivedObjectReferences")
                     << QString("Error: failed to apply the base node [%1] to the derived object "
                                "[%2]!")
                        .arg(baseNode->absoluteNodePath(), node->absoluteNodePath());
            return ReferenceResolutionResult::Error;
        }
    }

    // Check if config override node is fully resolved
    if (!isFullyResolved(node->derivedObject()->config()))
    {
        // Resolve the config override node
        auto configOverride = node->derivedObject()->config().clone();
        configOverride.setParent(parentNode);

        switch (resolveReferences(&configOverride))
        {
            case ReferenceResolutionResult::Resolved:
            {
                // Update the config override with the fully resolved node and continue
                node->derivedObject()->config() = std::move(configOverride);
                break;
            }

            case ReferenceResolutionResult::Unresolved:
            {
                // Update the config override with the partially resolved node and exit
                node->derivedObject()->config() = std::move(configOverride);
                return ReferenceResolutionResult::Unresolved;
            }

            case ReferenceResolutionResult::Error:
            default:
            {
                qDebug() << DEBUG_METHOD("resolveDerivedObjectReferences")
                         << "Error: failed to resolve the config override node at path:"
                         << node->absoluteNodePath();
                return ReferenceResolutionResult::Error;
            }
        }
    }

    // Apply overrides to the derived object
    if (!node->derivedObject()->config().isNull())
    {
        if (!derivedObjectNode->applyObject(node->derivedObject()->config()))
        {
            qDebug() << DEBUG_METHOD("resolveDerivedObjectReferences")
                     << QString("Error: failed to apply the config override node to the derived "
                                "object [%1]!")
                        .arg(node->absoluteNodePath());
            return ReferenceResolutionResult::Error;
        }
    }

    *node = std::move(*derivedObjectNode);
    return ReferenceResolutionResult::Resolved;
}

// -------------------------------------------------------------------------------------------------

bool ConfigReader::Impl::isFullyResolved(const ConfigNode &node)
{
    switch (node.type())
    {
        case ConfigNode::Type::Null:
        case ConfigNode::Type::Value:
        {
            return true;
        }

        case ConfigNode::Type::Array:
        {
            for (const auto *element : node.elements())
            {
                if (!isFullyResolved(*element))
                {
                    return false;
                }
            }

            return true;
        }

        case ConfigNode::Type::Object:
        {
            for (const auto &memberName : node.memberNames())
            {
                if (!isFullyResolved(*node.member(memberName)))
                {
                    return false;
                }
            }

            return true;
        }

        case ConfigNode::Type::NodeReference:
        case ConfigNode::Type::DerivedArray:
        case ConfigNode::Type::DerivedObject:
        {
            return false;
        }
    }

    return false;
}

} // namespace CppConfigFramework
