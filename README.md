# C++ Config Framework

The purpose of this config framework is to simplify creation and loading of config files for C++ applications.

The framework can be used to create cross-platform applications, since it is using only Qt and STL libraries.


## Requirements

Requirements document can be found [here](docs/Requirements.md).


## Design

Design document can be found [here](docs/Design.md).


## Build and deployment

For building the following requirements are needed:

* CMake v3.5 or later
* C++14 compliant compiler
* Qt v5.9 or later (only Core and Test modules)
* [Cedar Framework](https://github.com/djurodrljaca/CedarFramework)

If the library needs to be deployed to a custom location you need to set the `CMAKE_INSTALL_PREFIX` variable or if you need to use a custom Qt installation and/or *CedarFramework* is not deployed to one of the standard locations you need to set the `CMAKE_PREFIX_PATH` variable.

Example:

```
$ cmake -DCMAKE_INSTALL_PREFIX=path/to/install/dir -DCMAKE_PREFIX_PATH="path/to/qt/dir;path/to/CedarFramework/dir" path/to/source/dir
```

Then build and deploy the library:

```
$ cmake --build . --target install
```


## Usage

### CMake Integration

To use this module in a CMake project you need to have a built and deployed *CedarFramework* dependency. Then build and deploy CppConfigFramework and add this to your project's CMakeLists.txt:

```CMake
find_package(CppConfigFramework REQUIRED)
```

And link it to your target:

```CMake
target_link_libraries(target_name PUBLIC CppConfigFramework::CppConfigFramework)
```


### Creating a configuration class

Here is an example of a configuration class that has three parameters with one of them being a sub-configuration class:

```c++
#include <CppConfigFramework/ConfigLoader.hpp>

class ExampleSubConfig : public CppConfigFramework::ConfigLoader
{
public:
    ~ExampleSubConfig() override = default;

    QString name;

private:
    bool loadConfigParameters(
        const CppConfigFramework::ConfigObjectNode &config) override
    {
        return loadRequiredConfigParameter(&name, "name", config));
    }
};

class ExampleConfig : public CppConfigFramework::ConfigLoader
{
public:
    ~ExampleConfig() override = default;

    QString type;
    int count;
    ExampleSubConfig sub;

private:
    bool loadConfigParameters(
        const CppConfigFramework::ConfigObjectNode &config) override
    {
        if (!loadRequiredConfigParameter(&type, "type", config))
        {
            return false;
        }

        count = 0;
        if (!loadOptionalConfigParameter(&count, "count", config))
        {
            return false;
        }

        return sub.loadConfig("sub", config);
    }
};
```


### Loading a configuration file

To load the configuration you need to do something like this:

```c++
CppConfigFramework::ConfigReader configReader;
CppConfigFramework::EnvironmentVariables environmentVariables;

auto config = configReader.read("path/to/config/file",
                                QDir("path/to/config/dir"),
                                ConfigNodePath::ROOT_PATH,
                                ConfigNodePath::ROOT_PATH,
                                {},
                                &environmentVariables);

ExampleConfig exampleConfig;
exampleConfig.loadConfig(*config);
```

For this example the following configuration file will be used:

```json
{
    "config":
    {
        "type": "test",
        "sub":
        {
            "name": "name"
        }
    }
}
```

Which will produce the following values:

| Parameter                | Value
| ------------------------ | -----------------
| `exampleConfig.type`     | `"test"`
| `exampleConfig.count`    | `0` (default value)
| `exampleConfig.sub.name` | `"name"`
