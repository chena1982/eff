#pragma once

struct PossibleValue;
typedef std::vector<PossibleValue*> VectorPossibleValue;

struct PossibleValue
{
	std::string name;
	std::vector<std::string> values;
};

