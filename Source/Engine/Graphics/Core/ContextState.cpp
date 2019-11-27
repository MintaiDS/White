#include "ContextState.h"
#include "InterfaceProvider.h"

namespace White {
namespace Engine {
namespace Graphics {

ContextState::ContextState() : isInitialized(false) {}

bool ContextState::IsInitialized() const {
    return isInitialized;
}

void ContextState::Init() {
    program.Create();
    vertexShader.Create(GL_VERTEX_SHADER);
    std::wstring path = modelFormat.shaders[0];
    vertexShader.Source(path);
    vertexShader.Compile();
    program.Attach(vertexShader);
    path = modelFormat.shaders[1];
    fragmentShader.Create(GL_FRAGMENT_SHADER);
    fragmentShader.Source(path);
    fragmentShader.Compile();
    program.Attach(fragmentShader);
    program.Link();
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);  
    renderData.Init();
    drawingCommand.UpdateCommand(modelFormat);
    isInitialized = true;
}

void ContextState::Activate() {
    program.Use();
    renderData.Activate();
}

void ContextState::Destroy() {
    program.Detach(vertexShader);
    program.Detach(fragmentShader);
    vertexShader.Delete();
    fragmentShader.Delete();
    program.Delete();
}

void ContextState::Render() {
    InterfaceProvider ip;
    std::vector<unsigned>& models = renderData.GetModels(); 
    int indicesCnt = 0;
    for (int i = 0; i < models.size(); i++) {
        auto modelObject = *ip.Query<Model>(models[i]);
        auto mesh = *ip.Query<Mesh<float>>(modelObject.GetMesh());
        Vector<GLfloat> rotation = mesh.GetRotation();
        Matrix<GLfloat> rotationMatrix 
            = Matrix<GLfloat>::Rotation({rotation[0], rotation[1], 
                                         rotation[2], 1.0f});  
        Vector<GLfloat> translation = mesh.GetTranslation();
        Matrix<GLfloat> translationMatrix
            = Matrix<GLfloat>::Translation(translation);
        Vector<GLfloat> scaling = mesh.GetScaling();
        Matrix<GLfloat> scalingMatrix 
            = Matrix<GLfloat>::Scaling({scaling[0], scaling[1], 
                                        scaling[2], 1.0f});
        Matrix<GLfloat> model = translationMatrix 
                                * rotationMatrix 
                                * scalingMatrix;
        std::unique_ptr<GLfloat[]> raw 
            = std::make_unique<GLfloat[]>(model.rows * model.columns);
        for (int i = 0; i < model.rows; i++) {
            for (int j = 0; j < model.columns; j++) {
                raw.get()[i * model.columns + j] = model[i][j];
            }
        }
        GLint location = glGetUniformLocation(program.id, "model");
        glProgramUniformMatrix4fv(program.id, location, 1, GL_TRUE, raw.get());
        DrawingCommand::Arguments args;
        args.topology = GL_TRIANGLES;
        args.indicesCnt = mesh.indices.size();
        args.indexType = GL_UNSIGNED_INT;
        args.offset  = reinterpret_cast<const void*>(sizeof(GLuint) 
                                                     * indicesCnt);
        renderData.UpdateData(models[i]);
        drawingCommand.UpdateData(args);
        drawingCommand.Invoke();
        indicesCnt += mesh.indices.size();
    }

}

void ContextState::SetRenderData(RenderData renderData) {
    this->renderData = renderData;
}

void ContextState::SetModelFormat(ModelFormat modelFormat) {
    this->modelFormat = modelFormat;
    renderData.SetModelFormat(modelFormat);
}

RenderData& ContextState::GetRenderData() {
    return renderData;
}

ModelFormat ContextState::GetModelFormat() {
    return modelFormat;
}

}
}
}
