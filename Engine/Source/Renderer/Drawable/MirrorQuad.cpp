#include "Renderer/Drawable/MirrorQuad.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapesData.h"
#include "Renderer/OpenGL/Buffer/IndexBuffer.h"
#include "Renderer/OpenGL/Buffer/VertexBufferLayout.h"
#include "Renderer/OpenGL/Buffer/VertexBuffer.h"
#include "Renderer/OpenGL/Buffer/VertexArrayObject.h"
#include "Renderer/OpenGL/OpenGLShader.h"
#include "Renderer/OpenGL/OpenGLTexture.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/OpenGL/RenderCommand.h"
#include "Renderer/OpenGL/OpenGLRenderer.h"
#include "Core/ObjectCreation.h"

MirrorQuad::MirrorQuad() = default;
MirrorQuad::~MirrorQuad() = default;

void MirrorQuad::SetupDrawCommands()
{
	const eastl::string vaoName = "mirrorVAO";
	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
	ASSERT(false); // Not working with Generic renderer
	//const bool existingVAO = RHI->GetOrCreateVAO(vaoName, thisVAO); 
	const bool existingVAO = false;

	if (!existingVAO)
	{
		// TODO: Buffers creation should be delegated to the renderer
		IndexBuffer ibo = IndexBuffer{};
		int32_t indicesCount = BasicShapesData::GetQuadIndicesCount();
		ibo.SetIndices(BasicShapesData::GetQuadIndices(), indicesCount, GL_STATIC_DRAW);

		VertexBufferLayout layout = VertexBufferLayout{};
		// Vertex points
		layout.Push<float>(3);
		// Vertex Tex Coords
		layout.Push<float>(2);

		VertexBuffer vbo = VertexBuffer{ ibo, layout };
		int32_t verticesCount = BasicShapesData::GetQuadVerticesCount();
		vbo.SetData(BasicShapesData::GetQuadVertices(), verticesCount, GL_STATIC_DRAW);

		thisVAO->VBuffer = vbo;
	}
	// Mirrors have to have unique materials
	static int mirrorNr = 0;
	const eastl::string mirrorMaterialName(eastl::string::CtorSprintf{}, "mirror_material_%d", ++mirrorNr);

	MaterialsManager& matManager = MaterialsManager::Get();
	bool materialExists = false;
	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial(mirrorMaterialName, materialExists);
	//eastl::string texturePath = "../Data/Textures/openGLExampleTransparentWindow.png";

	if (!materialExists)
	{
		//OpenGLTexture tex{ texturePath, texureBaseNr + 0 };
		//cubeMaterial->Textures.push_back(tex);
		material->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/QuadTexFragmentShader.glsl");
	}

	//material->Textures.push_back(OpenGLTexture());

	RenderCommand newCommand;
	newCommand.Material = material;
	newCommand.VAO = thisVAO;
	newCommand.Parent = this_shared(this);
	newCommand.DrawType = EDrawCallType::DrawElements;

	//RHI->AddMirrorCommand(newCommand);

}