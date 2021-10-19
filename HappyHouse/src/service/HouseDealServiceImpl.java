package service;

import java.util.ArrayList;

import com.ssafy.dao.HouseDealDao;
import com.ssafy.daoImpl.HouseDealDaoImpl;
import com.ssafy.dto.HouseDealDto;
import com.ssafy.dto.HouseDto;

public class HouseDealServiceImpl implements HouseDealService {
	private static HouseDealService houseDealService = new HouseDealServiceImpl();
	private HouseDealDao houseDealDao;

	public HouseDealService getHouseDealService() {
		return houseDealService;
	}

	public HouseDealServiceImpl() {
		houseDealDao = HouseDealDaoImpl.getHouseDealDao();
	}

	@Override
	public HouseDto getHouseDealInfo() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public ArrayList<HouseDealDto> getHouseDeals(String dongCode, String name) throws Exception {
		return houseDealDao.searchByName(dongCode, name);
	}
}
