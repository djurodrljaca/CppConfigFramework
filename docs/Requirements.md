# Requirements


## [R1] Configuration structure

Configuration shall be contained in one or more configuration files. In case of multiple configuration files one of them shall be the main configuration file which includes the others either directly or indirectly.


## [R2] Configuration file format

Different type of configuration files shall be possible, but only *JSON* file format shall be directly supported by this library. Other configuration file formats can be supported as long as they shall provide a compatible configuration structure.

For example other configuration file formats can be supported for integration with other configuration sources. They could for example then be imported just like *JSON* configuration files defined in this document.


## [R3] Container for environment variables

The configuration framework shall make it possible to use environment variables in the configuration. The current state of the environment variables shall be available to the currently processed configuration file and they shall be stored in the *environment variables container*.


## [R3.1] Initial environment variables

The application shall be able to load the process's environment into the *environment variables container*. It shall also be able to set application specific environment variables.


## [R3.2] Custom environment variables

The *custom environment variables* shall be stored in the *environment_variables* member in the root *JSON object*.

The *environment_variables* member shall be an optional *JSON object* value (i.e. *null*, or an empty *JSON object*, or without the *environment_variables* member).

If an environment variable with the same name is already in the environment variables object, then the *custom environment variable's* value shall be ignored.

The environment variables shall have names that follow this regex rule:

```
[a-zA-Z_][a-zA-Z0-9_]*
```


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

A configuration parameter node shall be a member in a *JSON object* or an element in a *JSON array*. The *JSON object* member's name and the *JSON array* element's index shall be used as the node name.


### [R4.3] Escape sequences

There are special cases where it shall be necessary to escape specific characters in different kind of strings:

* Configuration parameter's name or value
* Environment variable's name or value
* Reference to a node path
* etc.

In those cases the specific characters shall be escaped with a backslash character. Any character following the backslash character shall be interpreted as a literal character and it shall not used for any special behavior.

Note: When a character is escaped in a JSON string it shall be necessary to use double backslash sequence ("\\\\") because JSON uses the backslash character as an escape character so to produce an actual backslash character it is required to escape it (i.e. write it two times).


### [R4.4] Configuration parameter name

A configuration parameter name shall be any string (including an empty string).

There are special cases where it shall be necessary to escape specific characters in the configuration parameter's name. In those cases the specific characters shall be escaped with a backslash character (see [R4.3]).

| Special case                                                                                                  | Raw value | Escaped value |
|---------------------------------------------------------------------------------------------------------------|-----------|---------------|
| Name starts with a "&" character (the same as *reference decorator* as defined in [R4.6] and [R4.7])          | &abc      | \\\\&abc      |
| Name contains a backslash character "\\"                                                                      | ab\\c     | ab\\\\\\c     |
| Name contains "${" sequence that looks like a replacement of references to environment variables (see [R4.9]) | a${b      | a\\\\${b      |


### [R4.5] Referencing a configuration parameter node

A configuration parameter's node shall be referenced with either a relative or an absolute node path.


#### Node path

A node path shall be either a single node name or a sequence of node names separated by a "/" character. Each node shall represent another level in the node hierarchy.

A special node name ".." (*parent reference*) shall be interpreted as a reference to the parent of the current node in the node path.

There are special cases where it shall be necessary to escape specific characters in the node path. In those cases the specific characters shall be escaped with a backslash character (see [R4.3]).

| Special case                                                                                                       | Raw value | Escaped value |
|--------------------------------------------------------------------------------------------------------------------|-----------|---------------|
| Node name contains a backslash character "\\"                                                                      | a/b\\c/d  | a/b\\\\\\c/d  |
| Node name contains "${" sequence that looks like a replacement of references to environment variables (see [R4.9]) | a/b${c/d  | a/b\\\\${c/d  |
| Node name equals to ".." but is not actually a *parent reference*                                                  | a/../b    | a/\\\\../b    |
| Node name contains "/" character but is not actually a node separator                                              | a/b/c/d   | a/b\\\\/c/d   |
| First node in a relative node path has an empty node name                                                          | /a/b      | \\\\0/a/b     |
| Middle node in a node path has an empty node name (escaping is optional)                                           | a//b      | a//b          |
| Last node in a node path has an empty node name (escaping is optional)                                             | a/b/      | a/b/          |
| Second node in an absolute node path that looks like a root node has an empty node name                            | /         | /\\\\0        |


#### Relative node path

A relative node path shall be an ordinary node path except that it shall not start with a '/' character.

Note: It shall not be possible to reference a parent node of the root node (using *parent references*)!


#### Absolute node path

An absolute node path starts with a "/" character (representing the *root node*) optionally followed by a node path in the *relative node path* format.


#### Examples

Configuration parameter structure that will be used in the node path examples below:

```json
{
    "config":
    {
        "": "empty node name",
        "root_node":
        {
            "":
            {
                "comment": "empty parent node name"
            },
            "..":
            {
                "comment": "parent node name that looks like a parent reference"
            },
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
                },
                "sub_node3":
                [
                    1,
                    {
                        "a": 1,
                        "b": 2
                    },
                    3
                ],
            },
            "\\&not_a_ref": true,
            "param/with/slashes": true,
            "param\\\\with\\\\backslashes": true
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

* Element in an array node:
    
    * Current node path: "/root_node/sub_node/sub_node1/array_param"
    * Node path: "0"
    * Result node path: "/root_node/sub_node/sub_node1/array_param/0"

* Sub-node of an element in an array node:
    
    * Current node path: "/root_node/sub_node"
    * Node path: "sub_node3/1/a"
    * Result node path: "/root_node/sub_node/sub_node3/1/a"

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

* Node that starts with a "&" character:

    * Current node path: "/root_node"
    * Node path: "&not_a_ref"
    * Result node path: "/root_node/&not_a_ref"
    * Note: no need to escape the "&" character in a node path

* Node that contains a "/" character:

    * Current node path: "/root_node"
    * Node path: "param\\\\/with\\\\/slashes"
    * Result node path: "/root_node/param\\\\/with\\\\/slashes"

* Node that contains a "\\" character:

    * Current node path: "/root_node"
    * Node path: "param\\\\with\\\\backslashes"
    * Result node path: "/root_node/param\\\\/with\\\\/slashes"

* Node with an empty node name:

    * Current node path: "/"
    * Node path: "\\\\0"
    * Result node path: "/\\\\0"

* Sub-node with an empty node name:

    * Current node path: "/root_node"
    * Node path: "\\\\0" or "" (escaping is optional here)
    * Result node path: "/root_node/"

* Sub-node with an empty node name:

    * Current node path: "/root_node"
    * Node path: "\\\\0/a"
    * Result node path: "/root_node/\\\\0/a"

* Node with a node name that looks like a parent reference:

    * Current node path: "/root_node"
    * Node path: "\\\\.." or ".\\\\."
    * Result node path: "/\\\\.."


#### Absolute node path examples:

* Specific node:
    
    * Node path: "/root_node/sub_node/sub_node1/boolParam"
    * Result node path: "/root_node/sub_node/sub_node1/boolParam"

* Specific node with parent node reference:
    
    * Node path: "/root_node/sub_node/sub_node1/../sub_node2/int_param"
    * Result node path: "/root_node/sub_node/sub_node2/int_param"


### [R4.6] Referencing a configuration parameter node

A configuration parameter shall be able to reference another node (i.e. copy the node's structure). This shall be done by prefixing the configuration parameter name with the *reference decorator* ("&" character) and specifying the reference to another node as a string parameter value containing a valid node path.


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


### [R4.7] Configuration parameter inheritance for Object nodes

An Object configuration parameter shall be able to inherit the structure and values of one or more other configuration parameters and set overrides for the specified sub-parameters. This shall be done by prefixing the configuration parameter name with the *reference decorator* ("&" character) and using the following structure as the parameter value:

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


### [R4.8] Configuration parameter inheritance for Array nodes

An Array configuration parameter shall be able to inherit the structure and values of one or more other configuration parameters and set overrides for the specified sub-parameters. This shall be done by prefixing the configuration parameter name with the *reference decorator* ("&" character) and using the following structure as the parameter value:

```json
{
    "&derived_config":
    [
        {
            "type": "prepend_element",
            "base": "/node/path/to/base/config",
            "config": {}
        },
        {
            "type": "prepend_array",
            "base": "/node/path/to/base/config",
            "source_index": 0,
            "count": 1,
            "config": []
        },
        {
            "type": "insert_element",
            "destination_index": 0,
            "base": "/node/path/to/base/config",
            "config": {}
        },
        {
            "type": "insert_array",
            "destination_index": 0,
            "base": "/node/path/to/base/config",
            "source_index": 0,
            "count": 1,
            "config": []
        },
        {
            "type": "append_element",
            "base": "/node/path/to/base/config",
            "config": {}
        },
        {
            "type": "append_array",
            "base": "/node/path/to/base/config",
            "source_index": 0,
            "count": 1,
            "config": []
        },
        {
            "type": "override_element",
            "destination_index": 0,
            "base": "/node/path/to/base/config",
            "config": {}
        },
        {
            "type": "override_elements",
            "destination_index": 0,
            "base": "/node/path/to/base/config",
            "source_index": 0,
            "count": 1,
            "config": []
        },
        {
            "type": "remove_elements",
            "destination_index": 0,
            "count": 1
        }
    ]
}
```

A derived Array node shall have zero or more operations which shall be applied in the same order as listed in the structure.

The *type* member shall be required and it shall represent the type of operation:

* **prepend_element**: inserts an element at the start of the Array node
* **prepend_elements**: inserts elements at the start of the Array node
* **insert_element**: inserts an element at the specified index of the Array node
* **insert_elements**: inserts elements at the specified index of the Array node
* **append_element**: inserts an element at the end of the Array node
* **append_elements**: inserts elements at the end of the Array node
* **override_element**: overrides an element at the specified index of the Array node
* **override_elements**: overrides elements starting at the specified index of the Array node
* **remove_elements**: removes one or more elements starting at the specified index of the Array node

The *destination_index* member shall point to the index in the resulting Array node where the operation needs to be applied. It shall be available and required for operations:

* *insert_element*
* *insert_elements*
* *override_element*
* *override_elements*
* *remove_elements*

The *base* member shall contain a reference to the configuration parameter node to use as a base. In case the *type* represents insertion or overriding of elements then the referenced configuration parameter node shall also be an Array node. It shall be available and optional for insertion and overriding operations.

The *source_index* member shall point to the index in the Array node from *base* where the operation needs to be applied. It shall be available and optional (default value: "0") for operations:

* *prepend_elements*
* *insert_elements*
* *append_elements*
* *override_elements*

The *count* member shall be optional and a positive non-zero integer. It shall be available for the removal operation with the default value "1" and for insertion and overriding operations where the default value shall match the number of elements from Array node from *base* from it's *source_index* to the end of the Array node.

The *config* member shall enable the user to override the configuration parameters for that operation. In case the *type* represents insertion or overriding of elements then the *config* member shall be a *JSON Array*. It shall be available and optional for insertion and overriding operations.

The configuration parameter node the *base* node and the *config* member shall be merged together with *base* node being applied first and the *config* node last. In case of Array nodes the element with the same index will be merged with the resulting Array node having the same number or elements as the larger of the two Array nodes.


#### Example

```json
{
    "config":
    {
        "base_nodes":
        {
            "param1": 1,
            "param2": 2,
            "param3": 3,
            "array1":
            [
                11,
                22,
                33
            ],
            "&derived_array1":
            [
                {
                    "type": "append_element",
                    "base": "../param1"
                },
                {
                    "type": "append_element",
                    "base": "../param2"
                },
                {
                    "type": "append_element",
                    "base": "../param3"
                },
                {
                    "type": "prepend_element",
                    "config": 4
                },
                {
                    "type": "insert_array",
                    "destination_index": 1,
                    "base": "../array1",
                    "source_index": 1
                },
                {
                    "type": "append_element",
                    "config": 123
                },
                {
                    "type": "override_element",
                    "destination_index": 6,
                    "base": "/node/path/to/base/config",
                    "config": 321
                }
            ]
        },
        "&result_node1":
        [
            {
                "type": "append_array",
                "base": "/base_nodes/derived_array1",
                "source_index": 2,
                "count": 4
            },
            {
                "type": "remove_elements",
                "destination_index": 1,
                "count": 2
            }
        ],
        "&result_node2":
        [
            {
                "type": "append_array",
                "base": "/base_nodes/derived_array1"
            },
            {
                "type": "override_elements",
                "destination_index": 1,
                "config":
                [
                    55,
                    66
                ]
            }
        ]
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
            "derived_array1":
            [
                4,
                22,
                33,
                1,
                2,
                3,
                321
            ]
        },
        "result_node1":
        [
            33,
            3
        ],
        "result_node2":
        [
            4,
            55,
            66
            1,
            2,
            3,
            321
        ]
    }
}
```


### [R4.9] Replacement of references to environment variables

It shall be possible to replace a references to an environment variable with its value. Any string in the *JSON* representation of the configuration data structure (*JSON Object* keys and string *JSON Values*) shall be able to have references to environment variables.

The format for a reference to an environment variable is described in [R3.3].

Note: If the string shall contain "\${" sequence as part of its value then it shall be required to escape this sequence as "\\\\\${".


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
        "value1": "${ENV_VAR1} != ${ENV_VAR2}",
        "value2":
        {
            "${ENV_VAR1}": 1,
            "${ENV_VAR2}": 2
        },
        "value3":
        [
            "${ENV_VAR1}",
            "${ENV_VAR2}"
        ],
        "value_${ENV_VAR1}": "abc",
        "value_\\${ENV_VAR1}": "abc"
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
        ],
        "value_aaa": "abc",
        "value_${ENV_VAR1}": "abc"
    }
}
```


### [R4.10] Reference resolution using externally provided configurations

Reference resolution shall be done in two phases. First the read configuration node shall be used to try to resolve references. In case the references cannot be resolved another attempt shall be made also using the externally provided configurations.

A potential use of this feature is in reading included configuration files. When an included configuration file is read the already read configuration from other included configuration files can then be used to resolve references in the currently read configuration file.


### [R4.11] Directory of the currently read configuration file

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

The ```"CppConfigFramework"``` value shall be used as a default value and it shall represent configuration files described in this document.

It shall be possible to register other configuration file types. The registration shall require the *type* and the parser algorithm.

The ```"CppConfigFramework"``` configuration file type shall extend this with some additional members:

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

The file path shall be expanded to a real absolute path. This means that any references to the parent directory (the ".." sequence) or environment variables (in format ```"${name}"``` where *name* represents the environment variable name) shall be applied to the path (see [R4.9]).


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


## [R7] Loading of a configuration node to native objects

It shall be possible to load a *configuration node* to native configuration objects (C++ classes).

There shall be the following types of *native configuration objects*:

* Configuration parameter: a value that can be directly mapped to a native data type (e.g. Boolean value, integer, string, map, list, date, time etc.)
* Configuration item: a structure containing *configuration parameters*
* Configuration container: a sequential or an associative container holding items of the same type of *configuration item*

Each *configuration parameter* and *configuration item* shall provide the following information:

* If it is mandatory or optional
* Validation algorithm (optional, e.g. list of allowed values, regular expression, custom algorithm etc.)

The *configuration node* shall be fully loaded before any *configuration parameter* or *configuration object* is loaded. This makes it possible for them to load their value(s) immediately when they are created.


## [R8] Storing of a native object to configuration nodes

It shall be possible to store native configuration objects (C++ classes) to a *configuration node*.

This operation shall be similar to loading of a configuration node to native objects (see [R7]) just that it shall make the conversion in the other direction.


## [R9] Conversion of an *Object* node to *JSON*

It shall be possible to convert a fully resolved *Object* node to *JSON*. This shall be done using the following rules:

* Member names in the *Object* node shall be used as keys in A *JSON Object*
* A *Value* node shall be stored directly as a value in A *JSON Object*
* An *Object* node shall first be converted to a *JSON Object* and then stored as a value in A *JSON Object*

Note: A fully resolved *Object* node has only *Value* and *Object* nodes in it and no other node types in its entire hierarchy.


## [R10] Generation of a configuration file from an *Object* node

It shall be possible to store an *Object* node to a file (of *CppConfigFramework* type). The Object node's structure shall be written to the *config* member (without *environment_variables* and *includes* members).
