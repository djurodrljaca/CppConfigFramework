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

    void testUInt64();
    void testUInt64_data();

    void testFloat();
    void testFloat_data();

    void testDouble();
    void testDouble_data();

    void testChar();
    void testChar_data();

    void testString();
    void testString_data();

    void testByteArray();
    void testByteArray_data();

    void testStdString();
    void testStdString_data();

    void testDate();
    void testDate_data();

    void testTime();
    void testTime_data();

    void testDateTime();
    void testDateTime_data();

    void testVariant();
    void testVariant_data();

    void testUrl();
    void testUrl_data();

    void testUuid();
    void testUuid_data();
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

    // Boolean values
    QTest::newRow("bool: false") << QVariant(false) << false << true;
    QTest::newRow("bool: true") << QVariant(true) << true << true;

    // Numeric values
    QTest::newRow("int: min") << QVariant(MinNumeric<int>()) << false << false;
    QTest::newRow("int: -1") << QVariant(-1) << false << false;
    QTest::newRow("int: 0") << QVariant(0) << false << true;
    QTest::newRow("int: 1") << QVariant(1) << true << true;
    QTest::newRow("int: 2") << QVariant(2) << false << false;
    QTest::newRow("int: max") << QVariant(MaxNumeric<int>()) << false << false;

    QTest::newRow("uint: 0") << QVariant(0u) << false << true;
    QTest::newRow("uint: 1") << QVariant(1u) << true << true;
    QTest::newRow("uint: 2") << QVariant(2u) << false << false;
    QTest::newRow("uint: max") << QVariant(MaxNumeric<unsigned int>()) << false << false;

    QTest::newRow("float: min") << QVariant(MinNumeric<float>()) << false << false;
    QTest::newRow("float: < 0.0") << QVariant(-0.01f) << false << false;
    QTest::newRow("float: 0.0") << QVariant(0.0f) << false << true;
    QTest::newRow("float: 1.0") << QVariant(1.0f) << true << true;
    QTest::newRow("float: > 1.0") << QVariant(1.01f) << false << false;
    QTest::newRow("float: max") << QVariant(MaxNumeric<float>()) << false << false;

    QTest::newRow("double: min") << QVariant(MinNumeric<double>()) << false << false;
    QTest::newRow("double: < 0.0") << QVariant(-0.01) << false << false;
    QTest::newRow("double: 0.0") << QVariant(0.0) << false << true;
    QTest::newRow("double: 1.0") << QVariant(1.0) << true << true;
    QTest::newRow("double: > 1.0") << QVariant(1.01) << false << false;
    QTest::newRow("double: max") << QVariant(MaxNumeric<double>()) << false << false;

    // String values
    QTest::newRow("byte array: false") << QVariant(QByteArray("false")) << false << true;
    QTest::newRow("byte array: true") << QVariant(QByteArray("true")) << true << true;
    QTest::newRow("byte array: invalid 1") << QVariant(QByteArray("False")) << false << false;
    QTest::newRow("byte array: invalid 2") << QVariant(QByteArray("True")) << false << false;
    QTest::newRow("byte array: invalid 3") << QVariant(QByteArray("asd")) << false << false;

    QTest::newRow("byte array: < 0.0") << QVariant(QByteArray("-0.01")) << false << false;
    QTest::newRow("byte array: 0.0") << QVariant(QByteArray("0.0")) << false << true;
    QTest::newRow("byte array: 1.0") << QVariant(QByteArray("1.0")) << true << true;
    QTest::newRow("byte array: > 1.0") << QVariant(QByteArray("1.01")) << false << false;

    QTest::newRow("string: false") << QVariant(QString("false")) << false << true;
    QTest::newRow("string: true") << QVariant(QString("true")) << true << true;
    QTest::newRow("string: invalid 1") << QVariant(QString("False")) << false << false;
    QTest::newRow("string: invalid 2") << QVariant(QString("True")) << false << false;
    QTest::newRow("string: invalid 3") << QVariant(QString("asd")) << false << false;

    QTest::newRow("string: < 0.0") << QVariant(QString("-0.01")) << false << false;
    QTest::newRow("string: 0.0") << QVariant(QString("0.0")) << false << true;
    QTest::newRow("string: 1.0") << QVariant(QString("1.0")) << true << true;
    QTest::newRow("string: > 1.0") << QVariant(QString("1.01")) << false << false;

    // Invalid type
    QTest::newRow("Invalid: type 1") << QVariant::fromValue(QChar('0')) << false << false;
    QTest::newRow("Invalid: type 2") << QVariant::fromValue(QChar('1')) << false << false;
    QTest::newRow("Invalid: type 3") << QVariant::fromValue(QChar('A')) << false << false;
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

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << CastNumeric<int8_t>(0)
                               << false;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << CastNumeric<int8_t>(0)
                                << false;

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
    QTest::newRow("byte array: invalid string")
            << QVariant::fromValue(QByteArrayLiteral("123t"))
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
    QTest::newRow("string: invalid string")
            << QVariant::fromValue(QStringLiteral("123t"))
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

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << CastNumeric<uint8_t>(0)
                               << false;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << CastNumeric<uint8_t>(0)
                                << false;

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
    QTest::newRow("byte array: invalid string")
            << QVariant::fromValue(QByteArrayLiteral("123t"))
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
    QTest::newRow("string: invalid string")
            << QVariant::fromValue(QStringLiteral("123t"))
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

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << CastNumeric<int16_t>(0)
                               << false;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << CastNumeric<int16_t>(0)
                                << false;

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
    QTest::newRow("byte array: invalid string")
            << QVariant::fromValue(QByteArrayLiteral("123t"))
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
    QTest::newRow("string: invalid string")
            << QVariant::fromValue(QStringLiteral("123t"))
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

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << CastNumeric<uint16_t>(0)
                               << false;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << CastNumeric<uint16_t>(0)
                                << false;

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
    QTest::newRow("byte array: invalid string")
            << QVariant::fromValue(QByteArrayLiteral("123t"))
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
    QTest::newRow("string: invalid string")
            << QVariant::fromValue(QStringLiteral("123t"))
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

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << CastNumeric<int32_t>(0)
                               << false;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << CastNumeric<int32_t>(0)
                                << false;

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
    QTest::newRow("byte array: invalid string")
            << QVariant::fromValue(QByteArrayLiteral("123t"))
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
    QTest::newRow("string: invalid string")
            << QVariant::fromValue(QStringLiteral("123t"))
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

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << CastNumeric<uint32_t>(0)
                               << false;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << CastNumeric<uint32_t>(0)
                                << false;

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
    QTest::newRow("byte array: invalid string")
            << QVariant::fromValue(QByteArrayLiteral("123t"))
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
    QTest::newRow("string: invalid string")
            << QVariant::fromValue(QStringLiteral("123t"))
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

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << CastNumeric<int64_t>(0)
                               << false;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << CastNumeric<int64_t>(0)
                                << false;

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
            // Workaround for max int64 value that can be exactly represented as a float value
            << QVariant::fromValue(CastNumeric<float>(9223371487098961920LL))
            << CastNumeric<int64_t>(9223371487098961920LL)
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
    QTest::newRow("byte array: invalid string")
            << QVariant::fromValue(QByteArrayLiteral("123t"))
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
    QTest::newRow("string: invalid string")
            << QVariant::fromValue(QStringLiteral("123t"))
            << CastNumeric<int64_t>(0)
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QChar('0'))
                                   << CastNumeric<int64_t>(0)
                                   << false;
}

// Test: load uint64 value --------------------------------------------------------------------------

void TestConfigParameterLoader::testUInt64()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(uint64_t, expectedParameterValue);
    QFETCH(bool, expectedResult);

    uint64_t parameterValue = 0U;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testUInt64: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testUInt64_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<uint64_t>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << CastNumeric<uint64_t>(0)
                               << false;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << CastNumeric<uint64_t>(0)
                                << false;

    // int8
    QTest::newRow("int8: min invalid") << QVariant::fromValue(MinNumeric<int8_t>())
                                       << CastNumeric<uint64_t>(0)
                                       << false;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << CastNumeric<uint64_t>(0)
                             << true;
    QTest::newRow("int8: max valid") << QVariant::fromValue(MaxNumeric<int8_t>())
                                     << CastNumeric<uint64_t>(MaxNumeric<int8_t>())
                                     << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << CastNumeric<uint64_t>(0)
                              << true;
    QTest::newRow("uint8: max") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                << CastNumeric<uint64_t>(MaxNumeric<uint8_t>())
                                << true;

    // int16
    QTest::newRow("int16: min invalid 1") << QVariant::fromValue(MinNumeric<int16_t>())
                                          << CastNumeric<uint64_t>(0)
                                          << false;
    QTest::newRow("int16: min invalid 2") << QVariant::fromValue(CastNumeric<int16_t>(-1))
                                          << CastNumeric<uint64_t>(0)
                                          << false;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << CastNumeric<uint64_t>(0)
                              << true;
    QTest::newRow("int16: max")
            << QVariant::fromValue(MaxNumeric<int16_t>())
            << CastNumeric<uint64_t>(MaxNumeric<int16_t>())
            << true;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << CastNumeric<uint64_t>(0)
                               << true;
    QTest::newRow("uint16: max")
            << QVariant::fromValue(MaxNumeric<uint16_t>())
            << CastNumeric<uint64_t>(MaxNumeric<uint16_t>())
            << true;

    // int32
    QTest::newRow("int32: min invalid 1") << QVariant::fromValue(MinNumeric<int32_t>())
                                          << CastNumeric<uint64_t>(0)
                                          << false;
    QTest::newRow("int32: min invalid 2") << QVariant::fromValue(CastNumeric<int32_t>(-1))
                                          << CastNumeric<uint64_t>(0)
                                          << false;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << CastNumeric<uint64_t>(0)
                              << true;
    QTest::newRow("int32: max")
            << QVariant::fromValue(MaxNumeric<int32_t>())
            << CastNumeric<uint64_t>(MaxNumeric<int32_t>())
            << true;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << CastNumeric<uint64_t>(0)
                               << true;
    QTest::newRow("uint32: max")
            << QVariant::fromValue(MaxNumeric<uint32_t>())
            << CastNumeric<uint64_t>(MaxNumeric<uint32_t>())
            << true;

    // int64
    QTest::newRow("int64: min invalid 1") << QVariant::fromValue(MinNumeric<int64_t>())
                                          << CastNumeric<uint64_t>(0)
                                          << false;
    QTest::newRow("int64: min invalid 2") << QVariant::fromValue(CastNumeric<int64_t>(-1LL))
                                          << CastNumeric<uint64_t>(0)
                                          << false;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << CastNumeric<uint64_t>(0)
                              << true;
    QTest::newRow("int64: max valid")
            << QVariant::fromValue(MaxNumeric<int64_t>())
            << CastNumeric<uint64_t>(MaxNumeric<int64_t>())
            << true;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << CastNumeric<uint64_t>(0)
                               << true;
    QTest::newRow("uint64: max")
            << QVariant::fromValue(MaxNumeric<uint64_t>())
            << MaxNumeric<uint64_t>()
            << true;

    // float
    QTest::newRow("float: min invalid 1") << QVariant::fromValue(MinNumeric<float>())
                                          << CastNumeric<uint64_t>(0)
                                          << false;
    QTest::newRow("float: min invalid 2") << QVariant::fromValue(CastNumeric<float>(-1))
                                          << CastNumeric<uint64_t>(0)
                                          << false;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << CastNumeric<uint64_t>(0)
                              << true;
    QTest::newRow("float: max valid")
            // Workaround for max uint64 value that can be exactly represented as a float value
            << QVariant::fromValue(CastNumeric<float>(18446742974197923840ULL))
            << CastNumeric<uint64_t>(18446742974197923840ULL)
            << true;
    QTest::newRow("float: max invalid 1")
            << QVariant::fromValue(CastNumeric<float>(1.8446746272732807e+19f))
            << CastNumeric<uint64_t>(0)
            << false;
    QTest::newRow("float: max invalid 2") << QVariant::fromValue(MaxNumeric<float>())
                                          << CastNumeric<uint64_t>(0)
                                          << false;

    // double
    QTest::newRow("double: min invalid 1") << QVariant::fromValue(MinNumeric<double>())
                                           << CastNumeric<uint64_t>(0)
                                           << false;
    QTest::newRow("double: min invalid 2") << QVariant::fromValue(CastNumeric<double>(-1))
                                           << CastNumeric<uint64_t>(0)
                                           << false;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                               << CastNumeric<uint64_t>(0)
                               << true;
    QTest::newRow("double: max valid")
            << QVariant::fromValue(CastNumeric<double>(18446744073709549568ULL))
            << CastNumeric<uint64_t>(18446744073709549568ULL)
            << true;
    QTest::newRow("double: max invalid 1")
            << QVariant::fromValue(CastNumeric<double>(1.8446744073709556e+19))
            << CastNumeric<uint64_t>(0)
            << false;
    QTest::newRow("double: max invalid 2") << QVariant::fromValue(MaxNumeric<double>())
                                           << CastNumeric<uint64_t>(0)
                                           << false;

    // byte array
    QTest::newRow("byte array: min invalid") << QVariant::fromValue(QByteArray::number(-1))
                                             << CastNumeric<uint64_t>(0)
                                             << false;
    QTest::newRow("byte array: min valid")
            // Workaround: for some reason uint64_t does not work here
            << QVariant::fromValue(QByteArray::number(MinNumeric<qulonglong>()))
            << MinNumeric<uint64_t>()
            << true;
    QTest::newRow("byte array: 0") << QVariant::fromValue(QByteArray::number(0))
                                   << CastNumeric<uint64_t>(0)
                                   << true;
    QTest::newRow("byte array: max valid")
            // Workaround: for some reason uint64_t does not work here
            << QVariant::fromValue(QByteArray::number(MaxNumeric<qulonglong>()))
            << MaxNumeric<uint64_t>()
            << true;
    QTest::newRow("byte array: max invalid")
            << QVariant::fromValue(QByteArray::number(1.8446744073709556e+19))
            << CastNumeric<uint64_t>(0)
            << false;
    QTest::newRow("byte array: invalid string")
            << QVariant::fromValue(QByteArrayLiteral("123t"))
            << CastNumeric<uint64_t>(0)
            << false;

    // string
    QTest::newRow("string: min invalid") << QVariant::fromValue(QString::number(-1))
                                         << CastNumeric<uint64_t>(0)
                                         << false;
    QTest::newRow("string: min valid")
            << QVariant::fromValue(QString::number(MinNumeric<uint64_t>()))
            << MinNumeric<uint64_t>()
            << true;
    QTest::newRow("string: 0") << QVariant::fromValue(QString::number(0))
                               << CastNumeric<uint64_t>(0)
                               << true;
    QTest::newRow("string: max valid")
            << QVariant::fromValue(QString::number(MaxNumeric<uint64_t>()))
            << MaxNumeric<uint64_t>()
            << true;
    QTest::newRow("string: max invalid")
            << QVariant::fromValue(QString::number(1.8446744073709556e+19))
            << CastNumeric<uint64_t>(0)
            << false;
    QTest::newRow("string: invalid string")
            << QVariant::fromValue(QStringLiteral("123t"))
            << CastNumeric<uint64_t>(0)
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QChar('0'))
                                   << CastNumeric<uint64_t>(0)
                                   << false;
}

// Test: load float value --------------------------------------------------------------------------

void TestConfigParameterLoader::testFloat()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(float, expectedParameterValue);
    QFETCH(bool, expectedResult);

    float parameterValue = 0;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testFloat: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testFloat_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<float>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << CastNumeric<float>(0)
                               << false;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << CastNumeric<float>(0)
                                << false;

    // int8
    QTest::newRow("int8: min") << QVariant::fromValue(MinNumeric<int8_t>())
                               << CastNumeric<float>(MinNumeric<int8_t>())
                               << true;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << CastNumeric<float>(0)
                             << true;
    QTest::newRow("int8: max") << QVariant::fromValue(MaxNumeric<int8_t>())
                               << CastNumeric<float>(MaxNumeric<int8_t>())
                               << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << CastNumeric<float>(0)
                              << true;
    QTest::newRow("uint8: max") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                << CastNumeric<float>(MaxNumeric<uint8_t>())
                                << true;

    // int16
    QTest::newRow("int16: min") << QVariant::fromValue(MinNumeric<int16_t>())
                                << CastNumeric<float>(MinNumeric<int16_t>())
                                << true;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << CastNumeric<float>(0)
                              << true;
    QTest::newRow("int16: max") << QVariant::fromValue(MaxNumeric<int16_t>())
                                << CastNumeric<float>(MaxNumeric<int16_t>())
                                << true;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << CastNumeric<float>(0)
                               << true;
    QTest::newRow("uint16: max") << QVariant::fromValue(MaxNumeric<uint16_t>())
                                 << CastNumeric<float>(MaxNumeric<uint16_t>())
                                 << true;

    // int32
    QTest::newRow("int32: min") << QVariant::fromValue(MinNumeric<int32_t>())
                                << CastNumeric<float>(MinNumeric<int32_t>())
                                << true;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << CastNumeric<float>(0)
                              << true;
    QTest::newRow("int32: max") << QVariant::fromValue(MaxNumeric<int32_t>())
                                << CastNumeric<float>(MaxNumeric<int32_t>())
                                << true;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << CastNumeric<float>(0)
                               << true;
    QTest::newRow("uint32: max") << QVariant::fromValue(MaxNumeric<uint32_t>())
                                 << CastNumeric<float>(MaxNumeric<uint32_t>())
                                 << true;

    // int64
    QTest::newRow("int64: min") << QVariant::fromValue(MinNumeric<int64_t>())
                                << CastNumeric<float>(MinNumeric<int64_t>())
                                << true;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << CastNumeric<float>(0)
                              << true;
    QTest::newRow("int64: max") << QVariant::fromValue(MaxNumeric<int64_t>())
                                << CastNumeric<float>(MaxNumeric<int64_t>())
                                << true;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << CastNumeric<float>(0)
                               << true;
    QTest::newRow("uint64: max")
            // Workaround for max uint64 value that can be exactly represented as a float value
            << QVariant::fromValue(CastNumeric<uint64_t>(18446742974197923840ULL))
            << CastNumeric<float>(18446742974197923840ULL)
            << true;

    // float
    QTest::newRow("float: min") << QVariant::fromValue(MinNumeric<float>())
                                << MinNumeric<float>()
                                << true;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << CastNumeric<float>(0.0f)
                              << true;
    QTest::newRow("float: max") << QVariant::fromValue(MaxNumeric<float>())
                                << MaxNumeric<float>()
                                << true;

    // double
    QTest::newRow("double: min invalid 1") << QVariant::fromValue(MinNumeric<double>())
                                           << CastNumeric<float>(0.0f)
                                           << false;
    QTest::newRow("double: min invalid 2")
            // Workaround for the next value that is smaller than min float
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<float>() - 1.9e22))
            << CastNumeric<float>(0)
            << false;
    QTest::newRow("double: min valid")
            << QVariant::fromValue(CastNumeric<double>(MinNumeric<float>()))
            << MinNumeric<float>()
            << true;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                              << CastNumeric<float>(0)
                              << true;
    QTest::newRow("double: max valid")
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<float>()))
            << MaxNumeric<float>()
            << true;
    QTest::newRow("double: max invalid 1")
            // Workaround for the next value that is bigger than max float
            << QVariant::fromValue(CastNumeric<double>(MaxNumeric<float>() + 1.9e22))
            << CastNumeric<float>(0)
            << false;
    QTest::newRow("double: max invalid 2") << QVariant::fromValue(MaxNumeric<double>())
                                           << CastNumeric<float>(0)
                                           << false;

    // byte array
    QTest::newRow("byte array: min invalid")
            // Workaround for the next value that is smaller than min float
            << QVariant::fromValue(QByteArray::number(MinNumeric<float>() - 1.5337e32))
            << CastNumeric<float>(0)
            << false;
    QTest::newRow("byte array: min valid")
            << QVariant::fromValue(QByteArray::number(MinNumeric<float>()))
            << MinNumeric<float>()
            << true;
    QTest::newRow("byte array: 0") << QVariant::fromValue(QByteArray::number(0))
                                   << CastNumeric<float>(0)
                                   << true;
    QTest::newRow("byte array: max valid")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<float>()))
            << MaxNumeric<float>()
            << true;
    QTest::newRow("byte array: max invalid")
            // Workaround for the next value that is bigger than max float
            << QVariant::fromValue(QByteArray::number(MaxNumeric<float>() + 1.5337e32))
            << CastNumeric<float>(0)
            << false;
    QTest::newRow("byte array: invalid string")
            << QVariant::fromValue(QByteArrayLiteral("123t"))
            << CastNumeric<float>(0)
            << false;

    // string
    QTest::newRow("string: min invalid")
            // Workaround for the next value that is smaller than min float
            << QVariant::fromValue(QString::number(MinNumeric<float>() - 1.5337e32))
            << CastNumeric<float>(0)
            << false;
    QTest::newRow("string: min valid")
            << QVariant::fromValue(QString::number(MinNumeric<float>()))
            << MinNumeric<float>()
            << true;
    QTest::newRow("string: 0") << QVariant::fromValue(QString::number(0))
                               << CastNumeric<float>(0)
                               << true;
    QTest::newRow("string: max valid")
            << QVariant::fromValue(QString::number(MaxNumeric<float>()))
            << MaxNumeric<float>()
            << true;
    QTest::newRow("string: max invalid")
            // Workaround for the next value that is bigger than max float
            << QVariant::fromValue(QString::number(MaxNumeric<float>() + 1.5337e32))
            << CastNumeric<float>(0)
            << false;
    QTest::newRow("string: invalid string")
            << QVariant::fromValue(QStringLiteral("123t"))
            << CastNumeric<float>(0)
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QChar('0'))
                                   << CastNumeric<float>(0)
                                   << false;
}

// Test: load double value -------------------------------------------------------------------------

void TestConfigParameterLoader::testDouble()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(double, expectedParameterValue);
    QFETCH(bool, expectedResult);

    double parameterValue = 0;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testDouble: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testDouble_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<double>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << CastNumeric<double>(0)
                               << false;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << CastNumeric<double>(0)
                                << false;

    // int8
    QTest::newRow("int8: min") << QVariant::fromValue(MinNumeric<int8_t>())
                               << CastNumeric<double>(MinNumeric<int8_t>())
                               << true;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << CastNumeric<double>(0)
                             << true;
    QTest::newRow("int8: max") << QVariant::fromValue(MaxNumeric<int8_t>())
                               << CastNumeric<double>(MaxNumeric<int8_t>())
                               << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << CastNumeric<double>(0)
                              << true;
    QTest::newRow("uint8: max") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                << CastNumeric<double>(MaxNumeric<uint8_t>())
                                << true;

    // int16
    QTest::newRow("int16: min") << QVariant::fromValue(MinNumeric<int16_t>())
                                << CastNumeric<double>(MinNumeric<int16_t>())
                                << true;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << CastNumeric<double>(0)
                              << true;
    QTest::newRow("int16: max") << QVariant::fromValue(MaxNumeric<int16_t>())
                                << CastNumeric<double>(MaxNumeric<int16_t>())
                                << true;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << CastNumeric<double>(0)
                               << true;
    QTest::newRow("uint16: max") << QVariant::fromValue(MaxNumeric<uint16_t>())
                                 << CastNumeric<double>(MaxNumeric<uint16_t>())
                                 << true;

    // int32
    QTest::newRow("int32: min") << QVariant::fromValue(MinNumeric<int32_t>())
                                << CastNumeric<double>(MinNumeric<int32_t>())
                                << true;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << CastNumeric<double>(0)
                              << true;
    QTest::newRow("int32: max") << QVariant::fromValue(MaxNumeric<int32_t>())
                                << CastNumeric<double>(MaxNumeric<int32_t>())
                                << true;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << CastNumeric<double>(0)
                               << true;
    QTest::newRow("uint32: max") << QVariant::fromValue(MaxNumeric<uint32_t>())
                                 << CastNumeric<double>(MaxNumeric<uint32_t>())
                                 << true;

    // int64
    QTest::newRow("int64: min") << QVariant::fromValue(MinNumeric<int64_t>())
                                << CastNumeric<double>(MinNumeric<int64_t>())
                                << true;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << CastNumeric<double>(0)
                              << true;
    QTest::newRow("int64: max") << QVariant::fromValue(MaxNumeric<int64_t>())
                                << CastNumeric<double>(MaxNumeric<int64_t>())
                                << true;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << CastNumeric<double>(0)
                               << true;
    QTest::newRow("uint64: max") << QVariant::fromValue(MaxNumeric<uint64_t>())
                                 << CastNumeric<double>(MaxNumeric<uint64_t>())
                                 << true;

    // float
    QTest::newRow("float: min") << QVariant::fromValue(MinNumeric<float>())
                                << CastNumeric<double>(MinNumeric<float>())
                                << true;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << CastNumeric<double>(0.0)
                              << true;
    QTest::newRow("float: max") << QVariant::fromValue(MaxNumeric<float>())
                                << CastNumeric<double>(MaxNumeric<float>())
                                << true;

    // double
    QTest::newRow("double: min")
            << QVariant::fromValue(MinNumeric<double>())
            << MinNumeric<double>()
            << true;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                              << CastNumeric<double>(0.0)
                              << true;
    QTest::newRow("double: max") << QVariant::fromValue(MaxNumeric<double>())
                                 << MaxNumeric<double>()
                                 << true;

    // byte array
    QTest::newRow("byte array: min")
            << QVariant::fromValue(QByteArray::number(MinNumeric<double>(), 'g', 14))
            << MinNumeric<double>()
            << true;
    QTest::newRow("byte array: 0") << QVariant::fromValue(QByteArray::number(0))
                                   << CastNumeric<double>(0.0)
                                   << true;
    QTest::newRow("byte array: max")
            << QVariant::fromValue(QByteArray::number(MaxNumeric<double>(), 'g', 14))
            << MaxNumeric<double>()
            << true;
    QTest::newRow("byte array: invalid string")
            << QVariant::fromValue(QByteArrayLiteral("123t"))
            << CastNumeric<double>(0)
            << false;

    // string
    QTest::newRow("string: min")
            << QVariant::fromValue(QString::number(MinNumeric<double>(), 'g', 14))
            << MinNumeric<double>()
            << true;
    QTest::newRow("string: 0") << QVariant::fromValue(QString::number(0))
                                   << CastNumeric<double>(0.0)
                                   << true;
    QTest::newRow("string: max")
            << QVariant::fromValue(QString::number(MaxNumeric<double>(), 'g', 14))
            << MaxNumeric<double>()
            << true;
    QTest::newRow("string: invalid string")
            << QVariant::fromValue(QStringLiteral("123t"))
            << CastNumeric<double>(0)
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QChar('0'))
                                   << CastNumeric<double>(0)
                                   << false;
}

// Test: load char value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testChar()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QChar, expectedParameterValue);
    QFETCH(bool, expectedResult);

    QChar parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testChar: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testChar_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QChar>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // QChar
    QTest::newRow("char: '\\0'") << QVariant::fromValue(QChar('\0'))
                               << QChar('\0')
                               << true;
    QTest::newRow("char: '0'") << QVariant::fromValue(QChar('0'))
                               << QChar('0')
                               << true;
    QTest::newRow("char: '9'") << QVariant::fromValue(QChar('9'))
                               << QChar('9')
                               << true;
    QTest::newRow("char: 'A'") << QVariant::fromValue(QChar('A'))
                               << QChar('A')
                               << true;
    QTest::newRow("char: 'Z'") << QVariant::fromValue(QChar('Z'))
                               << QChar('Z')
                               << true;
    QTest::newRow("char: 'a'") << QVariant::fromValue(QChar('a'))
                               << QChar('a')
                               << true;
    QTest::newRow("char: 'z'") << QVariant::fromValue(QChar('z'))
                               << QChar('z')
                               << true;

    // byte array
    QTest::newRow("byte array: '0'") << QVariant::fromValue(QByteArray("0"))
                                       << QChar('0')
                                       << true;
    QTest::newRow("byte array: '9'") << QVariant::fromValue(QByteArray("9"))
                                     << QChar('9')
                                     << true;
    QTest::newRow("byte array: 'A'") << QVariant::fromValue(QByteArray("A"))
                                     << QChar('A')
                                     << true;
    QTest::newRow("byte array: 'Z'") << QVariant::fromValue(QByteArray("Z"))
                                     << QChar('Z')
                                     << true;
    QTest::newRow("byte array: 'a'") << QVariant::fromValue(QByteArray("a"))
                                     << QChar('a')
                                     << true;
    QTest::newRow("byte array: 'z'") << QVariant::fromValue(QByteArray("z"))
                                     << QChar('z')
                                     << true;
    QTest::newRow("byte array: '\\0'") << QVariant::fromValue(QByteArray("\0"))
                                       << QChar()
                                       << false;
    QTest::newRow("byte array: invalid too short") << QVariant::fromValue(QByteArray(""))
                                                   << QChar()
                                                   << false;
    QTest::newRow("byte array: invalid too long") << QVariant::fromValue(QByteArray("aa"))
                                                  << QChar()
                                                  << false;

    // string
    QTest::newRow("string: '0'") << QVariant::fromValue(QString("0"))
                                     << QChar('0')
                                     << true;
    QTest::newRow("string: '9'") << QVariant::fromValue(QString("9"))
                                     << QChar('9')
                                     << true;
    QTest::newRow("string: 'A'") << QVariant::fromValue(QString("A"))
                                     << QChar('A')
                                     << true;
    QTest::newRow("string: 'Z'") << QVariant::fromValue(QString("Z"))
                                     << QChar('Z')
                                     << true;
    QTest::newRow("string: 'a'") << QVariant::fromValue(QString("a"))
                                     << QChar('a')
                                     << true;
    QTest::newRow("string: 'z'") << QVariant::fromValue(QString("z"))
                                     << QChar('z')
                                     << true;
    QTest::newRow("string: '\\0'") << QVariant::fromValue(QString("\0"))
                                   << QChar()
                                   << false;
    QTest::newRow("string: invalid too short") << QVariant::fromValue(QString(""))
                                                   << QChar()
                                                   << false;
    QTest::newRow("string: invalid too long") << QVariant::fromValue(QString("aa"))
                                                  << QChar()
                                                  << false;

    // Invalid type
    QTest::newRow("Invalid: bool") << QVariant::fromValue(true) << QChar() << false;
    QTest::newRow("Invalid: int8") << QVariant::fromValue(CastNumeric<int8_t>(50))
                                   << QChar()
                                   << false;
    QTest::newRow("Invalid: uint8") << QVariant::fromValue(CastNumeric<uint8_t>(50))
                                    << QChar()
                                    << false;
    QTest::newRow("Invalid: int16") << QVariant::fromValue(CastNumeric<int16_t>(50))
                                    << QChar()
                                    << false;
    QTest::newRow("Invalid: uint16") << QVariant::fromValue(CastNumeric<uint16_t>(50))
                                     << QChar()
                                     << false;
    QTest::newRow("Invalid: int32") << QVariant::fromValue(CastNumeric<int32_t>(50))
                                    << QChar()
                                    << false;
    QTest::newRow("Invalid: uint32") << QVariant::fromValue(CastNumeric<uint32_t>(50))
                                     << QChar()
                                     << false;
    QTest::newRow("Invalid: int64") << QVariant::fromValue(CastNumeric<int64_t>(50))
                                    << QChar()
                                    << false;
    QTest::newRow("Invalid: uint64") << QVariant::fromValue(CastNumeric<uint64_t>(50))
                                     << QChar()
                                     << false;
    QTest::newRow("Invalid: float") << QVariant::fromValue(CastNumeric<float>(50))
                                    << QChar()
                                    << false;
    QTest::newRow("Invalid: double") << QVariant::fromValue(CastNumeric<double>(50))
                                     << QChar()
                                     << false;
}

// Test: load string value -------------------------------------------------------------------------

void TestConfigParameterLoader::testString()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QString, expectedParameterValue);
    QFETCH(bool, expectedResult);

    QString parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testString: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testString_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QString>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << QString("false")
                               << true;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << QString("true")
                                << true;

    // int8
    QTest::newRow("int8: min") << QVariant::fromValue(MinNumeric<int8_t>())
                               << QString::number(MinNumeric<int8_t>())
                               << true;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << QString::number(0)
                             << true;
    QTest::newRow("int8: max") << QVariant::fromValue(MaxNumeric<int8_t>())
                               << QString::number(MaxNumeric<int8_t>())
                               << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << QString::number(0)
                              << true;
    QTest::newRow("uint8: max") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                << QString::number(MaxNumeric<uint8_t>())
                                << true;

    // int16
    QTest::newRow("int16: min") << QVariant::fromValue(MinNumeric<int16_t>())
                                << QString::number(MinNumeric<int16_t>())
                                << true;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << QString::number(0)
                              << true;
    QTest::newRow("int16: max") << QVariant::fromValue(MaxNumeric<int16_t>())
                                << QString::number(MaxNumeric<int16_t>())
                                << true;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << QString::number(0)
                               << true;
    QTest::newRow("uint16: max") << QVariant::fromValue(MaxNumeric<uint16_t>())
                                 << QString::number(MaxNumeric<uint16_t>())
                                 << true;

    // int32
    QTest::newRow("int32: min") << QVariant::fromValue(MinNumeric<int32_t>())
                                << QString::number(MinNumeric<int32_t>())
                                << true;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << QString::number(0)
                              << true;
    QTest::newRow("int32: max") << QVariant::fromValue(MaxNumeric<int32_t>())
                                << QString::number(MaxNumeric<int32_t>())
                                << true;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << QString::number(0)
                               << true;
    QTest::newRow("uint32: max") << QVariant::fromValue(MaxNumeric<uint32_t>())
                                 << QString::number(MaxNumeric<uint32_t>())
                                 << true;

    // int64
    QTest::newRow("int64: min") << QVariant::fromValue(MinNumeric<int64_t>())
                                << QString::number(MinNumeric<int64_t>())
                                << true;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << QString::number(0)
                              << true;
    QTest::newRow("int64: max") << QVariant::fromValue(MaxNumeric<int64_t>())
                                << QString::number(MaxNumeric<int64_t>())
                                << true;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << QString::number(0)
                               << true;
    QTest::newRow("uint64: max") << QVariant::fromValue(MaxNumeric<uint64_t>())
                                 << QString::number(MaxNumeric<uint64_t>())
                                 << true;

    // float
    QTest::newRow("float: min") << QVariant::fromValue(MinNumeric<float>())
                                << QString::number(MinNumeric<float>())
                                << true;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << QString::number(0.0)
                              << true;
    QTest::newRow("float: max") << QVariant::fromValue(MaxNumeric<float>())
                                << QString::number(MaxNumeric<float>())
                                << true;

    // double
    QTest::newRow("double: min") << QVariant::fromValue(MinNumeric<double>())
                                 << QString::number(MinNumeric<double>())
                                 << true;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                              << QString::number(0.0)
                              << true;
    QTest::newRow("double: max") << QVariant::fromValue(MaxNumeric<double>())
                                 << QString::number(MaxNumeric<double>())
                                 << true;

    // byte array
    QTest::newRow("byte array") << QVariant::fromValue(QByteArray("test string"))
                                << QString::fromUtf8(QByteArray("test string"))
                                << true;

    // string
    QTest::newRow("string") << QVariant::fromValue(QString("test string"))
                            << QString("test string")
                            << true;

    // char
    QTest::newRow("char") << QVariant::fromValue(QChar('0')) << QString("0") << true;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QMap<int, bool>())
                                   << QString()
                                   << false;
}

// Test: load byte array value ---------------------------------------------------------------------

void TestConfigParameterLoader::testByteArray()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QByteArray, expectedParameterValue);
    QFETCH(bool, expectedResult);

    QByteArray parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testByteArray: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testByteArray_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QByteArray>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << QByteArray("false")
                               << true;
    QTest::newRow("bool: true") << QVariant::fromValue(true)
                                << QByteArray("true")
                                << true;

    // int8
    QTest::newRow("int8: min") << QVariant::fromValue(MinNumeric<int8_t>())
                               << QByteArray::number(MinNumeric<int8_t>())
                               << true;
    QTest::newRow("int8: 0") << QVariant::fromValue(CastNumeric<int8_t>(0))
                             << QByteArray::number(0)
                             << true;
    QTest::newRow("int8: max") << QVariant::fromValue(MaxNumeric<int8_t>())
                               << QByteArray::number(MaxNumeric<int8_t>())
                               << true;

    // uint8
    QTest::newRow("uint8: 0") << QVariant::fromValue(CastNumeric<uint8_t>(0))
                              << QByteArray::number(0)
                              << true;
    QTest::newRow("uint8: max") << QVariant::fromValue(MaxNumeric<uint8_t>())
                                << QByteArray::number(MaxNumeric<uint8_t>())
                                << true;

    // int16
    QTest::newRow("int16: min") << QVariant::fromValue(MinNumeric<int16_t>())
                                << QByteArray::number(MinNumeric<int16_t>())
                                << true;
    QTest::newRow("int16: 0") << QVariant::fromValue(CastNumeric<int16_t>(0))
                              << QByteArray::number(0)
                              << true;
    QTest::newRow("int16: max") << QVariant::fromValue(MaxNumeric<int16_t>())
                                << QByteArray::number(MaxNumeric<int16_t>())
                                << true;

    // uint16
    QTest::newRow("uint16: 0") << QVariant::fromValue(CastNumeric<uint16_t>(0))
                               << QByteArray::number(0)
                               << true;
    QTest::newRow("uint16: max") << QVariant::fromValue(MaxNumeric<uint16_t>())
                                 << QByteArray::number(MaxNumeric<uint16_t>())
                                 << true;

    // int32
    QTest::newRow("int32: min") << QVariant::fromValue(MinNumeric<int32_t>())
                                << QByteArray::number(MinNumeric<int32_t>())
                                << true;
    QTest::newRow("int32: 0") << QVariant::fromValue(CastNumeric<int32_t>(0))
                              << QByteArray::number(0)
                              << true;
    QTest::newRow("int32: max") << QVariant::fromValue(MaxNumeric<int32_t>())
                                << QByteArray::number(MaxNumeric<int32_t>())
                                << true;

    // uint32
    QTest::newRow("uint32: 0") << QVariant::fromValue(CastNumeric<uint32_t>(0))
                               << QByteArray::number(0)
                               << true;
    QTest::newRow("uint32: max") << QVariant::fromValue(MaxNumeric<uint32_t>())
                                 << QByteArray::number(MaxNumeric<uint32_t>())
                                 << true;

    // int64
    QTest::newRow("int64: min")
            // Workaround: for some reason int64_t does not work here
            << QVariant::fromValue(MinNumeric<int64_t>())
            << QByteArray::number(MinNumeric<qlonglong>())
            << true;
    QTest::newRow("int64: 0") << QVariant::fromValue(CastNumeric<int64_t>(0))
                              << QByteArray::number(0)
                              << true;
    QTest::newRow("int64: max")
            // Workaround: for some reason int64_t does not work here
            << QVariant::fromValue(MaxNumeric<int64_t>())
            << QByteArray::number(MaxNumeric<qlonglong>())
            << true;

    // uint64
    QTest::newRow("uint64: 0") << QVariant::fromValue(CastNumeric<uint64_t>(0))
                               << QByteArray::number(0)
                               << true;
    QTest::newRow("uint64: max")
            // Workaround: for some reason uint64_t does not work here
            << QVariant::fromValue(MaxNumeric<uint64_t>())
            << QByteArray::number(MaxNumeric<qulonglong>())
            << true;

    // float
    QTest::newRow("float: min") << QVariant::fromValue(MinNumeric<float>())
                                << QByteArray::number(MinNumeric<float>())
                                << true;
    QTest::newRow("float: 0") << QVariant::fromValue(CastNumeric<float>(0.0f))
                              << QByteArray::number(0.0)
                              << true;
    QTest::newRow("float: max") << QVariant::fromValue(MaxNumeric<float>())
                                << QByteArray::number(MaxNumeric<float>())
                                << true;

    // double
    QTest::newRow("double: min") << QVariant::fromValue(MinNumeric<double>())
                                 << QByteArray::number(MinNumeric<double>())
                                 << true;
    QTest::newRow("double: 0") << QVariant::fromValue(CastNumeric<double>(0.0))
                              << QByteArray::number(0.0)
                              << true;
    QTest::newRow("double: max") << QVariant::fromValue(MaxNumeric<double>())
                                 << QByteArray::number(MaxNumeric<double>())
                                 << true;

    // byte array
    QTest::newRow("byte array") << QVariant::fromValue(QByteArray("test string"))
                                << QByteArray("test string")
                                << true;

    // string
    QTest::newRow("string") << QVariant::fromValue(QString("test string"))
                            << QByteArray("test string")
                            << true;

    // char
    QTest::newRow("char") << QVariant::fromValue(QChar('0')) << QByteArray("0") << true;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QMap<int, bool>())
                                   << QByteArray()
                                   << false;
}

// Test: load string value -------------------------------------------------------------------------

void TestConfigParameterLoader::testStdString()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QString, expectedParameterValue);
    QFETCH(bool, expectedResult);

    // std::string
    std::string parameterValue1;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue1, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testString: error string:" << error;

    QCOMPARE(parameterValue1, expectedParameterValue.toStdString());

    // std::wstring
    std::wstring parameterValue2;
    error.clear();
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue2, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testString: error string:" << error;

    QCOMPARE(parameterValue2, expectedParameterValue.toStdWString());

    // std::u16string
    std::u16string parameterValue3;
    error.clear();
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue3, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testString: error string:" << error;

    QCOMPARE(parameterValue3, expectedParameterValue.toStdU16String());

    // std::u32string
    std::u32string parameterValue4;
    error.clear();
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue4, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testString: error string:" << error;

    QCOMPARE(parameterValue4, expectedParameterValue.toStdU32String());
}

void TestConfigParameterLoader::testStdString_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QString>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // bool
    QTest::newRow("bool: false") << QVariant::fromValue(false)
                               << QString("false")
                               << true;

    // int32
    QTest::newRow("int32: min") << QVariant::fromValue(MinNumeric<int32_t>())
                                << QString::number(MinNumeric<int32_t>())
                                << true;

    // uint32
    QTest::newRow("uint32: max") << QVariant::fromValue(MaxNumeric<uint32_t>())
                                 << QString::number(MaxNumeric<uint32_t>())
                                 << true;

    // byte array
    QTest::newRow("byte array") << QVariant::fromValue(QByteArray("test string"))
                                << QString("test string")
                                << true;

    // string
    QTest::newRow("string") << QVariant::fromValue(QString("test string"))
                            << QString("test string")
                            << true;

    // char
    QTest::newRow("char") << QVariant::fromValue(QChar('0')) << QString("0") << true;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(QMap<int, bool>())
                                   << QString()
                                   << false;
}

// Test: load date value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testDate()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QDate, expectedParameterValue);
    QFETCH(bool, expectedResult);

    QDate parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testDate: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testDate_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QDate>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // date
    QTest::newRow("date: valid") << QVariant::fromValue(QDate(2019, 11, 8))
                                 << QDate(2019, 11, 8)
                                 << true;
    QTest::newRow("date: null node value") << QVariant()
                                           << QDate()
                                           << false;
    QTest::newRow("date: null date") << QVariant::fromValue(QDate())
                                     << QDate()
                                     << false;
    QTest::newRow("date: invalid date value") << QVariant::fromValue(QDate(2019, 13, 8))
                                              << QDate()
                                              << false;

    // string
    QTest::newRow("string: valid") << QVariant::fromValue(QString("2019-11-08"))
                                   << QDate(2019, 11, 8)
                                   << true;
    QTest::newRow("string: empty") << QVariant::fromValue(QString())
                                   << QDate()
                                   << false;
    QTest::newRow("string: invalid") << QVariant::fromValue(QString("2019-13-08"))
                                     << QDate()
                                     << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(true)
                                   << QDate()
                                   << false;
}

// Test: load time value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testTime()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QTime, expectedParameterValue);
    QFETCH(bool, expectedResult);

    QTime parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testTime: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testTime_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QTime>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // time
    QTest::newRow("time: valid") << QVariant::fromValue(QTime(22, 33, 44, 555))
                                 << QTime(22, 33, 44, 555)
                                 << true;
    QTest::newRow("time: null node value") << QVariant()
                                           << QTime()
                                           << false;
    QTest::newRow("time: null time") << QVariant::fromValue(QTime())
                                     << QTime()
                                     << false;
    QTest::newRow("time: invalid time value") << QVariant::fromValue(QTime(24, 33, 44, 555))
                                              << QTime()
                                              << false;

    // string
    QTest::newRow("string: valid") << QVariant::fromValue(QString("22:33:44.555"))
                                   << QTime(22, 33, 44, 555)
                                   << true;
    QTest::newRow("string: empty") << QVariant::fromValue(QString())
                                   << QTime()
                                   << false;
    QTest::newRow("string: invalid") << QVariant::fromValue(QString("24:33:44.555"))
                                     << QTime()
                                     << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(true)
                                   << QTime()
                                   << false;
}

// Test: load datetime value -----------------------------------------------------------------------

void TestConfigParameterLoader::testDateTime()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QDateTime, expectedParameterValue);
    QFETCH(bool, expectedResult);

    QDateTime parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testDateTime: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testDateTime_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QDateTime>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // datetime
    QTest::newRow("datetime: valid")
            << QVariant::fromValue(QDateTime(QDate(2019, 11, 8), QTime(22, 33, 44, 555), Qt::UTC))
            << QDateTime(QDate(2019, 11, 8), QTime(22, 33, 44, 555), Qt::UTC)
            << true;
    QTest::newRow("datetime: null node value") << QVariant()
                                               << QDateTime()
                                               << false;
    QTest::newRow("datetime: null datetime") << QVariant::fromValue(QDateTime())
                                             << QDateTime()
                                             << false;
    QTest::newRow("datetime: invalid time value")
            // Note: this works because QDateTime always converts the invalid time component to
            //       midnight!
            << QVariant::fromValue(QDateTime(QDate(2019, 11, 8), QTime(24, 33, 44, 555), Qt::UTC))
            << QDateTime(QDate(2019, 11, 8), QTime(0, 0, 0, 0), Qt::UTC)
            << true;
    QTest::newRow("datetime: invalid date value")
            << QVariant::fromValue(QDateTime(QDate(2019, 13, 8), QTime(22, 33, 44, 555), Qt::UTC))
            << QDateTime()
            << false;

    // string
    QTest::newRow("string: valid") << QVariant::fromValue(QString("2019-11-08T22:33:44.555Z"))
                                   << QDateTime(QDate(2019, 11, 8), QTime(22, 33, 44, 555), Qt::UTC)
                                   << true;
    QTest::newRow("string: empty") << QVariant::fromValue(QString())
                                   << QDateTime()
                                   << false;
    QTest::newRow("string: invalid 1") << QVariant::fromValue(QString("2019-13-08T22:33:44.555Z"))
                                       << QDateTime()
                                       << false;
    QTest::newRow("string: invalid 2") << QVariant::fromValue(QString("2019-11-08T24:33:44.555Z"))
                                       << QDateTime()
                                       << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(true)
                                   << QDateTime()
                                   << false;
}

// Test: load variant value ------------------------------------------------------------------------

void TestConfigParameterLoader::testVariant()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(bool, expectedResult);

    QVariant parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testVariant: error string:" << error;

    QCOMPARE(parameterValue, nodeValue);
}

void TestConfigParameterLoader::testVariant_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("bool") << QVariant::fromValue(true) << true;
    QTest::newRow("int") << QVariant::fromValue(123) << true;
    QTest::newRow("string") << QVariant::fromValue(QString("test")) << true;
    QTest::newRow("datetime")
            << QVariant::fromValue(QDateTime(QDate(2019, 11, 8), QTime(22, 33, 44, 555), Qt::UTC))
            << true;
}

// Test: load URL value ----------------------------------------------------------------------------

void TestConfigParameterLoader::testUrl()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QUrl, expectedParameterValue);
    QFETCH(bool, expectedResult);

    QUrl parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testUrl: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testUrl_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QUrl>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // URL
    QTest::newRow("date: valid") << QVariant::fromValue(QUrl("http://www.google.com"))
                                 << QUrl("http://www.google.com")
                                 << true;
    QTest::newRow("date: null node value") << QVariant()
                                           << QUrl()
                                           << false;
    QTest::newRow("date: null URL") << QVariant::fromValue(QUrl())
                                     << QUrl()
                                     << false;
    QTest::newRow("date: invalid URL value") << QVariant::fromValue(QUrl("http://..."))
                                              << QUrl()
                                              << false;

    // string
    QTest::newRow("string: valid") << QVariant::fromValue(QString("http://www.google.com"))
                                   << QUrl("http://www.google.com")
                                   << true;
    QTest::newRow("string: empty") << QVariant::fromValue(QString())
                                   << QUrl()
                                   << false;
    QTest::newRow("string: invalid") << QVariant::fromValue(QString("http://..."))
                                     << QUrl()
                                     << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(true)
                                   << QUrl()
                                   << false;
}

// Test: load UUID value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testUuid()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QUuid, expectedParameterValue);
    QFETCH(bool, expectedResult);

    QUuid parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testUuid: error string:" << error;

    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testUuid_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QUuid>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // UUID
    QTest::newRow("date: valid")
            << QVariant::fromValue(QUuid("{01234567-8901-2345-6789-012345678901}"))
            << QUuid("{01234567-8901-2345-6789-012345678901}")
            << true;
    QTest::newRow("date: null node value") << QVariant()
                                           << QUuid()
                                           << false;
    QTest::newRow("date: null UUID") << QVariant::fromValue(QUuid())
                                    << QUuid()
                                    << false;
    QTest::newRow("date: invalid UUID value")
            << QVariant::fromValue(QUuid("{01234567890123456789012345678901}"))
            << QUuid()
            << false;

    // string
    QTest::newRow("string: valid")
            << QVariant::fromValue(QString("{01234567-8901-2345-6789-012345678901}"))
            << QUuid("{01234567-8901-2345-6789-012345678901}")
            << true;
    QTest::newRow("string: empty") << QVariant::fromValue(QString())
                                   << QUuid()
                                   << false;
    QTest::newRow("string: invalid")
            << QVariant::fromValue(QString("{01234567890123456789012345678901}"))
            << QUuid()
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(true)
                                   << QUuid()
                                   << false;
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigParameterLoader)
#include "testConfigParameterLoader.moc"
