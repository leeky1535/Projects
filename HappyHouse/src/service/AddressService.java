package service;

import java.util.ArrayList;

public interface AddressService {
	public ArrayList<String> getSidoList() throws Exception;

	public ArrayList<String> getGugunList(String sido) throws Exception;

	public ArrayList<String> getDongList(String sido, String gugun) throws Exception;
}
