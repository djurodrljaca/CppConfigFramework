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
 * Contains unit tests for ConfigWriter class
 */

// C++ Config Framework includes
#include <CppConfigFramework/ConfigDerivedObjectNode.hpp>
#include <CppConfigFramework/ConfigNodeReference.hpp>
#include <CppConfigFramework/ConfigObjectNode.hpp>
#include <CppConfigFramework/ConfigValueNode.hpp>
#include <CppConfigFramework/ConfigWriter.hpp>

// Qt includes
#include <QtCore/QDebug>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtTest/QTest>

// System includes

// Forward declarations

// Macros

// Test class declaration --------------------------------------------------------------------------

using namespace CppConfigFramework;
using ConfigNodePtr = std::shared_ptr<ConfigNode>;

Q_DECLARE_METATYPE(ConfigNodePtr);

class TestConfigWriter : public QObject
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
    void testWriteToJsonConfig();
    void testWriteToJsonConfigFile();
    void testConvertToJsonValue();

private:
    static ConfigObjectNode createConfig();
    static QJsonDocument createJson();

    QString m_testFilePath;
};

// Test Case init/cleanup methods ------------------------------------------------------------------

void TestConfigWriter::initTestCase()
{
}

void TestConfigWriter::cleanupTestCase()
{
}

// Test init/cleanup methods -----------------------------------------------------------------------

void TestConfigWriter::init()
{
    m_testFilePath = QDir::temp().absoluteFilePath("TestConfigWriter.json");
}

void TestConfigWriter::cleanup()
{
    if (QFile::exists(m_testFilePath))
    {
        QFile::remove(m_testFilePath);
    }
}

// Test: writeToJsonConfig() -----------------------------------------------------------------------

void TestConfigWriter::testWriteToJsonConfig()
{
    const auto doc = ConfigWriter::writeToJsonConfig(createConfig());

    QCOMPARE(doc, createJson());
}

// Test: writeToJsonConfigFile() -------------------------------------------------------------------

void TestConfigWriter::testWriteToJsonConfigFile()
{
    // Remove output file if it already exists
    if (QFile::exists(m_testFilePath))
    {
        QFile::remove(m_testFilePath);
    }

    // Write to file
    QVERIFY(ConfigWriter::writeToJsonConfigFile(createConfig(), m_testFilePath));

    // Check file contents
    QFile file(m_testFilePath);
    QVERIFY(file.exists());
    QVERIFY(file.open(QIODevice::ReadOnly));

    auto doc = QJsonDocument::fromJson(file.readAll());

    QCOMPARE(doc, createJson());
}

// Test: convertToJsonValue() -------------------------------------------------------------------

void TestConfigWriter::testConvertToJsonValue()
{
    // Positive tests
    ConfigObjectNode node1
    {
        { "a", ConfigValueNode(1) },
        {
            "b1", ConfigObjectNode
            {
                { "bool", ConfigValueNode(true) }
            }
        },
        {
            "b2", ConfigObjectNode
            {
                { "a", ConfigValueNode(123) },
                { "b", ConfigValueNode( QJsonArray{ 1, 2, 3 } ) },
                {
                    "c", ConfigObjectNode
                    {
                        { "a", ConfigValueNode("asd") },
                    }
                },
            }
        }
    };

    QJsonObject expectedJson
    {
        { "a", 1 },
        {
            "b1", QJsonObject
            {
                { "bool", true }
            }
        },
        {
            "b2", QJsonObject
            {
                { "a", 123 },
                { "b", QJsonArray{ 1, 2, 3 } },
                {
                    "c", QJsonObject
                    {
                        { "a", "asd" },
                    }
                },
            }
        }
    };

    QCOMPARE(ConfigWriter::convertToJsonValue(node1), expectedJson);

    // Negative tests
    ConfigObjectNode node2
    {
        { "a", ConfigValueNode(1) },
        { "ref", ConfigNodeReference(ConfigNodePath("/a")) }
    };

    QCOMPARE(ConfigWriter::convertToJsonValue(node2), QJsonValue(QJsonValue::Undefined));

    ConfigObjectNode node3
    {
        {
            "a", ConfigObjectNode
            {
                { "b", ConfigValueNode("b") }
            }
        },
        {
            "c", ConfigDerivedObjectNode({ ConfigNodePath("/a") },
                                         ConfigObjectNode { { "d", ConfigValueNode("d") } })
        }
    };

    QCOMPARE(ConfigWriter::convertToJsonValue(node3), QJsonValue(QJsonValue::Undefined));
}

// -------------------------------------------------------------------------------------------------

ConfigObjectNode TestConfigWriter::createConfig()
{
    return ConfigObjectNode
    {
        { "a", ConfigValueNode(1) },
        {
            "b1", ConfigObjectNode
            {
                { "bool", ConfigValueNode(true) }
            }
        },
        {
            "b2", ConfigObjectNode
            {
                { "a", ConfigNodeReference(ConfigNodePath("/a")) }
            }
        },
        { "c", ConfigNodeReference(ConfigNodePath("/a")) },
        {
            "d1", ConfigDerivedObjectNode({ ConfigNodePath("/b1") },
                                          ConfigObjectNode { { "d1", ConfigValueNode("d1") } })
        },
        {
            "d2", ConfigDerivedObjectNode({ ConfigNodePath("/b1"), ConfigNodePath("/b2") },
                                          ConfigObjectNode { { "d2", ConfigValueNode("d2") } })
        }
    };
}

// -------------------------------------------------------------------------------------------------

QJsonDocument TestConfigWriter::createJson()
{
    const QJsonObject json
    {
        {
            "config", QJsonObject
            {
                { "#a", QJsonValue(1) },
                {
                    "b1", QJsonObject
                    {
                        { "#bool", true }
                    }
                },
                {
                    "b2", QJsonObject
                    {
                        { "&a", "/a" }
                    }
                },
                { "&c", "/a" },
                {
                    "&d1", QJsonObject
                    {
                        { "base", "/b1" },
                        {
                            "config", QJsonObject
                            {
                                { "#d1", "d1" }
                            }
                        }
                    }
                },
                {
                    "&d2", QJsonObject
                    {
                        { "base", QJsonArray { "/b1", "/b2" } },
                        {
                            "config", QJsonObject
                            {
                                { "#d2", "d2" }
                            }
                        }
                    }
                }
            }
        }
    };

    return QJsonDocument(json);
}

// Main function -----------------------------------------------------------------------------------

QTEST_MAIN(TestConfigWriter)
#include "testConfigWriter.moc"
