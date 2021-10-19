package service;

import java.util.ArrayList;

import com.ssafy.dao.AddressDao;
import com.ssafy.daoImpl.AddressDaoImpl;

public class AddressServiceImpl implements AddressService {
	private static AddressService addressService = new AddressServiceImpl();
	private AddressDao addressDao;

	public static AddressService getAddressService() {
		return addressService;
	}

	public AddressServiceImpl() {
		addressDao = AddressDaoImpl.getAddressDao();
	}

	@Override
	public ArrayList<String> getSidoList() throws Exception {
		return addressDao.getSidoList();
	}

	@Override
	public ArrayList<String> getGugunList(String sido) throws Exception {
		return addressDao.getGugunList(sido);
	}

	@Override
	public ArrayList<String> getDongList(String sido, String gugun) throws Exception {
		return addressDao.getDongList(sido, gugun);
	}

}
