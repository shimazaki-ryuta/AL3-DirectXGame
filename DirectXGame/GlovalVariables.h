#pragma once
#include <variant>
#include <map>
#include <stdint.h>
#include <string>
#include "Vector3.h"
class GlovalVariables {
public:
	struct Item {
		//項目
		std::variant<int32_t,float,Vector3> value;

	};
	struct Group {
		std::map<std::string, Item> items;
	};


	static GlovalVariables* GetInstance();
	void CreateGroup(const std::string& groupName);

	void SetValue(const std::string& groupName, const std::string& key, int32_t value);
	void SetValue(const std::string& groupName, const std::string& key, float value);
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	void Update();

private:
	GlovalVariables() = default;
	~GlovalVariables() = default;
	GlovalVariables(const GlovalVariables&) = delete;
	GlovalVariables operator=(const GlovalVariables&) = delete;

	std::map<std::string, Group> datas_;
};
