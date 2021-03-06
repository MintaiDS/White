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
    renderData.SetModelFormat(modelFormat);
    renderData.Init();
    drawingCommand.UpdateCommand(modelFormat);
    renderData.Update();
    isInitialized = true;
}

void ContextState::UpdateCamera(Matrix<float> view, Matrix<float> projection) {
    std::unique_ptr<GLfloat[]> raw 
        = std::make_unique<GLfloat[]>(view.rows * view.columns);

    for (int i = 0; i < view.rows; i++) {
        for (int j = 0; j < view.columns; j++) {
            raw.get()[i * view.columns + j] = view[i][j];
        }
    }
    GLint location = glGetUniformLocation(program.id, "view");
    glUniformMatrix4fv(location, 1, GL_TRUE, raw.get());

    for (int i = 0; i < projection.rows; i++) {
        for (int j = 0; j < projection.columns; j++) {
            raw.get()[i * projection.columns + j] = projection[i][j];
        }
    }
    location = glGetUniformLocation(program.id, "projection");
    glUniformMatrix4fv(location, 1, GL_TRUE, raw.get());
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
        auto modelObject = ip.Query<Model>(models[i]);
        auto mesh = ip.Query<Mesh<float>>(modelObject->GetMesh());
        //Vector<GLfloat> rotation = mesh->GetRotation();
        //Matrix<GLfloat> rotationMatrix 
        //    = Matrix<GLfloat>::Rotation({rotation[0], rotation[1], 
        //                                 rotation[2], 1.0f});  
        //Vector<GLfloat> translation = mesh->GetTranslation();
        //Matrix<GLfloat> translationMatrix
        //    = Matrix<GLfloat>::Translation(translation);
        //Vector<GLfloat> scaling = mesh->GetScaling();
        //Matrix<GLfloat> scalingMatrix 
        //    = Matrix<GLfloat>::Scaling({scaling[0], scaling[1], 
        //                        * rotationMatrix 
        //                        * scalingMatrix;
        //std::unique_ptr<GLfloat[]> raw 
        //    = std::make_unique<GLfloat[]>(model.rows * model.columns);
        //for (int i = 0; i < model.rows; i++) {
        //    for (int j = 0; j < model.columns; j++) {
        //        raw.get()[i * model.columns + j] = model[i][j];
        //    }
        //}
        Matrix<float>& model = mesh->GetTransformationMatrix();
        GLint location = glGetUniformLocation(program.id, "model");
        glUniformMatrix4fv(location, 1, GL_TRUE, model.GetDataPtr());
        DrawingCommand::Arguments args;
        args.topology = GL_TRIANGLES;
        args.indicesCnt = mesh->indices.size();
        args.indexType = GL_UNSIGNED_INT;
        args.offset = reinterpret_cast<const void*>(sizeof(GLuint) 
                                                    * indicesCnt);
        renderData.UpdateData(models[i]);
        drawingCommand.UpdateData(args);
        drawingCommand.Invoke();
        indicesCnt += mesh->indices.size();
    }

}

void ContextState::SetRenderData(RenderData renderData) {
    this->renderData = renderData;
    renderData.SetModelFormat(modelFormat);
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
