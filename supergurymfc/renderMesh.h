#pragma once

#include <G3DAll.h>



namespace RBX
{
	namespace Render
	{
		class Mesh
		{
		private:

			VARAreaRef worldGeometry_NormalStream;
			VARAreaRef worldGeometry_VertexStream;
			VARAreaRef worldGeometry_TexcoordStream;

			Array<uint32> worldGeometry_index;

			Array<Vector3> worldGeometry_Vertexarray;
			Array<Vector3> worldGeometry_Normalarray;
			Array<Vector2> worldGeometry_Texcoordarray;

		public:

			void write(Vector3 vertex, Vector3 normal, Vector2 texCoord, Color4 color);

			static Mesh* get();

			Mesh()
			{
				worldGeometry_NormalStream = VARArea::create(1024 * 1024);
				worldGeometry_VertexStream = VARArea::create(1024 * 1024);
				worldGeometry_TexcoordStream = VARArea::create(1024 * 1024);
			}
		};
	}
}