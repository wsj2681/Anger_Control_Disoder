#pragma once
class Input final
{
public:

	Input() = default;
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;
	~Input() = default;

private:

	bool m_keys[256];

public:

	void Init();
	void Release();
	void KeyDown(const unsigned int& input);
	void KeyUp(const unsigned int& input);
	bool IsKeyDown(const unsigned int& input);
	
};
