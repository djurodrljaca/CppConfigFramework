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
 * Contains a base class for the loading configuration nodes to actual configuration parameters
 */

#ifndef CPPCONFIGFRAMEWORK_CONFIGLOADER_HPP
#define CPPCONFIGFRAMEWORK_CONFIGLOADER_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigContainerHelper.hpp>
#include <CppConfigFramework/ConfigNode.hpp>
#include <CppConfigFramework/ConfigObjectNode.hpp>
#include <CppConfigFramework/ConfigParameterLoader.hpp>
#include <CppConfigFramework/ConfigParameterValidator.hpp>

// Qt includes

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

//! This class holds the base class for a configuration class
class CPPCONFIGFRAMEWORK_EXPORT ConfigLoader
{
public:
    //! Constructor
    ConfigLoader() = default;

    //! Copy constructor
    ConfigLoader(const ConfigLoader &) = default;

    //! Move constructor
    ConfigLoader(ConfigLoader &&) = default;

    //! Destructor
    virtual ~ConfigLoader() = default;

    //! Copy assignment operator
    ConfigLoader &operator=(const ConfigLoader &) = default;

    //! Move assignment operator
    ConfigLoader &operator=(ConfigLoader &&) = default;

    /*!
     * Loads configuration parameters for this configuration structure
     *
     * \param   path    Node path to the configuration node for this configuration structure
     * \param   config  Configuration node from which this configuration structure should be loaded
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadConfigAtPath(const ConfigNodePath &path,
                          const ConfigObjectNode &config,
                          QString *error = nullptr);

    //! \copydoc    ConfigLoader::loadConfigAtPath()
    bool loadConfigAtPath(const QString &path,
                          const ConfigObjectNode &config,
                          QString *error = nullptr);

    /*!
     * Loads configuration parameters for this optional configuration structure
     *
     * \param   path    Node path to the configuration node for this configuration structure
     * \param   config  Configuration node from which this configuration structure should be loaded
     *
     * \param[out]  loaded  Optional output for the loading result
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadOptionalConfigAtPath(const ConfigNodePath &path,
                                  const ConfigObjectNode &config,
                                  bool *loaded = nullptr,
                                  QString *error = nullptr);

    //! \copydoc    ConfigLoader::loadOptionalConfigAtPath()
    bool loadOptionalConfigAtPath(const QString &path,
                                  const ConfigObjectNode &config,
                                  bool *loaded = nullptr,
                                  QString *error = nullptr);

    /*!
     * Loads configuration parameters for this configuration structure
     *
     * \param   parameterName   Name of the parameter (member in 'config') for this configuration
     *                          structure
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadConfig(const QString &parameterName,
                    const ConfigObjectNode &config,
                    QString *error = nullptr);

    /*!
     * Loads configuration parameters for this optional configuration structure
     *
     * \param   parameterName   Name of the parameter (member in 'config') for this configuration
     *                          structure
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \param[out]  loaded  Optional output for the loading result
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadOptionalConfig(const QString &parameterName,
                            const ConfigObjectNode &config,
                            bool *loaded = nullptr,
                            QString *error = nullptr);

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
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadRequiredConfigParameter(T *parameterValue,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     QString *error = nullptr);

    /*!
     * Loads the required configuration parameter from the configuration node with validation
     *
     * \tparam  T           Data type of the parameter to load
     * \tparam  Validator   Validator must be an executable object with signature:
     *                      bool validator(const T &value, QString *error)
     *
     * \param[out]  parameterValue  Output for the configuration parameter value
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     * \param   validator       Validator for the loaded parameter value
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T, typename Validator>
    bool loadRequiredConfigParameter(T *parameterValue,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     Validator validator,
                                     QString *error = nullptr);

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
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadOptionalConfigParameter(T *parameterValue,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     bool *loaded = nullptr,
                                     QString *error = nullptr);

    /*!
     * Loads the optional configuration parameter from the configuration node with validation
     *
     * \tparam  T           Data type of the parameter to load
     * \tparam  Validator   Validator must be an executable object with signature:
     *                      bool validator(const T &value, QString *error)
     *
     * \param[out]  parameterValue  Output for the configuration parameter value
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     * \param   validator       Validator for the loaded parameter value
     *
     * \param[out]  loaded  Optional output for the loading result
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T, typename Validator>
    bool loadOptionalConfigParameter(T *parameterValue,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     Validator validator,
                                     bool *loaded = nullptr,
                                     QString *error = nullptr);

    /*!
     * Loads the required configuration container from the configuration node
     *
     * \tparam  T   Data type of the container to load (its value type needs to be derived from
     *              ConfigLoader class)
     *
     * \param[out]  container   Output for the configuration container
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadRequiredConfigContainer(T *container,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     QString *error = nullptr);

    /*!
     * Loads the required configuration container from the configuration node
     *
     * \tparam  T           Data type of the container to load (its value type needs to be derived
     *                      from ConfigLoader class)
     * \tparam  ItemCreator Item creator must be an executable object with signature:
     *                      bool itemCreator(const QString &name)
     *
     * \param[out]  container   Output for the configuration container
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     * \param   itemCreator     Functor for creating the initial item instances for the container
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T, typename ItemCreator>
    bool loadRequiredConfigContainer(T *container,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     ItemCreator itemCreator,
                                     QString *error = nullptr);

    /*!
     * Loads the optional configuration container from the configuration node
     *
     * \tparam  T   Data type of the container to load (its value type needs to be derived from
     *              ConfigLoader class)
     *
     * \param[out]  container   Output for the configuration container
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \param[out]  loaded  Optional output for the loading result
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T>
    bool loadOptionalConfigContainer(T *container,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     bool *loaded = nullptr,
                                     QString *error = nullptr);

    /*!
     * Loads the optional configuration container from the configuration node
     *
     * \tparam  T           Data type of the container to load (its value type needs to be derived
     *                      from ConfigLoader class)
     * \tparam  ItemCreator Item creator must be an executable object with signature:
     *                      bool itemCreator(const QString &name)
     *
     * \param[out]  container   Output for the configuration container
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     * \param   itemCreator     Functor for creating the initial item instances for the container
     *
     * \param[out]  loaded  Optional output for the loading result
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T, typename ItemCreator>
    bool loadOptionalConfigContainer(T *container,
                                     const QString &parameterName,
                                     const ConfigObjectNode &config,
                                     ItemCreator itemCreator,
                                     bool *loaded = nullptr,
                                     QString *error = nullptr);

private:
    /*!
     * Loads configuration parameters for this configuration structure
     *
     * \param   node    Configuration node from which this configuration structure should be loaded
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadConfigFromNode(const ConfigNode &node, QString *error = nullptr);

    /*!
     * Loads the configuration parameter from the configuration node with validation
     *
     * \tparam  T           Data type of the parameter to load
     * \tparam  Validator   Validator must be an executable object with signature:
     *                      bool validator(const T &value, QString *error)
     *
     * \param[out]  parameterValue  Output for the configuration parameter value
     *
     * \param   node            Configuration node from which this configuration parameter should be
     *                          loaded
     * \param   validator       Validator for the loaded parameter value
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration parameter loading result
     */
    template<typename T, typename Validator>
    bool loadConfigParameterFromNode(T *parameterValue,
                                     const ConfigNode &node,
                                     Validator validator,
                                     QString *error);

    /*!
     * Validates the configuration parameter
     *
     * \tparam  T           Data type of the parameter to validate
     * \tparam  Validator   Validator must be an executable object with signature:
     *                      bool validator(const T &value, QString *error)
     *
     * \param   parameterValue  Configuration parameter value
     * \param   node            Configuration node from which this configuration parameter should be
     *                          loaded
     * \param   validator       Validator for the loaded parameter value
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration parameter loading result
     */
    template<typename T, typename Validator>
    bool validateConfigParameter(const T &parameterValue,
                                 const ConfigNode &node,
                                 Validator validator,
                                 QString *error);

    /*!
     * Loads the configuration container from the configuration node
     *
     * \tparam  T           Data type of the container to load (its value type needs to be derived
     *                      from ConfigLoader class)
     * \tparam  ItemCreator Item creator must be an executable object with signature:
     *                      bool itemCreator(const QString &name)
     *
     * \param[out]  container   Output for the configuration container
     *
     * \param   node        Configuration node from which this configuration container should be
     *                      loaded
     * \param   itemCreator Functor for creating the initial item instances for the container
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    template<typename T, typename ItemCreator>
    bool loadConfigContainerFromNode(T *container,
                                     const ConfigNode &node,
                                     ItemCreator itemCreator,
                                     QString *error = nullptr);

    /*!
     * Default container item creator
     *
     * \tparam  T   Data type of an item in the container to load (needs to be derived from
     *              ConfigLoader class)
     *
     * \return  Default constructed item instance
     */
    template<typename T>
    static T defaultContainerItemCreator(const QString &)
    {
        return T();
    }

    /*!
     * Loads configuration parameters for this configuration structure
     *
     * \param   config  Configuration node from which this configuration parameters should be loaded
     *
     * \param[out]  error   Output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    virtual bool loadConfigParameters(const ConfigObjectNode &config, QString *error = nullptr) = 0;

    /*!
     * Validates the configuration structure
     *
     * \param[out]  error   Output for the error string
     *
     * \return  Empty string if the configuration structure is valid or the error string
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
bool ConfigLoader::loadRequiredConfigParameter(T *parameterValue,
                                               const QString &parameterName,
                                               const ConfigObjectNode &config,
                                               QString *error)
{
    return loadRequiredConfigParameter(parameterValue,
                                       parameterName,
                                       config,
                                       ConfigParameterDefaultValidator<T>(),
                                       error);
}

// -------------------------------------------------------------------------------------------------

template<typename T, typename Validator>
bool ConfigLoader::loadRequiredConfigParameter(T *parameterValue,
                                               const QString &parameterName,
                                               const ConfigObjectNode &config,
                                               Validator validator,
                                               QString *error)
{
    // Validate parameters
    Q_ASSERT(parameterValue != nullptr);

    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Configuration parameter name [%1] is not valid "
                                            "(configuration node [%2])!")
                                    .arg(parameterName, config.nodePath().path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    // Get parameter's configuration node
    const auto *node = config.member(parameterName);

    if (node == nullptr)
    {
        const QString errorString = QString("Configuration parameter node with name [%1] was not "
                                            "found in configuration node [%2]!")
                                    .arg(parameterName, config.nodePath().path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    // Load configuration parameter from the configuration node
    return loadConfigParameterFromNode(parameterValue, *node, validator, error);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigLoader::loadOptionalConfigParameter(T *parameterValue,
                                               const QString &parameterName,
                                               const ConfigObjectNode &config,
                                               bool *loaded,
                                               QString *error)
{
    return loadOptionalConfigParameter(parameterValue,
                                       parameterName,
                                       config,
                                       ConfigParameterDefaultValidator<T>(),
                                       loaded,
                                       error);
}

// -------------------------------------------------------------------------------------------------

template<typename T, typename Validator>
bool ConfigLoader::loadOptionalConfigParameter(T *parameterValue,
                                               const QString &parameterName,
                                               const ConfigObjectNode &config,
                                               Validator validator,
                                               bool *loaded,
                                               QString *error)
{
    // Validate parameters
    Q_ASSERT(parameterValue != nullptr);

    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Configuration parameter name [%1] is not valid "
                                            "(configuration node [%2])!")
                                    .arg(parameterName, config.nodePath().path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }

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
    const bool result = loadConfigParameterFromNode(parameterValue, *node, validator, error);

    if (loaded != nullptr)
    {
        *loaded = result;
    }
    return result;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigLoader::loadRequiredConfigContainer(T *container,
                                               const QString &parameterName,
                                               const ConfigObjectNode &config,
                                               QString *error)
{
    using ItemType = typename ConfigContainerHelper<T>::ItemType;

    return loadRequiredConfigContainer(container,
                                       parameterName,
                                       config,
                                       ConfigLoader::defaultContainerItemCreator<ItemType>,
                                       error);
}

// -------------------------------------------------------------------------------------------------

template<typename T, typename ItemCreator>
bool ConfigLoader::loadRequiredConfigContainer(T *container,
                                               const QString &parameterName,
                                               const ConfigObjectNode &config,
                                               ItemCreator itemCreator,
                                               QString *error)
{
    container->clear();

    // Validate parameters
    Q_ASSERT(container != nullptr);

    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Configuration parameter name [%1] is not valid "
                                            "(configuration node [%2])!")
                                    .arg(parameterName, config.nodePath().path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    // Get container's configuration node
    const auto *node = config.member(parameterName);

    if (node == nullptr)
    {
        const QString errorString = QString("Configuration parameter node with name [%1] was not "
                                            "found in configuration node [%2]!")
                                    .arg(parameterName, config.nodePath().path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    // Load configuration container from the configuration node
    return loadConfigContainerFromNode(container, *node, itemCreator, error);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigLoader::loadOptionalConfigContainer(T *container,
                                               const QString &parameterName,
                                               const ConfigObjectNode &config,
                                               bool *loaded,
                                               QString *error)
{
    using ItemType = typename ConfigContainerHelper<T>::ItemType;

    return loadOptionalConfigContainer(container,
                                       parameterName,
                                       config,
                                       ConfigLoader::defaultContainerItemCreator<ItemType>,
                                       loaded,
                                       error);
}

// -------------------------------------------------------------------------------------------------

template<typename T, typename ItemCreator>
bool ConfigLoader::loadOptionalConfigContainer(T *container,
                                               const QString &parameterName,
                                               const ConfigObjectNode &config,
                                               ItemCreator itemCreator,
                                               bool *loaded,
                                               QString *error)
{
    container->clear();

    // Validate parameters
    Q_ASSERT(container != nullptr);

    if (!ConfigNodePath::validateNodeName(parameterName))
    {
        const QString errorString = QString("Configuration parameter name [%1] is not valid "
                                            "(configuration node [%2])!")
                                    .arg(parameterName, config.nodePath().path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
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
    const bool result = loadConfigContainerFromNode(container, *node, itemCreator, error);

    if (loaded != nullptr)
    {
        *loaded = result;
    }
    return result;
}

// -------------------------------------------------------------------------------------------------

template<typename T, typename Validator>
bool ConfigLoader::loadConfigParameterFromNode(T *parameterValue,
                                               const ConfigNode &node,
                                               Validator validator,
                                               QString *error)
{
    if (!node.isValue())
    {
        const QString errorString = QString("Configuration parameter node [%1] is not a Value "
                                            "node!").arg(node.nodePath().path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    // Load the node value to the parameter
    QString loadingError;

    if (!ConfigParameterLoader::load(node.toValue().value(), parameterValue, &loadingError))
    {
        const QString errorString = QString("Failed to load configuration parameter's value [%1]. "
                                            "Error: [%2]").arg(node.nodePath().path(),
                                                               loadingError);
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    // Validate the value
    QString validationError;

    if (!validator(*parameterValue, &validationError))
    {
        const QString errorString = QString("Configuration parameter's value [%1] is not "
                                            "valid. Error: [%2]").arg(node.nodePath().path(),
                                                                      validationError);
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
        return false;
    }

    // Value is loaded and valid
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T, typename ItemCreator>
bool ConfigLoader::loadConfigContainerFromNode(T *container,
                                               const ConfigNode &node,
                                               ItemCreator itemCreator,
                                               QString *error)
{
    if (!node.isObject())
    {
        const QString errorString = QString("Configuration container node [%1] is not an Object"
                                            "node!").arg(node.nodePath().path());
        handleError(errorString);

        if (error != nullptr)
        {
            *error = errorString;
        }
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

        if (!item.loadConfigFromNode(*itemNode, error))
        {
            return false;
        }

        // Add the node to the container
        ConfigContainerHelper<T>::addItem(container, itemName, std::move(item));
    }

    return true;
}

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGLOADER_HPP
