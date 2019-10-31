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
 * Contains std::hash template specializations for Qt types
 */

#ifndef CPPCONFIGFRAMEWORK_HASHFUNCTIONS_HPP
#define CPPCONFIGFRAMEWORK_HASHFUNCTIONS_HPP

// C++ Config Framework includes

// Qt includes
#include <QtCore/QDateTime>

// System includes
#include <functional>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

template<> struct std::hash<QChar>
{
    inline size_t operator()(const QChar &value) const noexcept
    {
        return static_cast<size_t>(qHash(value));
    }
};

// -------------------------------------------------------------------------------------------------

template<> struct std::hash<QString>
{
    inline size_t operator()(const QString &value) const noexcept
    {
        return static_cast<size_t>(qHash(value));
    }
};

// -------------------------------------------------------------------------------------------------

template<> struct std::hash<QLatin1String>
{
    inline size_t operator()(const QLatin1String &value) const noexcept
    {
        return static_cast<size_t>(qHash(value));
    }
};

// -------------------------------------------------------------------------------------------------

template<> struct std::hash<QByteArray>
{
    inline size_t operator()(const QByteArray &value) const noexcept
    {
        return static_cast<size_t>(qHash(value));
    }
};

// -------------------------------------------------------------------------------------------------

template<> struct std::hash<QDate>
{
    inline size_t operator()(const QDate &value) const noexcept
    {
        return static_cast<size_t>(qHash(value));
    }
};

// -------------------------------------------------------------------------------------------------

template<> struct std::hash<QTime>
{
    inline size_t operator()(const QTime &value) const noexcept
    {
        return static_cast<size_t>(qHash(value));
    }
};

// -------------------------------------------------------------------------------------------------

template<> struct std::hash<QDateTime>
{
    inline size_t operator()(const QDateTime &value) const noexcept
    {
        return static_cast<size_t>(qHash(value));
    }
};

#endif // CPPCONFIGFRAMEWORK_HASHFUNCTIONS_HPP
