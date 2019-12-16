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
 * Contains unit tests for EnvironmentVariables class
 */

// C++ Config Framework includes
#include <CppConfigFramework/EnvironmentVariables.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtCore/QProcessEnvironment>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;

class TestEnvironmentVariables : public QObject
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
    void testLoadFromProcess();

    void testValue();
    void testValue_data();

    void testExpandText();
    void testExpandText_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestEnvironmentVariables::initTestCase()
{
}

void TestEnvironmentVariables::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestEnvironmentVariables::init()
{
}

void TestEnvironmentVariables::cleanup()
{
}

// Test: loadFromProcess() method ------------------------------------------------------------------

void TestEnvironmentVariables::testLoadFromProcess()
{
    // Add a test environment variable to the process
    qputenv("TEST_VAR1", "test1");
    QProcessEnvironment systemEnvironment = QProcessEnvironment::systemEnvironment();
    QCOMPARE(systemEnvironment.value("TEST_VAR1"), "test1");

    // Load environment variables from the process
    auto environmentVariables = EnvironmentVariables::loadFromProcess();

    // Check if all variables from the process were loaded
    auto environmentVariablesNames = environmentVariables.names();
    auto systemEnvironmentKeys = systemEnvironment.keys();

    QCOMPARE(QSet<QString>(environmentVariablesNames.begin(), environmentVariablesNames.end()),
             QSet<QString>(systemEnvironmentKeys.begin(), systemEnvironmentKeys.end()));

    for (const QString &name : environmentVariables.names())
    {
        QCOMPARE(environmentVariables.value(name), systemEnvironment.value(name));
    }

    // Add another environment variable to the process and make sure it is not also set in the
    // already loaded instance
    qputenv("TEST_VAR2", "test2");
    QCOMPARE(QProcessEnvironment::systemEnvironment().value("TEST_VAR2"), "test2");

    QVERIFY(!environmentVariables.contains("TEST_VAR2"));
}

// Test: value methods -----------------------------------------------------------------------------

void TestEnvironmentVariables::testValue()
{
    QFETCH(QString, name);
    QFETCH(QString, value);

    // Make sure that an empty instance initially does not contain the variable
    EnvironmentVariables environmentVariables;
    QVERIFY(!environmentVariables.contains(name));
    QCOMPARE(environmentVariables.value(name), QString());

    // Set the variable and check if it now contains the variable
    environmentVariables.setValue(name, value);
    QVERIFY(environmentVariables.contains(name));
    QCOMPARE(environmentVariables.value(name), value);
}

void TestEnvironmentVariables::testValue_data()
{
    QTest::addColumn<QString>("name");
    QTest::addColumn<QString>("value");

    QTest::newRow("") << "asd" << "123";
    QTest::newRow("") << "a1" << "asd";
    QTest::newRow("") << "AAA_BBB_123" << "xyz";
}

// Test: expandText() method -----------------------------------------------------------------------

void TestEnvironmentVariables::testExpandText()
{
    QFETCH(QString, text);
    QFETCH(QString, expected);

    EnvironmentVariables environmentVariables;
    environmentVariables.setValue("TEST1", "value");
    environmentVariables.setValue("TEST2", "${TEST1}");
    environmentVariables.setValue("TEST_LOOP1", "${TEST_LOOP2}");
    environmentVariables.setValue("TEST_LOOP2", "${TEST_LOOP1}");

    QCOMPARE(environmentVariables.expandText(text), expected);
}

void TestEnvironmentVariables::testExpandText_data()
{
    QTest::addColumn<QString>("text");
    QTest::addColumn<QString>("expected");

    QTest::newRow("no vars") << "test1" << "test1";
    QTest::newRow("var single ref") << "test2 ${TEST1}" << "test2 value";
    QTest::newRow("var double ref") << "test3 ${TEST2}" << "test3 value";
    QTest::newRow("loop") << "${TEST_LOOP1}" << QString();
    QTest::newRow("non-existent var") << "${TEST_VAR_DOES_NOT_EXIST}" << QString();
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestEnvironmentVariables)
#include "testEnvironmentVariables.moc"
