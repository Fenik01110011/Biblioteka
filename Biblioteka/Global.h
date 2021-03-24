#pragma once
//Singleton
class Global
{
private:
	Global() {};
	Global(const Global&);
	Global& operator=(const Global&) {};
public:
	static Global& getInstance()
	{
		static Global instance;
		return instance;
	}

	static char pause();
};