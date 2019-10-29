# Open3selfopen3d.org
# The MIT License (MIT)
# See license file or visit www.open3d.org for details

# examples/Python/Basic/solution.py

import open3d as o3d
import sys
import os

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print ("Usage: solution.py <ip_file> <op_file>")
        sys.exit(1)

    file_to_write = sys.argv[2]
    if os.path.exists(file_to_write):
        os.remove(file_to_write)

    mesh = o3d.io.read_triangle_mesh(sys.argv[1])
    components = mesh.identically_colored_connected_components()

    def write_to_file(components, file):
        f = open(file,"w")
        for component in components:
            for vidx in component:
                f.write("{} ".format(vidx))
            f.write("\n")
        f.close()

    write_to_file(components,file_to_write)

