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
#include <QtCore/QLine>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test types --------------------------------------------------------------------------------------

using TestPair = QPair<QString, int>;
using TestStdPair = std::pair<QString, int>;
using TestList = QList<QDate>;
using TestStdList = std::list<QTime>;
using TestVector = QVector<float>;
using TestStdVector = std::vector<double>;
using TestSet = QSet<int>;
using TestMap = QMap<QString, int>;
using TestStdMap = std::map<QString, int>;
using TestHash = QHash<QString, int>;
using TestStdHash = std::unordered_map<QString, int>;
using TestMultiMap = QMultiMap<QString, int>;
using TestMultiHash = QMultiHash<QString, int>;

Q_DECLARE_METATYPE(TestStdHash);
Q_DECLARE_METATYPE(TestMultiMap);
Q_DECLARE_METATYPE(TestMultiHash);

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

    void testSize();
    void testSize_data();

    void testPoint();
    void testPoint_data();

    void testLine();
    void testLine_data();

    void testRect();
    void testRect_data();

    void testStringList();
    void testStringList_data();

    void testPair();
    void testPair_data();

    void testList();
    void testList_data();

    void testStdList();
    void testStdList_data();

    void testVector();
    void testVector_data();

    void testStdVector();
    void testStdVector_data();

    void testSet();
    void testSet_data();

    void testMap();
    void testMap_data();

    void testStdMap();
    void testStdMap_data();

    void testHash();
    void testHash_data();

    void testStdHash();
    void testStdHash_data();

    void testMultiMap();
    void testMultiMap_data();

    void testMultiHash();
    void testMultiHash_data();
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
    QTest::newRow("URL: valid") << QVariant::fromValue(QUrl("http://www.google.com"))
                                << QUrl("http://www.google.com")
                                << true;
    QTest::newRow("URL: null node value") << QVariant()
                                          << QUrl()
                                          << false;
    QTest::newRow("URL: null URL") << QVariant::fromValue(QUrl())
                                   << QUrl()
                                   << false;
    QTest::newRow("URL: invalid URL value") << QVariant::fromValue(QUrl("http://..."))
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
    QTest::newRow("UUID: valid")
            << QVariant::fromValue(QUuid("{01234567-8901-2345-6789-012345678901}"))
            << QUuid("{01234567-8901-2345-6789-012345678901}")
            << true;
    QTest::newRow("UUID: null node value") << QVariant()
                                           << QUuid()
                                           << false;
    QTest::newRow("UUID: null UUID") << QVariant::fromValue(QUuid())
                                     << QUuid()
                                     << false;
    QTest::newRow("UUID: invalid UUID value")
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

// Test: load size value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testSize()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QSize, expectedParameterValue1);
    QFETCH(QSizeF, expectedParameterValue2);
    QFETCH(bool, expectedResult);

    QSize parameterValue1;
    QString error1;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue1, &error1), expectedResult);
    qDebug() << "TestConfigParameterLoader::testSize: error string:" << error1;
    QCOMPARE(parameterValue1, expectedParameterValue1);

    QSizeF parameterValue2;
    QString error2;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue2, &error2), expectedResult);
    qDebug() << "TestConfigParameterLoader::testSize: error string:" << error2;
    QCOMPARE(parameterValue2, expectedParameterValue2);
}

void TestConfigParameterLoader::testSize_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QSize>("expectedParameterValue1");
    QTest::addColumn<QSizeF>("expectedParameterValue2");
    QTest::addColumn<bool>("expectedResult");

    // size with integers
    QTest::newRow("size (int): valid")
            << QVariant::fromValue(QSize(100, 200))
            << QSize(100, 200)
            << QSizeF(100.0, 200.0)
            << true;
    QTest::newRow("size (int): null node value") << QVariant()
                                                 << QSize()
                                                 << QSizeF()
                                                 << false;
    QTest::newRow("size (int): null size") << QVariant::fromValue(QSize())
                                           << QSize()
                                           << QSizeF()
                                           << false;
    QTest::newRow("size (int): invalid size value")
            << QVariant::fromValue(QSize(-1, -1))
            << QSize()
            << QSizeF()
            << false;

    // size with floating-point
    QTest::newRow("size (double): valid")
            << QVariant::fromValue(QSize(100.0, 200.0))
            << QSize(100, 200)
            << QSizeF(100.0, 200.0)
            << true;
    QTest::newRow("size (double): null node value") << QVariant()
                                                    << QSize()
                                                    << QSizeF()
                                                    << false;
    QTest::newRow("size (double): null size") << QVariant::fromValue(QSizeF())
                                              << QSize()
                                              << QSizeF()
                                              << false;
    QTest::newRow("size (double): invalid size value")
            << QVariant::fromValue(QSize(-1.0, -1.0))
            << QSize()
            << QSizeF()
            << false;

    // map
    QTest::newRow("map: valid")
            << QVariant::fromValue(QVariantMap { {"width", 100}, {"height", 200} } )
            << QSize(100, 200)
            << QSizeF(100.0, 200.0)
            << true;
    QTest::newRow("map: empty") << QVariant::fromValue(QVariantMap())
                                << QSize()
                                << QSizeF()
                                << false;
    QTest::newRow("map: missing height")
            << QVariant::fromValue(QVariantMap { {"width", 100} } )
            << QSize()
            << QSizeF()
            << false;
    QTest::newRow("map: missing width")
            << QVariant::fromValue(QVariantMap { {"height", 200} } )
            << QSize()
            << QSizeF()
            << false;
    QTest::newRow("map: invalid keys")
            << QVariant::fromValue(QVariantMap { {"WIDTH", 100}, {"HEIGHT", 200} } )
            << QSize()
            << QSizeF()
            << false;
    QTest::newRow("map: invalid width")
            << QVariant::fromValue(QVariantMap { {"width", "asd"}, {"height", 200} } )
            << QSize()
            << QSizeF()
            << false;
    QTest::newRow("map: invalid height")
            << QVariant::fromValue(QVariantMap { {"width", 100}, {"height", "asd"} } )
            << QSize()
            << QSizeF()
            << false;
    QTest::newRow("map: too many members")
            << QVariant::fromValue(QVariantMap { {"width", 100}, {"height", 200}, {"asd", 0} } )
            << QSize()
            << QSizeF()
            << false;
    QTest::newRow("map: invalid size")
            << QVariant::fromValue(QVariantMap { {"width", -1}, {"height", -1} } )
            << QSize()
            << QSizeF()
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(true)
                                   << QSize()
                                   << QSizeF()
                                   << false;
}

// Test: load point value --------------------------------------------------------------------------

void TestConfigParameterLoader::testPoint()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QPoint, expectedParameterValue1);
    QFETCH(QPointF, expectedParameterValue2);
    QFETCH(bool, expectedResult);

    QPoint parameterValue1;
    QString error1;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue1, &error1), expectedResult);
    qDebug() << "TestConfigParameterLoader::testPoint: error string:" << error1;
    QCOMPARE(parameterValue1, expectedParameterValue1);

    QPointF parameterValue2;
    QString error2;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue2, &error2), expectedResult);
    qDebug() << "TestConfigParameterLoader::testPoint: error string:" << error2;
    QCOMPARE(parameterValue2, expectedParameterValue2);
}

void TestConfigParameterLoader::testPoint_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QPoint>("expectedParameterValue1");
    QTest::addColumn<QPointF>("expectedParameterValue2");
    QTest::addColumn<bool>("expectedResult");

    // point with integers
    QTest::newRow("point (int): positive values")
            << QVariant::fromValue(QPoint(100, 200))
            << QPoint(100, 200)
            << QPointF(100.0, 200.0)
            << true;
    QTest::newRow("point (int): negative values")
            << QVariant::fromValue(QPoint(-100, -200))
            << QPoint(-100, -200)
            << QPointF(-100.0, -200.0)
            << true;
    QTest::newRow("point (int): null node value") << QVariant()
                                                  << QPoint()
                                                  << QPointF()
                                                  << false;
    QTest::newRow("point (int): null point") << QVariant::fromValue(QPoint())
                                             << QPoint(0, 0)
                                             << QPointF(0.0, 0.0)
                                             << true;

    // point with floating-point
    QTest::newRow("point (double): positive values")
            << QVariant::fromValue(QPoint(100.0, 200.0))
            << QPoint(100, 200)
            << QPointF(100.0, 200.0)
            << true;
    QTest::newRow("point (double): negitive values")
            << QVariant::fromValue(QPoint(-100.0, -200.0))
            << QPoint(-100, -200)
            << QPointF(-100.0, -200.0)
            << true;
    QTest::newRow("point (double): null node value") << QVariant()
                                                     << QPoint()
                                                     << QPointF()
                                                     << false;
    QTest::newRow("point (double): null point") << QVariant::fromValue(QPointF())
                                                << QPoint(0, 0)
                                                << QPointF(0.0, 0.0)
                                                << true;

    // map
    QTest::newRow("map: valid")
            << QVariant::fromValue(QVariantMap { {"x", 100}, {"y", 200} } )
            << QPoint(100, 200)
            << QPointF(100.0, 200.0)
            << true;
    QTest::newRow("map: empty") << QVariant::fromValue(QVariantMap())
                                << QPoint()
                                << QPointF()
                                << false;
    QTest::newRow("map: missing y")
            << QVariant::fromValue(QVariantMap { {"x", 100} } )
            << QPoint()
            << QPointF()
            << false;
    QTest::newRow("map: missing x")
            << QVariant::fromValue(QVariantMap { {"y", 200} } )
            << QPoint()
            << QPointF()
            << false;
    QTest::newRow("map: invalid keys")
            << QVariant::fromValue(QVariantMap { {"X", 100}, {"Y", 200} } )
            << QPoint()
            << QPointF()
            << false;
    QTest::newRow("map: too many members")
            << QVariant::fromValue(QVariantMap { {"x", 100}, {"y", 200}, {"asd", 0} } )
            << QPoint()
            << QPointF()
            << false;
    QTest::newRow("map: invalid x")
            << QVariant::fromValue(QVariantMap { {"x", "asd"}, {"y", 200} } )
            << QPoint()
            << QPointF()
            << false;
    QTest::newRow("map: invalid y")
            << QVariant::fromValue(QVariantMap { {"x", 100}, {"y", "asd"} } )
            << QPoint()
            << QPointF()
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(true)
                                   << QPoint()
                                   << QPointF()
                                   << false;
}

// Test: load line value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testLine()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QLine, expectedParameterValue1);
    QFETCH(QLineF, expectedParameterValue2);
    QFETCH(bool, expectedResult);

    QLine parameterValue1;
    QString error1;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue1, &error1), expectedResult);
    qDebug() << "TestConfigParameterLoader::testLine: error string:" << error1;
    QCOMPARE(parameterValue1, expectedParameterValue1);

    QLineF parameterValue2;
    QString error2;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue2, &error2), expectedResult);
    qDebug() << "TestConfigParameterLoader::testLine: error string:" << error2;
    QCOMPARE(parameterValue2, expectedParameterValue2);
}

void TestConfigParameterLoader::testLine_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QLine>("expectedParameterValue1");
    QTest::addColumn<QLineF>("expectedParameterValue2");
    QTest::addColumn<bool>("expectedResult");

    // line with integers
    QTest::newRow("line (int): positive values")
            << QVariant::fromValue(QLine(100, 200, 300, 400))
            << QLine(100, 200, 300, 400)
            << QLineF(100.0, 200.0, 300.0, 400.0)
            << true;
    QTest::newRow("line (int): negative values")
            << QVariant::fromValue(QLine(-100, -200, -300, -400))
            << QLine(-100, -200, -300, -400)
            << QLineF(-100.0, -200.0, -300, -400)
            << true;
    QTest::newRow("line (int): null node value") << QVariant()
                                                 << QLine()
                                                 << QLineF()
                                                 << false;
    QTest::newRow("line (int): null line") << QVariant::fromValue(QLine())
                                           << QLine(0, 0, 0, 0)
                                           << QLineF(0.0, 0.0, 0.0, 0.0)
                                           << true;

    // line with floating-line
    QTest::newRow("line (double): positive values")
            << QVariant::fromValue(QLine(100.0, 200.0, 300.0, 400.0))
            << QLine(100, 200, 300, 400)
            << QLineF(100.0, 200.0, 300.0, 400.0)
            << true;
    QTest::newRow("line (double): negitive values")
            << QVariant::fromValue(QLine(-100.0, -200.0, -300.0, -400.0))
            << QLine(-100, -200, -300, -400)
            << QLineF(-100.0, -200.0, -300.0, -400.0)
            << true;
    QTest::newRow("line (double): null node value") << QVariant()
                                                    << QLine()
                                                    << QLineF()
                                                    << false;
    QTest::newRow("line (double): null line") << QVariant::fromValue(QLineF())
                                              << QLine(0, 0, 0, 0)
                                              << QLineF(0.0, 0.0, 0.0, 0.0)
                                              << true;

    // map
    QTest::newRow("map: valid")
            << QVariant::fromValue(QVariantMap {
                                       {"x1", 100}, {"y1", 200}, {"x2", 300}, {"y2", 400} } )
            << QLine(100, 200, 300, 400)
            << QLineF(100.0, 200.0, 300.0, 400.0)
            << true;
    QTest::newRow("map: empty") << QVariant::fromValue(QVariantMap())
                                << QLine()
                                << QLineF()
                                << false;
    QTest::newRow("map: missing y")
            << QVariant::fromValue(QVariantMap { {"x1", 100}, {"x2", 300} } )
            << QLine()
            << QLineF()
            << false;
    QTest::newRow("map: missing x")
            << QVariant::fromValue(QVariantMap { {"y1", 200}, {"y2", 400} } )
            << QLine()
            << QLineF()
            << false;
    QTest::newRow("map: invalid keys")
            << QVariant::fromValue(QVariantMap {
                                       {"X1", 100}, {"Y1", 200}, {"X2", 300}, {"Y2", 400} } )
            << QLine()
            << QLineF()
            << false;
    QTest::newRow("map: too many members")
            << QVariant::fromValue(QVariantMap {
                                       {"x1", 100}, {"y1", 200},
                                       {"x2", 300}, {"y2", 400},
                                       {"asd", 0} } )
            << QLine()
            << QLineF()
            << false;
    QTest::newRow("map: invalid x1")
            << QVariant::fromValue(QVariantMap {
                                       {"x1", "asd"}, {"y1", 200}, {"x2", 300}, {"y2", 400} } )
            << QLine()
            << QLineF()
            << false;
    QTest::newRow("map: invalid y1")
            << QVariant::fromValue(QVariantMap {
                                       {"x1", 100}, {"y1", "asd"}, {"x2", 300}, {"y2", 400} } )
            << QLine()
            << QLineF()
            << false;
    QTest::newRow("map: invalid x2")
            << QVariant::fromValue(QVariantMap {
                                       {"x1", 100}, {"y1", 200}, {"x2", "asd"}, {"y2", 400} } )
            << QLine()
            << QLineF()
            << false;
    QTest::newRow("map: invalid y2")
            << QVariant::fromValue(QVariantMap {
                                       {"x1", 100}, {"y1", 200}, {"x2", 300}, {"y2", "asd"} } )
            << QLine()
            << QLineF()
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(true)
                                   << QLine()
                                   << QLineF()
                                   << false;
}

// Test: load rect value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testRect()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QRect, expectedParameterValue1);
    QFETCH(QRectF, expectedParameterValue2);
    QFETCH(bool, expectedResult);

    QRect parameterValue1;
    QString error1;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue1, &error1), expectedResult);
    qDebug() << "TestConfigParameterLoader::testRect: error string:" << error1;
    QCOMPARE(parameterValue1, expectedParameterValue1);

    QRectF parameterValue2;
    QString error2;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue2, &error2), expectedResult);
    qDebug() << "TestConfigParameterLoader::testRect: error string:" << error2;
    QCOMPARE(parameterValue2, expectedParameterValue2);
}

void TestConfigParameterLoader::testRect_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QRect>("expectedParameterValue1");
    QTest::addColumn<QRectF>("expectedParameterValue2");
    QTest::addColumn<bool>("expectedResult");

    // rect with integers
    QTest::newRow("rect (int): positive values")
            << QVariant::fromValue(QRect(100, 200, 300, 400))
            << QRect(100, 200, 300, 400)
            << QRectF(100.0, 200.0, 300.0, 400.0)
            << true;
    QTest::newRow("rect (int): negative values")
            << QVariant::fromValue(QRect(-100, -200, 300, 400))
            << QRect(-100, -200, 300, 400)
            << QRectF(-100.0, -200.0, 300, 400)
            << true;
    QTest::newRow("rect (int): invalid width")
            << QVariant::fromValue(QRect(-100, -200, -1, 400))
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("rect (int): invalid height")
            << QVariant::fromValue(QRect(-100, -200, 300, -1))
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("rect (int): null node value") << QVariant()
                                                 << QRect()
                                                 << QRectF()
                                                 << false;
    QTest::newRow("rect (int): null rect") << QVariant::fromValue(QRect())
                                           << QRect()
                                           << QRectF()
                                           << false;

    // rect with floating-rect
    QTest::newRow("rect (double): positive values")
            << QVariant::fromValue(QRect(100.0, 200.0, 300.0, 400.0))
            << QRect(100, 200, 300, 400)
            << QRectF(100.0, 200.0, 300.0, 400.0)
            << true;
    QTest::newRow("rect (double): negitive values")
            << QVariant::fromValue(QRect(-100.0, -200.0, 300.0, 400.0))
            << QRect(-100, -200, 300, 400)
            << QRectF(-100.0, -200.0, 300.0, 400.0)
            << true;
    QTest::newRow("rect (double): invalid width")
            << QVariant::fromValue(QRect(-100.0, -200.0, -1.0, 400.0))
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("rect (double): invalid height")
            << QVariant::fromValue(QRect(-100.0, -200.0, 300.0, -1.0))
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("rect (double): null node value") << QVariant()
                                                    << QRect()
                                                    << QRectF()
                                                    << false;
    QTest::newRow("rect (double): null rect") << QVariant::fromValue(QRectF())
                                              << QRect()
                                              << QRectF()
                                              << false;

    // map
    QTest::newRow("map: valid")
            << QVariant::fromValue(QVariantMap {
                                       {"x", 100}, {"y", 200}, {"width", 300}, {"height", 400} } )
            << QRect(100, 200, 300, 400)
            << QRectF(100.0, 200.0, 300.0, 400.0)
            << true;
    QTest::newRow("map: empty") << QVariant::fromValue(QVariantMap())
                                << QRect()
                                << QRectF()
                                << false;
    QTest::newRow("map: missing x")
            << QVariant::fromValue(QVariantMap { {"y", 200}, {"width", 300}, {"height", 400} } )
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("map: missing y")
            << QVariant::fromValue(QVariantMap { {"x", 100}, {"width", 300}, {"height", 400} } )
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("map: missing width")
            << QVariant::fromValue(QVariantMap { {"x", 100}, {"y", 200}, {"height", 400} } )
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("map: missing height")
            << QVariant::fromValue(QVariantMap { {"x", 100}, {"y", 200}, {"width", 300} } )
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("map: invalid keys")
            << QVariant::fromValue(QVariantMap {
                                       {"X", 100}, {"Y", 200}, {"WIDTH", 300}, {"HEIGHT", 400} } )
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("map: too many members")
            << QVariant::fromValue(QVariantMap {
                                       {"x", 100}, {"y", 200}, {"width", 300}, {"height", 400},
                                       {"asd", 0} } )
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("map: invalid x")
            << QVariant::fromValue(QVariantMap {
                                       {"x", "asd"}, {"y", 200}, {"width", 300}, {"height", 400} } )
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("map: invalid y")
            << QVariant::fromValue(QVariantMap {
                                       {"x", 100}, {"y", "asd"}, {"width", 300}, {"height", 400} } )
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("map: invalid width")
            << QVariant::fromValue(QVariantMap {
                                       {"x", 100}, {"y", 200}, {"width", "asd"}, {"height", 400} } )
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("map: invalid height")
            << QVariant::fromValue(QVariantMap {
                                       {"x", 100}, {"y", 200}, {"width", 300}, {"height", "asd"} } )
            << QRect()
            << QRectF()
            << false;
    QTest::newRow("map: invalid rect")
            << QVariant::fromValue(QVariantMap {
                                       {"x", 100}, {"y", 200}, {"width", -1}, {"height", -1} } )
            << QRect()
            << QRectF()
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(true)
                                   << QRect()
                                   << QRectF()
                                   << false;
}

// Test: load string list value --------------------------------------------------------------------

void TestConfigParameterLoader::testStringList()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(QStringList, expectedParameterValue);
    QFETCH(bool, expectedResult);

    QStringList parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testStringList: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testStringList_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<QStringList>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // string list
    QTest::newRow("string list: valid") << QVariant(QStringList { "a", "b", "c" } )
                                        << QStringList { "a", "b", "c" }
                                        << true;
    QTest::newRow("string list: empty")
            << QVariant::fromValue(QStringList()) << QStringList() << true;
    QTest::newRow("string list: null node value") << QVariant() << QStringList() << false;

    // int list
    QTest::newRow("int list: valid") << QVariant::fromValue(QList<int> { 1, 2, 3 } )
                                     << QStringList { "1", "2", "3" }
                                     << true;
    QTest::newRow("int list: empty") << QVariant::fromValue(QList<int>()) << QStringList() << true;

    // uint vector
    QTest::newRow("uint vector: valid") << QVariant::fromValue(QVector<uint32_t> { 1u, 2u, 3u } )
                                        << QStringList { "1", "2", "3" }
                                        << true;
    QTest::newRow("int list: empty") << QVariant::fromValue(QVector<uint32_t>())
                                     << QStringList()
                                     << true;

    // variant list
    QTest::newRow("variant list: valid") << QVariant(QVariantList { "a", "b", "c" } )
                                         << QStringList { "a", "b", "c" }
                                         << true;
    QTest::newRow("string list: empty") << QVariant(QVariantList()) << QStringList() << true;
    QTest::newRow("variant list: invalid") << QVariant(QVariantList { "a", 1, QPoint() } )
                                           << QStringList()
                                           << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(true) << QStringList() << false;
}

// Test: load pair value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testPair()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestPair, expectedParameterValue1);
    QFETCH(TestStdPair, expectedParameterValue2);
    QFETCH(bool, expectedResult);

    TestPair parameterValue1;
    QString error1;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue1, &error1), expectedResult);
    qDebug() << "TestConfigParameterLoader::testPair: error string:" << error1;
    QCOMPARE(parameterValue1, expectedParameterValue1);

    TestStdPair parameterValue2;
    QString error2;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue2, &error2), expectedResult);
    qDebug() << "TestConfigParameterLoader::testPair: error string:" << error2;
    QCOMPARE(parameterValue2, expectedParameterValue2);
}

void TestConfigParameterLoader::testPair_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestPair>("expectedParameterValue1");
    QTest::addColumn<TestStdPair>("expectedParameterValue2");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("valid")
            << QVariant::fromValue(QVariantMap { {"first", "test"}, {"second", 100} } )
            << TestPair("test", 100)
            << TestStdPair("test", 100)
            << true;
    QTest::newRow("empty") << QVariant::fromValue(QVariantMap())
                           << TestPair()
                           << TestStdPair()
                           << false;
    QTest::newRow("invalid first")
            << QVariant::fromValue(QVariantMap { {"first", QPoint()}, {"second", 100} } )
            << TestPair()
            << TestStdPair()
            << false;
    QTest::newRow("invalid second")
            << QVariant::fromValue(QVariantMap { {"first", "test"}, {"second", "asd"} } )
            << TestPair()
            << TestStdPair()
            << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(true)
                                   << TestPair()
                                   << TestStdPair()
                                   << false;
}

// Test: load list value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testList()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestList, expectedParameterValue);
    QFETCH(bool, expectedResult);

    TestList parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testList: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testList_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestList>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // string list
    QTest::newRow("string list: valid")
            << QVariant(QStringList { "2019-11-19", "2019-11-20", "2019-11-21" } )
            << TestList { QDate(2019, 11, 19), QDate(2019, 11, 20), QDate(2019, 11, 21) }
            << true;
    QTest::newRow("string list: empty") << QVariant(QStringList()) << TestList() << true;
    QTest::newRow("string list: invalid")
            << QVariant(QStringList { {"2019-13-19"}, {"2019-11-20"}, {"2019-11-21"} } )
            << TestList()
            << false;

    // date list
    QTest::newRow("date list: valid")
            << QVariant::fromValue(TestList {
                                       QDate(2019, 11, 19), QDate(2019, 11, 20), QDate(2019, 11, 21) } )
            << TestList { QDate(2019, 11, 19), QDate(2019, 11, 20), QDate(2019, 11, 21) }
            << true;
    QTest::newRow("date list: empty") << QVariant::fromValue(TestList()) << TestList() << true;
    QTest::newRow("date list: invalid")
            << QVariant::fromValue(
                   TestList { QDate(2019, 13, 19), QDate(2019, 11, 20), QDate(2019, 11, 21) } )
            << TestList()
            << false;

    // variant list
    QTest::newRow("variant list: valid")
            << QVariant(
                   QVariantList { QDate(2019, 11, 19), QDate(2019, 11, 20), QDate(2019, 11, 21) } )
            << TestList { QDate(2019, 11, 19), QDate(2019, 11, 20), QDate(2019, 11, 21) }
            << true;
    QTest::newRow("variant list: empty") << QVariant(QVariantList()) << TestList() << true;
    QTest::newRow("variant list: invalid")
            << QVariant(QVariantList { "asd", QDate(2019, 11, 20), QDate(2019, 11, 21) } )
            << TestList()
            << false;

    // null node
    QTest::newRow("null node value") << QVariant() << TestList() << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(1) << TestList() << false;
}

// Test: load std::list value ----------------------------------------------------------------------

void TestConfigParameterLoader::testStdList()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestStdList, expectedParameterValue);
    QFETCH(bool, expectedResult);

    TestStdList parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testStdList: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testStdList_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestStdList>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // string list
    QTest::newRow("string list: valid")
            << QVariant(QStringList { "12:34:56", "12:34:57", "12:34:58" } )
            << TestStdList { QTime(12, 34, 56), QTime(12, 34, 57), QTime(12, 34, 58) }
            << true;
    QTest::newRow("string list: invalid")
            << QVariant(QStringList { "24:34:56", "12:34:57", "12:34:58" } )
            << TestStdList()
            << false;
    QTest::newRow("string list: empty") << QVariant(QStringList()) << TestStdList() << true;

    // time list
    QTest::newRow("time list: valid")
            << QVariant::fromValue(
                   TestStdList { QTime(12, 34, 56), QTime(12, 34, 57), QTime(12, 34, 58) } )
            << TestStdList { QTime(12, 34, 56), QTime(12, 34, 57), QTime(12, 34, 58) }
            << true;
    QTest::newRow("time list: empty") << QVariant::fromValue(TestStdList())
                                      << TestStdList()
                                      << true;
    QTest::newRow("time list: invalid")
            << QVariant::fromValue(
                   TestStdList { QTime(24, 34, 56), QTime(12, 34, 57), QTime(12, 34, 58) } )
            << TestStdList()
            << false;

    // variant list
    QTest::newRow("variant list: valid")
            << QVariant(QVariantList { QTime(12, 34, 56), QTime(12, 34, 57), QTime(12, 34, 58) } )
            << TestStdList { QTime(12, 34, 56), QTime(12, 34, 57), QTime(12, 34, 58) }
            << true;
    QTest::newRow("variant list: empty") << QVariant(QVariantList()) << TestStdList() << true;
    QTest::newRow("variant list: invalid")
            << QVariant(QVariantList { "asd", QTime(12, 34, 57), QTime(12, 34, 58) } )
            << TestStdList()
            << false;

    // null node
    QTest::newRow("null node value") << QVariant() << TestStdList() << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(1) << TestStdList() << false;
}

// Test: load vector value -------------------------------------------------------------------------

void TestConfigParameterLoader::testVector()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestVector, expectedParameterValue);
    QFETCH(bool, expectedResult);

    TestVector parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testVector: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testVector_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestVector>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // string list
    QTest::newRow("string list: valid") << QVariant(QStringList { "1.2", "3.4", "5.6" } )
                                        << TestVector { 1.2f, 3.4f, 5.6f }
                                        << true;
    QTest::newRow("string list: empty") << QVariant(QStringList()) << TestVector() << true;
    QTest::newRow("string list: invalid") << QVariant(QStringList { "asd", "3.4", "5.6" } )
                                          << TestVector()
                                          << false;

    // float vector
    QTest::newRow("float vector: valid") << QVariant::fromValue(TestVector { 1.2f, 3.4f, 5.6f } )
                                         << TestVector { 1.2f, 3.4f, 5.6f }
                                         << true;
    QTest::newRow("float vector: empty") << QVariant::fromValue(TestVector())
                                         << TestVector()
                                         << true;

    // variant list
    QTest::newRow("variant list: valid") << QVariant(QVariantList { "1.2", 3.4f, 5.6 } )
                                         << TestVector { 1.2f, 3.4f, 5.6f }
                                         << true;
    QTest::newRow("variant list: empty") << QVariant(QVariantList()) << TestVector() << true;
    QTest::newRow("variant list: invalid") << QVariant(QVariantList { "asd", "3.4", "5.6" } )
                                           << TestVector()
                                           << false;

    // null node
    QTest::newRow("null node value") << QVariant() << TestVector() << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(1) << TestVector() << false;
}

// Test: load std::vector value --------------------------------------------------------------------

void TestConfigParameterLoader::testStdVector()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestStdVector, expectedParameterValue);
    QFETCH(bool, expectedResult);

    TestStdVector parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testStdVector: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testStdVector_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestStdVector>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // string list
    QTest::newRow("string list: valid") << QVariant(QStringList { "1.2", "3.4", "5.6" } )
                                        << TestStdVector { 1.2, 3.4, 5.6 }
                                        << true;
    QTest::newRow("string list: empty") << QVariant(QStringList()) << TestStdVector() << true;
    QTest::newRow("string list: invalid") << QVariant(QStringList { "asd", "3.4", "5.6" } )
                                          << TestStdVector()
                                          << false;

    // float vector
    QTest::newRow("float vector: valid") << QVariant::fromValue(TestStdVector { 1.2, 3.4, 5.6 } )
                                         << TestStdVector { 1.2, 3.4, 5.6 }
                                         << true;
    QTest::newRow("float vector: empty") << QVariant::fromValue(TestStdVector())
                                         << TestStdVector()
                                         << true;

    // variant list
    QTest::newRow("variant list: valid") << QVariant(QVariantList { "1.2", 3.4, 5.6 } )
                                         << TestStdVector { 1.2, 3.4, 5.6 }
                                         << true;
    QTest::newRow("variant list: empty") << QVariant(QVariantList()) << TestStdVector() << true;
    QTest::newRow("variant list: invalid") << QVariant(QVariantList { "asd", "3.4", "5.6" } )
                                           << TestStdVector()
                                           << false;

    // null node
    QTest::newRow("null node value") << QVariant() << TestStdVector() << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(1) << TestStdVector() << false;
}

// Test: load set value ----------------------------------------------------------------------------

void TestConfigParameterLoader::testSet()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestSet, expectedParameterValue);
    QFETCH(bool, expectedResult);

    TestSet parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testSet: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testSet_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestSet>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // string list
    QTest::newRow("string list: valid") << QVariant(QStringList { "1", "3", "5" } )
                                        << TestSet { 1, 3, 5 }
                                        << true;
    QTest::newRow("string list: empty") << QVariant(QStringList()) << TestSet() << true;
    QTest::newRow("string list: invalid") << QVariant(QStringList { "asd", "3", "5" } )
                                          << TestSet()
                                          << false;
    QTest::newRow("string list: duplicates") << QVariant(QStringList { "1", "3", "1", "5" } )
                                             << TestSet()
                                             << false;

    // int set
    QTest::newRow("int vector: valid") << QVariant::fromValue(TestSet { 1, 3, 5 } )
                                       << TestSet { 1, 3, 5 }
                                       << true;
    QTest::newRow("int vector: empty") << QVariant::fromValue(TestSet())
                                       << TestSet()
                                       << true;

    // variant list
    QTest::newRow("variant list: valid") << QVariant(QVariantList { "1", 3, 5 } )
                                         << TestSet { 1, 3, 5 }
                                         << true;
    QTest::newRow("variant list: empty") << QVariant(QVariantList()) << TestSet() << true;
    QTest::newRow("variant list: invalid") << QVariant(QVariantList { "asd", "3", "5" } )
                                           << TestSet()
                                           << false;
    QTest::newRow("variant list: duplicates") << QVariant(QVariantList { 1, 3, 5, 3 } )
                                              << TestSet()
                                              << false;

    // null node
    QTest::newRow("null node value") << QVariant() << TestSet() << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(1) << TestSet() << false;
}

// Test: load map value ----------------------------------------------------------------------------

void TestConfigParameterLoader::testMap()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestMap, expectedParameterValue);
    QFETCH(bool, expectedResult);

    TestMap parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testMap: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testMap_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestMap>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // map
    QTest::newRow("map: valid")
            << QVariant::fromValue(TestMap { {"a", 1}, {"b", 2}, {"c", 3} } )
            << TestMap { {"a", 1}, {"b", 2}, {"c", 3} }
            << true;
    QTest::newRow("map: empty") << QVariant::fromValue(TestMap()) << TestMap() << true;

    // variant map
    QTest::newRow("variant map: valid")
            << QVariant(QVariantMap { {"a", 1}, {"b", "2"}, {"c", 3u} } )
            << TestMap { {"a", 1}, {"b", 2}, {"c", 3} }
            << true;
    QTest::newRow("variant map: empty") << QVariant(QVariantMap()) << TestMap() << true;
    QTest::newRow("variant map: invalid key")
            << QVariant::fromValue(QMap<QPair<int, int>, int> { {{1, 2}, 3} } )
            << TestMap()
            << false;
    QTest::newRow("variant map: invalid value")
            << QVariant(QVariantMap { {"a", "a"}, {"b", 2}, {"c", 3} } )
            << TestMap()
            << false;

    // null node
    QTest::newRow("null node value") << QVariant() << TestMap() << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(1) << TestMap() << false;
}

// Test: load std::map value -----------------------------------------------------------------------

void TestConfigParameterLoader::testStdMap()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestStdMap, expectedParameterValue);
    QFETCH(bool, expectedResult);

    TestStdMap parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::TestStdMap: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testStdMap_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestStdMap>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // map
    QTest::newRow("map: valid")
            << QVariant::fromValue(TestStdMap { {"a", 1}, {"b", 2}, {"c", 3} } )
            << TestStdMap { {"a", 1}, {"b", 2}, {"c", 3} }
            << true;
    QTest::newRow("map: empty") << QVariant::fromValue(TestStdMap()) << TestStdMap() << true;

    // variant map
    QTest::newRow("variant map: valid")
            << QVariant(QVariantMap { {"a", 1}, {"b", "2"}, {"c", 3u} } )
            << TestStdMap { {"a", 1}, {"b", 2}, {"c", 3} }
            << true;
    QTest::newRow("variant map: empty") << QVariant(QVariantMap()) << TestStdMap() << true;
    QTest::newRow("variant map: invalid key")
            << QVariant::fromValue(QMap<QPair<int, int>, int> { {{1, 2}, 3} } )
            << TestStdMap()
            << false;
    QTest::newRow("variant map: invalid value")
            << QVariant(QVariantMap { {"a", "a"}, {"b", 2}, {"c", 3} } )
            << TestStdMap()
            << false;

    // null node
    QTest::newRow("null node value") << QVariant() << TestStdMap() << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(1) << TestStdMap() << false;
}

// Test: load hash value ---------------------------------------------------------------------------

void TestConfigParameterLoader::testHash()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestHash, expectedParameterValue);
    QFETCH(bool, expectedResult);

    TestHash parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testHash: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testHash_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestHash>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // hash
    QTest::newRow("hash: valid")
            << QVariant::fromValue(TestHash { {"a", 1}, {"b", 2}, {"c", 3} } )
            << TestHash { {"a", 1}, {"b", 2}, {"c", 3} }
            << true;
    QTest::newRow("hash: empty") << QVariant::fromValue(TestHash()) << TestHash() << true;

    // variant map
    QTest::newRow("variant map: valid")
            << QVariant(QVariantMap { {"a", 1}, {"b", "2"}, {"c", 3u} } )
            << TestHash { {"a", 1}, {"b", 2}, {"c", 3} }
            << true;
    QTest::newRow("variant map: empty") << QVariant(QVariantMap()) << TestHash() << true;
    QTest::newRow("variant map: invalid key")
            << QVariant::fromValue(QHash<QPair<int, int>, int> { {{1, 2}, 3} } )
            << TestHash()
            << false;
    QTest::newRow("variant map: invalid value")
            << QVariant(QVariantMap { {"a", "a"}, {"b", 2}, {"c", 3} } )
            << TestHash()
            << false;

    // null node
    QTest::newRow("null node value") << QVariant() << TestHash() << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(1) << TestHash() << false;
}

// Test: load std::unordered_map value -------------------------------------------------------------

void TestConfigParameterLoader::testStdHash()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestStdHash, expectedParameterValue);
    QFETCH(bool, expectedResult);

    TestStdHash parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::TestStdHash: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testStdHash_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestStdHash>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // variant map
    QTest::newRow("variant map: valid")
            << QVariant(QVariantMap { {"a", 1}, {"b", "2"}, {"c", 3u} } )
            << TestStdHash { {"a", 1}, {"b", 2}, {"c", 3} }
            << true;
    QTest::newRow("variant map: empty") << QVariant(QVariantMap()) << TestStdHash() << true;
    QTest::newRow("variant map: invalid key")
            << QVariant::fromValue(QHash<QPair<int, int>, int> { {{1, 2}, 3} } )
            << TestStdHash()
            << false;
    QTest::newRow("variant map: invalid value")
            << QVariant(QVariantMap { {"a", "a"}, {"b", 2}, {"c", 3} } )
            << TestStdHash()
            << false;

    // null node
    QTest::newRow("null node value") << QVariant() << TestStdHash() << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(1) << TestStdHash() << false;
}

// Test: load multi map value ----------------------------------------------------------------------

void TestConfigParameterLoader::testMultiMap()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestMultiMap, expectedParameterValue);
    QFETCH(bool, expectedResult);

    TestMultiMap parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testMultiMap: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testMultiMap_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestMultiMap>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // variant map
    QTest::newRow("variant map: valid")
            << QVariant(QVariantMap { {"a", QVariantList {1, 2, 3}},
                                      {"b", QVariantList {4, 5, 6}},
                                      {"c", QVariantList {7, 8, 9}}} )
            << TestMultiMap ({ {"a", 1}, {"a", 2}, {"a", 3},
                               {"b", 4}, {"b", 5}, {"b", 6},
                               {"c", 7}, {"c", 8}, {"c", 9} })
            << true;
    QTest::newRow("variant map: empty") << QVariant(QVariantMap()) << TestMultiMap() << true;
    QTest::newRow("variant map: invalid key")
            << QVariant::fromValue(QMap<QPair<int, int>, int> { {{1, 2}, 3} } )
            << TestMultiMap()
            << false;
    QTest::newRow("variant map: invalid value")
            << QVariant(QVariantMap { {"a", QVariantList {"a", 2, 3}},
                                      {"b", QVariantList {4, 5, 6}},
                                      {"c", QVariantList {7, 8, 9}}} )
            << TestMultiMap()
            << false;

    // null node
    QTest::newRow("null node value") << QVariant() << TestMultiMap() << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(1) << TestMultiMap() << false;
}

// Test: load multi hash value ---------------------------------------------------------------------

void TestConfigParameterLoader::testMultiHash()
{
    QFETCH(QVariant, nodeValue);
    QFETCH(TestMultiHash, expectedParameterValue);
    QFETCH(bool, expectedResult);

    TestMultiHash parameterValue;
    QString error;
    QCOMPARE(ConfigParameterLoader::load(nodeValue, &parameterValue, &error), expectedResult);
    qDebug() << "TestConfigParameterLoader::testMultiHash: error string:" << error;
    QCOMPARE(parameterValue, expectedParameterValue);
}

void TestConfigParameterLoader::testMultiHash_data()
{
    QTest::addColumn<QVariant>("nodeValue");
    QTest::addColumn<TestMultiHash>("expectedParameterValue");
    QTest::addColumn<bool>("expectedResult");

    // variant map
    QTest::newRow("variant map: valid")
            << QVariant(QVariantMap { {"a", QVariantList {1, 2, 3}},
                                      {"b", QVariantList {4, 5, 6}},
                                      {"c", QVariantList {7, 8, 9}}} )
            << TestMultiHash ({ {"a", 1}, {"a", 2}, {"a", 3},
                                {"b", 4}, {"b", 5}, {"b", 6},
                                {"c", 7}, {"c", 8}, {"c", 9} })
            << true;
    QTest::newRow("variant map: empty") << QVariant(QVariantMap()) << TestMultiHash() << true;
    QTest::newRow("variant map: invalid key")
            << QVariant::fromValue(QMap<QPair<int, int>, int> { {{1, 2}, 3} } )
            << TestMultiHash()
            << false;
    QTest::newRow("variant map: invalid value")
            << QVariant(QVariantMap { {"a", QVariantList {"a", 2, 3}},
                                      {"b", QVariantList {4, 5, 6}},
                                      {"c", QVariantList {7, 8, 9}}} )
            << TestMultiHash()
            << false;

    // null node
    QTest::newRow("null node value") << QVariant() << TestMultiHash() << false;

    // Invalid type
    QTest::newRow("Invalid: type") << QVariant::fromValue(1) << TestMultiHash() << false;
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigParameterLoader)
#include "testConfigParameterLoader.moc"
