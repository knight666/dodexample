#include "voxels/oop/LogicOOP.hpp"

namespace Tmpl {

	LogicOOP::LogicOOP()
		: m_voxelsActive(0)
	{
	}

	LogicOOP::~LogicOOP()
	{
	}

	bool LogicOOP::initialize(
		const Options& options,
		const std::vector<VoxelData>& voxels)
	{
		m_voxels.resize(voxels.size());
		m_rays.resize(voxels.size());

		m_voxelsActive = 0;

		for (const auto& voxel : voxels)
		{
			m_voxels[m_voxelsActive++].setup(
				voxel.position,
				options.voxelHalfSize,
				voxel.color);
		}

		return true;
	}

	size_t LogicOOP::cullVoxels(
		const Options& options,
		const glm::vec3& targetPosition)
	{
		size_t culled = 0;

		for (size_t i = 0; i < m_voxelsActive; ++i)
		{
			Voxel& voxelCurrent = m_voxels[i];
			Ray& rayCurrent = m_rays[i];

			rayCurrent.setup(
				voxelCurrent,
				targetPosition,
				glm::vec3(1.0f) / glm::normalize(voxelCurrent.getPosition() - targetPosition));

			for (size_t j = 0; j < m_voxelsActive; ++j)
			{
				rayCurrent.intersects(m_voxels[j]);
			}

			voxelCurrent.setCulled(rayCurrent.getClosest() != &voxelCurrent);

			if (voxelCurrent.isCulled())
			{
				culled++;
			}
		}

		return culled;
	}

	size_t LogicOOP::render(
		const Options& options,
		Renderer::Vertex* target)
	{
		Renderer::Vertex* dst = target;

		if (!options.culling)
		{
			for (size_t i = 0; i < m_voxelsActive; ++i)
			{
				dst->position = m_voxels[i].getPosition();
				dst->color = m_voxels[i].getColor();

				dst++;
			}

			return m_voxelsActive;
		}
		else if (
			options.showCulled)
		{
			static const glm::vec3 ColorCulled(1.0f, 0.0f, 0.0f);
			static const glm::vec3 ColorVisible(0.0f, 1.0f, 0.0f);

			for (size_t i = 0; i < m_voxelsActive; ++i)
			{
				dst->position = m_voxels[i].getPosition();
				dst->color =
					m_voxels[i].isCulled()
						? ColorCulled
						: ColorVisible;

				dst++;
			}

			return m_voxelsActive;
		}
		else
		{
			size_t used = 0;

			for (size_t i = 0; i < m_voxelsActive; ++i)
			{
				if (!m_voxels[i].isCulled())
				{
					dst->position = m_voxels[i].getPosition();
					dst->color = m_voxels[i].getColor();

					dst++;
					used++;
				}
			}

			return used;
		}
	}

}; // namespace Tmpl