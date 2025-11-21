#pragma once

class Fader {
public:
	enum class FADE_MODE {
		NONE,
		FADE_OUT,
		FADE_IN,
	};

	enum class PROC {
		NONE,
		DELAY,
		FADE,
		WAIT,
	};

	Fader();
	~Fader();

	void SetFadeMode(FADE_MODE mode, unsigned int time, unsigned int delay = 0U, unsigned int wait = 0U);
	void Update();
	void Draw();

	FADE_MODE GetFadeMode() const;
	void ForceSetMode(FADE_MODE);
	PROC GetNowProc() const;
	bool IsFadeEnd() const;

private:
	static constexpr float MAX_ALPHA = 255.0F;

	FADE_MODE mode_;
	PROC proc_;
	unsigned int time_;
	unsigned int delay_;
	unsigned int wait_;

	unsigned int delayTimer_;
	unsigned int waitTimer_;
	bool isFadeEnd_;

	unsigned int color_;
	float alpha_;
};

