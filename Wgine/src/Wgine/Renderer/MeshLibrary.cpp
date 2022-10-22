#include "WginePCH.h"
#include "MeshLibrary.h"

namespace Wgine
{
	static std::unordered_map<std::string, Ref<Mesh>> s_FileLibrary;

    #pragma region Quad
    static Vertex s_QuadVertices[4] = {
        { {  0.5f,  0.5f, 0.f }, { 1.f, 1.f, 1.f }, { 1.f, 0.f } },
        { {  0.5f, -0.5f, 0.f }, { 1.f, 1.f, 1.f }, { 0.f, 0.f } },
        { { -0.5f, -0.5f, 0.f }, { 1.f, 1.f, 1.f }, { 0.f, 1.f } },
        { { -0.5f,  0.5f, 0.f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f } },
    };

    static uint32_t s_QuadIndices[6] = {
        0, 1, 2, 2, 3, 0,
    };

    static Ref<Mesh> s_Quad = MakeRef<Mesh>(s_QuadVertices, 4, s_QuadIndices, 6);
    #pragma endregion

    #pragma region Cube
	static Vertex s_CubeVertices[24] = {
        // -z side
		{ { -0.5f, -0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 0.f, 1.f } },
        { {  0.5f, -0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f } },
        { {  0.5f,  0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 1.f, 0.f } },
        { { -0.5f,  0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 0.f, 0.f } },

        // +z side
        { { -0.5f, -0.5f, 0.5f },  { 1.f, 1.f, 1.f }, { 0.f, 1.f } },
        { { -0.5f,  0.5f, 0.5f },  { 1.f, 1.f, 1.f }, { 0.f, 0.f } },
        { {  0.5f,  0.5f, 0.5f },  { 1.f, 1.f, 1.f }, { 1.f, 0.f } },
        { {  0.5f, -0.5f, 0.5f },  { 1.f, 1.f, 1.f }, { 1.f, 1.f } },

        // -x side
        { { -0.5f,  0.5f,  0.5f }, { 1.f, 1.f, 1.f }, { 0.f, 1.f } },
        { { -0.5f, -0.5f,  0.5f }, { 1.f, 1.f, 1.f }, { 0.f, 0.f } },
        { { -0.5f, -0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 1.f, 0.f } },
        { { -0.5f,  0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f } },

        // +x side
        { { 0.5f,  0.5f,  0.5f },  { 1.f, 1.f, 1.f }, { 0.f, 1.f } },
        { { 0.5f,  0.5f, -0.5f },  { 1.f, 1.f, 1.f }, { 1.f, 1.f } },
        { { 0.5f, -0.5f, -0.5f },  { 1.f, 1.f, 1.f }, { 1.f, 0.f } },
        { { 0.5f, -0.5f,  0.5f },  { 1.f, 1.f, 1.f }, { 0.f, 0.f } },

        // -y side
        { { -0.5f, -0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 0.f, 1.f } },
        { { -0.5f, -0.5f,  0.5f }, { 1.f, 1.f, 1.f }, { 0.f, 0.f } },
        { {  0.5f, -0.5f,  0.5f }, { 1.f, 1.f, 1.f }, { 1.f, 0.f } },
        { {  0.5f, -0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f } },

        // +y side
        { { -0.5f, 0.5f, -0.5f },  { 1.f, 1.f, 1.f }, { 0.f, 1.f } },
        { {  0.5f, 0.5f, -0.5f },  { 1.f, 1.f, 1.f }, { 1.f, 1.f } },
        { {  0.5f, 0.5f,  0.5f },  { 1.f, 1.f, 1.f }, { 1.f, 0.f } },
        { { -0.5f, 0.5f,  0.5f },  { 1.f, 1.f, 1.f }, { 0.f, 0.f } },
	};

	static uint32_t s_CubeIndices[36] = {
        0, 1, 2, 2, 3, 0,       // -z side
        4, 5, 6, 6, 7, 4,       // +z side
        8, 9, 10, 10, 11, 8,    // -x side
        12, 13, 14, 14, 15, 12, // +x side
        16, 17, 18, 18, 19, 16, // -y side
        20, 21, 22, 22, 23, 20, // +y side
	};

    static Ref<Mesh> s_Cube = MakeRef<Mesh>(s_CubeVertices, 24, s_CubeIndices, 36);

    static Vertex s_CubeSmoothVertices[8] = {
        // -z side
        { { -0.5f, -0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 0.f, 1.f } },
        { {  0.5f, -0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f } },
        { {  0.5f,  0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 1.f, 0.f } },
        { { -0.5f,  0.5f, -0.5f }, { 1.f, 1.f, 1.f }, { 0.f, 0.f } },

        // +z side
        { { -0.5f, -0.5f, 0.5f },  { 1.f, 1.f, 1.f }, { 0.f, 1.f } },
        { { -0.5f,  0.5f, 0.5f },  { 1.f, 1.f, 1.f }, { 0.f, 0.f } },
        { {  0.5f,  0.5f, 0.5f },  { 1.f, 1.f, 1.f }, { 1.f, 0.f } },
        { {  0.5f, -0.5f, 0.5f },  { 1.f, 1.f, 1.f }, { 1.f, 1.f } },
    };

    static uint32_t s_CubeSmoothIndices[36] = {
        0, 1, 2, 2, 3, 0, // -z side
        4, 5, 6, 6, 7, 4, // +z side
        5, 4, 0, 0, 3, 5, // -x side
        7, 6, 2, 2, 1, 7, // +x side
        1, 0, 4, 4, 7, 1, // -y side
        5, 3, 2, 2, 6, 5, // +y side
    };

	static Ref<Mesh> s_CubeSmooth = MakeRef<Mesh>(s_CubeSmoothVertices, 8, s_CubeSmoothIndices, 36);
    #pragma endregion

	Ref<Mesh> MeshLibrary::Get(const std::string &filePath)
	{
		WGINE_CORE_ASSERT(false, "TODO: Implement mesh importing from file");
		return MakeRef<Mesh>();
		//if (s_FileLibrary.find(filePath) != s_FileLibrary.end())
		//	return s_FileLibrary[filePath];

		//return s_FileLibrary[filePath] = Mesh::Create(filePath);
	}

    Ref<Mesh> MeshLibrary::GetQuad() { return s_Quad; }
	Ref<Mesh> MeshLibrary::GetCube() { return s_Cube; }
    Ref<Mesh> MeshLibrary::GetCubeSmooth() { return s_CubeSmooth; }
    Ref<Mesh> MeshLibrary::GetSphere(uint32_t slices, uint32_t stacks)
    {
        // source: https://www.danielsieger.com/blog/2021/03/27/generating-spheres.html

        auto mesh = MakeRef<Mesh>();

        // top vertex
        Vertex topV = { { 0.f, 1.f, 0.f } };
        uint32_t topIndex = mesh->GetVertices().size();
        mesh->AddVertex(topV);

        // stack / slice verts
        for (int i = 0; i < stacks - 1; i++)
        {
            auto phi = M_PI * (float)(i + 1) / stacks;
            for (int j = 0; j < slices; j++)
            {
                auto theta = 2.f * M_PI * j / (float) slices;
                auto x = glm::sin(phi) * glm::cos(theta);
                auto y = glm::cos(phi);
                auto z = glm::sin(phi) * glm::sin(theta);
                mesh->AddVertex({ {x, y, z} });
            }
        }

        // bottom vertex
        Vertex bottomV = { { 0.f, -1.f, 0.f } };
        uint32_t bottomIndex = mesh->GetVertices().size();
        mesh->AddVertex(bottomV);

        // top / bottom tris
        for (int i = 0; i < slices; i++)
        {
            auto i0 = i + 1;
            auto i1 = (i + 1) % slices + 1;
            mesh->AddIndex(i0);
            mesh->AddIndex(i1);
            mesh->AddIndex(topIndex);
            i0 = i + slices * (stacks - 2) + 1;
            i1 = (i + 1) % slices + slices * (stacks - 2) + 1;
            mesh->AddIndex(i1);
            mesh->AddIndex(i0);
            mesh->AddIndex(bottomIndex);
        }

        // in-between quads (tris)
        for (int j = 0; j < stacks - 2; j++)
        {
            auto j0 = j * slices + 1;
            auto j1 = (j + 1) * slices + 1;
            for (int i = 0; i < slices; i++)
            {
                auto i0 = j0 + i;
                auto i1 = j0 + (i + 1) % slices;
                auto i2 = j1 + (i + 1) % slices;
                auto i3 = j1 + i;
                mesh->AddIndex(i2);
                mesh->AddIndex(i1);
                mesh->AddIndex(i0);
                mesh->AddIndex(i0);
                mesh->AddIndex(i3);
                mesh->AddIndex(i2);
            }
        }

        mesh->RecalculateNormals(); // TODO: debug normals by drawing them (legacy ogl2 thingy?)

        return mesh;
    }
}
