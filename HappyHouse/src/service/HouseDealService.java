package service;

import java.util.ArrayList;

import com.ssafy.dto.HouseDealDto;
import com.ssafy.dto.HouseDto;

public interface HouseDealService {
	HouseDto getHouseDealInfo();

	ArrayList<HouseDealDto> getHouseDeals(String dongCode, String name) throws Exception;
}
