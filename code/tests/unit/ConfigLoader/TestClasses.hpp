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
 * Contains test classes
 */

#ifndef CPPCONFIGFRAMEWORK_TESTCLASSES_HPP
#define CPPCONFIGFRAMEWORK_TESTCLASSES_HPP

// C++ Config Framework includes
#include <CppConfigFramework/ConfigLoader.hpp>

// Qt includes
#include <QtCore/QLine>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtCore/QSize>
#include <QtCore/QUrl>
#include <QtCore/QUuid>

// System includes
#include <functional>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

class TestRequiredConfigParameter : public ConfigLoader
{
public:
    bool param;

private:
    bool loadConfigParameters(const ConfigObjectNode &config, QString *error) override;
};

// -------------------------------------------------------------------------------------------------

class TestOptionalConfigParameter : public ConfigLoader
{
public:
    bool param;

private:
    bool loadConfigParameters(const ConfigObjectNode &config, QString *error) override;
};

// -------------------------------------------------------------------------------------------------

//class TestConfigNumeric : public ConfigLoader
//{
//public:
//    bool boolParam;
//    int8_t int8Param;
//    uint8_t uint8Param;
//    int16_t int16Param;
//    uint16_t uint16Param;
//    int32_t int32Param;
//    uint32_t uint32Param;
//    int64_t int64Param;
//    uint64_t uint64Param;
//    float floatParam;
//    double doubleParam;

//private:
//    bool loadConfigParameters(const ConfigObjectNode &config, QString *error) override;
//    QString validateConfig() const override;
//};

//// -------------------------------------------------------------------------------------------------

//class TestConfigString : public ConfigLoader
//{
//public:
//    QChar charParam;
//    QString qstringParam;
//    std::string stdstringParam;
//    std::wstring stdwstringParam;
//    std::u16string stdu16stringParam;
//    std::u32string stdu32stringParam;
//    QByteArray bytearrayParam;
//    QUrl urlParam;
//    QUuid uuidParam;

//private:
//    bool loadConfigParameters(const ConfigObjectNode &config, QString *error) override;
//    QString validateConfig() const override;
//};

//// -------------------------------------------------------------------------------------------------

//class TestConfigOtherPrimitives : public ConfigLoader
//{
//public:
//    QDate dateParam;
//    QTime timeParam;
//    QDateTime datetimeParam;
//    QVariant variantParam;
//    QSize sizeParam;
//    QSizeF sizefParam;
//    QPoint pointParam;
//    QPointF pointfParam;
//    QLine lineParam;
//    QLineF linefParam;
//    QRect rectParam;
//    QRectF rectfParam;

//private:
//    bool loadConfigParameters(const ConfigObjectNode &config, QString *error) override;
//    QString validateConfig() const override;
//};

//// -------------------------------------------------------------------------------------------------

//class TestConfigContainers : public ConfigLoader
//{
//public:
//    QStringList stringlistParam;
//    QPair<QChar, QString> pairParam;
//    std::pair<QDate, QTime> stdpairParam;
//    QList<int8_t> listParam;
//    std::list<uint8_t> stdlistParam;
//    QVector<int16_t> vectorParam;
//    std::vector<uint16_t> stdvectorParam;
//    QMap<int32_t, uint32_t> mapParam;
//    std::map<int32_t, uint32_t> stdmapParam;
//    QHash<int32_t, uint32_t> hashParam;
//    std::unordered_map<int32_t, uint32_t> stdhashParam;
//    QMultiMap<int64_t, uint64_t> multimapParam;
//    QMultiHash<int64_t, uint64_t> multihashParam;

//private:
//    bool loadConfigParameters(const ConfigObjectNode &config, QString *error) override;
//    QString validateConfig() const override;
//};

} // namespace CppConfigFramework

#endif // CPPCONFIGFRAMEWORK_TESTCLASSES_HPP
