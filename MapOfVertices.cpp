
#include "pch.h"
#include <iostream>
#include <map>
#include <vector>
#include <math.h>   
#include <array>

using VisitedPairs = std::map<std::pair<int, int>, int>;

struct Vertex
{
	float x, y, z;
};

struct Face
{
	int vertex[3];
};

Face makeIndices(int a, int b, int c) {
	Face face;
	face.vertex[0] = a;
	face.vertex[1] = b;
	face.vertex[2] = c;
	
	return face;
}

Vertex setVertices(float x, float y, float z) {
	Vertex temp;
	temp.x = x;
	temp.y = y;
	temp.z = z;
	return temp;
}

Vertex setNormalizedVertices(float x, float y, float z) {
	float length = sqrtf(x * x + y * y + z * z);
	Vertex temp;
	temp.x = x/length;
	temp.y = y/length;
	temp.z = z/length;
	return temp;
}

Vertex midpoint(Vertex a, Vertex b) {
	Vertex temp;
	temp.x = (a.x + b.x) / 2;
	temp.y = (a.y + b.y) / 2;
	temp.z = (a.z + b.z) / 2;
	return temp;
}

int vertexMapping(VisitedPairs& visitedPairs, std::vector<Vertex>& vertices, int first, int second) {
	
	//create the key we are searching for
	VisitedPairs::key_type key(first, second);

	//must swap keys if they are out of order
	if (key.first > key.second)
		std::swap(key.first, key.second);

	//insert the pair with the value of the index
	auto inserted = visitedPairs.insert({ key, vertices.size() });
	//inserted = visitedPairs.insert({ key, 1 });

	//If pair is not in the map add the midpoint
	if (inserted.second) {
		auto& edge0 = vertices[first];
		auto& edge1 = vertices[second];

		//find the midpoint
		auto point = midpoint(edge0, edge1);

		//Normalize the point to the unit sphere
		point = setNormalizedVertices(point.x, point.y, point.z);
		
		std::cout << point.x << " " << point.y << " " << point.z << "\n";

		//Add mid point to the vector of vertices
		vertices.push_back(point);
	}
	//return the index of the newly added pair of points and edge
	return (inserted.first)->second;
}

std::vector<Face> subdivide(std::vector<Vertex>& vertices, std::vector<Face>& faces) {
	VisitedPairs VisitedPairs;
	std::vector<Face> newFaces;

	for (auto&& each : faces) {
		std::array<int, 3> mid;
		for (int edge = 0; edge < 3; edge++)
		{
			mid[edge] = vertexMapping(VisitedPairs, vertices,
				each.vertex[edge], each.vertex[(edge + 1) % 3]);
		}

		newFaces.push_back({ each.vertex[0], mid[0], mid[2] });
		newFaces.push_back({ each.vertex[1], mid[1], mid[0] });
		newFaces.push_back({ each.vertex[2], mid[2], mid[1] });
		newFaces.push_back({ mid[0], mid[1], mid[2] });
	}

	return newFaces;

}


int main()
{
	std::vector<Vertex> vertices;

	float pos = (1.0 + sqrtf(5.0)) / 2.0;

	//icosphere points
	vertices.push_back(setVertices(-1, pos, 0));
	vertices.push_back(setVertices(1, pos, 0));
	vertices.push_back(setVertices(-1, -pos, 0));
	vertices.push_back(setVertices(1, -pos, 0));

	vertices.push_back(setVertices(0, -1, pos));
	vertices.push_back(setVertices(0, 1, pos));
	vertices.push_back(setVertices(0, -1, -pos));
	vertices.push_back(setVertices(0, 1, -pos));

	vertices.push_back(setVertices(pos, 0, -1));
	vertices.push_back(setVertices(pos, 0, 1));
	vertices.push_back(setVertices(-pos, 0, -1));
	vertices.push_back(setVertices(-pos, 0, 1));

	std::vector<Face> faces;

	//Top
	faces.push_back(makeIndices(1, 0, 5));
	faces.push_back(makeIndices(1, 5, 9));
	faces.push_back(makeIndices(1, 9, 8));
	faces.push_back(makeIndices(1, 8, 7));
	faces.push_back(makeIndices(1, 7, 0));

	//Bottom
	faces.push_back(makeIndices(2, 11, 4));
	faces.push_back(makeIndices(2, 4, 3));
	faces.push_back(makeIndices(2, 3, 6));
	faces.push_back(makeIndices(2, 6, 10));
	faces.push_back(makeIndices(2, 10, 11));

	//Side
	faces.push_back(makeIndices(0, 11, 5));
	faces.push_back(makeIndices(5, 11, 4));
	faces.push_back(makeIndices(5, 4, 9));
	faces.push_back(makeIndices(9, 4, 3));
	faces.push_back(makeIndices(9, 3, 8));
	faces.push_back(makeIndices(8, 3, 6));
	faces.push_back(makeIndices(8, 6, 7));
	faces.push_back(makeIndices(7, 6, 10));
	faces.push_back(makeIndices(0, 10, 11));
	faces.push_back(makeIndices(7, 10, 0));
	
	
	//Map for holding visited pairs of vertices
	VisitedPairs visitedPairs;

	//The key is a pair of vertices
	int first = 0;
	int second = 1;

	
	std::cout << vertexMapping(visitedPairs,vertices,first,second) << std::endl;
	for (auto && each : vertices) {
		std::cout << "x: " << each.x << " y: " << each.y << " z: " << each.z << std::endl;
	}

	int i = 1;
	for (auto && each : faces) {
		std::cout << i << ": Vertex1: " << each.vertex[0] << " Vertex2: " << each.vertex[1] << " Vertex3: " << each.vertex[2] << std::endl;
		i++;
	}


	std::vector<Face> newFaces	= subdivide(vertices, faces);
	i = 1;
	for (auto && each : newFaces) {
		std::cout << i << ": Vertex1: " << each.vertex[0] << " Vertex2: " << each.vertex[1] << " Vertex3: " << each.vertex[2] << std::endl;
		i++;
	}


}
