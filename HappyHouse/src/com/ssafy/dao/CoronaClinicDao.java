package com.ssafy.dao;

import java.util.List;

import com.ssafy.dto.CoronaClinicDto;

public interface CoronaClinicDao {
	List<CoronaClinicDto> searchByGugun(String gugun);

	List<CoronaClinicDto> searchAll();
}
