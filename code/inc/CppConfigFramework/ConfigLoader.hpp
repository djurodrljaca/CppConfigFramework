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
    //! Destructor
    virtual ~ConfigLoader() = default;

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
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadOptionalConfigAtPath(const ConfigNodePath &path,
                                  const ConfigObjectNode &config,
                                  QString *error = nullptr);

    //! \copydoc    ConfigLoader::loadOptionalConfigAtPath()
    bool loadOptionalConfigAtPath(const QString &path,
                                  const ConfigObjectNode &config,
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
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Success
     * \retval  false   Failure
     */
    bool loadOptionalConfig(const QString &parameterName,
                            const ConfigObjectNode &config,
                            QString *error = nullptr);

protected:
    /*!
     * Loads the required configuration parameter from the configuration node without validation
     *
     * \param[out]  parameterValue  Output for the configuration parameter value
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration parameter loading result
     */
    template<typename T>
    bool loadRequiredConfigParameter(
            T *parameterValue,
            const QString &parameterName,
            const ConfigObjectNode &config,
            QString *error = nullptr);

    /*!
     * Loads the required configuration parameter from the configuration node with validation
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
     * \return  Configuration parameter loading result
     */
    template<typename T>
    bool loadRequiredConfigParameter(
            T *parameterValue,
            const QString &parameterName,
            const ConfigObjectNode &config,
            const ConfigParameterValidator<T> &validator,
            QString *error = nullptr);

    /*!
     * Loads the optional configuration parameter from the configuration node without validation
     *
     * \param[out]  parameterValue  Output for the configuration parameter value
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   config          Configuration node from which this configuration structure should be
     *                          loaded
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration parameter loading result
     */
    template<typename T>
    bool loadOptionalConfigParameter(
            T *parameterValue,
            const QString &parameterName,
            const ConfigObjectNode &config,
            QString *error = nullptr);

    /*!
     * Loads the optional configuration parameter from the configuration node with validation
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
     * \return  Configuration parameter loading result
     */
    template<typename T>
    bool loadOptionalConfigParameter(
            T *parameterValue,
            const QString &parameterName,
            const ConfigObjectNode &config,
            const ConfigParameterValidator<T> &validator,
            QString *error = nullptr);

    // TODO: load config container

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
     * \param[out]  parameterValue  Output for the configuration parameter value
     *
     * \param   parameterName   Name of the parameter (member name in the configuration node)
     * \param   node            Configuration node from which this configuration structure should be
     *                          loaded
     * \param   validator       Validator for the loaded parameter value
     *
     * \param[out]  error   Optional output for the error string
     *
     * \return  Configuration parameter loading result
     */
    template<typename T>
    bool loadConfigParameterFromNode(
            T *parameterValue,
            const ConfigNode &node,
            const ConfigParameterValidator<T> &validator,
            QString *error = nullptr);

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
     * for example the error could be writen in a log or the application could be stopped.
     *
     * \note    Default implementation does not do anything!
     */
    virtual void handleError(const QString &error);
};

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigLoader::loadRequiredConfigParameter(
        T *parameterValue,
        const QString &parameterName,
        const ConfigObjectNode &config,
        QString *error)
{
    return loadRequiredConfigParameter(parameterValue,
                                       parameterName,
                                       config,
                                       ConfigParameterValidator<T>(),
                                       error);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigLoader::loadRequiredConfigParameter(
        T *parameterValue,
        const QString &parameterName,
        const ConfigObjectNode &config,
        const ConfigParameterValidator<T> &validator,
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
bool ConfigLoader::loadOptionalConfigParameter(
        T *parameterValue,
        const QString &parameterName,
        const ConfigObjectNode &config,
        QString *error)
{
    return loadOptionalConfigParameter(parameterValue,
                                       parameterName,
                                       config,
                                       ConfigParameterValidator<T>(),
                                       error);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigLoader::loadOptionalConfigParameter(
        T *parameterValue,
        const QString &parameterName,
        const ConfigObjectNode &config,
        const ConfigParameterValidator<T> &validator,
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
        // Node was not found, skip it
        return true;
    }

    // Load configuration parameter from the configuration node
    return loadConfigParameterFromNode(parameterValue, *node, validator, error);
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool ConfigLoader::loadConfigParameterFromNode(
        T *parameterValue,
        const ConfigNode &node,
        const ConfigParameterValidator<T> &validator,
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
                                            "Inner error: [%2]").arg(node.nodePath().path(),
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

    if (!validator.validate(*parameterValue, &validationError))
    {
        const QString errorString = QString("Configuration parameter's value [%1] is not valid. "
                                            "Inner error: [%2]").arg(node.nodePath().path(),
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

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGLOADER_HPP
