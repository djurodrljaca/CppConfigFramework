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
 * Contains classes used for loading of configuration parameter values
 */

// Own header
#include <CppConfigFramework/ConfigParameterLoader.hpp>

// C++ Config Framework includes

// Qt includes
#include <QtCore/QLine>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtCore/QUrl>
#include <QtCore/QUuid>

// System includes

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

// -------------------------------------------------------------------------------------------------
// Helper methods
// -------------------------------------------------------------------------------------------------

namespace Internal
{

template<typename T>
using IsMax64BitInteger = std::enable_if_t<std::is_integral<T>::value && (sizeof(T) <= 8), bool>;

template<typename T>
using IsMax64BitFloatingPoint = std::enable_if_t<
std::is_floating_point<T>::value && (sizeof(T) <= 8),
bool
>;

// -------------------------------------------------------------------------------------------------

template<typename T_IN, IsMax64BitInteger<T_IN> = true>
bool convertBooleanValue(const T_IN inputValue, bool *outputValue, QString *error)
{
    if (inputValue == static_cast<T_IN>(0))
    {
        *outputValue = false;
        return true;
    }

    if (inputValue == static_cast<T_IN>(1))
    {
        *outputValue = true;
        return true;
    }

    *error = QString("Not a valid Boolean value in integral format [%1]! "
                     "Only [0] and [1] values are allowed!").arg(inputValue);
    return false;
}

// -------------------------------------------------------------------------------------------------

bool convertBooleanValue(const double inputValue, bool *outputValue, QString *error)
{
    constexpr double limit = 1e-3;

    // Check if value is very close to 0
    if (std::abs(inputValue) < limit)
    {
        *outputValue = false;
        return true;
    }

    // Check if value is very close to 1
    if (std::abs(inputValue - 1.0) < limit)
    {
        *outputValue = true;
        return true;
    }

    // Error
    if (error != nullptr)
    {
        *error = QString("Not a valid Boolean value in floating-point format [%1]! "
                         "Only [0] and [1] values are allowed!").arg(inputValue);
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

bool convertBooleanValue(const QString &inputValue, bool *outputValue, QString *error)
{
    // Try to extract the value from a string
    if (inputValue == QStringLiteral("false"))
    {
        *outputValue = false;
        return true;
    }

    if (inputValue == QStringLiteral("true"))
    {
        *outputValue = true;
        return true;
    }

    // Try to extract the value from a numeric value
    bool ok = false;
    const auto floatingPointValue = inputValue.toDouble(&ok);

    if (ok)
    {
        if (Internal::convertBooleanValue(floatingPointValue, outputValue, nullptr))
        {
            return true;
        }
    }

    // Error
    if (error != nullptr)
    {
        *error = QString("Not a valid Boolean value in string format [%1]! "
                         "Only [false], [true], [0], and [1] values are allowed!").arg(inputValue);
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<typename T_OUT, IsMax64BitInteger<T_OUT> = true>
bool convertIntegerValue(const int64_t inputValue, T_OUT *outputValue, QString *error)
{
    constexpr auto lowwerLimit = std::numeric_limits<T_OUT>::min();
    constexpr auto upperLimit = std::numeric_limits<T_OUT>::max();

    // Check if output value is also signed
    if (std::is_signed<T_OUT>::value)
    {
        // Output value is also signed, just check the valid range
        if ((inputValue < static_cast<int64_t>(lowwerLimit)) ||
            (inputValue > static_cast<int64_t>(upperLimit)))
        {
            if (error != nullptr)
            {
                *error = QString("Parameter value [%1] is out of range for the "
                                 "its data type (min: [%2], max: [%3])!")
                         .arg(inputValue)
                         .arg(lowwerLimit)
                         .arg(upperLimit);
            }
            return false;
        }
    }
    else
    {
        // Output is unsigned

        // A negative value cannot be stored in the output or a positive value greater than the
        // max value that can be stored in the output
        if ((inputValue < 0LL) ||
            (static_cast<uint64_t>(inputValue) > static_cast<uint64_t>(upperLimit)))
        {
            if (error != nullptr)
            {
                *error = QString("Parameter value [%1] is out of range for the "
                                 "its data type (min: [%2], max: [%3])!")
                         .arg(inputValue)
                         .arg(lowwerLimit)
                         .arg(upperLimit);
            }
            return false;
        }
    }

    *outputValue = static_cast<T_OUT>(inputValue);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T_OUT, IsMax64BitInteger<T_OUT> = true>
bool convertIntegerValue(const uint64_t inputValue, T_OUT *outputValue, QString *error)
{
    constexpr auto lowwerLimit = std::numeric_limits<T_OUT>::min();
    constexpr auto upperLimit = std::numeric_limits<T_OUT>::max();

    // Make sure that the input value can be stored in the output
    if (inputValue > static_cast<uint64_t>(upperLimit))
    {
        if (error != nullptr)
        {
            *error = QString("Parameter value [%1] is out of range for the "
                             "its data type (min: [%2], max: [%3])!")
                     .arg(inputValue)
                     .arg(lowwerLimit)
                     .arg(upperLimit);
        }
        return false;
    }

    *outputValue = static_cast<T_OUT>(inputValue);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T_OUT, IsMax64BitInteger<T_OUT> = true>
bool convertIntegerValue(const double inputValue, T_OUT *outputValue, QString *error)
{
    constexpr auto lowwerLimit = std::numeric_limits<T_OUT>::min();
    constexpr auto upperLimit = std::numeric_limits<T_OUT>::max();

    // Make sure that the input value can be stored in the output
    if ((inputValue < static_cast<double>(lowwerLimit)) ||
        (inputValue > static_cast<double>(upperLimit)))
    {
        if (error != nullptr)
        {
            *error = QString("Parameter value [%1] is out of range for the "
                             "its data type (min: [%2], max: [%3])!")
                     .arg(inputValue)
                     .arg(lowwerLimit)
                     .arg(upperLimit);
        }
        return false;
    }

    *outputValue = static_cast<T_OUT>(inputValue);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T_OUT, IsMax64BitFloatingPoint<T_OUT> = true>
bool convertFloatingPointValue(const double inputValue, T_OUT *outputValue, QString *error)
{
    constexpr auto lowwerLimit = -std::numeric_limits<T_OUT>::max();
    constexpr auto upperLimit = std::numeric_limits<T_OUT>::max();

    // Make sure that the input value can be stored in the output
    if ((inputValue < static_cast<double>(lowwerLimit)) ||
        (inputValue > static_cast<double>(upperLimit)))
    {
        if (error != nullptr)
        {
            *error = QString("Parameter value [%1] is out of range for the "
                             "its data type (min: [%2], max: [%3])!")
                     .arg(inputValue)
                     .arg(lowwerLimit)
                     .arg(upperLimit);
        }
        return false;
    }

    *outputValue = static_cast<T_OUT>(inputValue);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool loadIntegerParameter(const QVariant &nodeValue, T *parameterValue, QString *error)
{
    static_assert(std::is_integral<T>::value, "T must be of an integer type");

    const int nodeValueType = nodeValue.userType();

    if (qMetaTypeId<T>() == nodeValueType)
    {
        // Exactly the same integer type is in the QVariant value
        *parameterValue = nodeValue.value<T>();
        return true;
    }

    // Check for a compatible numeric type in the QVariant value
    switch (nodeValueType)
    {
        case qMetaTypeId<uint8_t>():
        case qMetaTypeId<uint16_t>():
        case qMetaTypeId<uint32_t>():
        case qMetaTypeId<uint64_t>():
        case qMetaTypeId<qulonglong>():
        {
            return convertIntegerValue(nodeValue.value<uint64_t>(), parameterValue, error);
        }

        case qMetaTypeId<int8_t>():
        case qMetaTypeId<int16_t>():
        case qMetaTypeId<int32_t>():
        case qMetaTypeId<int64_t>():
        case qMetaTypeId<qlonglong>():
        {
            return convertIntegerValue(nodeValue.value<int64_t>(), parameterValue, error);
        }

        case qMetaTypeId<float>():
        case qMetaTypeId<double>():
        {
            return convertIntegerValue(nodeValue.value<double>(), parameterValue, error);
        }

        case qMetaTypeId<QByteArray>():
        case qMetaTypeId<QString>():
        {
            if (std::is_signed<T>::value)
            {
                bool ok = false;
                const auto value = nodeValue.toString().toLongLong(&ok);

                if (!ok)
                {
                    if (error != nullptr)
                    {
                        *error = QString("Failed to convert the parameter value [%1] to a "
                                         "signed integer!").arg(nodeValue.toString());
                    }
                    return false;
                }

                return convertIntegerValue(static_cast<int64_t>(value), parameterValue, error);
            }
            else
            {
                bool ok = false;
                const auto value = nodeValue.toString().toULongLong(&ok);

                if (!ok)
                {
                    if (error != nullptr)
                    {
                        *error = QString("Failed to convert the parameter value [%1] to an "
                                         "unsigned integer!").arg(nodeValue.toString());
                    }
                    return false;
                }

                return convertIntegerValue(static_cast<uint64_t>(value), parameterValue, error);
            }
        }

        default:
        {
            break;
        }
    }

    // Error
    if (error != nullptr)
    {
        *error = QStringLiteral("Not a numeric value!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool loadFloatingPointParameter(const QVariant &nodeValue, T *parameterValue, QString *error)
{
    static_assert(std::is_floating_point<T>::value, "T must be of a floating-point type");

    const int nodeValueType = nodeValue.userType();

    if (qMetaTypeId<T>() == nodeValueType)
    {
        // Exactly the same integer type is in the QVariant value
        *parameterValue = nodeValue.value<T>();
        return true;
    }

    // Check for a compatible numeric type in the QVariant value
    switch (nodeValueType)
    {
        case qMetaTypeId<uint8_t>():
        case qMetaTypeId<uint16_t>():
        case qMetaTypeId<uint32_t>():
        case qMetaTypeId<uint64_t>():
        case qMetaTypeId<qulonglong>():
        case qMetaTypeId<int8_t>():
        case qMetaTypeId<int16_t>():
        case qMetaTypeId<int32_t>():
        case qMetaTypeId<int64_t>():
        case qMetaTypeId<qlonglong>():
        case qMetaTypeId<float>():
        case qMetaTypeId<double>():
        {
            return convertFloatingPointValue(nodeValue.value<double>(), parameterValue, error);
        }

        case qMetaTypeId<QByteArray>():
        case qMetaTypeId<QString>():
        {
            bool ok = false;
            const auto value = nodeValue.toString().toDouble(&ok);

            if (!ok)
            {
                if (error != nullptr)
                {
                    *error = QString("Failed to convert the parameter value [%1] to a "
                                     "floating-point value!").arg(nodeValue.toString());
                }
                return false;
            }

            return convertFloatingPointValue(value, parameterValue, error);
        }

        default:
        {
            break;
        }
    }

    // Error
    if (error != nullptr)
    {
        *error = QStringLiteral("Not a numeric value!");
    }
    return false;
}

} // namespace CppConfigFramework


// -------------------------------------------------------------------------------------------------
// Implementation
// -------------------------------------------------------------------------------------------------

namespace ConfigParameterLoader
{

template<>
bool load(const QVariant &nodeValue, bool *parameterValue, QString *error)
{
    switch (nodeValue.userType())
    {
        case qMetaTypeId<bool>():
        {
            *parameterValue = nodeValue.toBool();
            return true;
        }

        case qMetaTypeId<uint8_t>():
        case qMetaTypeId<uint16_t>():
        case qMetaTypeId<uint32_t>():
        case qMetaTypeId<uint64_t>():
        case qMetaTypeId<qulonglong>():
        {
            return Internal::convertBooleanValue(nodeValue.toULongLong(), parameterValue, error);
        }

        case qMetaTypeId<int8_t>():
        case qMetaTypeId<int16_t>():
        case qMetaTypeId<int32_t>():
        case qMetaTypeId<int64_t>():
        case qMetaTypeId<qlonglong>():
        {
            return Internal::convertBooleanValue(nodeValue.toLongLong(), parameterValue, error);
        }

        case qMetaTypeId<float>():
        case qMetaTypeId<double>():
        {
            return Internal::convertBooleanValue(nodeValue.toDouble(), parameterValue, error);
        }

        case qMetaTypeId<QByteArray>():
        case qMetaTypeId<QString>():
        {
            return Internal::convertBooleanValue(nodeValue.toString(), parameterValue, error);
        }

        default:
        {
            break;
        }
    }

    // Error
    if (error != nullptr)
    {
        *error = QStringLiteral("Not a Boolean value!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, int8_t *parameterValue, QString *error)
{
    return Internal::loadIntegerParameter(nodeValue, parameterValue, error);
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, uint8_t *parameterValue, QString *error)
{
    return Internal::loadIntegerParameter(nodeValue, parameterValue, error);
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, int16_t *parameterValue, QString *error)
{
    return Internal::loadIntegerParameter(nodeValue, parameterValue, error);
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, uint16_t *parameterValue, QString *error)
{
    return Internal::loadIntegerParameter(nodeValue, parameterValue, error);
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, int32_t *parameterValue, QString *error)
{
    return Internal::loadIntegerParameter(nodeValue, parameterValue, error);
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, uint32_t *parameterValue, QString *error)
{
    return Internal::loadIntegerParameter(nodeValue, parameterValue, error);
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, int64_t *parameterValue, QString *error)
{
    return Internal::loadIntegerParameter(nodeValue, parameterValue, error);
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, uint64_t *parameterValue, QString *error)
{
    return Internal::loadIntegerParameter(nodeValue, parameterValue, error);
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, float *parameterValue, QString *error)
{
    return Internal::loadFloatingPointParameter(nodeValue, parameterValue, error);
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, double *parameterValue, QString *error)
{
    return Internal::loadFloatingPointParameter(nodeValue, parameterValue, error);
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QChar *parameterValue, QString *error)
{
    const auto nodeValueType = nodeValue.type();

    if (nodeValueType == qMetaTypeId<QChar>())
    {
        *parameterValue = nodeValue.toChar();
        return true;
    }

    if (nodeValueType == qMetaTypeId<QString>())
    {
        const auto value = nodeValue.toString();

        if (value.size() != 1)
        {
            if (error != nullptr)
            {
                *error = QString("String in the node value [%1] must have exactly one character!")
                         .arg(value);
            }
            return false;
        }

        *parameterValue = value.front();
        return true;
    }

    if (nodeValueType == qMetaTypeId<QByteArray>())
    {
        const auto value = nodeValue.toByteArray();

        if (value.size() != 1)
        {
            if (error != nullptr)
            {
                *error = QString("String in the node value [%1] must have exactly one character!")
                         .arg(QString::fromUtf8(value));
            }
            return false;
        }

        *parameterValue = QChar(value.front());
        return true;
    }

    // Error
    if (error != nullptr)
    {
        *error = QStringLiteral("Not a character value");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QString *parameterValue, QString *error)
{
    // Check for the same type
    if (static_cast<QMetaType::Type>(nodeValue.type()) == QMetaType::QString)
    {
        *parameterValue = nodeValue.toString();
        return true;
    }

    // Check for a compatible numeric type in the QVariant value
    switch (nodeValue.userType())
    {
        case qMetaTypeId<int8_t>():
        case qMetaTypeId<int16_t>():
        case qMetaTypeId<int32_t>():
        case qMetaTypeId<int64_t>():
        case qMetaTypeId<qlonglong>():
        {
            *parameterValue = QString::number(nodeValue.toLongLong());
            return true;
        }

        case qMetaTypeId<uint8_t>():
        case qMetaTypeId<uint16_t>():
        case qMetaTypeId<uint32_t>():
        case qMetaTypeId<uint64_t>():
        case qMetaTypeId<qulonglong>():
        {
            *parameterValue = QString::number(nodeValue.toULongLong());
            return true;
        }

        case qMetaTypeId<float>():
        {
            *parameterValue = QString::number(nodeValue.toFloat());
            return true;
        }

        case qMetaTypeId<double>():
        {
            *parameterValue = QString::number(nodeValue.toDouble());
            return true;
        }

        default:
        {
            break;
        }
    }

    // Just convert it to a string if possible
    if (!nodeValue.canConvert<QString>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be convertible to a string!");
        }
        return false;
    }

    *parameterValue = nodeValue.toString();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QByteArray *parameterValue, QString *error)
{
    // Check for the same type
    if (static_cast<QMetaType::Type>(nodeValue.type()) == QMetaType::QByteArray)
    {
        *parameterValue = nodeValue.toByteArray();
        return true;
    }

    // Convert it to string first and then to UTF8 encoded byte array
    QString value;

    if (!load(nodeValue, &value, nullptr))
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be convertible to a byte array!");
        }
        return false;
    }

    *parameterValue = value.toUtf8();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, std::string *parameterValue, QString *error)
{
    // Convert it to string first and then to the std string
    QString value;

    if (!load(nodeValue, &value, error))
    {
        return false;
    }

    *parameterValue = value.toStdString();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, std::wstring *parameterValue, QString *error)
{
    // Convert it to string first and then to the std string
    QString value;

    if (!load(nodeValue, &value, error))
    {
        return false;
    }

    *parameterValue = value.toStdWString();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, std::u16string *parameterValue, QString *error)
{
    // Convert it to string first and then to the std string
    QString value;

    if (!load(nodeValue, &value, error))
    {
        return false;
    }

    *parameterValue = value.toStdU16String();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, std::u32string *parameterValue, QString *error)
{
    // Convert it to string first and then to the std string
    QString value;

    if (!load(nodeValue, &value, error))
    {
        return false;
    }

    *parameterValue = value.toStdU32String();
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QDate *parameterValue, QString *error)
{
    // Check for the same type
    if (static_cast<QMetaType::Type>(nodeValue.type()) == QMetaType::QDate)
    {
        const auto value = nodeValue.toDate();

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid date object!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    // Convert it to string first and then to date
    QString stringValue;

    if (load(nodeValue, &stringValue, nullptr))
    {
        const auto value = QDate::fromString(stringValue, Qt::ISODate);

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QString("Node value [%1] does not contain a valid date object in ISO 8601 "
                                 "format!").arg(nodeValue.toString());
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must be either a date object or a date string in "
                                "ISO 8601 format!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QTime *parameterValue, QString *error)
{
    // Check for the same type
    if (static_cast<QMetaType::Type>(nodeValue.type()) == QMetaType::QTime)
    {
        const auto value = nodeValue.toTime();

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid time object!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    // Convert it to string first and then to time
    QString stringValue;

    if (load(nodeValue, &stringValue, nullptr))
    {
        const auto value = QTime::fromString(stringValue, Qt::ISODateWithMs);

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QString("Node value [%1] does not contain a valid time object in ISO 8601 "
                                 "format!").arg(nodeValue.toString());
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must be either a time object or a time string in "
                                "ISO 8601 format!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QDateTime *parameterValue, QString *error)
{
    // Check for the same type
    if (static_cast<QMetaType::Type>(nodeValue.type()) == QMetaType::QDateTime)
    {
        const auto value = nodeValue.toDateTime();

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid datetime object!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    // Convert it to string first and then to datetime
    QString stringValue;

    if (load(nodeValue, &stringValue, nullptr))
    {
        const auto value = QDateTime::fromString(stringValue, Qt::ISODateWithMs);

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QString("Node value [%1] does not contain a valid datetime object in "
                                 "ISO 8601 format!").arg(nodeValue.toString());
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must be either a datetime object or a datetime string "
                                "in ISO 8601 format!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QVariant *parameterValue, QString *error)
{
    Q_UNUSED(error)
    *parameterValue = nodeValue;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QUrl *parameterValue, QString *error)
{
    const auto nodeValueType = static_cast<QMetaType::Type>(nodeValue.type());

    // Check for the same type
    if (nodeValueType == QMetaType::QUrl)
    {
        const auto value = nodeValue.toUrl();

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid URL!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    // Convert it to string first and then to URL
    if (nodeValueType == QMetaType::QString)
    {
        const auto value = QUrl(nodeValue.toString());

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QString("Node value [%1] is not a valid URL").arg(nodeValue.toString());
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must be a valid URL!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QUuid *parameterValue, QString *error)
{
    const auto nodeValueType = static_cast<QMetaType::Type>(nodeValue.type());

    // Check for the same type
    if (nodeValueType == QMetaType::QUuid)
    {
        const auto value = nodeValue.toUuid();

        if (value.isNull())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid UUID!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    // Convert it to string first and then to URL
    if (nodeValueType == QMetaType::QString)
    {
        const auto value = QUuid(nodeValue.toString());

        if (value.isNull())
        {
            if (error != nullptr)
            {
                *error = QString("Node value [%1] is not a valid UUID (format: "
                                 "'{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}')")
                         .arg(nodeValue.toString());
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must be a valid UUID object or a string in format "
                                "'{xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx}'!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QSize *parameterValue, QString *error)
{
    const auto nodeValueType = static_cast<QMetaType::Type>(nodeValue.type());

    // Check for size types
    if ((nodeValueType == QMetaType::QSize) ||
        (nodeValueType == QMetaType::QSizeF))
    {
        const auto value = nodeValue.toSize();

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid size object!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    // Extract the value from a map
    if (nodeValue.canConvert<QVariantMap>())
    {
        // Needs to be a map with 'width' and 'height' parameters
        const auto container = nodeValue.toMap();

        if ((container.size() != 2) ||
            (!container.contains(QStringLiteral("width"))) ||
            (!container.contains(QStringLiteral("height"))))
        {
            if (error != nullptr)
            {
                *error = QString("Node value should have only 'width' and 'height' members but it "
                                 "has these [%1]!").arg(container.keys().join(", "));
            }
            return false;
        }

        // Extract width
        int width;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("width")),
                                            &width,
                                            error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'width' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract height
        int height;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("height")),
                                            &height,
                                            error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'height' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        const QSize value(width, height);

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid size object!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must either be a size object an Object container with "
                                "only 'width' and 'height' members!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QSizeF *parameterValue, QString *error)
{
    const auto nodeValueType = static_cast<QMetaType::Type>(nodeValue.type());

    // Check for size types
    if ((nodeValueType == QMetaType::QSize) ||
        (nodeValueType == QMetaType::QSizeF))
    {
        const auto value = nodeValue.toSizeF();

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid size object!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    // Extract the value from a map
    if (nodeValue.canConvert<QVariantMap>())
    {
        // Needs to be a map with 'width' and 'height' parameters
        const auto container = nodeValue.toMap();

        if ((container.size() != 2) ||
            (!container.contains(QStringLiteral("width"))) ||
            (!container.contains(QStringLiteral("height"))))
        {
            if (error != nullptr)
            {
                *error = QString("Node value should have only 'width' and 'height' members but it "
                                 "has these [%1]!").arg(container.keys().join(", "));
            }
            return false;
        }

        // Extract width
        qreal width;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("width")),
                                                  &width,
                                                  error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'width' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract height
        qreal height;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("height")),
                                                  &height,
                                                  error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'height' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        const QSizeF value(width, height);

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid size object!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must either be a size object an Object container with "
                                "only 'width' and 'height' members!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QPoint *parameterValue, QString *error)
{
    const auto nodeValueType = static_cast<QMetaType::Type>(nodeValue.type());

    // Check for point types
    if ((nodeValueType == QMetaType::QPoint) ||
        (nodeValueType == QMetaType::QPointF))
    {
        *parameterValue = nodeValue.toPoint();
        return true;
    }

    // Extract the value from a map
    if (nodeValue.canConvert<QVariantMap>())
    {
        // Needs to be a map with 'x' and 'y' parameters
        const auto container = nodeValue.toMap();

        if ((container.size() != 2) ||
            (!container.contains(QStringLiteral("x"))) ||
            (!container.contains(QStringLiteral("y"))))
        {
            if (error != nullptr)
            {
                *error = QString("Node value should have only 'x' and 'y' members but it has these "
                                 "[%1]!").arg(container.keys().join(", "));
            }
            return false;
        }

        // Extract x
        int x;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("x")), &x, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'x' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract y
        int y;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("y")), &y, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'y' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        *parameterValue = QPoint(x, y);
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must either be a point object an Object container with "
                                "only 'x' and 'y' members!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QPointF *parameterValue, QString *error)
{
    const auto nodeValueType = static_cast<QMetaType::Type>(nodeValue.type());

    // Check for point types
    if ((nodeValueType == QMetaType::QPoint) ||
        (nodeValueType == QMetaType::QPointF))
    {
        *parameterValue = nodeValue.toPointF();
        return true;
    }

    // Extract the value from a map
    if (nodeValue.canConvert<QVariantMap>())
    {
        // Needs to be a map with 'x' and 'y' parameters
        const auto container = nodeValue.toMap();

        if ((container.size() != 2) ||
            (!container.contains(QStringLiteral("x"))) ||
            (!container.contains(QStringLiteral("y"))))
        {
            if (error != nullptr)
            {
                *error = QString("Node value should have only 'x' and 'y' members but it has these "
                                 "[%1]!").arg(container.keys().join(", "));
            }
            return false;
        }

        // Extract x
        qreal x;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("x")), &x, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'x' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract y
        qreal y;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("y")), &y, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'y' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        *parameterValue = QPointF(x, y);
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must either be a point object an Object container with "
                                "only 'x' and 'y' members!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QLine *parameterValue, QString *error)
{
    const auto nodeValueType = static_cast<QMetaType::Type>(nodeValue.type());

    // Check for line types
    if ((nodeValueType == QMetaType::QLine) ||
        (nodeValueType == QMetaType::QLineF))
    {
        *parameterValue = nodeValue.toLine();
        return true;
    }

    // Extract the value from a map
    if (nodeValue.canConvert<QVariantMap>())
    {
        // Needs to be a map with 'x1', 'y1', 'x2', and 'y2' parameters
        const auto container = nodeValue.toMap();

        if ((container.size() != 4) ||
            (!container.contains(QStringLiteral("x1"))) ||
            (!container.contains(QStringLiteral("y1"))) ||
            (!container.contains(QStringLiteral("x2"))) ||
            (!container.contains(QStringLiteral("y2"))))
        {
            if (error != nullptr)
            {
                *error = QString("Node value should have only 'x1', 'y1', 'x2', and 'y2' members "
                                 "but it has these [%1]!").arg(container.keys().join(", "));
            }
            return false;
        }

        // Extract x1
        int x1;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("x1")), &x1, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'x1' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract y1
        int y1;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("y1")), &y1, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'y1' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract x2
        int x2;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("x2")), &x2, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'x2' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract y2
        int y2;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("y2")), &y2, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'y2' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        *parameterValue = QLine(x1, y1, x2, y2);
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must either be a line object an Object container with "
                                "only 'x1', 'y1', 'x2', and 'y2' members!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QLineF *parameterValue, QString *error)
{
    const auto nodeValueType = static_cast<QMetaType::Type>(nodeValue.type());

    // Check for line types
    if ((nodeValueType == QMetaType::QLine) ||
        (nodeValueType == QMetaType::QLineF))
    {
        *parameterValue = nodeValue.toLineF();
        return true;
    }

    // Extract the value from a map
    if (nodeValue.canConvert<QVariantMap>())
    {
        // Needs to be a map with 'x1', 'y1', 'x2', and 'y2' parameters
        const auto container = nodeValue.toMap();

        if ((container.size() != 4) ||
            (!container.contains(QStringLiteral("x1"))) ||
            (!container.contains(QStringLiteral("y1"))) ||
            (!container.contains(QStringLiteral("x2"))) ||
            (!container.contains(QStringLiteral("y2"))))
        {
            if (error != nullptr)
            {
                *error = QString("Node value should have only 'x1', 'y1', 'x2', and 'y2' members "
                                 "but it has these [%1]!").arg(container.keys().join(", "));
            }
            return false;
        }

        // Extract x1
        qreal x1;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("x1")),
                                                  &x1,
                                                  error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'x1' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract y1
        qreal y1;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("y1")),
                                                  &y1,
                                                  error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'y1' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract x2
        qreal x2;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("x2")),
                                                  &x2,
                                                  error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'x2' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract y2
        qreal y2;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("y2")),
                                                  &y2,
                                                  error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'y2' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        *parameterValue = QLineF(x1, y1, x2, y2);
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must either be a line object an Object container with "
                                "only 'x1', 'y1', 'x2', and 'y2' members!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QRect *parameterValue, QString *error)
{
    const auto nodeValueType = static_cast<QMetaType::Type>(nodeValue.type());

    // Check for rect types
    if ((nodeValueType == QMetaType::QRect) ||
        (nodeValueType == QMetaType::QRectF))
    {
        const auto value = nodeValue.toRect();

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid rect object!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    // Extract the value from a map
    if (nodeValue.canConvert<QVariantMap>())
    {
        // Needs to be a map with 'x', 'y', 'width', and 'height' parameters
        const auto container = nodeValue.toMap();

        if ((container.size() != 4) ||
            (!container.contains(QStringLiteral("x"))) ||
            (!container.contains(QStringLiteral("y"))) ||
            (!container.contains(QStringLiteral("width"))) ||
            (!container.contains(QStringLiteral("height"))))
        {
            if (error != nullptr)
            {
                *error = QString("Node value should have only 'x', 'y', 'width', and 'height' "
                                 "members but it has these [%1]!").arg(container.keys().join(", "));
            }
            return false;
        }

        // Extract x
        int x;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("x")), &x, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'x' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract y
        int y;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("y")), &y, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'y' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract width
        int width;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("width")),
                                            &width,
                                            error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'width' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract height
        int height;

        if (!Internal::loadIntegerParameter(container.value(QStringLiteral("height")),
                                            &height,
                                            error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'height' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        const QRect value(x, y, width, height);

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid rect object!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must either be a rect object or an Object value with "
                                "only 'x', 'y', 'width', and 'height' members!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QRectF *parameterValue, QString *error)
{
    const auto nodeValueType = static_cast<QMetaType::Type>(nodeValue.type());

    // Check for rect types
    if ((nodeValueType == QMetaType::QRect) ||
        (nodeValueType == QMetaType::QRectF))
    {
        const auto value = nodeValue.toRectF();

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid rect object!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    // Extract the value from a map
    if (nodeValue.canConvert<QVariantMap>())
    {
        // Needs to be a map with 'x', 'y', 'width', and 'height' parameters
        const auto container = nodeValue.toMap();

        if ((container.size() != 4) ||
            (!container.contains(QStringLiteral("x"))) ||
            (!container.contains(QStringLiteral("y"))) ||
            (!container.contains(QStringLiteral("width"))) ||
            (!container.contains(QStringLiteral("height"))))
        {
            if (error != nullptr)
            {
                *error = QString("Node value should have only 'x', 'y', 'width', and 'height' "
                                 "members but it has these [%1]!").arg(container.keys().join(", "));
            }
            return false;
        }

        // Extract x
        qreal x;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("x")), &x, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'x' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract y
        qreal y;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("y")), &y, error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'y' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract width
        qreal width;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("width")),
                                                  &width,
                                                  error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'width' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        // Extract height
        qreal height;

        if (!Internal::loadFloatingPointParameter(container.value(QStringLiteral("height")),
                                                  &height,
                                                  error))
        {
            if (error != nullptr)
            {
                *error = QString("The 'height' value is invalid! Error: [%1]").arg(*error);
            }
            return false;
        }

        const QRectF value(x, y, width, height);

        if (!value.isValid())
        {
            if (error != nullptr)
            {
                *error = QStringLiteral("Node value contains an invalid rect object!");
            }
            return false;
        }

        *parameterValue = value;
        return true;
    }

    if (error != nullptr)
    {
        *error = QStringLiteral("Node value must either be a rect object or an Object value with "
                                "only 'x', 'y', 'width', and 'height' members!");
    }
    return false;
}

// -------------------------------------------------------------------------------------------------

template<>
bool load(const QVariant &nodeValue, QStringList *parameterValue, QString *error)
{
    // Check for the same type
    if (static_cast<QMetaType::Type>(nodeValue.type()) == QMetaType::QStringList)
    {
        *parameterValue = nodeValue.toStringList();
        return true;
    }

    // Extract the value from a list
    QList<QString> value;

    if (!load(nodeValue, &value, error))
    {
        return false;
    }

    *parameterValue = value;
    return true;
}

} // namespace ConfigParameterLoader

} // namespace CppConfigFramework
