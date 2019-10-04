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
#include <CppConfigFramework/ConfigNode.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

Q_DECLARE_METATYPE(CppConfigFramework::ConfigNode::Type);

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;

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
    void testDefaultConstructor();

    void testConstructor();
    void testConstructor_data();

    void testMoveNull();
    void testMoveValue();
    void testMoveArray();
    void testMoveObject();

    void testCloneNull();
    void testCloneValue();
    void testCloneArray();
    void testCloneObject();

    void testNodePath();

    void testApplyNode();
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

// Test: Default constructor -----------------------------------------------------------------------

void TestConfigNode::testDefaultConstructor()
{
    ConfigNode node;
    QVERIFY(node.isNull());
    QCOMPARE(node.parent(), nullptr);
}

// Test: Constructor -------------------------------------------------------------------------------

void TestConfigNode::testConstructor()
{
    QFETCH(ConfigNode::Type, nodeType);

    ConfigNode nodeWithoutParent(nodeType);
    QCOMPARE(nodeWithoutParent.type(), nodeType);
    QCOMPARE(nodeWithoutParent.parent(), nullptr);

    ConfigNode parent(ConfigNode::Type::Object);
    ConfigNode nodeWithParent(nodeType, &parent);
    QCOMPARE(nodeWithParent.type(), nodeType);
    QCOMPARE(nodeWithParent.parent(), &parent);
}

void TestConfigNode::testConstructor_data()
{
    QTest::addColumn<ConfigNode::Type>("nodeType");

    QTest::newRow("Null")   << ConfigNode::Type::Null;
    QTest::newRow("Value")  << ConfigNode::Type::Value;
    QTest::newRow("Array")  << ConfigNode::Type::Array;
    QTest::newRow("Object") << ConfigNode::Type::Object;
}

// Test: Move constructor and move assignment operator ---------------------------------------------

void TestConfigNode::testMoveNull()
{
    ConfigNode parentNode1(ConfigNode::Type::Object);
    ConfigNode parentNode2(ConfigNode::Type::Object);

    // Move constructor
    ConfigNode movedNode(ConfigNode(ConfigNode::Type::Null, &parentNode1));
    QVERIFY(movedNode.isNull());
    QCOMPARE(movedNode.parent(), &parentNode1);

    // Move assignement operator
    movedNode = ConfigNode(ConfigNode::Type::Null, &parentNode2);
    QVERIFY(movedNode.isNull());
    QCOMPARE(movedNode.parent(), &parentNode2);
}

void TestConfigNode::testMoveValue()
{
    ConfigNode parentNode1(ConfigNode::Type::Object);
    ConfigNode parentNode2(ConfigNode::Type::Object);

    // Move constructor
    {
        ConfigNode node(ConfigNode::Type::Value, &parentNode1);
        node.setValue(123);

        ConfigNode movedNode(std::move(node));
        QVERIFY(movedNode.isValue());
        QCOMPARE(movedNode.parent(), &parentNode1);
        QCOMPARE(movedNode.value(), QVariant(123));
    }

    // Move assignement operator
    {
        ConfigNode node(ConfigNode::Type::Value, &parentNode2);
        node.setValue(456);

        ConfigNode movedNode;
        movedNode = std::move(node);
        QVERIFY(movedNode.isValue());
        QCOMPARE(movedNode.parent(), &parentNode2);
        QCOMPARE(movedNode.value(), QVariant(456));
    }
}

void TestConfigNode::testMoveArray()
{
    ConfigNode parentNode1(ConfigNode::Type::Object);
    ConfigNode parentNode2(ConfigNode::Type::Object);

    // Move constructor
    {
        ConfigNode node(ConfigNode::Type::Array, &parentNode1);
        node.appendElement(ConfigNode(ConfigNode::Type::Null));
        node.appendElement(ConfigNode(ConfigNode::Type::Value));

        ConfigNode movedNode(std::move(node));
        QVERIFY(movedNode.isArray());
        QCOMPARE(movedNode.parent(), &parentNode1);
        QCOMPARE(movedNode.count(), 2);

        QCOMPARE(movedNode.element(0)->type(), ConfigNode::Type::Null);
        QCOMPARE(movedNode.element(0)->parent(), &movedNode);

        QCOMPARE(movedNode.element(1)->type(), ConfigNode::Type::Value);
        QCOMPARE(movedNode.element(1)->parent(), &movedNode);
    }

    // Move assignement operator
    {
        ConfigNode node(ConfigNode::Type::Array, &parentNode1);
        node.appendElement(ConfigNode(ConfigNode::Type::Array));
        node.appendElement(ConfigNode(ConfigNode::Type::Object));

        ConfigNode movedNode;
        movedNode = std::move(node);
        QVERIFY(movedNode.isArray());
        QCOMPARE(movedNode.parent(), &parentNode1);
        QCOMPARE(movedNode.count(), 2);

        QCOMPARE(movedNode.element(0)->type(), ConfigNode::Type::Array);
        QCOMPARE(movedNode.element(0)->parent(), &movedNode);

        QCOMPARE(movedNode.element(1)->type(), ConfigNode::Type::Object);
        QCOMPARE(movedNode.element(1)->parent(), &movedNode);
    }
}

void TestConfigNode::testMoveObject()
{
    ConfigNode parentNode1(ConfigNode::Type::Object);
    ConfigNode parentNode2(ConfigNode::Type::Object);

    // Move constructor
    {
        ConfigNode node(ConfigNode::Type::Object, &parentNode1);
        node.setMember("item1", ConfigNode(ConfigNode::Type::Null));
        node.setMember("item2", ConfigNode(ConfigNode::Type::Value));

        ConfigNode movedNode(std::move(node));
        QVERIFY(movedNode.isObject());
        QCOMPARE(movedNode.parent(), &parentNode1);
        QCOMPARE(movedNode.count(), 2);

        QCOMPARE(movedNode.member("item1")->type(), ConfigNode::Type::Null);
        QCOMPARE(movedNode.member("item1")->parent(), &movedNode);

        QCOMPARE(movedNode.member("item2")->type(), ConfigNode::Type::Value);
        QCOMPARE(movedNode.member("item2")->parent(), &movedNode);
    }

    // Move assignement operator
    {
        ConfigNode node(ConfigNode::Type::Object, &parentNode1);
        node.setMember("item1", ConfigNode(ConfigNode::Type::Null));
        node.setMember("item2", ConfigNode(ConfigNode::Type::Value));

        ConfigNode movedNode;
        movedNode = std::move(node);
        QVERIFY(movedNode.isObject());
        QCOMPARE(movedNode.parent(), &parentNode1);
        QCOMPARE(movedNode.count(), 2);

        QCOMPARE(movedNode.member("item1")->type(), ConfigNode::Type::Null);
        QCOMPARE(movedNode.member("item1")->parent(), &movedNode);

        QCOMPARE(movedNode.member("item2")->type(), ConfigNode::Type::Value);
        QCOMPARE(movedNode.member("item2")->parent(), &movedNode);
    }
}

// Test: clone() method ----------------------------------------------------------------------------

void TestConfigNode::testCloneNull()
{
    // Clone without parent
    ConfigNode node(ConfigNode::Type::Null, nullptr);

    ConfigNode clonedNode(node.clone());
    QVERIFY(clonedNode.isNull());
    QCOMPARE(clonedNode.parent(), nullptr);

    // Clone with parent
    ConfigNode parentNode(ConfigNode::Type::Object);
    node.setParent(&parentNode);

    clonedNode = node.clone();
    QVERIFY(clonedNode.isNull());
    QCOMPARE(clonedNode.parent(), nullptr);
}

void TestConfigNode::testCloneValue()
{
    // Clone without parent
    ConfigNode node(ConfigNode::Type::Value, nullptr);
    node.setValue(123);

    ConfigNode clonedNode(node.clone());
    QVERIFY(clonedNode.isValue());
    QCOMPARE(clonedNode.parent(), nullptr);
    QCOMPARE(clonedNode.value(), QVariant(123));

    // Clone with parent
    ConfigNode parentNode(ConfigNode::Type::Object);
    node.setParent(&parentNode);
    node.setValue(456);

    clonedNode = node.clone();
    QVERIFY(clonedNode.isValue());
    QCOMPARE(clonedNode.parent(), nullptr);
    QCOMPARE(clonedNode.value(), QVariant(456));
}

void TestConfigNode::testCloneArray()
{
    // Clone without parent
    ConfigNode node(ConfigNode::Type::Array, nullptr);
    node.appendElement(ConfigNode(ConfigNode::Type::Null));
    node.appendElement(ConfigNode(ConfigNode::Type::Value));

    ConfigNode clonedNode(node.clone());
    QVERIFY(clonedNode.isArray());
    QCOMPARE(clonedNode.parent(), nullptr);
    QCOMPARE(clonedNode.count(), 2);

    QCOMPARE(clonedNode.element(0)->type(), ConfigNode::Type::Null);
    QCOMPARE(clonedNode.element(0)->parent(), &clonedNode);

    QCOMPARE(clonedNode.element(1)->type(), ConfigNode::Type::Value);
    QCOMPARE(clonedNode.element(1)->parent(), &clonedNode);

    // Clone with parent
    ConfigNode parentNode(ConfigNode::Type::Object);
    node.setParent(&parentNode);
    node.removeAll();
    node.appendElement(ConfigNode(ConfigNode::Type::Array));
    node.appendElement(ConfigNode(ConfigNode::Type::Object));

    clonedNode = node.clone();
    QVERIFY(clonedNode.isArray());
    QCOMPARE(clonedNode.parent(), nullptr);
    QCOMPARE(clonedNode.count(), 2);

    QCOMPARE(clonedNode.element(0)->type(), ConfigNode::Type::Array);
    QCOMPARE(clonedNode.element(0)->parent(), &clonedNode);

    QCOMPARE(clonedNode.element(1)->type(), ConfigNode::Type::Object);
    QCOMPARE(clonedNode.element(1)->parent(), &clonedNode);
}

void TestConfigNode::testCloneObject()
{
    // Clone without parent
    ConfigNode node(ConfigNode::Type::Object, nullptr);
    node.setMember("item1", ConfigNode(ConfigNode::Type::Null));
    node.setMember("item2", ConfigNode(ConfigNode::Type::Value));

    ConfigNode clonedNode(node.clone());
    QVERIFY(clonedNode.isObject());
    QCOMPARE(clonedNode.parent(), nullptr);
    QCOMPARE(clonedNode.count(), 2);

    QCOMPARE(clonedNode.member("item1")->type(), ConfigNode::Type::Null);
    QCOMPARE(clonedNode.member("item1")->parent(), &clonedNode);

    QCOMPARE(clonedNode.member("item2")->type(), ConfigNode::Type::Value);
    QCOMPARE(clonedNode.member("item2")->parent(), &clonedNode);

    // Clone with parent
    ConfigNode parentNode(ConfigNode::Type::Object);
    node.setParent(&parentNode);
    node.removeAll();
    node.setMember("item1", ConfigNode(ConfigNode::Type::Array));
    node.setMember("item2", ConfigNode(ConfigNode::Type::Object));

    clonedNode = node.clone();
    QVERIFY(clonedNode.isObject());
    QCOMPARE(clonedNode.parent(), nullptr);
    QCOMPARE(clonedNode.count(), 2);

    QCOMPARE(clonedNode.member("item1")->type(), ConfigNode::Type::Array);
    QCOMPARE(clonedNode.member("item1")->parent(), &clonedNode);

    QCOMPARE(clonedNode.member("item2")->type(), ConfigNode::Type::Object);
    QCOMPARE(clonedNode.member("item2")->parent(), &clonedNode);
}

// Test: rootNode() method -------------------------------------------------------------------------

void TestConfigNode::testNodePath()
{
    // Root node
    ConfigNode rootNode(ConfigNode::Type::Object);

    QVERIFY(rootNode.isRoot());
    QCOMPARE(rootNode.rootNode(), &rootNode);
    QCOMPARE(rootNode.absoluteNodePath(), "/");
    QCOMPARE(rootNode.nodeAtPath("/"), &rootNode);

    // Level 1
    rootNode.setMember("level1", ConfigNode(ConfigNode::Type::Object));
    ConfigNode *level1 = rootNode.member("level1");
    QVERIFY(level1 != nullptr);
    QCOMPARE(level1->rootNode(), &rootNode);
    QCOMPARE(level1->absoluteNodePath(), "/level1");
    QCOMPARE(rootNode.nodeAtPath("/level1"), level1);

    // Level 2
    level1->setMember("level2", ConfigNode(ConfigNode::Type::Object));
    ConfigNode *level2 = level1->member("level2");
    QVERIFY(level2 != nullptr);
    QCOMPARE(level2->rootNode(), &rootNode);
    QCOMPARE(level2->absoluteNodePath(), "/level1/level2");
    QCOMPARE(rootNode.nodeAtPath("/level1/level2"), level2);

    // Level 3
    level2->setMember("level3", ConfigNode(ConfigNode::Type::Array));
    ConfigNode *level3 = level2->member("level3");
    QVERIFY(level3 != nullptr);
    QCOMPARE(level3->rootNode(), &rootNode);
    QCOMPARE(level3->absoluteNodePath(), "/level1/level2/level3");
    QCOMPARE(rootNode.nodeAtPath("/level1/level2/level3"), level3);

    // Level 3 elements
    level3->appendElement(ConfigNode(ConfigNode::Type::Null));
    ConfigNode *level3Element0 = level3->element(0);
    QVERIFY(level3Element0 != nullptr);
    QCOMPARE(level3Element0->rootNode(), &rootNode);
    QCOMPARE(level3Element0->absoluteNodePath(), "/level1/level2/level3/0");
    QCOMPARE(level3Element0, rootNode.nodeAtPath("/level1/level2/level3/0"));
    QCOMPARE(rootNode.nodeAtPath("/level1/level2/level3/0"), level3Element0);

    level3->appendElement(ConfigNode(ConfigNode::Type::Value));
    ConfigNode *level3Element1 = level3->element(1);
    QVERIFY(level3Element1 != nullptr);
    QCOMPARE(level3Element1->rootNode(), &rootNode);
    QCOMPARE(level3Element1->absoluteNodePath(), "/level1/level2/level3/1");
    QCOMPARE(rootNode.nodeAtPath("/level1/level2/level3/1"), level3Element1);

    // Test absolute paths on non-root items
    QCOMPARE(level3Element0->nodeAtPath("/"), &rootNode);
    QCOMPARE(level3Element1->nodeAtPath("/"), &rootNode);

    QCOMPARE(level3Element0->nodeAtPath("/level1"), level1);
    QCOMPARE(level3Element1->nodeAtPath("/level1/level2"), level2);

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

    QCOMPARE(level3Element1->nodeAtPath("../0"), level3Element0);
}

// Test: apply() method ----------------------------------------------------------------------------

void TestConfigNode::testApplyNode()
{
    // Create 3 level node structure
    ConfigNode node(ConfigNode::Type::Object);
    node.setMember("value", ConfigNode(ConfigNode::Type::Value));
    node.member("value")->setValue(111);
    node.setMember("array", ConfigNode(ConfigNode::Type::Array));
    node.member("array")->appendElement(ConfigNode(ConfigNode::Type::Value));
    node.member("array")->appendElement(ConfigNode(ConfigNode::Type::Value));

    node.setMember("level1", ConfigNode(ConfigNode::Type::Object));

    node.member("level1")->setMember("level2", ConfigNode(ConfigNode::Type::Object));
    node.nodeAtPath("level1/level2")->setMember("null", ConfigNode());
    node.nodeAtPath("level1/level2")->setMember("value", ConfigNode());

    // Create a compatible node structure for the update
    ConfigNode update(ConfigNode::Type::Object);
    update.setMember("null", ConfigNode());
    update.setMember("value", ConfigNode(ConfigNode::Type::Value));
    update.member("value")->setValue(123);
    update.setMember("array", ConfigNode(ConfigNode::Type::Array));
    update.member("array")->appendElement(ConfigNode(ConfigNode::Type::Object));

    update.setMember("level1", ConfigNode(ConfigNode::Type::Object));
    update.member("level1")->setMember("null", ConfigNode());
    update.member("level1")->setMember("value", ConfigNode(ConfigNode::Type::Value));
    update.nodeAtPath("level1/value")->setValue(456);

    update.member("level1")->setMember("level2", ConfigNode(ConfigNode::Type::Object));
    update.nodeAtPath("level1/level2")->setMember("null", ConfigNode());
    update.nodeAtPath("level1/level2")->setMember("value", ConfigNode(ConfigNode::Type::Value));
    update.nodeAtPath("level1/level2/value")->setValue(789);

    // Apply the update
    QVERIFY(node.apply(update));

    // Check the root level
    QVERIFY(node.isObject());

    QVERIFY(node.containsMember("null"));
    QVERIFY(node.member("null")->isNull());

    QVERIFY(node.containsMember("value"));
    QVERIFY(node.member("value")->isValue());
    QCOMPARE(node.member("value")->value(), QVariant(123));

    QVERIFY(node.containsMember("array"));
    QVERIFY(node.member("array")->isArray());
    QCOMPARE(node.member("array")->count(), 1);
    QVERIFY(node.member("array")->element(0)->isObject());
    QCOMPARE(node.member("array")->element(0)->count(), 0);

    // Check the level 1
    QVERIFY(node.containsMember("level1"));
    QVERIFY(node.member("level1")->isObject());

    QVERIFY(node.member("level1")->containsMember("null"));
    QVERIFY(node.nodeAtPath("level1/null")->isNull());

    QVERIFY(node.nodeAtPath("level1/value")->isValue());
    QCOMPARE(node.nodeAtPath("level1/value")->value(), QVariant(456));

    // Check the level 2
    QVERIFY(node.member("level1")->containsMember("level2"));
    QVERIFY(node.nodeAtPath("level1/level2")->isObject());

    QVERIFY(node.nodeAtPath("level1/level2")->containsMember("null"));
    QVERIFY(node.nodeAtPath("level1/level2/null")->isNull());

    QVERIFY(node.nodeAtPath("level1/level2")->containsMember("value"));
    QVERIFY(node.nodeAtPath("level1/level2/value")->isValue());
    QCOMPARE(node.nodeAtPath("level1/level2/value")->value(), QVariant(789));
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigNode)
#include "testConfigNode.moc"
