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
 * Contains a class for accessing environment variables
 */

// Own header
#include <CppConfigFramework/EnvironmentVariables.hpp>

// C++ Config Framework includes
#include <CppConfigFramework/ConfigNodePath.hpp>

// Qt includes
#include <QtCore/QProcessEnvironment>
#include <QtCore/QRegularExpression>

// Forward declarations

// Macros

// -------------------------------------------------------------------------------------------------

namespace CppConfigFramework
{

EnvironmentVariables EnvironmentVariables::loadFromProcess()
{
    EnvironmentVariables env;

    const QProcessEnvironment systemEnvironment = QProcessEnvironment::systemEnvironment();
    env.m_variables.clear();

    for (const QString &name : systemEnvironment.keys())
    {
        env.m_variables[name] = systemEnvironment.value(name);
    }

    return env;
}

// -------------------------------------------------------------------------------------------------

QStringList EnvironmentVariables::names() const
{
    return m_variables.keys();
}

// -------------------------------------------------------------------------------------------------

bool EnvironmentVariables::contains(const QString &name) const
{
    return m_variables.contains(name);
}

// -------------------------------------------------------------------------------------------------

QString EnvironmentVariables::value(const QString &name) const
{
    return m_variables[name];
}

// -------------------------------------------------------------------------------------------------

void EnvironmentVariables::setValue(const QString &name, const QString &value)
{
    m_variables[name] = value;
}

// -------------------------------------------------------------------------------------------------

QString EnvironmentVariables::expandText(const QString &text) const
{
    // Set the initial value
    QString expandedText = text;

    // Expand environment text in a loop in case of "nested" environment variables (max 100 cycles)
    static const QRegularExpression regex(QStringLiteral("\\${([a-zA-Z0-9_]+)}"));
    constexpr int maxCycles = 100;

    for (int i = 0; i < maxCycles; i++)
    {
        // Find and replace all environment variable references with their value
        auto it = regex.globalMatch(expandedText);
        bool variableReferenceReplaced = false;

        while (it.hasNext())
        {
            auto match = it.next();

            const QString reference = match.captured(0);
            const QString referenceName = match.captured(1);

            if (contains(referenceName))
            {
                expandedText.replace(reference, value(referenceName));
                variableReferenceReplaced = true;
            }
        }

        if (!variableReferenceReplaced)
        {
            // No variable reference was replaced in the last cycle, nothing left to be done
            break;
        }
    }

    // Check if all environment variable references were replaced with their value
    if (regex.match(expandedText).hasMatch())
    {
        return QString();
    }

    return expandedText;
}

} // namespace CppConfigFramework
