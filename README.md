

Voxelizer
------------------------------------------------
by Rama Hoetzlein (ramakarl.com)
This code is licensed CC0, public domain

The interface lets you move the vertices of the triangle.
Use A,S,D,F or space to change the technique.

Code provides the basic math for the 3D voxelization of a single triangle.

Four techniques are provided. The first three are triangle-box intersection tests, and test the triangle against every voxel in the domain O(V^3). The last is a direct DDA Rasterizer, which only visits the voxels touched by the triangle O(Vt).

1- Explicit edge tests: Use edge fuctions to explicitly test each edge of triangle. 27 tests/voxel.
2- Schwarz-Seidel tests: Recent paper that uses a reduced, efficient set of edge tests. 9 tests/voxel.
3- Akenine-Moller tests: The first paper to provide efficient, exact voxelization. 18 tests/voxel.
4- DDA Rasterizer: Starts at one corner and scans along the edges, performing a DDA (2D differential analyzer) to fill the interior of the triangle. This technique is much faster for voxelization as it only visits interior voxels.

Rama Hoetzlein (c) 2019

