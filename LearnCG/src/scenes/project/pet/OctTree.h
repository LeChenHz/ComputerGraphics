#pragma once

#include <vector>
#include <map>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class OctTree 
{
	std::map<int, OctTree*> childs; // 子节点
	int acc = 0; // 当前节点的浓度
	glm::vec3 size; // 大小
	glm::vec3 center; // 中心
	int depth = 0; // 深度（根节点深度是8）
	int max = -1; // 叶子节点的浓度最大值
	glm::vec3 regionVector(int region) const;

public:
	OctTree(glm::vec3 size, glm::vec3 center, int maxDepth);
	~OctTree();
	int getRegion(glm::vec3 point) const; // 判断某个点所在的区域
	void addPoint(glm::vec3 point); // 添加点
	glm::vec3 getCenter() const;
	glm::vec3 getSize() const;
	int getMax() const;
	int getAcc() const;
	// 获取叶子节点，如果小于filter_max就过滤掉
	void getLeaves(std::vector<const OctTree*> &result, int filter_max = 0) const;
};
