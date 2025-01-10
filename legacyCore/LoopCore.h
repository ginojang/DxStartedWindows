#pragma once


class LoopCore
{
public:
	LoopCore();
	~LoopCore();

	void Start(uint32 *pBuffer, int width, int height);
	void FixedUpdate(float time);
	void Update();
	void Close();

};


extern LoopCore loopCore;