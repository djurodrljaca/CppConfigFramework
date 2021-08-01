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
 * Contains a base class for reading configurations
 */

// Own header
#include <CppConfigFramework/ConfigReaderBase.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigDerivedObjectNode.hpp>
#include <CppConfigFramework/ConfigNodeReference.hpp>
#include <CppConfigFramework/ConfigObjectNode.hpp>
#include <CppConfigFramework/ConfigValueNode.hpp>
#include <CppConfigFramework/LoggingCategories.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

uint32_t ConfigReaderBase::referenceResolutionMaxCycles() const
{
    return m_referenceResolutionMaxCycles;
}

// -------------------------------------------------------------------------------------------------

void ConfigReaderBase::setReferenceResolutionMaxCycles(const uint32_t referenceResolutionMaxCycles)
{
    m_referenceResolutionMaxCycles = referenceResolutionMaxCycles;
}

// -------------------------------------------------------------------------------------------------

bool ConfigReaderBase::isFullyResolved(const ConfigNode &node)
{
    switch (node.type())
    {
        case ConfigNode::Type::Value:
        {
            return true;
        }

        case ConfigNode::Type::Object:
        {
            const auto &objectNode = node.toObject();

            for (const auto &name : objectNode.names())
            {
                if (!isFullyResolved(*objectNode.member(name)))
                {
                    return false;
                }
            }

            return true;
        }

        default:
        {
            break;
        }
    }

    return false;
}

// -------------------------------------------------------------------------------------------------

QStringList ConfigReaderBase::unresolvedReferences(const ConfigObjectNode &node)
{
    QStringList references;

    // Iterate over all members and add all nodes of a reference type to the list
    for (const QString &name : node.names())
    {
        const auto *member = node.member(name);

        if (member->isNodeReference() || member->isDerivedObject())
        {
            references.append(member->nodePath().path());
        }
        else if (member->isObject())
        {
            references.append(unresolvedReferences(member->toObject()));
        }
        else
        {
            // No unresolved references
        }
    }

    return references;
}

// -------------------------------------------------------------------------------------------------

bool ConfigReaderBase::resolveReferences(
        const std::vector<const ConfigObjectNode *> &externalConfigs,
        ConfigObjectNode *config) const
{
    auto result = ReferenceResolutionResult::Unchanged;
    uint32_t resolutionCycle;

    for (resolutionCycle = 0;
         (resolutionCycle < m_referenceResolutionMaxCycles) &&
         (result != ReferenceResolutionResult::Resolved);
         resolutionCycle++)
    {
        // Try to resolve references without external configuration nodes
        auto newResult = resolveObjectReferences({}, config);

        switch (newResult)
        {
            case ReferenceResolutionResult::Resolved:
            case ReferenceResolutionResult::PartiallyResolved:
            {
                result = newResult;
                break;
            }

            case ReferenceResolutionResult::Unchanged:
            {
                // Check if external configuration nodes are provided
                if (externalConfigs.empty())
                {
                    result = ReferenceResolutionResult::Unchanged;
                    break;
                }

                // External configuration nodes are provided, try to resolve references with them
                newResult = resolveObjectReferences(externalConfigs, config);

                switch (newResult)
                {
                    case ReferenceResolutionResult::Resolved:
                    case ReferenceResolutionResult::PartiallyResolved:
                    {
                        result = newResult;
                        break;
                    }

                    case ReferenceResolutionResult::Unchanged:
                    {
                        // Still unchanged
                        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                                << QString("No references were resolved in the last cycle even "
                                           "after using the external configuration nodes:"
                                           "\n    cycle no.: %1"
                                           "\n    unresolved references: [%2]")
                                   .arg(resolutionCycle)
                                   .arg(unresolvedReferences(*config).join("; "));
                        return false;
                    }

                    case ReferenceResolutionResult::Error:
                    {
                        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                                << QString("Failed to resolve references when using the external "
                                           "configuration nodes:"
                                           "\n    cycle no.: %1"
                                           "\n    unresolved references: [%2]")
                                   .arg(resolutionCycle)
                                   .arg(unresolvedReferences(*config).join("; "));
                        return false;
                    }
                }
                break;
            }

            case ReferenceResolutionResult::Error:
            {
                qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                        << QString("Failed to resolve references:"
                                   "\n    cycle no.: %1"
                                   "\n    unresolved references: [%2]")
                           .arg(resolutionCycle)
                           .arg(unresolvedReferences(*config).join("; "));
                return false;
            }
        }
    }

    if (result != ReferenceResolutionResult::Resolved)
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << QString("Failed to fully resolve references:"
                           "\n    cycle no.: %1"
                           "\n    unresolved references: [%2]")
                   .arg(resolutionCycle)
                   .arg(unresolvedReferences(*config).join("; "));
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

ConfigReaderBase::ReferenceResolutionResult ConfigReaderBase::resolveObjectReferences(
        const std::vector<const ConfigObjectNode *> &externalConfigs, ConfigObjectNode *node)
{
    // Iterate over all members and try to resolve their references
    auto result = ReferenceResolutionResult::Unchanged;

    for (const QString &name : node->names())
    {
        // Try to resolve the member's references
        auto *member = node->member(name);

        switch (member->type())
        {
            case ConfigNode::Type::Value:
            {
                // Not a reference type, leave the result as is
                break;
            }

            case ConfigNode::Type::Object:
            {
                // Try to resolve the member's (Object node) references
                auto newResult = resolveObjectReferences(externalConfigs, &member->toObject());

                result = updateObjectResolutionResult(result, newResult);
                break;
            }

            case ConfigNode::Type::NodeReference:
            {
                // Try to resolve the member's (NodeReference node) reference
                auto newResult = resolveNodeReference(externalConfigs, &member->toNodeReference());

                result = updateObjectResolutionResult(result, newResult);
                break;
            }

            case ConfigNode::Type::DerivedObject:
            {
                // Try to resolve the member's (DerivedObject node) references
                auto newResult = resolveDerivedObjectReferences(externalConfigs,
                                                                &member->toDerivedObject());

                result = updateObjectResolutionResult(result, newResult);
                break;
            }
        }

        // Stop on error
        if (result == ReferenceResolutionResult::Error)
        {
            return ReferenceResolutionResult::Error;
        }
    }

    // Check if the object is fully resolved
    if (isFullyResolved(*node))
    {
        return ReferenceResolutionResult::Resolved;
    }

    return result;
}

// -------------------------------------------------------------------------------------------------

ConfigReaderBase::ReferenceResolutionResult ConfigReaderBase::updateObjectResolutionResult(
        const ConfigReaderBase::ReferenceResolutionResult currentResult,
        const ConfigReaderBase::ReferenceResolutionResult newResult)
{
    auto result = currentResult;

    switch (newResult)
    {
        case ReferenceResolutionResult::Resolved:
        {
            if (result == ReferenceResolutionResult::Unchanged)
            {
                result = ReferenceResolutionResult::PartiallyResolved;
            }
            break;
        }

        case ReferenceResolutionResult::Unchanged:
        {
            if (result == ReferenceResolutionResult::Resolved)
            {
                result = ReferenceResolutionResult::PartiallyResolved;
            }
            break;
        }

        case ReferenceResolutionResult::PartiallyResolved:
        {
            if ((result == ReferenceResolutionResult::Resolved) ||
                (result == ReferenceResolutionResult::Unchanged))
            {
                result = ReferenceResolutionResult::PartiallyResolved;
            }
            break;
        }

        case ReferenceResolutionResult::Error:
        {
            result = ReferenceResolutionResult::Error;
        }
    }

    return result;
}

// -------------------------------------------------------------------------------------------------

ConfigReaderBase::ReferenceResolutionResult ConfigReaderBase::resolveNodeReference(
        const std::vector<const ConfigObjectNode *> &externalConfigs, ConfigNodeReference *node)
{
    // Try to get the referenced node
    auto *parentNode = node->parent();
    const auto *referencedNode = findReferencedConfigNode(node->reference(),
                                                          *parentNode,
                                                          externalConfigs);

    if (referencedNode == nullptr)
    {
        return ReferenceResolutionResult::Unchanged;
    }

    auto result = (isFullyResolved(*referencedNode) ? ReferenceResolutionResult::Resolved
                                                    : ReferenceResolutionResult::PartiallyResolved);

    // Replace the current node with the referenced node
    if (!parentNode->setMember(parentNode->name(*node), *referencedNode))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << QString("Failed to store the resolved NodeReference node [%1] to the parent "
                           "object at node path [%2]")
                   .arg(node->reference().path(), parentNode->nodePath().path());
        return ReferenceResolutionResult::Error;
    }

    return result;
}

// -------------------------------------------------------------------------------------------------

ConfigReaderBase::ReferenceResolutionResult ConfigReaderBase::resolveDerivedObjectReferences(
        const std::vector<const ConfigObjectNode *> &externalConfigs, ConfigDerivedObjectNode *node)
{
    // Derive the config node from the all of the base nodes
    auto *parentNode = node->parent();
    std::vector<const ConfigObjectNode *> baseNodes;
    baseNodes.reserve(node->bases().size());

    for (const auto &baseNodePath : node->bases())
    {
        // Try to find the base node
        const auto *baseNode = findReferencedConfigNode(baseNodePath, *parentNode, externalConfigs);

        if (baseNode == nullptr)
        {
            return ReferenceResolutionResult::Unchanged;
        }

        // Check if the base node is fully resolved
        if (!isFullyResolved(*baseNode))
        {
            // Base node is not resolved (yet)
            return ReferenceResolutionResult::Unchanged;
        }

        // Check if the node is an object
        if (!baseNode->isObject())
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << QString("Base node [%1] in a DerivedObject node [%2] is referencing a "
                               "node that is not an Object node!")
                       .arg(baseNodePath.path(), node->nodePath().path());
            return ReferenceResolutionResult::Error;
        }

        // Store the base node to the temporary container
        baseNodes.push_back(&baseNode->toObject());
    }

    // All the bases are resolved so they can now be applied to an empty object in the listed order
    // to create a derived object node
    ConfigObjectNode derivedObjectNode(parentNode);

    for (const auto *baseNode : baseNodes)
    {
        derivedObjectNode.apply(*baseNode);
    }

    // Apply overrides to the derived object node
    if (node->config().count() > 0)
    {
        derivedObjectNode.apply(node->config());
    }

    auto result = (isFullyResolved(derivedObjectNode)
                   ? ReferenceResolutionResult::Resolved
                   : ReferenceResolutionResult::PartiallyResolved);


    // Replace the current node with the referenced node
    if (!parentNode->setMember(parentNode->name(*node), derivedObjectNode))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                << QString("Failed to store the resolved DerivedObject node [%1] to the parent "
                           "object at node path [%2]")
                   .arg(node->nodePath().path(), parentNode->nodePath().path());
        return ReferenceResolutionResult::Error;
    }

    return result;
}

// -------------------------------------------------------------------------------------------------

const ConfigNode *ConfigReaderBase::findReferencedConfigNode(
        const ConfigNodePath &referenceNodePath,
        const ConfigObjectNode &parentNode,
        const std::vector<const ConfigObjectNode *> &externalConfigs)
{
    const auto *referencedNode = parentNode.nodeAtPath(referenceNodePath);

    if (referencedNode == nullptr)
    {
        // Unable to find the node reference, try to find it in one of the external configuration
        // nodes (use last found node)
        for (const auto *externalConfig : externalConfigs)
        {
            if (referenceNodePath.isAbsolute())
            {
                const auto *foundNode = externalConfig->nodeAtPath(referenceNodePath);

                if (foundNode != nullptr)
                {
                    referencedNode = foundNode;
                }
            }
            else
            {
                // First try to find the equivalent parent node from the external config and then
                // try to get the node using the relative path
                const auto *externalConfigParent =
                        externalConfig->nodeAtPath(parentNode.nodePath());

                if (externalConfigParent != nullptr)
                {
                    const auto *foundNode = externalConfigParent->nodeAtPath(referenceNodePath);

                    if (foundNode != nullptr)
                    {
                        referencedNode = foundNode;
                    }
                }
            }
        }
    }

    return referencedNode;
}

// -------------------------------------------------------------------------------------------------

std::unique_ptr<ConfigObjectNode> ConfigReaderBase::transformConfig(
        std::unique_ptr<ConfigObjectNode> config,
        const ConfigNodePath &sourceNodePath,
        const ConfigNodePath &destinationNodePath)
{
    Q_ASSERT(config != nullptr);
    Q_ASSERT(sourceNodePath.isAbsolute());
    Q_ASSERT(destinationNodePath.isAbsolute());

    // Check if transformation is needed
    if (sourceNodePath.isRoot() && destinationNodePath.isRoot())
    {
        // Transformation is not needed, just return the original configuration node
        return config;
    }

    // Take the source node
    std::unique_ptr<ConfigNode> sourceConfig;

    if (sourceNodePath.isRoot())
    {
        sourceConfig = std::move(config);
    }
    else
    {
        const auto *node = config->nodeAtPath(sourceNodePath);

        if (node == nullptr)
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << "Failed to get the source config node at node path:"
                    << sourceNodePath.path();
            return {};
        }

        sourceConfig = node->clone();
    }

    // For "root" destination just return the source node
    if (destinationNodePath.isRoot())
    {
        if (!sourceConfig->isObject())
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigReader)
                    << "Source config node at node path is not an Object:" << sourceNodePath.path();
            return {};
        }

        return std::make_unique<ConfigObjectNode>(std::move(sourceConfig->toObject()));
    }

    // Create a new config node from the destination node path and set the source config node to it
    auto transformedConfig = std::make_unique<ConfigObjectNode>();

    const QStringList nodeNames = destinationNodePath.nodeNames();
    ConfigObjectNode *currentNode = transformedConfig.get();

    for (int i = 0; i < nodeNames.size(); i++)
    {
        // Create the next node in the node path
        const QString &nodeName = nodeNames.at(i);

        if (i == (nodeNames.size() - 1))
        {
            // For the last element just store the source config node
            currentNode->setMember(nodeName, std::move(sourceConfig));
        }
        else
        {
            // For all intermediate node create an empty Object config node
            currentNode->setMember(nodeName, std::make_unique<ConfigObjectNode>());

            auto *member = currentNode->member(nodeName);
            Q_ASSERT(member != nullptr);
            Q_ASSERT(member->isObject());

            currentNode = &(member->toObject());
        }
    }

    return transformedConfig;
}

} // namespace CppConfigFramework
