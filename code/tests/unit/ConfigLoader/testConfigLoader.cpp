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
#include <CppConfigFramework/ConfigLoader.hpp>
#include <CppConfigFramework/ConfigReader.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtCore/QLine>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test config classes -----------------------------------------------------------------------------

using namespace CppConfigFramework;

class TestRequiredConfigParameter : public ConfigLoader
{
public:
    int param = 0;

private:
    bool loadConfigParameters(const ConfigObjectNode &config, QString *error) override
    {
        return loadRequiredConfigParameter(&param,
                                           "param",
                                           config,
                                           ConfigParameterRangeValidator<int>(-50, 50),
                                           error);
    }

    QString validateConfig() const override
    {
        if (param > 20)
        {
            return "validateConfig error";
        }

        return QString();
    }
};

class TestOptionalConfigParameter : public ConfigLoader
{
public:
    int param = 0;

private:
    bool loadConfigParameters(const ConfigObjectNode &config, QString *error) override
    {
        bool loaded = false;
        return loadOptionalConfigParameter(&param,
                                           "param",
                                           config,
                                           ConfigParameterRangeValidator<int>(-50, 50),
                                           &loaded,
                                           error);
    }
};

// Test class declaration --------------------------------------------------------------------------

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
    void testLoadConfigAtPath();
    void testLoadConfigAtPath_data();

    void testLoadOptionalConfigAtPath();
    void testLoadOptionalConfigAtPath_data();

    void testLoadConfig();
    void testLoadConfig_data();

    void testLoadOptionalConfig();
    void testLoadOptionalConfig_data();

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

// Test: loadConfigAtPath() method -----------------------------------------------------------------

void TestConfigLoader::testLoadConfigAtPath()
{
    QFETCH(QString, path);
    QFETCH(bool, expectedResult);
    QFETCH(int, expectedResultValue);

    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/TestLoadConfigAtPath.json"));
    ConfigReader configReader;

    auto config = configReader.read(configFilePath);
    QVERIFY(config);

    TestRequiredConfigParameter configStructure;
    QString error;
    QCOMPARE(configStructure.loadConfigAtPath(path, *config, &error), expectedResult);
    qDebug() << "TestConfigLoader::testLoadConfigAtPath: error string:" << error;

    QCOMPARE(configStructure.param, expectedResultValue);
}

void TestConfigLoader::testLoadConfigAtPath_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<int>("expectedResultValue");

    QTest::newRow("Absolute path exists") << "/level1/level2/actualConfig" << true << 1;
    QTest::newRow("Relative path exists") << "level1/level2/actualConfig" << true << 1;

    QTest::newRow("Absolute path: missing") << "/level1/level2/missingConfig" << false << 0;
    QTest::newRow("Relative path: missing") << "level1/level2/missingConfig" << false << 0;

    QTest::newRow("Absolute path: invalid") << "/level1/level2/123" << false << 0;
    QTest::newRow("Relative path: invalid") << "level1/level2/123" << false << 0;

    QTest::newRow("Invalid config node") << "/level1/level2/invalidConfigNode" << false << 0;
    QTest::newRow("Invalid config parameter value range")
            << "/level1/level2/configWithInvalidParamValueRange" << false << -100;
}

// Test: loadOptionalConfigAtPath() method ---------------------------------------------------------

void TestConfigLoader::testLoadOptionalConfigAtPath()
{
    QFETCH(QString, path);
    QFETCH(bool, expectedResult);
    QFETCH(bool, expectedResultLoaded);
    QFETCH(int, expectedResultValue);

    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/TestLoadConfigAtPath.json"));
    ConfigReader configReader;

    auto config = configReader.read(configFilePath);
    QVERIFY(config);

    TestRequiredConfigParameter configStructure;
    bool loaded = false;
    QString error;
    QCOMPARE(configStructure.loadOptionalConfigAtPath(path, *config, &loaded, &error),
             expectedResult);
    qDebug() << "TestConfigLoader::testLoadOptionalConfigAtPath: error string:" << error;

    QCOMPARE(loaded, expectedResultLoaded);
    QCOMPARE(configStructure.param, expectedResultValue);
}

void TestConfigLoader::testLoadOptionalConfigAtPath_data()
{
    QTest::addColumn<QString>("path");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<bool>("expectedResultLoaded");
    QTest::addColumn<int>("expectedResultValue");

    QTest::newRow("Absolute path: exists") << "/level1/level2/actualConfig" << true << true << 1;
    QTest::newRow("Relative path: exists") << "level1/level2/actualConfig" << true << true << 1;

    QTest::newRow("Absolute path: missing") << "/level1/level2/missingConfig" << true << false << 0;
    QTest::newRow("Relative path: missing") << "level1/level2/missingConfig" << true << false << 0;

    QTest::newRow("Absolute path: invalid") << "/level1/level2/123" << false << false << 0;
    QTest::newRow("Relative path: invalid") << "level1/level2/123" << false << false << 0;

    QTest::newRow("Invalid config node")
            << "/level1/level2/invalidConfigNode" << false << false << 0;
    QTest::newRow("Invalid config parameter value range")
            << "/level1/level2/configWithInvalidParamValueRange" << false << false << -100;
}

// Test: loadConfig() method -----------------------------------------------------------------------

void TestConfigLoader::testLoadConfig()
{
    QFETCH(QString, parameterName);
    QFETCH(bool, expectedResult);
    QFETCH(int, expectedResultValue);

    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/TestLoadConfig.json"));
    ConfigReader configReader;

    auto config = configReader.read(configFilePath);
    QVERIFY(config);

    TestRequiredConfigParameter configStructure;
    QString error;
    QCOMPARE(configStructure.loadConfig(parameterName, *config, &error), expectedResult);
    qDebug() << "TestConfigLoader::testLoadConfig: error string:" << error;

    QCOMPARE(configStructure.param, expectedResultValue);
}

void TestConfigLoader::testLoadConfig_data()
{
    QTest::addColumn<QString>("parameterName");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<int>("expectedResultValue");

    QTest::newRow("Config exists") << "actualConfig" << true << 1;
    QTest::newRow("Missing config") << "missingConfig"  << false << 0;
    QTest::newRow("Invalid config") << "123" << false << 0;
    QTest::newRow("Invalid config node") << "invalidConfigNode" << false << 0;
    QTest::newRow("Invalid config parameter value range")
            << "configWithInvalidParamValueRange" << false << -100;
    QTest::newRow("Invalid config parameter value")
            << "configWithInvalidParamValue" << false << 30;
}

// Test: loadOptionalConfig() method ---------------------------------------------------------------

void TestConfigLoader::testLoadOptionalConfig()
{
    QFETCH(QString, parameterName);
    QFETCH(bool, expectedResult);
    QFETCH(bool, expectedResultLoaded);
    QFETCH(int, expectedResultValue);

    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/TestLoadConfig.json"));
    ConfigReader configReader;

    auto config = configReader.read(configFilePath);
    QVERIFY(config);

    TestRequiredConfigParameter configStructure;
    bool loaded = false;
    QString error;
    QCOMPARE(configStructure.loadOptionalConfig(parameterName, *config, &loaded, &error),
             expectedResult);
    qDebug() << "TestConfigLoader::testLoadOptionalConfig: error string:" << error;

    QCOMPARE(loaded, expectedResultLoaded);
    QCOMPARE(configStructure.param, expectedResultValue);
}

void TestConfigLoader::testLoadOptionalConfig_data()
{
    QTest::addColumn<QString>("parameterName");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<bool>("expectedResultLoaded");
    QTest::addColumn<int>("expectedResultValue");

    QTest::newRow("Config exists") << "actualConfig" << true << true << 1;
    QTest::newRow("Missing config") << "missingConfig" << true << false << 0;
    QTest::newRow("Invalid config") << "123" << false << false << 0;
    QTest::newRow("Invalid config node") << "invalidConfigNode" << false << false << 0;
    QTest::newRow("Invalid config parameter value range")
            << "configWithInvalidParamValueRange" << false << false << -100;
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

        QCOMPARE(required.loadConfig("configWithParam", *config), true);
        QCOMPARE(optional.loadConfig("configWithParam", *config), true);

        QCOMPARE(required.param, 1);
        QCOMPARE(optional.param, 1);
    }

    // Load config without "param" parameter
    {
        TestRequiredConfigParameter required;
        TestOptionalConfigParameter optional;

        QCOMPARE(required.loadConfig("configWithoutParam", *config), false);
        QCOMPARE(optional.loadConfig("configWithoutParam", *config), true);

        QCOMPARE(required.param, 0);
        QCOMPARE(optional.param, 0);

        // Make sure that the values stay unchaged
        required.param = 11;
        optional.param = 11;

        QCOMPARE(required.loadConfig("configWithoutParam", *config), false);
        QCOMPARE(optional.loadConfig("configWithoutParam", *config), true);

        QCOMPARE(required.param, 11);
        QCOMPARE(optional.param, 11);
    }
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigLoader)
#include "testConfigLoader.moc"
