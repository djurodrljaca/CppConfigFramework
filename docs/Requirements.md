# Requirements


## [R1] Configuration structure

Configuration shall be contained in one or more configuration files. In case of multiple configuration files one of them shall be the main configuration file which includes the others either directly or indirectly.


## [R2] Configuration file format

Different type of configuration files shall be possible, but only *JSON* file format shall be directly supported by this library. Other configuration file formats can be supported as long as they shall provide a compatible configuration structure.

For example other configuration file formats can be supported for integration with other configuration sources. They could for example then be imported just like *JSON* configuration files defined in this document.


## [R3] Container for environment variables

The configuration framework shall make it possible to use environment variables in the configuration. The current state of the environment variables shall be available to the currently processed configuration file and they shall be stored in the *environment variables container*.


## [R3.1] Initial environment variables

The application shall be able to load the process's environment into the *environment variables container*. It shall also be able to load and application specific environment variables.


## [R3.2] Custom environment variables

The *custom environment variables* shall be stored in the *environment_variables* member in the root *JSON object*.

The *environment_variables* member shall be an optional *JSON object* value (i.e. *null*, or an empty *JSON object*, or without the *environment_variables* member).

If an environment variable with the same name is already in the environment variables object, then the *custom environment variable's* value shall be ignored.


#### Example

```json
{
    "environment_variables":
    {
        "ENV_VAR1": "aaa",
        "ENV_VAR2": "bbb"
    }
}
```


## [R3.3] References to environment variables

A reference to an environment variable shall be in the following format:

```
${environment_variable_name}
```


## [R4] Container for configuration parameters

The configuration parameters shall be stored in the *config* member in the root *JSON object*.

The *config* member shall be an optional *JSON object* value (i.e. *null* or an empty *JSON object*).


### [R4.1] Configuration parameter structure

All the native data types supported by *JSON* shall be available for the configuration parameters with a limitation that the *config* member in the root *JSON object* must be a *JSON object*. This limitation is needed so that *config* member provides only *named* configuration parameters.

### [R4.2] Configuration parameter node

A configuration parameter node shall be a member in a *JSON object*. The member's name shall be used as the node name.


### [R4.3] Configuration parameter name

A configuration parameter name shall have one or more characters where the first character shall be letter.

The name format shall conform to this regular expression:

```
^[a-zA-Z][a-zA-Z0-9_]*$
```


### [R4.4] Referencing a configuration parameter node

A configuration parameter's node shall be referenced with either a relative or an absolute node path.


#### Node path

A node path shall be either a single node name or a sequence of node names separated by a '/' character. Each node shall represent another level in the node hierarchy.

A special node name ".." (*parent reference*) shall be interpreted as a reference to the parent of the current node in the node path.


#### Relative node path

A relative node path shall be an ordinary node path except that it shall not start with a '/' character.

It shall not be possible to reference a parent node of the root node (using *parent references*)!


#### Absolute node path

An absolute node path starts with a '/' character (representing the *root node*) optionally followed by a node path in the *relative node path* format.


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

* Specific node with parent node reference:
    
    * Node path: "/root_node/sub_node/sub_node1/../sub_node2/int_param"
    * Result node path: "/root_node/sub_node/sub_node2/int_param"


### [R4.5] Referencing a configuration parameter node

A configuration parameter shall be able to reference another node (i.e. copy the node's structure). This shall be done by prefixing the configuration parameter name with the *reference decorator* ('&' character) and specifying the reference to another node as a string parameter value containing a valid node path.


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


### [R4.6] Configuration parameter inheritance

A configuration parameter shall be able to inherit the structure and values of one or more other configuration parameters and set overrides for the specified sub-parameters. This shall be done by prefixing the configuration parameter name with the *reference decorator* ('&' character) and using the following structure as the parameter value:

* Configuration parameter with a single base configuration object node:

```json
{
    "&derived_config":
    {
        "base": "/node/path/to/base/config",
        "config": {}
    }
}
```

* Configuration parameter with multiple base configuration object nodes:

```json
{
    "&derived_config":
    {
        "base":
        [
            "/node/path/to/base/config1",
            "/node/path/to/base/config2"
        ],
        "config": {}
    }
}
```

The *base* member shall contain a single reference (a string) or multiple references (an array of strings) to the configuration parameter node(s) to use as a base. The *base* node(s) shall be applied in the same order as listed in the array and their values shall be *merged*. This means that all *unique* configuration parameters shall be added to the resulting node and the last *duplicated* configuration parameter value shall be applied to the result.

The *config* member shall be optional and it shall enable the user to either add new or override existing configuration parameters.

The *base* node(s) and the *config* member shall both be *JSON objects*. The configuration parameter nodes from all *base* node(s) and the *config* member shall be merged together with *base* node(s) being applied first and the *config* node last.


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
        "&result_node1":
        {
            "base": "/base_nodes/object_base1"
        },
        "&result_node2":
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
        "base_nodes":
        {
            ...
        },
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


### [R4.7] Explicit Value node

All *JSON values* that are not *JSON Objects* and do not reference nodes are interpreted as a Value node. To explicitly interpret a *JSON Object* as a *Value* node then the parameter's name needs to be prefixed with the *explicit value* decorator ('#' character).

Note: The *explicit value* decorator shall be allowed also for *JSON Objects* which will result in the value being stored as a *JSON Object* value and not a configuration *Object* node!


#### Example

```json
{
    "config":
    {
        "#value1":
        {
            "param1": 1,
            "param2":
            {
                "item": true
            },
            "param3": [1, 2, 3]
        },
        "#value2": [1, 2, 3],
        "#value3": "asd"
    }
}
```


### [R4.8] Explicit Value node with references to environment variables

It shall be possible to configure a *Value* node where any string in it (including the keys of *JSON Objects*!) can have references to environment variables. This shall be achieved by prefixing the parameter's name with the *explicit value with references to environment variables* decorator ('&' character).

If the *JSON value* does not have any references to environment variables then the the effect shall be same as if the *explicit value* decorator was used.


#### Example

```json
{
    "environment_variables":
    {
        "ENV_VAR1": "aaa",
        "ENV_VAR2": "bbb"
    },
    "config":
    {
        "$value1": "${ENV_VAR1} != ${ENV_VAR2}",
        "$value2":
        {
            "${ENV_VAR1}": 1,
            "${ENV_VAR2}": 2
        },
        "$value3":
        [
            "${ENV_VAR1}",
            "${ENV_VAR2}"
        ]
    }
}
```

Result after environment variables are resolved:

```json
{
    "config":
    {
        "value1": "aaa != bbb",
        "value2":
        {
            "aaa": 1,
            "bbb": 2
        },
        "value3":
        [
            "aaa",
            "bbb"
        ]
    }
}
```


### [R4.9] Reference resolution using externally provided configurations

Reference resolution shall be done in two phases. First the read configuration node shall be used to try to resolve references. In case the references cannot be resolved another attempt shall be made also using the externally provided configurations.

A potential use of this feature is in reading included configuration files. When an included configuration file is read the already read configuration from other included configuration files can then be used to resolve references in the currently read configuration file.


### [R4.10] Directory of the currently read configuration file

The (absolute) directory of the currently read configuration file shall be exposed as an environment variable named ```CPPCONFIGFRAMEWORK_CURRENT_DIR```.


## [R5] Loading of configuration parameters from other configuration files

Links to other configuration files shall be stored in the *includes* member in the root *JSON object*.

The *includes* member shall be an optional *JSON array* value (i.e. *null*, or an empty *JSON array*, or without the *includes* member).

The specified configuration files shall be loaded in the specified order and before the main configuration is loaded (stored in *config* member of *JSON object*).

The *includes* member's elements shall have the following minimal structure:

```json
{
    "type": "configuration type",
    "destination_node": "/path/to/node"
}
```


### [R5.1] Configuration file type

The *type* member shall be optional and it shall represent the type of the configuration file.

The "CppConfigFramework" value shall be used as a default value and it shall represent configuration files described in this document.

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

For consistency these additional members (*file_path* and *source_node*) shall also be used in other configuration file types (if applicable).


### [R5.2] Destination node

The *destination_node* member shall be optional and it shall represent the configuration parameter node to which the selected node (*source_path*) must be copied.

The *root node* path (i.e. ```"/"```) shall be used as a default.


### [R5.3] File path

The *file_path* member shall represent either an absolute or a relative path.

The file path shall be expanded to a real absolute path. This means that any references to the parent directory (the ".." sequence) or environment variables (in format ```"${name}"``` where *name* represents the environment variable name) shall be applied to the path.


### [R5.4] Source node

The *source_node* member shall be optional and it shall represent the configuration parameter node from the included configuration that shall be taken (copied).

The *root node* path (i.e. ```"/"```) shall be used as a default.


### [R5.5] References to configuration parameters from included configuration files

Referencing the configuration parameters from included configuration files shall follow the same rules as for the ordinary references.


## [R6] Sequence for loading the full configuration

The following sequence shall be used to load the full configuration:

1. All environment variables shall be read and stored in the *environment variables container*
2. If present the configuration files from its *includes* member shall be read individually and their references resolved before reading the next included configuration file
3. The configuration parameters from the *config* member shall be loaded and applied to the currently read configuration (from included configuration files)
4. All the references from the resulting configuration shall be resolved.
5. The final configuration parameter structure shall be generated from the loaded configuration files (configuration node at *source path* shall be taken and moved to the *destination path*)


## [R7] Loading of the configuration parameter structure to native objects

It shall be possible to load the *configuration parameter structure* to native configuration objects (C++ classes).

There shall be the following types of *native configuration objects*:

* Configuration parameter: a value that can be directly mapped to a native data type (e.g. Boolean value, integer, string, map, list etc.)
* Configuration object: an associative container of *configuration parameters*
* Configuration container: a sequential or associative container holding items of the same type of *configuration objects*

Each *configuration parameter* and *configuration object* shall provide the following information:

* If it is mandatory or optional
* Validation algorithm (optional, e.g. list of allowed values, regular expression, custom algorithm etc.)

The *configuration parameter structure* shall be fully loaded before any *configuration parameter* or *configuration object* is loaded. This makes it possible for them to load their value(s) immediately when they are created.


## [R8] Generation of a configuration file from an *Object* node

It shall be possible to store an *Object* node to a file (of *CppConfigFramework* type). The Object node's structure shall be written to the *config* member (without *environment_variables* and *includes* members).


## [R9] Conversion of an *Object* node to *JSON*

It shall be possible to convert a fully resolved *Object* node to *JSON*. This shall be done using the following rules:

* Member names in the *Object* node shall be used as keys in A *JSON Object*
* A *Value* node shall be stored directly as a value in A *JSON Object*
* An *Object* node shall first be converted to a *JSON Object* and then stored as a value in A *JSON Object*

Note: A fully resolved *Object* node has only *Value* and *Object* nodes in it and no other node types in its entire hierarchy.
