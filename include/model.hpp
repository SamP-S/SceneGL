#pragma once

#include <filesystem>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include "resource.hpp"
#include "mesh.hpp"
#include "la_extended.h"
using namespace LA;
#include "transform.hpp"

class Model : public Resource {

private:

	int x = 0;

public:

	Model(std::string name)
		: Resource(name) {
	}

};

ResourceManager<Model> resourceModels = ResourceManager<Model>();