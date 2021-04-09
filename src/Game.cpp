#include "Game.h"
#include "BasicEntities.h"

Coordinator gCoordinator;

void Game::initECS()
{
    gCoordinator.Init();
    _entities.reserve(MAX_ENTITIES);

    gCoordinator.RegisterComponent<Transform>();
    gCoordinator.RegisterComponent<Mesh>();
    gCoordinator.RegisterComponent<Camera>();
    gCoordinator.RegisterComponent<Material>();
    gCoordinator.RegisterComponent<Fluid2D>();

    _physicsSys = gCoordinator.RegisterSystem<Physics>();
    _meshRendererSys = gCoordinator.RegisterSystem<MeshRenderer>();
    Camera camera
    {
        .yaw = 90,
        .pitch = -10,
        .speed = 0.1f,
        .FOV = 60,
        .transform = Transform
            {
                .position = {0.065, 5, -12},
                .rotation = {0, 0, 0},
                .scale = {1, 1, 1}
            }
    };
    _meshRendererSys->init(std::make_shared<Renderer>(_renderer), camera);
    _fluidsSys = gCoordinator.RegisterSystem<Fluids>();

    Signature signaturePhysics;
    signaturePhysics.set(gCoordinator.GetComponentType<Transform>());
    gCoordinator.SetSystemSignature<Physics>(signaturePhysics);

    Signature signatureMeshRenderer;
    signatureMeshRenderer.set(gCoordinator.GetComponentType<Transform>());
    signatureMeshRenderer.set(gCoordinator.GetComponentType<Mesh>());
    signatureMeshRenderer.set(gCoordinator.GetComponentType<Material>());
    gCoordinator.SetSystemSignature<MeshRenderer>(signatureMeshRenderer);

    Signature signatureFluids;
    signatureFluids.set(gCoordinator.GetComponentType<Transform>());
    signatureFluids.set(gCoordinator.GetComponentType<Fluid2D>());
    gCoordinator.SetSystemSignature<Fluids>(signatureFluids);
}

void Game::run(WindowInfos windowInfos)
{
	_window->init(windowInfos);
	_renderer.init(_window);
    initECS();

    BasicEntities::addFluid2D(glm::vec3{0,0,0}, glm::vec3{0,0,0}, glm::vec3{10,10,10});
    
	mainLoop();
}

void Game::mainLoop()
{
    float dt = 0.0f;
	while (!_window->windowShouldClose())
	{
        auto startTime = std::chrono::high_resolution_clock::now();

        _physicsSys->update(dt);
        _fluidsSys->update();
        _renderer.prePass();
        _meshRendererSys->update();

        _window->swapBuffers();
		_window->pollEvents();

        auto stopTime = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        //std::cout << 1/dt << std::endl;
	}
}

