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
 * Contains unit tests for ConfigParameterLoader class
 */

// C++ Config Framework includes
#include <CppConfigFramework/ConfigParameterLoader.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test helper methods -----------------------------------------------------------------------------

template<typename T>
using IsInteger = std::enable_if_t<std::is_integral<T>::value, bool>;

template<typename T>
using IsFloatingPoint = std::enable_if_t<std::is_floating_point<T>::value, bool>;

template<typename T>
using IsNumeric = std::enable_if_t<
(std::is_integral<T>::value || std::is_floating_point<T>::value),
bool
>;

template<typename T_OUT, typename T_IN, IsNumeric<T_OUT> = true, IsNumeric<T_IN> = true>
static T_OUT CastNumeric(const T_IN value)
{
    return static_cast<T_OUT>(value);
}

// -------------------------------------------------------------------------------------------------

template<typename T, IsInteger<T> = true>
static T MinNumeric()
{
    return std::numeric_limits<T>::min();
}

// -------------------------------------------------------------------------------------------------

template<typename T, IsFloatingPoint<T> = true>
static T MinNumeric()
{
    return static_cast<T>(-std::numeric_limits<T>::max());
}

// -------------------------------------------------------------------------------------------------

template<typename T, IsNumeric<T> = true>
static T MaxNumeric()
{
    return std::numeric_limits<T>::max();
}

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;

class TestConfigParameterLoader : public QObject
{
    Q_OBJECT

private slots:
    // Functions executed by QtTest before and after test suite
    void initTestCase();
    void cleanupTestCase();

    // Functions executed by QtTest before and after each test
    void init();
    void cleanup();

    // Test functions
    void testBool();
    void testBool_data();

    void testInt8();
    void testInt8_data();

    void testUInt8();
    void testUInt8_data();

    void testInt16();
    void testInt16_data();

    void testUInt16();
    void testUInt16_data();

    void testInt32();
    void testInt32_data();

    void testUInt32();
    void testUInt32_data();

    void testInt64();
    void testInt64_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestConfigParameterLoader::initTestCase()
{
}

void TestConfigParameterLoader::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestConfigParameterLoader::init()
{
}

void TestConfigParameterLoader::cleanup()
{
}

// Test: load bool value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testBool()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(bool, expectedResult);
    QFETCH(bool, expectedParameterValue);

    bool parameterValue = false;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testBool: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testBool_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<bool>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("false") << QVariant(false) << false << true;
    QTest::newRow("true") << QVariant(true) << true << true;

    QTest::newRow("int") << QVariant(1) << false << false;
    QTest::newRow("uint") << QVariant(1u) << false << false;
    QTest::newRow("double") << QVariant(1.0) << false << false;
    QTest::newRow("string") << QVariant(QString("true")) << false << false;
}

// Test: load int8 value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testInt8()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(int8_t, expectedParameterValue);
    QFETCH(bool, expectedResult);

    int8_t parameterValue = 0;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testInt8: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testInt8_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<int8_t>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // int8
    QTest::newRow("int8: min") << QVariant::fromValue(MinNumeric<int8_t>())
                               << MinNumeric<int8_t>()
                               << true;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << CastNumeric<int8_t>(0)
                             << true;
    QTest::newRow("int8: max") << QVariant::fromValue(MaxNumeric<int8_t>())
                               << MaxNumeric<int8_t>()
                               << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << CastNumeric<int8_t>(0)
                              << true;
    QTest::newRow("uint8: max valid")
            << QVariant::fromValue(CastNumeric<uint8_t>(MaxNumeric<int8_t>()))
            << MaxNumeric<int8_t>()
            << true;
    QTest::newRow("uint8: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint8_t>(MaxNumeric<int8_t>() + 1))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("uint8: max invalid 2") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                          << CastNumeric<int8_t>(0)
                                          << false;

    // int16
    QTest::newRow("int16: min invalid 1") << QVariant::fromValue(MinNumeric<int16_t>())
                                          << CastNumeric<int8_t>(0)
                                          << false;
    QTest::newRow("int16: min invalid 2")
            << QVariant::fromValue(CastNumeric<int16_t>(MinNumeric<int8_t>() - 1))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("int16: min valid")
            << QVariant::fromValue(CastNumeric<int16_t>(MinNumeric<int8_t>()))
            << MinNumeric<int8_t>()
            << true;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << CastNumeric<int8_t>(0)
                              << true;
    QTest::newRow("int16: max valid")
            << QVariant::fromValue(CastNumeric<int16_t>(MaxNumeric<int8_t>()))
            << MaxNumeric<int8_t>()
            << true;
    QTest::newRow("int16: max invalid 1")
            << QVariant::fromValue(CastNumeric<int16_t>(MaxNumeric<int8_t>() + 1))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("int16: max invalid 2") << QVariant::fromValue(MaxNumeric<int16_t>())
                                          << CastNumeric<int8_t>(0)
                                          << false;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << CastNumeric<int8_t>(0)
                               << true;
    QTest::newRow("uint16: max valid")
            << QVariant::fromValue(CastNumeric<uint16_t>(MaxNumeric<int8_t>()))
            << MaxNumeric<int8_t>()
            << true;
    QTest::newRow("uint16: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint16_t>(MaxNumeric<int8_t>() + 1))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("uint16: max invalid 2") << QVariant::fromValue(MaxNumeric<uint16_t>())
                                           << CastNumeric<int8_t>(0)
                                           << false;

    // int32
    QTest::newRow("int32: min invalid 1") << QVariant::fromValue(MinNumeric<int32_t>())
                                          << CastNumeric<int8_t>(0)
                                          << false;
    QTest::newRow("int32: min invalid 2")
            << QVariant::fromValue(CastNumeric<int32_t>(MinNumeric<int8_t>() - 1))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("int32: min valid")
            << QVariant::fromValue(CastNumeric<int32_t>(MinNumeric<int8_t>()))
            << MinNumeric<int8_t>()
            << true;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << CastNumeric<int8_t>(0)
                              << true;
    QTest::newRow("int32: max valid")
            << QVariant::fromValue(CastNumeric<int32_t>(MaxNumeric<int8_t>()))
            << MaxNumeric<int8_t>()
            << true;
    QTest::newRow("int32: max invalid 1")
            << QVariant::fromValue(CastNumeric<int32_t>(MaxNumeric<int8_t>() + 1))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("int32: max invalid 2") << QVariant::fromValue(MaxNumeric<int32_t>())
                                          << CastNumeric<int8_t>(0)
                                          << false;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << CastNumeric<int8_t>(0)
                               << true;
    QTest::newRow("uint32: max valid")
            << QVariant::fromValue(CastNumeric<uint32_t>(MaxNumeric<int8_t>()))
            << MaxNumeric<int8_t>()
            << true;
    QTest::newRow("uint32: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint32_t>(MaxNumeric<int8_t>() + 1))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("uint32: max invalid 2") << QVariant::fromValue(MaxNumeric<uint32_t>())
                                           << CastNumeric<int8_t>(0)
                                           << false;

    // int64
    QTest::newRow("int64: min invalid 1") << QVariant::fromValue(MinNumeric<int64_t>())
                                          << CastNumeric<int8_t>(0)
                                          << false;
    QTest::newRow("int64: min invalid 2")
            << QVariant::fromValue(CastNumeric<int64_t>(MinNumeric<int8_t>() - 1LL))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("int64: min valid")
            << QVariant::fromValue(CastNumeric<int64_t>(MinNumeric<int8_t>()))
            << MinNumeric<int8_t>()
            << true;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << CastNumeric<int8_t>(0)
                              << true;
    QTest::newRow("int64: max valid")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<int8_t>()))
            << MaxNumeric<int8_t>()
            << true;
    QTest::newRow("int64: max invalid 1")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<int8_t>() + 1LL))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("int64: max invalid 2") << QVariant::fromValue(MaxNumeric<int64_t>())
                                          << CastNumeric<int8_t>(0)
                                          << false;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << CastNumeric<int8_t>(0)
                               << true;
    QTest::newRow("uint64: max valid")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<int8_t>()))
            << MaxNumeric<int8_t>()
            << true;
    QTest::newRow("uint64: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<int8_t>() + 1ULL))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("uint64: max invalid 2") << QVariant::fromValue(MaxNumeric<uint64_t>())
                                           << CastNumeric<int8_t>(0)
                                           << false;

    // float
    QTest::newRow("float: min invalid 1") << QVariant::fromValue(MinNumeric<float>())
                                          << CastNumeric<int8_t>(0)
                                          << false;
    QTest::newRow("float: min invalid 2")
            << QVariant::fromValue(CastNumeric<float>(MinNumeric<int8_t>() - 1.0f))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("float: min valid")
            << QVariant::fromValue(CastNumeric<float>(MinNumeric<int8_t>()))
            << MinNumeric<int8_t>()
            << true;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << CastNumeric<int8_t>(0)
                              << true;
    QTest::newRow("float: max valid")
            << QVariant::fromValue(CastNumeric<float>(MaxNumeric<int8_t>()))
            << MaxNumeric<int8_t>()
            << true;
    QTest::newRow("float: max invalid 1")
            << QVariant::fromValue(CastNumeric<float>(MaxNumeric<int8_t>() + 1.0f))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("float: max invalid 2") << QVariant::fromValue(MaxNumeric<float>())
                                          << CastNumeric<int8_t>(0)
                                          << false;

    // double
    QTest::newRow("double: min invalid 1") << QVariant::fromValue(MinNumeric<double>())
                                           << CastNumeric<int8_t>(0)
                                           << false;
    QTest::newRow("double: min invalid 2")
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<int8_t>() - 1))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("double: min valid")
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<int8_t>()))
            << MinNumeric<int8_t>()
            << true;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                              << CastNumeric<int8_t>(0)
                              << true;
    QTest::newRow("double: max valid")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<int8_t>()))
            << MaxNumeric<int8_t>()
            << true;
    QTest::newRow("double: max invalid 1")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<int8_t>() + 1))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("double: max invalid 2") << QVariant::fromValue(MaxNumeric<double>())
                                           << CastNumeric<int8_t>(0)
                                           << false;

    // byte array
    QTest::newRow("byte array: min invalid")
            << QVariant::fromValue(QByteArray::number(MinNumeric<int8_t>() - 1))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("byte array: min valid")
            << QVariant::fromValue(QByteArray::number(MinNumeric<int8_t>()))
            << MinNumeric<int8_t>()
            << true;
    QTest::newRow("byte array: 0") << QVariant::fromValue(QByteArray::number(0))
                                   << CastNumeric<int8_t>(0)
                                   << true;
    QTest::newRow("byte array: max valid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<int8_t>()))
            << MaxNumeric<int8_t>()
            << true;
    QTest::newRow("byte array: max invalid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<int8_t>() + 1))
            << CastNumeric<int8_t>(0)
            << false;

    // string
    QTest::newRow("string: min invalid")
            << QVariant::fromValue(QString::number(MinNumeric<int8_t>() - 1))
            << CastNumeric<int8_t>(0)
            << false;
    QTest::newRow("string: min valid") << QVariant::fromValue(QString::number(MinNumeric<int8_t>()))
                                       << MinNumeric<int8_t>()
                                       << true;
    QTest::newRow("string: 0") << QVariant::fromValue(QString::number(0))
                               << CastNumeric<int8_t>(0)
                               << true;
    QTest::newRow("string: max valid") << QVariant::fromValue(QString::number(MaxNumeric<int8_t>()))
                                       << MaxNumeric<int8_t>()
                                       << true;
    QTest::newRow("string: max invalid")
            << QVariant::fromValue(QString::number(MaxNumeric<int8_t>() + 1))
            << CastNumeric<int8_t>(0)
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QChar('0'))
                                   << CastNumeric<int8_t>(0)
                                   << false;
}

// Test: load uint8 value --------------------------------------------------------------------------

void TestConfigParameterLoader::testUInt8()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(uint8_t, expectedParameterValue);
    QFETCH(bool, expectedResult);

    uint8_t parameterValue = 0U;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testUInt8: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testUInt8_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<uint8_t>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // int8
    QTest::newRow("int8: min invalid") << QVariant::fromValue(MinNumeric<int8_t>())
                                       << CastNumeric<uint8_t>(0)
                                       << false;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << CastNumeric<uint8_t>(0)
                             << true;
    QTest::newRow("int8: max") << QVariant::fromValue(MaxNumeric<int8_t>())
                               << CastNumeric<uint8_t>(MaxNumeric<int8_t>())
                               << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << CastNumeric<uint8_t>(0)
                              << true;
    QTest::newRow("uint8: max") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                << MaxNumeric<uint8_t>()
                                << true;

    // int16
    QTest::newRow("int16: min invalid 1") << QVariant::fromValue(MinNumeric<int16_t>())
                                          << CastNumeric<uint8_t>(0)
                                          << false;
    QTest::newRow("int16: min invalid 2") << QVariant::fromValue(CastNumeric<int16_t>(-1))
                                          << CastNumeric<uint8_t>(0)
                                          << false;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << CastNumeric<uint8_t>(0)
                              << true;
    QTest::newRow("int16: max valid")
            << QVariant::fromValue(CastNumeric<int16_t>(MaxNumeric<uint8_t>()))
            << MaxNumeric<uint8_t>()
            << true;
    QTest::newRow("int16: max invalid 1")
            << QVariant::fromValue(CastNumeric<int16_t>(MaxNumeric<uint8_t>() + 1))
            << CastNumeric<uint8_t>(0)
            << false;
    QTest::newRow("int16: max invalid 2") << QVariant::fromValue(MaxNumeric<int16_t>())
                                          << CastNumeric<uint8_t>(0)
                                          << false;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << CastNumeric<uint8_t>(0)
                               << true;
    QTest::newRow("uint16: max valid")
            << QVariant::fromValue(CastNumeric<uint16_t>(MaxNumeric<uint8_t>()))
            << MaxNumeric<uint8_t>()
            << true;
    QTest::newRow("uint16: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint16_t>(MaxNumeric<uint8_t>() + 1))
            << CastNumeric<uint8_t>(0)
            << false;
    QTest::newRow("uint16: max invalid 2") << QVariant::fromValue(MaxNumeric<uint16_t>())
                                           << CastNumeric<uint8_t>(0)
                                           << false;

    // int32
    QTest::newRow("int32: min invalid 1") << QVariant::fromValue(MinNumeric<int32_t>())
                                          << CastNumeric<uint8_t>(0)
                                          << false;
    QTest::newRow("int32: min invalid 2") << QVariant::fromValue(CastNumeric<int32_t>(-1))
                                          << CastNumeric<uint8_t>(0)
                                          << false;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << CastNumeric<uint8_t>(0)
                              << true;
    QTest::newRow("int32: max valid")
            << QVariant::fromValue(CastNumeric<int32_t>(MaxNumeric<uint8_t>()))
            << MaxNumeric<uint8_t>()
            << true;
    QTest::newRow("int32: max invalid 1")
            << QVariant::fromValue(CastNumeric<int32_t>(MaxNumeric<uint8_t>() + 1))
            << CastNumeric<uint8_t>(0)
            << false;
    QTest::newRow("int32: max invalid 2") << QVariant::fromValue(MaxNumeric<int32_t>())
                                          << CastNumeric<uint8_t>(0)
                                          << false;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << CastNumeric<uint8_t>(0)
                               << true;
    QTest::newRow("uint32: max valid")
            << QVariant::fromValue(CastNumeric<uint32_t>(MaxNumeric<uint8_t>()))
            << MaxNumeric<uint8_t>()
            << true;
    QTest::newRow("uint32: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint32_t>(MaxNumeric<uint8_t>() + 1))
            << CastNumeric<uint8_t>(0)
            << false;
    QTest::newRow("uint32: max invalid 2") << QVariant::fromValue(MaxNumeric<uint32_t>())
                                           << CastNumeric<uint8_t>(0)
                                           << false;

    // int64
    QTest::newRow("int64: min invalid 1") << QVariant::fromValue(MinNumeric<int64_t>())
                                          << CastNumeric<uint8_t>(0)
                                          << false;
    QTest::newRow("int64: min invalid 2") << QVariant::fromValue(CastNumeric<int64_t>(-1LL))
                                          << CastNumeric<uint8_t>(0)
                                          << false;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << CastNumeric<uint8_t>(0)
                              << true;
    QTest::newRow("int64: max valid")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<uint8_t>()))
            << MaxNumeric<uint8_t>()
            << true;
    QTest::newRow("int64: max invalid 1")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<uint8_t>() + 1LL))
            << CastNumeric<uint8_t>(0)
            << false;
    QTest::newRow("int64: max invalid 2") << QVariant::fromValue(MaxNumeric<int64_t>())
                                          << CastNumeric<uint8_t>(0)
                                          << false;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << CastNumeric<uint8_t>(0)
                               << true;
    QTest::newRow("uint64: max valid")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<uint8_t>()))
            << MaxNumeric<uint8_t>()
            << true;
    QTest::newRow("uint64: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<uint8_t>() + 1ULL))
            << CastNumeric<uint8_t>(0)
            << false;
    QTest::newRow("uint64: max invalid 2") << QVariant::fromValue(MaxNumeric<uint64_t>())
                                           << CastNumeric<uint8_t>(0)
                                           << false;

    // float
    QTest::newRow("float: min invalid 1") << QVariant::fromValue(MinNumeric<float>())
                                          << CastNumeric<uint8_t>(0)
                                          << false;
    QTest::newRow("float: min invalid 2") << QVariant::fromValue(CastNumeric<float>(-1))
                                          << CastNumeric<uint8_t>(0)
                                          << false;
    QTest::newRow("float: min valid")
            << QVariant::fromValue(CastNumeric<float>(MinNumeric<uint8_t>()))
            << MinNumeric<uint8_t>()
            << true;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << CastNumeric<uint8_t>(0)
                              << true;
    QTest::newRow("float: max valid")
            << QVariant::fromValue(CastNumeric<float>(MaxNumeric<uint8_t>()))
            << MaxNumeric<uint8_t>()
            << true;
    QTest::newRow("float: max invalid 1")
            << QVariant::fromValue(CastNumeric<float>(MaxNumeric<uint8_t>() + 1.0f))
            << CastNumeric<uint8_t>(0)
            << false;
    QTest::newRow("float: max invalid 2") << QVariant::fromValue(MaxNumeric<float>())
                                          << CastNumeric<uint8_t>(0)
                                          << false;

    // double
    QTest::newRow("double: min invalid 1") << QVariant::fromValue(MinNumeric<double>())
                                           << CastNumeric<uint8_t>(0)
                                           << false;
    QTest::newRow("double: min invalid 2") << QVariant::fromValue(CastNumeric<double>(-1))
                                           << CastNumeric<uint8_t>(0)
                                           << false;
    QTest::newRow("double: min valid")
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<uint8_t>()))
            << MinNumeric<uint8_t>()
            << true;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                              << CastNumeric<uint8_t>(0)
                              << true;
    QTest::newRow("double: max valid")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<uint8_t>()))
            << MaxNumeric<uint8_t>()
            << true;
    QTest::newRow("double: max invalid 1")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<uint8_t>() + 1))
            << CastNumeric<uint8_t>(0)
            << false;
    QTest::newRow("double: max invalid 2") << QVariant::fromValue(MaxNumeric<double>())
                                           << CastNumeric<uint8_t>(0)
                                           << false;

    // byte array
    QTest::newRow("byte array: min invalid") << QVariant::fromValue(QByteArray::number(-1))
                                             << CastNumeric<uint8_t>(0)
                                             << false;
    QTest::newRow("byte array: min valid")
            << QVariant::fromValue(QByteArray::number(MinNumeric<uint8_t>()))
            << MinNumeric<uint8_t>()
            << true;
    QTest::newRow("byte array: 0") << QVariant::fromValue(QByteArray::number(0))
                                   << CastNumeric<uint8_t>(0)
                                   << true;
    QTest::newRow("byte array: max valid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<uint8_t>()))
            << MaxNumeric<uint8_t>()
            << true;
    QTest::newRow("byte array: max invalid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<uint8_t>() + 1))
            << CastNumeric<uint8_t>(0)
            << false;

    // string
    QTest::newRow("string: min invalid") << QVariant::fromValue(QString::number(-1))
                                         << CastNumeric<uint8_t>(0)
                                         << false;
    QTest::newRow("string: min valid")
            << QVariant::fromValue(QString::number(MinNumeric<uint8_t>()))
            << MinNumeric<uint8_t>()
            << true;
    QTest::newRow("string: 0") << QVariant::fromValue(QString::number(0))
                               << CastNumeric<uint8_t>(0)
                               << true;
    QTest::newRow("string: max valid")
            << QVariant::fromValue(QString::number(MaxNumeric<uint8_t>()))
            << MaxNumeric<uint8_t>()
            << true;
    QTest::newRow("string: max invalid")
            << QVariant::fromValue(QString::number(MaxNumeric<uint8_t>() + 1))
            << CastNumeric<uint8_t>(0)
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QChar('0'))
                                   << CastNumeric<uint8_t>(0)
                                   << false;
}

// Test: load int16 value --------------------------------------------------------------------------

void TestConfigParameterLoader::testInt16()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(int16_t, expectedParameterValue);
    QFETCH(bool, expectedResult);

    int16_t parameterValue = 0;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testInt16: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testInt16_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<int16_t>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // int8
    QTest::newRow("int8: min") << QVariant::fromValue(MinNumeric<int8_t>())
                               << CastNumeric<int16_t>(MinNumeric<int8_t>())
                               << true;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << CastNumeric<int16_t>(0)
                             << true;
    QTest::newRow("int8: max") << QVariant::fromValue(MaxNumeric<int8_t>())
                               << CastNumeric<int16_t>(MaxNumeric<int8_t>())
                               << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << CastNumeric<int16_t>(0)
                              << true;
    QTest::newRow("uint8: max") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                << CastNumeric<int16_t>(MaxNumeric<uint8_t>())
                                << true;

    // int16
    QTest::newRow("int16: min") << QVariant::fromValue(MinNumeric<int16_t>())
                                << MinNumeric<int16_t>()
                                << true;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << CastNumeric<int16_t>(0)
                              << true;
    QTest::newRow("int16: max") << QVariant::fromValue(MaxNumeric<int16_t>())
                                << MaxNumeric<int16_t>()
                                << true;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << CastNumeric<int16_t>(0)
                               << true;
    QTest::newRow("uint16: max valid")
            << QVariant::fromValue(CastNumeric<uint16_t>(MaxNumeric<int16_t>()))
            << MaxNumeric<int16_t>()
            << true;
    QTest::newRow("uint16: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint16_t>(MaxNumeric<int16_t>() + 1))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("uint16: max invalid 2")
            << QVariant::fromValue(MaxNumeric<uint16_t>())
            << CastNumeric<int16_t>(0)
            << false;

    // int32
    QTest::newRow("int32: min invalid 1") << QVariant::fromValue(MinNumeric<int32_t>())
                                          << CastNumeric<int16_t>(0)
                                          << false;
    QTest::newRow("int32: min invalid 2")
            << QVariant::fromValue(CastNumeric<int32_t>(MinNumeric<int16_t>() - 1))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("int32: min valid")
            << QVariant::fromValue(CastNumeric<int32_t>(MinNumeric<int16_t>()))
            << MinNumeric<int16_t>()
            << true;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << CastNumeric<int16_t>(0)
                              << true;
    QTest::newRow("int32: max valid")
            << QVariant::fromValue(CastNumeric<int32_t>(MaxNumeric<int16_t>()))
            << MaxNumeric<int16_t>()
            << true;
    QTest::newRow("int32: max invalid 1")
            << QVariant::fromValue(CastNumeric<int32_t>(MaxNumeric<int16_t>() + 1))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("int32: max invalid 2") << QVariant::fromValue(MaxNumeric<int32_t>())
                                          << CastNumeric<int16_t>(0)
                                          << false;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << CastNumeric<int16_t>(0)
                               << true;
    QTest::newRow("uint32: max valid")
            << QVariant::fromValue(CastNumeric<uint32_t>(MaxNumeric<int16_t>()))
            << MaxNumeric<int16_t>()
            << true;
    QTest::newRow("uint32: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint32_t>(MaxNumeric<int16_t>() + 1))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("uint32: max invalid 2") << QVariant::fromValue(MaxNumeric<uint32_t>())
                                           << CastNumeric<int16_t>(0)
                                           << false;

    // int64
    QTest::newRow("int64: min invalid 1") << QVariant::fromValue(MinNumeric<int64_t>())
                                          << CastNumeric<int16_t>(0)
                                          << false;
    QTest::newRow("int64: min invalid 2")
            << QVariant::fromValue(CastNumeric<int64_t>(MinNumeric<int16_t>() - 1LL))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("int64: min valid")
            << QVariant::fromValue(CastNumeric<int64_t>(MinNumeric<int16_t>()))
            << MinNumeric<int16_t>()
            << true;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << CastNumeric<int16_t>(0)
                              << true;
    QTest::newRow("int64: max valid")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<int16_t>()))
            << MaxNumeric<int16_t>()
            << true;
    QTest::newRow("int64: max invalid 1")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<int16_t>() + 1LL))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("int64: max invalid 2") << QVariant::fromValue(MaxNumeric<int64_t>())
                                          << CastNumeric<int16_t>(0)
                                          << false;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << CastNumeric<int16_t>(0)
                               << true;
    QTest::newRow("uint64: max valid")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<int16_t>()))
            << MaxNumeric<int16_t>()
            << true;
    QTest::newRow("uint64: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<int16_t>() + 1ULL))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("uint64: max invalid 2") << QVariant::fromValue(MaxNumeric<uint64_t>())
                                           << CastNumeric<int16_t>(0)
                                           << false;

    // float
    QTest::newRow("float: min invalid 1") << QVariant::fromValue(MinNumeric<float>())
                                          << CastNumeric<int16_t>(0)
                                          << false;
    QTest::newRow("float: min invalid 2")
            << QVariant::fromValue(CastNumeric<float>(MinNumeric<int16_t>() - 1.0f))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("float: min valid")
            << QVariant::fromValue(CastNumeric<float>(MinNumeric<int16_t>()))
            << MinNumeric<int16_t>()
            << true;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << CastNumeric<int16_t>(0)
                              << true;
    QTest::newRow("float: max valid")
            << QVariant::fromValue(CastNumeric<float>(MaxNumeric<int16_t>()))
            << MaxNumeric<int16_t>()
            << true;
    QTest::newRow("float: max invalid 1")
            << QVariant::fromValue(CastNumeric<float>(MaxNumeric<int16_t>() + 1.0f))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("float: max invalid 2") << QVariant::fromValue(MaxNumeric<float>())
                                          << CastNumeric<int16_t>(0)
                                          << false;

    // double
    QTest::newRow("double: min invalid 1") << QVariant::fromValue(MinNumeric<double>())
                                           << CastNumeric<int16_t>(0)
                                           << false;
    QTest::newRow("double: min invalid 2")
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<int16_t>() - 1))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("double: min valid")
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<int16_t>()))
            << MinNumeric<int16_t>()
            << true;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                              << CastNumeric<int16_t>(0)
                              << true;
    QTest::newRow("double: max valid")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<int16_t>()))
            << MaxNumeric<int16_t>()
            << true;
    QTest::newRow("double: max invalid 1")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<int16_t>() + 1))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("double: max invalid 2") << QVariant::fromValue(MaxNumeric<double>())
                                           << CastNumeric<int16_t>(0)
                                           << false;

    // byte array
    QTest::newRow("byte array: min invalid")
            << QVariant::fromValue(QByteArray::number(MinNumeric<int16_t>() - 1))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("byte array: min valid")
            << QVariant::fromValue(QByteArray::number(MinNumeric<int16_t>()))
            << MinNumeric<int16_t>()
            << true;
    QTest::newRow("byte array: 0") << QVariant::fromValue(QByteArray::number(0))
                                   << CastNumeric<int16_t>(0)
                                   << true;
    QTest::newRow("byte array: max valid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<int16_t>()))
            << MaxNumeric<int16_t>()
            << true;
    QTest::newRow("byte array: max invalid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<int16_t>() + 1))
            << CastNumeric<int16_t>(0)
            << false;

    // string
    QTest::newRow("string: min invalid")
            << QVariant::fromValue(QString::number(MinNumeric<int16_t>() - 1))
            << CastNumeric<int16_t>(0)
            << false;
    QTest::newRow("string: min valid")
            << QVariant::fromValue(QString::number(MinNumeric<int16_t>()))
            << MinNumeric<int16_t>()
            << true;
    QTest::newRow("string: 0") << QVariant::fromValue(QString::number(0))
                               << CastNumeric<int16_t>(0)
                               << true;
    QTest::newRow("string: max valid")
            << QVariant::fromValue(QString::number(MaxNumeric<int16_t>()))
            << MaxNumeric<int16_t>()
            << true;
    QTest::newRow("string: max invalid")
            << QVariant::fromValue(QString::number(MaxNumeric<int16_t>() + 1))
            << CastNumeric<int16_t>(0)
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QChar('0'))
                                   << CastNumeric<int16_t>(0)
                                   << false;
}

// Test: load uint16 value --------------------------------------------------------------------------

void TestConfigParameterLoader::testUInt16()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(uint16_t, expectedParameterValue);
    QFETCH(bool, expectedResult);

    uint16_t parameterValue = 0U;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testUInt16: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testUInt16_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<uint16_t>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // int8
    QTest::newRow("int8: min invalid") << QVariant::fromValue(MinNumeric<int8_t>())
                                       << CastNumeric<uint16_t>(0)
                                       << false;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << CastNumeric<uint16_t>(0)
                             << true;
    QTest::newRow("int8: max valid") << QVariant::fromValue(MaxNumeric<int8_t>())
                                     << CastNumeric<uint16_t>(MaxNumeric<int8_t>())
                                     << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << CastNumeric<uint16_t>(0)
                              << true;
    QTest::newRow("uint8: max") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                << CastNumeric<uint16_t>(MaxNumeric<uint8_t>())
                                << true;

    // int16
    QTest::newRow("int16: min invalid 1") << QVariant::fromValue(MinNumeric<int16_t>())
                                          << CastNumeric<uint16_t>(0)
                                          << false;
    QTest::newRow("int16: min invalid 2") << QVariant::fromValue(CastNumeric<int16_t>(-1))
                                          << CastNumeric<uint16_t>(0)
                                          << false;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << CastNumeric<uint16_t>(0)
                              << true;
    QTest::newRow("int16: max")
            << QVariant::fromValue(MaxNumeric<int16_t>())
            << CastNumeric<uint16_t>(MaxNumeric<int16_t>())
            << true;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << CastNumeric<uint16_t>(0)
                               << true;
    QTest::newRow("uint16: max")
            << QVariant::fromValue(MaxNumeric<uint16_t>())
            << MaxNumeric<uint16_t>()
            << true;

    // int32
    QTest::newRow("int32: min invalid 1") << QVariant::fromValue(MinNumeric<int32_t>())
                                          << CastNumeric<uint16_t>(0)
                                          << false;
    QTest::newRow("int32: min invalid 2") << QVariant::fromValue(CastNumeric<int32_t>(-1))
                                          << CastNumeric<uint16_t>(0)
                                          << false;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << CastNumeric<uint16_t>(0)
                              << true;
    QTest::newRow("int32: max valid")
            << QVariant::fromValue(CastNumeric<int32_t>(MaxNumeric<uint16_t>()))
            << MaxNumeric<uint16_t>()
            << true;
    QTest::newRow("int32: max invalid 1")
            << QVariant::fromValue(CastNumeric<int32_t>(MaxNumeric<uint16_t>() + 1))
            << CastNumeric<uint16_t>(0)
            << false;
    QTest::newRow("int32: max invalid 2") << QVariant::fromValue(MaxNumeric<int32_t>())
                                          << CastNumeric<uint16_t>(0)
                                          << false;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << CastNumeric<uint16_t>(0)
                               << true;
    QTest::newRow("uint32: max valid")
            << QVariant::fromValue(CastNumeric<uint32_t>(MaxNumeric<uint16_t>()))
            << MaxNumeric<uint16_t>()
            << true;
    QTest::newRow("uint32: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint32_t>(MaxNumeric<uint16_t>() + 1))
            << CastNumeric<uint16_t>(0)
            << false;
    QTest::newRow("uint32: max invalid 2") << QVariant::fromValue(MaxNumeric<uint32_t>())
                                           << CastNumeric<uint16_t>(0)
                                           << false;

    // int64
    QTest::newRow("int64: min invalid 1") << QVariant::fromValue(MinNumeric<int64_t>())
                                          << CastNumeric<uint16_t>(0)
                                          << false;
    QTest::newRow("int64: min invalid 2") << QVariant::fromValue(CastNumeric<int64_t>(-1LL))
                                          << CastNumeric<uint16_t>(0)
                                          << false;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << CastNumeric<uint16_t>(0)
                              << true;
    QTest::newRow("int64: max valid")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<uint16_t>()))
            << MaxNumeric<uint16_t>()
            << true;
    QTest::newRow("int64: max invalid 1")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<uint16_t>() + 1LL))
            << CastNumeric<uint16_t>(0)
            << false;
    QTest::newRow("int64: max invalid 2") << QVariant::fromValue(MaxNumeric<int64_t>())
                                          << CastNumeric<uint16_t>(0)
                                          << false;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << CastNumeric<uint16_t>(0)
                               << true;
    QTest::newRow("uint64: max valid")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<uint16_t>()))
            << MaxNumeric<uint16_t>()
            << true;
    QTest::newRow("uint64: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<uint16_t>() + 1ULL))
            << CastNumeric<uint16_t>(0)
            << false;
    QTest::newRow("uint64: max invalid 2") << QVariant::fromValue(MaxNumeric<uint64_t>())
                                           << CastNumeric<uint16_t>(0)
                                           << false;

    // float
    QTest::newRow("float: min invalid 1") << QVariant::fromValue(MinNumeric<float>())
                                          << CastNumeric<uint16_t>(0)
                                          << false;
    QTest::newRow("float: min invalid 2") << QVariant::fromValue(CastNumeric<float>(-1))
                                          << CastNumeric<uint16_t>(0)
                                          << false;
    QTest::newRow("float: min valid")
            << QVariant::fromValue(CastNumeric<float>(MinNumeric<uint16_t>()))
            << MinNumeric<uint16_t>()
            << true;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << CastNumeric<uint16_t>(0)
                              << true;
    QTest::newRow("float: max valid")
            << QVariant::fromValue(CastNumeric<float>(MaxNumeric<uint16_t>()))
            << MaxNumeric<uint16_t>()
            << true;
    QTest::newRow("float: max invalid 1")
            << QVariant::fromValue(CastNumeric<float>(MaxNumeric<uint16_t>() + 1.0f))
            << CastNumeric<uint16_t>(0)
            << false;
    QTest::newRow("float: max invalid 2") << QVariant::fromValue(MaxNumeric<float>())
                                          << CastNumeric<uint16_t>(0)
                                          << false;

    // double
    QTest::newRow("double: min invalid 1") << QVariant::fromValue(MinNumeric<double>())
                                           << CastNumeric<uint16_t>(0)
                                           << false;
    QTest::newRow("double: min invalid 2") << QVariant::fromValue(CastNumeric<double>(-1))
                                           << CastNumeric<uint16_t>(0)
                                           << false;
    QTest::newRow("double: min valid")
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<uint16_t>()))
            << MinNumeric<uint16_t>()
            << true;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                              << CastNumeric<uint16_t>(0)
                              << true;
    QTest::newRow("double: max valid")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<uint16_t>()))
            << MaxNumeric<uint16_t>()
            << true;
    QTest::newRow("double: max invalid 1")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<uint16_t>() + 1))
            << CastNumeric<uint16_t>(0)
            << false;
    QTest::newRow("double: max invalid 2") << QVariant::fromValue(MaxNumeric<double>())
                                           << CastNumeric<uint16_t>(0)
                                           << false;

    // byte array
    QTest::newRow("byte array: min invalid") << QVariant::fromValue(QByteArray::number(-1))
                                             << CastNumeric<uint16_t>(0)
                                             << false;
    QTest::newRow("byte array: min valid")
            << QVariant::fromValue(QByteArray::number(MinNumeric<uint16_t>()))
            << MinNumeric<uint16_t>()
            << true;
    QTest::newRow("byte array: 0") << QVariant::fromValue(QByteArray::number(0))
                                   << CastNumeric<uint16_t>(0)
                                   << true;
    QTest::newRow("byte array: max valid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<uint16_t>()))
            << MaxNumeric<uint16_t>()
            << true;
    QTest::newRow("byte array: max invalid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<uint16_t>() + 1))
            << CastNumeric<uint16_t>(0)
            << false;

    // string
    QTest::newRow("string: min invalid") << QVariant::fromValue(QString::number(-1))
                                         << CastNumeric<uint16_t>(0)
                                         << false;
    QTest::newRow("string: min valid")
            << QVariant::fromValue(QString::number(MinNumeric<uint16_t>()))
            << MinNumeric<uint16_t>()
            << true;
    QTest::newRow("string: 0") << QVariant::fromValue(QString::number(0))
                               << CastNumeric<uint16_t>(0)
                               << true;
    QTest::newRow("string: max valid")
            << QVariant::fromValue(QString::number(MaxNumeric<uint16_t>()))
            << MaxNumeric<uint16_t>()
            << true;
    QTest::newRow("string: max invalid")
            << QVariant::fromValue(QString::number(MaxNumeric<uint16_t>() + 1))
            << CastNumeric<uint16_t>(0)
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QChar('0'))
                                   << CastNumeric<uint16_t>(0)
                                   << false;
}

// Test: load int32 value --------------------------------------------------------------------------

void TestConfigParameterLoader::testInt32()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(int32_t, expectedParameterValue);
    QFETCH(bool, expectedResult);

    int32_t parameterValue = 0;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testInt32: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testInt32_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<int32_t>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // int8
    QTest::newRow("int8: min") << QVariant::fromValue(MinNumeric<int8_t>())
                               << CastNumeric<int32_t>(MinNumeric<int8_t>())
                               << true;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << CastNumeric<int32_t>(0)
                             << true;
    QTest::newRow("int8: max") << QVariant::fromValue(MaxNumeric<int8_t>())
                               << CastNumeric<int32_t>(MaxNumeric<int8_t>())
                               << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << CastNumeric<int32_t>(0)
                              << true;
    QTest::newRow("uint8: max") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                << CastNumeric<int32_t>(MaxNumeric<uint8_t>())
                                << true;

    // int16
    QTest::newRow("int16: min") << QVariant::fromValue(MinNumeric<int16_t>())
                                << CastNumeric<int32_t>(MinNumeric<int16_t>())
                                << true;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << CastNumeric<int32_t>(0)
                              << true;
    QTest::newRow("int16: max") << QVariant::fromValue(MaxNumeric<int16_t>())
                                << CastNumeric<int32_t>(MaxNumeric<int16_t>())
                                << true;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << CastNumeric<int32_t>(0)
                               << true;
    QTest::newRow("uint16: max") << QVariant::fromValue(MaxNumeric<uint16_t>())
                                 << CastNumeric<int32_t>(MaxNumeric<uint16_t>())
                                 << true;

    // int32
    QTest::newRow("int32: min") << QVariant::fromValue(MinNumeric<int32_t>())
                                << MinNumeric<int32_t>()
                                << true;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << CastNumeric<int32_t>(0)
                              << true;
    QTest::newRow("int32: max") << QVariant::fromValue(MaxNumeric<int32_t>())
                                << MaxNumeric<int32_t>()
                                << true;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << CastNumeric<int32_t>(0)
                               << true;
    QTest::newRow("uint32: max valid")
            << QVariant::fromValue(CastNumeric<uint32_t>(MaxNumeric<int32_t>()))
            << MaxNumeric<int32_t>()
            << true;
    QTest::newRow("uint32: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint32_t>(MaxNumeric<int32_t>() + 1))
            << CastNumeric<int32_t>(0)
            << false;
    QTest::newRow("uint32: max invalid 2") << QVariant::fromValue(MaxNumeric<uint32_t>())
                                           << CastNumeric<int32_t>(0)
                                           << false;

    // int64
    QTest::newRow("int64: min invalid 1") << QVariant::fromValue(MinNumeric<int64_t>())
                                          << CastNumeric<int32_t>(0)
                                          << false;
    QTest::newRow("int64: min invalid 2")
            << QVariant::fromValue(CastNumeric<int64_t>(MinNumeric<int32_t>() - 1LL))
            << CastNumeric<int32_t>(0)
            << false;
    QTest::newRow("int64: min valid")
            << QVariant::fromValue(CastNumeric<int64_t>(MinNumeric<int32_t>()))
            << MinNumeric<int32_t>()
            << true;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << CastNumeric<int32_t>(0)
                              << true;
    QTest::newRow("int64: max valid")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<int32_t>()))
            << MaxNumeric<int32_t>()
            << true;
    QTest::newRow("int64: max invalid 1")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<int32_t>() + 1LL))
            << CastNumeric<int32_t>(0)
            << false;
    QTest::newRow("int64: max invalid 2") << QVariant::fromValue(MaxNumeric<int64_t>())
                                          << CastNumeric<int32_t>(0)
                                          << false;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << CastNumeric<int32_t>(0)
                               << true;
    QTest::newRow("uint64: max valid")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<int32_t>()))
            << MaxNumeric<int32_t>()
            << true;
    QTest::newRow("uint64: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<int32_t>() + 1ULL))
            << CastNumeric<int32_t>(0)
            << false;
    QTest::newRow("uint64: max invalid 2") << QVariant::fromValue(MaxNumeric<uint64_t>())
                                           << CastNumeric<int32_t>(0)
                                           << false;

    // float
    QTest::newRow("float: min invalid 1") << QVariant::fromValue(MinNumeric<float>())
                                          << CastNumeric<int32_t>(0)
                                          << false;
    QTest::newRow("float: min invalid 2")
            // Workaround for the next smallest min int32 value that can be exactly represented as a
            // float value (min value is the same as min int32 value)
            << QVariant::fromValue(CastNumeric<float>(-2147483904.0f))
            << CastNumeric<int32_t>(0)
            << false;
    QTest::newRow("float: min valid")
            << QVariant::fromValue(CastNumeric<float>(MinNumeric<int32_t>()))
            << MinNumeric<int32_t>()
            << true;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << CastNumeric<int32_t>(0)
                              << true;
    QTest::newRow("float: max valid")
            // Workaround for max int32 value that can be exactly represented as a float value (next
            // one is max int32 + 1)
            << QVariant::fromValue(CastNumeric<float>(2147483520))
            << CastNumeric<int32_t>(2147483520)
            << true;
    QTest::newRow("float: max invalid 1")
            << QVariant::fromValue(CastNumeric<float>(MaxNumeric<int32_t>() + 1.0f))
            << CastNumeric<int32_t>(0)
            << false;
    QTest::newRow("float: max invalid 2") << QVariant::fromValue(MaxNumeric<float>())
                                          << CastNumeric<int32_t>(0)
                                          << false;

    // double
    QTest::newRow("double: min invalid 1") << QVariant::fromValue(MinNumeric<double>())
                                           << CastNumeric<int32_t>(0)
                                           << false;
    QTest::newRow("double: min invalid 2")
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<int32_t>() - 1LL))
            << CastNumeric<int32_t>(0)
            << false;
    QTest::newRow("double: min valid")
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<int32_t>()))
            << MinNumeric<int32_t>()
            << true;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                              << CastNumeric<int32_t>(0)
                              << true;
    QTest::newRow("double: max valid")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<int32_t>()))
            << MaxNumeric<int32_t>()
            << true;
    QTest::newRow("double: max invalid 1")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<int32_t>() + 1LL))
            << CastNumeric<int32_t>(0)
            << false;
    QTest::newRow("double: max invalid 2") << QVariant::fromValue(MaxNumeric<double>())
                                           << CastNumeric<int32_t>(0)
                                           << false;

    // byte array
    QTest::newRow("byte array: min invalid")
            << QVariant::fromValue(QByteArray::number(MinNumeric<int32_t>() - 1LL))
            << CastNumeric<int32_t>(0)
            << false;
    QTest::newRow("byte array: min valid")
            << QVariant::fromValue(QByteArray::number(MinNumeric<int32_t>()))
            << MinNumeric<int32_t>()
            << true;
    QTest::newRow("byte array: 0") << QVariant::fromValue(QByteArray::number(0))
                                   << CastNumeric<int32_t>(0)
                                   << true;
    QTest::newRow("byte array: max valid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<int32_t>()))
            << MaxNumeric<int32_t>()
            << true;
    QTest::newRow("byte array: max invalid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<int32_t>() + 1LL))
            << CastNumeric<int32_t>(0)
            << false;

    // string
    QTest::newRow("string: min invalid")
            << QVariant::fromValue(QString::number(MinNumeric<int32_t>() - 1LL))
            << CastNumeric<int32_t>(0)
            << false;
    QTest::newRow("string: min valid")
            << QVariant::fromValue(QString::number(MinNumeric<int32_t>()))
            << MinNumeric<int32_t>()
            << true;
    QTest::newRow("string: 0") << QVariant::fromValue(QString::number(0))
                               << CastNumeric<int32_t>(0)
                               << true;
    QTest::newRow("string: max valid")
            << QVariant::fromValue(QString::number(MaxNumeric<int32_t>()))
            << MaxNumeric<int32_t>()
            << true;
    QTest::newRow("string: max invalid")
            << QVariant::fromValue(QString::number(MaxNumeric<int32_t>() + 1LL))
            << CastNumeric<int32_t>(0)
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QChar('0'))
                                   << CastNumeric<int32_t>(0)
                                   << false;
}

// Test: load uint32 value --------------------------------------------------------------------------

void TestConfigParameterLoader::testUInt32()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(uint32_t, expectedParameterValue);
    QFETCH(bool, expectedResult);

    uint32_t parameterValue = 0U;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testUInt32: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testUInt32_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<uint32_t>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // int8
    QTest::newRow("int8: min invalid") << QVariant::fromValue(MinNumeric<int8_t>())
                                       << CastNumeric<uint32_t>(0)
                                       << false;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << CastNumeric<uint32_t>(0)
                             << true;
    QTest::newRow("int8: max valid") << QVariant::fromValue(MaxNumeric<int8_t>())
                                     << CastNumeric<uint32_t>(MaxNumeric<int8_t>())
                                     << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << CastNumeric<uint32_t>(0)
                              << true;
    QTest::newRow("uint8: max") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                << CastNumeric<uint32_t>(MaxNumeric<uint8_t>())
                                << true;

    // int16
    QTest::newRow("int16: min invalid 1") << QVariant::fromValue(MinNumeric<int16_t>())
                                          << CastNumeric<uint32_t>(0)
                                          << false;
    QTest::newRow("int16: min invalid 2") << QVariant::fromValue(CastNumeric<int16_t>(-1))
                                          << CastNumeric<uint32_t>(0)
                                          << false;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << CastNumeric<uint32_t>(0)
                              << true;
    QTest::newRow("int16: max")
            << QVariant::fromValue(MaxNumeric<int16_t>())
            << CastNumeric<uint32_t>(MaxNumeric<int16_t>())
            << true;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << CastNumeric<uint32_t>(0)
                               << true;
    QTest::newRow("uint16: max")
            << QVariant::fromValue(MaxNumeric<uint16_t>())
            << CastNumeric<uint32_t>(MaxNumeric<uint16_t>())
            << true;

    // int32
    QTest::newRow("int32: min invalid 1") << QVariant::fromValue(MinNumeric<int32_t>())
                                          << CastNumeric<uint32_t>(0)
                                          << false;
    QTest::newRow("int32: min invalid 2") << QVariant::fromValue(CastNumeric<int32_t>(-1))
                                          << CastNumeric<uint32_t>(0)
                                          << false;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << CastNumeric<uint32_t>(0)
                              << true;
    QTest::newRow("int32: max")
            << QVariant::fromValue(MaxNumeric<int32_t>())
            << CastNumeric<uint32_t>(MaxNumeric<int32_t>())
            << true;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << CastNumeric<uint32_t>(0)
                               << true;
    QTest::newRow("uint32: max")
            << QVariant::fromValue(MaxNumeric<uint32_t>())
            << MaxNumeric<uint32_t>()
            << true;

    // int64
    QTest::newRow("int64: min invalid 1") << QVariant::fromValue(MinNumeric<int64_t>())
                                          << CastNumeric<uint32_t>(0)
                                          << false;
    QTest::newRow("int64: min invalid 2") << QVariant::fromValue(CastNumeric<int64_t>(-1LL))
                                          << CastNumeric<uint32_t>(0)
                                          << false;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << CastNumeric<uint32_t>(0)
                              << true;
    QTest::newRow("int64: max valid")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<uint32_t>()))
            << MaxNumeric<uint32_t>()
            << true;
    QTest::newRow("int64: max invalid 1")
            << QVariant::fromValue(CastNumeric<int64_t>(MaxNumeric<uint32_t>() + 1LL))
            << CastNumeric<uint32_t>(0)
            << false;
    QTest::newRow("int64: max invalid 2") << QVariant::fromValue(MaxNumeric<int64_t>())
                                          << CastNumeric<uint32_t>(0)
                                          << false;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << CastNumeric<uint32_t>(0)
                               << true;
    QTest::newRow("uint64: max valid")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<uint32_t>()))
            << MaxNumeric<uint32_t>()
            << true;
    QTest::newRow("uint64: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<uint32_t>() + 1ULL))
            << CastNumeric<uint32_t>(0)
            << false;
    QTest::newRow("uint64: max invalid 2") << QVariant::fromValue(MaxNumeric<uint64_t>())
                                           << CastNumeric<uint32_t>(0)
                                           << false;

    // float
    QTest::newRow("float: min invalid 1") << QVariant::fromValue(MinNumeric<float>())
                                          << CastNumeric<uint32_t>(0)
                                          << false;
    QTest::newRow("float: min invalid 2") << QVariant::fromValue(CastNumeric<float>(-1))
                                          << CastNumeric<uint32_t>(0)
                                          << false;
    QTest::newRow("float: min valid")
            << QVariant::fromValue(CastNumeric<float>(MinNumeric<uint32_t>()))
            << MinNumeric<uint32_t>()
            << true;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << CastNumeric<uint32_t>(0)
                              << true;
    QTest::newRow("float: max valid")
            // Workaround for max uint32 value that can be exactly represented as a float value
            // (next one is max uint32 + 1)
            << QVariant::fromValue(CastNumeric<float>(4294967040U))
            << CastNumeric<uint32_t>(4294967040U)
            << true;
    QTest::newRow("float: max invalid 1")
            << QVariant::fromValue(CastNumeric<float>(MaxNumeric<uint32_t>() + 1.0f))
            << CastNumeric<uint32_t>(0)
            << false;
    QTest::newRow("float: max invalid 2") << QVariant::fromValue(MaxNumeric<float>())
                                          << CastNumeric<uint32_t>(0)
                                          << false;

    // double
    QTest::newRow("double: min invalid 1") << QVariant::fromValue(MinNumeric<double>())
                                           << CastNumeric<uint32_t>(0)
                                           << false;
    QTest::newRow("double: min invalid 2") << QVariant::fromValue(CastNumeric<double>(-1))
                                           << CastNumeric<uint32_t>(0)
                                           << false;
    QTest::newRow("double: min valid")
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<uint32_t>()))
            << MinNumeric<uint32_t>()
            << true;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                              << CastNumeric<uint32_t>(0)
                              << true;
    QTest::newRow("double: max valid")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<uint32_t>()))
            << MaxNumeric<uint32_t>()
            << true;
    QTest::newRow("double: max invalid 1")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<uint32_t>() + 1ULL))
            << CastNumeric<uint32_t>(0)
            << false;
    QTest::newRow("double: max invalid 2") << QVariant::fromValue(MaxNumeric<double>())
                                           << CastNumeric<uint32_t>(0)
                                           << false;

    // byte array
    QTest::newRow("byte array: min invalid") << QVariant::fromValue(QByteArray::number(-1))
                                             << CastNumeric<uint32_t>(0)
                                             << false;
    QTest::newRow("byte array: min valid")
            << QVariant::fromValue(QByteArray::number(MinNumeric<uint32_t>()))
            << MinNumeric<uint32_t>()
            << true;
    QTest::newRow("byte array: 0") << QVariant::fromValue(QByteArray::number(0))
                                   << CastNumeric<uint32_t>(0)
                                   << true;
    QTest::newRow("byte array: max valid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<uint32_t>()))
            << MaxNumeric<uint32_t>()
            << true;
    QTest::newRow("byte array: max invalid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<uint32_t>() + 1ULL))
            << CastNumeric<uint32_t>(0)
            << false;

    // string
    QTest::newRow("string: min invalid") << QVariant::fromValue(QString::number(-1))
                                         << CastNumeric<uint32_t>(0)
                                         << false;
    QTest::newRow("string: min valid")
            << QVariant::fromValue(QString::number(MinNumeric<uint32_t>()))
            << MinNumeric<uint32_t>()
            << true;
    QTest::newRow("string: 0") << QVariant::fromValue(QString::number(0))
                               << CastNumeric<uint32_t>(0)
                               << true;
    QTest::newRow("string: max valid")
            << QVariant::fromValue(QString::number(MaxNumeric<uint32_t>()))
            << MaxNumeric<uint32_t>()
            << true;
    QTest::newRow("string: max invalid")
            << QVariant::fromValue(QString::number(MaxNumeric<uint32_t>() + 1ULL))
            << CastNumeric<uint32_t>(0)
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QChar('0'))
                                   << CastNumeric<uint32_t>(0)
                                   << false;
}

// Test: load int64 value --------------------------------------------------------------------------

void TestConfigParameterLoader::testInt64()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(int64_t, expectedParameterValue);
    QFETCH(bool, expectedResult);

    int64_t parameterValue = 0;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testInt64: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testInt64_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<int64_t>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // int8
    QTest::newRow("int8: min") << QVariant::fromValue(MinNumeric<int8_t>())
                               << CastNumeric<int64_t>(MinNumeric<int8_t>())
                               << true;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << CastNumeric<int64_t>(0)
                             << true;
    QTest::newRow("int8: max") << QVariant::fromValue(MaxNumeric<int8_t>())
                               << CastNumeric<int64_t>(MaxNumeric<int8_t>())
                               << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << CastNumeric<int64_t>(0)
                              << true;
    QTest::newRow("uint8: max") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                << CastNumeric<int64_t>(MaxNumeric<uint8_t>())
                                << true;

    // int16
    QTest::newRow("int16: min") << QVariant::fromValue(MinNumeric<int16_t>())
                                << CastNumeric<int64_t>(MinNumeric<int16_t>())
                                << true;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << CastNumeric<int64_t>(0)
                              << true;
    QTest::newRow("int16: max") << QVariant::fromValue(MaxNumeric<int16_t>())
                                << CastNumeric<int64_t>(MaxNumeric<int16_t>())
                                << true;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << CastNumeric<int64_t>(0)
                               << true;
    QTest::newRow("uint16: max") << QVariant::fromValue(MaxNumeric<uint16_t>())
                                 << CastNumeric<int64_t>(MaxNumeric<uint16_t>())
                                 << true;

    // int32
    QTest::newRow("int32: min") << QVariant::fromValue(MinNumeric<int32_t>())
                                << CastNumeric<int64_t>(MinNumeric<int32_t>())
                                << true;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << CastNumeric<int64_t>(0)
                              << true;
    QTest::newRow("int32: max") << QVariant::fromValue(MaxNumeric<int32_t>())
                                << CastNumeric<int64_t>(MaxNumeric<int32_t>())
                                << true;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << CastNumeric<int64_t>(0)
                               << true;
    QTest::newRow("uint32: max") << QVariant::fromValue(MaxNumeric<uint32_t>())
                                 << CastNumeric<int64_t>(MaxNumeric<uint32_t>())
                                 << true;

    // int64
    QTest::newRow("int64: min") << QVariant::fromValue(MinNumeric<int64_t>())
                                << MinNumeric<int64_t>()
                                << true;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << CastNumeric<int64_t>(0)
                              << true;
    QTest::newRow("int64: max") << QVariant::fromValue(MaxNumeric<int64_t>())
                                << MaxNumeric<int64_t>()
                                << true;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << CastNumeric<int64_t>(0)
                               << true;
    QTest::newRow("uint64: max valid")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<int64_t>()))
            << MaxNumeric<int64_t>()
            << true;
    QTest::newRow("uint64: max invalid 1")
            << QVariant::fromValue(CastNumeric<uint64_t>(MaxNumeric<int64_t>() + 1ULL))
            << CastNumeric<int64_t>(0)
            << false;
    QTest::newRow("uint64: max invalid 2") << QVariant::fromValue(MaxNumeric<uint64_t>())
                                           << CastNumeric<int64_t>(0)
                                           << false;

    // float
    QTest::newRow("float: min invalid 1") << QVariant::fromValue(MinNumeric<float>())
                                          << CastNumeric<int64_t>(0)
                                          << false;
    QTest::newRow("float: min invalid 2")
            // Workaround for the next smallest min int64 value that can be exactly represented as a
            // float value (min value is the same as min int64 value)
            << QVariant::fromValue(CastNumeric<float>(-9.223373136366404e+18f))
            << CastNumeric<int64_t>(0)
            << false;
    QTest::newRow("float: min valid")
            << QVariant::fromValue(CastNumeric<float>(MinNumeric<int64_t>()))
            << MinNumeric<int64_t>()
            << true;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << CastNumeric<int64_t>(0)
                              << true;
    QTest::newRow("float: max valid")
            // Workaround for max int64 value that can be exactly represented as a float value (next
            // one is max int64 + 1)
            << QVariant::fromValue(CastNumeric<float>(2147483520))
            << CastNumeric<int64_t>(2147483520)
            << true;
    QTest::newRow("float: max invalid 1")
            // Workaround for the next smallest max int64 value that can be exactly represented as a
            // float value
            << QVariant::fromValue(CastNumeric<float>(9.223373136366404e+18))
            << CastNumeric<int64_t>(0)
            << false;
    QTest::newRow("float: max invalid 2") << QVariant::fromValue(MaxNumeric<float>())
                                          << CastNumeric<int64_t>(0)
                                          << false;

    // double
    QTest::newRow("double: min invalid 1") << QVariant::fromValue(MinNumeric<double>())
                                           << CastNumeric<int64_t>(0)
                                           << false;
    QTest::newRow("double: min invalid 2")
            // Workaround for the next smallest min int64 value that can be exactly represented as a
            // double value (min value is the same as min int64 value)
            << QVariant::fromValue(CastNumeric<double>(-9.223372036854778e+18))
            << CastNumeric<int64_t>(0)
            << false;
    QTest::newRow("double: min valid")
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<int64_t>()))
            << MinNumeric<int64_t>()
            << true;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                              << CastNumeric<int64_t>(0)
                              << true;
    QTest::newRow("double: max valid")
            // Workaround for max int64 value that can be exactly represented as a double value
            << QVariant::fromValue(CastNumeric<double>(9223372036854774784.0))
            << CastNumeric<int64_t>(9223372036854774784LL)
            << true;
    QTest::newRow("double: max invalid 1")
            // Workaround for the next smallest max int64 value that can be exactly represented as a
            // double value
            << QVariant::fromValue(CastNumeric<double>(9.223372036854778e+18))
            << CastNumeric<int64_t>(0)
            << false;
    QTest::newRow("double: max invalid 2") << QVariant::fromValue(MaxNumeric<double>())
                                           << CastNumeric<int64_t>(0)
                                           << false;

    // byte array
    QTest::newRow("byte array: min invalid")
            // Workaround for the next smallest min int64 value that can be exactly represented as a
            // double value (min value is the same as min int64 value)
            << QVariant::fromValue(QByteArray::number(-9.223372036854778e+18))
            << CastNumeric<int64_t>(0)
            << false;
    QTest::newRow("byte array: min valid")
            // Workaround: for some reason int64_t does not work here
            << QVariant::fromValue(QByteArray::number(MinNumeric<qlonglong>()))
            << MinNumeric<int64_t>()
            << true;
    QTest::newRow("byte array: 0") << QVariant::fromValue(QByteArray::number(0))
                                   << CastNumeric<int64_t>(0)
                                   << true;
    QTest::newRow("byte array: max valid")
            // Workaround: for some reason int64_t does not work here
            << QVariant::fromValue(QByteArray::number(MaxNumeric<qlonglong>()))
            << MaxNumeric<int64_t>()
            << true;
    QTest::newRow("byte array: max invalid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<int64_t>() + 1ULL))
            << CastNumeric<int64_t>(0)
            << false;

    // string
    QTest::newRow("string: min invalid")
            // Workaround for the next smallest min int64 value that can be exactly represented as a
            // double value (min value is the same as min int64 value)
            << QVariant::fromValue(QString::number(-9.223372036854778e+18))
            << CastNumeric<int64_t>(0)
            << false;
    QTest::newRow("string: min valid")
            << QVariant::fromValue(QString::number(MinNumeric<int64_t>()))
            << MinNumeric<int64_t>()
            << true;
    QTest::newRow("string: 0") << QVariant::fromValue(QString::number(0))
                               << CastNumeric<int64_t>(0)
                               << true;
    QTest::newRow("string: max valid")
            << QVariant::fromValue(QString::number(MaxNumeric<int64_t>()))
            << MaxNumeric<int64_t>()
            << true;
    QTest::newRow("string: max invalid")
            << QVariant::fromValue(QString::number(MaxNumeric<int64_t>() + 1ULL))
            << CastNumeric<int64_t>(0)
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QChar('0'))
                                   << CastNumeric<int64_t>(0)
                                   << false;
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigParameterLoader)
#include "testConfigParameterLoader.moc"
