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
 * Contains methods used for loading of configuration parameter values
 */

#ifndef CPPCONFIGFRAMEWORK_CONFIGPARAMETERLOADER_HPP
#define CPPCONFIGFRAMEWORK_CONFIGPARAMETERLOADER_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigValueNode.hpp>
#include <CppConfigFramework/LoggingCategories.hpp>

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
 *
 * \retval  true    Success
 * \retval  false   Failure
 */
template<typename T>
bool load(const QVariant &nodeValue, T *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, bool *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, int8_t *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, uint8_t *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, int16_t *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, uint16_t *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, int32_t *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, uint32_t *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, int64_t *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, uint64_t *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, float *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, double *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QChar *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QString *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QByteArray *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, std::string *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, std::wstring *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, std::u16string *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, std::u32string *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QDate *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QTime *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QDateTime *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QVariant *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QUrl *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QUuid *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QSize *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QSizeF *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QPoint *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QPointF *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QLine *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QLineF *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QRect *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QRectF *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<>
CPPCONFIGFRAMEWORK_EXPORT bool load(const QVariant &nodeValue, QStringList *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename T1, typename T2>
bool load(const QVariant &nodeValue, QPair<T1, T2> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename T1, typename T2>
bool load(const QVariant &nodeValue, std::pair<T1, T2> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename T>
bool load(const QVariant &nodeValue, QList<T> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename T>
bool load(const QVariant &nodeValue, std::list<T> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename T>
bool load(const QVariant &nodeValue, QVector<T> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename T>
bool load(const QVariant &nodeValue, std::vector<T> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename T>
bool load(const QVariant &nodeValue, QSet<T> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, QMap<K, V> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, std::map<K, V> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, QHash<K, V> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, std::unordered_map<K, V> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, QMultiMap<K, V> *parameterValue);

//! \copydoc    ConfigParameterLoader::load()
template<typename K, typename V>
bool load(const QVariant &nodeValue, QMultiHash<K, V> *parameterValue);

// -------------------------------------------------------------------------------------------------
// Template definitions
// -------------------------------------------------------------------------------------------------

template<typename T1, typename T2>
bool load(const QVariant &nodeValue, QPair<T1, T2> *parameterValue)
{
    // Needs to be a map with 'first' and 'second' parameters
    if (!nodeValue.canConvert<QVariantMap>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must either be an Object value with only 'first' and 'second' "
                   "members!";
        return false;
    }

    const auto container = nodeValue.toMap();

    if ((container.size() != 2) ||
        (!container.contains(QStringLiteral("first"))) ||
        (!container.contains(QStringLiteral("second"))))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value should have only 'first' and 'second' members but it has these:"
                << container.keys().join(", ");
        return false;
    }

    // Extract first
    T1 first;

    if (!load(container.value(QStringLiteral("first")), &first))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "The 'first' value is invalid!";
        return false;
    }

    // Extract second
    T2 second;

    if (!load(container.value(QStringLiteral("second")), &second))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "The 'second' value is invalid!";
        return false;
    }

    // Load the parameter value
    *parameterValue = QPair<T1, T2>(first, second);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T1, typename T2>
bool load(const QVariant &nodeValue, std::pair<T1, T2> *parameterValue)
{
    // Needs to be a map with 'first' and 'second' parameters
    if (!nodeValue.canConvert<QVariantMap>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must either be an Object value with only 'first' and 'second' "
                   "members!";
        return false;
    }

    const auto container = nodeValue.toMap();

    if ((container.size() != 2) ||
        (!container.contains(QStringLiteral("first"))) ||
        (!container.contains(QStringLiteral("second"))))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value should have only 'first' and 'second' members but it has these:"
                << container.keys().join(", ");
        return false;
    }

    // Extract first
    T1 first;

    if (!load(container.value(QStringLiteral("first")), &first))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "The 'first' value is invalid!";
        return false;
    }

    // Extract second
    T2 second;

    if (!load(container.value(QStringLiteral("second")), &second))
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "The 'second' value is invalid!";
        return false;
    }

    // Load the parameter value
    *parameterValue = std::make_pair(first, second);
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool load(const QVariant &nodeValue, QList<T> *parameterValue)
{
    if (!nodeValue.canConvert<QVariantList>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must be an Array!";
        return false;
    }

    auto iterable = nodeValue.value<QSequentialIterable>();
    QList<T> container;
    container.reserve(iterable.size());

    for (const QVariant &item : iterable)
    {
        T value;

        if (!load(item, &value))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the element at index:" << container.size();
            return false;
        }

        container.append(value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool load(const QVariant &nodeValue, std::list<T> *parameterValue)
{
    if (!nodeValue.canConvert<QVariantList>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must be an Array!";
        return false;
    }

    auto iterable = nodeValue.value<QSequentialIterable>();
    std::list<T> container;

    for (const QVariant &item : iterable)
    {
        T value;

        if (!load(item, &value))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the element at index:" << container.size();
            return false;
        }

        container.push_back(value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool load(const QVariant &nodeValue, QVector<T> *parameterValue)
{
    if (!nodeValue.canConvert<QVariantList>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must be an Array!";
        return false;
    }

    auto iterable = nodeValue.value<QSequentialIterable>();
    QVector<T> container;
    container.reserve(iterable.size());

    for (const QVariant &item : iterable)
    {
        T value;

        if (!load(item, &value))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the element at index:" << container.size();
            return false;
        }

        container.append(value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool load(const QVariant &nodeValue, std::vector<T> *parameterValue)
{
    if (!nodeValue.canConvert<QVariantList>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must be an Array!";
        return false;
    }

    auto iterable = nodeValue.value<QSequentialIterable>();
    std::vector<T> container;
    container.reserve(static_cast<size_t>(iterable.size()));

    for (const QVariant &item : iterable)
    {
        T value;

        if (!load(item, &value))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the element at index:" << container.size();
            return false;
        }

        container.push_back(value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename T>
bool load(const QVariant &nodeValue, QSet<T> *parameterValue)
{
    if (!nodeValue.canConvert<QVariantList>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must be an Array!";
        return false;
    }

    auto iterable = nodeValue.value<QSequentialIterable>();
    QSet<T> container;

    for (const QVariant &item : iterable)
    {
        T value;

        if (!load(item, &value))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the element at index:" << container.size();
            return false;
        }

        if (container.contains(value))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "The element at index is a duplicate:" << container.size();
            return false;
        }

        container.insert(value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool load(const QVariant &nodeValue, QMap<K, V> *parameterValue)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must be an Object!";
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    QMap<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's key!";
            return false;
        }

        // Load value
        V value;

        if (!load(it.value(), &value))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's value!";
            return false;
        }

        container.insert(key, value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool load(const QVariant &nodeValue, std::map<K, V> *parameterValue)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must be an Object!";
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    std::map<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's key!";
            return false;
        }

        // Load value
        V value;

        if (!load(it.value(), &value))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's value!";
            return false;
        }

        container.emplace(key, value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool load(const QVariant &nodeValue, QHash<K, V> *parameterValue)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must be an Object!";
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    QHash<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's key!";
            return false;
        }

        // Load value
        V value;

        if (!load(it.value(), &value))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's value!";
            return false;
        }

        container.insert(key, value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool load(const QVariant &nodeValue, std::unordered_map<K, V> *parameterValue)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must be an Object!";
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    std::unordered_map<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's key!";
            return false;
        }

        // Load value
        V value;

        if (!load(it.value(), &value))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's value!";
            return false;
        }

        container.emplace(key, value);
    }

    *parameterValue = container;
    return true;
}

// -------------------------------------------------------------------------------------------------

template<typename K, typename V>
bool load(const QVariant &nodeValue, QMultiMap<K, V> *parameterValue)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must be an Object!";
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    QMultiMap<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's key!";
            return false;
        }

        // Load values
        QVector<V> values;

        if (!load(it.value(), &values))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's values!";
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
bool load(const QVariant &nodeValue, QMultiHash<K, V> *parameterValue)
{
    if (!nodeValue.canConvert<QVariantMap>())
    {
        qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                << "Node value must be an Object!";
        return false;
    }

    auto iterable = nodeValue.value<QAssociativeIterable>();
    QMultiHash<K, V> container;

    for (auto it = iterable.begin(); it != iterable.end(); it++)
    {
        // Load key
        K key;

        if (!load(it.key(), &key))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's key!";
            return false;
        }

        // Load values
        QVector<V> values;

        if (!load(it.value(), &values))
        {
            qCWarning(CppConfigFramework::LoggingCategory::ConfigParameterLoader)
                    << "Failed to load the member's values!";
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
