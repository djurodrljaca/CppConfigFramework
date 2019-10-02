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

private:
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
    QVERIFY(config->containsMember("root_node"));
    const auto *rootNode = config->member("root_node");
    QVERIFY(rootNode != nullptr);
    QVERIFY(rootNode->isObject());
    QCOMPARE(rootNode->count(), 1);

    // Check "/root_node/sub_node"
    {
        QVERIFY(rootNode->containsMember("sub_node"));
        const auto *subNode = rootNode->member("sub_node");
        QVERIFY(subNode != nullptr);
        QVERIFY(subNode->isObject());
        QCOMPARE(subNode->count(), 2);

        // Check "/root_node/sub_node/sub_node1"
        {
            QVERIFY(subNode->containsMember("sub_node1"));
            const auto *subNode1 = subNode->member("sub_node1");
            QVERIFY(subNode1 != nullptr);
            QVERIFY(subNode1->isObject());
            QCOMPARE(subNode1->count(), 2);

            // Check "/root_node/sub_node/sub_node1/bool_param"
            QVERIFY(subNode1->containsMember("bool_param"));
            const auto *boolParam = subNode1->member("bool_param");
            QVERIFY(boolParam != nullptr);
            QVERIFY(boolParam->isValue());
            QCOMPARE(boolParam->value(), QVariant(true));

            // Check "/root_node/sub_node/sub_node1/array_param"
            QVERIFY(subNode1->containsMember("array_param"));
            const auto *arrayParam = subNode1->member("array_param");
            QVERIFY(arrayParam != nullptr);
            QVERIFY(arrayParam->isArray());
            QCOMPARE(arrayParam->count(), 3);

            const int values[3] = {5, 6, 7};

            for (int i = 0; i < 3; i++)
            {
                const auto *element = arrayParam->element(i);
                QVERIFY(element != nullptr);
                QVERIFY(element->isValue());
                QCOMPARE(element->value(), QVariant(values[i]));
            }
        }

        // Check "/root_node/sub_node/sub_node2"
        {
            QVERIFY(subNode->containsMember("sub_node2"));
            const auto *subNode2 = subNode->member("sub_node2");
            QVERIFY(subNode2 != nullptr);
            QVERIFY(subNode2->isObject());
            QCOMPARE(subNode2->count(), 1);

            // Check "/root_node/sub_node/sub_node2/int_param"
            QVERIFY(subNode2->containsMember("int_param"));
            const auto *intParam = subNode2->member("int_param");
            QVERIFY(intParam != nullptr);
            QVERIFY(intParam->isValue());
            QCOMPARE(intParam->value(), QVariant(1));
        }
    }
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigReader)
#include "testConfigReader.moc"
