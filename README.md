![image](https://github.com/ramakarl/voxelizer/blob/master/voxelizer.jpg)

## Voxelizer
------------------------------------------------
by Rama Karl Hoetzlein, http://ramakarl.com<br>
This code is licensed CC-BY (c) 2021
<br><br>
The interface lets you move the vertices of the triangle.<br>
Use 1,2,3,4 keys to change the Algorithm.
<br><br>
Code gives the basic math for the 3D voxelization of a triangle.
<br><br>
Four techniques are provided. The first three are triangle-box intersection tests, and test the triangle against every voxel in the domain O(V^3). The last is a direct DDA Rasterizer, which only visits the voxels touched by the triangle O(Vt).
<br><br>
1- **Explicit edge tests**: Use edge fuctions to explicitly test each edge of triangle. 27 tests/voxel.<br>
2- **Schwarz-Seidel tests**: Recent paper that uses a reduced, efficient set of edge tests. 9 tests/voxel.<br>
3- **Akenine-Moller tests**: The first paper to provide efficient, exact voxelization. 18 tests/voxel.<br>
4- **DDA Rasterizer**: Starts at one corner and scans along the edges, performing a DDA (2D differential analyzer) to fill the interior of the triangle in 3D. This technique is much faster as it only visits voxels on the triangle.<br>

Sources:<br>
Schwarz & Seidel, 2010, Fast Parallel Surface and Solid Voxelization on GPUs, http://research.michael-schwarz.com/publ/files/vox-siga10.pdf<br>
Akenine-Moller, 2001, Fast 3D Triangle-Box Overlap Testing, https://fileadmin.cs.lth.se/cs/Personal/Tomas_Akenine-Moller/pubs/tribox.pdf<br>
Pineda, 1998, A Parallel Algorithm for Polygon Rasterization (Edge Functions),  https://www.cs.drexel.edu/~david/Classes/Papers/comp175-06-pineda.pdf<br>

Citation of this code when used in papers or projects is appreciated:<br>
2019. Hoetzlein, Rama. *Voxelization of a triangle in 3D*. https://github.com/ramakarl/voxelizer

### How to Build

1. Run cmake in \libmin
2. Build the solution for libmin
3. Run cmake in \math_voxelizer
4. During cmake it should find libmin. If not, set the LIBMIN_ROOT_DIR
4. Build the solution. Run it.

<br><br>
Rama Hoetzlein (c) 2019

