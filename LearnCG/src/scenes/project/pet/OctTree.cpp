#include "OctTree.h"

using namespace glm;

OctTree::~OctTree() 
{
	for (auto iter = childs.begin(); iter != childs.end(); iter++) 
	{
		delete iter->second;
	}
}

int OctTree::getRegion(glm::vec3 point) const 
{
	vec3 local = point - center;
	if (local.x >= 0 && local.y >= 0 && local.z >= 0) {
		return 0;
	}
	else if (local.x >= 0 && local.y >= 0 && local.z < 0) {
		return 1;
	}
	else if (local.x < 0 && local.y >= 0 && local.z < 0) {
		return 2;
	}
	else if (local.x < 0 && local.y >= 0 && local.z >= 0) {
		return 3;
	}
	else if (local.x >= 0 && local.y < 0 && local.z >= 0) {
		return 4;
	}
	else if (local.x >= 0 && local.y < 0 && local.z < 0) {
		return 5;
	}
	else if (local.x < 0 && local.y < 0 && local.z < 0) {
		return 6;
	}
	else if (local.x < 0 && local.y < 0 && local.z >= 0) {
		return 7;
	}
}

void OctTree::addPoint(glm::vec3 point) 
{
	acc += 1;
	int region = getRegion(point);
	auto iter = childs.find(region);
	if (iter != childs.end()) 
	{
		iter->second->addPoint(point);
	}
	else
	{	
		//新建结点
		vec3 half_size = size / 2.0f;
		vec3 quad_size = half_size / 2.0f;
		vec3 offset = quad_size * regionVector(region);
		vec3 cen = center + offset;
		if (depth > 0)
		{
			auto ch = new OctTree(half_size, cen, depth - 1);
			ch->addPoint(point);
			childs.insert(std::pair<int, OctTree*>(region, ch));
		}
	}
	if (depth == 0) //叶子节点
	{
		max = acc;
	}
	else 
	{
		for (auto iter : childs) 
		{
			if (iter.second->getMax() > max) 
				max = iter.second->getMax();
		}
	}
}

OctTree::OctTree(glm::vec3 size, glm::vec3 center, int maxDepth) 
	: size(size), center(center), depth(maxDepth) {
	acc += 0;
}

glm::vec3 OctTree::regionVector(int region) const {
	vec3 res;
	switch (region) {
	case 0:
		res = vec3(1.0f, 1.0f, 1.0f);
		break;
	case 1:
		res = vec3(1.0f, 1.0f, -1.0f);
		break;
	case 2:
		res = vec3(-1.0f, 1.0f, -1.0f);
		break;
	case 3:
		res = vec3(-1.0f, 1.0f, 1.0f);
		break;
	case 4:
		res = vec3(1.0f, -1.0f, 1.0f);
		break;
	case 5:
		res = vec3(1.0f, -1.0f, -1.0f);
		break;
	case 6:
		res = vec3(-1.0f, -1.0f, -1.0f);
		break;
	case 7:
		res = vec3(-1.0f, -1.0f, 1.0f);
		break;
	default:
		res = vec3(0.0f);
		break;
	}
	return res;
}

glm::vec3 OctTree::getCenter() const {
	return center;
}

glm::vec3 OctTree::getSize() const {
	return size;
}

void OctTree::getLeaves(std::vector<const OctTree*> &result, int filter_max) const {
	if (acc < filter_max) 
		return;
	if (depth == 0) { //如果是叶子节点
		result.push_back(this);
	}
	else {
		for (auto iter : childs) {
			iter.second->getLeaves(result, filter_max);
		}
	}
}

int OctTree::getAcc() const {
	return acc;
}

int OctTree::getMax() const {
	return max;
}