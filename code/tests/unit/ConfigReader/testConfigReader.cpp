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
    ConfigReader configReader;

    auto config = configReader.read(configFilePath);
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
    ConfigReader configReader;

    auto config = configReader.read(configFilePath);
    QVERIFY(config);

    // Check config configs
    QVERIFY(config->isRoot());
    QCOMPARE(config->count(), 2);

    // Check "/root_node2/ref_value1
    {
        // References "/root_node1/value" node
        const auto *ref_value1 = config->nodeAtPath(ConfigNodePath("/root_node2/ref_value1"));
        QVERIFY(ref_value1 != nullptr);
        QVERIFY(ref_value1->isValue());
        QCOMPARE(ref_value1->toValue().value(), 1);
    }

    // Check "/root_node2/ref_value2
    {
        // References "/root_node1/sub_node" node
        const auto *ref_value2 = config->nodeAtPath(ConfigNodePath("/root_node2/ref_value2"));
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
        const auto *ref_value3 = config->nodeAtPath(ConfigNodePath("/root_node2/ref_value3"));
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
    ConfigReader configReader;

    auto config = configReader.read(configFilePath);
    QVERIFY(config);

    // Check "/derived_object1"
    {
        const auto *derived_object1 = config->nodeAtPath(ConfigNodePath("/derived_object1"));
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
        const auto *derived_object2 = config->nodeAtPath(ConfigNodePath("/derived_object2"));
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
        const auto *derived_object3 = config->nodeAtPath(ConfigNodePath("/derived_object3"));
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
    ConfigReader configReader;

    auto config = configReader.read(configFilePath);
    QVERIFY(config);
    QVERIFY(config->isObject());
    QCOMPARE(config->count(), 3);

    // Check "/included_config1"
    {
        const auto *included_config1 = config->nodeAtPath(ConfigNodePath("/included_config1"));
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
        const auto *included_config2_subnode =
                config->nodeAtPath(ConfigNodePath("/included_config2_subnode"));
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
        const auto *included_value3 = config->nodeAtPath(ConfigNodePath("/included_value3"));
        QVERIFY(included_value3 != nullptr);
        QVERIFY(included_value3->isValue());
        QCOMPARE(included_value3->toValue().value(), 3);
    }
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigReader)
#include "testConfigReader.moc"
