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

#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
template<>
struct std::hash<QString> {
    std::size_t operator()(const QString &v) const noexcept
    {
        return qHash(v);
    }
};
#endif

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

    void testNodePath();

    void testObjectNode();
    void testApplyObject();

    void testDerivedObjectNode();

    void testEqualityOperatorsValue();
    void testEqualityOperatorsObject();
    void testEqualityOperatorsNodeReference();
    void testEqualityOperatorsDerivedObject();
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
    QCOMPARE(node1.value(), QJsonValue(1));

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
        QCOMPARE(movedNode.value(), QJsonValue(123));
    }

    // Move assignment operator
    {
        ConfigValueNode node(456, &parentNode2);

        ConfigValueNode movedNode;
        movedNode = std::move(node);
        QVERIFY(movedNode.isValue());
        QCOMPARE(movedNode.parent(), &parentNode2);
        QCOMPARE(movedNode.value(), QJsonValue(456));

        // Self assignment
        movedNode = std::move(movedNode);
        QVERIFY(movedNode.isValue());
        QCOMPARE(movedNode.parent(), &parentNode2);
        QCOMPARE(movedNode.value(), QJsonValue(456));
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
    QCOMPARE(clonedNode->toValue().value(), QJsonValue(123));

    // Clone with parent
    ConfigObjectNode parentNode;
    node.setParent(&parentNode);
    node.setValue(456);

    clonedNode = node.clone();
    QVERIFY(clonedNode->isValue());
    QCOMPARE(clonedNode->parent(), nullptr);
    QCOMPARE(clonedNode->toValue().value(), QJsonValue(456));
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
    QCOMPARE(clonedNode->toObject().member("item1")->toValue().value(), QJsonValue(1));

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
    QCOMPARE(clonedNode->toObject().member("item1")->toValue().value(), QJsonValue(2));

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
    QCOMPARE(level3Item1->toValue().value(), QJsonValue(1));
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
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    QCOMPARE(QSet<QString>::fromList(object.names()),
             QSet<QString>({"item1", "item2", "item3"}));
#else
    auto objectNames = object.names();
    QCOMPARE(QSet<QString>(objectNames.begin(), objectNames.end()),
             QSet<QString>({"item1", "item2", "item3"}));
#endif
    QVERIFY(object.member("item1")->isValue());
    QCOMPARE(object.member("item1")->toValue().value(), QJsonValue(true));
    QVERIFY(object.member("item2")->isValue());
    QCOMPARE(object.member("item2")->toValue().value(), QJsonValue(123));
    QVERIFY(object.member("item3")->isObject());

    QCOMPARE(object.member("item1"), objectConst.member("item1"));

    QCOMPARE(object.member("item9"), nullptr);
    QCOMPARE(objectConst.member("item9"), nullptr);

    object.setMember("item2", ConfigValueNode("str"));
    QCOMPARE(object.member("item2")->toValue().value(), QJsonValue("str"));

    QVERIFY(object.remove("item2"));
    QVERIFY(!object.remove("item9"));

    QCOMPARE(object.count(), 2);
#if QT_VERSION < QT_VERSION_CHECK(5, 14, 0)
    QCOMPARE(QSet<QString>::fromList(object.names()),
             QSet<QString>({"item1", "item3"}));
#else
    objectNames = object.names();
    QCOMPARE(QSet<QString>(objectNames.begin(), objectNames.end()),
             QSet<QString>({"item1", "item3"}));
#endif
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
    QCOMPARE(node.member("value")->toValue().value(), QJsonValue(123));

    QVERIFY(node.contains("valueToObject"));
    QVERIFY(node.member("valueToObject")->isObject());

    // Check the level 1
    QVERIFY(node.contains("level1"));
    QVERIFY(node.member("level1")->isObject());

    QVERIFY(node.member("level1")->toObject().contains("null"));
    QVERIFY(node.nodeAtPath("level1/null")->isValue());
    QCOMPARE(node.nodeAtPath("level1/null")->toValue().value(), QJsonValue());

    QVERIFY(node.nodeAtPath("level1/value")->isValue());
    QCOMPARE(node.nodeAtPath("level1/value")->toValue().value(), QJsonValue(456));

    // Check the level 2
    QVERIFY(node.member("level1")->toObject().contains("level2"));
    QVERIFY(node.nodeAtPath("level1/level2")->isObject());

    QVERIFY(node.nodeAtPath("level1/level2")->toObject().contains("null"));
    QVERIFY(node.nodeAtPath("level1/level2/null")->isValue());
    QCOMPARE(node.nodeAtPath("level1/level2/null")->toValue().value(), QJsonValue());

    QVERIFY(node.nodeAtPath("level1/level2")->toObject().contains("value"));
    QVERIFY(node.nodeAtPath("level1/level2/value")->isValue());
    QCOMPARE(node.nodeAtPath("level1/level2/value")->toValue().value(), QJsonValue(789));
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
    QCOMPARE(derivedObject.config().member("a")->toValue().value(), QJsonValue(1));

    QVERIFY(derivedObject.config().contains("b"));
    QVERIFY(derivedObject.config().member("b")->isValue());
    QCOMPARE(derivedObject.config().member("b")->toValue().value(), QJsonValue("str"));
}

// Test: Equality operators for Value node ---------------------------------------------------------

void TestConfigNode::testEqualityOperatorsValue()
{
    // Test with no parent
    const ConfigValueNode node1(123);
    const ConfigValueNode node2(123);
    const ConfigValueNode node3(100);

    QVERIFY(node1 == node2);
    QVERIFY(!(node1 != node2));

    QVERIFY(node1 != node3);
    QVERIFY(!(node1 == node3));

    // Test with same node path
    ConfigObjectNode root1;
    root1.setMember("test", node1.clone());

    ConfigObjectNode root2;
    root2.setMember("test", node2.clone());

    ConfigObjectNode root3;
    root3.setMember("test", node3.clone());

    QVERIFY(root1.member("test")->toValue() == root2.member("test")->toValue());
    QVERIFY(!(root1.member("test")->toValue() != root2.member("test")->toValue()));

    QVERIFY(root1.member("test")->toValue() != root3.member("test")->toValue());
    QVERIFY(!(root1.member("test")->toValue() == root3.member("test")->toValue()));

    // Test with other node path
    root2.setMember("other", node2.clone());
    root3.setMember("other", node3.clone());

    QVERIFY(root1.member("test")->toValue() != root2.member("other")->toValue());
    QVERIFY(!(root1.member("test")->toValue() == root2.member("other")->toValue()));

    QVERIFY(root1.member("test")->toValue() != root3.member("other")->toValue());
    QVERIFY(!(root1.member("test")->toValue() == root3.member("other")->toValue()));
}

// Test: Equality operators for Object node --------------------------------------------------------

void TestConfigNode::testEqualityOperatorsObject()
{
    // Test with no parent
    ConfigValueNode childValueNode1(123);
    ConfigValueNode childValueNode2(100);

    ConfigObjectNode childObjectNode1;
    childObjectNode1.setMember("value", ConfigValueNode(123));

    ConfigObjectNode childObjectNode2;
    childObjectNode2.setMember("value", ConfigValueNode(100));

    ConfigNodeReference childNodeReference1(ConfigNodePath("/ref1"));
    ConfigNodeReference childNodeReference2(ConfigNodePath("/ref2"));

    ConfigDerivedObjectNode childDerivedObjectNode1({}, childObjectNode1);
    ConfigDerivedObjectNode childDerivedObjectNode2({}, childObjectNode2);

    ConfigObjectNode node11;
    node11.setMember("value", childValueNode1.clone());

    ConfigObjectNode node12;
    node12.setMember("value", childValueNode1.clone());

    ConfigObjectNode node13;
    node13.setMember("value", childValueNode2.clone());

    ConfigObjectNode node14;

    ConfigObjectNode node15;
    node15.setMember("value", childObjectNode1.clone());

    ConfigObjectNode node16;
    node16.setMember("xyz", childValueNode1.clone());

    ConfigObjectNode node17;
    node17.setMember("value", childValueNode1.clone());
    node17.setMember("xyz", childValueNode1.clone());

    ConfigObjectNode node21;
    node21.setMember("object", childObjectNode1.clone());

    ConfigObjectNode node22;
    node22.setMember("object", childObjectNode1.clone());

    ConfigObjectNode node23;
    node23.setMember("object", childObjectNode2.clone());

    ConfigObjectNode node31;
    node31.setMember("ref", childNodeReference1.clone());

    ConfigObjectNode node32;
    node32.setMember("ref", childNodeReference1.clone());

    ConfigObjectNode node33;
    node33.setMember("ref", childNodeReference2.clone());

    ConfigObjectNode node41;
    node41.setMember("derived", childDerivedObjectNode1.clone());

    ConfigObjectNode node42;
    node42.setMember("derived", childDerivedObjectNode1.clone());

    ConfigObjectNode node43;
    node43.setMember("derived", childDerivedObjectNode2.clone());

    QVERIFY(node11 == node12);
    QVERIFY(!(node11 != node12));

    QVERIFY(node11 != node13);
    QVERIFY(!(node11 == node13));

    QVERIFY(node11 != node14);
    QVERIFY(!(node11 == node14));

    QVERIFY(node11 != node15);
    QVERIFY(!(node11 == node15));

    QVERIFY(node11 != node16);
    QVERIFY(!(node11 == node16));

    QVERIFY(node11 != node17);
    QVERIFY(!(node11 == node17));

    QVERIFY(node21 == node22);
    QVERIFY(!(node21 != node22));

    QVERIFY(node21 != node23);
    QVERIFY(!(node21 == node23));

    QVERIFY(node31 == node32);
    QVERIFY(!(node31 != node32));

    QVERIFY(node31 != node33);
    QVERIFY(!(node31 == node33));

    QVERIFY(node41 == node42);
    QVERIFY(!(node41 != node42));

    QVERIFY(node41 != node43);
    QVERIFY(!(node41 == node43));

    // Test with same node path
    ConfigObjectNode root1;
    root1.setMember("test", node11.clone());

    ConfigObjectNode root2;
    root2.setMember("test", node12.clone());

    ConfigObjectNode root3;
    root3.setMember("test", node13.clone());

    QVERIFY(root1.member("test")->toObject() == root2.member("test")->toObject());
    QVERIFY(!(root1.member("test")->toObject() != root2.member("test")->toObject()));

    QVERIFY(root1.member("test")->toObject() != root3.member("test")->toObject());
    QVERIFY(!(root1.member("test")->toObject() == root3.member("test")->toObject()));

    // Test with other node path
    root2.setMember("other", node12.clone());
    root3.setMember("other", node13.clone());

    QVERIFY(root1.member("test")->toObject() != root2.member("other")->toObject());
    QVERIFY(!(root1.member("test")->toObject() == root2.member("other")->toObject()));

    QVERIFY(root1.member("test")->toObject() != root3.member("other")->toObject());
    QVERIFY(!(root1.member("test")->toObject() == root3.member("other")->toObject()));
}

// Test: Equality operators for NodeReference node -------------------------------------------------

void TestConfigNode::testEqualityOperatorsNodeReference()
{
    // Test with no parent
    const ConfigNodeReference node1(ConfigNodePath("/refA"));
    const ConfigNodeReference node2(ConfigNodePath("/refA"));
    const ConfigNodeReference node3(ConfigNodePath("/refB"));

    QVERIFY(node1 == node2);
    QVERIFY(!(node1 != node2));

    QVERIFY(node1 != node3);
    QVERIFY(!(node1 == node3));

    // Test with same node path
    ConfigObjectNode root1;
    root1.setMember("test", node1.clone());

    ConfigObjectNode root2;
    root2.setMember("test", node2.clone());

    ConfigObjectNode root3;
    root3.setMember("test", node3.clone());

    QVERIFY(root1.member("test")->toNodeReference() == root2.member("test")->toNodeReference());
    QVERIFY(!(root1.member("test")->toNodeReference() != root2.member("test")->toNodeReference()));

    QVERIFY(root1.member("test")->toNodeReference() != root3.member("test")->toNodeReference());
    QVERIFY(!(root1.member("test")->toNodeReference() == root3.member("test")->toNodeReference()));

    // Test with other node path
    root2.setMember("other", node2.clone());
    root3.setMember("other", node3.clone());

    QVERIFY(root1.member("test")->toNodeReference() != root2.member("other")->toNodeReference());
    QVERIFY(!(root1.member("test")->toNodeReference() == root2.member("other")->toNodeReference()));

    QVERIFY(root1.member("test")->toNodeReference() != root3.member("other")->toNodeReference());
    QVERIFY(!(root1.member("test")->toNodeReference() == root3.member("other")->toNodeReference()));
}

// Test: Equality operators for DerivedObject node -------------------------------------------------

void TestConfigNode::testEqualityOperatorsDerivedObject()
{
    // Test with no parent
    ConfigObjectNode childObjectNode1;
    childObjectNode1.setMember("value", ConfigValueNode(123));

    ConfigObjectNode childObjectNode2;
    childObjectNode2.setMember("value", ConfigValueNode(100));

    ConfigDerivedObjectNode node11({ConfigNodePath("/base")});
    ConfigDerivedObjectNode node12({ConfigNodePath("/base")});
    ConfigDerivedObjectNode node13({ConfigNodePath("/xyz")});

    ConfigDerivedObjectNode node21({}, childObjectNode1);
    ConfigDerivedObjectNode node22({}, childObjectNode1);
    ConfigDerivedObjectNode node23({}, childObjectNode2);

    ConfigDerivedObjectNode node31({ConfigNodePath("/base")}, childObjectNode1);
    ConfigDerivedObjectNode node32({ConfigNodePath("/base")}, childObjectNode1);
    ConfigDerivedObjectNode node33({ConfigNodePath("/xyz")}, childObjectNode2);

    QVERIFY(node11 == node12);
    QVERIFY(!(node11 != node12));

    QVERIFY(node11 != node13);
    QVERIFY(!(node11 == node13));

    QVERIFY(node21 == node22);
    QVERIFY(!(node21 != node22));

    QVERIFY(node21 != node23);
    QVERIFY(!(node21 == node23));

    QVERIFY(node31 == node32);
    QVERIFY(!(node31 != node32));

    QVERIFY(node31 != node33);
    QVERIFY(!(node31 == node33));

    // Test with same node path
    ConfigObjectNode root1;
    root1.setMember("test", node11.clone());

    ConfigObjectNode root2;
    root2.setMember("test", node12.clone());

    ConfigObjectNode root3;
    root3.setMember("test", node13.clone());

    QVERIFY(root1.member("test")->toDerivedObject() == root2.member("test")->toDerivedObject());
    QVERIFY(!(root1.member("test")->toDerivedObject() != root2.member("test")->toDerivedObject()));

    QVERIFY(root1.member("test")->toDerivedObject() != root3.member("test")->toDerivedObject());
    QVERIFY(!(root1.member("test")->toDerivedObject() == root3.member("test")->toDerivedObject()));

    // Test with other node path
    root2.setMember("other", node12.clone());
    root3.setMember("other", node13.clone());

    QVERIFY(root1.member("test")->toDerivedObject() != root2.member("other")->toDerivedObject());
    QVERIFY(!(root1.member("test")->toDerivedObject() == root2.member("other")->toDerivedObject()));

    QVERIFY(root1.member("test")->toDerivedObject() != root3.member("other")->toDerivedObject());
    QVERIFY(!(root1.member("test")->toDerivedObject() == root3.member("other")->toDerivedObject()));
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigNode)
#include "testConfigNode.moc"
