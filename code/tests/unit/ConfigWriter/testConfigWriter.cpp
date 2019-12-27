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
    void testWriteToJson();
    void testWriteToFile();

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

// Test: writeToJson() -----------------------------------------------------------------------------

void TestConfigWriter::testWriteToJson()
{
    const auto doc = ConfigWriter::writeToJson(createConfig());

    QCOMPARE(doc, createJson());
}

// Test: writeToFile() -----------------------------------------------------------------------------

void TestConfigWriter::testWriteToFile()
{
    // Remove output file if it already exists
    if (QFile::exists(m_testFilePath))
    {
        QFile::remove(m_testFilePath);
    }

    // Write to file
    QVERIFY(ConfigWriter::writeToFile(createConfig(), m_testFilePath));

    // Check file contents
    QFile file(m_testFilePath);
    QVERIFY(file.exists());
    QVERIFY(file.open(QIODevice::ReadOnly));

    auto doc = QJsonDocument::fromJson(file.readAll());

    QCOMPARE(doc, createJson());
}

// -------------------------------------------------------------------------------------------------

ConfigObjectNode TestConfigWriter::createConfig()
{
    return ConfigObjectNode
    {
        { "a", ConfigValueNode(1) },
        {
            "b", ConfigObjectNode
            {
                { "b1", ConfigValueNode(true) }
            }
        },
        { "c", ConfigNodeReference(ConfigNodePath("/a")) },
        {
            "d", ConfigDerivedObjectNode({ ConfigNodePath("/b") },
                                         ConfigObjectNode {
                                             { "d1", ConfigValueNode("d") }
                                         })
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
                    "b", QJsonObject
                    {
                        { "#b1", QJsonValue(true) }
                    }
                },
                { "&c", QJsonValue("/a") },
                {
                    "&d", QJsonObject
                    {
                        { "base", QJsonValue("/b") },
                        {
                            "config", QJsonObject
                            {
                                { "#d1", QJsonValue("d") }
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
