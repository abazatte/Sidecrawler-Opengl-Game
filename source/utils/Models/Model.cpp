

#include "utils/Models/Model.h"
#include "../shader/PhongShader.h"
#include <list>


Model::Model() : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0) {
}

Model::Model(const char *ModelFile, bool FitSize) : pMeshes(NULL), MeshCount(0), pMaterials(NULL), MaterialCount(0) {
    bool ret = load(ModelFile, FitSize);
    if (!ret)
        throw std::exception();
}

Model::~Model() {
    // TODO: Add your code (Exercise 3)
    // Speicher freigeben

    delete[] pMeshes;
    delete[] pMaterials;
    deleteNodes(&RootNode);
}

void Model::deleteNodes(Node *pNode) {
    if (!pNode)
        return;
    for (unsigned int i = 0; i < pNode->ChildCount; ++i)
        deleteNodes(&(pNode->Children[i]));
    if (pNode->ChildCount > 0)
        delete[] pNode->Children;
    if (pNode->MeshCount > 0)
        delete[] pNode->Meshes;
}

bool Model::load(const char *ModelFile, bool FitSize) {
    const aiScene *pScene = aiImportFile(ModelFile, aiProcessPreset_TargetRealtime_Fast | aiProcess_TransformUVCoords);

    if (pScene == NULL || pScene->mNumMeshes <= 0)
        return false;

    Filepath = ModelFile;
    Path = Filepath;
    size_t pos = Filepath.rfind('/');
    if (pos == std::string::npos)
        pos = Filepath.rfind('\\');
    if (pos != std::string::npos)
        Path.resize(pos + 1);

    loadMeshes(pScene, FitSize);
    loadMaterials(pScene);
    loadNodes(pScene);

    return true;
}

void Model::loadMeshes(const aiScene *pScene, bool FitSize) {
    //scaling
    calcBoundingBox(pScene, BoundingBox);
    const float maxSize = 5;
    Vector boundDim = BoundingBox.size();

    float maxDim = boundDim.X > boundDim.Y ? boundDim.X : boundDim.Y;
    maxDim = maxDim > boundDim.Z ? maxDim : boundDim.Z;
    double scaleFactor = maxSize / maxDim;

    MeshCount = pScene->mNumMeshes;
    pMeshes = new Mesh[MeshCount];
    for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
        aiMesh *m = (pScene->mMeshes[i]);
        pMeshes[i].MaterialIdx = m->mMaterialIndex;

        pMeshes[i].VB.begin();

        for (unsigned int j = 0; j < m->mNumVertices; j++) {
            pMeshes[i].VB.addNormal(m->mNormals[j].x, m->mNormals[j].y, m->mNormals[j].z);

            if (m->mTextureCoords[0] != nullptr) {
                pMeshes[i].VB.addTexcoord0(m->mTextureCoords[0][j].x, -m->mTextureCoords[0][j].y, 0);
            }

            // Tangente und Bitangenten raussaugen
            if (m->HasTangentsAndBitangents()) {
                hasNormals = true;
                pMeshes[i].VB.addTexcoord1(m->mTangents->x, m->mTangents->y, m->mTangents->z);
                pMeshes[i].VB.addTexcoord2(m->mBitangents->x, m->mBitangents->y, m->mBitangents->z);
            }


            Vector vertex(m->mVertices[j].x, m->mVertices[j].y, m->mVertices[j].z);;
            if (FitSize) {
                vertex = vertex * scaleFactor;
            }
            pMeshes[i].VB.addVertex(vertex);
        }

        pMeshes[i].VB.end();


        pMeshes[i].IB.begin();

        for (unsigned int j = 0; j < m->mNumFaces; j++) {
            aiFace face = m->mFaces[j];
            for (unsigned int k = 0; k < face.mNumIndices; k++) {
                pMeshes[i].IB.addIndex(face.mIndices[k]);
            }
        }

        pMeshes[i].IB.end();
    }
}

void Model::loadMaterials(const aiScene *pScene) {
    // TODO: Add your code (Exercise 3)
    MaterialCount = pScene->mNumMaterials;
    pMaterials = new Material[MaterialCount];

    aiString path;

    for (int i = 0; i < MaterialCount; i++) {
        aiMaterial *aiMaterial = pScene->mMaterials[i];


        int numTextures = pScene->mMaterials[i]->GetTextureCount(aiTextureType_DIFFUSE);
        if (numTextures > 0) {
            pScene->mMaterials[i]->Get(AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), path);
            std::string textureFileName = Path + path.data;
            pMaterials[i].DiffTex = Texture::LoadShared(textureFileName.c_str());
        }


        //Diffusion laden
        aiColor3D color;
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color)) {
            pMaterials[i].DiffColor = Color(color.r, color.g, color.b);
        }

        //Specular Color laden
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color)) {
            pMaterials[i].SpecColor = Color(color.r, color.g, color.b);
        }

        //Ambient Color laden
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color)) {
            pMaterials[i].AmbColor = Color(color.r, color.g, color.b);
        }

        //Specular exponent laden
        float exponent;
        if (AI_SUCCESS == aiMaterial->Get(AI_MATKEY_SHININESS, exponent)) {
            pMaterials[i].SpecExp = exponent;
        }


        // Hier 2.1.b)
        // Pruefe ob eine Diffuse-Texturdatei mit dem Namen
        //<name>.<ext>(AI_MATKEY_TEXTURE_DIFFUSE) gibt...
        if (hasNormals) {
            std::string filePath = Path + path.data;

            if (filePath != Path) {
                int pos = filePath.rfind('.');
                std::string normalFilePath = filePath;
                normalFilePath.insert(pos, "_n");

                pMaterials[i].DiffTex = Texture::LoadShared(filePath.data());
                if (fopen(normalFilePath.data(), "r") != nullptr) {
                    pMaterials[i].NormalMap = Texture::LoadShared(normalFilePath.data());
                } else {
                    pMaterials[i].NormalMap = nullptr;
                }
            }
        }


    }
}


void Model::calcBoundingBox(const aiScene *pScene, AABB &Box) {
    Vector *min = &Box.Min;
    Vector *max = &Box.Max;
    min->X = min->Y = min->Z = std::numeric_limits<float>::max();
    max->X = max->Y = max->Z = std::numeric_limits<float>::lowest();

    for (unsigned int i = 0; i < pScene->mNumMeshes; i++) {
        aiMesh *m = pScene->mMeshes[i];
        for (unsigned int j = 0; j < m->mNumVertices; j++) {
            aiVector3D v = m->mVertices[j];
            min->X = min->X < v.x ? min->X : v.x;
            min->Y = min->Y < v.y ? min->Y : v.y;
            min->Z = min->Z < v.z ? min->Z : v.z;

            max->X = max->X > v.x ? max->X : v.x;
            max->Y = max->Y > v.y ? max->Y : v.y;
            max->Z = max->Z > v.z ? max->Z : v.z;
        }
    }
}


void Model::loadNodes(const aiScene *pScene) {
    deleteNodes(&RootNode);
    copyNodesRecursive(pScene->mRootNode, &RootNode);
}

void Model::copyNodesRecursive(const aiNode *paiNode, Node *pNode) {
    pNode->Name = paiNode->mName.C_Str();
    pNode->Trans = convert(paiNode->mTransformation);

    if (paiNode->mNumMeshes > 0) {
        pNode->MeshCount = paiNode->mNumMeshes;
        pNode->Meshes = new int[pNode->MeshCount];
        for (unsigned int i = 0; i < pNode->MeshCount; ++i)
            pNode->Meshes[i] = (int) paiNode->mMeshes[i];
    }

    if (paiNode->mNumChildren <= 0)
        return;

    pNode->ChildCount = paiNode->mNumChildren;
    pNode->Children = new Node[pNode->ChildCount];
    for (unsigned int i = 0; i < paiNode->mNumChildren; ++i) {
        copyNodesRecursive(paiNode->mChildren[i], &(pNode->Children[i]));
        pNode->Children[i].Parent = pNode;
    }
}

void Model::applyMaterial(unsigned int index) {
    if (index >= MaterialCount)
        return;

    PhongShader *pPhong = dynamic_cast<PhongShader *>(shader());
    if (!pPhong) {
        //std::cout <<
        //	"Model::applyMaterial(): WARNING Invalid Shader-type. Please apply PhongShader for rendering models.\n";
        return;
    }

    Material *pMat = &pMaterials[index];
    pPhong->ambientColor(pMat->AmbColor);
    pPhong->diffuseColor(pMat->DiffColor);
    pPhong->specularExp(pMat->SpecExp);
    pPhong->specularColor(pMat->SpecColor);
    pPhong->diffuseTexture(pMat->DiffTex);
    // Wurde hinzugefuegt!
    if (hasNormals) {
        pPhong->normalTexture(pMat->NormalMap);
    }

}

void Model::draw(const BaseCamera &Cam) {
    if (!pShader) {
        std::cout << "BaseModel::draw() no Shader found" << std::endl;
        return;
    }
    pShader->modelTransform(transform());

    std::list<Node *> DrawNodes;
    DrawNodes.push_back(&RootNode);

    while (!DrawNodes.empty()) {
        Node *pNode = DrawNodes.front();
        Matrix GlobalTransform;

        if (pNode->Parent != NULL)
            pNode->GlobalTrans = pNode->Parent->GlobalTrans * pNode->Trans;
        else
            pNode->GlobalTrans = transform() * pNode->Trans;

        pShader->modelTransform(pNode->GlobalTrans);

        for (unsigned int i = 0; i < pNode->MeshCount; ++i) {
            Mesh &mesh = pMeshes[pNode->Meshes[i]];
            mesh.VB.activate();
            mesh.IB.activate();
            applyMaterial(mesh.MaterialIdx);
            pShader->activate(Cam);
            glDrawElements(GL_TRIANGLES, mesh.IB.indexCount(), mesh.IB.indexFormat(), 0);
            mesh.IB.deactivate();
            mesh.VB.deactivate();
        }
        for (unsigned int i = 0; i < pNode->ChildCount; ++i)
            DrawNodes.push_back(&(pNode->Children[i]));

        DrawNodes.pop_front();
    }
}

Matrix Model::convert(const aiMatrix4x4 &m) {
    return Matrix(m.a1, m.a2, m.a3, m.a4,
                  m.b1, m.b2, m.b3, m.b4,
                  m.c1, m.c2, m.c3, m.c4,
                  m.d1, m.d2, m.d3, m.d4);
}
