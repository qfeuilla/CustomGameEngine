#include "App.h"
#include <algorithm>
#include "CustomMath.h"
#include "imgui/imgui.h"
#include "StringUtils.h"
#include "Testing.h"
#include "PerfLog.h"
#include "TestModelProbe.h"

namespace dx = DirectX;

App::App( const std::string& commandLine )
	:
	commandLine( commandLine ),
	wnd( WIDTH,HEIGHT,"Custom Game engine" ),
	scriptCommander( TokenizeQuoted( commandLine ) ),
	light( wnd.Gfx() )
{
	cube.SetPos( { 4.0f,0.0f,0.0f } );
	cube.SetScaling(1.2f, 2.1f, 1.8f);
	cube2.SetPos( { 0.0f,4.0f,0.0f } );
	nano.SetRootTransform(
		dx::XMMatrixRotationY(PI / 2.f) *
		dx::XMMatrixTranslation(27.f, -0.56f, 1.7f)
	);
	gobber.SetRootTransform(
		dx::XMMatrixRotationY(-PI / 2.f) *
		dx::XMMatrixTranslation(-8.f, 10.f, 0.f)
	);
	shark.SetRootTransform(
		dx::XMMatrixTranslation(0.0f, 50.0f, 0.f)
	);

	cube.LinkTechniques(rg);
	cube2.LinkTechniques(rg);
	light.LinkTechniques(rg);
	shark.LinkTechniques(rg);
	gobber.LinkTechniques(rg);
	nano.LinkTechniques(rg);

	wnd.Gfx().SetProjection( dx::XMMatrixPerspectiveLH( 1.0f,9.0f / 16.0f,0.5f,400.0f ) );
}

void App::HandleInput(float dt)
{
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
		{
			continue;
		}
		switch (e->GetCode())
		{
		case VK_ESCAPE:
			if (wnd.CursorEnabled())
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
			break;
		case VK_F1:
			showDemoWindow = true;
			break;
		}
	}
}

void App::DoFrame(float dt)
{
	wnd.Gfx().BeginFrame(0.07f, 0.0f, 0.12f);
	wnd.Gfx().SetCamera(cam.GetMatrix());
	light.Bind(wnd.Gfx(), cam.GetMatrix());

	light.Submit();
	cube.Submit();
	shark.Submit();
	cube2.Submit();
	gobber.Submit();
	nano.Submit();

	rg.Execute(wnd.Gfx());

	// imgui windows
	static MP sponzeProbe{ "Sponza" };
	static MP gobberProbe{ "Gobber" };
	static MP nanoProbe{ "Nano" };
	sponzeProbe.SpawnWindow(shark);
	gobberProbe.SpawnWindow(gobber);
	nanoProbe.SpawnWindow(nano);
	cam.SpawnControlWindow();
	light.SpawnControlWindow();
	ShowImguiDemoWindow();
	cube.SpawnControlWindow(wnd.Gfx(), "Cube 1");
	cube2.SpawnControlWindow(wnd.Gfx(), "Cube 2");
	rg.RenderWidgets(wnd.Gfx());

	// present
	wnd.Gfx().EndFrame();
	rg.Reset();
}

void App::ShowImguiDemoWindow()
{
	if( showDemoWindow )
	{
		ImGui::ShowDemoWindow( &showDemoWindow );
	}
}

void App::FPSCamRotation()
{
	while (const auto delta = wnd.mouse.ReadRawDelta()) {
		if (!wnd.CursorEnabled())
		{
			cam.Rotate((float)delta->x, (float)delta->y);
		}
	}
}

void App::FPSCamTranslation(float dt)
{
	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed('Z'))
		{
			cam.Translate({ 0.0f,0.0f,dt });
		}
		if (wnd.kbd.KeyIsPressed('Q'))
		{
			cam.Translate({ -dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			cam.Translate({ 0.0f,0.0f,-dt });
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			cam.Translate({ dt,0.0f,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('E'))
		{
			cam.Translate({ 0.0f,dt,0.0f });
		}
		if (wnd.kbd.KeyIsPressed('A'))
		{
			cam.Translate({ 0.0f,-dt,0.0f });
		}
	}
}

App::~App()
{}


int App::Go()
{
	while( true )
	{
		// process all messages pending, but to not block for new messages
		if( const auto ecode = Window::ProcessMessages() )
		{
			// if return optional has value, means we're quitting so return exit code
			return *ecode;
		}
		const auto dt = timer.Mark() * speed_factor;
		HandleInput(dt);
		FPSCamRotation();
		FPSCamTranslation(dt);
		DoFrame(dt);
	}
}