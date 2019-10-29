// ----------------------------------------------------------------------------
// -                        Open3D: www.open3d.org                            -
// ----------------------------------------------------------------------------
// The MIT License (MIT)
//
// Copyright (c) 2018 www.open3d.org
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
// ----------------------------------------------------------------------------

#include <iostream>
#include <fstream>

#include "Open3D/Open3D.h"

void PrintHelp() {
	using namespace open3d;
	utility::LogInfo("Usage :\n");
	utility::LogInfo("    > Solution <ip_file> <op_file> <runTests> \n");
}

#define _TEST_STATUS(expression,msg)		\
		if(!expression)						\
		{									\
			open3d::utility::LogError("TriangleMesh::IdenticallyColoredConnectedComponents Test Failed: {}\n",msg); \
			return false;					\
		}									\

bool UnitTests() {
	using namespace open3d;

	//          X   --    X
	//        /   \    /    \
	//     X    --  X   --    X     X  --  X
	//       \    /   \     /        \    /
	//          X        X             X

	auto mesh = std::make_shared<geometry::TriangleMesh>();

	{
		//Check when no vertices
		geometry::TriangleMesh::ConnectedComponentList list = mesh->IdenticallyColoredConnectedComponents();
		_TEST_STATUS((list.size() == 0), "No vertex")
	}

	mesh->vertices_.push_back(Eigen::Vector3d(0, 0, 0));
	mesh->vertices_.push_back(Eigen::Vector3d(0, 1, 0));
	mesh->vertices_.push_back(Eigen::Vector3d(1, 0, 0));

	mesh->triangles_.push_back(Eigen::Vector3i(0, 1, 2));

	{
		//Check when no coloring applied to vertices
		geometry::TriangleMesh::ConnectedComponentList list = mesh->IdenticallyColoredConnectedComponents();
		_TEST_STATUS((list.size() == 0), "No vertex color")
	}

	mesh->vertex_colors_.push_back(Eigen::Vector3d(255, 0, 0));
	mesh->vertex_colors_.push_back(Eigen::Vector3d(0, 255, 0));
	mesh->vertex_colors_.push_back(Eigen::Vector3d(0, 0, 255));

	{
		//Basic test for single triangle with different vertex color
		geometry::TriangleMesh::ConnectedComponentList truth;
		geometry::TriangleMesh::ConnectedComponent c1; c1.insert(0); truth.push_back(c1);
		geometry::TriangleMesh::ConnectedComponent c2; c2.insert(1); truth.push_back(c2);
		geometry::TriangleMesh::ConnectedComponent c3; c3.insert(2); truth.push_back(c3);

		geometry::TriangleMesh::ConnectedComponentList list = mesh->IdenticallyColoredConnectedComponents();
		_TEST_STATUS((list == truth), "Single triangle different vertex color ")
	}

	mesh->vertex_colors_[1] = Eigen::Vector3d(255, 0, 0);
	mesh->vertex_colors_[2] = Eigen::Vector3d(255, 0, 0);

	{
		//Basic test for single triangle with same vertex color
		geometry::TriangleMesh::ConnectedComponentList truth;
		geometry::TriangleMesh::ConnectedComponent c1; c1.insert(0); c1.insert(1), c1.insert(2); truth.push_back(c1);
	
		geometry::TriangleMesh::ConnectedComponentList list = mesh->IdenticallyColoredConnectedComponents();
		_TEST_STATUS((list == truth), "Single triangle same vertex color")
	}

	mesh->vertices_.push_back(Eigen::Vector3d(1, -1, 0));
	mesh->vertices_.push_back(Eigen::Vector3d(-1, -1, 0));
	mesh->vertices_.push_back(Eigen::Vector3d(-1, 0, 0));
	mesh->vertices_.push_back(Eigen::Vector3d(-1, 1, 0));
	
	mesh->vertex_colors_.push_back(Eigen::Vector3d(255, 0, 0));
	mesh->vertex_colors_.push_back(Eigen::Vector3d(255, 0, 0));
	mesh->vertex_colors_.push_back(Eigen::Vector3d(255, 0, 0));
	mesh->vertex_colors_.push_back(Eigen::Vector3d(255, 0, 0));	

	mesh->triangles_.push_back(Eigen::Vector3i(0, 2, 3));
	mesh->triangles_.push_back(Eigen::Vector3i(0, 3, 4));
	mesh->triangles_.push_back(Eigen::Vector3i(0, 4, 5));
	mesh->triangles_.push_back(Eigen::Vector3i(0, 5, 6));

	//Disjoint triangle
	mesh->vertices_.push_back(Eigen::Vector3d(2, 0, 0));
	mesh->vertices_.push_back(Eigen::Vector3d(4, 0, 0));
	mesh->vertices_.push_back(Eigen::Vector3d(3, -1, 0));

	mesh->vertex_colors_.push_back(Eigen::Vector3d(255, 0, 0));
	mesh->vertex_colors_.push_back(Eigen::Vector3d(255, 0, 0));
	mesh->vertex_colors_.push_back(Eigen::Vector3d(255, 0, 0));

	mesh->triangles_.push_back(Eigen::Vector3i(7, 8, 9));

	{
		//Test algorithm with disjoint components
		geometry::TriangleMesh::ConnectedComponentList list = mesh->IdenticallyColoredConnectedComponents();

		geometry::TriangleMesh::ConnectedComponentList truth;
		geometry::TriangleMesh::ConnectedComponent c1; c1.insert(0); c1.insert(1); c1.insert(2); c1.insert(3); c1.insert(4); c1.insert(5); c1.insert(6); truth.push_back(c1);
		geometry::TriangleMesh::ConnectedComponent c2; c2.insert(7); c2.insert(8); c2.insert(9); truth.push_back(c2);
		
		_TEST_STATUS((list == truth), "Finds disjoint components")
	}

	mesh->vertex_colors_[2] = Eigen::Vector3d(0, 255, 0);
	mesh->vertex_colors_[5] = Eigen::Vector3d(0, 255, 0);
	mesh->vertex_colors_[3] = Eigen::Vector3d(0, 255, 255);
	mesh->vertex_colors_[4] = Eigen::Vector3d(0, 255, 255);

	mesh->vertex_colors_[7] = Eigen::Vector3d(0, 255, 0);

	{
		//Though all tests do check for ascending order of components and indices within components
		//Let us test this again with a more complex scenario
		geometry::TriangleMesh::ConnectedComponentList list = mesh->IdenticallyColoredConnectedComponents();

		geometry::TriangleMesh::ConnectedComponentList truth;
		geometry::TriangleMesh::ConnectedComponent c1; c1.insert(0); c1.insert(1); c1.insert(6); truth.push_back(c1);
		geometry::TriangleMesh::ConnectedComponent c2; c2.insert(2); truth.push_back(c2);
		geometry::TriangleMesh::ConnectedComponent c3; c3.insert(3); c3.insert(4); truth.push_back(c3);
		geometry::TriangleMesh::ConnectedComponent c4; c4.insert(5); truth.push_back(c4);
		geometry::TriangleMesh::ConnectedComponent c5; c5.insert(7); truth.push_back(c5);
		geometry::TriangleMesh::ConnectedComponent c6; c6.insert(8); c6.insert(9); truth.push_back(c6);

		_TEST_STATUS((list == truth), "Ascending order for components and vertex indices")
	}

	{
		//If non-functional requirements available good to also include tests for runtime and memory consumption!
		//Skipping for now!
	}

	return true;
}

void WriteToFile(open3d::geometry::TriangleMesh::ConnectedComponentList &list, const std::string file)
{
	std::ofstream ofs(file);

	for (auto component : list) {

		for (auto vidx : component) {
			ofs << vidx << " ";
		}
		ofs << std::endl;//Adding extra line!
	}

	ofs.close();
}

int main(int argc, char *argv[]) {
    using namespace open3d;

    utility::SetVerbosityLevel(utility::VerbosityLevel::Debug);

	if (argc < 3) {
		PrintHelp();
		return 1;
	}

	const std::string file_to_read = argv[1];
	const std::string file_to_write = argv[2];

	std::remove(file_to_write.c_str());

	if (argc > 3)
	{
		if (std::atoi(argv[3]))
		{
			bool test_status = UnitTests();

			if (!test_status)
			{
				return 1;
			}
		}
	}

	auto mesh = io::CreateMeshFromFile(file_to_read);

	geometry::TriangleMesh::ConnectedComponentList list = mesh->IdenticallyColoredConnectedComponents();	

	WriteToFile(list, file_to_write);

	return 0;
}
