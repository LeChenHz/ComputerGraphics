#ifndef MODEL_H
#define MODEL_H

#include "stb_image.h"
#include "Shader.h"
#include "Mesh.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <vector>
using namespace std;

class Model
{
public:
	vector<Texture> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	vector<Mesh *> meshes;
	string directory;
	bool gammaCorrection;

	Model(string const &path, bool gamma = false);
	~Model();

	void Draw(Shader *shader);

private:
	unsigned int TextureFromFile(const char *path, const string &directory, bool gamma = false);

	void loadModel(string const &path);

	void processNode(aiNode *node, const aiScene *scene);

	Mesh * processMesh(aiMesh *mesh, const aiScene *scene);

	vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
};


#endif