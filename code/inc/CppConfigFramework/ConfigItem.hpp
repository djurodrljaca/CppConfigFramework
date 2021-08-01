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
 * Contains a base class for loading configuration nodes to actual configuration parameters and
 * storing actual configuration parameters to configuration nodes
 */

#pragma once

// C++ Config Framework includes
#include <CppConfigFramework/ConfigContainerHelper.hpp>
#include <CppConfigFramework/ConfigParameterValidator.hpp>
#include <CppConfigFramework/ConfigObjectNode.hpp>
#include <CppConfigFramework/ConfigValueNode.hpp>
#include <CppConfigFramework/ConfigWriter.hpp>

// Cedar Framework includes
#include <CedarFramework/Deserialization.hpp>
#include <CedarFramework/Serialization.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

/*!
 * This class holds the base class for loading configuration nodes to actual configuration
 * parameters and storing actual configuration parameters to configuration nodes
 */
class CPPCONFIGFRAMEWORK_EXPORT ConfigItem
{
public:
    /*!
     * Type alias for container item creator
     *
     * \tparam  T   Data type of an item in the container to load (needs to be derived from
     *              ConfigItem class)
     *
     * \param   name    Item name
     *
     * \return  Container item instance
     */
    template<typename T, std::enable_if_t<std::is_base_of<ConfigItem, T>::value, bool> = true>
    using ContainerItemCreator = std::function<T(const QString &name)>;

public:
    //! Constructor
    ConfigItem() = default;

    //! Copy constructor
    ConfigItem(const ConfigItem &) = default;

    //! Move constructor
    ConfigItem(ConfigItem &&) = default;

    //! Destructor
    virtual ~ConfigItem() = default;

    //! Copy assignment operator
    ConfigItem &operator=(const ConfigItem &) = default;

    //! Move assignment operator
    ConfigItem &operator=(ConfigItem &&) = default;

    /*!
     * Loads configuration parameters for this configuration structure
     *
     * \param   config  Configuration node from which this configuration structure should be loaded
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadConfig(const ConfigObjectNode &config);

    /*!
     * Loads configuration parameters for this configuration structure
     *
     * \param   parameterName   Name of the parameter (member in 'config') for this configuration
     *                          structure
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadConfig(const QString &parameterName, const ConfigObjectNode &config);

    /*!
     * Loads configuration parameters for this optional configuration structure
     *
     * \param   parameterName   Name of the parameter (member in 'config') for this configuration
     *                          structure
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \param[out]  loaded  Optional output for the loading result
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadOptionalConfig(const QString &parameterName,
                            const ConfigObjectNode &config,
                            bool *loaded = nullptr);

    /*!
     * Loads configuration parameters for this configuration structure
     *
     * \param   path    Node path to the configuration node for this configuration structure
     * \param   config  Configuration node from which this configuration structure should be loaded
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadConfigAtPath(const ConfigNodePath &path, const ConfigObjectNode &config);

    //! \copydoc    ConfigItem::loadConfigAtPath()
    bool loadConfigAtPath(const QString &path, const ConfigObjectNode &config);

    /*!
     * Loads configuration parameters for this optional configuration structure
     *
     * \param   path    Node path to the configuration node for this configuration structure
     * \param   config  Configuration node from which this configuration structure should be loaded
     *
     * \param[out]  loaded  Optional output for the loading result
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadOptionalConfigAtPath(const ConfigNodePath &path,
                                  const ConfigObjectNode &config,
                                  bool *loaded = nullptr);

    //! \copydoc    ConfigItem::loadOptionalConfigAtPath()
    bool loadOptionalConfigAtPath(const QString &path,
                                  const ConfigObjectNode &config,
                                  bool *loaded = nullptr);

    /*!
     * Stores configuration parameters for this configuration structure
     *
     * \param[out]  config  Configuration node to which this configuration structure should be
     *                      stored to
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool storeConfig(ConfigObjectNode *config);

    /*!
     * Stores configuration parameters for this configuration structure
     *
     * \param   parameterName   Name of the parameter (member in 'config') for this configuration
     *                          structure
     *
     * \param[out]  config  Configuration node to which this configuration structure should be
     *                      stored to
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool storeConfig(const QString &parameterName, ConfigObjectNode *config);

    /*!
     * Stores configuration parameters for this configuration structure
     *
     * \param   path    Node path to the configuration node for this configuration structure
     *
     * \param[out]  config  Configuration node to which this configuration structure should be
     *                      stored to
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool storeConfigAtPath(const ConfigNodePath &path, ConfigObjectNode *config);

    //! \copydoc    ConfigItem::storeConfigAtPath()
    bool storeConfigAtPath(const QString &path, ConfigObjectNode *config);

protected:
    /*!
     * Loads the required configuration parameter from the configuration node without validation
     *
     * \tparam  T   Data type of the parameter to load
     *
     * \param[out]  parameterValue  Output for the configuration parameter value
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadRequiredConfigParameter(T *parameterValue,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config);

    /*!
     * Loads the required configuration parameter from the configuration node with validation
     *
     * \tparam  T   Data type of the parameter to load
     *
     * \param[out]  parameterValue  Output for the configuration parameter value
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     * \param   validator       Validator for the loaded parameter value
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadRequiredConfigParameter(T *parameterValue,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     ConfigParameterValidator<T> validator);

    /*!
     * Loads the optional configuration parameter from the configuration node without validation
     *
     * \tparam  T   Data type of the parameter to load
     *
     * \param[out]  parameterValue  Output for the configuration parameter value
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \param[out]  loaded  Optional output for the loading result
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadOptionalConfigParameter(T *parameterValue,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     bool *loaded = nullptr);

    /*!
     * Loads the optional configuration parameter from the configuration node with validation
     *
     * \tparam  T   Data type of the parameter to load
     *
     * \param[out]  parameterValue  Output for the configuration parameter value
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     * \param   validator       Validator for the loaded parameter value
     *
     * \param[out]  loaded  Optional output for the loading result
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadOptionalConfigParameter(T *parameterValue,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     ConfigParameterValidator<T> validator,
                                     bool *loaded = nullptr);

    /*!
     * Loads the required configuration container from the configuration node
     *
     * \tparam  T   Data type of the container to load (its value type needs to be derived from
     *              ConfigItem class)
     *
     * \param[out]  container   Output for the configuration container
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadRequiredConfigContainer(T *container,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config);

    /*!
     * Loads the required configuration container from the configuration node
     *
     * \tparam  T   Data type of the container to load (its value type needs to be derived from
     *              ConfigItem class)
     *
     * \param[out]  container   Output for the configuration container
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     * \param   itemCreator     Functor for creating the initial item instances for the container
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadRequiredConfigContainer(
            T *container,
            const QString &parameterName,
            const ConfigObjectNode &config,
            ContainerItemCreator<typename ConfigContainerHelper<T>::ItemType> itemCreator);

    /*!
     * Loads the optional configuration container from the configuration node
     *
     * \tparam  T   Data type of the container to load (its value type needs to be derived from
     *              ConfigItem class)
     *
     * \param[out]  container   Output for the configuration container
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \param[out]  loaded  Optional output for the loading result
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadOptionalConfigContainer(T *container,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     bool *loaded = nullptr);

    /*!
     * Loads the optional configuration container from the configuration node
     *
     * \tparam  T   Data type of the container to load (its value type needs to be derived from
     *              ConfigItem class)
     *
     * \param[out]  container   Output for the configuration container
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     * \param   itemCreator     Functor for creating the initial item instances for the container
     *
     * \param[out]  loaded  Optional output for the loading result
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadOptionalConfigContainer(
            T *container,
            const QString &parameterName,
            const ConfigObjectNode &config,
            ContainerItemCreator<typename ConfigContainerHelper<T>::ItemType> itemCreator,
            bool *loaded = nullptr);

    /*!
     * Loads the configuration parameter to the configuration node
     *
     * \tparam  T   Data type of the parameter to store
     *
     * \param   parameterValue  Configuration parameter value
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     *
     * \param[out]  config  Configuration node to which this configuration parameter should be
     *                      stored to
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool storeConfigParameter(const T &parameterValue,
                              const QString &parameterName,
                              ConfigObjectNode *config);

    /*!
     * Stores the configuration container to the configuration node
     *
     * \tparam  T   Data type of the container to store (its value type needs to be derived from
     *              ConfigItem class)
     *
     * \param   container       Configuration container
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     *
     * \param[out]  config  Configuration node to which this configuration parameter should be
     *                      stored to
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool storeConfigContainer(T &container,
                              const QString &parameterName,
                              ConfigObjectNode *config);

    /*!
     * Converts a JSON value to a string in JSON format
     *
     * \param   value   JSON value
     *
     * \return  String representation of the JSON value
     */
    static QString jsonToString(const QJsonValue &value);

private:
    /*!
     * Loads the configuration parameter from the configuration node with validation
     *
     * \tparam  T   Data type of the parameter to load
     *
     * \param[out]  parameterValue  Output for the configuration parameter value
     *
     * \param   node            Configuration node from which this configuration parameter should be
     *                          loaded
     * \param   validator       Validator for the loaded parameter value
     *
     * \return  Configuration parameter loading result
     */
    template<typename T>
    bool loadConfigParameterFromNode(T *parameterValue,
                                     const ConfigNode &node,
                                     ConfigParameterValidator<T> validator);

    /*!
     * Validates the configuration parameter
     *
     * \tparam  T   Data type of the parameter to validate
     *
     * \param   parameterValue  Configuration parameter value
     * \param   node            Configuration node from which this configuration parameter should be
     *                          loaded
     * \param   validator       Validator for the loaded parameter value
     *
     * \return  Configuration parameter loading result
     */
    template<typename T>
    bool validateConfigParameter(const T &parameterValue,
                                 const ConfigNode &node,
                                 ConfigParameterValidator<T> validator);

    /*!
     * Loads the configuration container from the configuration node
     *
     * \tparam  T   Data type of the container to load (its value type needs to be derived from
     *              ConfigItem class)
     *
     * \param[out]  container   Output for the configuration container
     *
     * \param   node        Configuration node from which this configuration container should be
     *                      loaded
     * \param   itemCreator Functor for creating the initial item instances for the container
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadConfigContainerFromNode(
            T *container,
            const ConfigNode &node,
            ContainerItemCreator<typename ConfigContainerHelper<T>::ItemType> itemCreator);

    /*!
     * Default container item creator
     *
     * \tparam  T   Data type of an item in the container to load (needs to be derived from
     *              ConfigItem class)
     *
     * \return  Default constructed item instance
     */
    template<typename T>
    static ContainerItemCreator<T> defaultContainerItemCreator()
    {
        return [](const QString &)
        {
            return T();
        };
    }

    /*!
     * Loads configuration parameters for this configuration structure
     *
     * \param   config  Configuration node from which this configuration parameters should be loaded
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    virtual bool loadConfigParameters(const ConfigObjectNode &config) = 0;

    /*!
     * Stores configuration parameters for this configuration structure
     *
     * \param[out]  config  Configuration node to which this configuration structure should be
     *                      stored to
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    virtual bool storeConfigParameters(ConfigObjectNode *config) = 0;

    /*!
     * Validates the configuration structure
     *
     * \return  Empty string if the configuration structure is valid
     *
     * \note    Default implementation just returns an empty string
     */
    virtual QString validateConfig() const;

    /*!
     * Handle error
     *
     * \param   error   Error string
     *
     * This method shall be called when an error occurs during loading of the configuration
     * structure or a configuration parameter. The configuration class can then react on the error,
     * for example the error could be written in a log or the application could be stopped.
     *
     * \note    Default implementation does not do anything!
     */
    virtual void handleError(const QString &error);
};

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::loadRequiredConfigParameter(T *parameterValue,
                                             const QString &parameterName,
                                             const ConfigObjectNode &config)
{
    return loadRequiredConfigParameter(parameterValue,
                                       parameterName,
                                       config,
                                       { defaultConfigParameterValidator<T> });
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::loadRequiredConfigParameter(T *parameterValue,
                                             const QString &parameterName,
                                             const ConfigObjectNode &config,
                                             ConfigParameterValidator<T> validator)
{
    // Validate parameters
    Q_ASSERT(parameterValue != nullptr);

    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Configuration parameter name [%1] is not valid "
                                            "(configuration node [%2])!")
                                    .arg(parameterName, config.nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Get parameter's configuration node
    const auto *node = config.member(parameterName);

    if (node == nullptr)
    {
        const QString errorString = QString("Configuration parameter node with name [%1] was not "
                                            "found in configuration node [%2]!")
                                    .arg(parameterName, config.nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Load configuration parameter from the configuration node
    return loadConfigParameterFromNode(parameterValue, *node, validator);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::loadOptionalConfigParameter(T *parameterValue,
                                             const QString &parameterName,
                                             const ConfigObjectNode &config,
                                             bool *loaded)
{
    return loadOptionalConfigParameter(parameterValue,
                                       parameterName,
                                       config,
                                       { defaultConfigParameterValidator<T> },
                                       loaded);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::loadOptionalConfigParameter(T *parameterValue,
                                             const QString &parameterName,
                                             const ConfigObjectNode &config,
                                             ConfigParameterValidator<T> validator,
                                             bool *loaded)
{
    // Validate parameters
    Q_ASSERT(parameterValue != nullptr);

    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Configuration parameter name [%1] is not valid "
                                            "(configuration node [%2])!")
                                    .arg(parameterName, config.nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);

        if (loaded != nullptr)
        {
            *loaded = false;
        }
        return false;
    }

    // Get parameter's configuration node
    const auto *node = config.member(parameterName);

    if (node == nullptr)
    {
        // Node was not found, skip it
        if (loaded != nullptr)
        {
            *loaded = false;
        }
        return true;
    }

    // Load configuration parameter from the configuration node
    const bool result = loadConfigParameterFromNode(parameterValue, *node, validator);

    if (loaded != nullptr)
    {
        *loaded = result;
    }
    return result;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::loadRequiredConfigContainer(T *container,
                                             const QString &parameterName,
                                             const ConfigObjectNode &config)
{
    using ItemType = typename ConfigContainerHelper<T>::ItemType;

    return loadRequiredConfigContainer(container,
                                       parameterName,
                                       config,
                                       ConfigItem::defaultContainerItemCreator<ItemType>());
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::loadRequiredConfigContainer(
        T *container,
        const QString &parameterName,
        const ConfigObjectNode &config,
        ContainerItemCreator<typename ConfigContainerHelper<T>::ItemType> itemCreator)
{
    container->clear();

    // Validate parameters
    Q_ASSERT(container != nullptr);

    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Configuration parameter name [%1] is not valid "
                                            "(configuration node [%2])!")
                                    .arg(parameterName, config.nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Get container's configuration node
    const auto *node = config.member(parameterName);

    if (node == nullptr)
    {
        const QString errorString = QString("Configuration parameter node with name [%1] was not "
                                            "found in configuration node [%2]!")
                                    .arg(parameterName, config.nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Load configuration container from the configuration node
    return loadConfigContainerFromNode(container, *node, itemCreator);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::loadOptionalConfigContainer(T *container,
                                             const QString &parameterName,
                                             const ConfigObjectNode &config,
                                             bool *loaded)
{
    using ItemType = typename ConfigContainerHelper<T>::ItemType;

    return loadOptionalConfigContainer(container,
                                       parameterName,
                                       config,
                                       ConfigItem::defaultContainerItemCreator<ItemType>(),
                                       loaded);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::loadOptionalConfigContainer(
        T *container,
        const QString &parameterName,
        const ConfigObjectNode &config,
        ContainerItemCreator<typename ConfigContainerHelper<T>::ItemType> itemCreator,
        bool *loaded)
{
    container->clear();

    // Validate parameters
    Q_ASSERT(container != nullptr);

    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Configuration parameter name [%1] is not valid "
                                            "(configuration node [%2])!")
                                    .arg(parameterName, config.nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Get container's configuration node
    const auto *node = config.member(parameterName);

    if (node == nullptr)
    {
        // Node was not found, skip it
        if (loaded != nullptr)
        {
            *loaded = false;
        }
        return true;
    }

    // Load configuration container from the configuration node
    const bool result = loadConfigContainerFromNode(container, *node, itemCreator);

    if (loaded != nullptr)
    {
        *loaded = result;
    }
    return result;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::storeConfigParameter(const T &parameterValue,
                                      const QString &parameterName,
                                      ConfigObjectNode *config)
{
    // Validate parameters
    Q_ASSERT(config != nullptr);

    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Configuration parameter name [%1] is not valid "
                                            "(configuration node [%2])!")
                                    .arg(parameterName, config->nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Convert parameter value to JSON
    const auto jsonValue = CedarFramework::serialize(parameterValue);

    // Store configuration parameter to the configuration node
    if (!config->setMember(parameterName, std::make_unique<ConfigValueNode>(jsonValue)))
    {
        const QString errorString = QString("Failed to store configuration parameter with name "
                                            "[%1] and value: [%2]")
                                    .arg(parameterName, jsonToString(jsonValue));
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::storeConfigContainer(T &container,
                                      const QString &parameterName,
                                      ConfigObjectNode *config)
{
    // Validate parameters
    Q_ASSERT(config != nullptr);

    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Configuration parameter name [%1] is not valid "
                                            "(configuration node [%2])!")
                                    .arg(parameterName, config->nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Convert container to an Object node
    config->setMember(parameterName, std::make_unique<ConfigObjectNode>());
    ConfigObjectNode *parameterNode = &config->member(parameterName)->toObject();

    for (auto &it : ConfigContainerHelper<T>::toMap(container))
    {
        const QString &key = it.first;
        ConfigItem *containerItem = it.second;

        containerItem->storeConfig(key, parameterNode);
    }

    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::loadConfigParameterFromNode(T *parameterValue,
                                             const ConfigNode &node,
                                             ConfigParameterValidator<T> validator)
{
    // Load the node value to the parameter
    QJsonValue nodeJsonValue;

    switch (node.type())
    {
        case ConfigNode::Type::Value:
        {
            nodeJsonValue = node.toValue().value();
            break;
        }

        case ConfigNode::Type::Object:
        {
            nodeJsonValue = ConfigWriter::convertToJsonValue(node.toObject());

            if (nodeJsonValue.isUndefined())
            {
                const QString errorString = QString("Configuration parameter node [%1] has "
                                                    "unresolved references!")
                                            .arg(node.nodePath().path());
                qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
                handleError(errorString);
                return false;
            }
            break;
        }

        default:
        {
            const QString errorString = QString("Configuration parameter node [%1] is neither a "
                                                "Value nor an Object node!")
                                        .arg(node.nodePath().path());
            qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
            handleError(errorString);
            return false;
        }
    }

    if (!CedarFramework::deserialize(nodeJsonValue, parameterValue))
    {
        const QString errorString = QString("Failed to load configuration parameter's value at "
                                            "node path [%1]").arg(node.nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Validate the value
    if (!validator(*parameterValue))
    {
        const QString errorString = QString("Configuration parameter's value [%1] is not valid")
                                    .arg(node.nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Value is loaded and valid
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigItem::loadConfigContainerFromNode(
        T *container,
        const ConfigNode &node,
        ContainerItemCreator<typename ConfigContainerHelper<T>::ItemType> itemCreator)
{
    if (!node.isObject())
    {
        const QString errorString = QString("Configuration container node [%1] is not an Object"
                                            "node!").arg(node.nodePath().path());
        qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
        handleError(errorString);
        return false;
    }

    // Load individual configuration items from the node object to the container
    const auto &nodeObject = node.toObject();

    for (const QString &itemName : nodeObject.names())
    {
        // Load item's node
        auto item = itemCreator(itemName);
        const auto *itemNode = nodeObject.member(itemName);
        Q_ASSERT(itemNode != nullptr);

        if (!itemNode->isObject())
        {
            const QString errorString = QString("Configuration node [%1] is not an Object node!")
                                        .arg(itemNode->nodePath().path());
            qCWarning(CppConfigFramework::LoggingCategory::ConfigItem) << errorString;
            handleError(errorString);
            return false;
        }

        if (!item.loadConfig(itemNode->toObject()))
        {
            return false;
        }

        // Add the node to the container
        ConfigContainerHelper<T>::addItem(container, itemName, std::move(item));
    }

    return true;
}

} // namespace CppConfigFramework
