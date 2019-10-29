# Open3D

## Description
This version of Open3D adds a new API 'IdenticallyColoredConnectedComponents' in geometry::TriangleMesh class.
Project is derived from [Open3D](https://github.com/intel-isl/Open3D/tree/v0.8.0)

This API allows computation of identically colored connected components (vertices) of a given triangular mesh. 
Component connectedness is established based on:
1. Topological connectivity - Two vertices are connected if part of same triangle
2. Color similarity - Two vertices share same color property
 
Algorithm uses breadth first search (BFS) to traverse over vertices for establishing topological connectivity. It uses pre-computed adjacency list for faster graph traversal.
It maintains memorization to minimize number of BFS calls to improve performance.
   
The output is return as TriangleMesh::ConnectedComponentList, which is a STL vector storing individual connected components as TriangleMesh::ConnectedComponent.
As source vertex traversal occurs in ascending order of vertex index; the ConnectedComponentList is guranteed to be order in ascending order by smallest vertex index in each ConnectedComponent. 
ConnectedComponent is a STL set which gurantees vertex indices are order in ascending order.

## Python Binding
The implemented API in C++ has python binding as 'identically_colored_connected_components' method in  TriangleMesh class.

## Build Instructions

1. Clone repository
2. Ensure GIT sub-modules are initialized and updated
3. Refer to [BUILD INSTRUCTIONS](http://www.open3d.org/docs/release/compilation.html) for project configuration and build 
   - Ensure BUILD_PYBIND11, BUILD_PYTHON_MODULE, BUILD_CPP_EXAMPLES are selected in CMAKE
4. Project is currently tested on system with Windows 10 (64 bit) OS, VS 2015 compiler, python 3.6 and cmake 3.16

## Unit Tests
As Open3D unit tests are disabled on windows system no separate unit tests were implemented using google test framework.
Instead unit tests were written in the CPP example and can be triggered optionally.

## CPP Example
Build project 'Solution' under example/CPP. 
Run generated Solution.exe with command line arguments: <input file (.ply) > < output file > < 0/1 (optionally run unit-tests))

## Python Example
Ensure build with python binding. Install Open3D python package by building 'install-pip-package' project.
Run examples\Python\Basic\solution.py python script with command line argument: <input file (.ply) > < output file >