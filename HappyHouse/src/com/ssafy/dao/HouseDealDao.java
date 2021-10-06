package com.ssafy.dao;

import java.util.List;

import com.ssafy.dto.HouseDealDto;

public interface HouseDealDao {
	void register(HouseDealDto houseDealDto);

	HouseDealDto searchById(int houseNo);

	List<HouseDealDto> searchByDong(String houseDong);

	List<HouseDealDto> searchAll();

	void updateDealAmount(int houseNo, String houseDealAmount);

	void deleteHouse(int houseNo);
}
