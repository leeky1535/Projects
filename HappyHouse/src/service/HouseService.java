package service;

import java.util.ArrayList;

import com.ssafy.dto.HouseDto;

public interface HouseService {
	HouseDto getHouseInfo() throws Exception;

	ArrayList<HouseDto> getHouseList(String sido, String gugun, String dong, String name) throws Exception;

	String[] getDongLatLng(String sido, String gugun, String dong) throws Exception;
}
