package com.ssafy.dao;

import java.util.ArrayList;

public interface AddressDao {
	public ArrayList<String> getSidoList() throws Exception;

	public ArrayList<String> getGugunList(String sido) throws Exception;

	public ArrayList<String> getDongList(String sido, String gugun) throws Exception;
}
