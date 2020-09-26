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
 * Contains unit tests for ConfigNode class
 */

// C++ Config Framework includes
#include <CppConfigFramework/ConfigDerivedObjectNode.hpp>
#include <CppConfigFramework/ConfigNodeReference.hpp>
#include <CppConfigFramework/ConfigObjectNode.hpp>
#include <CppConfigFramework/ConfigValueNode.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtCore/QLine>
#include <QtCore/QLineF>
#include <QtCore/QRect>
#include <QtCore/QRectF>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;
using ConfigNodePtr = std::shared_ptr<ConfigNode>;

Q_DECLARE_METATYPE(ConfigNode::Type);
Q_DECLARE_METATYPE(ConfigNodePtr);

class TestConfigNode : public QObject
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
    void testTypeToString();
    void testTypeToString_data();

    void testConstructorValue();
    void testConstructorObject();
    void testConstructorNodeReference();
    void testConstructorDerivedObject();

    void testMoveValue();
    void testMoveObject();
    void testMoveNodeReference();
    void testMoveDerivedObject();

    void testCloneValue();
    void testCloneObject();
    void testCloneNodeReference();
    void testCloneDerivedObject();

    void testValueNodeToJson();
    void testValueNodeToJson_data();

    void testObjectNodeToJson();
    void testObjectNodeToJson_data();

    void testNodeReferenceToJson();
    void testNodeReferenceToJson_data();

    void testDerivedObjectNodeToJson();
    void testDerivedObjectNodeToJson_data();

    void testNodePath();

    void testObjectNode();
    void testApplyObject();

    void testDerivedObjectNode();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestConfigNode::initTestCase()
{
}

void TestConfigNode::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestConfigNode::init()
{
}

void TestConfigNode::cleanup()
{
}

// Test: typeToString() method ---------------------------------------------------------------------

void TestConfigNode::testTypeToString()
{
    QFETCH(ConfigNode::Type, type);
    QFETCH(QString, expectedResult);

    QCOMPARE(ConfigNode::typeToString(type), expectedResult);
}

void TestConfigNode::testTypeToString_data()
{
    QTest::addColumn<ConfigNode::Type>("type");
    QTest::addColumn<QString>("expectedResult");

    QTest::newRow("Value") << ConfigNode::Type::Value << "Value";
    QTest::newRow("Object") << ConfigNode::Type::Object << "Object";
    QTest::newRow("NodeReference") << ConfigNode::Type::NodeReference << "NodeReference";
    QTest::newRow("DerivedObject") << ConfigNode::Type::DerivedObject << "DerivedObject";
}

// Test: Constructor -------------------------------------------------------------------------------

void TestConfigNode::testConstructorValue()
{
    ConfigValueNode node1(1);
    QCOMPARE(node1.type(), ConfigNode::Type::Value);
    QVERIFY(node1.isValue());
    QVERIFY(!node1.isObject());
    QVERIFY(!node1.isNodeReference());
    QVERIFY(!node1.isDerivedObject());
    QVERIFY(node1.isRoot());
    QCOMPARE(node1.rootNode(), nullptr);
    QCOMPARE(node1.parent(), nullptr);
    QCOMPARE(node1.value(), 1);

    ConfigObjectNode root;
    ConfigValueNode node2("str", &root);
    QCOMPARE(node2.type(), ConfigNode::Type::Value);
    QVERIFY(node2.isValue());
    QVERIFY(!node2.isObject());
    QVERIFY(!node2.isNodeReference());
    QVERIFY(!node2.isDerivedObject());
    QVERIFY(!node2.isRoot());
    QCOMPARE(node2.rootNode(), &root);
    QCOMPARE(node2.parent(), &root);
    QCOMPARE(node2.value(), QJsonValue("str"));
}

void TestConfigNode::testConstructorObject()
{
    ConfigObjectNode node1;
    QCOMPARE(node1.type(), ConfigNode::Type::Object);
    QVERIFY(!node1.isValue());
    QVERIFY(node1.isObject());
    QVERIFY(!node1.isNodeReference());
    QVERIFY(!node1.isDerivedObject());
    QVERIFY(node1.isRoot());
    QCOMPARE(node1.rootNode(), &node1);
    QCOMPARE(node1.parent(), nullptr);
    QCOMPARE(node1.count(), 0);

    ConfigObjectNode root;
    ConfigObjectNode node2(&root);
    QCOMPARE(node2.type(), ConfigNode::Type::Object);
    QVERIFY(!node2.isValue());
    QVERIFY(node2.isObject());
    QVERIFY(!node2.isNodeReference());
    QVERIFY(!node2.isDerivedObject());
    QVERIFY(!node2.isRoot());
    QCOMPARE(node2.rootNode(), &root);
    QCOMPARE(node2.parent(), &root);
    QCOMPARE(node2.count(), 0);
}

void TestConfigNode::testConstructorNodeReference()
{
    ConfigNodeReference node1(ConfigNodePath("/aaa/bbb"));
    QCOMPARE(node1.type(), ConfigNode::Type::NodeReference);
    QVERIFY(!node1.isValue());
    QVERIFY(!node1.isObject());
    QVERIFY(node1.isNodeReference());
    QVERIFY(!node1.isDerivedObject());
    QVERIFY(node1.isRoot());
    QCOMPARE(node1.rootNode(), nullptr);
    QCOMPARE(node1.parent(), nullptr);
    QCOMPARE(node1.reference(), ConfigNodePath("/aaa/bbb"));

    ConfigObjectNode root;
    ConfigNodeReference node2(ConfigNodePath("ccc/ddd"), &root);
    QCOMPARE(node2.type(), ConfigNode::Type::NodeReference);
    QVERIFY(!node2.isValue());
    QVERIFY(!node2.isObject());
    QVERIFY(node2.isNodeReference());
    QVERIFY(!node2.isDerivedObject());
    QVERIFY(!node2.isRoot());
    QCOMPARE(node2.rootNode(), &root);
    QCOMPARE(node2.parent(), &root);
    QCOMPARE(node2.reference(), ConfigNodePath("ccc/ddd"));
}

void TestConfigNode::testConstructorDerivedObject()
{
    QList<ConfigNodePath> bases = {
        ConfigNodePath("/aaa/bbb"),
        ConfigNodePath("ccc/ddd")
    };

    ConfigObjectNode config;
    config.setMember("a", ConfigValueNode(1));
    config.setMember("b", ConfigValueNode("str"));

    ConfigDerivedObjectNode node1(bases, config, nullptr);
    QCOMPARE(node1.type(), ConfigNode::Type::DerivedObject);
    QVERIFY(!node1.isValue());
    QVERIFY(!node1.isObject());
    QVERIFY(!node1.isNodeReference());
    QVERIFY(node1.isDerivedObject());
    QVERIFY(node1.isRoot());
    QCOMPARE(node1.rootNode(), nullptr);
    QCOMPARE(node1.parent(), nullptr);
    QCOMPARE(node1.bases(), bases);
    QCOMPARE(node1.config().count(), 2);

    ConfigObjectNode root;
    bases.append(ConfigNodePath("eee"));
    config.setMember("c", ConfigValueNode(true));
    ConfigDerivedObjectNode node2(bases, config, &root);
    QCOMPARE(node2.type(), ConfigNode::Type::DerivedObject);
    QVERIFY(!node2.isValue());
    QVERIFY(!node2.isObject());
    QVERIFY(!node2.isNodeReference());
    QVERIFY(node2.isDerivedObject());
    QVERIFY(!node2.isRoot());
    QCOMPARE(node2.rootNode(), &root);
    QCOMPARE(node2.parent(), &root);
    QCOMPARE(node2.config().count(), 3);
}

// Test: Move constructor and move assignment operator ---------------------------------------------

void TestConfigNode::testMoveValue()
{
    ConfigObjectNode parentNode1;
    ConfigObjectNode parentNode2;

    // Move constructor
    {
        ConfigValueNode node(123, &parentNode1);

        ConfigValueNode movedNode(std::move(node));
        QVERIFY(movedNode.isValue());
        QCOMPARE(movedNode.parent(), &parentNode1);
        QCOMPARE(movedNode.value(), 123);
    }

    // Move assignment operator
    {
        ConfigValueNode node(456, &parentNode2);

        ConfigValueNode movedNode;
        movedNode = std::move(node);
        QVERIFY(movedNode.isValue());
        QCOMPARE(movedNode.parent(), &parentNode2);
        QCOMPARE(movedNode.value(), 456);

        // Self assignment
        movedNode = std::move(movedNode);
        QVERIFY(movedNode.isValue());
        QCOMPARE(movedNode.parent(), &parentNode2);
        QCOMPARE(movedNode.value(), 456);
    }
}

void TestConfigNode::testMoveObject()
{
    ConfigObjectNode parentNode1;
    ConfigObjectNode parentNode2;

    // Move constructor
    {
        ConfigObjectNode node(&parentNode1);
        node.setMember("item1", ConfigValueNode(1));
        node.setMember("item2", ConfigValueNode("str"));

        ConfigObjectNode movedNode(std::move(node));
        QVERIFY(movedNode.isObject());
        QCOMPARE(movedNode.parent(), &parentNode1);
        QCOMPARE(movedNode.count(), 2);

        QCOMPARE(movedNode.member("item1")->type(), ConfigNode::Type::Value);
        QCOMPARE(movedNode.member("item1")->parent(), &movedNode);

        QCOMPARE(movedNode.member("item2")->type(), ConfigNode::Type::Value);
        QCOMPARE(movedNode.member("item2")->parent(), &movedNode);
    }

    // Move assignment operator
    {
        ConfigObjectNode node(&parentNode1);
        node.setMember("item1", ConfigValueNode(1));
        node.setMember("item2", ConfigValueNode("str"));

        ConfigObjectNode movedNode;
        movedNode = std::move(node);
        QVERIFY(movedNode.isObject());
        QCOMPARE(movedNode.parent(), &parentNode1);
        QCOMPARE(movedNode.count(), 2);

        QCOMPARE(movedNode.member("item1")->type(), ConfigNode::Type::Value);
        QCOMPARE(movedNode.member("item1")->parent(), &movedNode);

        QCOMPARE(movedNode.member("item2")->type(), ConfigNode::Type::Value);
        QCOMPARE(movedNode.member("item2")->parent(), &movedNode);

        // Self assignment
        movedNode = std::move(movedNode);
        QVERIFY(movedNode.isObject());
        QCOMPARE(movedNode.parent(), &parentNode1);
        QCOMPARE(movedNode.count(), 2);

        QCOMPARE(movedNode.member("item1")->type(), ConfigNode::Type::Value);
        QCOMPARE(movedNode.member("item1")->parent(), &movedNode);

        QCOMPARE(movedNode.member("item2")->type(), ConfigNode::Type::Value);
        QCOMPARE(movedNode.member("item2")->parent(), &movedNode);
    }
}

void TestConfigNode::testMoveNodeReference()
{
    ConfigObjectNode parentNode1;
    ConfigObjectNode parentNode2;

    // Move constructor
    {
        ConfigNodeReference node(ConfigNodePath("ref"), &parentNode1);

        ConfigNodeReference movedNode(std::move(node));
        QVERIFY(movedNode.isNodeReference());
        QCOMPARE(movedNode.parent(), &parentNode1);
        QCOMPARE(movedNode.reference(), ConfigNodePath("ref"));
    }

    // Move assignment operator
    {
        ConfigNodeReference node(ConfigNodePath("ref"), &parentNode2);

        ConfigNodeReference movedNode;
        movedNode = std::move(node);
        QVERIFY(movedNode.isNodeReference());
        QCOMPARE(movedNode.parent(), &parentNode2);
        QCOMPARE(movedNode.reference(), ConfigNodePath("ref"));

        // Self assignment
        movedNode = std::move(movedNode);
        QVERIFY(movedNode.isNodeReference());
        QCOMPARE(movedNode.parent(), &parentNode2);
        QCOMPARE(movedNode.reference(), ConfigNodePath("ref"));
    }
}

void TestConfigNode::testMoveDerivedObject()
{
    ConfigObjectNode parentNode1;
    ConfigObjectNode parentNode2;

    QList<ConfigNodePath> expectedBases { ConfigNodePath("base1"), ConfigNodePath("base2")};
    ConfigObjectNode config;
    config.setMember("null", ConfigValueNode());

    // Move constructor
    {
        ConfigDerivedObjectNode node(expectedBases, config, &parentNode1);

        ConfigDerivedObjectNode movedNode(std::move(node));
        QVERIFY(movedNode.isDerivedObject());
        QCOMPARE(movedNode.parent(), &parentNode1);
        QCOMPARE(movedNode.bases(), expectedBases);
        QVERIFY(movedNode.config().isObject());
        QCOMPARE(movedNode.config().count(), 1);
        QVERIFY(movedNode.config().contains("null"));
        QVERIFY(movedNode.config().member("null")->isValue());
    }

    // Move assignment operator
    {
        ConfigDerivedObjectNode node(expectedBases, config, &parentNode2);

        ConfigDerivedObjectNode movedNode;
        movedNode = std::move(node);
        QVERIFY(movedNode.isDerivedObject());
        QCOMPARE(movedNode.parent(), &parentNode2);
        QCOMPARE(movedNode.bases(), expectedBases);
        QVERIFY(movedNode.config().isObject());
        QCOMPARE(movedNode.config().count(), 1);
        QVERIFY(movedNode.config().contains("null"));
        QVERIFY(movedNode.config().member("null")->isValue());

        // Self assignment
        movedNode = std::move(movedNode);
        QVERIFY(movedNode.isDerivedObject());
        QCOMPARE(movedNode.parent(), &parentNode2);
        QCOMPARE(movedNode.bases(), expectedBases);
        QVERIFY(movedNode.config().isObject());
        QCOMPARE(movedNode.config().count(), 1);
        QVERIFY(movedNode.config().contains("null"));
        QVERIFY(movedNode.config().member("null")->isValue());
    }
}

// Test: clone() method ----------------------------------------------------------------------------

void TestConfigNode::testCloneValue()
{
    // Clone without parent
    ConfigValueNode node(123);

    auto clonedNode = node.clone();
    QVERIFY(clonedNode->isValue());
    QCOMPARE(clonedNode->parent(), nullptr);
    QCOMPARE(clonedNode->toValue().value(), 123);

    // Clone with parent
    ConfigObjectNode parentNode;
    node.setParent(&parentNode);
    node.setValue(456);

    clonedNode = node.clone();
    QVERIFY(clonedNode->isValue());
    QCOMPARE(clonedNode->parent(), nullptr);
    QCOMPARE(clonedNode->toValue().value(), 456);
}

void TestConfigNode::testCloneObject()
{
    // Clone without parent
    ConfigObjectNode node;
    node.setMember("item1", ConfigValueNode(1));
    node.setMember("item2", ConfigValueNode("str"));

    auto clonedNode = node.clone();
    QVERIFY(clonedNode->isObject());
    QCOMPARE(clonedNode->parent(), nullptr);
    QCOMPARE(clonedNode->toObject().count(), 2);

    QCOMPARE(clonedNode->toObject().member("item1")->type(), ConfigNode::Type::Value);
    QCOMPARE(clonedNode->toObject().member("item1")->parent(), clonedNode.get());
    QCOMPARE(clonedNode->toObject().member("item1")->toValue().value(), 1);

    QCOMPARE(clonedNode->toObject().member("item2")->type(), ConfigNode::Type::Value);
    QCOMPARE(clonedNode->toObject().member("item2")->parent(), clonedNode.get());
    QCOMPARE(clonedNode->toObject().member("item2")->toValue().value(), QJsonValue("str"));

    // Clone with parent
    ConfigObjectNode parentNode;
    node.setParent(&parentNode);
    node.removeAll();
    node.setMember("item1", ConfigValueNode(2));
    node.setMember("item2", ConfigValueNode("asd"));

    clonedNode = node.clone();
    QVERIFY(clonedNode->isObject());
    QCOMPARE(clonedNode->parent(), nullptr);
    QCOMPARE(clonedNode->toObject().count(), 2);

    QCOMPARE(clonedNode->toObject().member("item1")->type(), ConfigNode::Type::Value);
    QCOMPARE(clonedNode->toObject().member("item1")->parent(), clonedNode.get());
    QCOMPARE(clonedNode->toObject().member("item1")->toValue().value(), 2);

    QCOMPARE(clonedNode->toObject().member("item2")->type(), ConfigNode::Type::Value);
    QCOMPARE(clonedNode->toObject().member("item2")->parent(), clonedNode.get());
    QCOMPARE(clonedNode->toObject().member("item2")->toValue().value(), QJsonValue("asd"));
}

void TestConfigNode::testCloneNodeReference()
{
    // Clone without parent
    ConfigNodeReference node;
    node.setReference(ConfigNodePath("ref1"));

    auto clonedNode = node.clone();
    QVERIFY(clonedNode->isNodeReference());
    QCOMPARE(clonedNode->parent(), nullptr);
    QCOMPARE(clonedNode->toNodeReference().reference(), ConfigNodePath("ref1"));

    // Clone with parent
    ConfigObjectNode parentNode;
    node.setParent(&parentNode);
    node.setReference(ConfigNodePath("ref2"));

    clonedNode = node.clone();
    QVERIFY(clonedNode->isNodeReference());
    QCOMPARE(clonedNode->parent(), nullptr);
    QCOMPARE(clonedNode->toNodeReference().reference(), ConfigNodePath("ref2"));
}

void TestConfigNode::testCloneDerivedObject()
{
    QList<ConfigNodePath> expectedBases { ConfigNodePath("base1"), ConfigNodePath("base2")};
    ConfigObjectNode config;
    config.setMember("null", ConfigValueNode());

    // Clone without parent
    ConfigDerivedObjectNode node(expectedBases, config);

    auto clonedNode = node.clone();
    QVERIFY(clonedNode->isDerivedObject());
    QCOMPARE(clonedNode->parent(), nullptr);
    QCOMPARE(clonedNode->toDerivedObject().bases(), expectedBases);
    QVERIFY(clonedNode->toDerivedObject().config().isObject());
    QCOMPARE(clonedNode->toDerivedObject().config().count(), 1);
    QVERIFY(clonedNode->toDerivedObject().config().contains("null"));
    QVERIFY(clonedNode->toDerivedObject().config().member("null")->isValue());

    // Clone with parent
    ConfigObjectNode parentNode;
    node.setParent(&parentNode);

    clonedNode = node.clone();
    QVERIFY(clonedNode->isDerivedObject());
    QCOMPARE(clonedNode->parent(), nullptr);
    QCOMPARE(clonedNode->toDerivedObject().bases(), expectedBases);
    QVERIFY(clonedNode->toDerivedObject().config().isObject());
    QCOMPARE(clonedNode->toDerivedObject().config().count(), 1);
    QVERIFY(clonedNode->toDerivedObject().config().contains("null"));
    QVERIFY(clonedNode->toDerivedObject().config().member("null")->isValue());
}

// Test: ConfigValueNode::toJson() method ----------------------------------------------------------

void TestConfigNode::testValueNodeToJson()
{
    QFETCH(ConfigNodePtr, node);
    QFETCH(QJsonValue, expectedResult);

    auto actualValue = node->toJson();
    QCOMPARE(actualValue, expectedResult);
}

void TestConfigNode::testValueNodeToJson_data()
{
    QTest::addColumn<ConfigNodePtr>("node");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const QJsonValue expectedResult(true);
        ConfigValueNode node(expectedResult);

        QTest::newRow("ConfigValueNode: bool") << ConfigNodePtr(node.clone()) << expectedResult;
    }

    {
        const QJsonValue expectedResult(123.0);
        ConfigValueNode node(expectedResult);

        QTest::newRow("ConfigValueNode: double") << ConfigNodePtr(node.clone()) << expectedResult;
    }

    {
        const QJsonValue expectedResult("asd");
        ConfigValueNode node(expectedResult);

        QTest::newRow("ConfigValueNode: string") << ConfigNodePtr(node.clone()) << expectedResult;
    }

    {
        const QJsonArray expectedResult { 1, 2, 3 };
        ConfigValueNode node(expectedResult);

        QTest::newRow("ConfigValueNode: array") << ConfigNodePtr(node.clone())
                                                << QJsonValue(expectedResult);
    }

    {
        const QJsonObject expectedResult { { "a", 1 }, { "b", 2 }, { "c", 3 },  };
        ConfigValueNode node(expectedResult);

        QTest::newRow("ConfigValueNode: array") << ConfigNodePtr(node.clone())
                                                << QJsonValue(expectedResult);
    }
}

// Test: ConfigObjectNode::toJson() method ---------------------------------------------------------

void TestConfigNode::testObjectNodeToJson()
{
    QFETCH(ConfigNodePtr, node);
    QFETCH(QJsonValue, expectedResult);

    auto actualValue = node->toJson();
    QCOMPARE(actualValue, expectedResult);
}

void TestConfigNode::testObjectNodeToJson_data()
{
    QTest::addColumn<ConfigNodePtr>("node");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        ConfigObjectNode node;
        const QJsonObject expectedResult;

        QTest::newRow("ConfigObjectNode: empty") << ConfigNodePtr(node.clone())
                                                 << QJsonValue(expectedResult);
    }

    {
        ConfigObjectNode node;
        node.setMember("param", ConfigValueNode(123));

        const QJsonObject expectedResult { { "#param", 123 } };

        QTest::newRow("ConfigObjectNode: value node param") << ConfigNodePtr(node.clone())
                                                            << QJsonValue(expectedResult);
    }

    {
        ConfigObjectNode param;
        param.setMember("value", ConfigValueNode(123));

        ConfigObjectNode node;
        node.setMember("param", param);

        const QJsonObject expectedResult
        {
            {
                "param", QJsonObject
                {
                    { "#value", 123 }
                }
            }
        };

        QTest::newRow("ConfigObjectNode: object node param") << ConfigNodePtr(node.clone())
                                                             << QJsonValue(expectedResult);
    }

    {
        const ConfigNodePath reference("node/path");

        ConfigObjectNode node;
        node.setMember("param", ConfigNodeReference(reference));

        const QJsonObject expectedResult { { "&param", reference.path() } };

        QTest::newRow("ConfigObjectNode: node ref param") << ConfigNodePtr(node.clone())
                                                          << QJsonValue(expectedResult);
    }

    {
        const QList<ConfigNodePath> bases = { ConfigNodePath("node/path") };

        ConfigObjectNode config;
        config.setMember("value", ConfigValueNode(123));

        const ConfigDerivedObjectNode param(bases, config);

        ConfigObjectNode node;
        node.setMember("param", param);

        const QJsonObject expectedResult
        {
            {
                "&param", QJsonObject
                {
                    { "base", "node/path" },
                    {
                        "config", QJsonObject
                        {
                            { "#value", 123 }
                        }
                    }
                }
            }
        };

        QTest::newRow("ConfigObjectNode: derived object node param") << ConfigNodePtr(node.clone())
                                                                     << QJsonValue(expectedResult);
    }
}

// Test: ConfigNodeReference::toJson() method ------------------------------------------------------

void TestConfigNode::testNodeReferenceToJson()
{
    QFETCH(ConfigNodePtr, node);
    QFETCH(QJsonValue, expectedResult);

    auto actualValue = node->toJson();
    QCOMPARE(actualValue, expectedResult);
}

void TestConfigNode::testNodeReferenceToJson_data()
{
    QTest::addColumn<ConfigNodePtr>("node");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const ConfigNodePath reference("node/path");
        const ConfigNodeReference node(reference);
        const QJsonValue expectedResult(reference.path());

        QTest::newRow("ConfigNodeReference") << ConfigNodePtr(node.clone()) << expectedResult;
    }
}

// Test: ConfigDerivedObjectNode::toJson() method --------------------------------------------------

void TestConfigNode::testDerivedObjectNodeToJson()
{
    QFETCH(ConfigNodePtr, node);
    QFETCH(QJsonValue, expectedResult);

    auto actualValue = node->toJson();
    QCOMPARE(actualValue, expectedResult);
}

void TestConfigNode::testDerivedObjectNodeToJson_data()
{
    QTest::addColumn<ConfigNodePtr>("node");
    QTest::addColumn<QJsonValue>("expectedResult");

    {
        const ConfigDerivedObjectNode node;
        const QJsonObject expectedResult = { { "config", QJsonObject() } };

        QTest::newRow("ConfigDerivedObjectNode: no base, empty config")
                << ConfigNodePtr(node.clone())
                << QJsonValue(expectedResult);
    }

    {
        ConfigObjectNode config;
        config.setMember("param", ConfigValueNode(123));

        const ConfigDerivedObjectNode node({}, config);
        const QJsonObject expectedResult
        {
            {
                "config",
                QJsonObject
                {
                    { "#param", 123 }
                }
            }
        };

        QTest::newRow("ConfigDerivedObjectNode: no base, non-empty config")
                << ConfigNodePtr(node.clone())
                << QJsonValue(expectedResult);
    }

    {
        const QList<ConfigNodePath> bases = { ConfigNodePath("node/path") };

        ConfigObjectNode config;
        config.setMember("param", ConfigValueNode(123));

        const ConfigDerivedObjectNode node(bases, config);
        const QJsonObject expectedResult
        {
            { "base", "node/path" },
            {
                "config", QJsonObject
                {
                    { "#param", 123 }
                }
            }
        };

        QTest::newRow("ConfigDerivedObjectNode: single base, non-empty config")
                << ConfigNodePtr(node.clone())
                << QJsonValue(expectedResult);
    }

    {
        const QList<ConfigNodePath> bases =
        {
            ConfigNodePath("node1"),
            ConfigNodePath("node2")
        };

        ConfigObjectNode config;
        config.setMember("param", ConfigValueNode(123));

        const ConfigDerivedObjectNode node(bases, config);
        const QJsonObject expectedResult
        {
            { "base", QJsonArray { "node1", "node2" } },
            {
                "config", QJsonObject
                {
                    { "#param", 123 }
                }
            }
        };

        QTest::newRow("ConfigDerivedObjectNode: multiple bases, non-empty config")
                << ConfigNodePtr(node.clone())
                << QJsonValue(expectedResult);
    }
}

// Test: nodePath() method -------------------------------------------------------------------------

void TestConfigNode::testNodePath()
{
    // Root node
    ConfigObjectNode rootNode;

    QVERIFY(rootNode.isRoot());
    QCOMPARE(rootNode.rootNode(), &rootNode);
    QCOMPARE(rootNode.nodePath(), ConfigNodePath::ROOT_PATH);
    QCOMPARE(rootNode.nodeAtPath(ConfigNodePath::ROOT_PATH), &rootNode);

    // Level 1
    rootNode.setMember("level1", ConfigObjectNode());
    ConfigNode *level1 = rootNode.member("level1");
    QVERIFY(level1 != nullptr);
    QVERIFY(level1->isObject());
    QCOMPARE(level1->rootNode(), &rootNode);
    QCOMPARE(level1->nodePath(), ConfigNodePath("/level1"));
    QCOMPARE(rootNode.nodeAtPath(ConfigNodePath("/level1")), level1);

    // Level 2
    level1->toObject().setMember("level2", ConfigObjectNode());
    ConfigNode *level2 = level1->toObject().member("level2");
    QVERIFY(level2 != nullptr);
    QVERIFY(level2->isObject());
    QCOMPARE(level2->rootNode(), &rootNode);
    QCOMPARE(level2->nodePath(), ConfigNodePath("/level1/level2"));
    QCOMPARE(rootNode.nodeAtPath(ConfigNodePath("/level1/level2")), level2);

    // Level 3
    level2->toObject().setMember("level3", ConfigObjectNode());
    ConfigNode *level3 = level2->toObject().member("level3");
    QVERIFY(level3 != nullptr);
    QVERIFY(level3->isObject());
    QCOMPARE(level3->rootNode(), &rootNode);
    QCOMPARE(level3->nodePath(), ConfigNodePath("/level1/level2/level3"));
    QCOMPARE(rootNode.nodeAtPath(ConfigNodePath("/level1/level2/level3")), level3);

    // Level 3 members
    level3->toObject().setMember("item1", ConfigValueNode(1));
    ConfigNode *level3Item1 = level3->toObject().member("item1");
    QVERIFY(level3Item1 != nullptr);
    QVERIFY(level3Item1->isValue());
    QCOMPARE(level3Item1->toValue().value(), 1);
    QCOMPARE(level3Item1->rootNode(), &rootNode);
    QCOMPARE(level3Item1->nodePath(), ConfigNodePath("/level1/level2/level3/item1"));
    QCOMPARE(rootNode.nodeAtPath(ConfigNodePath("/level1/level2/level3/item1")), level3Item1);

    level3->toObject().setMember("item2", ConfigValueNode("str"));
    ConfigNode *level3Item2 = level3->toObject().member("item2");
    QVERIFY(level3Item2 != nullptr);
    QVERIFY(level3Item2->isValue());
    QCOMPARE(level3Item2->toValue().value(), QJsonValue("str"));
    QCOMPARE(level3Item2->rootNode(), &rootNode);
    QCOMPARE(level3Item2->nodePath(), ConfigNodePath("/level1/level2/level3/item2"));
    QCOMPARE(rootNode.nodeAtPath(ConfigNodePath("/level1/level2/level3/item2")), level3Item2);

    // Test absolute paths on non-root items
    QCOMPARE(level3Item1->nodeAtPath(ConfigNodePath::ROOT_PATH), &rootNode);
    QCOMPARE(level3Item2->nodeAtPath(ConfigNodePath::ROOT_PATH), &rootNode);

    QCOMPARE(level3Item1->nodeAtPath("/level1"), level1);
    QCOMPARE(level3Item2->nodeAtPath("/level1/level2"), level2);

    // Test relative paths
    QCOMPARE(rootNode.nodeAtPath("/level1/.."), &rootNode);
    QCOMPARE(rootNode.nodeAtPath("level1/.."), &rootNode);

    QCOMPARE(rootNode.nodeAtPath("level1"), level1);
    QCOMPARE(rootNode.nodeAtPath("/level1/level2/.."), level1);
    QCOMPARE(rootNode.nodeAtPath("/level1/../level1/level2/.."), level1);
    QCOMPARE(rootNode.nodeAtPath("level1/level2/.."), level1);

    QCOMPARE(level1->nodeAtPath(".."), &rootNode);
    QCOMPARE(level1->nodeAtPath("level2/.."), level1);
    QCOMPARE(level1->nodeAtPath("level2"), level2);

    QCOMPARE(level3Item2->nodeAtPath("../item1"), level3Item1);

    QCOMPARE(level1->nodeAtPath(""), nullptr);
    QCOMPARE(level1->nodeAtPath("../.."), nullptr);
    QCOMPARE(level3->nodeAtPath("error"), nullptr);
    QCOMPARE(level3->nodeAtPath("item1/error"), nullptr);

    // Test non-Object "root" nodes
    std::unique_ptr<ConfigNode> nonObjectRootNodes[] =
    {
        ConfigValueNode().clone(),
        ConfigNodeReference().clone(),
        ConfigDerivedObjectNode().clone()
    };

    for (const auto &item : nonObjectRootNodes)
    {
        QVERIFY(item->isRoot());
        QCOMPARE(item->rootNode(), nullptr);
        QCOMPARE(item->nodeAtPath(ConfigNodePath::ROOT_PATH), nullptr);
        QCOMPARE(item->nodeAtPath(ConfigNodePath("/anything")), nullptr);
    }
}

// Test: Object node -------------------------------------------------------------------------------

void TestConfigNode::testObjectNode()
{
    ConfigObjectNode object;
    const auto &objectConst = static_cast<const ConfigObjectNode&>(object);
    QVERIFY(object.setMember("item1", ConfigValueNode(true)));
    QVERIFY(object.setMember("item2", ConfigValueNode(123)));
    QVERIFY(object.setMember("item3", ConfigObjectNode()));

    QVERIFY(!object.setMember("0item", ConfigValueNode()));

    QCOMPARE(object.count(), 3);
    auto objectNames = object.names();
    QCOMPARE(QSet<QString>(objectNames.begin(), objectNames.end()),
             QSet<QString>({"item1", "item2", "item3"}));
    QVERIFY(object.member("item1")->isValue());
    QCOMPARE(object.member("item1")->toValue().value(), true);
    QVERIFY(object.member("item2")->isValue());
    QCOMPARE(object.member("item2")->toValue().value(), 123);
    QVERIFY(object.member("item3")->isObject());

    QCOMPARE(object.member("item1"), objectConst.member("item1"));

    QCOMPARE(object.member("item9"), nullptr);
    QCOMPARE(objectConst.member("item9"), nullptr);

    object.setMember("item2", ConfigValueNode("str"));
    QCOMPARE(object.member("item2")->toValue().value(), QJsonValue("str"));

    QVERIFY(object.remove("item2"));
    QVERIFY(!object.remove("item9"));

    QCOMPARE(object.count(), 2);
    objectNames = object.names();
    QCOMPARE(QSet<QString>(objectNames.begin(), objectNames.end()),
             QSet<QString>({"item1", "item3"}));
    QVERIFY(object.member("item1")->isValue());
    QVERIFY(object.member("item3")->isObject());

    QCOMPARE(object.name(*object.member("item1")), QString("item1"));
    QCOMPARE(object.name(object), QString());

    object.removeAll();
    QCOMPARE(object.count(), 0);
}

// Test: ConfigObjectNode::apply() method ----------------------------------------------------------

void TestConfigNode::testApplyObject()
{
    // Create 3 level node structure
    ConfigObjectNode node;
    node.setMember("value", ConfigValueNode(111));
    node.setMember("valueToObject", ConfigValueNode(1));

    node.setMember("level1", ConfigObjectNode());

    node.member("level1")->toObject().setMember("level2", ConfigObjectNode());
    node.nodeAtPath("level1/level2")->toObject().setMember("null", ConfigValueNode());
    node.nodeAtPath("level1/level2")->toObject().setMember("value", ConfigValueNode(123));

    // Create a compatible node structure for the update
    ConfigObjectNode update;
    update.setMember("null", ConfigValueNode());
    update.setMember("value", ConfigValueNode(123));
    update.setMember("valueToObject", ConfigObjectNode());

    update.setMember("level1", ConfigObjectNode());
    update.member("level1")->toObject().setMember("null", ConfigValueNode());
    update.member("level1")->toObject().setMember("value", ConfigValueNode(456));

    update.member("level1")->toObject().setMember("level2", ConfigObjectNode());
    update.nodeAtPath("level1/level2")->toObject().setMember("value", ConfigValueNode(789));

    // Apply the update
    node.apply(update);

    // Check the root level
    QVERIFY(node.isObject());

    QVERIFY(node.contains("null"));
    QVERIFY(node.member("null")->isValue());
    QCOMPARE(node.member("null")->toValue().value(), QJsonValue());

    QVERIFY(node.contains("value"));
    QVERIFY(node.member("value")->isValue());
    QCOMPARE(node.member("value")->toValue().value(), 123);

    QVERIFY(node.contains("valueToObject"));
    QVERIFY(node.member("valueToObject")->isObject());

    // Check the level 1
    QVERIFY(node.contains("level1"));
    QVERIFY(node.member("level1")->isObject());

    QVERIFY(node.member("level1")->toObject().contains("null"));
    QVERIFY(node.nodeAtPath("level1/null")->isValue());
    QCOMPARE(node.nodeAtPath("level1/null")->toValue().value(), QJsonValue());

    QVERIFY(node.nodeAtPath("level1/value")->isValue());
    QCOMPARE(node.nodeAtPath("level1/value")->toValue().value(), 456);

    // Check the level 2
    QVERIFY(node.member("level1")->toObject().contains("level2"));
    QVERIFY(node.nodeAtPath("level1/level2")->isObject());

    QVERIFY(node.nodeAtPath("level1/level2")->toObject().contains("null"));
    QVERIFY(node.nodeAtPath("level1/level2/null")->isValue());
    QCOMPARE(node.nodeAtPath("level1/level2/null")->toValue().value(), QJsonValue());

    QVERIFY(node.nodeAtPath("level1/level2")->toObject().contains("value"));
    QVERIFY(node.nodeAtPath("level1/level2/value")->isValue());
    QCOMPARE(node.nodeAtPath("level1/level2/value")->toValue().value(), 789);
}

// Test: DerivedObject node ------------------------------------------------------------------------

void TestConfigNode::testDerivedObjectNode()
{
    ConfigDerivedObjectNode derivedObject;

    QVERIFY(derivedObject.bases().isEmpty());
    QCOMPARE(derivedObject.config().count(), 0);

    QList<ConfigNodePath> bases = {
        ConfigNodePath("/aaa/bbb"),
        ConfigNodePath("ccc/ddd")
    };

    derivedObject.setBases(bases);
    QCOMPARE(derivedObject.bases(), bases);

    ConfigObjectNode config;
    config.setMember("a", ConfigValueNode(1));
    config.setMember("b", ConfigValueNode("str"));

    derivedObject.setConfig(config);
    QCOMPARE(derivedObject.config().count(), config.count());

    QVERIFY(derivedObject.config().contains("a"));
    QVERIFY(derivedObject.config().member("a")->isValue());
    QCOMPARE(derivedObject.config().member("a")->toValue().value(), 1);

    QVERIFY(derivedObject.config().contains("b"));
    QVERIFY(derivedObject.config().member("b")->isValue());
    QCOMPARE(derivedObject.config().member("b")->toValue().value(), QJsonValue("str"));
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigNode)
#include "testConfigNode.moc"
