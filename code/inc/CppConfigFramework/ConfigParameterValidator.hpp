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
 * This configuration parameter validator does not do any validation, but just returns "true"
 *
 * \tparam  T   Data type of the value to validate
 */
template<typename T>
class ConfigParameterDefaultValidator
{
public:
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
    bool operator()(const T &value, QString *error) const
    {
        Q_UNUSED(value)
        Q_UNUSED(error)
        return true;
    }
};

// -------------------------------------------------------------------------------------------------

/*!
 * This configuration parameter validator checks if the value is in the defined range using the
 * algorithm: minValue ≤ value ≤ maxValue
 *
 * \tparam  T   Data type of the value to validate
 */
template<typename T>
class ConfigParameterRangeValidator
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

    //! \copydoc    ConfigParameterDefaultValidator::operator()()
    bool operator()(const T &value, QString *error) const
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
    const T m_minValue;

    //! Holds the max value
    const T m_maxValue;
};

// -------------------------------------------------------------------------------------------------

/*!
 * This configuration parameter validator checks if the value matches any one from the
 * "valid values" list
 *
 * \tparam  T   Data type of the value to validate
 */
template<typename T>
class ConfigParameterListValidator
{
public:
    /*!
     * Constructor
     *
     * \param   validValues     List of valid values
     */
    ConfigParameterListValidator(const QList<T> &validValues)
        : m_validValues(validValues)
    {
    }

    //! \copydoc    ConfigParameterDefaultValidator::operator()()
    bool operator()(const T &value, QString *error) const
    {
        if (!m_validValues.contains(value))
        {
            if (error != nullptr)
            {
                QStringList allowedValuesPrintable;

                for (const T &item : m_validValues)
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
    //! Holds the list of valid values
    const QList<T> m_validValues;
};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGPARAMETERVALIDATOR_HPP
