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
 * Contains types to make it easier to work with config containers
 */

#ifndef CPPCONFIGFRAMEWORK_CONFIGCONTAINERHELPER_HPP
#define CPPCONFIGFRAMEWORK_CONFIGCONTAINERHELPER_HPP

// C++ Config Framework includes
#include <CppConfigFramework/CppConfigFrameworkExport.hpp>

// Qt includes
#include <QtCore/QVector>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QHash>

// System includes
#include <functional>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>

// Forward declarations
namespace CppConfigFramework
{
class ConfigLoader;
}

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

template<typename T>
using DerivedFromConfigLoader = std::enable_if_t<std::is_base_of<ConfigLoader, T>::value, T>;

// -------------------------------------------------------------------------------------------------

template<typename T>
using IsMovable = std::enable_if_t<std::is_move_constructible<T>::value, bool>;

// -------------------------------------------------------------------------------------------------

template <typename C>
struct ConfigContainerHelper;

// -------------------------------------------------------------------------------------------------

template <typename CI>
struct ConfigContainerHelper<QVector<CI>>
{
    using ItemType = DerivedFromConfigLoader<CI>;

    static void addItem(QVector<CI> *container, const QString &key, const CI &item)
    {
        Q_UNUSED(key)
        container->append(item);
    }
};

// -------------------------------------------------------------------------------------------------

template <typename CI>
struct ConfigContainerHelper<QList<CI>>
{
    using ItemType = DerivedFromConfigLoader<CI>;

    static void addItem(QList<CI> *container, const QString &key, const CI &item)
    {
        Q_UNUSED(key)
        container->append(item);
    }
};

// -------------------------------------------------------------------------------------------------

template <typename CI>
struct ConfigContainerHelper<QMap<QString, CI>>
{
    using ItemType = DerivedFromConfigLoader<CI>;

    static void addItem(QMap<QString, CI> *container, const QString &key, const CI &item)
    {
        container->insert(key, item);
    }
};

// -------------------------------------------------------------------------------------------------

template <typename CI>
struct ConfigContainerHelper<QHash<QString, CI>>
{
    using ItemType = DerivedFromConfigLoader<CI>;

    static void addItem(QHash<QString, CI> *container, const QString &key, const CI &item)
    {
        container->insert(key, item);
    }
};

// -------------------------------------------------------------------------------------------------

template <typename CI>
struct ConfigContainerHelper<std::vector<CI>>
{
    using ItemType = DerivedFromConfigLoader<CI>;

    static void addItem(std::vector<CI> *container, const QString &key, const CI &item)
    {
        Q_UNUSED(key)
        container->push_back(item);
    }

    template<IsMovable<CI> = true>
    static void addItem(std::vector<QString, CI> *container, const QString &key, CI &&item)
    {
        Q_UNUSED(key)
        container->emplace_back(std::move(item));
    }
};

// -------------------------------------------------------------------------------------------------

template <typename CI>
struct ConfigContainerHelper<std::list<CI>>
{
    using ItemType = DerivedFromConfigLoader<CI>;

    static void addItem(std::list<CI> *container, const QString &key, const CI &item)
    {
        Q_UNUSED(key)
        container->push_back(item);
    }

    template<IsMovable<CI> = true>
    static void addItem(std::list<QString, CI> *container, const QString &key, CI &&item)
    {
        Q_UNUSED(key)
        container->emplace_back(std::move(item));
    }
};

// -------------------------------------------------------------------------------------------------

template <typename CI>
struct ConfigContainerHelper<std::map<QString, CI>>
{
    using ItemType = DerivedFromConfigLoader<CI>;

    static void addItem(std::map<QString, CI> *container, const QString &key, const CI &item)
    {
        container->emplace(key, item);
    }

    template<IsMovable<CI> = true>
    static void addItem(std::map<QString, CI> *container, const QString &key, CI &&item)
    {
        container->emplace(key, std::move(item));
    }
};

// -------------------------------------------------------------------------------------------------

template <typename CI>
struct ConfigContainerHelper<std::unordered_map<QString, CI>>
{
    using ItemType = DerivedFromConfigLoader<CI>;

    static void addItem(std::unordered_map<QString, CI> *container,
                        const QString &key,
                        const CI &item)
    {
        container->emplace(key, item);
    }

    template<IsMovable<CI> = true>
    static void addItem(std::unordered_map<QString, CI> *container,
                        const QString &key,
                        CI &&item)
    {
        container->emplace(key, std::move(item));
    }
};


} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_CONFIGCONTAINERHELPER_HPP
