
#include "Mesh.h"
//#include "mgl.h"

#include<iostream>
#include<stdint.h>
#include<cstring>

using namespace libthing;
using namespace std;

//#include "log.h"



	/// requires firstLayerSlice height, and general layer height
Mesh::Mesh(Scalar const firstSliceZ, Scalar const layerH)
		:zTapeMeasure(firstSliceZ, layerH)
	{ 	}

const std::vector<Triangle3>& Mesh::readAllTriangles() const
{
	return allTriangles;
}

const BoundingBox& Mesh::readLimits() const
{
	return boundingBox;
}

const LayerMeasure& Mesh::readLayerMeasure() const
{
	return zTapeMeasure;
}

const SliceTable& Mesh::readSliceTable() const
{
	return sliceTable;
}


//
// Adds a triangle to the global array and for each slice of interest
//
void Mesh::addTriangle(Triangle3 const& t)
{

	Vector3 a, b, c;
	t.zSort(a,b,c);

	unsigned int minSliceIndex = this->zTapeMeasure.zToLayerAbove(a.z);
	if(minSliceIndex > 0)
		minSliceIndex --;

	unsigned int maxSliceIndex = this->zTapeMeasure.zToLayerAbove(c.z);
	if (maxSliceIndex - minSliceIndex > 1)
		maxSliceIndex --;

//		Log::often() << "Min max index = [" <<  minSliceIndex << ", "<< maxSliceIndex << "]"<< std::endl;
//		Log::often() << "Max index =" <<  maxSliceIndex << std::endl;
	unsigned int currentSliceCount = sliceTable.size();
	if (maxSliceIndex >= currentSliceCount)
	{
		unsigned int newSize = maxSliceIndex+1;
		sliceTable.resize(newSize); // make room for potentially new slices
//			Log::often() << "- new slice count: " << sliceTable.size() << std::endl;
	}

	allTriangles.push_back(t);

	size_t newTriangleId = allTriangles.size() -1;

//		 Log::often() << "adding triangle " << newTriangleId << " to layer " << minSliceIndex  << " to " << maxSliceIndex << std::endl;
	for (size_t i= minSliceIndex; i<= maxSliceIndex; i++)
	{
		TriangleIndices &trianglesForSlice = sliceTable[i];
		trianglesForSlice.push_back(newTriangleId);
//			Log::often() << "   !adding triangle " << newTriangleId << " to layer " << i  << " (size = " << trianglesForSlice.size() << ")" << std::endl;
	}

	boundingBox.grow(t[0]);
	boundingBox.grow(t[1]);
	boundingBox.grow(t[2]);


}


void Mesh::dump(std::ostream &out)
{
	//out << "dumping " << this << std::endl;
	//out << "Nb of triangles: " << allTriangles.size() << std::endl;
	size_t sliceCount = sliceTable.size();

	//out << "triangles per slice: (" << sliceCount << " slices)" << std::endl;
	for (size_t i= 0; i< sliceCount; i++)
	{
		TriangleIndices &trianglesForSlice = sliceTable[i];
		//trianglesForSlice.push_back(newTriangleId);
		//out << "  slice " << i << " size: " << trianglesForSlice.size() << std::endl;
        //Log::often() << "adding triangle " << newTriangleId << " to layer " << i << std::endl;
	}
}



size_t Mesh::triangleCount() {
	return allTriangles.size();
   //Log::often() << "all triangle count" << allTriangles.size();
}
