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
 * Contains unit tests for ConfigLoader class
 */

// C++ Config Framework includes
#include <CppConfigFramework/ConfigReader.hpp>
#include "TestClasses.hpp"

// Qt includes
#include <QtCore/QDebug>
#include <QtCore/QLine>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;

class TestConfigLoader : public QObject
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
    void testConfigParameterRequirement();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestConfigLoader::initTestCase()
{
}

void TestConfigLoader::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestConfigLoader::init()
{
}

void TestConfigLoader::cleanup()
{
}

// Test: load a config to a config class -----------------------------------------------------------

void TestConfigLoader::testConfigParameterRequirement()
{
    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/TestConfigParameterRequirement.json"));
    ConfigReader configReader;

    auto config = configReader.read(configFilePath);
    QVERIFY(config);

    // Load config with "param" parameter
    {
        TestRequiredConfigParameter required;
        TestOptionalConfigParameter optional;

        required.param = false;
        optional.param = false;

        QCOMPARE(required.loadConfig("configWithParam", *config), true);
        QCOMPARE(optional.loadConfig("configWithParam", *config), true);

        QCOMPARE(required.param, true);
        QCOMPARE(optional.param, true);
    }

    // Load config without "param" parameter
    {
        TestRequiredConfigParameter required;
        TestOptionalConfigParameter optional;

        required.param = false;
        optional.param = false;

        QCOMPARE(required.loadConfig("configWithoutParam", *config), false);
        QCOMPARE(optional.loadConfig("configWithoutParam", *config), true);

        QCOMPARE(required.param, false);
        QCOMPARE(optional.param, false);

        // Make sure that the values stay unchaged
        required.param = true;
        optional.param = true;

        QCOMPARE(required.loadConfig("configWithoutParam", *config), false);
        QCOMPARE(optional.loadConfig("configWithoutParam", *config), true);

        QCOMPARE(required.param, true);
        QCOMPARE(optional.param, true);
    }
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigLoader)
#include "testConfigLoader.moc"
