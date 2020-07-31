#include "DynamicConstant.h"
#include "LayoutCodex.h"
#include <cstring>
#include "Vertex.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Material.h"
#include "Mesh.h"

namespace dx = DirectX;

void TestDynamicMeshLoading()
{
	using namespace dynamical;

	Assimp::Importer imp;
	const auto pScene = imp.ReadFile("Models\\brick_wall\\brick_wall.obj",
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);
	auto layout = VertexLayout{}
		.Append(VertexLayout::Position3D)
		.Append(VertexLayout::Normal)
		.Append(VertexLayout::Tangent)
		.Append(VertexLayout::Bitangent)
		.Append(VertexLayout::Texture2D);
	VertexBuffer buf{ std::move(layout),*pScene->mMeshes[0] };

	for (auto i = 0ull, end = buf.Size(); i < end; i++)
	{
		const auto a = buf[i].Attr<VertexLayout::Position3D>();
		const auto b = buf[i].Attr<VertexLayout::Normal>();
		const auto c = buf[i].Attr<VertexLayout::Tangent>();
		const auto d = buf[i].Attr<VertexLayout::Bitangent>();
		const auto e = buf[i].Attr<VertexLayout::Texture2D>();
	}
}

void TestMaterialSystemLoading(Graphics& gfx)
{
	std::string path = "Models\\brick_wall\\brick_wall.obj";
	Assimp::Importer imp;
	const auto pScene = imp.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace
	);
	Material mat{ gfx,*pScene->mMaterials[1],path };
	Mesh mesh{ gfx,mat,*pScene->mMeshes[0] };
}

void TestDynamicConstant()
{
	using namespace std::string_literals;
	// data roundtrip tests
	{
		dynamical::RawLayout s;
		s.Add<dynamical::Struct>("testa"s);
		s["testa"s].Add<dynamical::Float3>("testb"s);
		s["testa"s].Add<dynamical::Float>("testc"s);
		s.Add<dynamical::Float>("testd"s);
		s.Add<dynamical::Array>("arr"s);
		s["arr"s].Set<dynamical::Struct>(4);
		s["arr"s].T().Add<dynamical::Float3>("teste"s);
		s["arr"s].T().Add<dynamical::Array>("testf"s);
		s["arr"s].T()["testf"s].Set<dynamical::Float>(6);
		s["arr"s].T().Add<dynamical::Array>("testg"s);
		s["arr"s].T()["testg"s].Set<dynamical::Array>(6);
		s["arr"s].T()["testg"s].T().Set<dynamical::Matrix>(4);
		s["arr"s].T().Add<dynamical::Bool>("testh");

		// fails: duplicate symbol name
		// s.Add<dynamical::Bool>( "arr"s );

		// fails: bad symbol name
		//s.Add<dynamical::Bool>( "69man" );

		auto b = dynamical::Buffer(std::move(s));

		// fails to compile: conversion not in type map
		//b["testd"s] = "#"s;

		const auto sig = b.GetRootLayoutElement().GetSignature();


		{
			auto exp = 42.0f;
			b["testd"s] = exp;
			float act = b["testd"s];
			assert(act == exp);
		}
		{
			auto exp = 420.0f;
			b["testa"s]["testc"s] = exp;
			float act = b["testa"s]["testc"s];
			assert(act == exp);
		}
		{
			auto exp = 111.0f;
			b["arr"s][2]["testf"s][5] = exp;
			float act = b["arr"s][2]["testf"s][5];
			assert(act == exp);
		}
		{
			auto exp = DirectX::XMFLOAT3{ 69.0f,0.0f,0.0f };
			b["testa"s]["testb"s] = exp;
			dx::XMFLOAT3 act = b["testa"s]["testb"s];
			assert(!std::memcmp(&exp, &act, sizeof(DirectX::XMFLOAT3)));
		}
		{
			DirectX::XMFLOAT4X4 exp;
			dx::XMStoreFloat4x4(
				&exp,
				dx::XMMatrixIdentity()
			);
			b["arr"s][2]["testg"s][5][3] = exp;
			dx::XMFLOAT4X4 act = b["arr"s][2]["testg"s][5][3];
			assert(!std::memcmp(&exp, &act, sizeof(DirectX::XMFLOAT4X4)));
		}
		{
			auto exp = true;
			b["arr"s][2]["testh"s] = exp;
			bool act = b["arr"s][2]["testh"s];
			assert(act == exp);
		}
		{
			auto exp = false;
			b["arr"s][2]["testh"s] = exp;
			bool act = b["arr"s][2]["testh"s];
			assert(act == exp);
		}
		// exists
		{
			assert(b["testa"s]["testb"s].Exists());
			assert(!b["testa"s]["testi"s].Exists());
			if (auto ref = b["testa"s]["testb"s]; ref.Exists())
			{
				dx::XMFLOAT3 f = ref;
				assert(f.x == 69.0f);
			}
		}
		// set if exists
		{
			assert(b["testa"s]["testb"s].SetIfExists(dx::XMFLOAT3{ 1.0f,2.0f,3.0f }));
			auto& f3 = static_cast<const dx::XMFLOAT3&>(b["testa"s]["testb"s]);
			assert(f3.x == 1.0f && f3.y == 2.0f && f3.z == 3.0f);
			assert(!b["testa"s]["testj"s].SetIfExists(dx::XMFLOAT3{ 2.0f,2.0f,7.0f }));
		}

		const auto& cb = b;
		{
			dx::XMFLOAT4X4 act = cb["arr"s][2]["testg"s][5][3];
			assert(act._11 == 1.0f);
		}
		// this doesn't compile: buffer is const
		// cb["arr"][2]["testh"] = true;
		// static_cast<bool&>(cb["arr"][2]["testh"]) = true;

		// this fails assertion: array out of bounds
		// cb["arr"s][200];

	}
	// size test array of arrays
	{
		dynamical::RawLayout s;
		s.Add<dynamical::Array>("arr");
		s["arr"].Set<dynamical::Array>(6);
		s["arr"].T().Set<dynamical::Matrix>(4);
		auto b = dynamical::Buffer(std::move(s));

		auto act = b.GetSizeInBytes();
		assert(act == 16u * 4u * 4u * 6u);
	}
	// size test array of structs with padding
	{
		dynamical::RawLayout s;
		s.Add<dynamical::Array>("arr");
		s["arr"].Set<dynamical::Struct>(6);
		s["arr"s].T().Add<dynamical::Float2>("a");
		s["arr"].T().Add<dynamical::Float3>("b"s);
		auto b = dynamical::Buffer(std::move(s));

		auto act = b.GetSizeInBytes();
		assert(act == 16u * 2u * 6u);
	}
	// size test array of primitive that needs padding
	{
		dynamical::RawLayout s;
		s.Add<dynamical::Array>("arr");
		s["arr"].Set<dynamical::Float3>(6);
		auto b = dynamical::Buffer(std::move(s));

		auto act = b.GetSizeInBytes();
		assert(act == 16u * 6u);
	}
	// testing CookedLayout
	{
		dynamical::RawLayout s;
		s.Add<dynamical::Array>("arr");
		s["arr"].Set<dynamical::Float3>(6);
		auto cooked = dynamical::LayoutCodex::Resolve(std::move(s));
		// raw is cleared after donating
		s.Add<dynamical::Float>("arr");
		// fails to compile, cooked returns const&
		// cooked["arr"].Add<dynamical::Float>("buttman");
		auto b1 = dynamical::Buffer(cooked);
		b1["arr"][0] = dx::XMFLOAT3{ 69.0f,0.0f,0.0f };
		auto b2 = dynamical::Buffer(cooked);
		b2["arr"][0] = dx::XMFLOAT3{ 420.0f,0.0f,0.0f };
		assert(static_cast<dx::XMFLOAT3>(b1["arr"][0]).x == 69.0f);
		assert(static_cast<dx::XMFLOAT3>(b2["arr"][0]).x == 420.0f);
	}
	// specific testing scenario (packing error)
	{
		dynamical::RawLayout pscLayout;
		pscLayout.Add<dynamical::Float3>("materialColor");
		pscLayout.Add<dynamical::Float3>("specularColor");
		pscLayout.Add<dynamical::Float>("specularWeight");
		pscLayout.Add<dynamical::Float>("specularGloss");
		auto cooked = dynamical::LayoutCodex::Resolve(std::move(pscLayout));
		assert(cooked.GetSizeInBytes() == 48u);
	}
	// array non-packing
	{
		dynamical::RawLayout pscLayout;
		pscLayout.Add<dynamical::Array>("arr");
		pscLayout["arr"].Set<dynamical::Float>(10);
		auto cooked = dynamical::LayoutCodex::Resolve(std::move(pscLayout));
		assert(cooked.GetSizeInBytes() == 160u);
	}
	// array of struct w/ padding
	{
		dynamical::RawLayout pscLayout;
		pscLayout.Add<dynamical::Array>("arr");
		pscLayout["arr"].Set<dynamical::Struct>(10);
		pscLayout["arr"].T().Add<dynamical::Float3>("x");
		pscLayout["arr"].T().Add<dynamical::Float2>("y");
		auto cooked = dynamical::LayoutCodex::Resolve(std::move(pscLayout));
		assert(cooked.GetSizeInBytes() == 320u);
	}
	// testing pointer stuff
	{
		dynamical::RawLayout s;
		s.Add<dynamical::Struct>("testa"s);
		s["testa"s].Add<dynamical::Float3>("testb"s);
		s["testa"s].Add<dynamical::Float>("testc"s);

		auto b = dynamical::Buffer(std::move(s));
		const auto exp = 696969.6969f;
		b["testa"s]["testc"s] = 696969.6969f;
		assert((float&)b["testa"s]["testc"s] == exp);
		assert(*(float*)&b["testa"s]["testc"s] == exp);
		const auto exp2 = 42.424242f;
		*(float*)&b["testa"s]["testc"s] = exp2;
		assert((float&)b["testa"s]["testc"s] == exp2);
	}
	// specific testing scenario (packing error)
	{
		dynamical::RawLayout lay;
		lay.Add<dynamical::Bool>("normalMapEnabled");
		lay.Add<dynamical::Bool>("specularMapEnabled");
		lay.Add<dynamical::Bool>("hasGlossMap");
		lay.Add<dynamical::Float>("specularPower");
		lay.Add<dynamical::Float3>("specularColor");
		lay.Add<dynamical::Float>("specularMapWeight");

		auto buf = dynamical::Buffer(std::move(lay));
		assert(buf.GetSizeInBytes() == 32u);
	}
}