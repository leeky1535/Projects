package com.ssafy.dao;

import java.util.ArrayList;

import com.ssafy.dto.HouseDealDto;

public interface HouseDealDao {
	void register(HouseDealDto houseDealDto) throws Exception;

	ArrayList<HouseDealDto> searchByDong(String sido, String gugun, String dong) throws Exception;

	ArrayList<HouseDealDto> searchByName(String dongCode, String name) throws Exception;

//	List<HouseDealDto> searchAll();
//
//	void updateDealAmount(int houseNo, String houseDealAmount);
//
//	void deleteHouse(int houseNo);
}
