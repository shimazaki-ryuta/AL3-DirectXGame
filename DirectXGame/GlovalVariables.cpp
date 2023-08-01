#include "GlovalVariables.h"
#include "ImGuiManager.h"

GlovalVariables* GlovalVariables::GetInstance() {
	static GlovalVariables instance;
	return &instance;
}

void GlovalVariables::CreateGroup(const std::string& groupName) { 
	datas_[groupName]; 
}

void GlovalVariables::SetValue(
	const std::string& groupName,
	const std::string& key,int32_t value)
{
	Group& group = datas_[groupName];

	//新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	group.items[key] = newItem;
}
void GlovalVariables::SetValue(
    const std::string& groupName, const std::string& key, float value) {
	Group& group = datas_[groupName];

	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	group.items[key] = newItem;
}
void GlovalVariables::SetValue(
    const std::string& groupName, const std::string& key,const Vector3& value) {
	Group& group = datas_[groupName];

	// 新しい項目のデータを設定
	Item newItem{};
	newItem.value = value;
	group.items[key] = newItem;
}

void GlovalVariables::Update(){
	if (!ImGui::Begin("Groval Variables",nullptr,ImGuiWindowFlags_MenuBar)) 
	{
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar()) return;

	for (std::map<std::string,Group>::iterator itGroup = datas_.begin();
		itGroup != datas_.end();++itGroup)
	{
		const std::string& groupName = itGroup->first;

		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str())) continue;
		for (std::map<std::string, Item>::iterator itItem = group.items.begin();
			itItem != group.items.end();++itItem)
		{
			const std::string& itemName = itItem->first;
			Item& item = itItem->second;

			if (std::holds_alternative<int32_t>(item.value)){
				int32_t* ptr = std::get_if<int32_t>(&item.value);
				ImGui::SliderInt(itemName.c_str(),ptr,0,100);
			} else if (std::holds_alternative<float>(item.value)){
				float* ptr = std::get_if<float>(&item.value);
				ImGui::SliderFloat(itemName.c_str(), ptr, -10.0f, 10.0f);
			} else if (std::holds_alternative<Vector3>(item.value)) {
				Vector3* ptr = std::get_if<Vector3>(&item.value);
				ImGui::SliderFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), -10.0f,10.0f);
			}
		}


		ImGui::EndMenu();
	}


	ImGui::EndMenuBar();
	ImGui::End();
}
