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
 * Contains unit tests for ConfigParameterValidator class
 */

// C++ Config Framework includes
#include <CppConfigFramework/ConfigParameterValidator.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;

class TestConfigParameterValidator : public QObject
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
    void testConfigParameterDefaultValidator();
    void testConfigParameterDefaultValidator_data();

    void testConfigParameterRangeValidator();
    void testConfigParameterRangeValidator_data();

    void testConfigParameterListValidator();
    void testConfigParameterListValidator_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestConfigParameterValidator::initTestCase()
{
}

void TestConfigParameterValidator::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestConfigParameterValidator::init()
{
}

void TestConfigParameterValidator::cleanup()
{
}

// Test: ConfigParameterValidator ------------------------------------------------------------------

void TestConfigParameterValidator::testConfigParameterDefaultValidator()
{
    QFETCH(int, parameterValue);

    // Validates everything as true
    ConfigParameterDefaultValidator<int> validator;

    QString error;
    QVERIFY(validator(parameterValue, &error));
}

void TestConfigParameterValidator::testConfigParameterDefaultValidator_data()
{
    QTest::addColumn<int>("parameterValue");

    QTest::newRow("min") << std::numeric_limits<int>::min();
    QTest::newRow("0") << 0;
    QTest::newRow("max") << std::numeric_limits<int>::max();
}

// Test: ConfigParameterRangeValidator -------------------------------------------------------------

void TestConfigParameterValidator::testConfigParameterRangeValidator()
{
    QFETCH(int, parameterValue);
    QFETCH(bool, expectedResult);

    // Validates values in range
    ConfigParameterRangeValidator<int> validator(-1000, 1000);

    QString error;
    QCOMPARE(validator(parameterValue, &error), expectedResult);
}

void TestConfigParameterValidator::testConfigParameterRangeValidator_data()
{
    QTest::addColumn<int>("parameterValue");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("-1000") << -1000 << true;
    QTest::newRow("-100") << -100 << true;
    QTest::newRow("0") << 0 << true;
    QTest::newRow("100") << 100 << true;
    QTest::newRow("1000") << 1000 << true;

    QTest::newRow("min") << std::numeric_limits<int>::min() << false;
    QTest::newRow("-1001") << -1001 << false;
    QTest::newRow("1001") << 1001 << false;
    QTest::newRow("max") << std::numeric_limits<int>::max() << false;
}

// Test: ConfigParameterListValidator --------------------------------------------------------------

void TestConfigParameterValidator::testConfigParameterListValidator()
{
    QFETCH(QString, parameterValue);
    QFETCH(bool, expectedResult);

    // Validates all from the list
    ConfigParameterListValidator<QString> validator({ "a", "b", "c" });

    QString error;
    QCOMPARE(validator(parameterValue, &error), expectedResult);
}

void TestConfigParameterValidator::testConfigParameterListValidator_data()
{
    QTest::addColumn<QString>("parameterValue");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("a") << "a" << true;
    QTest::newRow("b") << "b" << true;
    QTest::newRow("c") << "c" << true;

    QTest::newRow("A") << "A" << false;
    QTest::newRow("d") << "d" << false;
    QTest::newRow("empty") << "" << false;
    QTest::newRow("default") << QString() << false;
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigParameterValidator)
#include "testConfigParameterValidator.moc"
