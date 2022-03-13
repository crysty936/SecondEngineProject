#include "BasicShapes.h"
#include "Renderer/ShapesUtils/BasicShapesData.h"
#include "Renderer/OpenGL/Buffer/IndexBuffer.h"
#include "Renderer/OpenGL/Buffer/VertexBufferLayout.h"
#include "Renderer/OpenGL/Buffer/VertexBuffer.h"
#include "Renderer/OpenGL/Buffer/VertexArrayObject.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer/Model/3D/Mesh3D.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/OpenGL/RenderCommand.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Core/ObjectCreation.h"

const uint32_t texureBaseNr = GL_TEXTURE0;

// eastl::shared_ptr<IDrawable> BasicShapes::CreateTriangleObject(eastl::string inTexturePath)
// {
// 	if (inTexturePath.empty())
// 	{
// 		inTexturePath = eastl::string("../Data/Textures/ExampleContainer.jpg");
// 	}
// 
// 	eastl::shared_ptr<SimpleShapeDrawable> obj = eastl::make_shared<TriangleShape>(inTexturePath);
// 
// 	return obj;
// }
// 
// TriangleShape::TriangleShape(const eastl::string& inTexturePath)
// {
// 	IndexBuffer ibo = IndexBuffer{};
// 	int32_t indicesCount = BasicShapesData::GetTriangleIndicesCount();
// 	ibo.SetIndices(BasicShapesData::GetTriangleIndices(), indicesCount, GL_STATIC_DRAW);
// 
// 	VertexBufferLayout layout = VertexBufferLayout{};
// 	// Vertex points
// 	layout.Push<float>(3);
// 	// Vertex Tex Coords
// 	layout.Push<float>(2);
// 
// 	VertexBuffer vbo = VertexBuffer{ ibo, layout };
// 	int32_t verticesCount = BasicShapesData::GetTriangleVerticesCount();
// 	vbo.SetVertices(BasicShapesData::GetTriangleVertices(), verticesCount, GL_STATIC_DRAW);
// 
// 	ObjectVAO.VBuffer = vbo;
// 	ObjectVAO.SetupState();
// 
// 	Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicTexFragmentShader.glsl");
// 
// 	OpenGLTexture tex{ inTexturePath, texureBaseNr + 0 };
// 	AddTexture(tex);
// }
// 
// TriangleShape::~TriangleShape() = default;

// eastl::shared_ptr<IDrawable> BasicShapes::CreateSquareObject(eastl::string inTexturePath)
// {
// 	if (inTexturePath.empty())
// 	{
// 		inTexturePath = eastl::string("../Data/Textures/ExampleContainer.jpg");
// 	}
// 
// 	eastl::shared_ptr<SimpleShapeDrawable> obj = eastl::make_shared<SquareShape>(inTexturePath);
// 
// 	return obj;
// }
// 
 eastl::shared_ptr<CubeShape> BasicShapes::CreateCubeObject()
 {
	eastl::shared_ptr<CubeShape> obj = ObjectCreation::NewObject<CubeShape>();

 	return obj;
 }

CubeShape::CubeShape() = default;
CubeShape::~CubeShape() = default;

void CubeShape::SetupDrawCommands()
{
	static eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };

	if (!thisVAO)
	{
		// TODO: Buffers creation should be delegated to the renderer
		IndexBuffer ibo = IndexBuffer{};
		int32_t indicesCount = BasicShapesData::GetCubeIndicesCount();
		ibo.SetIndices(BasicShapesData::GetCubeIndices(), indicesCount, GL_STATIC_DRAW);

		VertexBufferLayout layout = VertexBufferLayout{};
		// Vertex points
		layout.Push<float>(3);
		// Vertex Tex Coords
		layout.Push<float>(2);

		VertexBuffer vbo = VertexBuffer{ ibo, layout };
		int32_t verticesCount = BasicShapesData::GetCubeVerticesCount();
		vbo.SetVertices(BasicShapesData::GetCubeVertices(), verticesCount, GL_STATIC_DRAW);

		thisVAO = eastl::make_shared<VertexArrayObject>();
		thisVAO->VBuffer = vbo;
		thisVAO->SetupState();
	}

	MaterialsManager& matManager = MaterialsManager::Get();
	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial> cubeMaterial = matManager.GetOrAddMaterial("cube_material", materialExists);
	eastl::string texturePath = "../Data/Textures/ExampleContainer.jpg";

	if (!materialExists)
	{
		OpenGLTexture tex{ texturePath, texureBaseNr + 0 };
		cubeMaterial->Textures.push_back(tex);
		cubeMaterial->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/BasicTexFragmentShader.glsl");
	}

	eastl::shared_ptr<MeshNode> cubeNode = eastl::make_shared<MeshNode>();
	AddChild(cubeNode);

	RenderCommand newCommand;
	newCommand.Material = cubeMaterial;
	newCommand.VAO = thisVAO;
	newCommand.Parent = cubeNode;
	newCommand.DrawType = EDrawCallType::DrawArrays;

	RHI->AddCommand(newCommand);
}