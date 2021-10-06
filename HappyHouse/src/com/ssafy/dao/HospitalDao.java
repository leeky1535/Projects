package com.ssafy.dao;

import java.util.List;

import com.ssafy.dto.HospitalDto;

public interface HospitalDao {
	List<HospitalDto> searchByGugun(String gugun);

	List<HospitalDto> searchAll();
}
