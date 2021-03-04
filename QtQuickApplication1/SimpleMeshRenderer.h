#pragma once
#include "MeshRenderer.h"

class SimpleMeshRenderer final: public MeshRenderer
{
public:

	void bindAlbedo()
	{
		shader->setUniformValue("albedoCount", static_cast<int>(material->textures.size()));
		if(!material->textures.empty())
		{
			shader->setUniformValue("texture_diffuse", 0);
			functions->glActiveTexture(GL_TEXTURE0 + 0);
			material->textures[0].texture->bind();
		}
	}
	
	void render(GLCamera& camera, const std::vector<std::shared_ptr<LightSource>>& lights = std::vector<std::shared_ptr<LightSource>>{}, Background* background = nullptr) override
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		shader->bind();
		material->uploadToShader(shader);
		uploadCameraDetails(camera);
		uploadLights(lights);

		bindAlbedo();

		if(background!=nullptr)
		{
			shader->setUniformValue("texture_background", 1);
			functions->glActiveTexture(GL_TEXTURE0 + 1);
			background->image->bind();
		}
		
		shader->setUniformValue("useBackground", background != nullptr);
		shader->setUniformValue("isLightSource", material->isLightSource);

		shader->setUniformValue("wireframe", false);
		vao->bind();
		glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, 0);
		vao->release();

	}
	void uploadLights(const std::vector<std::shared_ptr<LightSource>>& lights) const
	{
		shader->setUniformValue("lightsCount", static_cast<int>(lights.size()));
		for (int i = 0; i < lights.size(); ++i)
		{
			shader->setUniformValue(("lights[" + std::to_string(i) + "].position").c_str(), lights[i]->position);
			shader->setUniformValue(("lights[" + std::to_string(i) + "].color").c_str(), lights[i]->color);
		}
	}
};
