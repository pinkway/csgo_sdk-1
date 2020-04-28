#pragma once

class i_weapon_system {
	virtual void				unused0() = 0;
	virtual void				unused1() = 0;
public:
	virtual c_cs_weapon_data*	get_cs_weapon_data(unsigned int item_definition_index) = 0;
};