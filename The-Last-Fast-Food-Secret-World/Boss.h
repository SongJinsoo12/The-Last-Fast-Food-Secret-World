#pragma once
#include "Status.h"

class Boss : public Status
{
public:
	Boss();
	~Boss();

private:

};

class Boss_Easy : public Boss
{
public:
	Boss_Easy();
	~Boss_Easy();

private:

};

class Boss_Mid : public Boss
{
public:
	Boss_Mid();
	~Boss_Mid();

private:

};


class Boss_Hard : public Boss
{
public:
	Boss_Hard();
	~Boss_Hard();

private:

};

