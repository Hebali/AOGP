//////////////////////////////////////////////////
/*          Art of Graphics Programming         */
/*           Taught by Patrick Hebron           */
/* Interactive Telecommunications Program (ITP) */
/*             New York University              */
/*                  Fall 2014                   */
//////////////////////////////////////////////////

#pragma once

#include <algorithm>
#include <vector>

#include "cinder/gl/gl.h"
#include "cinder/gl/Vbo.h"

/** @brief a container used in the construction and manipulation of VBO meshes */
struct ProtoMesh
{
	/** @brief vertex container */
	struct Vertex
	{
		ci::Vec3f mPosition;		//!< the vertex's position
		ci::Vec3f mNormal;		//!< the vertex's normal
		ci::Vec2f mUV;			//!< the vertex's texture coordinate
		
		/** @brief default constructor */
		Vertex() {}
		
		/** @brief basic constructor */
		Vertex(const ci::Vec2f& iUv) : mUV( iUv ) {}
	};
	
	/** @brief draws the normals for this mesh */
	void drawDebug(const float& iNormalLength)
	{
		// Set draw state:
		glPointSize( 5.0 );
		glLineWidth( 2.0 );
		glColor3f( 0.0, 0.0, 1.0 );
		// Draw Normals:
		glBegin(GL_LINES);
		for(std::vector<Vertex>::const_iterator it = mVertices.cbegin(); it != mVertices.cend(); it++) {
			glVertex3f((*it).mPosition.x,
					   (*it).mPosition.y,
					   (*it).mPosition.z);
			glVertex3f((*it).mPosition.x + (*it).mNormal.x * iNormalLength,
					   (*it).mPosition.y + (*it).mNormal.y * iNormalLength,
					   (*it).mPosition.z + (*it).mNormal.z * iNormalLength);
		}
		glEnd();
	}
	
	std::vector<uint32_t>	mIndices;	//!< the protomesh's triangle strip indices
	std::vector<Vertex>		mVertices;	//!< the protomesh's vertices
};

/** @brief initializes a generic mesh for the given UV dimensions */
static void initializeGenericMesh(const uint32_t& iDimensionU, const uint32_t& iDimensionV, ProtoMesh& oMesh)
{
	// Set mesh dimensions (must have at least 2 points per axis):
	uint32_t tDimU = std::max<uint32_t>( iDimensionU, 2 );
	uint32_t tDimV = std::max<uint32_t>( iDimensionV, 2 );
	
	// Initialize vertices and setup the mesh UV coordinates:
	for(uint32_t v = 0; v < tDimV; v++) {
		for(uint32_t u = 0; u < tDimU; u++) {
			oMesh.mVertices.push_back( ProtoMesh::Vertex( ci::Vec2f( static_cast<float>( u ) / (tDimU - 1), static_cast<float>( v ) / (tDimV - 1 ) ) ) );
		}
	}
	
	// Setup subdivisions:
	for(uint32_t v = 0; v < tDimV - 1; v++) {
		// Determine the direction of the current row:
		bool goingRight = (v % 2 == 0);
		
		// Find the indices for the first and last column of the row:
		uint32_t firstCol = 0;
		uint32_t lastCol  = tDimU - 1;
		
		// Get the current column index, direction dependent:
		uint32_t u = ( goingRight ) ? ( firstCol ) : ( lastCol );
		
		// Iterate over each column in the row:
		bool rowComplete = false;
		while( !rowComplete ) {
			// For each column, determine the indices of the current rectangular subdivision
			// This depends on whether we're going right or left in the current row
			uint32_t iA, iB, iC, iD;
			if( goingRight ) {
				// Rightward triangles cba, bcd:
				// a   c
				//
				// b   d
				
				// Get the four indices of the current subdivision:
				iA = (v) * (tDimU) + (u);
				iB = (v + 1) * (tDimU) + (u);
				iC = (v) * (tDimU) + (u + 1);
				iD = (v + 1) * (tDimU) + (u + 1);
			}
			else {
				// Leftward triangles abc, dcb:
				// c   a
				//
				// d   b
				
				// Get the four indices of the current subdivision:
				iA = (v) * (tDimU) + (u);
				iB = (v + 1) * (tDimU) + (u);
				iC = (v) * (tDimU) + (u - 1);
				iD = (v + 1) * (tDimU) + (u - 1);
			}
			
			// Add the four indices of current subdivision to triangle strip:
			oMesh.mIndices.push_back( iA );
			oMesh.mIndices.push_back( iB );
			oMesh.mIndices.push_back( iC );
			oMesh.mIndices.push_back( iD );
			
			// Iterate through each column in the row, direction dependent:
			if( goingRight ) {
				u++;
			}
			else {
				u--;
			}
			
			// Check whether we've reached the end of the row
			if( ( goingRight && u == lastCol ) || ( !goingRight && u == firstCol ) ) {
				// At the end of the row, add last index of current subdivision
				// two more times to create "degenerate triangles":
				oMesh.mIndices.push_back( iD );
				oMesh.mIndices.push_back( iD );
				// Prepare to exit row:
				rowComplete = true;
			}
		}
	}
}

static void createSphere(const uint32_t& iDimensionU, const uint32_t& iDimensionV, const float& iRadius, ProtoMesh& oMesh)
{
	// Initialize a basic mesh:
	initializeGenericMesh( iDimensionU, iDimensionV, oMesh );
	
	// Iterate over each vertex:
	for(std::vector<ProtoMesh::Vertex>::iterator it = oMesh.mVertices.begin(); it != oMesh.mVertices.end(); it++) {
		// Find the current angle within the current longitudinal profile:
		float thetaU  = M_PI * 2.0 * (*it).mUV.x;
		
		// Find the current angle within the latitudinal arc,
		// offseting by a quarter circle (M_PI / 2.0) so that
		// we start at the pole rather than the equator:
		float thetaV  = (M_PI * (*it).mUV.y) - (M_PI / 2.0);
		
		// Compute the current position on the surface of the sphere:
		float x = iRadius * cos(thetaV) * cos(thetaU);
		float y = iRadius * cos(thetaV) * sin(thetaU);
		float z = iRadius * sin(thetaV);
		
		// Set vertex position:
		(*it).mPosition = ci::Vec3f( x, y, z );
		
		// Compute normal:
		// Note: This approach works for spheres.
		// Other primitives will require a different approach.
		(*it).mNormal = ci::Vec3f( x, y, z ).normalized();
	}
}

static void updateMeshVbo(ProtoMesh& iMesh, ci::gl::VboMesh& oMeshVbo)
{
	// Prepare VBO vertex iterator:
	ci::gl::VboMesh::VertexIter itvbo = oMeshVbo.mapVertexBuffer();
	// Iterate over each vertex in protomesh:
	for(std::vector<ProtoMesh::Vertex>::const_iterator itv = iMesh.mVertices.cbegin(); itv != iMesh.mVertices.cend(); itv++) {
		// Update vertices:
		itvbo.setPosition( (*itv).mPosition );
		// Update normals:
		itvbo.setNormal( (*itv).mNormal );
		// Update tex coords:
		itvbo.setTexCoord2d0( (*itv).mUV );
		// Advance VBO vertex iterator:
		++itvbo;
	}
	// Buffer triangle strip indices:
	oMeshVbo.bufferIndices( iMesh.mIndices );
}

static void createMeshVbo(ProtoMesh& iMesh, ci::gl::VboMesh& oMeshVbo)
{
	// Prepare VBO mesh settings:
	// Note: These setting allow us to manipulate the vertex data later
	ci::gl::VboMesh::Layout tLayout;
	tLayout.setStaticIndices();
	tLayout.setDynamicPositions();
	tLayout.setDynamicNormals();
	tLayout.setDynamicTexCoords2d();
	
	// Create VBO mesh:
	oMeshVbo = ci::gl::VboMesh( iMesh.mVertices.size(), iMesh.mIndices.size(), tLayout, GL_TRIANGLE_STRIP );
	
	// Set VBO mesh internals from input mesh:
	updateMeshVbo( iMesh, oMeshVbo );
}
