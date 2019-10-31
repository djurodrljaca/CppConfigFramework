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
 * Contains classes used for validation of configuration parameters
 */

#ifndef CPPCONFIGFRAMEWORK_CONFIGPARAMETERVALIDATOR_HPP
#define CPPCONFIGFRAMEWORK_CONFIGPARAMETERVALIDATOR_HPP

// C++ Config Framework includes

// Qt includes
#include <QtCore/QVariant>

// System includes
#include <functional>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

/*!
 * This is a base class for a configuration parameter validatior
 *
 * \tparam  T   Data type of the value to validate
 *
 * \note    The default implementation just declares all values as valid
 */
template<typename T>
class ConfigParameterValidator
{
public:
    //! Destructor
    virtual ~ConfigParameterValidator() = default;

    /*!
     * Validates the value
     *
     * \param   value   Value to validate
     *
     * \param[out]  error   Optional output for the error string
     *
     * \retval  true    Value is valid
     * \retval  false   Value is not valid
     */
    virtual bool validate(const T &value, QString *error = nullptr) const
    {
        Q_UNUSED(value)
        Q_UNUSED(error)
        return true;
    }
};

// -------------------------------------------------------------------------------------------------

/*!
 * This configuration parameter validatior checks if the value is in the defined range using the
 * algorithm: minValue ≤ value ≤ maxValue
 *
 * \tparam  T   Data type of the value to validate
 */
template<typename T>
class ConfigParameterRangeValidator : public ConfigParameterValidator<T>
{
public:
    /*!
     * Constructor
     *
     * \param   minValue    Min value
     * \param   maxValue    Max value
     */
    ConfigParameterRangeValidator(const T &minValue, const T &maxValue)
        : m_minValue(minValue),
          m_maxValue(maxValue)
    {
    }

    //! Destructor
    virtual ~ConfigParameterRangeValidator() = default;

    //! \copydoc    ConfigParameterValidator<T>::validate()
    virtual bool validate(const T &value, QString *error = nullptr) const
    {
        if (value < m_minValue)
        {
            if (error != nullptr)
            {
                *error = QString("Value [%1] is less than the min value [%2]!")
                         .arg(value).arg(m_minValue);
            }
            return false;
        }

        if (value > m_maxValue)
        {
            if (error != nullptr)
            {
                *error = QString("Value [%1] is greater than the max value [%2]!")
                         .arg(value).arg(m_maxValue);
            }
            return false;
        }

        return true;
    }

private:
    //! Holds the min value
    const T &m_minValue;

    //! Holds the max value
    const T &m_maxValue;
};

// -------------------------------------------------------------------------------------------------

/*!
 * This configuration parameter validatior checks if the value matches any from the specified list
 *
 * \tparam  T   Data type of the value to validate
 */
template<typename T>
class ConfigParameterListValidator : public ConfigParameterValidator<T>
{
public:
    /*!
     * Constructor
     *
     * \param   allowedValues   List of allowed values
     */
    ConfigParameterListValidator(const QList<T> &allowedValues)
        : m_allowedValues(allowedValues)
    {
    }

    //! Destructor
    virtual ~ConfigParameterListValidator() = default;

    //! \copydoc    ConfigParameterValidator<T>::validate()
    virtual bool validate(const T &value, QString *error = nullptr) const
    {
        if (!m_allowedValues.contains(value))
        {
            if (error != nullptr)
            {
                QStringList allowedValuesPrintable;

                for (const T &item : m_allowedValues)
                {
                    allowedValuesPrintable.append(QString("'%1'").arg(item));
                }

                *error = QString("Value [%1] does not match any of the allowed values [%2]!")
                         .arg(value).arg(allowedValuesPrintable.join(", "));
            }
            return false;
        }

        return true;
    }

private:
    //! Holds the list of allowed values
    const QList<T> &m_allowedValues;
};

// -------------------------------------------------------------------------------------------------

/*!
 * This configuration parameter validatior executes the specified "functor" (function, lambda,
 * "functor" object etc.)
 *
 * \tparam  T   Data type of the value to validate
 */
template<typename T>
class ConfigParameterGenericValidator : public ConfigParameterValidator<T>
{
public:
    //! Functor type
    using Functor = std::function<bool(const T &, QString *)>;

public:
    /*!
     * Constructor
     *
     * \param   functor     Functor to use for validation
     */
    ConfigParameterGenericValidator(Functor functor)
        : m_functor(functor)
    {
    }

    //! Destructor
    virtual ~ConfigParameterGenericValidator() = default;

    //! \copydoc    ConfigParameterValidator<T>::validate()
    virtual bool validate(const T &value, QString *error = nullptr) const
    {
        return m_functor(value, error);
    }

private:
    //! Holds the list of allowed values
    Functor m_functor;
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGPARAMETERVALIDATOR_HPP
