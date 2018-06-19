#ifndef TIMELINE_H
#define TIMELINE_H

#include <vector>
#include <algorithm>

/**
 * \brief Timeline emulates sequence of images
 */
class TimeLine
{
public:
	TimeLine(const std::vector<const char*>& source, const bool loop, const int fps)
	{
		Init(source, loop, fps);
	}

	void Init(const std::vector<const char*>& source, const bool loop, const int fps)
	{
		m_frames = source;
		m_totalFrames = m_frames.size();
		m_currentFrame = 0;
		m_loop = loop;
		m_fps = fps;
	}

	void Update(const float dt)
	{
		static int elapsed = 0;
		const float gameFps = 1 / dt;
		if (++elapsed >= gameFps / m_fps) {
			const int nextFrame = m_currentFrame + 1;
			if (m_loop && nextFrame >= m_totalFrames) 
				m_currentFrame = 0;
			else
				m_currentFrame = std::min(nextFrame, m_totalFrames - 1);
			elapsed = 0;
		}
	}

	const char* GetImage()
	{
		return m_frames[m_currentFrame];
	}

private:
	bool m_loop{};
	int m_fps{};
	int m_currentFrame{};
	int m_totalFrames{};
	std::vector<const char*> m_frames;
};

#endif