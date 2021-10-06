package com.ssafy.dao;

import java.util.List;

import com.ssafy.dto.HouseDto;

public interface HouseDao {
	void register(HouseDto houseDto);

	HouseDto searchById(int houseNo);

	List<HouseDto> searchByDong(String houseDong);

	List<HouseDto> searchAll();

//	void updateHouse(String houseNo, int productPrice);

	void deleteHouse(String houseNo);
}
