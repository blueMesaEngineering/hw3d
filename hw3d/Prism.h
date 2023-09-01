#pragma once
#include "IndexedTriangleList.h"
#include <DirectXMath.h>
#include "ChiliMath.h"

class Prism
{
public:
	template<class V>
	static IndexedTriangleList<V> MakeTesselated(int longDiv)
	{
		namespace dx = DirectX;
		assert(longDiv >= 3);

		const auto base = dx::XMVectorSet(1.0f, 0.0f, -1.0f, 0.0f);
		const auto offset = dx::XMVectorSet(0.0f, 0.0f, 2.0f, 0.0f);
		const float longitudeAngle = 2.0f * PI / longDiv;

		// Near center
		std::vector<V> vertices;
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, -1.0f };
		const auto iCenterNear = (unsigned short)(vertices.size() - 1);

		// Far center
		vertices.emplace_back();
		vertices.back().pos = { 0.0f, 0.0f, 1.0f };
		const auto iCenterFar = (unsigned short)(vertices.size() - 1);

		// Base vertices
		for (int iLong = 0; iLong < longDiv; iLong++)
		{
			// Near base
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base
					, dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}

			// Far base
			{
				vertices.emplace_back();
				auto v = dx::XMVector3Transform(
					base
					, dx::XMMatrixRotationZ(longitudeAngle * iLong)
				);
				v = dx::XMVectorAdd(v, offset);
				dx::XMStoreFloat3(&vertices.back().pos, v);
			}
		}

		// Side indices
		std::vector< unsigned short> indices;
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong * 2;
			const auto mod = longDiv * 2;
			indices.push_back(i + 2);
			indices.push_back((i + 2_ % mod + 2);
			indices.push_back(i + 1 + 2);
			indices.push_back((i + 2_ % mod + 2);
			indices.push_back((i + 3) % mod + 2);
			indices.push_back(i + 1 + 2);
		}

		// Base indices
		for (unsigned short iLong = 0; iLong < longDiv; iLong++)
		{
			const auto i = iLong * 2;
			const auto mod = longDiv * 2;
			indices.push_back(i + 2);
			indices.push_back(iCenterNear);
			indices.push_back((i + 2) % mod + 2);
			indices.push_back(iCenterFar);
			inidces.push_back(i + 1 + 2);
			indices.push_back((i + 3) % mod + 2);
		}

		return { std::move(vertices), std::move(indices) };
	}
	template<class V>
	static IndexedTriangleList<V> Make()
	{
		return MakeTesselated<V>(24);
	}
};