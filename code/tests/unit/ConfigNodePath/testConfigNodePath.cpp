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
 * Contains unit tests for ConfigNodePath class
 */

// C++ Config Framework includes
#include <CppConfigFramework/ConfigNodePath.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;

class TestConfigNodePath : public QObject
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
    void testCopy();
    void testMove();
    void testCompare();

    void testIsRoot();
    void testIsRoot_data();

    void testIsAbsolute();
    void testIsAbsolute_data();

    void testIsRelative();
    void testIsRelative_data();

    void testIsValid();
    void testIsValid_data();

    void testHasUnresolvedReferences();
    void testHasUnresolvedReferences_data();

    void testResolveReferences();
    void testResolveReferences_data();

    void testNodeNames();

    void testToAbsolute();
    void testToAbsolute_data();

    void testAppendNodeName();
    void testAppendNodeName_data();

    void testAppendNodePath();
    void testAppendNodePath_data();
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestConfigNodePath::initTestCase()
{
}

void TestConfigNodePath::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestConfigNodePath::init()
{
}

void TestConfigNodePath::cleanup()
{
}

// Test: Default constructor -----------------------------------------------------------------------

void TestConfigNodePath::testDefaultConstructor()
{
    ConfigNodePath nodePath;
    QVERIFY(!nodePath.isValid());
}

// Test: Constructor -------------------------------------------------------------------------------

void TestConfigNodePath::testConstructor()
{
    const QString nodePath("/node/path");

    QCOMPARE(ConfigNodePath(nodePath).path(),          nodePath);
    QCOMPARE(ConfigNodePath(QString(nodePath)).path(), nodePath);
}

// Test: Copy constructor and copy assignment operator ---------------------------------------------

void TestConfigNodePath::testCopy()
{
    const ConfigNodePath nodePath1("/node/path1");

    // Copy constructor
    ConfigNodePath movedNodePath((ConfigNodePath(nodePath1)));
    QCOMPARE(movedNodePath.path(), nodePath1.path());

    // Copy assignment operator
    const ConfigNodePath nodePath2("/node/path2");
    movedNodePath = ConfigNodePath(nodePath2);
    QCOMPARE(movedNodePath.path(), nodePath2.path());
}

// Test: Move constructor and move assignment operator ---------------------------------------------

void TestConfigNodePath::testMove()
{
    const QString nodePath1("/node/path1");

    // Move constructor
    ConfigNodePath movedNodePath((ConfigNodePath(nodePath1)));
    QCOMPARE(movedNodePath.path(), nodePath1);

    // Move assignment operator
    const QString nodePath2("/node/path2");
    movedNodePath = ConfigNodePath(nodePath2);
    QCOMPARE(movedNodePath.path(), nodePath2);
}

// Test: Equality operators ------------------------------------------------------------------------

void TestConfigNodePath::testCompare()
{
    const ConfigNodePath nodePath1a("/node/path1");
    const ConfigNodePath nodePath1b("/node/path1");
    const ConfigNodePath nodePath2("/node/path2");

    // Compare with self
    QCOMPARE((nodePath1a == nodePath1a), true);
    QCOMPARE((nodePath1b == nodePath1b), true);
    QCOMPARE((nodePath2  == nodePath2),  true);

    QCOMPARE((nodePath1a != nodePath1a), false);
    QCOMPARE((nodePath1b != nodePath1b), false);
    QCOMPARE((nodePath2  != nodePath2),  false);

    // Compare with same value
    QCOMPARE((nodePath1a == nodePath1b), true);
    QCOMPARE((nodePath1b == nodePath1a), true);

    QCOMPARE((nodePath1a != nodePath1b), false);
    QCOMPARE((nodePath1b != nodePath1a), false);

    // Compare with different value
    QCOMPARE((nodePath1a == nodePath2),  false);
    QCOMPARE((nodePath1b == nodePath2),  false);
    QCOMPARE((nodePath2  == nodePath1a), false);
    QCOMPARE((nodePath2  == nodePath1b), false);

    QCOMPARE((nodePath1a != nodePath2),  true);
    QCOMPARE((nodePath1b != nodePath2),  true);
    QCOMPARE((nodePath2  != nodePath1a), true);
    QCOMPARE((nodePath2  != nodePath1b), true);
}

// Test: isRoot() method ---------------------------------------------------------------------------

void TestConfigNodePath::testIsRoot()
{
    QFETCH(QString, nodePath);
    QFETCH(bool, expectedResult);

    QCOMPARE(ConfigNodePath(nodePath).isRoot(), expectedResult);
}

void TestConfigNodePath::testIsRoot_data()
{
    QTest::addColumn<QString>("nodePath");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Root path") << "/" << true;

    QTest::newRow("Absolute path") << "/asd" << false;
    QTest::newRow("Relative path") << "asd" << false;
    QTest::newRow("Parent reference") << ".." << false;
    QTest::newRow("Invalid absolute path") << "/0asd" << false;
    QTest::newRow("Invalid relative path") << "0asd" << false;
    QTest::newRow("Empty path") << "" << false;
}

// Test: isAbsolute() method -----------------------------------------------------------------------

void TestConfigNodePath::testIsAbsolute()
{
    QFETCH(QString, nodePath);
    QFETCH(bool, expectedResult);

    QCOMPARE(ConfigNodePath(nodePath).isAbsolute(), expectedResult);
}

void TestConfigNodePath::testIsAbsolute_data()
{
    QTest::addColumn<QString>("nodePath");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Root path") << "/" << true;
    QTest::newRow("Absolute path") << "/asd" << true;
    QTest::newRow("Invalid absolute path") << "/0asd" << true;

    QTest::newRow("Relative path") << "asd" << false;
    QTest::newRow("Parent reference") << ".." << false;
    QTest::newRow("Invalid relative path") << "0asd" << false;
    QTest::newRow("Empty path") << "" << false;
}

// Test: isRelative() method -----------------------------------------------------------------------

void TestConfigNodePath::testIsRelative()
{
    QFETCH(QString, nodePath);
    QFETCH(bool, expectedResult);

    QCOMPARE(ConfigNodePath(nodePath).isRelative(), expectedResult);
}

void TestConfigNodePath::testIsRelative_data()
{
    QTest::addColumn<QString>("nodePath");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Relative path") << "asd" << true;
    QTest::newRow("Parent reference") << ".." << true;
    QTest::newRow("Invalid relative path") << "0asd" << true;

    QTest::newRow("Root path") << "/" << false;
    QTest::newRow("Absolute path") << "/asd" << false;
    QTest::newRow("Invalid absolute path") << "/0asd" << false;
    QTest::newRow("Empty path") << "" << false;
}

// Test: isValid() method --------------------------------------------------------------------------

void TestConfigNodePath::testIsValid()
{
    QFETCH(QString, nodePath);
    QFETCH(bool, expectedResult);

    QCOMPARE(ConfigNodePath(nodePath).isValid(), expectedResult);
}

void TestConfigNodePath::testIsValid_data()
{
    QTest::addColumn<QString>("nodePath");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Root path") << "/" << true;
    QTest::newRow("Absolute path") << "/asd" << true;
    QTest::newRow("Relative path") << "asd" << true;
    QTest::newRow("Parent reference") << ".." << true;
    QTest::newRow("Absolute path with parent ref") << "/asd/aaa/bbb/../../ccc" << true;
    QTest::newRow("Relative path with parent ref") << "asd/aaa/bbb/../../ccc" << true;
    QTest::newRow("Relative path with initial parent ref") << "../asd/../../ccc" << true;

    QTest::newRow("Empty path") << "" << false;
    QTest::newRow("Invalid absolute path") << "/0asd" << false;
    QTest::newRow("Invalid relative path") << "0asd" << false;
    QTest::newRow("Absolute path with following slash") << "/asd/" << false;
    QTest::newRow("Relative path with following slash") << "asd/" << false;
    QTest::newRow("Absolute path with parent ref before root") << "/asd/../../ccc" << false;
}

// Test: hasUnresolvedReferences() method ----------------------------------------------------------

void TestConfigNodePath::testHasUnresolvedReferences()
{
    QFETCH(QString, nodePath);
    QFETCH(bool, expectedResult);

    QCOMPARE(ConfigNodePath(nodePath).hasUnresolvedReferences(), expectedResult);
}

void TestConfigNodePath::testHasUnresolvedReferences_data()
{
    QTest::addColumn<QString>("nodePath");
    QTest::addColumn<bool>("expectedResult");

    QTest::newRow("Just parent ref") << ".." << true;
    QTest::newRow("First parent ref") << "../asd" << true;
    QTest::newRow("Mid parent ref") << "asd/../asd" << true;
    QTest::newRow("Last parent ref") << "asd/.." << true;
    QTest::newRow("Multi parent ref") << "../asd/../asd/.." << true;
    QTest::newRow("Invalid absolute path with parent ref") << "/0asd/.." << true;
    QTest::newRow("Invalid relative path with parent ref") << "0asd/.." << true;

    QTest::newRow("Root path") << "/" << false;
    QTest::newRow("Absolute path") << "/asd" << false;
    QTest::newRow("Relative path") << "asd" << false;
    QTest::newRow("Empty path") << "" << false;
    QTest::newRow("Invalid absolute path") << "/0asd" << false;
    QTest::newRow("Invalid relative path") << "0asd" << false;
}

// Test: hasUnresolvedReferences() method ----------------------------------------------------------

void TestConfigNodePath::testResolveReferences()
{
    QFETCH(QString, nodePath);
    QFETCH(bool, expectedResult);
    QFETCH(QString, expectedNodePath);

    ConfigNodePath configNodePath(nodePath);
    QCOMPARE(configNodePath.resolveReferences(), expectedResult);
    QCOMPARE(configNodePath.path(), expectedNodePath);
}

void TestConfigNodePath::testResolveReferences_data()
{
    QTest::addColumn<QString>("nodePath");
    QTest::addColumn<bool>("expectedResult");
    QTest::addColumn<QString>("expectedNodePath");

    QTest::newRow("Root path") << "/" << true << "/";
    QTest::newRow("Absolute path") << "/asd" << true << "/asd";
    QTest::newRow("Relative path") << "asd" << true << "asd";

    QTest::newRow("Just parent ref") << ".." << true << "..";
    QTest::newRow("First parent ref") << "../asd" << true << "../asd";
    QTest::newRow("Mid parent ref") << "asd/../aaa" << true << "aaa";
    QTest::newRow("Last parent ref") << "asd/aaa/.." << true << "asd";
    QTest::newRow("Multi parent ref") << "../asd/../aaa/.." << true << "..";
    QTest::newRow("Absolute path with parent ref")
            << "/asd/aaa/bbb/../../ccc" << true << "/asd/ccc";
    QTest::newRow("Absolute path with parent ref until root") << "/asd/aaa/../.." << true << "/";
    QTest::newRow("Relative path with parent ref") << "asd/aaa/bbb/../../ccc" << true << "asd/ccc";
    QTest::newRow("Relative path with initial parent ref")
            << "../asd/../../ccc" << true << "../../ccc";
    QTest::newRow("Empty path") << "" << true << "";
    QTest::newRow("Invalid absolute path") << "/0asd" << true << "/0asd";
    QTest::newRow("Invalid relative path") << "0asd" << true << "0asd";
    QTest::newRow("Invalid absolute path with parent ref") << "/0asd/aaa/.." << true << "/0asd";
    QTest::newRow("Invalid relative path with parent ref") << "0asd/aaa/.." << true << "0asd";
    QTest::newRow("Absolute path with following slash") << "/asd/" << true << "/asd/";
    QTest::newRow("Relative path with following slash") << "asd/" << true << "asd/";

    QTest::newRow("Self") << "asd/.." << false << "asd/..";
    QTest::newRow("Absolute path with parent ref before root")
            << "/asd/../../ccc" << false << "/asd/../../ccc";
    QTest::newRow("Absolute path with parent ref and following slash")
            << "/asd/aaa/../" << false << "/asd/aaa/../";
    QTest::newRow("Relative path with parent ref and following slash")
            << "asd/aaa/../" << false << "asd/aaa/../";
}

// Test: nodeNames() method ------------------------------------------------------------------------

void TestConfigNodePath::testNodeNames()
{
    ConfigNodePath nodePath("/a/b/c");
    QStringList nodeNames = { "a", "b", "c" };
    QCOMPARE(nodePath.nodeNames(), nodeNames);

    nodePath.setPath("d/e/f");
    nodeNames = QStringList { "d", "e", "f" };
    QCOMPARE(nodePath.nodeNames(), nodeNames);
}

// Test: toAbsolute() method -----------------------------------------------------------------------

void TestConfigNodePath::testToAbsolute()
{
    QFETCH(QString, nodePath1);
    QFETCH(QString, nodePath2);
    QFETCH(QString, expectedNodePath);

    QCOMPARE(ConfigNodePath(nodePath1).toAbsolute(ConfigNodePath(nodePath2)).path(),
             expectedNodePath);
}

void TestConfigNodePath::testToAbsolute_data()
{
    QTest::addColumn<QString>("nodePath1");
    QTest::addColumn<QString>("nodePath2");
    QTest::addColumn<QString>("expectedNodePath");

    QTest::newRow("Root path without param") << "/" << QString() << "/";
    QTest::newRow("Root path with param: absolute path") << "/" << "/aaa/bbb" << "/";
    QTest::newRow("Root path with param: relative path") << "/" << "aaa/bbb" << "/";
    QTest::newRow("Root path with param: invalid path") << "/" << "0aaa/bbb" << "/";

    QTest::newRow("Absolute path without param") << "/asd/fgh" << QString() << "/asd/fgh";
    QTest::newRow("Absolute path with param: absolute path")
            << "/asd/fgh" << "/aaa/bbb" << "/asd/fgh";
    QTest::newRow("Absolute path with param: relative path")
            << "/asd/fgh" << "aaa/bbb" << "/asd/fgh";
    QTest::newRow("Absolute path with param: invalid path")
            << "/asd/fgh" << "0aaa/bbb" << "/asd/fgh";

    QTest::newRow("Relative path without param") << "asd/fgh" << QString() << "";
    QTest::newRow("Relative path with param: absolute path")
            << "asd/fgh" << "/aaa/bbb" << "/aaa/bbb/asd/fgh";
    QTest::newRow("Relative path with param: relative path") << "asd/fgh" << "aaa/bbb" << "";
    QTest::newRow("Relative path with param: invalid path") << "asd/fgh" << "0aaa/bbb" << "";

    QTest::newRow("Parent reference 1") << ".." << "/aaa/bbb" << "/aaa/bbb/..";
    QTest::newRow("Parent reference 2") << "aaa/bbb" << "/asd/fgh/.." << "/asd/fgh/../aaa/bbb";

    QTest::newRow("Invalid absolute path") << "/0asd" << "/aaa/bbb" << "";
    QTest::newRow("Invalid relative path") << "0asd" << "/aaa/bbb" << "";

    QTest::newRow("Empty path without param") << "" << QString() << "";
    QTest::newRow("Empty path with param: absolute path") << "" << "/aaa/bbb" << "";
    QTest::newRow("Empty path with param: relative path") << "" << "aaa/bbb" << "";
    QTest::newRow("Empty path with param: invalid path") << "" << "0aaa/bbb" << "";
}

// Test: append(nodeName) method -------------------------------------------------------------------

void TestConfigNodePath::testAppendNodeName()
{
    QFETCH(QString, nodePath);
    QFETCH(QString, nodeName);
    QFETCH(QString, expectedNodePath);

    // Const
    const ConfigNodePath configNodePath(nodePath);
    QCOMPARE(configNodePath.append(nodeName).path(), expectedNodePath);

    // Non-const
    QCOMPARE(ConfigNodePath(nodePath).append(nodeName).path(), expectedNodePath);
}

void TestConfigNodePath::testAppendNodeName_data()
{
    QTest::addColumn<QString>("nodePath");
    QTest::addColumn<QString>("nodeName");
    QTest::addColumn<QString>("expectedNodePath");

    QTest::newRow("Root path") << "/" << "asd" << "/asd";
    QTest::newRow("Absolute path") << "/asd" << "aaa" << "/asd/aaa";
    QTest::newRow("Relative path") << "asd" << "aaa" << "asd/aaa";
    QTest::newRow("Parent reference") << ".." << "aaa" << "../aaa";

    QTest::newRow("Invalid absolute path") << "/0asd" << "aaa" << "";
    QTest::newRow("Invalid relative path") << "0asd" << "aaa" << "";
    QTest::newRow("Invalid node name") << "asd" << "0aaa" << "";
    QTest::newRow("Empty path") << "" << "aaa" << "";
}

// Test: append(nodePath) method -------------------------------------------------------------------

void TestConfigNodePath::testAppendNodePath()
{
    QFETCH(QString, nodePath1);
    QFETCH(QString, nodePath2);
    QFETCH(QString, expectedNodePath);

    // Const
    const ConfigNodePath configNodePath1(nodePath1);
    const ConfigNodePath configNodePath2(nodePath2);
    QCOMPARE(configNodePath1.append(configNodePath2).path(), expectedNodePath);

    // Non-const
    QCOMPARE(ConfigNodePath(nodePath1).append(configNodePath2).path(), expectedNodePath);
}

void TestConfigNodePath::testAppendNodePath_data()
{
    QTest::addColumn<QString>("nodePath1");
    QTest::addColumn<QString>("nodePath2");
    QTest::addColumn<QString>("expectedNodePath");

    QTest::newRow("Root path") << "/" << "aaa/bbb" << "/aaa/bbb";
    QTest::newRow("Absolute path") << "/asd" << "aaa/bbb" << "/asd/aaa/bbb";
    QTest::newRow("Relative path") << "asd" << "aaa/bbb" << "asd/aaa/bbb";
    QTest::newRow("Parent reference 1") << ".." << "aaa/bbb" << "../aaa/bbb";
    QTest::newRow("Parent reference 2") << "aaa/bbb" << ".." << "aaa/bbb/..";

    QTest::newRow("Invalid absolute path") << "/0asd" << "aaa/bbb" << "";
    QTest::newRow("Invalid relative path") << "0asd" << "aaa/bbb" << "";
    QTest::newRow("Invalid param: absolute path") << "/asd" << "/aaa/bbb" << "";
    QTest::newRow("Invalid param: invalid relative path") << "/asd" << "0aaa/bbb" << "";
    QTest::newRow("Invalid param: invalid absolute path") << "/asd" << "/0aaa/bbb" << "";
    QTest::newRow("Empty path") << "" << "aaa/bbb" << "";
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigNodePath)
#include "testConfigNodePath.moc"
