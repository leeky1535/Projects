package service;

import java.util.ArrayList;

import com.ssafy.dao.HouseDao;
import com.ssafy.daoImpl.HouseDaoImpl;
import com.ssafy.dto.HouseDto;

public class HouseServiceImpl implements HouseService {
	private static HouseService houseService = new HouseServiceImpl();
	private HouseDao houseDao;

	public HouseService getHouseService() {
		return houseService;
	}

	public HouseServiceImpl() {
		houseDao = HouseDaoImpl.getHouseDao();
	}

	@Override
	public HouseDto getHouseInfo() {
		// TODO Auto-generated method stub
		return null;
	}

	@Override
	public ArrayList<HouseDto> getHouseList(String sido, String gugun, String dong, String name) throws Exception {
		return houseDao.searchByName(sido, gugun, dong, name);
	}

	@Override
	public String[] getDongLatLng(String sido, String gugun, String dong) throws Exception {
		return houseDao.getDongLatLng(sido, gugun, dong);
	}
}
