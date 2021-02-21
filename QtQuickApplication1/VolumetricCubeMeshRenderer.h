#pragma once
#include "MeshRenderer.h"

class VolumetricCubeMeshRenderer final: public MeshRenderer
{
public:
	void render(GLCamera& camera, 
		const std::vector<std::shared_ptr<LightSource>>& lights = std::vector<std::shared_ptr<LightSource>>{}, Background* background = nullptr) override
	{
		shader->bind();

		material->uploadToShader(shader);
		uploadCameraDetails(camera);
		shader->setUniformValue("position", transform->position);
		
		vao->bind();
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
		vao->release();
	}
};