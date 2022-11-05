#include "Renderer/Drawable/MirrorQuad.h"
#include "Renderer/Drawable/ShapesUtils/BasicShapesData.h"
#include "Renderer/RHI/Resources/RenderDataContainer.h"
#include "Renderer/Material/MaterialsManager.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/ForwardRenderer.h"
#include "Core/ObjectCreation.h"

MirrorQuad::MirrorQuad() = default;
MirrorQuad::~MirrorQuad() = default;

void MirrorQuad::CreateProxy()
{
// 	const eastl::string vaoName = "mirrorVAO";
// 	eastl::shared_ptr<VertexArrayObject> thisVAO{ nullptr };
// 	ASSERT(false); // Not working with Generic renderer
// 	//const bool existingVAO = RHI->GetOrCreateVAO(vaoName, thisVAO); 
// 	const bool existingVAO = false;
// 
// 	if (!existingVAO)
// 	{
// 		// TODO: Buffers creation should be delegated to the renderer
// 		OpenGLIndexBuffer ibo = OpenGLIndexBuffer{};
// 		int32_t indicesCount = BasicShapesData::GetQuadIndicesCount();
// 		ibo.SetIndices(BasicShapesData::GetQuadIndices(), indicesCount);
// 
// 		VertexBufferLayout layout = VertexBufferLayout{};
// 		// Vertex points
// 		layout.Push<float>(3);
// 		// Vertex Tex Coords
// 		layout.Push<float>(2);
// 
// 		OpenGLVertexBuffer vbo = OpenGLVertexBuffer{ ibo, layout };
// 		int32_t verticesCount = BasicShapesData::GetQuadVerticesCount();
// 		vbo.SetData(BasicShapesData::GetQuadVertices(), verticesCount);
// 
// 		//thisVAO->VBuffer = vbo;// TODO
// 	}
// 	// Mirrors have to have unique materials
// 	static int mirrorNr = 0;
// 	const eastl::string mirrorMaterialName(eastl::string::CtorSprintf{}, "mirror_material_%d", ++mirrorNr);
// 
// 	MaterialsManager& matManager = MaterialsManager::Get();
// 	bool materialExists = false;
// 	eastl::shared_ptr<RenderMaterial> material = matManager.GetOrAddMaterial(mirrorMaterialName, materialExists);
// 	//eastl::string texturePath = "../Data/Textures/openGLExampleTransparentWindow.png";
// 
// 	if (!materialExists)
// 	{
// 		//OpenGLTexture tex{ texturePath, texureBaseNr + 0 };
// 		//cubeMaterial->Textures.push_back(tex);
// 		material->Shader = OpenGLShader::ConstructShaderFromPath("../Data/Shaders/BasicProjectionVertexShader.glsl", "../Data/Shaders/QuadTexFragmentShader.glsl");
// 	}
// 
// 	//material->Textures.push_back(OpenGLTexture());
// 
// 	RenderCommand newCommand;
// 	newCommand.Material = material;
// 	newCommand.VAO = thisVAO;
// 	newCommand.Parent = this_shared(this);
// 	newCommand.DrawType = EDrawCallType::DrawElements;
// 
// 	//RHI->AddMirrorCommand(newCommand);

}