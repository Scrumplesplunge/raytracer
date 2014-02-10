#include "Test.h"
#include <sstream>
#include <ctime>

const std::string Test::norm = Test::color(7);
const std::string Test::bad = Test::color(1);
const std::string Test::good = Test::color(2);
const std::string Test::note = Test::color(3);

int Test::operator()() {
	std::cout << norm << "Performing " << note << this->name() << norm << "..\n" << std::endl;
	int out = this->perform();
	std::cout << "\n" << note << this->name() << norm << " complete: ";
	std::cout << (out == 0 ? good : bad) << out << " errors" << norm << ".\n" << std::endl;
}

std::string Test::color(unsigned int foreground) {
	std::stringstream buf;
	buf << char(0x1B) << '[' << (30 + foreground) << 'm';
	return buf.str();
}

std::string Test::color(unsigned int foreground, unsigned int background) {
	std::stringstream buf;
	buf << char(0x1B) << '[' << (30 + foreground) << ';' << (40 + background) << 'm';
	return buf.str();
}

float Test::getTime() {
	return float(clock()) / CLOCKS_PER_SEC;
}

bool Test::equal(float a, float b) {
	return a - 1e-5 < b && b < a + 1e-5;
}

