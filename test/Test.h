#pragma once
#include <iostream>
#include <string>

class Test {
	public:
		static const std::string norm, bad, good, note;
		int operator()();
		virtual int perform() = 0;
		virtual std::string name() = 0;

		static std::string color(unsigned int foreground);
		static std::string color(unsigned int foreground, unsigned int background);

		static float getTime();
		static bool equal(float a, float b);
		static void test(std::string name);
		static void perf(float time, unsigned int reps);
};

