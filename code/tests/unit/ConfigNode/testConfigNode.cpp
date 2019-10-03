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

    void testMove();

private:
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
    QCOMPARE(node.type(), ConfigNode::Type::Null);
    QCOMPARE(node.parent(), nullptr);
}

// Test: Constructor -------------------------------------------------------------------------------

void TestConfigNode::testConstructor()
{
    QFETCH(ConfigNode::Type, nodeType);

    ConfigNode nodeWithoutParent(nodeType);
    QCOMPARE(nodeWithoutParent.type(), nodeType);
    QCOMPARE(nodeWithoutParent.parent(), nullptr);

    ConfigNode nodeWithParent(nodeType, &nodeWithoutParent);
    QCOMPARE(nodeWithParent.type(), nodeType);
    QCOMPARE(nodeWithParent.parent(), &nodeWithoutParent);
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

void TestConfigNode::testMove()
{
    ConfigNode parentNode;

    // Move constructor
    ConfigNode node(ConfigNode::Type::Value, &parentNode);
    node.setValue(123);

    ConfigNode movedNode(std::move(node));

    QCOMPARE(movedNode.type(), ConfigNode::Type::Value);
    QCOMPARE(movedNode.parent(), &parentNode);
    QCOMPARE(movedNode.value(), QVariant(123));

    // Move assignement operator
    node = ConfigNode(ConfigNode::Type::Array, &parentNode);
    node.appendElement(ConfigNode(ConfigNode::Type::Null));
    node.appendElement(ConfigNode(ConfigNode::Type::Value));

    movedNode = std::move(node);

    QCOMPARE(movedNode.type(), ConfigNode::Type::Array);
    QCOMPARE(movedNode.parent(), &parentNode);
    QCOMPARE(movedNode.count(), 2);

    QCOMPARE(movedNode.element(0)->type(), ConfigNode::Type::Null);
    QCOMPARE(movedNode.element(0)->parent(), &movedNode);

    QCOMPARE(movedNode.element(1)->type(), ConfigNode::Type::Value);
    QCOMPARE(movedNode.element(1)->parent(), &movedNode);
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigNode)
#include "testConfigNode.moc"
