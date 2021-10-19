package com.ssafy.dao;

import java.util.ArrayList;
import java.util.List;

import com.ssafy.dto.HouseDto;

public interface HouseDao {
	void register(HouseDto houseDto);

	HouseDto searchById(int houseNo);

	List<HouseDto> searchByDong(String houseDong);

	List<HouseDto> searchAll();

//	void updateHouse(String houseNo, int productPrice);

	void deleteHouse(String houseNo);

	ArrayList<HouseDto> searchByName(String sido, String gugun, String dong, String name) throws Exception;

	public String[] getDongLatLng(String sido, String gugun, String dong) throws Exception;
}
