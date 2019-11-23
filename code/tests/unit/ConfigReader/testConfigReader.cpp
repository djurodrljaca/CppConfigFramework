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
 * Contains unit tests for ConfigReader class
 */

// C++ Config Framework includes
#include <CppConfigFramework/ConfigObjectNode.hpp>
#include <CppConfigFramework/ConfigReader.hpp>
#include <CppConfigFramework/ConfigValueNode.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;

class TestConfigReader : public QObject
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
    void testReadValidConfig();
    void testReadConfigWithNodeReference();
    void testReadConfigWithDerivedObject();
    void testReadConfigWithIncludes();
    void testReadConfigWithIncludesAndEnv();
    void testReadConfigWithOnlyIncludes();
    void testReadConfigWithExternalConfigReferences();
    void testReadInvalidPathParameters();
    void testReadInvalidPathParameters_data();
    void testReadInvalidExternalConfigsParameter();
    void testReadInvalidConfigFile();
    void testReadInvalidConfigFile_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestConfigReader::initTestCase()
{
}

void TestConfigReader::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestConfigReader::init()
{
}

void TestConfigReader::cleanup()
{
}

// Test: read a valid config file ------------------------------------------------------------------

void TestConfigReader::testReadValidConfig()
{
    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/ValidConfig.json"));
    auto environmentVariables = EnvironmentVariables::loadFromProcess();
    ConfigReader configReader;
    QString error;

    auto config = configReader.read(configFilePath,
                                    QDir::current(),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables,
                                    &error);
    QVERIFY(config);

    // Check config configs
    QVERIFY(config->isRoot());
    QCOMPARE(config->count(), 1);

    // Check "/root_node"
    {
        QVERIFY(config->contains("root_node"));
        const auto *rootNode = config->member("root_node");
        QVERIFY(rootNode->isObject());
        QCOMPARE(rootNode->toObject().count(), 1);

        // Check "/root_node/sub_node"
        {
            QVERIFY(rootNode->toObject().contains("sub_node"));
            const auto *subNode = rootNode->toObject().member("sub_node");
            QVERIFY(subNode->isObject());
            QCOMPARE(subNode->toObject().count(), 2);

            // Check "/root_node/sub_node/sub_node1"
            {
                QVERIFY(subNode->toObject().contains("sub_node1"));
                const auto *subNode1 = subNode->toObject().member("sub_node1");
                QVERIFY(subNode1->isObject());
                QCOMPARE(subNode1->toObject().count(), 2);

                // Check "/root_node/sub_node/sub_node1/bool_param"
                QVERIFY(subNode1->toObject().contains("bool_param"));
                const auto *boolParam = subNode1->toObject().member("bool_param");
                QVERIFY(boolParam->isValue());
                QCOMPARE(boolParam->toValue().value(), true);

                // Check "/root_node/sub_node/sub_node1/array_param"
                QVERIFY(subNode1->toObject().contains("array_param"));
                const auto *arrayParam = subNode1->toObject().member("array_param");
                QVERIFY(arrayParam->isValue());
                QCOMPARE(arrayParam->toValue().value(), QVariantList({5, 6, 7}));
            }

            // Check "/root_node/sub_node/sub_node2"
            {
                QVERIFY(subNode->toObject().contains("sub_node2"));
                const auto *subNode2 = subNode->toObject().member("sub_node2");
                QVERIFY(subNode2->isObject());
                QCOMPARE(subNode2->toObject().count(), 1);

                // Check "/root_node/sub_node/sub_node2/int_param"
                QVERIFY(subNode2->toObject().contains("int_param"));
                const auto *intParam = subNode2->toObject().member("int_param");
                QVERIFY(intParam->isValue());
                QCOMPARE(intParam->toValue().value(), 1);
            }
        }
    }
}

// Test: read a config file with node references ---------------------------------------------------

void TestConfigReader::testReadConfigWithNodeReference()
{
    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/ConfigWithNodeReferences.json"));
    auto environmentVariables = EnvironmentVariables::loadFromProcess();
    ConfigReader configReader;
    QString error;

    auto config = configReader.read(configFilePath,
                                    QDir::current(),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables,
                                    &error);
    QVERIFY(config);

    // Check config configs
    QVERIFY(config->isRoot());
    QCOMPARE(config->count(), 2);

    // Check "/root_node2/ref_value1
    {
        // References "/root_node1/value" node
        const auto *ref_value1 = config->nodeAtPath("/root_node2/ref_value1");
        QVERIFY(ref_value1 != nullptr);
        QVERIFY(ref_value1->isValue());
        QCOMPARE(ref_value1->toValue().value(), 1);
    }

    // Check "/root_node2/ref_value2
    {
        // References "/root_node1/sub_node" node
        const auto *ref_value2 = config->nodeAtPath("/root_node2/ref_value2");
        QVERIFY(ref_value2 != nullptr);
        QVERIFY(ref_value2->isObject());

        // Check "/root_node1/sub_node/value"
        {
            QVERIFY(ref_value2->toObject().contains("value"));
            const auto *value = ref_value2->toObject().member("value");
            QVERIFY(value->isValue());
            QCOMPARE(value->toValue().value(), QVariant("str"));
        }
    }

    // Check "/root_node2/ref_value3
    {
        // References "/root_node2/sub_node/value" node
        const auto *ref_value3 = config->nodeAtPath("/root_node2/ref_value3");
        QVERIFY(ref_value3 != nullptr);
        QVERIFY(ref_value3->isValue());
        QCOMPARE(ref_value3->toValue().value(), QVariantList({1, 2, 3}));
    }
}

// Test: read a config file with derived objects ---------------------------------------------------

void TestConfigReader::testReadConfigWithDerivedObject()
{
    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/ConfigWithDerivedObjects.json"));
    auto environmentVariables = EnvironmentVariables::loadFromProcess();
    ConfigReader configReader;
    QString error;

    auto config = configReader.read(configFilePath,
                                    QDir::current(),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables,
                                    &error);
    QVERIFY(config);

    // Check "/derived_object1"
    {
        const auto *derived_object1 = config->nodeAtPath("/derived_object1");
        QVERIFY(derived_object1 != nullptr);
        QVERIFY(derived_object1->isObject());
        QCOMPARE(derived_object1->toObject().count(), 2);

        // Check "/derived_object1/value
        {
            QVERIFY(derived_object1->toObject().contains("value"));
            const auto *value = derived_object1->toObject().member("value");
            QVERIFY(value->isValue());
            QCOMPARE(value->toValue().value(), 1);
        }

        // Check "/derived_object1/sub_node
        {
            QVERIFY(derived_object1->toObject().contains("sub_node"));
            const auto *subNode = derived_object1->toObject().member("sub_node");
            QVERIFY(subNode->isObject());

            // Check "/derived_object1/sub_node/value"
            {
                QVERIFY(subNode->toObject().contains("value"));
                const auto *value = subNode->toObject().member("value");
                QVERIFY(value->isValue());
                QCOMPARE(value->toValue().value(), QVariant("str"));
            }
        }
    }

    // Check "/derived_object2"
    {
        const auto *derived_object2 = config->nodeAtPath("/derived_object2");
        QVERIFY(derived_object2 != nullptr);
        QVERIFY(derived_object2->isObject());
        QCOMPARE(derived_object2->toObject().count(), 2);

        // Check "/derived_object2/value
        {
            QVERIFY(derived_object2->toObject().contains("value"));
            const auto *value = derived_object2->toObject().member("value");
            QVERIFY(value->isValue());
            QCOMPARE(value->toValue().value(), QVariantList({1, 2, 3}));
        }

        // Check "/derived_object2/sub_node
        {
            QVERIFY(derived_object2->toObject().contains("sub_node"));
            const auto *subNode = derived_object2->toObject().member("sub_node");
            QVERIFY(subNode->isObject());

            // Check "/derived_object2/sub_node/value"
            {
                QVERIFY(subNode->toObject().contains("value"));
                const auto *value = subNode->toObject().member("value");
                QVERIFY(value->isValue());
                QCOMPARE(value->toValue().value(), QVariant("abc"));
            }
        }
    }

    // Check "/derived_object3"
    {
        const auto *derived_object3 = config->nodeAtPath("/derived_object3");
        QVERIFY(derived_object3 != nullptr);
        QVERIFY(derived_object3->isObject());
        QCOMPARE(derived_object3->toObject().count(), 2);

        // Check "/derived_object3/value
        {
            QVERIFY(derived_object3->toObject().contains("value"));
            const auto *value = derived_object3->toObject().member("value");
            QVERIFY(value->isValue());
            QCOMPARE(value->toValue().value(), 1);
        }

        // Check "/derived_object3/sub_node
        {
            QVERIFY(derived_object3->toObject().contains("sub_node"));
            const auto *subNode = derived_object3->toObject().member("sub_node");
            QVERIFY(subNode->isObject());

            // Check "/derived_object3/sub_node/value"
            {
                QVERIFY(subNode->toObject().contains("value"));
                const auto *value = subNode->toObject().member("value");
                QVERIFY(value->isValue());
                QCOMPARE(value->toValue().value(), QVariant("abc"));
            }
        }
    }
}

// Test: read a config file with includes ----------------------------------------------------------

void TestConfigReader::testReadConfigWithIncludes()
{
    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/ConfigWithIncludes.json"));
    auto environmentVariables = EnvironmentVariables::loadFromProcess();
    ConfigReader configReader;
    QString error;

    auto config = configReader.read(configFilePath,
                                    QDir::current(),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables,
                                    &error);
    QVERIFY(config);
    QVERIFY(config->isObject());
    QCOMPARE(config->count(), 3);

    // Check "/included_config1"
    {
        const auto *included_config1 = config->nodeAtPath("/included_config1");
        QVERIFY(included_config1 != nullptr);
        QVERIFY(included_config1->isObject());
        QCOMPARE(included_config1->toObject().count(), 2);

        // Check "/included_config1/value
        {
            QVERIFY(included_config1->toObject().contains("value"));
            const auto *value = included_config1->toObject().member("value");
            QVERIFY(value->isValue());
            QCOMPARE(value->toValue().value(), 99);
        }

        // Check "/included_config1/new_item
        {
            QVERIFY(included_config1->toObject().contains("new_item"));
            const auto *new_item = included_config1->toObject().member("new_item");
            QVERIFY(new_item->isValue());
            QCOMPARE(new_item->toValue().value(), QVariant("str"));
        }
    }

    // Check "/included_config2_subnode"
    {
        const auto *included_config2_subnode = config->nodeAtPath("/included_config2_subnode");
        QVERIFY(included_config2_subnode != nullptr);
        QVERIFY(included_config2_subnode->isObject());
        QCOMPARE(included_config2_subnode->toObject().count(), 2);

        // Check "/included_config2_subnode/value
        {
            QVERIFY(included_config2_subnode->toObject().contains("value"));
            const auto *value = included_config2_subnode->toObject().member("value");
            QVERIFY(value->isValue());
            QCOMPARE(value->toValue().value(), 2);
        }

        // Check "/included_config2_subnode/included_value1
        {
            QVERIFY(included_config2_subnode->toObject().contains("included_value1"));
            const auto *included_value1 =
                    included_config2_subnode->toObject().member("included_value1");
            QVERIFY(included_value1->isValue());
            QCOMPARE(included_value1->toValue().value(), 1);
        }
    }

    // Check "/included_value3"
    {
        const auto *included_config3 = config->nodeAtPath("/included_value3");
        QVERIFY(included_config3 != nullptr);
        QVERIFY(included_config3->isObject());
        QCOMPARE(included_config3->toObject().count(), 1);

        // Check "/included_value3/sub_node"
        {
            const auto *sub_node = included_config3->nodeAtPath("sub_node");
            QVERIFY(sub_node != nullptr);
            QVERIFY(sub_node->isObject());
            QCOMPARE(sub_node->toObject().count(), 1);

            // Check "/included_value3/sub_node/value
            {
                QVERIFY(sub_node->toObject().contains("value"));
                const auto *value = sub_node->toObject().member("value");
                QVERIFY(value->isValue());
                QCOMPARE(value->toValue().value(), 3);
            }
        }
    }
}

// Test: read a config file with includes and environment variables --------------------------------

void TestConfigReader::testReadConfigWithIncludesAndEnv()
{
    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/ConfigWithIncludesAndEnv.json"));
    auto environmentVariables = EnvironmentVariables::loadFromProcess();
    environmentVariables.setValue("TEST_DATA_DIR", ":/TestData");
    ConfigReader configReader;
    QString error;

    auto config = configReader.read(configFilePath,
                                    QDir::current(),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables,
                                    &error);
    QVERIFY(config);
    QVERIFY(config->isObject());
    QCOMPARE(config->count(), 2);

    // Check "/included_value1"
    {
        const auto *included_value1 = config->member("included_value1");
        QVERIFY(included_value1 != nullptr);
        QVERIFY(included_value1->isValue());
        QCOMPARE(included_value1->toValue().value(), 1);
    }

    // Check "/included_value2"
    {
        const auto *included_value2 = config->member("included_value2");
        QVERIFY(included_value2 != nullptr);
        QVERIFY(included_value2->isValue());
        QCOMPARE(included_value2->toValue().value(), 2);
    }
}

// Test: read a config file with only includes (empty config) --------------------------------------

void TestConfigReader::testReadConfigWithOnlyIncludes()
{
    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/ConfigWithOnlyIncludes.json"));
    auto environmentVariables = EnvironmentVariables::loadFromProcess();
    ConfigReader configReader;
    QString error;

    auto config = configReader.read(configFilePath,
                                    QDir::current(),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables,
                                    &error);
    QVERIFY(config);
    QVERIFY(config->isObject());
    QCOMPARE(config->count(), 1);

    // Check "/included_config1"
    {
        const auto *included_config1 = config->nodeAtPath("/included_config1");
        QVERIFY(included_config1 != nullptr);
        QVERIFY(included_config1->isObject());
        QCOMPARE(included_config1->toObject().count(), 1);

        // Check "/included_config1/value
        {
            QVERIFY(included_config1->toObject().contains("value"));
            const auto *value = included_config1->toObject().member("value");
            QVERIFY(value->isValue());
            QCOMPARE(value->toValue().value(), 1);
        }
    }
}

// Test: read a config file with an include that references a node from "external configs" ---------

void TestConfigReader::testReadConfigWithExternalConfigReferences()
{
    // Read config file
    const QString configFilePath(
                QStringLiteral(":/TestData/ConfigWithExternalConfigReferences.json"));
    auto environmentVariables = EnvironmentVariables::loadFromProcess();
    ConfigReader configReader;
    QString error;

    auto config = configReader.read(configFilePath,
                                    QDir::current(),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables,
                                    &error);
    QVERIFY(config);
    QVERIFY(config->isObject());
    QCOMPARE(config->count(), 3);

    // Check "/included_config1"
    {
        const auto *included_config1 = config->nodeAtPath("/included_config1");
        QVERIFY(included_config1 != nullptr);
        QVERIFY(included_config1->isObject());
        QCOMPARE(included_config1->toObject().count(), 1);

        // Check "/included_config1/value
        {
            QVERIFY(included_config1->toObject().contains("value"));
            const auto *value = included_config1->toObject().member("value");
            QVERIFY(value->isValue());
            QCOMPARE(value->toValue().value(), 1);
        }
    }

    // Check "/ref_absolute_path"
    {
        const auto *value = config->nodeAtPath("/ref_absolute_path");
        QVERIFY(value->isValue());
        QCOMPARE(value->toValue().value(), 1);
    }

    // Check "/ref_relative_path"
    {
        const auto *value = config->nodeAtPath("/ref_relative_path");
        QVERIFY(value->isValue());
        QCOMPARE(value->toValue().value(), 1);
    }
}

// Test: read a config file with invalid file, source, and destination parameters ------------------

void TestConfigReader::testReadInvalidPathParameters()
{
    QFETCH(QString, filePath);
    QFETCH(QString, sourceNodePath);
    QFETCH(QString, destinationNodePath);

    // Read config file
    const QString configFilePath(filePath);
    auto environmentVariables = EnvironmentVariables::loadFromProcess();
    ConfigReader configReader;
    QString error;

    auto config = configReader.read(configFilePath,
                                    QDir::current(),
                                    ConfigNodePath(sourceNodePath),
                                    ConfigNodePath(destinationNodePath),
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables,
                                    &error);
    QVERIFY(!config);
    qDebug() << "TestConfigReader::testReadInvalidPathParameters: error string:" << error;
}

void TestConfigReader::testReadInvalidPathParameters_data()
{
    QTest::addColumn<QString>("filePath");
    QTest::addColumn<QString>("sourceNodePath");
    QTest::addColumn<QString>("destinationNodePath");

    QTest::newRow("File path: invalid") << ":/TestData/MissingConfigFile.json" << "/" << "/";
    QTest::newRow("Source node path: relative") << ":/TestData/ValidConfig.json" << "asd" << "/";
    QTest::newRow("Source node path: invalid") << ":/TestData/ValidConfig.json" << "0asd" << "/";
    QTest::newRow("Destination node path: relative")
            << ":/TestData/ValidConfig.json" << "/" << "asd";
    QTest::newRow("Destination node path: invalid")
            << ":/TestData/ValidConfig.json" << "/" << "0asd";
}

// Test: read a config file with invalid external config parameters --------------------------------

void TestConfigReader::testReadInvalidExternalConfigsParameter()
{
    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/ValidConfig.json"));
    auto environmentVariables = EnvironmentVariables::loadFromProcess();
    ConfigReader configReader;
    QString error;

    auto config = configReader.read(configFilePath,
                                    QDir::current(),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *> {nullptr},
                                    &environmentVariables,
                                    &error);
    QVERIFY(!config);
    qDebug() << "TestConfigReader::testReadInvalidExternalConfigsParameter: error string:" << error;
}

// Test: read an invalid config file ---------------------------------------------------------------

void TestConfigReader::testReadInvalidConfigFile()
{
    QFETCH(QString, filePath);

    // Read config file
    auto environmentVariables = EnvironmentVariables::loadFromProcess();
    ConfigReader configReader;
    QString error;

    auto config = configReader.read(filePath,
                                    QDir::current(),
                                    ConfigNodePath::ROOT_PATH,
                                    ConfigNodePath::ROOT_PATH,
                                    std::vector<const ConfigObjectNode *>(),
                                    &environmentVariables,
                                    &error);
    QVERIFY(!config);
    qDebug() << "TestConfigReader::testReadInvalidConfigFile: error string:" << error;
}

void TestConfigReader::testReadInvalidConfigFile_data()
{
    QTest::addColumn<QString>("filePath");

    QTest::newRow("InvalidJsonFile") << ":/TestData/InvalidJsonFile.json";
    QTest::newRow("NonObjectConfigFile") << ":/TestData/NonObjectConfigFile.json";
    QTest::newRow("IncludesNotArray") << ":/TestData/IncludesNotArray.json";
    QTest::newRow("IncludesItemNotObject") << ":/TestData/IncludesItemNotObject.json";
    QTest::newRow("IncludesItemInvalidType1") << ":/TestData/IncludesItemInvalidType1.json";
    QTest::newRow("IncludesItemInvalidType2") << ":/TestData/IncludesItemInvalidType2.json";
    QTest::newRow("IncludesItemMissingFilePath") << ":/TestData/IncludesItemMissingFilePath.json";
    QTest::newRow("IncludesItemInvalidFilePath") << ":/TestData/IncludesItemInvalidFilePath.json";
    QTest::newRow("IncludesItemInvalidSourceNode1")
            << ":/TestData/IncludesItemInvalidSourceNode1.json";
    QTest::newRow("IncludesItemInvalidSourceNode2")
            << ":/TestData/IncludesItemInvalidSourceNode2.json";
    QTest::newRow("IncludesItemInvalidSourceNode3")
            << ":/TestData/IncludesItemInvalidSourceNode3.json";
    QTest::newRow("IncludesItemInvalidSourceNode4")
            << ":/TestData/IncludesItemInvalidSourceNode4.json";
    QTest::newRow("IncludesItemInvalidSourceNode5")
            << ":/TestData/IncludesItemInvalidSourceNode5.json";
    QTest::newRow("IncludesItemInvalidDestinationNode1")
            << ":/TestData/IncludesItemInvalidDestinationNode1.json";
    QTest::newRow("IncludesItemInvalidDestinationNode2")
            << ":/TestData/IncludesItemInvalidDestinationNode2.json";
    QTest::newRow("IncludesItemInvalidDestinationNode3")
            << ":/TestData/IncludesItemInvalidDestinationNode3.json";
    QTest::newRow("IncludesItemInvalidConfig") << ":/TestData/IncludesItemInvalidConfig.json";
    QTest::newRow("ConfigNotObject") << ":/TestData/ConfigNotObject.json";
    QTest::newRow("ConfigInvalidMemberName") << ":/TestData/ConfigInvalidMemberName.json";
    QTest::newRow("ConfigInvalidNodeReference") << ":/TestData/ConfigInvalidNodeReference.json";
    QTest::newRow("ConfigInvalidDerivedObject1") << ":/TestData/ConfigInvalidDerivedObject1.json";
    QTest::newRow("ConfigInvalidDerivedObject2") << ":/TestData/ConfigInvalidDerivedObject2.json";
    QTest::newRow("ConfigInvalidDerivedObject3") << ":/TestData/ConfigInvalidDerivedObject3.json";
    QTest::newRow("ConfigInvalidDerivedObject4") << ":/TestData/ConfigInvalidDerivedObject4.json";
    QTest::newRow("ConfigInvalidDerivedObject5") << ":/TestData/ConfigInvalidDerivedObject5.json";
    QTest::newRow("ConfigInvalidDerivedObject6") << ":/TestData/ConfigInvalidDerivedObject6.json";
    QTest::newRow("ConfigInvalidDerivedObject7") << ":/TestData/ConfigInvalidDerivedObject7.json";
    QTest::newRow("ConfigInvalidDerivedObject8") << ":/TestData/ConfigInvalidDerivedObject8.json";
    QTest::newRow("ConfigInvalidDerivedObject9") << ":/TestData/ConfigInvalidDerivedObject9.json";
    QTest::newRow("ConfigInvalidDerivedObject10") << ":/TestData/ConfigInvalidDerivedObject10.json";
    QTest::newRow("ConfigInvalidReferenceType") << ":/TestData/ConfigInvalidReferenceType.json";
    QTest::newRow("ConfigInvalidSubObjectNode") << ":/TestData/ConfigInvalidSubObjectNode.json";
    QTest::newRow("ConfigUnresolvedReference") << ":/TestData/ConfigUnresolvedReference.json";
    QTest::newRow("ConfigUnresolvableExternalConfigReferences")
            << ":/TestData/ConfigUnresolvableExternalConfigReferences.json";
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigReader)
#include "testConfigReader.moc"
