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

#ifndef CPPCONFIGFRAMEWORK_CONFIGPARAMETERLOADER_HPP
#define CPPCONFIGFRAMEWORK_CONFIGPARAMETERLOADER_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigValueNode.hpp>
#include <CppConfigFramework/HashFunctions.hpp>

// Qt includes
#include <QtCore/QVariant>

// System includes
#include <unordered_map>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

namespace ConfigParameterLoader
{

/*!
 * Loads the configuration parameter's value
 *
 * \tparam  T   Parameter value type
 *
 * \param   nodeValue   Configuration parameter's node value
 *
 * \param[out]  parameterValue  Output for the configuration parameter value
 * \param[out]  error           Optional output for the error string
 *
 * \retval  true    Success
 * \retval  false   Failure
 */
template<typename T>
bool load(const QVariant &nodeValue, T *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, bool *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, int8_t *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, uint8_t *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, int16_t *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, uint16_t *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, int32_t *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, uint32_t *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, int64_t *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, uint64_t *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, float *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, double *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QChar *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QString *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QByteArray *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, std::string *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, std::wstring *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, std::u16string *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, std::u32string *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QDate *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QTime *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QDateTime *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QVariant *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QUrl *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QUuid *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QSize *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QSizeF *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QPoint *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QPointF *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QLine *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QLineF *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QRect *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QRectF *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<>
bool load(const QVariant &nodeValue, QStringList *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename T1, typename T2>
bool load(const QVariant &nodeValue, QPair<T1, T2> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename T1, typename T2>
bool load(const QVariant &nodeValue, std::pair<T1, T2> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename T>
bool load(const QVariant &nodeValue, QList<T> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename T>
bool load(const QVariant &nodeValue, std::list<T> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename T>
bool load(const QVariant &nodeValue, QVector<T> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename T>
bool load(const QVariant &nodeValue, std::vector<T> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename T>
bool load(const QVariant &nodeValue, QSet<T> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, QMap<K, V> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, std::map<K, V> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, QHash<K, V> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, std::unordered_map<K, V> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, QMultiMap<K, V> *parameterValue, QString *error);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, QMultiHash<K, V> *parameterValue, QString *error);

// -------------------------------------------------------------------------------------------------
// Template definitions
// -------------------------------------------------------------------------------------------------

template<typename T1, typename T2>
bool load(const QVariant &nodeValue, QPair<T1, T2> *parameterValue, QString *error)
{
    // Needs to be a map with 'first' and 'second' parameters
    if (!nodeValue.canConvert<QVariantMap>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must either be an Object value with only 'first' "
                                    "and 'second' members!");
        }
        return false;
    }

    const auto container = nodeValue.toMap();

    if ((container.size() != 2) ||
        (!container.contains(QStringLiteral("first"))) ||
        (!container.contains(QStringLiteral("second"))))
    {
        if (error != nullptr)
        {
            *error = QString("Node value should have only 'first' and 'second' members but it "
                             "has these [%1]!").arg(container.keys().join(", "));
        }
        return false;
    }

    // Extract first
    T1 first;

    if (!load(container.value(QStringLiteral("first")), &first, error))
    {
        if (error != nullptr)
        {
            *error = QString("The 'first' value is invalid! Inner error: [%1]").arg(*error);
        }
        return false;
    }

    // Extract second
    T2 second;

    if (!load(container.value(QStringLiteral("second")), &second, error))
    {
        if (error != nullptr)
        {
            *error = QString("The 'second' value is invalid! Inner error: [%1]").arg(*error);
        }
        return false;
    }

    // Load the parameter value
    *parameterValue = QPair<T1, T2>(first, second);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T1, typename T2>
bool load(const QVariant &nodeValue, std::pair<T1, T2> *parameterValue, QString *error)
{
    // Needs to be a map with 'first' and 'second' parameters
    if (!nodeValue.canConvert<QVariantMap>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must either be an Object value with only 'first' "
                                    "and 'second' members!");
        }
        return false;
    }

    const auto container = nodeValue.toMap();

    if ((container.size() != 2) ||
        (!container.contains(QStringLiteral("first"))) ||
        (!container.contains(QStringLiteral("second"))))
    {
        if (error != nullptr)
        {
            *error = QString("Node value should have only 'first' and 'second' members but it "
                             "has these [%1]!").arg(container.keys().join(", "));
        }
        return false;
    }

    // Extract first
    T1 first;

    if (!load(container.value(QStringLiteral("first")), &first, error))
    {
        if (error != nullptr)
        {
            *error = QString("The 'first' value is invalid! Inner error: [%1]").arg(*error);
        }
        return false;
    }

    // Extract second
    T2 second;

    if (!load(container.value(QStringLiteral("second")), &second, error))
    {
        if (error != nullptr)
        {
            *error = QString("The 'second' value is invalid! Inner error: [%1]").arg(*error);
        }
        return false;
    }

    // Load the parameter value
    *parameterValue = std::make_pair(first, second);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool load(const QVariant &nodeValue, QList<T> *parameterValue, QString *error)
{
    if (!nodeValue.canConvert<QVariantList>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be an Array!");
        }
        return false;
    }

    auto iterable = nodeValue.value<QSequentialIterable>();
    QList<T> container;
    container.reserve(iterable.size());

    for (const QVariant &item : iterable)
    {
        T value;

        if (!load(item, &value, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the element at index [%1]! Inner error: [%2]")
                         .arg(container.size()).arg(*error);
            }
            return false;
        }

        container.append(value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool load(const QVariant &nodeValue, std::list<T> *parameterValue, QString *error)
{
    if (!nodeValue.canConvert<QVariantList>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be an Array!");
        }
        return false;
    }

    auto iterable = nodeValue.value<QSequentialIterable>();
    std::list<T> container;

    for (const QVariant &item : iterable)
    {
        T value;

        if (!load(item, &value, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the element at index [%1]! Inner error: [%2]")
                         .arg(container.size()).arg(*error);
            }
            return false;
        }

        container.push_back(value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool load(const QVariant &nodeValue, QVector<T> *parameterValue, QString *error)
{
    if (!nodeValue.canConvert<QVariantList>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be an Array!");
        }
        return false;
    }

    auto iterable = nodeValue.value<QSequentialIterable>();
    QVector<T> container;
    container.reserve(iterable.size());

    for (const QVariant &item : iterable)
    {
        T value;

        if (!load(item, &value, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the element at index [%1]! Inner error: [%2]")
                         .arg(container.size()).arg(*error);
            }
            return false;
        }

        container.append(value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool load(const QVariant &nodeValue, std::vector<T> *parameterValue, QString *error)
{
    if (!nodeValue.canConvert<QVariantList>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be an Array!");
        }
        return false;
    }

    auto iterable = nodeValue.value<QSequentialIterable>();
    std::vector<T> container;
    container.reserve(static_cast<size_t>(iterable.size()));

    for (const QVariant &item : iterable)
    {
        T value;

        if (!load(item, &value, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the element at index [%1]! Inner error: [%2]")
                         .arg(container.size()).arg(*error);
            }
            return false;
        }

        container.push_back(value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool load(const QVariant &nodeValue, QSet<T> *parameterValue, QString *error)
{
    if (!nodeValue.canConvert<QVariantList>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be an Array!");
        }
        return false;
    }

    auto iterable = nodeValue.value<QSequentialIterable>();
    QSet<T> container;

    for (const QVariant &item : iterable)
    {
        T value;

        if (!load(item, &value, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the element at index [%1]! Inner error: [%2]")
                         .arg(container.size()).arg(*error);
            }
            return false;
        }

        if (container.contains(value))
        {
            if (error != nullptr)
            {
                *error = QString("The element at index [%1] is a duplicate!").arg(container.size());
            }
            return false;
        }

        container.insert(value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool load(const QVariant &nodeValue, QMap<K, V> *parameterValue, QString *error)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be an Object!");
        }
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    QMap<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's key! Inner error: [%1]").arg(*error);
            }
            return false;
        }

        // Load value
        V value;

        if (!load(it.value(), &value, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's value! Inner error: [%1]")
                         .arg(*error);
            }
            return false;
        }

        container.insert(key, value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool load(const QVariant &nodeValue, std::map<K, V> *parameterValue, QString *error)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be an Object!");
        }
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    std::map<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's key! Inner error: [%1]").arg(*error);
            }
            return false;
        }

        // Load value
        V value;

        if (!load(it.value(), &value, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's value! Inner error: [%1]")
                         .arg(*error);
            }
            return false;
        }

        container.emplace(key, value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool load(const QVariant &nodeValue, QHash<K, V> *parameterValue, QString *error)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be an Object!");
        }
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    QHash<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's key! Inner error: [%1]").arg(*error);
            }
            return false;
        }

        // Load value
        V value;

        if (!load(it.value(), &value, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's value! Inner error: [%1]")
                         .arg(*error);
            }
            return false;
        }

        container.insert(key, value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool load(const QVariant &nodeValue, std::unordered_map<K, V> *parameterValue, QString *error)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be an Object!");
        }
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    std::unordered_map<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's key! Inner error: [%1]").arg(*error);
            }
            return false;
        }

        // Load value
        V value;

        if (!load(it.value(), &value, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's value! Inner error: [%1]")
                         .arg(*error);
            }
            return false;
        }

        container.emplace(key, value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool load(const QVariant &nodeValue, QMultiMap<K, V> *parameterValue, QString *error)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must be an Object!");
        }
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    QMultiMap<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's key! Inner error: [%1]").arg(*error);
            }
            return false;
        }

        // Load values
        QVector<V> values;

        if (!load(it.value(), &values, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's values! Inner error: [%1]")
                         .arg(*error);
            }
            return false;
        }

        for (const V& item : values)
        {
            container.insert(key, item);
        }
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool load(const QVariant &nodeValue, QMultiHash<K, V> *parameterValue, QString *error)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        if (error != nullptr)
        {
            *error = QStringLiteral("Node value must either be an Array!");
        }
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    QMultiHash<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's key! Inner error: [%2]").arg(*error);
            }
            return false;
        }

        // Load values
        QVector<V> values;

        if (!load(it.value(), &values, error))
        {
            if (error != nullptr)
            {
                *error = QString("Failed to load the member's values! Inner error: [%1]")
                         .arg(*error);
            }
            return false;
        }

        for (const V& item : values)
        {
            container.insert(key, item);
        }
    }

    *parameterValue = container;
    return true;
}

} // namespace ConfigParameterLoader

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGPARAMETERLOADER_HPP
