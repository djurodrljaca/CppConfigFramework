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
#include <CppConfigFramework/ConfigReader.hpp>

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
    void testReadConfigWithDerivedArray();
    void testReadConfigWithDerivedObject();
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
    QCOMPARE(config->type(), ConfigNode::Type::Object);
    QVERIFY(config->isRoot());
    QCOMPARE(config->count(), 1);

    // Check "/root_node"
    {
        QVERIFY(config->containsMember("root_node"));
        const auto *rootNode = config->member("root_node");
        QVERIFY(rootNode->isObject());
        QCOMPARE(rootNode->count(), 1);

        // Check "/root_node/sub_node"
        {
            QVERIFY(rootNode->containsMember("sub_node"));
            const auto *subNode = rootNode->member("sub_node");
            QVERIFY(subNode->isObject());
            QCOMPARE(subNode->count(), 2);

            // Check "/root_node/sub_node/sub_node1"
            {
                QVERIFY(subNode->containsMember("sub_node1"));
                const auto *subNode1 = subNode->member("sub_node1");
                QVERIFY(subNode1->isObject());
                QCOMPARE(subNode1->count(), 2);

                // Check "/root_node/sub_node/sub_node1/bool_param"
                QVERIFY(subNode1->containsMember("bool_param"));
                const auto *boolParam = subNode1->member("bool_param");
                QVERIFY(boolParam->isValue());
                QCOMPARE(boolParam->value(), QVariant(true));

                // Check "/root_node/sub_node/sub_node1/array_param"
                QVERIFY(subNode1->containsMember("array_param"));
                const auto *arrayParam = subNode1->member("array_param");
                QVERIFY(arrayParam->isArray());
                QCOMPARE(arrayParam->count(), 3);

                const int values[3] = {5, 6, 7};

                for (int i = 0; i < 3; i++)
                {
                    const auto *element = arrayParam->element(i);
                    QVERIFY(element->isValue());
                    QCOMPARE(element->value(), QVariant(values[i]));
                }
            }

            // Check "/root_node/sub_node/sub_node2"
            {
                QVERIFY(subNode->containsMember("sub_node2"));
                const auto *subNode2 = subNode->member("sub_node2");
                QVERIFY(subNode2->isObject());
                QCOMPARE(subNode2->count(), 1);

                // Check "/root_node/sub_node/sub_node2/int_param"
                QVERIFY(subNode2->containsMember("int_param"));
                const auto *intParam = subNode2->member("int_param");
                QVERIFY(intParam->isValue());
                QCOMPARE(intParam->value(), QVariant(1));
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
    QCOMPARE(config->type(), ConfigNode::Type::Object);
    QVERIFY(config->isRoot());
    QCOMPARE(config->count(), 2);

    // Check "/root_node2/ref_value1
    {
        // References "/root_node1/value" node
        const auto *ref_value1 = config->nodeAtPath("/root_node2/ref_value1");
        QVERIFY(ref_value1 != nullptr);
        QVERIFY(ref_value1->isValue());
        QCOMPARE(ref_value1->value(), 1);
    }

    // Check "/root_node2/ref_value2
    {
        // References "/root_node1/sub_node" node
        const auto *ref_value2 = config->nodeAtPath("/root_node2/ref_value2");
        QVERIFY(ref_value2 != nullptr);
        QVERIFY(ref_value2->isObject());

        // Check "/root_node1/sub_node/value"
        {
            QVERIFY(ref_value2->containsMember("value"));
            const auto *value = ref_value2->member("value");
            QVERIFY(value->isValue());
            QCOMPARE(value->value(), QVariant("str"));
        }
    }

    // Check "/root_node2/ref_value3
    {
        // References "/root_node2/sub_node/value" node
        const auto *ref_value3 = config->nodeAtPath("/root_node2/ref_value3");
        QVERIFY(ref_value3 != nullptr);
        QVERIFY(ref_value3->isValue());

        QVariantList expected = {1, 2, 3};
        QCOMPARE(ref_value3->value(), expected);
    }
}

// Test: read a config file with derived arrays ----------------------------------------------------

void TestConfigReader::testReadConfigWithDerivedArray()
{
    // Read config file
    const QString configFilePath(QStringLiteral(":/TestData/ConfigWithDerivedArrays.json"));
    ConfigReader configReader;

    auto config = configReader.read(configFilePath);
    QVERIFY(config);

    // Check "/derived_array1"
    {
        const auto *derived_array1 = config->nodeAtPath("/derived_array1");
        QVERIFY(derived_array1 != nullptr);
        QVERIFY(derived_array1->isArray());
        QCOMPARE(derived_array1->count(), 4);

        // Check "/derived_array1/0"
        {
            const auto *element = derived_array1->element(0);
            QVERIFY(element != nullptr);
            QVERIFY(element->isNull());
        }

        // Check "/derived_array1/1"
        {
            const auto *element = derived_array1->element(1);
            QVERIFY(element != nullptr);
            QVERIFY(element->isValue());
            QCOMPARE(element->value(), 1);
        }

        // Check "/derived_array1/2"
        {
            const auto *element = derived_array1->element(2);
            QVERIFY(element != nullptr);
            QVERIFY(element->isValue());

            QVariantList expected = {1, 2, 3};
            QCOMPARE(element->value(), expected);
        }

        // Check "/derived_array1/3"
        {
            const auto *element = derived_array1->element(3);
            QVERIFY(element != nullptr);
            QVERIFY(element->isObject());

            // Check "/derived_array1/3/item1"
            {
                QVERIFY(element->containsMember("item1"));
                const auto *item1 = element->member("item1");
                QVERIFY(item1->isValue());
                QCOMPARE(item1->value(), QVariant("str"));
            }

            // Check "/derived_array1/3/item2"
            {
                QVERIFY(element->containsMember("item2"));
                const auto *item2 = element->member("item2");
                QVERIFY(item2->isValue());
                QCOMPARE(item2->value(), QVariant(-1));
            }
        }
    }

    // Check "/derived_array2"
    {
        const auto *derived_array2 = config->nodeAtPath("/derived_array2");
        QVERIFY(derived_array2 != nullptr);
        QVERIFY(derived_array2->isArray());
        QCOMPARE(derived_array2->count(), 3);

        // Check "/derived_array2/0"
        {
            const auto *element = derived_array2->element(0);
            QVERIFY(element != nullptr);
            QVERIFY(element->isValue());
            QCOMPARE(element->value(), 1);
        }

        // Check "/derived_array2/1"
        {
            const auto *element = derived_array2->element(1);
            QVERIFY(element != nullptr);
            QVERIFY(element->isArray());

            // Check "/derived_array2/1/0"
            {
                const auto *subelement = element->element(0);
                QVERIFY(subelement != nullptr);
                QVERIFY(subelement->isValue());
                QCOMPARE(subelement->value(), 1);
            }
        }

        // Check "/derived_array2/2"
        {
            const auto *element = derived_array2->element(2);
            QVERIFY(element != nullptr);
            QVERIFY(element->isObject());
            QCOMPARE(element->count(), 2);

            // Check "/derived_array2/2/value
            {
                QVERIFY(element->containsMember("value"));
                const auto *value = element->member("value");
                QVERIFY(value->isValue());
                QCOMPARE(value->value(), QVariant("abc"));
            }

            // Check "/derived_array2/2/sub_node
            {
                QVERIFY(element->containsMember("sub_node"));
                const auto *subNode = element->member("sub_node");
                QVERIFY(subNode->isObject());

                // Check "/derived_array2/2/sub_node/value"
                {
                    QVERIFY(subNode->containsMember("value"));
                    const auto *value = subNode->member("value");
                    QVERIFY(value->isValue());
                    QCOMPARE(value->value(), QVariant("str"));
                }
            }
        }
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
        const auto *derived_object1 = config->nodeAtPath("/derived_object1");
        QVERIFY(derived_object1 != nullptr);
        QVERIFY(derived_object1->isObject());
        QCOMPARE(derived_object1->count(), 2);

        // Check "/derived_object1/value
        {
            QVERIFY(derived_object1->containsMember("value"));
            const auto *value = derived_object1->member("value");
            QVERIFY(value->isValue());
            QCOMPARE(value->value(), 1);
        }

        // Check "/derived_object1/sub_node
        {
            QVERIFY(derived_object1->containsMember("sub_node"));
            const auto *subNode = derived_object1->member("sub_node");
            QVERIFY(subNode->isObject());

            // Check "/derived_object1/sub_node/value"
            {
                QVERIFY(subNode->containsMember("value"));
                const auto *value = subNode->member("value");
                QVERIFY(value->isValue());
                QCOMPARE(value->value(), QVariant("str"));
            }
        }
    }

    // Check "/derived_object2"
    {
        const auto *derived_object2 = config->nodeAtPath("/derived_object2");
        QVERIFY(derived_object2 != nullptr);
        QVERIFY(derived_object2->isObject());
        QCOMPARE(derived_object2->count(), 2);

        // Check "/derived_object2/value
        {
            QVERIFY(derived_object2->containsMember("value"));
            const auto *value = derived_object2->member("value");
            QVERIFY(value->isValue());

            QVariantList expected = {1, 2, 3};
            QCOMPARE(value->value(), expected);
        }

        // Check "/derived_object2/sub_node
        {
            QVERIFY(derived_object2->containsMember("sub_node"));
            const auto *subNode = derived_object2->member("sub_node");
            QVERIFY(subNode->isObject());

            // Check "/derived_object2/sub_node/value"
            {
                QVERIFY(subNode->containsMember("value"));
                const auto *value = subNode->member("value");
                QVERIFY(value->isValue());
                QCOMPARE(value->value(), QVariant("abc"));
            }
        }
    }

    // Check "/derived_object3"
    {
        const auto *derived_object3 = config->nodeAtPath("/derived_object3");
        QVERIFY(derived_object3 != nullptr);
        QVERIFY(derived_object3->isObject());
        QCOMPARE(derived_object3->count(), 2);

        // Check "/derived_object3/value
        {
            QVERIFY(derived_object3->containsMember("value"));
            const auto *value = derived_object3->member("value");
            QVERIFY(value->isValue());
            QCOMPARE(value->value(), 1);
        }

        // Check "/derived_object3/sub_node
        {
            QVERIFY(derived_object3->containsMember("sub_node"));
            const auto *subNode = derived_object3->member("sub_node");
            QVERIFY(subNode->isObject());

            // Check "/derived_object3/sub_node/value"
            {
                QVERIFY(subNode->containsMember("value"));
                const auto *value = subNode->member("value");
                QVERIFY(value->isValue());
                QCOMPARE(value->value(), QVariant("abc"));
            }
        }
    }
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigReader)
#include "testConfigReader.moc"
