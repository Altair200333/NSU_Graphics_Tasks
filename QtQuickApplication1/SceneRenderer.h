#pragma once
#include "Scene.h"

class SceneRenderer final
{
public:
	bool drawWireframe = false;
	void renderLights(Scene& scene)
	{
		for (auto& light : scene.lights)
		{
			scene.lightSourceBlock->transform.translate(-scene.lightSourceBlock->transform.position);
			scene.lightSourceBlock->transform.translate(light->position);
			scene.lightSourceBlock->material.shadingMode = Material::materialColor;
			scene.lightSourceBlock->material.color = light->color;
			scene.lightSourceBlock->material.isLightSource = true;
			scene.lightSourceBlock->renderer->render(scene.camera, scene.lights);
		}
	}
	void render(Scene& scene)
	{
		glPolygonMode(GL_FRONT_AND_BACK, drawWireframe ? GL_LINE : GL_FILL);

		for (size_t i = 0; i < scene.objects.size(); ++i)
		{
			scene.objects[i]->renderer->render(scene.camera, scene.lights);
		}

		renderLights(scene);
	}
};