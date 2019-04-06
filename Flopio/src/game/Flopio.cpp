#include "Flopio.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include "../core/Engine.h"
#include "../core/graphics/rendercomps/TextureRC.h"
#include "../core/resourse/files/DirectoryResourceFile.h"
#include "../core/resourse/loaders/FragmentShaderResourceLoader.h"
#include "../core/resourse/loaders/VertexShaderResourceLoader.h"
#include "../core/resourse/loaders/TextureResourceLoader.h"
#include "../core/resourse/loaders/XmlResourceLoader.h"
#include "../core/graphics/viewports/ScreenViewport.h"
#include "../core/math/linearmath.h"
#include "../core/math/prng.h"
#include "../core/gamebasis/ActorFactory.h"
#include <filesystem>

namespace game
{
	using namespace engine;

	ScreenViewport viewport;

	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		((Flopio *)currentApp)->scrollCallback(window, xoffset, yoffset);
	}

	void drop_callback(GLFWwindow* window, int count, const char** paths)
	{
		((Flopio *)currentApp)->dropCallback(window, count, paths);
	}

	void Flopio::VOnResize(GLFWwindow* window, int width, int height) 
	{
		viewport.VResize(width, height);
	}

	void Flopio::VOnUpdate(const double delta)
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Hello, world!");
		if (ImGui::TreeNode("Keyboard, Mouse & Navigation State"))
		{

			ImGuiIO& io = ImGui::GetIO();

			if (ImGui::IsMousePosValid())
				ImGui::Text("Mouse pos: (%g, %g)", io.MousePos.x, io.MousePos.y);
			else
				ImGui::Text("Mouse pos: <INVALID>");
			ImGui::Text("Mouse delta: (%g, %g)", io.MouseDelta.x, io.MouseDelta.y);
			ImGui::Text("Mouse down:");     for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (io.MouseDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]); }
			ImGui::Text("Mouse clicked:");  for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
			ImGui::Text("Mouse dbl-clicked:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseDoubleClicked(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
			ImGui::Text("Mouse released:"); for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++) if (ImGui::IsMouseReleased(i)) { ImGui::SameLine(); ImGui::Text("b%d", i); }
			ImGui::Text("Mouse wheel: %.1f", io.MouseWheel);

			ImGui::Text("Keys down:");      for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (io.KeysDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("%d (%.02f secs)", i, io.KeysDownDuration[i]); }
			ImGui::Text("Keys pressed:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyPressed(i)) { ImGui::SameLine(); ImGui::Text("%d", i); }
			ImGui::Text("Keys release:");   for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++) if (ImGui::IsKeyReleased(i)) { ImGui::SameLine(); ImGui::Text("%d", i); }
			ImGui::Text("Keys mods: %s%s%s%s", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");

			ImGui::Text("NavInputs down:"); for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputs[i] > 0.0f) { ImGui::SameLine(); ImGui::Text("[%d] %.2f", i, io.NavInputs[i]); }
			ImGui::Text("NavInputs pressed:"); for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] == 0.0f) { ImGui::SameLine(); ImGui::Text("[%d]", i); }
			ImGui::Text("NavInputs duration:"); for (int i = 0; i < IM_ARRAYSIZE(io.NavInputs); i++) if (io.NavInputsDownDuration[i] >= 0.0f) { ImGui::SameLine(); ImGui::Text("[%d] %.2f", i, io.NavInputsDownDuration[i]); }

			ImGui::TreePop();
		}
		ImGui::End();

		updateCam(delta);
		root.update(delta);

		ImGui::Render();
		
	}

	void Flopio::VOnRender(const double ndelay)
	{
		root.render(ndelay);
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}


	DirectoryResourceFile resFile("Resources");
	FragmentShaderResourceLoader fragmentLoader;
	VertexShaderResourceLoader vertexLoader;
	TextureResourceLoader textureLoader;
	XmlResourceLoader xmlLoader;
	ActorFactory actorFactory;

	void Flopio::VOnInit()
	{
		secondsPerUpdate = 1.0 / 60.0;

		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplGlfw_InitForOpenGL(glfwWindowHandle, true);
		ImGui_ImplOpenGL3_Init("#version 450 core");

		root.setViewport(&viewport);
		int width, height;
		glfwGetWindowSize(glfwWindowHandle, &width, &height);
		glfwSetScrollCallback(glfwWindowHandle, scroll_callback);
		glfwSetDropCallback(glfwWindowHandle, drop_callback);
		viewport.VResize(width, height);

		textureLoader.generateMipmaps = false;

		resourceCache.addFile(std::shared_ptr<DirectoryResourceFile>(&resFile, [](DirectoryResourceFile*) {}));
		resourceCache.addLoader(std::shared_ptr<FragmentShaderResourceLoader>(&fragmentLoader, [](FragmentShaderResourceLoader*) {}));
		resourceCache.addLoader(std::shared_ptr<VertexShaderResourceLoader>(&vertexLoader, [](VertexShaderResourceLoader*) {}));
		resourceCache.addLoader(std::shared_ptr<TextureResourceLoader>(&textureLoader, [](TextureResourceLoader*) {}));
		resourceCache.addLoader(std::shared_ptr<XmlResourceLoader>(&xmlLoader, [](XmlResourceLoader*) {}));
		
		TextureRC::init();
		Scene::registerRenderer(Component::getId(TextureRC::name), &TextureRC::render);

		//std::sort(root.actors.begin(), root.actors.end(), [](SharedActor f, SharedActor l) 
		//{
		//	return f->getPosition().z < l->getPosition().z;
		//});
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Render();
	}

	void Flopio::VOnExit()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void Flopio::scrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		root.camera.set(root.camera.getPosition(), root.camera.getAngle(), root.camera.getScale() + root.camera.getScale()*yoffset*0.1f);
	}

	void Flopio::dropCallback(GLFWwindow* window, int count, const char** paths)
	{
		int width, height;
		double xpos, ypos;
		float angle = root.camera.getAngle();
		glfwGetWindowSize(window, &width, &height);
		glfwGetCursorPos(window, &xpos, &ypos);
		vec4 pos = vec4CreateSse(xpos - width / 2, height / 2 - ypos, 1, 1);
		pos.ssevalue = sseVecMat44Multiply(pos.ssevalue, &mat44TransformInverse(root.camera.getView()));
		vec3 pos3 = vec3Create(pos.x, pos.y, 1.f);

		for (int i = 0; i < count; i++)
		{
			std::filesystem::path path(paths[i]);
			
			std::string pathString = path.string();
			pathString.replace(pathString.find_last_of(std::filesystem::path::preferred_separator), 1, &Resource::separator);
			Resource res(pathString);
			std::shared_ptr<DirectoryResourceFile> resFile(new DirectoryResourceFile (pathString.substr(0, res.getSeparatorPos())));
			resourceCache.addFile(resFile);
			if (SharedActor newActor = actorFactory.createActor(res, &pos3, &angle))
			{
				root.addActor(newActor);
			}
		}
	}

	void Flopio::updateCam(const double delta)
	{
		const float speed = 400.f;
		Camera & cam = root.camera;
		float angle = cam.getAngle();
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_W) == GLFW_PRESS)
		{
			cam.set(vec2Add(cam.getPosition(), vec2Scale({ -sinf(angle), cosf(angle) }, speed * (float)delta * (1 / cam.getScale()))), cam.getAngle(), cam.getScale());
		}
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_S) == GLFW_PRESS)
		{
			cam.set(vec2Add(cam.getPosition(), vec2Scale({ sinf(angle), -cosf(angle) }, speed * (float)delta * (1 / cam.getScale()))), cam.getAngle(), cam.getScale());
		}
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_A) == GLFW_PRESS)
		{
			cam.set(vec2Add(cam.getPosition(), vec2Scale({ -cosf(angle), -sinf(angle) }, speed * (float)delta * (1 / cam.getScale()))), cam.getAngle(), cam.getScale());
		}
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_D) == GLFW_PRESS)
		{
			cam.set(vec2Add(cam.getPosition(), vec2Scale({ cosf(angle), sinf(angle) }, speed * (float)delta * (1 / cam.getScale()))), cam.getAngle(), cam.getScale());
		}
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_Q) == GLFW_PRESS)
		{
			cam.set(cam.getPosition(), cam.getAngle() + 1.f* delta, cam.getScale());
		}
		if (glfwGetKey(glfwWindowHandle, GLFW_KEY_E) == GLFW_PRESS)
		{
			cam.set(cam.getPosition(), cam.getAngle() + -1.f* delta, cam.getScale());
		}
	}
}
