# Requirements


## [R1] Configuration structure

Configuration shall be contained in one or more configuration files. In case of multiple configuration files one of them shall be specified as the main configuration file.


## [R2] Configuration file format

*JSON* shall be used as the file format for the configuration files.


## [R3] Container for configuration parameters

The configuration parameters shall be stored in the *config* member in the root *JSON object*.

The *config* member shall be an optional *JSON object* value (i.e. *null* or an empty *JSON object*).


### [R3.1] Configuration parameter structure

All the native data types supported by *JSON* shall be available for the configuration parameters with a limitation that the *config* member in the root *JSON object* must be a *JSON object* (can be empty). This limitation is needed so that *config* member provides only *named* configuration parameters.

### [R3.2] Configuration parameter node

A configuration parameter node shall be a member in a *JSON object*. The member's name shall be used as the node name.


### [R3.2] Configuration parameter name

A configuration parameter name shall have one or more characters where the first character shall be letter.

The name format shall conform to this regular expression: "^[a-zA-Z][a-zA-Z0-9_]*$"


### [R3.3] Referencing a configuration parameter node

A configuration parameter's node shall be referenced with either a relative or an absolute node path.

#### Node path

A node path shall be either a single node name or a sequence of node names separated by a '/' character. Each node shall represent another level in the node hierarchy.


#### Relative node path

A relative node path shall be just like an ordinary node path except that it can reference a parent node of the current node anywhere in the node path with a ".." sequence.

Note: It shall not be possible to reference a parent node of the root node!


#### Absolute node path

An absolute node path starts with a '/' character followed by a node path which could be in the *relative node path* format.


#### Examples

Example configuration parameter structure:

```json
{
    "config":
    {
        "root_node":
        {
            "sub_node":
            {
                "sub_node1":
                {
                    "bool_param": true,
                    "array_param": [5, 6, 7]
                },
                "sub_node2":
                {
                    "int_param": 1
                }
            }
        }
    }
}
```

#### Relative node path examples

* Sub node:
    
    * Current node path: "/root_node/sub_node/sub_node1"
    * Node path: "bool_param"
    * Result node path: "/root_node/sub_node/sub_node1/bool_param"

* Parent node:
    
    * Current node path: "/root_node/sub_node/sub_node1"
    * Node path: ".."
    * Result node path: "/root_node/sub_node"

* Element in an array parameter:
    
    * Current node path: "/root_node/sub_node/sub_node1/array_param"
    * Node path: "0"
    * Result node path: "/root_node/sub_node/sub_node1/array_param/0"

* Multiple nodes:
    
    * Current node path: "/root_node"
    * Node path: "sub_node/subNode1/bool_param"
    * Result node path: "/root_node/subNode/sub_node1/bool_param"

* Multiple nodes with a parent reference at the start:
    
    * Current node path: "/root_node/sub_node/subNode2"
    * Node path: "../sub_node1/boolParam"
    * Result node path: "/root_node/sub_node/sub_node1/boolParam"
    
* Multiple nodes with a parent reference in the middle:
    
    * Current node path: "/root_node/sub_node"
    * Node path: "sub_node1/../sub_node2/int_param"
    * Result node path: "/root_node/sub_node/sub_node2/int_param"


#### Absolute node path examples:

* Specific node:
    
    * Node path: "/root_node/sub_node/sub_node1/boolParam"
    * Result node path: "/root_node/sub_node/sub_node1/boolParam"

* Specific node with parent node refernece:
    
    * Node path: "/root_node/sub_node/sub_node1/../sub_node2/int_param"
    * Result node path: "/root_node/sub_node/sub_node2/int_param"


### [R3.3] Referencing of another configuration parameter node

A configuration parameter shall be able to reference another node (i.e. copy the full node). This shall be done by prefixing the configuration parameter name with a '&' character.


#### Example

In this example the value of "/root_node2/ref_value" configuration parameter shall be the same as the value of "/root_node1/value" (i.e. '1').

```json
{
    "config":
    {
        "root_node1":
        {
            "value": 1
        },
        "root_node2":
        {
            "&ref_value": "/root_node1/value"
        }
    }
}
```


### [R3.4] Configuration parameter inheritance

A configuration parameter shall be able to inherit the structure and values of one or more other configuration parameters and set overrides for the specified sub-parameters. This shall be done by prefixing the configuration parameter name with a '#' character and using the following structure as the parameter value:

* Single base configuration parameter:

```json
{
    "base": "/node/path/to/base/config",
    "config": {}
}
```

* Multiple base configuration parameter:

```json
{
    "base":
    [
        "/node/path/to/base/config1",
        "/node/path/to/base/config2"
    ],
    "config": {}
}
```

The *base* member shall contain a single reference (a string) or multiple references (an array of strings) to the configuration parameter node(s) to use as a base. The *base* nodes shall be applied in the same order as listed in the array and their values shall be *merged*. This means that all *unique* configuration parameters shall be added to the result and the last *duplicated* configuration parameter shall be applied to the result.

The *config* member shall be optional and it shall enable the user to either add new or override existing configuration parameters.

The *base* nodes and the *config* member shall both be *JSON objects*. The configuration parameter nodes from all *base* nodes and the *config* member shall be merged together with *config* member being applied last.


#### Example

```json
{
    "config":
    {
        "base_nodes":
        {
            "object_base1":
            {
                "a": 1,
                "b": 2,
                "c": 3,
                "d": 4
            },
            "object_base2":
            {
                "a": 10
            },
            "object_base3":
            {
                "b": 20,
                "e": 50
            }
        },
        "#result_node1":
        {
            "base": "/base_nodes/object_base1"
        },
        "#result_node2":
        {
            "base":
            [
                "/base_nodes/object_base1",
                "/base_nodes/object_base2",
                "/base_nodes/object_base3"
            ],
            "config":
            {
                "c": 30,
                "f": 60
            }
        }
    }
}
```

This configuration shall be equivalent to:

```json
{
    "config":
    {
        "base_nodes": {},
        "result_node1":
        {
            "a": 1,
            "b": 2,
            "c": 3,
            "d": 4
        },
        "result_node2":
        {
            "a": 10,
            "b": 20,
            "c": 30,
            "d": 4,
            "e": 50,
            "f": 60
        }
    }
}
```


## [R4] Loading of configuration parameters from other configuration files

Links to other configuration files shall be stored in the *includes* member in the root *JSON object*.

The *includes* member shall be an optional *JSON array* value (i.e. *null* or an empty *JSON array*).

The specified configuration files shall be loaded in the specified order and before the main configuration is loaded (stored in *config* member of *JSON object*).

The *includes* member's elements shall have the following minimal structure:

```json
{
    "type": "CppConfigFramework",
    "destination_node": "/path/to/node"
}
```


### [R4.1] Configuration file type

The *type* member shall be optional and it shall represent the type of the configuration file.

The "CppConfigFramework" value shall be used as a default value and it shall represent config files described in this document.

It shall be possible to register other configuration file types. The registration shall require the *type* and the parser algorithm.

The "CppConfigFramework" configuration file type shall extend this with some additional members:

```json
{
    "type": "CppConfigFramework",
    "file_path": "/path/to/config/file.json",
    "source_node": "/path/to/node",
    "destination_node": "/path/to/node"
}
```

For constitency these additional members ("file_path", "source_node") shall also be used in other configuration file types (if applicable).


### [R4.2] Destination node

The *destination_node* member shall be optional and it shall represent the configuration parameter node to which the selected node (*source_path*) must be copied.

The root node of the currently processed configuration file shall be used as a default (i.e. equivalent to "/").



### [R4.3] File path

The *file_path* member shall represent either an absolute or a relative path.

The file path shall be expanded to a real absolute path. This means that any references to the parent directory (the ".." sequence) or environment variables (in format "$(name)" where *name* represents the environment variable name) shall be applied to the path.


### [R4.4] Source node

The *source_node* member shall be optional and it shall represent the configuration parameter node that must be copied.

The root node of the configuration file shall be used as a default (i.e. equivalent to "/").


### [R4.5] References to configuration parameters from included configuration files

Referencing the configuration parameters from included configuration files shall follow the same rules as for the ordinary references.


## [R5] Sequence for loading the full configuration

The following sequence shall be used to load the full configuration

1. All environment variables shall be read and stored in an *JSON object* like container
2. The main configuration file shall be loaded by first loading all configuration files from its *includes* member and then the configuration parameters from the *config* member).
3. Each configuration file (also those from *includes* member) shall resolve references for all of their configuration parameters before processing of the next configuration file begins.
4. The final configuration parameter structure shall be generated from the loaded configuration files. This structure shall have the same data types and rules as *JSON*.


## [R6] Loading of the configuration parameter structure to native objects

It shall be possible to load the *configuration parameter structure* to native configuration objects (C++ classes).

There shall be the following types of *native configuration objects*:

* Configuration value: a configuration parameter that can be directly mapped to a native data type (e.g. Boolean value, integer, string, map, list etc.)
* Configuration object: a container of named items, where each item can be of any supported *native configuration object* type
* Configuration map: a container of the same type of *Configuration object* named items

Each *native configuration object* shall provide the following information:

* Description
* If it is mandatory or optional
* Recommended value (only if it is mandatory)
* Default value (only if it is optional)
* Validation algorithm (optional, e.g. list of allowed values, regular expression, custom algorithm etc.)

The *configuration parameter structure* shall be fully loaded before any *native configuration object* is loaded. This makes it possible for a *native configuration object* to load its value(s) immediately when they are created.


## [R7] Access to the configuration shall be thread safe

Access to the configuration shall be implemented in a thread safe way, so that *native configuration objects* can be created in any thread.


## [R8] Global access to the configuration

It shall be possible to load the configuration globally if explicitly set.

For example an application shall have a possibility to load the configuration into a global instance, but it could have a local configuration instead if that makes more sense for that application. The feature shall be available, but its use shall not be mandatory.


## [R9] Generation of the configuration documentation

An application shall have the possibility to generate a documentation of the actually loaded configuration. The documentation shall be *built* from all the information provided during the process of loading of all the *native configuration objects*.
